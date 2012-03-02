/*
 * AverageADC.cpp
 *
 * Created: 2012-02-29 06:00:13
 *  Author: Joel
 */ 

#include "tasks/AverageADC.h"

void processAverageADC()
{
	uint32_t current_time_us = s.Timer0.get_us_time();

	if (int32_t(current_time_us) - int32_t(last_process_time_us) < (period_in_us))
		return;

	last_process_time_us += (period_in_us);

	averageADC();
}

void averageADC()
{
	s.DataStore.MoyenneADCDroit   = (float)s.ADC1.sum_motor_right / (float)s.ADC1.sum_motor_right;
	s.DataStore.MoyenneADCGauche  = (float)s.ADC1.sum_motor_left / (float)s.ADC1.sum_motor_left;
	
	// TEST DE TASKER
	char mBuffer[20];
	char* mSender = &mBuffer[0];
	sprintf(mBuffer, "TEST : %i", last_process_time_us);
	s.Uart.printString(mBuffer);
}	