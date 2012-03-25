////////////////////////////////////////////////////////
//File: sonar.h
//
//Header file for sonar control
//
//This file contains methods to communicate with sonars
//using a TWI connection.
//
//Author: Vincent Léger, Jason Cormier
//
////////////////////////////////////////////////////////
/**************/
/***INCLUDES***/
/**************/
#include "ucos_core/includes.h"
#include "LedsControl.h"

#ifndef _SONARS_H_
#define _SONARS_H_




/**************/
/***DEFINES****/
/**************/
#define TWPS_PRESCALER_VALUE	1	//Desired speed = 10 000 Hz
#define TWBR_REGISTER_VALUE		198	// 16 000 000 / 10 000 = 1600
									//(16+2*(TWBR)*4^(TWPS)) = 1600

//TWCR values to send
#define TWI_ENABLE	(1<<TWINT)|(1<<TWEN)|(1<<TWIE)	//Set TWI for next transmission (interrupt mode)
#define TWI_ACK		(1<<TWEA)						//Acknoledge bit
#define TWI_START	TWI_ENABLE|(1<<TWSTA)			//Send start/repeated start condition
#define TWI_STOP	(1<<TWSTO)|(1<<TWINT)|(1<<TWEN)	//Kill pending transmission
#define TWI_DISABLE	(0<<TWEN)						//Kill TWI

#define TWI_ACKNOWLEDGE	(1<<TWEA)
#define TWI_CLEAR_INTERRUPT	(1<<TWINT)	//write this to TWCR at the end of WTI interrupt

//TWI TWSR values
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
#define SONAR_READ_FLAG	0x01

//SONAR Read/Write address
#define LEFT_SONAR_READ		0xE3
#define LEFT_SONAR_WRITE	0xE2
#define RIGHT_SONAR_READ	0xE1
#define RIGHT_SONAR_WRITE	0xE0	
//SONAR registers (write)
#define SONAR_CMD_REGISTER		0x00	//send command
#define SONAR_GAIN_REGISTER		0x01	//set gain
#define SONAR_RANGE_REGISTER		0x02	//set range
//SONAR registers (read)
#define SONAR_INFO_REGISTER			0x00	//unused
#define SONAR_MSB_VALUE_REGISTER	0x02	//get MSB ping value
#define SONAR_LSB_VALUE_REGISTER	0x03	//get LSB ping value

#define PING_RIGHT	1
#define PING_LEFT	2

#define INIT_SONAR_GAIN		10		//recommended: 8 up to 12
#define INIT_SONAR_RANGE	46		//recommended: 1.5 up to 3m
									//(lenght reg. + 1) * 4,3 cm -->(46+1)*0.043 = 2.021

#define MAX_DELAY_PING	10	//timer speed is 200Hz... 1/200Hz * 10 = 50ms
#define PING_CMD_CM		0x51//Command in CM

#define PING_ZEROVALUE_LSB	0xFF	//values when no object detected
#define PING_ZEROVALUE_MSB	0x0F

#define TWSR_MASK	(0xF8)	//mask for TW status register

//TWI state machine
#define STATE_1	0
#define STATE_2	1
#define STATE_3	2
#define STATE_4	3

/**************************/
/***Function prototypes****/
/**************************/
void configureTWI(void);	//init TWI
void processTWI(void);		//To put in main loop for TWI processing
void TWIcheckState(void);	//set all
void setTWIstream(void);	//Set next TWI transmission


/**************/
/***GLOBALS****/
/**************/
volatile unsigned char TWIstate;			//TWI TWSR value
volatile unsigned char objectDetectedRight;	//To stop motors
volatile unsigned char objectDetectedLeft;	
volatile unsigned char twiStateMachine;		//STATE MACHINE
volatile unsigned char pingCounter;			//Counts timer overflows for ping delay

//Contains all TWI data

volatile struct TWI_VALUES	//TODO: Some of these are flags, change chars for bits
{
	volatile unsigned char range;			//hold range value
	volatile unsigned char slaveAdd;		//slave adress to send
	volatile unsigned char slaveReg;		//targeted slave register
	volatile unsigned char valueToSend;		//additionnal data
	volatile unsigned char valueFlag;		//flag for additionnal data
	volatile unsigned char pingFlag;		//pinging mode
	volatile unsigned char pingingFlag;		//ping been sent, getting value from sonars
	volatile unsigned char initFlag;		//sonars registers needs to be changed
	volatile unsigned char pingSide;		//to toggle pings on both sonars 
											//--> 0=no ping, 1=left ping, 2=right ping
	volatile unsigned char sendCompleteFlag;//Flag for state machine--> TWI command complete
	volatile unsigned char valueRightLSB;	//right sonar LSB value
	volatile unsigned char valueRightMSB;	//right sonar MSB value
	volatile unsigned char valueLeftLSB;	//left sonar LSB value
	volatile unsigned char valueLeftMSB;	//left sonar MSB value
}SonarData;
	

/***********************/
/***External GLOBALS****/
/***********************/
//extern volatile unsigned char rightPingFlag;	//right ping sent, flash led
//extern volatile unsigned char lefttPingFlag;	//left ping sent, flash led
//extern volatile unsigned char rightObjectLed;	//object detected, toggle led
//extern volatile unsigned char leftObjectLed;




#endif //_SONARS_H_

