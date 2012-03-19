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
#include "moteur.h"
#include "timer.h"
#include "sonars.h"


#ifndef _MOTORCONTROL_H_
#define _MOTORCONTROL_H_



/**************/
/***DEFINES****/
/**************/
//Motors direction bits
#define DIR_G1		2
#define DIR_G2		3
#define DIR_D1		6
#define DIR_D2		7
#define CALIB_BIT	4

#define BASE_RATIO	(float)(0.00555555555555)	//1/180
#define ANGLE_RATIO	(float)(2*Pi*BASE_RATIO)	//pre-calculated value to avoid division
#define ABSOLUTE_ZERO (signed int)0

//Motor directions
#define LEFT_MOTOR_FORWARD		(1<<DIR_G1)|(0<<DIR_G2)
#define LEFT_MOTOR_BACKWARD		(0<<DIR_G1)|(1<<DIR_G2)
#define LEFT_MOTOR_STOP			(0<<DIR_G1)|(0<<DIR_G2)
#define LEFT_MOTOR_BRAKE		(1<<DIR_G1)|(1<<DIR_G2)
#define RIGHT_MOTOR_FORWARD		(1<<DIR_D1)|(0<<DIR_D2)
#define RIGHT_MOTOR_BACKWARD	(0<<DIR_D1)|(1<<DIR_D2)
#define RIGHT_MOTOR_STOP		(0<<DIR_D1)|(0<<DIR_D2)
#define RIGHT_MOTOR_BRAKE		(1<<DIR_D1)|(1<<DIR_D2)

//Calibration settings
#define CALIBRATE_LEFT	(1<<DIR_G1)|(0<<DIR_G2)|(0<<DIR_D1)|(0<<DIR_D2)
#define CALIBRATE_RIGHT	(0<<DIR_G1)|(0<<DIR_G2)|(1<<DIR_D1)|(0<<DIR_D2)

/**************************/
/***Function prototypes****/
/**************************/
//Commands globals
void setMotors(void);			//main motor function
void initMotors(void);			//init motors for normal operations
void checkDirection(void);		//check for motor direction
void setMotorOutputs(void);		//sets output for motor (direction + pwm)
void convertValuesToInt(void);	//converts floats to int

/**************************/
/*****Extern functions*****/
/**************************/
//to obtain real motor values
//extern void CalculPWM(float Vitesse_D, float Angle_D, float Vg, float Vd, float *Duty_G, float *Duty_D);

/**************/
/***GLOBALS****/
/**************/
volatile unsigned char adcCompletedFlag;	//flag for adc operations complete	
volatile unsigned char objectDetectedRight;	//flag for right object detected (sonar)
volatile unsigned char objectDetectedLeft;	//flag for left object detected (sonar)


volatile struct MotorCommands	//struct to hold received motor commands from controler
{
	unsigned char speed;
	unsigned char angle;
}requestedCommand, actualCommand;

volatile struct int_motorSpeeds	//value to send in OCR1A and OCR1B (pwm outputs)
{
	//unsigned int left;
	//unsigned int right;
	signed int left;
	signed int right;
}convertedMotorSpeed;

volatile struct motorSpeeds	//values from ADC
{
	float left;
	float right;
}adcValues;

struct float_requestedCommand	//command to send in calculPWM	
{
	float angle;
	float speed;
}computedCommand;

float dutyCycleLeft;	//left dutyCycle from calculPWM
float dutyCycleRight;	//right dutyCycle from calculPWM

volatile unsigned char motorDirection;	//Settings for motor direction output


#endif //_MOTORCONTROL_H_

