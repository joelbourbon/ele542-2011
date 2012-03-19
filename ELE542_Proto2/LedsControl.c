////////////////////////////////////////////////////////
//File: LedsControl.c
//
//Source file for leds control
//
//This file flasheds and toggle leds to show robot's state
//
//Author: Vincent Léger, Jason Cormier
//
////////////////////////////////////////////////////////
#include "LedsControl.h"

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
