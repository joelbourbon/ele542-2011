////////////////////////////////////////////////////////
//File: timer.h
//
//Header file for timer1 control
//
//This file contains methods to set motor outputs that 
//are connected to TIMER1 PWM outputs
//
//Author: Vincent Léger, Jason Cormier
//
////////////////////////////////////////////////////////

/**************/
/***INCLUDES***/
/**************/
#include "ucos_core/includes.h"
#include "sonars.h"

#ifndef _TIMER_H_
#define _TIMER_H_



/**************/
/***DEFINES****/
/**************/
#define MAX_OVERFLOW_VALUE 10000	//TIMER1 OVERFLOW value


/**************************/
/***Function prototypes****/
/**************************/
void initTimer1(void);
void dealWithMotors(void);


/**************************/
/*****Extern functions*****/
/**************************/
extern void setMotors(void);

/**************/
/***GLOBALS****/
/**************/
volatile unsigned char timer1ReadyFlag;		 //1 = cycle finished, 0 = running
volatile unsigned char motorReadyFlag;		 //timer 1 overflow flag
volatile unsigned char adcCalibrationFlag;	 //adc compile finished flag

/***********************/
/***External GLOBALS****/
/***********************/
extern volatile unsigned char pingCounter;	//used 





#endif //_TIMER_H_

