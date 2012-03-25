////////////////////////////////////////////////////////
//File: ADCcontrol.h
//
//Header file for ADC controls
//
//This file contains functions to collect, interpret
// and convert ADC data. It also contains calibration
// related functions
//
//Author: Vincent Léger, Jason Cormier
//
////////////////////////////////////////////////////////

/**************/
/***INCLUDES***/
/**************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "motorControl.h"

#ifndef _ADCCONTROL_H_
#define _ADCCONTROL_H_


/**************/
/***DEFINES****/
/**************/

//#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define MAX_ADC_SAMPLES	20
#define CAL_SAMPLES 25
#define CAL_SAMPLES_RATIO  (1/CAL_SAMPLES)
#define MAX_ADC_SAMPLES_RATIO	(1/MAX_ADC_SAMPLES)
//#define MAX_ADC_SAMPLES	20 	//ADC clock = 125kHz and motor outputs at 200Hz...
							//Max possible value: 125000/200 = 625 ticks
							//                    625 ticks / 2 motors = 312
							//					  312 ticks / 13(ticks/conversion) = 24conversions per motor 
#define ADC_MUX_READ_LEFT	((0<<REFS1)|(0<<REFS0)|(0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0)|(0<<ADLAR))
#define ADC_MUX_READ_RIGHT  ((0<<REFS1)|(0<<REFS0)|(0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(1<<MUX0)|(0<<ADLAR))
#define MAX_ADC_VALUE	1023
//#define MAX_ADC_RATIO	(float)(1/MAX_ADC_VALUE)
#define MAX_ADC_RATIO	/*(float)*/(0.0009775171065493646138807)//(1/MAX_ADC_VALUE)


#define MOTOR_DIR_LEFT		2
#define MOTOR_DIR_RIGHT		3

#define MOTOR_DIR_IN_LEFT	0x04 // DIRg on PORTA
#define MOTOR_DIR_IN_RIGHT	0x08 // DIRd on PORTA

/**************************/
/***Function prototypes****/
/**************************/

void initADC(void);
void calibrateMotors(void);
void readADC(void);
void startADC(void);
void stopADC(void);

void checkForAdc(void);

void compileAdcData(void);
void fetchAdcValues(void);
void fetchCalib(void); // test
void waitValueIsReady(void);

volatile float ADCcalib_value; //  Add ADC mean values during calibration

volatile unsigned char adcCompileReadyFlag; //When set, MAX_ADC_SAMPLES were added to a "total value" variable

//extern volatile unsigned char adcCompletedFlag; //ADC mean value for current cycle computed

volatile unsigned char adcReadyFlag;//ADC interrupt flag
volatile unsigned int adcValuesCounter;		//Counter for controling the number of ADC samples to take
volatile unsigned char adcMUXValue;			//Keeps the current state of ADC MUX register --> reading left or right ?
//extern volatile unsigned char adcCalibrationFlag;	//0 = Normal operations 1 = Calibration mode
volatile unsigned char i; // Calibration iterator

struct MotorLimits
{
	float posMax; // Vmax+
	float negMax; // Vmax-
	float posZero; // Vzero+
	float negZero; // Vzero-
	
	//Added to avoid problem while using compileAdcData in calibrateMotors
	//Could be removed by using a special ADC Data compiling function dedicated to calibrateMotors
	float posMaxTEMP;
	float negMaxTEMP;
	float posZeroTEMP;
	float negZeroTEMP;
	//int adcMaxValueHigh;
	//int adcMaxValueLow;
	
}leftLimits, rightLimits; // right & left motors

//extern volatile  struct motorSpeeds adcValues; //adcValues.left  and adcValues.right on a scale of [-1 to 1]


volatile unsigned int TotalValueLeft; // Add ADC values before calculating mean value for left motor
volatile unsigned int TotalValueRight; // Add ADC values before calculating mean value for right motor
volatile unsigned int totalCalibLeft;
volatile unsigned int totalCalibLeft;
volatile unsigned char fetchCalibDone;
//volatile unsigned int ActualValueLeft;
//volatile unsigned int ActualValueRight;
//volatile unsigned char motorDirLeft;
//volatile unsigned char motorDirRight;

#endif //_ADCCONTROL_H_


