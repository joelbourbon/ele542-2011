/*
 * AverageADC.h
 *
 * Created: 2012-02-29 06:00:27
 *  Author: Joel
 */ 


#ifndef AVERAGEADC_H_
#define AVERAGEADC_H_

#include "singleton.h"  

int32_t period_in_us;
uint32_t last_process_time_us;

void processAverageADC();
void averageADC();



#endif /* AVERAGEADC_H_ */