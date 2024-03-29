/************************************************************************/
/*  Title       : led.h                                                 */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Mass�                      */
/*	                                                                    */
/*  Summary     : Include file for the led                              */
/*                                                                      */
/************************************************************************/

#ifndef __LED_H_
#define __LED_H_

#include <stdint.h>
#include "core/PIN/pin.h"

#define INSTANCEDELAY 20

enum LedType
{
	Solid    = 0,
	Flashing = 1,
	Instance = 2
};

class led : public pin
{	
public:
  led(volatile uint8_t *iPORTX, volatile uint8_t *iDDRX, uint8_t iPinNumber, LedType iType);
  led();
	
	// Output Methods
	void ON();
	void OFF();
	void activateLED();
	void deactivateLED();
	
	LedType  Type;
	uint64_t NextEventTime;
	uint8_t  Active;
	
};

#endif //__LED_H_
