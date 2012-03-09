/************************************************************************/
/*  Title       : adc.cpp                                               */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Contains the init and loop for the normal mode        */
/*                                                                      */
/************************************************************************/

#include <singleton.h>
#include "adc.h"


adc::adc():
    first(1)
  , ADC_period_in_ms(1000)
{
	
	ADMUX = (0 << REFS1)      // 
	      | (0 << REFS0)      // Reference selection Bits: AREF, internal Vref turned off
        | (0 << ADLAR)     //
		    | (0 << MUX4)      //
		    | (0 << MUX3)      //
		    | (0 << MUX2)      //
		    | (0 << MUX1)       //  Analog Channel and gain selection bits
		    | (0 << MUX0);      // 
  
  SFIOR = (0 << ADTS2)      // Free Running mode
		    | (0 << ADTS1)      // 
		    | (0 << ADTS0);     // 
			
	ADCSRA = (1 << ADEN)      // ADC Enable
		     | (1 << ADSC)       // ADC Start Conversion
		     | (0 << ADATE)      // Auto trigger Disabled
		     | (1 << ADIE);     // ADC Interrupt Enabled 
  
}

ISR(ADC_vect){
	
	if(1==s.ADC1.first){
		s.ADC1.first=0;
		ADMUX^=(1<<0);
	}
	
  else
  {
	  s.ADC1.adc1.adc[0]=ADCL;
	  s.ADC1.adc1.adc[1]=ADCH;
	  
	 
	  
    if((ADMUX & MUXMASK) == 0){
      ADMUX&=~(1<<0);  
	    s.ADC1.sum_motor_left+=s.ADC1.adc1.adc_value;
		  s.ADC1.nb_measure_left++;
    }
  	 
    else{
      ADMUX|=(1<<0);  
	    s.ADC1.sum_motor_right+=s.ADC1.adc1.adc_value;
      s.ADC1.nb_measure_right++;
    }
  }
}

void adc::processAverageADC()
{
	uint32_t current_time_ms = s.Timer0.get_ms_time();

	if ((int32_t(current_time_ms) - int32_t(ADC_last_process_time_ms)) < ADC_period_in_ms)
		return;

	ADC_last_process_time_ms += (ADC_period_in_ms);
	
	// TEST DE TASKER
	char mBuffer[20];
	sprintf(mBuffer, "%u\r\n", /*ADC_last_process_time_us - */s.Timer0.time_ms);
	s.Uart.printString(mBuffer);

	//averageADC();
}

void adc::averageADC(){
	
	s.DataStore.MoyenneADCDroit   = (float)s.ADC1.sum_motor_right / (float)s.ADC1.sum_motor_right;
	s.DataStore.MoyenneADCGauche  = (float)s.ADC1.sum_motor_left / (float)s.ADC1.sum_motor_left;
	
}