/************************************************************************/
/*  Title       : adc.cpp                                               */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Contains the init and methods for the ADC             */
/*                                                                      */
/************************************************************************/

#include <singleton.h>
#include "adc.h"

adc::adc():
    ADC_period_in_ms(200)
	, ADC_last_process_time_ms(0)
	, rightADCAverage(0)
	, leftADCAverage(0)
	, sum_motor_left(0)
	, nb_measure_left(0)
	, sum_motor_right(0)
	, nb_measure_right(0)
	, directionMotorLeft(&PORTA, &DDRA, &PINA, PINA2, PinInput)
	, directionMotorRight(&PORTA, &DDRA, &PINA, PINA3, PinInput)
	, calibrationADC(&PORTA, &DDRA, &PINA, PINA4, PinOutput)
	, max_pos_left(0)
	, max_pos_right(0)
	, max_neg_left(0)
	, max_neg_right(0)
	, zero_pos_left(0)
	, zero_pos_right(0)
	, zero_neg_left(0)
	, zero_neg_right(0)
{
	calibrationADC.clearPIN();
	
	ADCSRA |= (1 << ADPS2)     // 
		     |  (1 << ADPS1)     // 
		     |  (1 << ADPS0)     // Prescaler set to 128 --> 125 kHz	
         |  (1 << ADEN)      // ADC Enable
		     |  (1 << ADSC)      // ADC Start Conversion
		     |  (1 << ADATE)     // Auto trigger Enabled		     
		     |  (1 << ADIE);     // ADC Interrupt Enabled
			 
	DDRA = ~((1 << DDA0) | (1 << DDA1));
}

//
//  End of sample interruption handling
//
ISR(ADC_vect)
{	
	// Read the data from the register and save to more practical UNION
	s.ADC1.adcRegister = ADC;
		  
	// If returning result for Channel 0 (Left motor)
  if((ADMUX & MUXMASK) == 0)
	{
		// Change the channel to 0 for the next interrupt
    ADMUX |=  (1 << 0);
	  s.ADC1.sum_motor_left += s.ADC1.adcRegister;
	  s.ADC1.nb_measure_left++;
  }
	  	 
	// If returning result for Channel 1 (Right motor)
  else
  {
		// Change the channel to 1 for the next interrupt
    ADMUX &= ~(1 << 0); 
    s.ADC1.sum_motor_right += s.ADC1.adcRegister;
    s.ADC1.nb_measure_right++;
  }  
}

//
//  Average Task that execute itself only when it's time
//
void adc::processAverageADC()
{
	uint64_t current_time_ms = s.Timer1.time_ms;

	if ((int64_t(current_time_ms) - int64_t(ADC_last_process_time_ms)) < ADC_period_in_ms)
		return;

	ADC_last_process_time_ms += (ADC_period_in_ms);

	averageADC();
}

//
//  Actual calculation of the averages of all the data gathered for each channel
//
void adc::averageADC()
{
	// Stop the interruptions to gather the data and do the long calculation after
	cli();
	float temp_sum_motor_right  = sum_motor_right;
	float temp_sum_motor_left   = sum_motor_left;
	float temp_nb_measure_right = nb_measure_right;
	float temp_nb_measure_left  = nb_measure_left;
	sei();
	
	// If 1 --> negative
	int8_t wDirectionLeft = 1;
	int8_t wDirectionRight = 1;
	
	if(directionMotorLeft.getState()  == High)
	  wDirectionLeft  = -1;
	if(directionMotorRight.getState() == High)
	  wDirectionRight = -1;
	
	// Averages calculation
	if(temp_nb_measure_right > 0)
	{
	  rightADCAverage = ((float) temp_sum_motor_right / (float) temp_nb_measure_right) * (float) wDirectionRight;
	  sum_motor_right  = 0;
	  nb_measure_right = 0;
	}
	
	if(temp_nb_measure_left  > 0)
	{
	  leftADCAverage  = ((float) temp_sum_motor_left  / (float) temp_nb_measure_left) * (float) wDirectionLeft;
	  sum_motor_left  = 0;
	  nb_measure_left = 0;
	}
}

float adc::getAvgRightMotor()
{	  
	float wReturnValue;
	if(rightADCAverage > 0)
	{
	  wReturnValue = rightADCAverage / (float)(max_pos_right - zero_pos_right);
    if(wReturnValue <= zero_pos_right)
	    return 0.f;
		return wReturnValue;
	}
	
	else
	{
		wReturnValue = rightADCAverage / (float)(zero_neg_right - max_neg_right);
		if(wReturnValue >= zero_neg_right)
		  return 0.0f;
		return wReturnValue;
	}
}

float adc::getAvgLeftMotor()
{
	float wReturnValue;
	if(leftADCAverage > 0)
	{
	  wReturnValue = leftADCAverage / (float)(max_pos_left - zero_pos_left);
    if(wReturnValue <= zero_pos_left)
	    return 0.f;
		return wReturnValue;
	}
	
	else
	{
		wReturnValue = leftADCAverage / (float)(zero_neg_left - max_neg_left);
		if(wReturnValue >= zero_neg_left)
		  return 0.0f;
		return wReturnValue;
	}
}	

void adc::clearAverageValues()
{
	sum_motor_left   = 0;
	nb_measure_left  = 0;
	sum_motor_right  = 0;
	nb_measure_right = 0;
}	

void adc::calibrate()
{
	uint32_t waitUntil = 0;
	// All the test is done with both PWM to 0
	s.Timer1.setCompareValueLeft(0);
	s.Timer1.setCompareValueRight(0);
	
	// Vmax+
	calibrationADC.setPIN();
	s.Moteur.ChangeMotorAction(MoteurDroit,  MarcheAvant);
	s.Moteur.ChangeMotorAction(MoteurGauche, MarcheAvant);
	
	clearAverageValues();
	waitUntil = s.Timer1.time_ms + 500;
	while(waitUntil > s.Timer1.time_ms)
	  ;
	
	averageADC();
	max_pos_left  = leftADCAverage;
	max_pos_right = rightADCAverage;
	
	// Vzero+
	calibrationADC.clearPIN();
	
	clearAverageValues();
	waitUntil = s.Timer1.time_ms + 500;
	while(waitUntil > s.Timer1.time_ms)
	  ;
	
	averageADC();
	zero_pos_left  = leftADCAverage;
	zero_pos_right = rightADCAverage;
	
	// Vzero-
	s.Moteur.ChangeMotorAction(MoteurDroit,  MarcheArriere);
	s.Moteur.ChangeMotorAction(MoteurGauche, MarcheArriere);
	
	clearAverageValues();
	waitUntil = s.Timer1.time_ms + 500;
	while(waitUntil > s.Timer1.time_ms)
	  ;
	
	averageADC();
	zero_neg_left  = leftADCAverage;
	zero_neg_right = rightADCAverage;
	
	// Vmax-
	calibrationADC.setPIN();
	
	clearAverageValues();
	waitUntil = s.Timer1.time_ms + 500;
	while(waitUntil > s.Timer1.time_ms)
	  ;
	
	averageADC();
	max_neg_left  = leftADCAverage;
	max_neg_right = rightADCAverage;
	
	uint16_t wResoNeg = zero_neg_left - max_neg_left;
	uint16_t wResoPos = max_pos_left - zero_pos_left;
	
	calibrationADC.clearPIN();
	
	s.Uart.printString("CALIBRATION TERMINE\r\n");
	char mBuffer[30];
	sprintf(mBuffer, "ResGNeg = %u\r\n", wResoNeg);
	s.Uart.printString(mBuffer);
}	