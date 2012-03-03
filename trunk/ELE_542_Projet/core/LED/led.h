/**
	@file moteur.h
	@brief Header pour le fichier moteur.c
	@author Bruno De Kelper et Louis-Bernard Lagueux
	@date 13 Octobre 2006
*/

#ifndef __LED_H_
#define __LED_H_

#include <stdint.h>
#include "core/PIN/pin.h"

#define INSTANCEDELAY 400

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
	uint32_t NextEventTime;
	uint8_t  Active;
	
};

#endif //__LED_H_
