////////////////////////////////////////////////////////
//File: ADCcontrol.c
//
//Source file for ADC controls
//
//This file contains functions to collect, interpret
// and convert ADC data. It also contains calibration
// related functions
//
//Author: Vincent Léger, Jason Cormier
//
////////////////////////////////////////////////////////


#include "ADCcontrol.h"

/***************************/
/***Function definitions****/
/***************************/

ISR(ADC_vect)
{
	if (adcCompletedFlag == 0)
		readADC();
}

/* Vérifier et effacer
void startADC(void)
{
	while (adcCompletedFlag == 0) {}
	adcCompletedFlag = 0;
	waitValueIsReady();
	while (adcCompletedFlag == 0) {}
	adcCompletedFlag = 0;
}
*/

/* Vérifier et effacer
void stopADC(void)
{
	while (adcCompletedFlag == 1) {}
	adcCompletedFlag = 1;
}
*/

///////////////////////////////////////////
//Function name: initADC
//
//Brief:   Set relevant registers and initialize all variables
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
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


	// Initiatlisation des valeurs limites précalibration
	// Entre -1023 et -0; 0 et 1023.
	
	// Initializing with theorical limit values 
	leftLimits.posMax = (float)MAX_ADC_VALUE; // Vmax+ = 1023
	leftLimits.negMax = (float)((-1)*MAX_ADC_VALUE); // Vmax- = 1023
	leftLimits.posZero = 0.0; // Vzero+ = 1023
	leftLimits.negZero = 0.0; // Vzero- = 1023
	
	rightLimits.posMax = (float)MAX_ADC_VALUE; // Vmax+ = 1023
	rightLimits.negMax = (float)((-1)*MAX_ADC_VALUE); // Vmax- = 1023
	rightLimits.posZero = 0.0; // Vzero+ = 1023
	rightLimits.negZero = 0.0; // Vzero- = 1023

	adcCompileReadyFlag = 0;
	

}

///////////////////////////////////////////
//Function name: readADC
//
//Brief:   Ask to fetch ADC values until enough samples are acquired
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void readADC(void)
{
	if (adcCompileReadyFlag == 0)
		fetchAdcValues();  // Collect an ADC value

	// When MAX_ADC_SAMPLES values are acquired 
	if (adcValuesCounter == MAX_ADC_SAMPLES)
	{
		adcCompileReadyFlag = 1; // Mean value may now be calculated 
	}
}

///////////////////////////////////////////
//Function name: checkForAdc
//
//Brief:   This will call the function that calculates the ADC mean value once readADC() 
//			allows it by setting adcCompileReadyFlag. This function is called in a loop in
//			the main as long as the robot is ALIVE.
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void checkForAdc(void)
{
	if (adcCompileReadyFlag == 1)
	{	
		compileAdcData(); // Compute ADC mean value for the current cycle
		adcCompileReadyFlag = 0; // New values must be acquired for tne next cycle
		motorReadyFlag = 1; // The newly computed ADC value is ready to be sent to CalculPWM
	}
}

///////////////////////////////////////////
//Function name: fetchAdcValues
//
//Brief: ADC value will be fetched according to adcMUXValue and 
//			added to a "total value" variable for each motor
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void fetchAdcValues(void)
{
	//from ADC... to integer
	if (adcMUXValue == ADC_MUX_READ_LEFT)
	{
		// Next acquisition must be for the RIGHT motor
		adcMUXValue = ADC_MUX_READ_RIGHT;
		ADMUX = ADC_MUX_READ_RIGHT;
		
		// Reading from ADC registers
		TotalValueLeft += (int)((unsigned char)ADCL);
		TotalValueLeft += (int)(((int)((unsigned char)ADCH))<<8);


	}
	else if (adcMUXValue == ADC_MUX_READ_RIGHT)
	{
		// Next acquisition must be for the LEFT motor
		adcMUXValue = ADC_MUX_READ_LEFT;
		ADMUX = ADC_MUX_READ_LEFT;

		// Reading from ADC registers
		TotalValueRight += (int)((unsigned char)ADCL);
		TotalValueRight += (int)(((int)((unsigned char)ADCH))<<8);
	

		adcValuesCounter++; //Once both motor speed have been read
	}
}

///////////////////////////////////////////
//Function name: compileAdcData
//
//Brief:   This will calculate the average of ADC values from TotalValueLeft and
//			TotalValueLeft. Then, it will assign a value between -1.0 and 1.0
// 			to adcValues.right and adcValues.left ready to be used in a 
//			CalculPWM function. It will also take into account calibration if it had been done
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void compileAdcData(void)
{	
	/* Calculate mean value */
	//adcValues.left and adcValues.right are from 0 to 1023 for now
	//they will be from -1 to 1 at the end of the function
	adcValues.left = TotalValueLeft / MAX_ADC_SAMPLES; 
	adcValues.right = TotalValueRight / MAX_ADC_SAMPLES;
	
	//LEFT MOTOR
	//Check motor direction input
	if (MOTOR_DIR_IN_LEFT & PINA)
	/*Left Motor BACKWARD case*/
	{
		adcValues.left = ((-1)*(adcValues.left)); //if Motor is spinning backward, the value should be from 0 to -1023

		if (adcValues.left < leftLimits.negZero)  // Comparaison avec valeur sur 1
		{
			if (adcValues.left < leftLimits.negMax)
			{
				//if value is over the calibration limit, it is set to the current limit
				//so that it takes the maximum value
				adcValues.left = leftLimits.negMax;
			}
			//Conversion to a unit scale
			// Relative value divided by effective range
			adcValues.left = (-1) * (adcValues.left-leftLimits.negZero) / (leftLimits.negMax-leftLimits.negZero);
		}
		else
		{
			// If value is between 0 and calibration zero, it is set to the lowest value, 0.0
			adcValues.left = 0.0;
		}
	}
	else
	/*Left Motor FORWARD case*/
	{
		if (adcValues.left > leftLimits.posZero)
		{
			if (adcValues.left > leftLimits.posMax)
			{
				adcValues.left = leftLimits.posMax;
			}
			//Conversion to a unit scale
			// Relative value divided by effective range
			adcValues.left = (adcValues.left-leftLimits.posZero) / (leftLimits.posMax-leftLimits.posZero);
		}
		else
		{
			// If value is between 0 and calibration zero, it is set to the lowest value, 0.0
			adcValues.left = 0.0;
		}
	}

	//RIGHT MOTOR
	//Check motor direction input
	if (MOTOR_DIR_IN_RIGHT & PINA)
	/*Right Motor BACKWARD case*/
	{
		adcValues.right = ((-1)*adcValues.right); //if Motor is spinning backward, the value should be from 0 to -1023
		if (adcValues.right < rightLimits.negZero)
		{
			if (adcValues.right < leftLimits.negMax)
			{
				//if value is over the calibration limit, it is set to the current limit
				//so that it takes the maximum value
				adcValues.right = rightLimits.negMax;
			}
			//Conversion to a unit scale
			// Relative value divided by effective range
			adcValues.right = (-1) * (adcValues.right-rightLimits.negZero) / (rightLimits.negMax-rightLimits.negZero);
		}
		else
		{
			// If value is between 0 and calibration zero, it is set to the lowest value, 0.0
			adcValues.right = 0.0;
		}
	}
	else
	/*Right Motor FORWARD case*/
	{
		if (adcValues.right > rightLimits.posZero)
		{
		//	adcValues.right -= rightLimits.posZero; //REMOVE
			if (adcValues.right > rightLimits.posMax)
			{
				//if value is over the calibration limit, it is set to the current limit
				//so that it takes the maximum value
				adcValues.right = rightLimits.posMax; 
			}
			//Conversion to a unit scale
			// Relative value divided by effective range
			adcValues.right = (adcValues.right-rightLimits.posZero) / (rightLimits.posMax-rightLimits.posZero);
		}
		else
		{
			// If value is between 0 and calibration zero, it is set to the lowest value, 0.0
			adcValues.right = 0.0;
		}
	}
	

	//reset values
	TotalValueRight = 0;
	TotalValueLeft = 0;
	adcValuesCounter = 0;
	
	adcCompletedFlag = 1;   //ADC final values computing is done
}

///////////////////////////////////////////
//Function name: calibrateMotors
//
//Brief:   Find the values that the ADC will send for limit values of both motor.
//			ADC values sent to CalculPWM will be converted to a [-1:1] range
//			according to the calibration range.
//
//			ADC range without calibration:
//			|-1023 | ... |0| ... |1023|
//
//			ADC range with calibration:
//			|Vmax- and lower| ... |Between Vzero- and Vzero+| ... |Vmax+ and higher|
//
//Inputs: -none
//
//Outputs:	-none
//
//Return value:	-none
//
///////////////////////////////////////////
void calibrateMotors(void)
{
	ADCcalib_value = 0.0;
	adcCalibrationFlag = 1; // Starting calibration
	
	//make sure motors are stopped so that PWM stays at 0
	OCR1AL = 0;
	OCR1AH = 0;
	OCR1BL = 0;
	OCR1BH = 0;

	/*#######################*/
	/* LEFT MOTOR CALIBRATION*/
	/*#######################*/

	// Left Max Forward Calibration
	PORTD = (LEFT_MOTOR_FORWARD)|(RIGHT_MOTOR_STOP);
	PORTA = (1<<CALIB_BIT);

	for (i=0;i<CAL_SAMPLES;i++){
		while (adcCompileReadyFlag == 0);
		compileAdcData();
		ADCcalib_value += adcValues.left;
		
		
	}
	leftLimits.posMaxTEMP = (ADCcalib_value * MAX_ADC_VALUE) / CAL_SAMPLES;
	ADCcalib_value = 0.0;

	// Left Zero Forward Calibration
	PORTD = (LEFT_MOTOR_FORWARD)|(RIGHT_MOTOR_STOP);
	PORTA = (0<<CALIB_BIT);

	for (i=0;i<CAL_SAMPLES;i++){
		while (adcCompileReadyFlag == 0);
		compileAdcData();
		ADCcalib_value += adcValues.left;
	}
	leftLimits.posZeroTEMP = (ADCcalib_value * MAX_ADC_VALUE) / CAL_SAMPLES;
	ADCcalib_value = 0.0;	

	// Left Max Backward Calibration
	PORTD = (LEFT_MOTOR_BACKWARD)|(RIGHT_MOTOR_STOP);
	PORTA = (1<<CALIB_BIT);

	for (i=0;i<CAL_SAMPLES;i++){
		while (adcCompileReadyFlag == 0);
		compileAdcData();
		ADCcalib_value += adcValues.left;
	}
	leftLimits.negMaxTEMP = (ADCcalib_value * MAX_ADC_VALUE) / CAL_SAMPLES;
	ADCcalib_value = 0.0;

	// Left Zero Backward Calibration
	PORTD = (LEFT_MOTOR_BACKWARD)|(RIGHT_MOTOR_STOP);
	PORTA = (0<<CALIB_BIT);

	for (i=0;i<CAL_SAMPLES;i++){
		while (adcCompileReadyFlag == 0);
		compileAdcData();
		ADCcalib_value += adcValues.left;
	}
	leftLimits.negZeroTEMP = (ADCcalib_value * MAX_ADC_VALUE) / CAL_SAMPLES;
	ADCcalib_value = 0.0;

	/*#######################*/
	/*RIGHT MOTOR CALIBRATION*/
	/*#######################*/ 

	// Right Max Forward Calibration
	PORTD = (RIGHT_MOTOR_FORWARD)|(LEFT_MOTOR_STOP);
	PORTA = (1<<CALIB_BIT);
	ADCcalib_value = 0.0;

	for (i=0;i<CAL_SAMPLES;i++){
		while (adcCompileReadyFlag == 0);
		compileAdcData();
		ADCcalib_value += adcValues.right;
	}
	rightLimits.posMaxTEMP = (ADCcalib_value * MAX_ADC_VALUE) / CAL_SAMPLES;
	ADCcalib_value = 0.0;

	// Right Zero Forward Calibration
	PORTD = (RIGHT_MOTOR_FORWARD)|(LEFT_MOTOR_STOP);
	PORTA = (0<<CALIB_BIT);

	for (i=0;i<CAL_SAMPLES;i++){
		while (adcCompileReadyFlag == 0);
		compileAdcData();
		ADCcalib_value += adcValues.right;
	}
	rightLimits.posZeroTEMP = (ADCcalib_value * MAX_ADC_VALUE) / CAL_SAMPLES;
	ADCcalib_value = 0.0;

	// Right Max Backward Calibration
	PORTD = (RIGHT_MOTOR_BACKWARD)|(LEFT_MOTOR_STOP);
	PORTA = (1<<CALIB_BIT);

	for (i=0;i<CAL_SAMPLES;i++){
		while (adcCompileReadyFlag == 0);
		compileAdcData();
		ADCcalib_value += adcValues.right;
	}
	rightLimits.negMaxTEMP = (ADCcalib_value * MAX_ADC_VALUE) / CAL_SAMPLES;
	ADCcalib_value = 0.0;

	// Right Zero Backward Calibration
	PORTD = (RIGHT_MOTOR_BACKWARD)|(LEFT_MOTOR_STOP);
	PORTA = (0<<CALIB_BIT);

	for (i=0;i<CAL_SAMPLES;i++){
		while (adcCompileReadyFlag == 0);
		compileAdcData();
		ADCcalib_value += adcValues.right;
	}
	rightLimits.negZeroTEMP = 	(ADCcalib_value * MAX_ADC_VALUE) / CAL_SAMPLES;
	ADCcalib_value = 0.0;

 //CHANGEMENT temporaire
	// Assign final values to limit var
	leftLimits.posMax = leftLimits.posMaxTEMP;
	leftLimits.posZero = leftLimits.posZeroTEMP;
	leftLimits.negMax = leftLimits.negMaxTEMP;
	leftLimits.negZero = leftLimits.negZeroTEMP;
	rightLimits.posMax = rightLimits.posMaxTEMP;
	rightLimits.posZero = rightLimits.posZeroTEMP;
	rightLimits.negMax = rightLimits.negMaxTEMP;
	rightLimits.negZero = rightLimits.negZeroTEMP;
	
	// End calibration
	adcCalibrationFlag = 0;
	PORTD = (RIGHT_MOTOR_STOP) | (LEFT_MOTOR_STOP);

}

/* Vérifier et supprimer
void waitValueIsReady(void)
{
	//adcCompletedFlag = 0;		
	while (adcCompletedFlag == 0){}

	adcCompletedFlag = 0;
	while (adcCompletedFlag == 0){}


}
*/
