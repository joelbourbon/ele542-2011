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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "string.h"	//debug not used anymore
#include "motorControl.h"

#ifndef _UART_H_
#define _UART_H_





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
#define UART_BAUD_SELECT (16000000/(UART_BAUD_RATE*16l)-1)
/* configure asynchronous operation, no parity, 1 stop bit, 8 data bits, Tx on rising edge */
#define UCSRC_REGISTER_SELECTIONS	((1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0)|(0<<UCPOL))
/* enable RxD/TxD and Rx int */
#define UCSRB_REGISTER_SELECTIONS	((1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(0<<TXEN)|(0<<UCSZ2))
#define UCSRB_REGISTER_SELECTIONS_DEBUG	((1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN)|(0<<UCSZ2))


//Receiver state machine
#define STATE_WAITING			0
#define STATE_CMD_RECEIVED		1
#define STATE_SPEED_RECEIVED	2
#define STATE_ANGLE_RECEIVED	3

#define MAX_VALUE_TO_SEND		4	//used to send byte 255=max value --> 3 char + carriage return
#define MAX_STRING_TO_SEND		50	//to prevent pointer overflow

volatile unsigned char stopFlag;	//stop command flag

//globals
volatile char echo_on_flag;// = 1;	//Echo back received bytes until debug string is sent
volatile char uartReadyFlag;
volatile unsigned char dataToCompute;
volatile char ReceiverState;// = STATE_WAITING;	//receiver state machine --> starts at waiting state
volatile unsigned char data;					//byte to be sent

//volatile unsigned char uartRxCompleteFlag;		//New data received
volatile unsigned char uartTxReady;				//Ready to send data
//volatile char uartTxReady;// = 1;
volatile char string_valueToSend[MAX_VALUE_TO_SEND];

volatile unsigned char debugStream[100];
volatile unsigned char debugValueToSend[10];
volatile unsigned char debugStreamPos;
//Extern Globals
//extern volatile struct MotorCommands requestedCommand;
//extern volatile struct MotorCommands actualCommand;
//extern volatile unsigned char uartLedFlag;


//Functions prototype
void uart_init(void);	//init uart operations
//void sendDebugValue(char* dataToSend, unsigned char valueToSend);	//send debug data (depracated)
void transmitByte(unsigned char dataToSend);	//tranmit to controler
void uart_send(char *buf, unsigned char size);	//send 1 byte
void processData(void);	//store received data
void uartReceive(void);	//process received data

unsigned char motorCommandCompletedFlag;	//flag for complete controler command received





#endif //_UART_H_

