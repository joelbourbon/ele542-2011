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

timer0::timer0()
{
	//
	// Complete configuration of the timer0
	// Note : This is too much but education oriented
	//
		mTop_Value = 9999;
		time_us=0;
    time_ms=0;
		
	TCCR0 = (1 << FOC0)  // Force on compare
	      | (1 << WGM01)   // 
	      | (0 << WGM00)   // CTC mode
	      | (0 << COM01)   // 
	      | (0 << COM00)   // Disconnected output compare
	      | (1 << CS00)   // 
	      | (0 << CS01)   // 
	      | (0 << CS02);   // Prescaler to 1

	OCR0 = 0xC7;
	
	ICR1H  = mTop_Value >> 7;
	ICR1L  = mTop_Value & 0x00FF;  // TOP value to 9999
	
	TIMSK |= (0 << TOIE0)
	      | (1 << OCIE0);
		  


	   
}
void timer0:: interupt_conversion(){
	if(time_us > 39) {
		time_ms+=1;
		time_us -= 40;
	} else
	time_us += 25;
	
}

/*ISR(TIMER0_COMP_vect){
	interupt_conversion();
}*/
