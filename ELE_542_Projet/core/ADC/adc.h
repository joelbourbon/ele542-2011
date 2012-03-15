//************************************************************************/
/*  Title       : adc.h                                                 */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Include file for the ADC                              */
/*                                                                      */
/************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "stdio.h"
#include "core/PIN/pin.h"

extern "C" void __vector_16();

#define MUXMASK 0b00011111

class adc
{
private:
	friend void __vector_16();
	void averageADC();
  void clearAverageValues();
	
	int64_t  ADC_period_in_ms;
  uint64_t ADC_last_process_time_ms;
  
  float  rightADCAverage;
  float  leftADCAverage;
	
public:
	adc();
  void processAverageADC();
  void calibrate();
  
  float getAvgLeftMotor();
  float getAvgRightMotor();
	
	// Variables used in the interrupt
	volatile  uint16_t  adcRegister;
	volatile  uint32_t  sum_motor_left;
	volatile  uint32_t  nb_measure_left;
	volatile  uint32_t  sum_motor_right;
	volatile  uint32_t  nb_measure_right;
	
	// Direction PINs
	pin directionMotorLeft;
	pin directionMotorRight;
	pin calibrationADC;
	
	// Calibration results
	uint16_t max_pos_left;
	uint16_t max_pos_right;
	uint16_t max_neg_left;
	uint16_t max_neg_right;
	uint16_t zero_pos_left;
	uint16_t zero_pos_right;
	uint16_t zero_neg_left;
	uint16_t zero_neg_right;
	
};

#endif /* ADC_H_ */