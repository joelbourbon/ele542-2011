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
/**************/
/***INCLUDES***/
/**************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "sonars.h"


#ifndef _PORTSCONTROL_H_
#define _PORTSCONTROL_H_



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
//extern void ledAliveState(void);	//set leds for normal operations

/**************/
/***GLOBALS****/
/**************/
volatile unsigned char robotState;	//robot's global state (dead or alive)



#endif //_PORTSCONTROL_H_

