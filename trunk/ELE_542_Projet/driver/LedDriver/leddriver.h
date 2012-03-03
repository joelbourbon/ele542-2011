/*
 * leddriver.h
 *
 * Created: 2012-03-03 12:56:57
 *  Author: Olivier
 */ 


#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_

#include <stdint.h>
#include "core/LED/led.h"

#define FLASHINGDELAY 200

class leddriver
{
private:
	led* LedList[8];
	led Led1;
	led Led2;
	led Led3;
	led Led4;
	led Led5;
	led Led6;
	led Led7;
	led Led8;
	
public:
  leddriver();
	void refreshLEDsState();
};

#endif /* LEDDRIVER_H_ */