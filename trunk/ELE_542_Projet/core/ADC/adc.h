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
#include "stdio.h"

extern "C" void __vector_16();

#define MUXMASK 0b00011111

union adc_register{
	
	uint16_t adc_value;
	uint8_t adc[2];
	
};

class adc{
private:
	friend void __vector_16();
	int64_t  ADC_period_in_ms;
  uint64_t ADC_last_process_time_ms;
  void averageADC();
	
public:
	adc();
  void processAverageADC();
  
	uint16_t sum_motor_left;
	uint8_t nb_measure_left;
	uint16_t sum_motor_right;
	uint8_t nb_measure_right;
	volatile adc_register adc1;
	volatile uint8_t first;

};
#endif /* ADC_H_ */