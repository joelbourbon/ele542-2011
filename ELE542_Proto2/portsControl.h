////////////////////////////////////////////////////////
//File: portsControl.h
//
//Header file for watchdog and switch inputs control
//
//This file contains methods set and reset watchdog,
//and to switch global robot's state with switch inputs
//
//Author: Vincent Léger, Jason Cormier
//
////////////////////////////////////////////////////////

#ifndef _PORTSCONTROL_H_
#define _PORTSCONTROL_H_

/**************/
/***INCLUDES***/
/**************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "sonars.h"
#include "LedsControl.h"

/**************/
/***DEFINES****/
/**************/
//robot's global states (dead or alive)
#define STATE_ASLEEP	0
#define STATE_ALIVE		1

//switch inputs
#define INPUT_DIE	(1<<7)
#define INPUT_LIVE	(1<<6)


/**************************/
/***Function prototypes****/
/**************************/
void initPorts(void);		//init all ports, should be called at first in main
void setWatchDog(void);		//set and tunes watchdog timer
void checkControls(void);	//verify switch inputs


/**************************/
/*****Extern functions*****/
/**************************/
extern void resetTWI(void);			//reset TWI COM
extern void ledAliveState(void);	//set leds for normal operations

/**************/
/***GLOBALS****/
/**************/
volatile unsigned char robotState;	//robot's global state (dead or alive)


/***************************/
/***Function definitions****/
/***************************/
///////////////////////////////////////////
//Function name: initPorts
//
//Brief:   init ports for all operations
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void initPorts(void)
{
	DDRB = 0xFF;            	//PortB output
	DDRD = 0xFE; 				//port D = all out but Rx bit
	DDRA = 0x10;				//All out but calib bit. Note that bit 5 is unused
	robotState = STATE_ASLEEP;	//start robot at DEAD state
}

///////////////////////////////////////////
//Function name: setWatchDog
//
//Brief:   init watchdog timer register
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void setWatchDog(void)
{
	//enable + presacaler value.... value is fine tuned for smooth operations
	WDTCR = (1<<WDTOE)|(1<<WDE)|(1<<WDP2)|(0<<WDP1)|(0<<WDP0);
}

///////////////////////////////////////////
//Function name: checkControls
//
//Brief:   check switch inputs and switch robot's 
//				MASTER state
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void checkControls(void)	//Check switch inputs 
{
	if (~PINA & INPUT_LIVE)
	{
		robotState = STATE_ALIVE;
		PORTB = 0x00;	//light leds to notice it received the input

		while(~PINA & INPUT_LIVE){}	// to make sure switch is released
		
		ledAliveState();	//set leds for normal ops
		setWatchDog();		//enable watchdog
		resetTWI();			//reset TWI com
	}
	if (~PINA & INPUT_DIE)
	{
		robotState = STATE_ASLEEP;
		
		OCR1A = 0;
		OCR1B = 0;
		PORTB = 0x00;	//light leds to notice it received the input

		while(~PINA & INPUT_DIE){}	// to make sure switch is released

		wdt_reset();
	}
}


#endif //_PORTSCONTROL_H_

