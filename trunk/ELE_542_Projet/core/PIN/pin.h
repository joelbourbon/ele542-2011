/**
	@file moteur.h
	@brief Header pour le fichier moteur.c
	@author Bruno De Kelper et Louis-Bernard Lagueux
	@date 13 Octobre 2006
*/
#ifndef __PIN_H_
#define __PIN_H_

#include <stdint.h>

typedef enum
{
	PinInput   = 0,
	PinOutput  = 1
}PINDirection;

class pin
{
private:
  uint8_t      *PORTX;
  uint8_t      *DDRX;
  uint8_t      *PINX;
  uint8_t       PINNumber;
  PINDirection  Direction;
	
public:
  pin(uint8_t *iPORTX, uint8_t *iDDRX, uint8_t *iPINX, uint8_t iPinNumber, PINDirection iPinDirection);
	
	// Output Methods
	void setPIN();
	void clearPIN();
	void toggle();
	
	// Input Methods
	
};

#endif //__PIN_H_
