/************************************************************************/
/*  Title       : pin.h                                                 */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Include file for the pin                              */
/*                                                                      */
/************************************************************************/

#ifndef __PIN_H_
#define __PIN_H_

#include <stdint.h>

typedef enum
{
	PinInput   = 0,
	PinOutput  = 1
}PINDirection;

typedef enum
{
	Low   = 0,
	High  = 1
}PinState;

class pin
{
private:
  volatile uint8_t      *PINX;
  volatile uint8_t      *PORTX;
  volatile uint8_t      *DDRX;
  uint8_t       PINNumber;
  PINDirection  Direction;
	
public:
  pin(volatile uint8_t *iPORTX, volatile uint8_t *iDDRX, volatile uint8_t *iPINX, uint8_t iPinNumber, PINDirection iPinDirection);
  pin(volatile uint8_t *iPORTX, volatile uint8_t *iDDRX, uint8_t iPinNumber, PINDirection iPinDirection);
  pin();
	
	// Output Methods
	void toggle();
	void setPIN();
	void clearPIN();
	
	// Inputs Methods
  PinState getState();
  
};

#endif //__PIN_H_
