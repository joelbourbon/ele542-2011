

#include "pin.h"


pin::pin(volatile uint8_t *iPORTX, volatile uint8_t *iDDRX, volatile uint8_t *iPINX, uint8_t iPinNumber, PINDirection iPinDirection)
{
  PORTX     = iPORTX;
  DDRX      = iDDRX;
  PINX      = iPINX;
  PINNumber = iPinNumber;
  Direction = iPinDirection;
  
  *DDRX = (*DDRX & ~(1 << PINNumber))    // Ensure a 0 in the pin position
        | (iPinDirection << PINNumber);  // Put the good value to set direction
		
	// Output --> Set (1)         Input --> Clear (0)
	if(iPinDirection == PinOutput)
	  *PORTX |=  (1 << PINNumber);
	else
	  *PORTX &= ~(1 << PINNumber);
}

pin::pin(volatile uint8_t *iPORTX, volatile uint8_t *iDDRX, uint8_t iPinNumber, PINDirection iPinDirection)
{
  PORTX     = iPORTX;
  DDRX      = iDDRX;
  PINNumber = iPinNumber;
  Direction = iPinDirection;
  
  *DDRX = (*DDRX & ~(1 << PINNumber))    // Ensure a 0 in the pin position
        | (iPinDirection << PINNumber);  // Put the good value to set direction
		
	if(iPinDirection == PinOutput)
	  *PORTX &= ~(1 << PINNumber);
}

pin::pin()
{
	
}

void pin::setPIN()
{
	*PORTX |= (1 << PINNumber);
}	
	
void pin::clearPIN()
{
	*PORTX &= ~(1 << PINNumber);
}	

void pin::toggle()
{		   
	*PORTX ^= (1 << PINNumber);
}

PinState pin::getState()
{
  return (*PINX & (1 << PINNumber)) > 0 ? High : Low ;
}

