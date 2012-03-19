////////////////////////////////////////////////////////
//File: timer.c
//
//Source file for timer1 control
//
//This file contains methods to set motor outputs that 
//are connected to TIMER1 PWM outputs
//
//Author: Vincent Léger, Jason Cormier
//
////////////////////////////////////////////////////////
#include "timer.h"

/*****************/
/***Interrupts****/
/*****************/
///////////////////////////////////////////
//Interrupt name: ISR(TIMER1_OVF_vect)
//
//Definition: TIMER1 OVERFLOW INTERRUPT
//
//Brief: Sets motorReadyFlag to set new values
//		 on motor outputs.
//
///////////////////////////////////////////
ISR(TIMER1_OVF_vect) //timer1 overflow
{
	motorReadyFlag = 1;	//flag for motors
}

/***************************/
/***Function definitions****/
/***************************/
///////////////////////////////////////////
//Function name: dealWithMotors
//
//Brief:   check if flag is ready for next
//			motor command. To be called from main.
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void dealWithMotors(void)
{
	if (motorReadyFlag == 1)
	{
		if (pingCounter <= MAX_DELAY_PING)	//increase counter for pings
		{
			pingCounter++;
		}	
		setMotors();	//set motor outputs
		motorReadyFlag = 0;
	}
}

///////////////////////////////////////////
//Function name: initTimer1
//
//Brief:   initialize TIMER1 for motor operations
//
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void initTimer1(void)
{
	//Set timer1 register values for desired operation
	//won't change output settings, fast PWM, using output compare, disable input capture
	TCCR1A = (0<<COM1A0) | (1<<COM1A1) | (0<<COM1B0) | (1<<COM1B1) | (0<<FOC1A) | (0<<FOC1B) | (1<<WGM11) | (0<<WGM10);
	//no noise cancelation, detection on falling edge, reserved bit, fast PWM, prescaler = 8 
	TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<5) | (1<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);

	ICR1 = MAX_OVERFLOW_VALUE;	//set max overflow value

	//enable interrupt on timer1 (input capture and output compare disable, enable interrupt on timer overflow)
	TIMSK |= (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1);

	//init outputs to 0
	OCR1A = 0;
	OCR1B = 0;
	
	//init flags
	timer1ReadyFlag = 0;
	motorReadyFlag = 0;
	adcCalibrationFlag = 0;
}
