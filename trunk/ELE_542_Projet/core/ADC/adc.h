//************************************************************************/
/*  Title       : adc.h                                          */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Include file for the normal mode                      */
/*                                                                      */
/************************************************************************/


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "singleton.h"
class adc{
	private:
	//friend void __vector_16();
	
	public:
	adc();
	volatile uint16_t sum_motorA;
	volatile uint8_t nb_measureA;
	volatile uint16_t sum_motorB;
	volatile uint8_t nb_measureB;
};
#endif /* ADC_H_ */