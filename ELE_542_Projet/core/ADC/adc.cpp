/************************************************************************/
/*  Title       : adc.cpp                                        */
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


adc::adc()
{
	
	ADMUX = (0 << REFS1)      // 
	      | (0 << REFS0)      // Reference selection Bits: AREF, internal Vref turned off
        | (0 << ADLAR )     //
		    | (0 << MUX4 )      //
		    | (0 << MUX3 )      //
		    | (0 << MUX2 )      //
		    | (0 << MUX1)       //  Analog Channel and gain selection bits
		    | (1 << MUX0);      // 
  
	ADCSRA = (1 << ADEN)      // ADC Enable
		    | (1 << ADSC)       // ADC Start Conversion
		    | (0 << ADATE)      // Auto trigger Disabled
		    | (1 << ADIE );     // ADC Interrupt Enabled 
  
	
  SFIOR = (0 << ADTS2)      // Free Running mode
		    | (0 << ADTS1)      // 
		    | (0 << ADTS0);     // 

}