#ifndef _SONNARS_H_
#define _SONNARS_H_


#include <avr/io.h>
#include <avr/interrupt.h>
//#include "timer.h"
#include "LedsControl.h"

#define MAX_SONNAR_SET_GAIN			20
#define MAX_SONNAR_SET_DISTANCE		20

#define INIT_SONNAR_GAIN	10		//recommended: 8 up to 12
#define INIT_SONNAR_DST		46		//recommended: 1.5 up to 3m
									//(lenght reg. + 1) * 4,3 cm
									//2m => (2m/(0.043m))-1 = 45.5
									//(46+1)*0.043 = 2.021
#define MAX_DELAY_PING	10	//timer speed is 200Hz... 1/200Hz * 10 = 50ms
#define PING_CMD_CM		0x51

#define PING_ZEROVALUE_LSB	0xFF
#define PING_ZEROVALUE_MSB	0x0F

//Design notes 

//==>(typical TWI transmition)
//
//-START on TWCR (initiate transmission)
//(HARDWARE)TWINT set, status code indicates START sent
//-check TWSR to see if start was sent
//-load SLA+W in TWDR
//-load appropriate control signals in TWCR (while making sure TWINT = 1 and TWSTA = 0)
//(HARDWARE)TWINT set, status code indicates SLA+W sent and ACK received
//-check TWSR to see if SLA+W was sent and ACK received
//-load data in TWDR
//-load appropriate control signals in TWCR (while making sure TWINT = 1)
//(HARDWARE)TWINT set, status code indicates data sent and ACK received
//-check TWSR to see if data was sent and ACK received
//-load appropriate control signals in TWCR to send STOP (while making sure TWINT = 1)


//==>(TWI frequency)
//
// SCL frequency = ((CPU CLK)/(16+2*(TWBR)*4^(TWPS))
//
//CPU CLK = 16 000 000 Hz
//Desired speed = 10 000 Hz
//
// 16 000 000 / 10 000 = 1600
//(16+2*(TWBR)*4^(TWPS)) = 1600
//TWPS -->1
//TWBR -->198
//(16+2*(198)*4^(1))

#define TWPS_PRESCALER_VALUE	1	//to TWSR
#define TWBR_REGISTER_VALUE		/*(unsigned char)*/198
//TWBR = (F_CPU / 10000UL - 16) / (2 * 4); //note: 4^TWPS_PRESCALER_VALUE = 4

#define TWI_ENABLE	(1<<TWINT)|(1<<TWEN)|(1<<TWIE)//|(1<<TWEA)	//Set TWI for next transmission (interrupt mode)
#define TWI_ACK		(1<<TWEA)
#define TWI_START	TWI_ENABLE|(1<<TWSTA)			//Send start/repeated start condition
#define TWI_STOP	(1<<TWSTO)|(1<<TWINT)|(1<<TWEN)
#define TWI_DISABLE	(0<<TWEN)

#define TWI_ACKNOWLEDGE	(1<<TWEA)
#define TWI_CLEAR_INTERRUPT	(1<<TWINT)	//write this to TWCR at the end of WTI interrupt

//TWI TWSR register values
#define	TWI_STATE_IDLE		0x00	//initial state
#define TW_START   			0x08	//start condition received
#define TW_REP_START   		0x10	//repeated start condition received
#define TW_SLA_ACK   		0x18	//SLA+W + ACK
#define TW_SLA_NACK   		0x20	//SLA+W + NACK
#define TW_DATA_ACK   		0x28	//data transmitted + ACK
#define TW_DATA_NACK		0x30	//data transmitted + NACK
#define TW_ARB_LOST			0x38	//arbitration lost
#define TW_SLA_READ_ACK		0x40	//SLA+R + ACK
#define TW_SLA_READ_NACK	0x48	//SLA+R + ACK
#define TW_SLAVE_DATA_ACK	0x50	//data received + ACK
#define TW_SLAVE_DATA_NACK	0x58	//data received + NACK
#define TW_BUS_ERROR		0x00	//BUSS ERROR
//to get status register without prescaler value, use TW_STATUS
#define TW_STATUS_MASK	0xF8
//#define TW_STATUS   	(TWSR & TW_STATUS_MASK)
#define SONNAR_READ_FLAG	0x01

//Sonnar addresss
#define LEFT_SONNAR_ADR		0xE2
#define RIGHT_SONNAR_ADR	0xE0
#define LEFT_SONNAR_READ	0xE3
#define LEFT_SONNAR_WRITE	0xE2
#define RIGHT_SONNAR_READ	0xE1
#define RIGHT_SONNAR_WRITE	0xE0
//Sonnar registers (write)
#define SONNAR_CMD_REGISTER		0x00
#define SONNAR_GAIN_REGISTER	0x01
#define SONNAR_DST_REGISTER		0x02
//Sonnar registers (read)
#define SONNAR_INFO_REGISTER		0x00
#define SONNAR_MSB_VALUE_REGISTER	0x02
#define SONNAR_LSB_VALUE_REGISTER	0x03

#define STATE_STOP		0x00
#define STATE_PINGING	0x01
#define STATE_SET_DST	0x02
#define STATE_INIT		0x03



#define MAX_TWI_STREAM	20
#define MAX_SONNAR_GAIN	16


#define TWSR_VALUE	(TWSR & 0xF8)	//to mask prescaler bits


void configureTWI(void);
void processTWI(void);
void TWIcheckState(void);

void setTWIstream(void);

volatile unsigned char TWIstream[MAX_TWI_STREAM];
volatile unsigned char TWIstreamLenght;
volatile unsigned char TWIdataPosition;

volatile unsigned char specialFlag;

volatile unsigned char TWIstate;

volatile unsigned char weirdFlag;

volatile unsigned char objectDetectedRight;
volatile unsigned char objectDetectedLeft;

extern volatile unsigned char rightPingFlag;
extern volatile unsigned char lefttPingFlag;
extern volatile unsigned char rightObjectLed;
extern volatile unsigned char leftObjectLed;

volatile unsigned char TWIreadyFlag;
volatile unsigned char twiStateMachine;
volatile unsigned char stateRxTx;
volatile unsigned char TWIstopRequest;

volatile unsigned char TWIgainValue;
volatile unsigned char TWIdistanceValue;
volatile unsigned char TWIslaveAddress;

volatile unsigned char pingCounter;

volatile struct TWI_VALUES
{
	volatile unsigned char changeDistanceFlag;
	volatile unsigned char distance;
	volatile unsigned char slaveAdd;
	volatile unsigned char slaveReg;
	volatile unsigned char valueToSend;
	volatile unsigned char valueFlag;
	volatile unsigned char pingFlag;
	volatile unsigned char pingingFlag;
	volatile unsigned char initFlag;
	volatile unsigned char highValueFlag;
	volatile unsigned char leftFlag;
	volatile unsigned char sendCompleteFlag;
	int valueLeft;
	int valueRight;
	volatile unsigned char valueRightLSB;
	volatile unsigned char valueRightMSB;
	volatile unsigned char valueLeftLSB;
	volatile unsigned char valueLeftMSB;
}SonarData;



//TWI interrupt vector
ISR(TWI_vect)
{	
/*	if (SonarData.sendCompleteFlag)	
		{TWIreadyFlag = 0;}
	else
		{TWIreadyFlag = 1;}*/

	
	TWIcheckState();

	//PORTB = 0x01;
	//TWIcheckState();
}

//TODO: ADD LEDS FOR PINGING...

void configureTWI(void)
{
	//memset(&TWIstream, 0, sizeof(TWIstream));

	memset((void*) &SonarData, 0, sizeof(SonarData));

	TWIreadyFlag = 0;
	TWIstate = TWI_STATE_IDLE;
	twiStateMachine = 0;

	
	
	TWSR = 2;
	TWBR = 198;


	//real shit
	//TWBR = TWBR_REGISTER_VALUE;
	//TWSR = TWPS_PRESCALER_VALUE;
	
	
	//TWBR = TWBR_REGISTER_VALUE;

	SonarData.initFlag = 1;
	SonarData.sendCompleteFlag = 1;
	pingCounter = 0;

}

void resetTWI(void)
{
	memset((void*) &SonarData, 0, sizeof(SonarData));

	TWIreadyFlag = 0;
	TWIstate = TWI_STATE_IDLE;
	twiStateMachine = 0;

	//set initial values
	SonarData.initFlag = 1;
	SonarData.sendCompleteFlag = 1;
	objectDetectedRight = 0;
	objectDetectedLeft = 0;
	pingCounter = 0;
	weirdFlag = 0;
	specialFlag = 0;

}

void processTWI(void)
{	

	/*if (specialFlag)
	{
		PORTB = ~(SonarData.sendCompleteFlag);
		while(1){}
	}*/
	if (SonarData.sendCompleteFlag)
	{
		SonarData.sendCompleteFlag = 0;
		setTWIstream();
		//SonarData.sendCompleteFlag = 0;
	}
	/*else if (TWIreadyFlag)
	{
		//TWIstate = TW_STATUS;	//get new TW status register value
		TWIcheckState();
		TWIreadyFlag = 0;
	}*/
}								//	set reg low, pinging
									
void TWIcheckState(void)
{
	TWIstate = (TWSR & 0xF8);
	switch (TWIstate)
	{
		case (TW_START):
		{	
			//PORTB = 0x21;
			TWDR = SonarData.slaveAdd;
			TWCR = TWI_ENABLE;
			//TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);
			break;
		}
		case (TW_REP_START):	
		{
			//PORTB = 0x01;
			if (SonarData.pingingFlag)
			{
				SonarData.pingingFlag = 0;
				//SonarData.slaveAdd |= SONNAR_READ_FLAG;	//sends read adress of sonnar currently being ping
				if (weirdFlag == 2)
				{
					SonarData.slaveAdd = LEFT_SONNAR_READ;
				}
				else
				{
					SonarData.slaveAdd = RIGHT_SONNAR_READ;
				}
			}
			TWDR = SonarData.slaveAdd;
			TWCR = TWI_ENABLE;
			break;
		}
		case (TW_SLA_ACK):
		{	
			//PORTB = 0x04;

			TWDR = SonarData.slaveReg;
			TWCR = TWI_ENABLE | TWI_ACK;
			break;
		}
		case (TW_SLA_NACK):
		{

			//PORTB = 0x05;
			//we could also only send STOP...

			/*TWDR = SonarData.slaveAdd;
			TWCR = TWI_ENABLE;*/
			//PORTB = 0x08;
			//while(1){}
			SonarData.sendCompleteFlag = 1;
			//TWCR = TWI_STOP;
			TWCR = TWI_ENABLE;
			break;
		}
		case (TW_DATA_ACK):
		{
			//PORTB = 0x03;
			
			if (SonarData.valueFlag)	//if we got additional value
			{
				TWDR = SonarData.valueToSend;
				SonarData.valueFlag = 0;	//only one data after register value.. leave it hardcoded for now
				TWCR = TWI_ENABLE;

			}
			else if (SonarData.pingFlag)	//have to change this... this prevents from sending ping command...
			{
				
				SonarData.pingingFlag = 1;
				TWCR = TWI_START;	//send repeated start condition
			}
			else
			{
				SonarData.sendCompleteFlag = 1;
				TWCR = TWI_STOP;	//after all data is sent
			}
			
			break;
		}
		case (TW_DATA_NACK):
		{
			//PORTB = 0x05;
			SonarData.sendCompleteFlag = 1;
			//TWCR = TWI_STOP;
			break;
		}
		case (TW_ARB_LOST):
		{
			//PORTB = 0x04;
			//while(1){}
			
			SonarData.sendCompleteFlag = 1;
			TWCR = TWI_STOP;	//give up
			break;
		}
		case (TW_SLA_READ_ACK):
		{
			//PORTB = 0x08;
			TWCR = TWI_ENABLE | TWI_ACK;
			break;
		}
		case (TW_SLA_READ_NACK):
		{
			//PORTB = 0x11;
			
			/*SonarData.sendCompleteFlag = 1;
			TWCR = TWI_STOP;*/
			//PORTB = 0x58;
			//while(1){}

		/*	PORTB = 0x09;
			while(1){}

			TWDR = SonarData.slaveAdd;
			TWCR = TWI_ENABLE;*/

			//TWCR = TWI_ENABLE | TWI_ACK;
			SonarData.sendCompleteFlag = 1;
			break;
		}
		case (TW_SLAVE_DATA_ACK):
		{
			//PORTB = 0x02;
			//if (SonarData.highValueFlag)
			if ((twiStateMachine == 2)&&(SonarData.pingFlag))
			{	
				if (weirdFlag == 2)
				{
					//SonarData.valueLeft = ((TWDR)<<8);
					SonarData.valueLeftMSB = TWDR;
					
				}
				else if (weirdFlag == 1)
				{
					//SonarData.valueRight = ((TWDR)<<8);
					SonarData.valueRightMSB = TWDR;
				}

			}



			//TODO: Think about using the same flag (for motor and leds)
			else if ((twiStateMachine == 3)&&(SonarData.pingFlag))
			{
				if (weirdFlag == 2)
				{
					//SonarData.valueLeft += (int)(TWDR);
					SonarData.valueLeftLSB = TWDR;

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
				/*	if (SonarData.valueLeft > 100)
					{
						objectDetectedLeft = 1;
						leftObjectLed = 1;

					}
					else
					{
						objectDetectedLeft = 0;
						leftObjectLed = 0;
					}
					SonarData.valueLeft = 0;*/
				}
				else if (weirdFlag == 1)
				{
					//SonarData.valueRight += (int)(TWDR);
					SonarData.valueRightLSB = TWDR;
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

				/*	if (SonarData.valueRight > 100)
					{
						objectDetectedRight = 1;
						rightObjectLed = 1;
					}
					else
					{
						objectDetectedRight = 0;
						rightObjectLed = 0;
					}
					SonarData.valueRight = 0;*/
				}
			}
			//SonarData.sendCompleteFlag = 1;


			//SonarData.sendCompleteFlag = 1;
			TWCR = TWI_ENABLE;
			//TWCR = TWI_STOP;
			break;
		}
		case (TW_SLAVE_DATA_NACK):
		{
			SonarData.sendCompleteFlag = 1;
			TWCR = TWI_STOP;
			break;
		}
		case (TW_BUS_ERROR):
		{
			//PORTB = 0x08;
			while(1){}
			//should not happen... send stop ?
			SonarData.sendCompleteFlag = 1;
			TWCR = TWI_STOP;
			break;
		}
		default:
		{
			//PORTB = 0x31;
			//while(1){}
			//SonarData.sendCompleteFlag = 1;
			//TWCR = TWI_ENABLE;
			SonarData.sendCompleteFlag = 1;
			break;
		}
	}
	TWIreadyFlag = 0;
}

void setTWIstream(void)
{

	if (SonarData.initFlag == 1)
	{
		switch (twiStateMachine)
		{
			case (0):
			{
				
				//PORTB = 0xF0;
				SonarData.slaveAdd = LEFT_SONNAR_WRITE;
				SonarData.slaveReg = SONNAR_DST_REGISTER;
				SonarData.valueFlag = 1;
				SonarData.valueToSend = INIT_SONNAR_DST;
				TWIreadyFlag = 0;
				twiStateMachine++;
				TWCR = TWI_START;
				break;
			}
			case (1):
			{
				//PORTB = 0x0F;
				SonarData.slaveAdd = RIGHT_SONNAR_WRITE;
				SonarData.slaveReg = SONNAR_DST_REGISTER;
				SonarData.valueFlag = 1;
				SonarData.valueToSend = INIT_SONNAR_DST;
				TWIreadyFlag = 0;
				TWCR = TWI_START;
				twiStateMachine++;
				break;
			}
			case (2):
			{
				//PORTB = 0x10;
				SonarData.slaveAdd = LEFT_SONNAR_WRITE;
				SonarData.slaveReg = SONNAR_GAIN_REGISTER;
				SonarData.valueFlag = 1;
				SonarData.valueToSend = INIT_SONNAR_GAIN;
				TWIreadyFlag = 0;
				TWCR = TWI_START;
				twiStateMachine++;
				break;
			}
			case (3):
			{
				//PORTB = 0x00;
				SonarData.slaveAdd = RIGHT_SONNAR_WRITE;
				SonarData.slaveReg = SONNAR_GAIN_REGISTER;
				SonarData.valueFlag = 1;
				SonarData.valueToSend = INIT_SONNAR_GAIN;
				TWIreadyFlag = 0;
				TWCR = TWI_START;
				twiStateMachine++;
				break;
			}

			default:
			{
				//PORTB = 0x30;
				twiStateMachine=0;
				SonarData.valueFlag = 0;
				SonarData.initFlag = 0;
				SonarData.pingFlag = 1;	//start pinging
				//that might also be the fuck... this can cause data send to fail...
				//try using a second flag for now
				SonarData.slaveAdd = LEFT_SONNAR_WRITE;	//useless
				pingCounter = 0;
				TWIreadyFlag = 0;
				SonarData.sendCompleteFlag = 1;
				weirdFlag = 1;
				break;
				//TODO: add something to go back to other state...
				//
			}
		}

	}/*	FUTUR GROTH PURPOSE: SONAR DISTANCE SETTINGS MUST CHANGE ALONG SPEED
	else if (SonarData.changeDistanceFlag)
	{

	}*/
	if (SonarData.pingFlag)	//pingFlag must be last possible case
	{
		//PORTB = 0x22;
		//while(1){}
		if (pingCounter >= MAX_DELAY_PING)
		{
			//PORTB = 0x66;
			
			//specialFlag ++;
			//switch something... and twiStateMachine = 0;
			/*if ((SonarData.slaveAdd & 0xFE) == LEFT_SONNAR_ADR)
			{
				SonarData.slaveAdd = LEFT_SONNAR_WRITE;
				PORTB = 0x11;
				
			}
			else
			{
				SonarData.slaveAdd = RIGHT_SONNAR_WRITE;
				PORTB = 0x22;

			}*/	

			

			if (weirdFlag == 2)
			{
				weirdFlag = 1;
				leftPingFlag = 1;
			}
			else
			{
				weirdFlag = 2;
				rightPingFlag= 1;
			}
			//SonarData.slaveAdd &= 0xFE; //TODO: that's the way to switch WRITE/READ... not the weirdFlag thing

		
			twiStateMachine = 0;
			pingCounter = 0;
			SonarData.sendCompleteFlag = 1;
			//specialFlag++;
		}		
		//PORTB = 0x99;
		//while(1){}
		switch (twiStateMachine)
		{		
			case (0):	//send ping LEFT
			{
				
				//PORTB = 0x45;
				//SonarData.slaveAdd = (SonarData.slaveAdd & ~SONNAR_READ_FLAG);	//remove write flag
				if (weirdFlag == 2)
				{
					SonarData.slaveAdd = RIGHT_SONNAR_WRITE;
				}
				else
				{
					SonarData.slaveAdd = LEFT_SONNAR_WRITE;
				}
				SonarData.slaveReg = SONNAR_CMD_REGISTER;
				SonarData.valueFlag = 1;
				SonarData.valueToSend = PING_CMD_CM;
				TWIreadyFlag = 0;
				twiStateMachine++;
			//	SonarData.highValueFlag = 1;
				TWCR = TWI_START;
				SonarData.pingFlag = 1;
				break;
			}
			case (1):	//ask value high right
			{
				//PORTB = 0x54;
				/*if ((SonarData.slaveAdd & 0xFE) == LEFT_SONNAR_WRITE)
				{
					SonarData.slaveAdd = RIGHT_SONNAR_WRITE;
				}
				else
				{
					SonarData.slaveAdd = LEFT_SONNAR_WRITE;
				}*/

				if (weirdFlag == 2)
				{
					SonarData.slaveAdd = LEFT_SONNAR_WRITE;
				}
				else
				{
					SonarData.slaveAdd = RIGHT_SONNAR_WRITE;
				}

				//SonarData.slaveAdd = SonarData.slaveAdd 
				SonarData.slaveReg = SONNAR_MSB_VALUE_REGISTER;
				SonarData.highValueFlag = 1;
				SonarData.valueFlag = 0;
				TWIreadyFlag = 0;
				TWCR = TWI_START;
				twiStateMachine++;
				SonarData.pingFlag = 1;
				break;
			}
			case (2):	//ask value low right
			{
				//PORTB=0x77;
				SonarData.slaveAdd = (SonarData.slaveAdd & ~SONNAR_READ_FLAG);	//remove write flag
				SonarData.slaveReg = SONNAR_LSB_VALUE_REGISTER;
				SonarData.highValueFlag = 0;
				SonarData.valueFlag = 0;
				TWIreadyFlag = 0;
				TWCR = TWI_START;
				twiStateMachine++;
				SonarData.pingFlag = 1;
				break;
			}
			default:
			{
				//pingCounter = 0;
				//SonarData.slaveAdd;
				//PORTB=0x64;
				twiStateMachine = 3;
				//TWCR = TWI_STOP;
				SonarData.sendCompleteFlag = 1;
				TWIreadyFlag = 0;
				//weirdFlag = 0;
				
				//PORTB = 0x88;
				SonarData.pingFlag = 1;
				//while(1){}
				break;
			}
		}

	}
	


}


#endif //_SONNARS_H_


