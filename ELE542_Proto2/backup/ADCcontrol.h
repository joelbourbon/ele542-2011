#ifndef _ADCCONTROL_H_
#define _ADCCONTROL_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "motorControl.h"
#include "timer.h"

//#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define MAX_ADC_SAMPLES	10
#define CAL_SAMPLES 24
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

#define MOTOR_DIR_IN_LEFT	0x04
#define MOTOR_DIR_IN_RIGHT	0x08

void initADC(void);
void calibrateMotors(void);
void readADC(void);
void startADC(void);
void stopADC(void);

void checkForAdc(void);

void compileAdcData(void);
void fetchAdcValues(void);
void waitValueIsReady(void);

volatile unsigned char adcCompileReadyFlag;

extern volatile unsigned char adcCompletedFlag;

volatile unsigned char adcReadyFlag;//Adc interrupt flag
volatile unsigned int adcValuesCounter;		//Counter for controling the number of ADC samples to take
volatile unsigned char adcMUXValue;			//Keeps the current state of ADC MUX register --> reading left or right ?
extern volatile unsigned char adcCalibrationFlag;	//0 = Normal operations 1 = Calibration mode
volatile unsigned char i; // Calibration iterator

struct MotorLimits
{
	float posMax;
	float negMax;
	float posZero;
	float negZero;
	int adcMaxValueHigh;
	int adcMaxValueLow;
}leftLimits, rightLimits;

extern volatile  struct motorSpeeds adcValues;


volatile unsigned int TotalValueLeft;
volatile unsigned int TotalValueRight;
volatile unsigned int ActualValueLeft;
volatile unsigned int ActualValueRight;
//volatile unsigned char motorDirLeft;
//volatile unsigned char motorDirRight;

ISR(ADC_vect)
{
	if (adcCompletedFlag == 0)
		readADC();
}

void startADC(void)
{
	while (adcCompletedFlag == 0) {}
	adcCompletedFlag = 0;
	waitValueIsReady();
	while (adcCompletedFlag == 0) {}
	adcCompletedFlag = 0;
}

void stopADC(void)
{
	while (adcCompletedFlag == 1) {}
	adcCompletedFlag = 1;
}


void initADC(void)
{
	//ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN)|(1<<ADIE)|(1<<ADSC);

	SFIOR = (0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0);

	adcMUXValue = ADC_MUX_READ_LEFT;
	ADMUX = adcMUXValue;
	
	ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN)|(1<<ADIE)|(1<<ADSC)|(1<<ADATE);

	adcCalibrationFlag = 0;
	adcValuesCounter = 0;
	TotalValueRight = 0;
	TotalValueLeft = 0;

	leftLimits.adcMaxValueHigh = MAX_ADC_VALUE;
	leftLimits.adcMaxValueLow = MAX_ADC_VALUE;
	leftLimits.posMax = (float)MAX_ADC_VALUE;
	leftLimits.negMax = (float)((-1)*MAX_ADC_VALUE);
	leftLimits.posZero = 0.0;
	leftLimits.negZero = 0.0;
	
	rightLimits.adcMaxValueHigh = MAX_ADC_VALUE;
	rightLimits.adcMaxValueLow = MAX_ADC_VALUE;
	rightLimits.posMax = (float)MAX_ADC_VALUE;
	rightLimits.negMax = (float)((-1)*MAX_ADC_VALUE);
	rightLimits.posZero = 0.0;
	rightLimits.negZero = 0.0;

	adcCompileReadyFlag = 0;
	
}

void readADC(void)
{
	if (adcCompileReadyFlag == 0)
		fetchAdcValues();	

	if (adcValuesCounter == MAX_ADC_SAMPLES)
	{
		adcCompileReadyFlag = 1;
	}
}

void checkForAdc(void)
{
	if (adcCompileReadyFlag == 1)
	{	
		compileAdcData();
		adcCompileReadyFlag = 0;
		motorReadyFlag = 1;
	}
}

void fetchAdcValues(void)
{
	//from ADC... to integer
	if (adcMUXValue == ADC_MUX_READ_LEFT)
	{
		adcMUXValue = ADC_MUX_READ_RIGHT;
		ADMUX = ADC_MUX_READ_RIGHT;

		TotalValueLeft += (int)((unsigned char)ADCL);
		TotalValueLeft += (int)(((int)((unsigned char)ADCH))*256);
	}
	else if (adcMUXValue == ADC_MUX_READ_RIGHT)
	{
		adcMUXValue = ADC_MUX_READ_LEFT;
		ADMUX = ADC_MUX_READ_LEFT;

		TotalValueRight += (int)((unsigned char)ADCL);
		TotalValueRight += (int)(((int)((unsigned char)ADCH))*256);
		
		adcValuesCounter++;
	}
}
void compileAdcData(void)
{
	//cli();

	ActualValueLeft = TotalValueLeft;
	ActualValueRight = TotalValueRight;

	//LEFT MOTOR
	//adcValues.left = TotalValueLeft * MAX_ADC_SAMPLES_RATIO;
	adcValues.left = ActualValueLeft / MAX_ADC_SAMPLES;
	//Check motor direction input
	if ((MOTOR_DIR_IN_LEFT & PINA) == MOTOR_DIR_IN_LEFT)
	{
		adcValues.left = ((-1)*(adcValues.left));
		if (adcValues.left < leftLimits.negZero)
		{
			adcValues.left -= leftLimits.negZero;
			if (adcValues.left < leftLimits.negMax)
			{
				adcValues.left = leftLimits.negMax;
			}
			adcValues.left = (adcValues.left / leftLimits.adcMaxValueLow);
		}
		else
		{
			adcValues.left = 0.0;
		}
	}
	else
	{
		if (adcValues.left > leftLimits.posZero)
		{
			adcValues.left -= leftLimits.posZero;
			if (adcValues.left > leftLimits.posMax)
			{
				adcValues.left = leftLimits.posMax;
			}
			adcValues.left = adcValues.left / leftLimits.adcMaxValueHigh;
		}
		else
		{
			adcValues.left = 0.0;
		}
	}

	//RIGHT MOTOR
	//adcValues.right = TotalValueRight * MAX_ADC_SAMPLES_RATIO;
	adcValues.right = ActualValueRight / MAX_ADC_SAMPLES;
	//Check motor direction input
	if ((MOTOR_DIR_IN_RIGHT & PINA) == MOTOR_DIR_IN_RIGHT)
	{
		adcValues.right = ((-1)*adcValues.right);
		if (adcValues.right < rightLimits.negZero)
		{
			adcValues.right -= rightLimits.negZero;
			if (adcValues.right < leftLimits.negMax)
			{
				adcValues.right = rightLimits.negMax;
			}
			adcValues.right = (adcValues.right / rightLimits.adcMaxValueLow);
		}
		else
		{
			adcValues.right = 0.0;
		}
	}
	else
	{
		if (adcValues.right > rightLimits.posZero)
		{
			adcValues.right -= rightLimits.posZero;
			if (adcValues.right > rightLimits.posMax)
			{
				adcValues.right = rightLimits.posMax;
			}
			adcValues.right = adcValues.right / rightLimits.adcMaxValueHigh;
		}
		else
		{
			adcValues.right = 0.0;
		}
	}


		
	//reset values
	TotalValueRight = 0;
	TotalValueLeft = 0;
	adcValuesCounter = 0;
	adcCompletedFlag = 1;

	//sei();
}

void calibrateMotors(void)
{
	adcCalibrationFlag = 1;
	//CA SERT A RIEN CE QUE JE FAIT POUR L'INSTANT... C'EST SUR LE RANGE QU'IL FAUT INTERPRÉTER LA DIFFÉRENCE
	//SINON C'EST JUSTE PLUS FACILE D'ATTEINDRE LES LIMITES
	//make sure motors are stopped
	OCR1AL = 0;
	OCR1AH = 0;
	OCR1BL = 0;
	OCR1BH = 0;

	for (i=0;i<CAL_SAMPLES;i++){

	PORTD = (LEFT_MOTOR_FORWARD)|(RIGHT_MOTOR_STOP);
	PORTA = (1<<CALIB_BIT);
	waitValueIsReady();
	leftLimits.posMax = adcValues.left * MAX_ADC_RATIO;

	}

	for (i=0;i<CAL_SAMPLES;i++){

	PORTD = (LEFT_MOTOR_FORWARD)|(RIGHT_MOTOR_STOP);
	PORTA = (0<<CALIB_BIT);
	waitValueIsReady();
	leftLimits.posZero = adcValues.left * MAX_ADC_RATIO;
	
	}	

	for (i=0;i<CAL_SAMPLES;i++){

	PORTD = (LEFT_MOTOR_BACKWARD)|(RIGHT_MOTOR_STOP);
	PORTA = (1<<CALIB_BIT);
	waitValueIsReady();
	leftLimits.negMax = adcValues.left * MAX_ADC_RATIO;

	}

	PORTD = (LEFT_MOTOR_BACKWARD)|(RIGHT_MOTOR_STOP);
	PORTA = (0<<CALIB_BIT);
	waitValueIsReady();
	leftLimits.negZero = adcValues.left * MAX_ADC_RATIO;

	leftLimits.posMax -= leftLimits.posZero;
	if (leftLimits.posMax > 1.0)
	{
		leftLimits.adcMaxValueHigh	= MAX_ADC_RATIO;
	}
	else
	{
		leftLimits.adcMaxValueHigh = (int)(MAX_ADC_RATIO * leftLimits.posMax);
	}
	leftLimits.negMax -= leftLimits.negZero;
	if (leftLimits.negMax < -1.0)
	{
		leftLimits.adcMaxValueLow = MAX_ADC_RATIO;
	}
	else
	{
		leftLimits.adcMaxValueLow = (int)((-1) * MAX_ADC_RATIO * leftLimits.negMax);
	}

	PORTD = (RIGHT_MOTOR_FORWARD)|(LEFT_MOTOR_STOP);
	PORTA = (1<<CALIB_BIT);
	waitValueIsReady();
	rightLimits.posMax = adcValues.right * MAX_ADC_RATIO;

	PORTD = (RIGHT_MOTOR_FORWARD)|(LEFT_MOTOR_STOP);
	PORTA = (0<<CALIB_BIT);
	waitValueIsReady();
	rightLimits.posZero = adcValues.right * MAX_ADC_RATIO;

	PORTD = (RIGHT_MOTOR_BACKWARD)|(LEFT_MOTOR_STOP);
	PORTA = (1<<CALIB_BIT);
	waitValueIsReady();
	rightLimits.negMax = adcValues.right * MAX_ADC_RATIO;

	PORTD = (RIGHT_MOTOR_BACKWARD)|(LEFT_MOTOR_STOP);
	PORTA = (0<<CALIB_BIT);
	waitValueIsReady();
	rightLimits.negZero = adcValues.right * MAX_ADC_RATIO;	

	rightLimits.posMax -= rightLimits.posZero;
	if (rightLimits.posMax > 1.0)
	{
		rightLimits.adcMaxValueHigh	= MAX_ADC_RATIO;
	}
	else
	{
		rightLimits.adcMaxValueHigh = (int)(MAX_ADC_RATIO * rightLimits.posMax);
	}
	rightLimits.negMax -= rightLimits.negZero;
	if (rightLimits.negMax < -1.0)
	{
		rightLimits.adcMaxValueLow = MAX_ADC_RATIO;
	}
	else
	{
		rightLimits.adcMaxValueLow = (int)((-1) * MAX_ADC_RATIO * rightLimits.negMax);
	}

	adcCalibrationFlag = 0;

	PORTD = (RIGHT_MOTOR_STOP) | (LEFT_MOTOR_STOP);
}

void waitValueIsReady(void)
{
	adcCompletedFlag = 0;
	while (adcCompletedFlag == 0){}
	adcCompletedFlag = 0;
	while (adcCompletedFlag == 0){}
}

#endif //_ADCCONTROL_H_


