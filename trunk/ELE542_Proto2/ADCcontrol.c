#include "ADCcontrol.h"


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


	// Initiatlisation des valeurs limites précalibration
	// Entre -1023 et -0; 0 et 1023.
	/* Inutile
	leftLimits.adcMaxValueHigh = MAX_ADC_VALUE;
	leftLimits.adcMaxValueLow = MAX_ADC_VALUE; */
	leftLimits.posMax = (float)MAX_ADC_VALUE;
	leftLimits.negMax = (float)((-1)*MAX_ADC_VALUE);
	leftLimits.posZero = 0.0;
	leftLimits.negZero = 0.0;
	
	/* Inutile
	rightLimits.adcMaxValueHigh = MAX_ADC_VALUE;
	rightLimits.adcMaxValueLow = MAX_ADC_VALUE; */
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
		TotalValueLeft += (int)(((int)((unsigned char)ADCH))<<8);


	}
	else if (adcMUXValue == ADC_MUX_READ_RIGHT)
	{
		adcMUXValue = ADC_MUX_READ_LEFT;
		ADMUX = ADC_MUX_READ_LEFT;

		TotalValueRight += (int)((unsigned char)ADCL);
		TotalValueRight += (int)(((int)((unsigned char)ADCH))<<8);
	

		adcValuesCounter++;
	}
}
void compileAdcData(void)
{	
	// ### Corrigé! Mais non optimisé.

	//cli();


	//LEFT MOTOR
	//adcValues.left = TotalValueLeft * MAX_ADC_SAMPLES_RATIO;
	adcValues.left = TotalValueLeft / MAX_ADC_SAMPLES; 
	adcValues.right = TotalValueRight / MAX_ADC_SAMPLES;
	//Check motor direction input
	if (MOTOR_DIR_IN_LEFT & PINA)
	//BACKWARD
	{
		adcValues.left = ((-1)*(adcValues.left)); //adcValues sur 1024

		if (adcValues.left < leftLimits.negZero)  // Comparaison avec valeur sur 1
		{
			//adcValues.left -= leftLimits.negZero; //REMOVE
			if (adcValues.left < leftLimits.negMax)
			{
				adcValues.left = leftLimits.negMax;
			}
			//On ramène sur 1
			adcValues.left = (-1) * (adcValues.left-leftLimits.negZero) / (leftLimits.negMax-leftLimits.negZero);
			//adcValues.left = (adcValues.left / leftLimits.adcMaxValueLow);
		}
		else
		{
			//adcValues.left = 0.0; Non, la valeur doit être ramené à negZero
			//VINCE: not sure... tu veux vraiment envoyer la valeur de la zone morte ?
			//			en fait ce qu'on veut c'est qu'il le voit comme un zéro.
			//Oui MONSIEUR! Encore une boulette. C'est arrangé.
			adcValues.left = 0.0;
		}
	}
	else
	//FORWARD
	{
		if (adcValues.left > leftLimits.posZero)
		{
		//	adcValues.left -= leftLimits.posZero; //REMOVE
			if (adcValues.left > leftLimits.posMax)
			{
				adcValues.left = leftLimits.posMax;
			}
			adcValues.left = (adcValues.left-leftLimits.posZero) / (leftLimits.posMax-leftLimits.posZero);
			//adcValues.left = (adcValues.left-leftLimits.posZero) / leftLimits.adcMaxValueHigh; //adcValues sur 1
		}
		else
		{
			//adcValues.left = 0.0; Non, la valeur doit être ramené à posZero
			adcValues.left = 0.0;
		}
	}

	//RIGHT MOTOR
	//adcValues.right = TotalValueRight * MAX_ADC_SAMPLES_RATIO;
	//adcValues.right = ActualValueRight / MAX_ADC_SAMPLES;
	//Check motor direction input
	if (MOTOR_DIR_IN_RIGHT & PINA)
	//BACKWARD
	{
		adcValues.right = ((-1)*adcValues.right); //adcValues sur 1024
		if (adcValues.right < rightLimits.negZero)
		{
			//adcValues.right -= rightLimits.negZero; //REMOVE
			if (adcValues.right < leftLimits.negMax)
			{
				adcValues.right = rightLimits.negMax;
			}
			adcValues.right = (-1) * (adcValues.right-rightLimits.negZero) / (rightLimits.negMax-rightLimits.negZero);
			//adcValues.right = (adcValues.right / rightLimits.adcMaxValueLow);
		}
		else
		{
			//adcValues.right = 0.0; NO
			adcValues.right = 0.0;
		}
	}
	else
	//FORWARD
	{
		if (adcValues.right > rightLimits.posZero)
		{
		//	adcValues.right -= rightLimits.posZero; //REMOVE
			if (adcValues.right > rightLimits.posMax)
			{
				adcValues.right = rightLimits.posMax; 
			}
			adcValues.right = (adcValues.right-rightLimits.posZero) / (rightLimits.posMax-rightLimits.posZero);
			//adcValues.right = adcValues.right / rightLimits.adcMaxValueHigh; //adcValues sur 1
		}
		else
		{
			//adcValues.right = 0.0; NOPE
			adcValues.right = 0.0;
		}
	}
	
		
//	sei();

	//reset values
	TotalValueRight = 0;
	TotalValueLeft = 0;
	adcValuesCounter = 0;
	adcCompletedFlag = 1;
}

void calibrateMotors(void)
{
	ADCcalib_value = 0.0;
	adcCalibrationFlag = 1;
	//adcCompileReadyFlag = 1;
	//CA SERT A RIEN CE QUE JE FAIT POUR L'INSTANT... C'EST SUR LE RANGE QU'IL FAUT INTERPRÉTER LA DIFFÉRENCE
	//SINON C'EST JUSTE PLUS FACILE D'ATTEINDRE LES LIMITES
	//make sure motors are stopped
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
	
	patate2 = 0;

	for (i=0;i<CAL_SAMPLES;i++){
		while (adcCompileReadyFlag == 0);
		compileAdcData();
		//waitValueIsReady();
		ADCcalib_value += adcValues.left;
		
		//if (adcValues.left > 0.5)
		//{
		//	patate2+=1;
		//	PORTB = (0x0E) | (<<4);
		//	while(1);
		//}
		//if (patate2<100)
		//{
		//	PORTB = 0x0F;
		//	while(1);	
		//}
		
	}
	leftLimits.posMaxTEMP = (ADCcalib_value * MAX_ADC_VALUE) / CAL_SAMPLES;
	ADCcalib_value = 0.0;

	// Left Zero Forward Calibration
	PORTD = (LEFT_MOTOR_FORWARD)|(RIGHT_MOTOR_STOP);
	PORTA = (0<<CALIB_BIT);

	for (i=0;i<CAL_SAMPLES;i++){
		while (adcCompileReadyFlag == 0);
		compileAdcData();
		//waitValueIsReady();
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
		//waitValueIsReady();
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
		//waitValueIsReady();
		ADCcalib_value += adcValues.left;
	}
	leftLimits.negZeroTEMP = (ADCcalib_value * MAX_ADC_VALUE) / CAL_SAMPLES;
	ADCcalib_value = 0.0;


	// Remove left motor deadzone NO REMOVE DAT
	//leftLimits.posMax -= leftLimits.posZero;
	//leftLimits.negMax -= leftLimits.negZero;

/*
	// Set calibrated left max values
	if (leftLimits.posMax > 1.0) // Impossible, pourrait être enlevé
	{
		leftLimits.adcMaxValueHigh = MAX_ADC_VALUE;
	}
	else
	{
		leftLimits.adcMaxValueHigh = (int)(leftLimits.posMax);
	}
	//leftLimits.negMax -= leftLimits.negZero;

	if (leftLimits.negMax < -1.0)
	{
		leftLimits.adcMaxValueLow = MAX_ADC_VALUE;
	}
	else
	{
		leftLimits.adcMaxValueLow = (int)((-1) * leftLimits.negMax);
	}
*/




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
		//waitValueIsReady();
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
		//waitValueIsReady();
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
		//waitValueIsReady();
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
		//waitValueIsReady();
		ADCcalib_value += adcValues.right;
	}
	rightLimits.negZeroTEMP = 	(ADCcalib_value * MAX_ADC_VALUE) / CAL_SAMPLES;
	ADCcalib_value = 0.0;

	// Remove right motor deadzone NO REMOVE THAT BLARG
	//rightLimits.posMax -= rightLimits.posZero;
	//rightLimits.negMax -= rightLimits.negZero;

	// Set calibrated right max values 

/* Même pas utile...
	//TODO: [JAY]: CHECK IF THIS WORKS
	if (rightLimits.posMax > 1.0)
	{
		rightLimits.adcMaxValueHigh	= MAX_ADC_VALUE;
	}
	else
	{
		rightLimits.adcMaxValueHigh = (int)(rightLimits.posMax);
	}
	//rightLimits.negMax -= rightLimits.negZero;
	if (rightLimits.negMax < -1.0)
	{
		rightLimits.adcMaxValueLow = MAX_ADC_RATIO;
	}
	else
	{
		rightLimits.adcMaxValueLow = (int)((-1) * rightLimits.negMax);
	}
	
*/ 

 //CHANGEMENT pour éviter chiage
	// Assign final values to limit var
	leftLimits.posMax = leftLimits.posMaxTEMP;
	leftLimits.posZero = leftLimits.posZeroTEMP;
	leftLimits.negMax = leftLimits.negMaxTEMP;
	leftLimits.negZero = leftLimits.negZeroTEMP;
	rightLimits.posMax = rightLimits.posMaxTEMP;
	rightLimits.posZero = rightLimits.posZeroTEMP;
	rightLimits.negMax = rightLimits.negMaxTEMP;
	rightLimits.negZero = rightLimits.negZeroTEMP;

	//DEBUG LAND
	if ((leftLimits.posMax > 25) && (leftLimits.posMax < 50))
	{
		PORTB=0x0E;
		while(1);
	}
	
	// End calibration
	adcCalibrationFlag = 0;
	PORTD = (RIGHT_MOTOR_STOP) | (LEFT_MOTOR_STOP);

}

void waitValueIsReady(void)
{
	//adcCompletedFlag = 0;		
	while (adcCompletedFlag == 0){}

	adcCompletedFlag = 0;
	while (adcCompletedFlag == 0){}


}

