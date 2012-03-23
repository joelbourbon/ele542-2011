////////////////////////////////////////////////////////
//File: sonar.c
//
//Source file for sonar control
//
//This file contains methods to communicate with sonars
//using a TWI connection.
//
//Author: Vincent Léger, Jason Cormier
//
////////////////////////////////////////////////////////
#include "sonars.h"

/*****************/
/***Interrupts****/
/*****************/
///////////////////////////////////////////
//Interrupt name: ISR(TWI_vect)
//
//Definition: TWI STATUS UPDATED FLAG
//
//Brief: Calls TWIcheckState which reads TWSR
//		 status to process TWI communications.
//
///////////////////////////////////////////
ISR(TWI_vect, ISR_NAKED)	//TWI interrupt vector
{		
	OSIntEnter();
	TWIcheckState();	//TWI Rx or Tx
	OSIntEnter();
}


/***************************/
/***Function definitions****/
/***************************/
///////////////////////////////////////////
//Function name: configureTWI
//
//Brief:   sets TWI for communication
//			with sonars
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void configureTWI(void)
{
	//init all data
	//memset((void*) &SonarData, 0, sizeof(SonarData));	//init all values	
	SonarData.initFlag = 1;
	SonarData.sendCompleteFlag = 1;
	pingCounter = 0;
	twiStateMachine = 0;
	
	//set TWI frequency
	TWSR = TWPS_PRESCALER_VALUE;
	TWBR = TWBR_REGISTER_VALUE;
	
}

///////////////////////////////////////////
//Function name: resetTWI
//
//Brief:   starts TWI communication
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void resetTWI(void)
{
	//memset((void*) &SonarData, 0, sizeof(SonarData));

	twiStateMachine = 0;

	//set initial values
	SonarData.initFlag = 1;
	SonarData.sendCompleteFlag = 1;
	objectDetectedRight = 0;
	objectDetectedLeft = 0;
	pingCounter = 0;
	SonarData.pingSide = 0;
	SonarData.range = INIT_SONAR_RANGE;
}

///////////////////////////////////////////
//Function name: processTWI
//
//Brief:   Verifies if TWI is ready for 
//			next state
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void processTWI(void)
{	
	if (SonarData.sendCompleteFlag)
	{
		SonarData.sendCompleteFlag = 0;
		setTWIstream();
	}
}	

///////////////////////////////////////////
//Function name: TWIcheckState
//
//Brief:   handles TWI COM (Rx or Tx)
//			Checks TWSR updated status and process
//			TWI communication accordingly.
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////									
void TWIcheckState(void)
{
	TWIstate = (TWSR & TWSR_MASK);	//get newest TW status value
	switch (TWIstate)
	{
		case (TW_START):	//start send, write slave address
		{	
			TWDR = SonarData.slaveAdd;
			TWCR = TWI_ENABLE;
			break;
		}
		case (TW_REP_START):	//repeated start...	
		{
			if (SonarData.pingingFlag)
			{
				SonarData.pingingFlag = 0;	//if pinging mode, send SLA+R
				if (SonarData.pingSide == 2)
				{
					SonarData.slaveAdd = LEFT_SONAR_READ;
				}
				else
				{
					SonarData.slaveAdd = RIGHT_SONAR_READ;
				}
			}
			TWDR = SonarData.slaveAdd;		//else, re-send actual address
			TWCR = TWI_ENABLE;
			break;
		}
		case (TW_SLA_ACK):	//slave write ack, send register to modify
		{	
			TWDR = SonarData.slaveReg;
			TWCR = TWI_ENABLE | TWI_ACK;
			break;
		}
		case (TW_SLA_NACK):	//slave write not ack, restart
		{
			SonarData.sendCompleteFlag = 1;
			TWCR = TWI_ENABLE;
			break;
		}
		case (TW_DATA_ACK):	//slave received data
		{		
			if (SonarData.valueFlag)	//if we got additional value to send
			{
				TWDR = SonarData.valueToSend;
				SonarData.valueFlag = 0;	//only one data after register value.. leave it hardcoded for now
				TWCR = TWI_ENABLE;

			}
			else if (SonarData.pingFlag) //if ping mode
			{
				
				SonarData.pingingFlag = 1;
				TWCR = TWI_START;	//send repeated start for read
			}
			else
			{
				SonarData.sendCompleteFlag = 1;
				TWCR = TWI_STOP;	//after all data is sent
			}
			
			break;
		}
		case (TW_DATA_NACK):	//data lost, break COM
		{
			SonarData.sendCompleteFlag = 1;
			break;
		}
		case (TW_ARB_LOST):		//this should never happen
		{
			SonarData.sendCompleteFlag = 1;
			TWCR = TWI_STOP;	//give up, release master powers
			break;
		}
		case (TW_SLA_READ_ACK):	//SLA+R ack
		{
			TWCR = TWI_ENABLE | TWI_ACK;	//send ack bit
			break;
		}
		case (TW_SLA_READ_NACK): //SLA+R no ack, break COM
		{
			SonarData.sendCompleteFlag = 1;
			break;
		}
		case (TW_SLAVE_DATA_ACK):	//data received from slave
		{
			//robustness purpose: makes sure it is in good state to receive data
			if ((twiStateMachine == STATE_3)&&(SonarData.pingFlag))
			{	
				if (SonarData.pingSide == PING_LEFT)
				{
					SonarData.valueLeftMSB = TWDR;
					
				}
				else if (SonarData.pingSide == PING_RIGHT)//robustness: reverify ping side
				{
					SonarData.valueRightMSB = TWDR;
				}

			}		
			//robustness purpose: makes sure it is in good state to receive data			
			else if ((twiStateMachine == STATE_4)&&(SonarData.pingFlag))
			{
				if (SonarData.pingSide == PING_LEFT)
				{
					SonarData.valueLeftLSB = TWDR;
					//These are actual values when no object in sight
					if ((SonarData.valueLeftLSB != PING_ZEROVALUE_LSB)&&(SonarData.valueLeftMSB != PING_ZEROVALUE_MSB))
					{
						objectDetectedLeft = 1;
						leftObjectLed = 1;
					}
					else
					{
						objectDetectedLeft = 0;
						leftObjectLed = 0;
					}
				}
				else if (SonarData.pingSide == PING_RIGHT)
				{
					SonarData.valueRightLSB = TWDR;
					//These are actual values when no object in sight
					if ((SonarData.valueRightLSB != PING_ZEROVALUE_LSB)&&(SonarData.valueRightMSB != PING_ZEROVALUE_MSB))
					{
						objectDetectedRight = 1;
						rightObjectLed = 1;
					}
					else
					{
						objectDetectedRight = 0;
						rightObjectLed = 0;
					}
				}
			}
			TWCR = TWI_ENABLE;	//send no-ack, data is received
			break;
		}
		case (TW_SLAVE_DATA_NACK):	//slave stops to send data
		{
			SonarData.sendCompleteFlag = 1;
			TWCR = TWI_STOP;	//stop COM
			break;
		}
		case (TW_BUS_ERROR):	//this should never happen
		{
			SonarData.sendCompleteFlag = 1;
			TWCR = TWI_STOP;	//give up, release master powers
			break;
		}
		default:
		{
			SonarData.sendCompleteFlag = 1;	//this should never happer
			break;							//NOP
		}
	}
}
///////////////////////////////////////////
//Function name: setTWIstream
//
//Brief:   triggers TWI state machine for 
//			next transmission
//
//	For each state: -set slaveAdd (target)
//					-set slaveReg (register in slave)
//					-set valueToSend and valueFlag if needed
//					-send start
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////	
void setTWIstream(void)
{

	if (SonarData.initFlag == 1)	//change register values
	{
		SonarData.valueFlag = 1;
		switch (twiStateMachine)
		{
			case (STATE_1):
			{

				SonarData.slaveAdd = LEFT_SONAR_WRITE;
				SonarData.slaveReg = SONAR_RANGE_REGISTER;
				SonarData.valueToSend = SonarData.range;
				twiStateMachine++;
				TWCR = TWI_START;
				break;
			}
			case (STATE_2):
			{
				SonarData.slaveAdd = RIGHT_SONAR_WRITE;
				SonarData.slaveReg = SONAR_RANGE_REGISTER;
				SonarData.valueToSend = SonarData.range;
				TWCR = TWI_START;
				twiStateMachine++;
				break;
			}
			case (STATE_3):
			{
				SonarData.slaveAdd = LEFT_SONAR_WRITE;
				SonarData.slaveReg = SONAR_GAIN_REGISTER;
				SonarData.valueToSend = INIT_SONAR_GAIN;
				TWCR = TWI_START;
				twiStateMachine++;
				break;
			}
			case (STATE_4):
			{
				SonarData.slaveAdd = RIGHT_SONAR_WRITE;
				SonarData.slaveReg = SONAR_GAIN_REGISTER;
				SonarData.valueToSend = INIT_SONAR_GAIN;
				TWCR = TWI_START;
				twiStateMachine++;
				break;
			}

			default:
			{
				twiStateMachine=0;			//start pinging
				SonarData.valueFlag = 0;	//robustness call
				SonarData.initFlag = 0;		//most of these values are already set
				SonarData.pingFlag = 1;					
				SonarData.slaveAdd = LEFT_SONAR_WRITE;
				pingCounter = 0;
				SonarData.sendCompleteFlag = 1;
				SonarData.pingSide = 1;
				SonarData.valueFlag = 0;
				break;
			}
		}

	}/*	FUTUR GROWTH: SONAR RANGE SETTINGS MUST CHANGE ALONG SPEED
	else if (SonarData.changeRangeFlag)
	{

	}*/
	if (SonarData.pingFlag)
	{
		//switch ping side
		if (pingCounter >= MAX_DELAY_PING)
		{

			if (SonarData.pingSide == PING_LEFT)
			{
				SonarData.pingSide = PING_RIGHT;
				leftPingFlag = 1;
			}
			else
			{
				SonarData.pingSide = PING_LEFT;
				rightPingFlag= PING_RIGHT;
			}

			//ready for next TWI COM
			twiStateMachine = 0;
			pingCounter = 0;	
			SonarData.sendCompleteFlag = 1;	

		}		
		//PINGING MODE
		switch (twiStateMachine)
		{		
			case (STATE_1):
			{		
				//send ping to other sonar
				if (SonarData.pingSide == PING_LEFT)
				{
					SonarData.slaveAdd = RIGHT_SONAR_WRITE;
				}
				else
				{
					SonarData.slaveAdd = LEFT_SONAR_WRITE;
				}
				SonarData.slaveReg = SONAR_CMD_REGISTER;
				SonarData.valueFlag = 1;
				SonarData.valueToSend = PING_CMD_CM;
				twiStateMachine++;
				TWCR = TWI_START;
				SonarData.pingFlag = 1;
				break;
			}
			case (STATE_2):
			{
				if (SonarData.pingSide == PING_LEFT)
				{
					SonarData.slaveAdd = LEFT_SONAR_WRITE;
				}
				else
				{
					SonarData.slaveAdd = RIGHT_SONAR_WRITE;
				}

				SonarData.slaveReg = SONAR_MSB_VALUE_REGISTER;
				SonarData.valueFlag = 0;
				TWCR = TWI_START;
				twiStateMachine++;
				SonarData.pingFlag = 1;
				break;
			}
			case (STATE_3):	//ask value low right
			{
				SonarData.slaveAdd = (SonarData.slaveAdd & ~SONAR_READ_FLAG);	//remove write flag
				SonarData.slaveReg = SONAR_LSB_VALUE_REGISTER;
				SonarData.valueFlag = 0;
				TWCR = TWI_START;
				twiStateMachine++;
				SonarData.pingFlag = 1;
				break;
			}
			default:
			{
				twiStateMachine = STATE_4;
				SonarData.sendCompleteFlag = 1;	
				SonarData.pingFlag = 1;
				//TODO: compute new range value from actual speed
				//speed command is 0-100.. forward = 50 to 100
				//range
				//Sonar.range = ???
				break;
			}
		}
	}
}
