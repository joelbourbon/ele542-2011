////////////////////////////////////////////////////////
//File: uart.h
//
//Header file for USART COM
//
//Handles USART communication
//
//Author: Vincent Léger, Jason Cormier
//
////////////////////////////////////////////////////////
#ifndef _UART_H_
#define _UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdint.h>
#include "string.h"
#include "motorControl.h"
#include "LedsControl.h"



//UART DEFINITIONS
//
// Baud rate = 9600
// Operation mode = asynch
// Parity = none
// Nb of stop bits = 1
// Nb of data bits = 8
// Tx detection = rising edge
// Rx interrupt = enabled
// Tx interrupt = disabled
#define UART_BAUD_RATE      9600      /* 9600 baud */
//To automate bit selection for desired baud rate in UBRR register 
#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)
/* configure asynchronous operation, no parity, 1 stop bit, 8 data bits, Tx on rising edge */
#define UCSRC_REGISTER_SELECTIONS	((1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0)|(0<<UCPOL))
/* enable RxD/TxD and Rx int */
#define UCSRB_REGISTER_SELECTIONS	((1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN)|(0<<UCSZ2))


//Receiver state machine
#define STATE_WAITING			0
#define STATE_CMD_RECEIVED		1
#define STATE_SPEED_RECEIVED	2
#define STATE_ANGLE_RECEIVED	3

#define MAX_VALUE_TO_SEND		4	//used to send byte 255=max value --> 3 char + carriage return
#define MAX_STRING_TO_SEND		50	//to prevent pointer overflow

volatile unsigned char stopFlag;	//stop command flag

//globals
volatile char echo_on_flag = 1;	//Echo back received bytes until debug string is sent
volatile char uartReadyFlag;
volatile unsigned char dataToCompute;
volatile char ReceiverState = STATE_WAITING;	//receiver state machine --> starts at waiting state
volatile unsigned char data;					//byte to be sent

//volatile unsigned char uartRxCompleteFlag;		//New data received
volatile unsigned char uartTxReady;				//Ready to send data
//volatile char uartTxReady = 1;
volatile char string_valueToSend[MAX_VALUE_TO_SEND];

//Extern Globals
extern volatile struct MotorCommands requestedCommand;
extern volatile struct MotorCommands actualCommand;
extern volatile unsigned char uartLedFlag;


//Functions prototype
void uart_init(void);	//init uart operations
//void sendDebugValue(char* dataToSend, unsigned char valueToSend);	//send debug data (depracated)
void transmitByte(unsigned char dataToSend);	//tranmit to controler
void uart_send(char *buf, unsigned char size);	//send 1 byte
void processData(void);	//process received data
void uartReceive(void);

//volatile unsigned char SUPERFLAG = 0;

unsigned char motorCommandCompletedFlag;



//Uart functions definitions

/*****************/
/***Interrupts****/
/*****************/
ISR(USART_TXC_vect) 	//TX ready     
// signal handler for uart txd ready interrupt
{
	uartTxReady = 1;
}

ISR(USART_RXC_vect)     //RX ready 
// signal handler for receive complete interrupt
{
	data = UDR;        // read byte for UART data buffer
	
	processData();
	if ((echo_on_flag == 1))
	{
		while(!uartTxReady){}
		UDR = data;
	}

	uartReadyFlag = 1;
}

//init uart for normal operations
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
	uartTxReady = 1;
	uartReadyFlag = 0;
	motorCommandCompletedFlag = 0;
	actualCommand.speed = 0;
	actualCommand.angle = 0;
	stopFlag = 0;
}

//toggles receiver's state machine for received data
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


//send data to controler (also sends special token)
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

//transmit one byte of data
void transmitByte(unsigned char dataToSend)
{
	while(!uartTxReady){}	//wait transmitter is ready	
	uartTxReady = 0;	
	UDR = dataToSend;		//send data
}

//receive uart data from controler
void uartReceive(void)
{
	//wait for new command from controler
	if (motorCommandCompletedFlag == 1)
	{		
		wdt_reset();	//reset watchdog
		
		if (actualCommand.speed != requestedCommand.speed)
		{
			actualCommand.speed = requestedCommand.speed;
			computedCommand.speed = (((float)(actualCommand.speed) - 100.0) / 100.0);
		}

		if (actualCommand.angle != requestedCommand.angle)
		{	
			actualCommand.angle = requestedCommand.angle;
			computedCommand.angle = ((actualCommand.angle)*ANGLE_RATIO);
		}			

		motorCommandCompletedFlag = 0;
	}
	else if (stopFlag)
	{
		wdt_reset(); //--> there must be a better way
		
		actualCommand.speed = 100;
		actualCommand.angle = 0;
		requestedCommand.speed = 100;
		//requestedCommand.angle = 0;
		computedCommand.speed = 0;
		//computedCommand.angle = 0;
		stopFlag = 2;
	}
}


//Depracated function, causes robot to lag
//send debug string + value
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


#endif //_UART_H_

