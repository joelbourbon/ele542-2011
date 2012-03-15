/************************************************************************/
/*  Title       : leddriver.h                                           */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Include file for the leddriver                        */
/*                                                                      */
/************************************************************************/

#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_

#include <stdint.h>
#include "core/LED/led.h"

#define FLASHINGDELAY 200
#define PROCESS_DELAY 50

class leddriver
{
private:
	led* LedList[4];
	
public:
  leddriver();
	void refreshLEDsState();
	
	led Led1;
	led Led2;
	led Led3;
	led Led4;
	led Led5;
	led Led6;
	led Led7;
	led Led8;
	
	uint64_t mNextEventTime;
};

#endif /* LEDDRIVER_H_ */