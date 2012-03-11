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

timer1::timer1():
  mTop_Value(9999)
{
	//
	// Complete configuration of the timer1
	// Note : This is too much but education oriented
	//	
	TCCR1A = 0;
	ICR1  = mTop_Value;
			
	TCCR1B = (1 << WGM12)   // 
	       | (1 << WGM13)   // Fast PWM Mode
         | (0 << CS12 )   // 
         | (1 << CS11 )   // 
         | (0 << CS10 );  // Prescaler 8
		 
	TCCR1A = (1 << COM1A1)  // 
	       | (0 << COM1A0)  // Clear on compare A & Set at bottom
		     | (1 << COM1B1)  //			
		     | (0 << COM1B0)  // Clear on compare B & Set at bottom
		     | (1 << WGM11 )  // 
		     | (0 << WGM10 ); // Fast PWM mode
	
	PORTD |= (1 << DDD4) | (1 << DDD5); // Ensure compare output pins are set as outputs
		
	OCR1A = mTop_Value / 2;
	
	OCR1B = mTop_Value / 2;
}

// PWMB = Left motor
void timer1::setCompareValueLeft( uint16_t iValue )
{
	OCR1BH = iValue >> 7;
	OCR1BL = iValue & 0x00FF;
}

// PWMA = Right motor
void timer1::setCompareValueRight( uint16_t iValue )
{
	OCR1AH = iValue >> 7;
	OCR1AL = iValue & 0x00FF;
}
