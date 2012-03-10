/************************************************************************/
/*  Title       : timer0.cpp                                            */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Class that creates the timer for PWN to control the   */
/*                motors of the µC                                      */
/************************************************************************/


#include <singleton.h>
#include "timer0.h"

timer0::timer0():
    mTop_Value(999)
	, time_us(0)
  , time_ms(0)
{
	//
	// Complete configuration of the timer0
	// Note : This is too much but education oriented
	//		
	TCCR0 = (0 << FOC0)    // Force on compare
	      | (1 << WGM01)   // 
	      | (0 << WGM00)   // CTC mode
	      | (0 << COM01)   // 
	      | (0 << COM00)   // Disconnected output compare
	      | (0 << CS00)    // 
	      | (1 << CS01)    // 
	      | (0 << CS02);   // Prescaler to 8

	OCR0 = 50;
	
	TIMSK |= (0 << TOIE0)
	      |  (1 << OCIE0);	   
}

uint64_t timer0::get_ms_time()
{
  return time_ms;
}	

ISR(TIMER0_COMP_vect){
	if(s.Timer0.time_us > s.Timer0.mTop_Value)
	{
		s.Timer0.time_ms += 1;
		s.Timer0.time_us  = 0;
	}
	
	else
	  s.Timer0.time_us += 25;
}
