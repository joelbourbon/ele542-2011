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
	// Complete configuration of the timer1
	// Note : This is too much but education oriented
	//
	mTop_Value = 9999;
	
	TCCR1A = (1 << COM1A1)  // 
	       | (0 << COM1A0)  // Clear on compare A & Set at bottom
		     | (1 << COM1B1)  //			
		     | (0 << COM1B0)  // Clear on compare B & Set at bottom
		     | (1 << WGM11 )  // 
		     | (0 << WGM10 ); // Fast PWM mode
			
	TCCR1B = (1 << WGM12)   // 
	       | (1 << WGM13)   // Fast PWM Mode
         | (0 << CS12 )   // 
         | (1 << CS11 )   // 
         | (0 << CS10 );  // Prescaler 8
		 
	ICR1H  = 0x27;
	ICR1L  = 0x0F;
		
	OCR1AH = 0x00;
	OCR1AL = 0x20;  // Compare value A initially 0
	
	OCR1BH = 0x00;
	OCR1BL = 0x20;  // Compare value B initially 0
	
	PORTD |= (1 << DDD4) | (1 << DDD5); // Ensure compare output pins are set as outputs
	
	//TIMSK |= (0 << TICIE1)
	      //|  (0 << OCIE1A)
	      //|  (0 << OCIE1A)
	      //|  (0 << TOIE1); // All interrupts OFF
}

// PWMB = Left motor
void timer1::setCompareValueLeft( uint16_t iValue )
{
	OCR1BH = (uint8_t) (iValue >> 7);
	OCR1BL = (uint8_t) (iValue & 0x00FF);
}

// PWMA = Right motor
void timer1::setCompareValueRight( uint16_t iValue )
{
	OCR1AH = (uint8_t) (iValue >> 7);
	OCR1AL = (uint8_t) (iValue & 0x00FF);
}
