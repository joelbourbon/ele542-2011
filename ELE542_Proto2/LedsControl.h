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

#ifndef _LEDSCONTROL_H_
#define _LEDSCONTROL_H_

/**************/
/***INCLUDES***/
/**************/
#include <avr/io.h>
#include <avr/interrupt.h>

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

/***************************/
/***Function definitions****/
/***************************/
///////////////////////////////////////////
//Function name: initLeds
//
//Brief:   init leds at start of program
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void initLeds(void)
{
	leftPingFlag  = 0;
	rightPingFlag = 0;
	uartLedFlag   = 0;
	ledDeadState();
	ledAliveState();
}

///////////////////////////////////////////
//Function name: ledDeadState
//
//Brief:   sets leds when robot is dead (LED6 on only).
//			Also resets all led values.
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void ledDeadState(void)
{
	ledState.leds = (1<<LED6);
	PORTB = ~(ledState.leds);
	ledState.uartCounter = 0;
	ledState.leftPingCounter = 0;
	ledState.rightPingCounter = 0;
	rightPingFlag = 0;
	leftPingFlag = 0;
	uartLedFlag = 0;
}

///////////////////////////////////////////
//Function name: obstacleDetectedLeds
//
//Brief:   lights LED4 and LED2 if objects are detected
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void obstacleDetectedLeds(void)
{
	if (leftObjectLed)
	{
		ledState.leds |= (1<<LED4);
	}
	else
	{
		ledState.leds &= ~(1<<LED4);
	}
	if (rightObjectLed)
	{
		ledState.leds |= (1<<LED2);
	}
	else
	{
		ledState.leds &= ~(1<<LED2);
	}
}

///////////////////////////////////////////
//Function name: ledAliveState
//
//Brief:   sets leds when robot is alive.
//			Also resets all led values.
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void ledAliveState(void)
{
	ledState.leds = (1<<LED7);
	PORTB = ~(ledState.leds);
	ledState.uartCounter = 0;
	ledState.leftPingCounter = 0;
	ledState.rightPingCounter = 0;
	rightPingFlag = 0;
	leftPingFlag = 0;
	uartLedFlag = 0;
	leftObjectLed = 0;
	rightObjectLed = 0;
}

///////////////////////////////////////////
//Function name: pingUart
//
//Brief:   flash led when USART is in COM with controler
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void pingUart(void)
{
	if ((uartLedFlag) && !(ledState.flashing & (1<<LED1)))//new led input received
	{
		ledState.flashing |= (1<<LED1);
	}
	if (ledState.uartCounter)	//if counter is running
	{
		ledState.uartCounter++;
		if (ledState.uartCounter >= LED_UART_CYCLE_TIME)
		{
			if (ledState.leds & (1<<LED1))
			{
				ledState.leds &= (0xFF & ~(1<<LED1));	//turn led off
				ledState.uartCounter = 1;	//calls a second loop to keep led off
			}
			else
			{
				ledState.uartCounter = 0; //flash is finished
				uartLedFlag = 0;
				ledState.flashing &= ~(1<<LED1);
			}
		}
	}			
	else if (ledState.flashing & (1<<LED1))	//starts led counter
	{		
		ledState.leds |= (1<<LED1);
		ledState.uartCounter = 1;
	}
}

///////////////////////////////////////////
//Function name: pingSonarLeft
//
//Brief:   Flash led 5 if left ping is sent
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void pingSonarLeft(void)
{
	if ((leftPingFlag) && !(ledState.flashing & (1<<LED5)))//new led input received
	{
		ledState.flashing |= (1<<LED5);
	}
	if (ledState.leftPingCounter)
	{
		ledState.leftPingCounter++;
		if (ledState.leftPingCounter >= LED_PING_CYCLE_TIME)
		{
			if (ledState.leds & (1<<LED5))
			{
				ledState.leds &= (0xFF & ~(1<<LED5));	//turn led off
				ledState.leftPingCounter = 1;	//calls a second loop to keep led off
			}
			else
			{
				ledState.leftPingCounter = 0; //flash is finished
				leftPingFlag = 0;
				ledState.flashing &= ~(1<<LED5);
			}
		}
	}			
	else if (ledState.flashing & (1<<LED5))//starts led counter
	{		
		ledState.leds |= (1<<LED5);
		ledState.leftPingCounter = 1;
	}
}

///////////////////////////////////////////
//Function name: pingSonarRight
//
//Brief:   Flash led 5 if right ping is sent
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void pingSonarRight(void)
{
	if ((rightPingFlag) && !(ledState.flashing & (1<<LED3)))//new led input received
	{
		ledState.flashing |= (1<<LED3);
	}
	if (ledState.rightPingCounter)
	{
		ledState.rightPingCounter++;
		if (ledState.rightPingCounter >= LED_PING_CYCLE_TIME)
		{
			if (ledState.leds & (1<<LED3))
			{
				ledState.leds &= (0xFF & ~(1<<LED3));	//turn led off
				ledState.rightPingCounter = 1;	//calls a second loop to keep led off
			}
			else
			{
				ledState.rightPingCounter = 0;	//flash is finished
				rightPingFlag = 0;
				ledState.flashing &= ~(1<<LED3);
			}
		}
	}			
	else if (ledState.flashing & (1<<LED3))//starts led counter
	{		
		ledState.leds |= (1<<LED3);
		ledState.rightPingCounter = 1;
	}
}

///////////////////////////////////////////
//Function name: flashLeds
//
//Brief:   Main led function. To be called by main loop.
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void flashLeds(void)
{
	pingUart();
	pingSonarLeft();
	pingSonarRight();
	obstacleDetectedLeds();
	ledState.leds |= (1<<LED7);
	PORTB = ~(ledState.leds);
}


#endif //_LEDSCONTROL_H_

