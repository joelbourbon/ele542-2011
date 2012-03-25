////////////////////////////////////////////////////////
//File: uart.c
//
//Source file for USART COM
//
//Handles USART communication
//
//Author: Vincent Léger, Jason Cormier
//
////////////////////////////////////////////////////////
#include "uart.h"



/*****************/
/***Interrupts****/
/*****************/
///////////////////////////////////////////
//Interrupt name: ISR(USART_TXC_vect)
//
//Definition: USART TX READY INTERRUPT
//
//Brief: Set uartTxReady flag for transmit methods
//
///////////////////////////////////////////
ISR(USART_TXC_vect, ISR_NAKED) 	//TX ready     
// signal handler for uart txd ready interrupt
{
	OSIntEnter();
UDR = debugStream[debugStreamPos];
	debugStreamPos++;
	//uartTxReady=0;
	if (strlen((char*)debugStream) <= debugStreamPos)
	{
		//disable Tx interrupt
		UCSRB = UCSRB_REGISTER_SELECTIONS;
		echo_on_flag = 1;
	}

	OSIntExit();
}
///////////////////////////////////////////
//Interrupt name: ISR(USART_RXC_vect)
//
//Definition: USART RX COMPLE INTERRUPT
//
//Brief: Takes new data from UDR, stores data
//       from controler and send ECHO back if
//		 not in debug mode.
//
///////////////////////////////////////////
ISR(USART_RXC_vect, ISR_NAKED)     //RX ready 
// signal handler for receive complete interrupt
{
	OSIntEnter();
	data = UDR;        // read byte for UART data buffer
	
	processData();
	if ((echo_on_flag == 1))
	{
		//while(!uartTxReady){}
		UDR = data;
	}

	//uartReadyFlag = 1;
}

/***************************/
/***Function definitions****/
/***************************/
///////////////////////////////////////////
//Function name: uart_init
//
//Brief:   init uart for normal operations
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void uart_init(void)
{
	
	//set uart set-up registers
	UCSRC = UCSRC_REGISTER_SELECTIONS;
	UCSRB = UCSRB_REGISTER_SELECTIONS;
    /* set baud rate */
    UBRRH = ((uint8_t)(UART_BAUD_SELECT >> 8));
    UBRRL = ((uint8_t)(UART_BAUD_SELECT & 0x00FF));
	//set receiver state machine to waiting
	ReceiverState = STATE_WAITING;    
	
	//init all flags
	echo_on_flag = 1;
	uartTxReady = 1;
	uartReadyFlag = 0;
	motorCommandCompletedFlag = 0;
	actualCommand.speed = 0;
	actualCommand.angle = 0;
	stopFlag = 0;
	uartTxReady = 0;
	strcpy((char*)debugStream, "");
	debugStreamPos;
}
///////////////////////////////////////////
//Function name: processData
//
//Brief:   Receiver's state machine.
//		   -->Stores commands from controler
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void processData(void)
{

	switch (ReceiverState)
	{
		case (STATE_WAITING):	//waiting for new command
			
			if (data == 0xF1) //signal for new incomming command
			{
				ReceiverState = STATE_CMD_RECEIVED;
			}
			else if (stopFlag == 2)	//if flag as been received
			{
				stopFlag = 0;
			}
			else if (data == 0xF0)	//stop command
			{
				ReceiverState = STATE_WAITING;	//come back to waiting
				stopFlag = 1;	
			}
			
			break;
		case (STATE_CMD_RECEIVED):	//command as been received, retrieve speed
		{
			requestedCommand.speed = data;
			ReceiverState = STATE_SPEED_RECEIVED;
			break;
		}
		case (STATE_SPEED_RECEIVED):	//speed as been received, retrieve angle
		{
			requestedCommand.angle = data;
			ReceiverState = STATE_ANGLE_RECEIVED;
			motorCommandCompletedFlag = 1;
			uartLedFlag = 1;
			break;
		}
		case (STATE_ANGLE_RECEIVED):	//angle is received, wait for main to reset state machine
			break;
		default:
			break;
	}

}

///////////////////////////////////////////
//Function name: uart_send
//
//Brief:   send data to controler
//			-->controler needs special begin+end tokens
//
//Inputs: -char* bug --> string to be sent
//		  -unsigned char size --> the size of the data to be sent
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void uart_send(char *buf, unsigned char size)
{  
	echo_on_flag = 0;				//Debug mode, stop echo on incomming data
	transmitByte(0xFE);				//Start debug string signal
	for (unsigned char i = 0; i<size; i++)	//Send all bytes
	{
		transmitByte(buf[i]);
	}
	transmitByte(0xFF);			//End of debug string signal

	echo_on_flag = 1;	//Remove if bug persist
}

///////////////////////////////////////////
//Function name: transmitByte
//
//Brief:   transmit one byte of data
//
//Inputs: -unsigned char dataToSend-->byte of data to be sent
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void transmitByte(unsigned char dataToSend)
{
	//while(!uartTxReady){}	//wait transmitter is ready	
	//uartTxReady = 0;		//reset tx ready flag
	//UDR = dataToSend;		//send data


	if (uartTxReady)
	{
		UDR = debugStream[debugStreamPos];
		debugStreamPos++;
		//uartTxReady=0;
		if (strlen((char*)debugStream) == debugStreamPos)
		{
			//disable Tx interrupt
			UCSRB = UCSRB_REGISTER_SELECTIONS;
			echo_on_flag = 1;
		}
	}
}

///////////////////////////////////////////
//Function name: uartReceive
//
//Brief:   compute new command from controler
//			if full command is received
//			can also understand stop commands
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void uartReceive(void)
{
	//wait for new command from controler
	if (motorCommandCompletedFlag == 1)	//if new command fully received
	{		
		wdt_reset();	//reset watchdog	//reset watchdog --> Robot still in control
		
		if (actualCommand.speed != requestedCommand.speed)	//if speed is different
		{													//compute new speed
			actualCommand.speed = requestedCommand.speed;
			computedCommand.speed = (((float)(actualCommand.speed) - 100.0) / 100.0); 
		}

		if (actualCommand.angle != requestedCommand.angle)	//same with angle
		{
			actualCommand.angle = requestedCommand.angle;
			computedCommand.angle = ((actualCommand.angle)*ANGLE_RATIO);
		}			

		motorCommandCompletedFlag = 0;
	}
	else if (stopFlag)
	{
		wdt_reset(); //reset watchdog --> Robot still listenning
		
		actualCommand.speed = 100;	//Emergency stop
		actualCommand.angle = 0;	
		requestedCommand.speed = 100;
		//requestedCommand.angle = 0;	
		computedCommand.speed = 0;
		//computedCommand.angle = 0;
		stopFlag = 2;				//signal for receiver state machine
									//-->stop command complet, can input new commands
	}
}

///////////////////////////////////////////
//Function name: sendDebugValue
//
//Brief:   send debug string + value
//
//		************************************************
//		****Depracated function, causes robot to lag****
//		************************************************
//
//Inputs: -char* dataToSend --> string to be sent with value
//        -unsigned char valueToSend --> debug value to send
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
/*void sendDebugValue(char* dataToSend, unsigned char valueToSend)
{
	unsigned char stringLenght;

	stringLenght = strlen(dataToSend);
	stringLenght--;
	if (stringLenght >= MAX_STRING_TO_SEND)
	{
		stringLenght = MAX_STRING_TO_SEND;
	}
	uart_send(dataToSend, stringLenght);

	itoa(valueToSend, (char*)string_valueToSend, 10);

	stringLenght = strlen((char*)string_valueToSend);
	if (stringLenght >= MAX_VALUE_TO_SEND)
	{
		stringLenght = MAX_VALUE_TO_SEND;
	}
	uart_send((char*)string_valueToSend, strlen((char*)string_valueToSend));
}
*/
//NEW AND IMPROVED
void sendDebugValue(char* dataToSend, unsigned char valueToSend)
{

	if (echo_on_flag)
	{
		UCSRB = UCSRB_REGISTER_SELECTIONS_DEBUG;	//enable TX interrupts
		echo_on_flag = 0;
	
		strncpy((char*)debugStream, dataToSend, strlen((char*)debugStream));
		//missing debug value
		//missing end stream;
		
		
		debugStream[strlen((char*)debugStream)] = valueToSend;
		debugStream[strlen((char*)debugStream)] = 0xFF;
		//strcat((char*)debugStream, debugValueToSend);
		//strcat((char*)debugStream, 0xFE);
		//sprintf("%s %i%c",dataToSend, valueToSend, 0xFF);

	
		UDR = 0xFE;
		debugStreamPos = 0;
		//uartTxReady
	}




}


