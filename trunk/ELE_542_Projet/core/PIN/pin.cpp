/************************************************************************/
/*  Title       : pin.cpp                                               */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Class to manage pins                                  */
/*                                                                      */
/************************************************************************/

#include "pin.h"


pin::pin(volatile uint8_t *iPORTX, volatile uint8_t *iDDRX, volatile uint8_t *iPINX, uint8_t iPinNumber, PINDirection iPinDirection)
{
  PORTX     = iPORTX;
  DDRX      = iDDRX;
  PINX      = iPINX;
  PINNumber = iPinNumber;
  Direction = iPinDirection;
  
  // Set direction of the PIN
  *DDRX = (*DDRX & ~(1 << PINNumber))    // Ensure a 0 in the pin position
        | (iPinDirection << PINNumber);  // Put the good value to set direction
		
	// Clear the pin by default if in Output Mode
	if(iPinDirection == PinOutput)
	  *PORTX &= ~(1 << PINNumber);
}

pin::pin(volatile uint8_t *iPORTX, volatile uint8_t *iDDRX, uint8_t iPinNumber, PINDirection iPinDirection)
{
  PORTX     = iPORTX;
  DDRX      = iDDRX;
  PINNumber = iPinNumber;
  Direction = iPinDirection;
  
  // Set direction of the PIN
  *DDRX = (*DDRX & ~(1 << PINNumber))    // Ensure a 0 in the pin position
        | (iPinDirection << PINNumber);  // Put the good value to set direction
		
	// Clear the pin by default if in Output Mode
	if(iPinDirection == PinOutput)
	  *PORTX &= ~(1 << PINNumber);
}

pin::pin()
{
	
}

//
//  Put the pin in High state (Output)
//
void pin::setPIN()
{
	*PORTX |= (1 << PINNumber);
}	
	
//
//  Put the pin in Low state (Output)
//
void pin::clearPIN()
{
	*PORTX &= ~(1 << PINNumber);
}	

//
//  Toggle the pin state (low/high) (Output)
//
void pin::toggle()
{		   
	*PORTX ^= (1 << PINNumber);
}

//
//  Get the state of the pin (low/high) (Input)
//
PinState pin::getState()
{
  return (*PINX & (1 << PINNumber)) > 0 ? High : Low ;
}

