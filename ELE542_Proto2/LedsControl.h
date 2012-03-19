////////////////////////////////////////////////////////
//File: LedsControl.h
//
//Header file for leds control
//
//This file flasheds and toggle leds to show robot's state
//
//Author: Vincent Léger, Jason Cormier
//
////////////////////////////////////////////////////////
/**************/
/***INCLUDES***/
/**************/
#include <avr/io.h>
#include <avr/interrupt.h>


#ifndef _LEDSCONTROL_H_
#define _LEDSCONTROL_H_



/**************/
/***DEFINES****/
/**************/
//	LEDS BEHAVIOR DESCRIPTION SUMMARY
#define LED0 0	//LED0 --> NOP
#define LED1 1	//LED1 --> TELEGUIDANCE COMMAND RECEIVED (FLASHES)
#define LED2 2	//LED2 --> RIGHT SONNAR, OBSTACLED DETECTED
#define LED3 3	//LED3 --> RIGHT SONNAR, PINGING (FLASHES)
#define LED4 4	//LED4 --> LEFT SONNAR, OBSTACLED DETECTED
#define LED5 5	//LED5 --> LEFT SONNAR, PINGING (FLASHES)
#define LED6 6	//LED6 --> ROBOT ASLEEP
#define LED7 7	//LED7 --> ROBOT AWAKE

//	LED cycles duration (different for uart and pings)
#define LED_UART_CYCLE_TIME	8000
#define LED_PING_CYCLE_TIME	10000

/**************************/
/***Function prototypes****/
/**************************/
void flashLeds(void);				//toggle leds (to be called from main)
void obstacleDetectedLeds(void);	//toggle leds for object detection
void pingUart(void);				//flash uart led
void ledDeadState(void);			//Robot is dead: light up dead light
void ledAliveState(void);			//Robot comes back to life, reset led state for normal operations

/**************/
/***GLOBALS****/
/**************/
struct led_struct	//struct to hold leds current state
{
	unsigned char leds;				//to keep actual leds state --> 1 = led off, 0 = led on
	unsigned char flashing;			//to remember which leds are flashing
	unsigned int uartCounter;		//counter for uart flashing led
	unsigned int leftPingCounter;	//counter for left ping flashing led
	unsigned int rightPingCounter;	//counter for right ping flashing led
}ledState;

//Flags for led control
//TODO: no need to use char for flags... use bits instead
volatile unsigned char uartLedFlag;		//uart command received
volatile unsigned char rightObjectLed;	//object detected right sonar
volatile unsigned char leftObjectLed;	//object detected left sonar
volatile unsigned char leftPingFlag;				//pinging left sonar
volatile unsigned char rightPingFlag;			//pinging right sonar




#endif //_LEDSCONTROL_H_

