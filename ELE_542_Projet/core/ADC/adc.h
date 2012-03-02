//************************************************************************/
/*  Title       : adc.h                                                 */
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


extern "C" void __vector_16();

#define MUXMASK 0b00011111

union adc_register{
	
	uint16_t adc_value;
	uint8_t adc[2];
	
};

class adc{
private:
	friend void __vector_16();
	
public:
	adc();
	volatile uint16_t sum_motor_left;
	volatile uint8_t nb_measure_left;
	volatile uint16_t sum_motor_right;
	volatile uint8_t nb_measure_right;
	volatile adc_register adc1;
	volatile uint8_t first;
};
#endif /* ADC_H_ */