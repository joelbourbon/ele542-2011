/************************************************************************/
/*  Title       : timer1.cpp                                            */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Class that creates the timer for PWN to control the   */
/*                motors of the µC                                      */
/************************************************************************/


#include <singleton.h>
#include "timer1.h"

timer1::timer1()
{
	//
	// Complete configuration of the timerr1
	// Note : This is too much but education oriented
	//
	
	TCCR1A = (1 << COM1A1)  // 
	      | (0 << COM1A0)  // 
		    | (1 << COM1B1) //
		    | (0 << COM1B0)  // 
		    | (1 << WGM11)  // 
		    | (0 << WGM10) ; //
			
	TCCR1B = (1 << WGM12)  // 
	      | (1 << WGM13)  // 
        | (0 << CS12)  // 
        | (0 << CS11)  // 
        | (1 << CS10);  // 
		
	OCR1AH = 0x00;
	OCR1AL = 0x00;
	OCR1BH = 0x00;
	OCR1BL = 0x00;
	
	TIMSK = (0 << TICIE1)
	      | (0 << OCIE1A)
	      | (0 << OCIE1A)
	      | (0 << TOIE1);
}

// PWMB = Left motor
void timer1::setCompareValueLeft( uint16_t iValue )
{
	OCR1BH = (iValue & 0xFF00);
	OCR1BL = (iValue & 0x00FF);
}

// PWMA = Left motor
void timer1::setCompareValueRight( uint16_t iValue )
{
	OCR1AH = (iValue & 0xFF00);
	OCR1AL = (iValue & 0x00FF);
}
