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

};

#endif //__PIN_H_
