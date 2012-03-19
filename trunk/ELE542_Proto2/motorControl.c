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
#include "motorControl.h"


/***************************/
/***Function definitions****/
/***************************/
///////////////////////////////////////////
//Function name: initMotors
//
//Brief:   init ports for motor operations
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void initMotors(void)
{
	//TODO: move calib here
	
	//initialize motor direction outputs
	motorDirection = (LEFT_MOTOR_BRAKE)|(RIGHT_MOTOR_BRAKE);
	PORTD = motorDirection;

	//Init motor command from controler
	requestedCommand.speed = 100;
	requestedCommand.angle = 0;

	//init all motor and adc data
	adcValues.left = 0;
	adcValues.right = 0;
	dutyCycleLeft = 0;
	dutyCycleRight = 0;
	computedCommand.speed = 0.0;
	computedCommand.angle = 0.0;
	
	//Set motors outputs to 0
	OCR1A = 0;
	OCR1B = 0;
}

///////////////////////////////////////////
//Function name: setMotors
//
//Brief:   main motor function, calls all other
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void setMotors(void)
{
	//get new duty cycle values
	CalculPWM((float)computedCommand.speed, (float)computedCommand.angle, adcValues.left, adcValues.right, &dutyCycleLeft, &dutyCycleRight);
	//convert flot duty cycles to int
	convertValuesToInt();
	//set direction outputs
	checkDirection();
	//set pwm values on timer1
	setMotorOutputs();

	adcCompletedFlag = 0;	//flag to ADC that it can restart its job

}

///////////////////////////////////////////
//Function name: checkDirection
//
//Brief:   sets motor direction outputs 
//			according to dutyCycle value 
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void checkDirection(void)
{

	if (convertedMotorSpeed.left > ABSOLUTE_ZERO)	//if dutyCycle is positive
	{
		motorDirection = LEFT_MOTOR_FORWARD;
	}	
	else if (convertedMotorSpeed.left < ABSOLUTE_ZERO)	//if it is negative
	{
		motorDirection = LEFT_MOTOR_BACKWARD;
		convertedMotorSpeed.left = ((-1)*convertedMotorSpeed.left);
	}
	else if (convertedMotorSpeed.left == ABSOLUTE_ZERO)	//if we got a big 0
	{
		motorDirection = LEFT_MOTOR_STOP;
	}
	
	if (convertedMotorSpeed.right > ABSOLUTE_ZERO)	//same thing as with left
	{
		motorDirection |= RIGHT_MOTOR_FORWARD;
	}	
	else if (convertedMotorSpeed.right < ABSOLUTE_ZERO)
	{
		motorDirection |= RIGHT_MOTOR_BACKWARD;
		convertedMotorSpeed.right = ((-1)*convertedMotorSpeed.right);
	}
	else if (convertedMotorSpeed.right == ABSOLUTE_ZERO)	//in the case of a 0
	{
		if (motorDirection == (LEFT_MOTOR_STOP))	//verify if left is also 0
		{
			motorDirection = (LEFT_MOTOR_BRAKE | RIGHT_MOTOR_BRAKE);	//then brake
		}
		else
		{
			motorDirection |= RIGHT_MOTOR_STOP;	//else only send stop
		}
	}

}

///////////////////////////////////////////
//Function name: setMotorOutputs
//
//Brief:   Send output values for motor
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void setMotorOutputs(void)
{


	//set motor outputs
	//TODO: Add a way to avoid obstacles
	
	if ((objectDetectedRight == 1) || (objectDetectedLeft == 1))
	{
		OCR1A = 0;
		OCR1B = 0;
		PORTD = (LEFT_MOTOR_BRAKE | RIGHT_MOTOR_BRAKE);

	}
	else
	{
		OCR1A = convertedMotorSpeed.left;
		OCR1B = convertedMotorSpeed.right;
		PORTD = motorDirection;
	}

}

///////////////////////////////////////////
//Function name: convertValuesToInt
//
//Brief:   Converts duty cycle values from float to int
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void convertValuesToInt(void)
{

	convertedMotorSpeed.left = (signed int)((dutyCycleLeft*MAX_OVERFLOW_VALUE));
	convertedMotorSpeed.right = (signed int)((dutyCycleRight*MAX_OVERFLOW_VALUE));

}
