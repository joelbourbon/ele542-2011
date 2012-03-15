/************************************************************************/
/*  Title       : leddriver.cpp                                         */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Managament of all the leds in one class  (task)       */
/*                                                                      */
/************************************************************************/

#include "leddriver.h"
#include "singleton.h"

leddriver::leddriver():
    Led1(&PORTB, &DDRB, PINB0, Flashing)
  , Led2(&PORTB, &DDRB, PINB1, Instance)
  , Led3(&PORTB, &DDRB, PINB2, Solid)
  , Led4(&PORTB, &DDRB, PINB3, Instance)
  , Led5(&PORTB, &DDRB, PINB4, Solid)
  , Led6(&PORTB, &DDRB, PINB5, Instance)
  , Led7(&PORTB, &DDRB, PINB6, Solid)
  , Led8(&PORTB, &DDRB, PINB7, Solid)
  , mNextEventTime(0)
{
  LedList[0] = &Led1;
  LedList[1] = &Led2;
  LedList[2] = &Led4;
  LedList[3] = &Led6;
}

//
//  Task to be called to refresh the status of the automatic leds
//
void leddriver::refreshLEDsState()
{
	if (mNextEventTime > s.Timer1.time_ms)
	  return;

	mNextEventTime = s.Timer1.time_ms + PROCESS_DELAY;
	
	for (int i = 0; i <= 3; i++)
	{
		led* wLed= LedList[i];
		uint32_t wActualTime = s.Timer1.time_ms;

		if (wLed->Active == 1 && wLed->NextEventTime < wActualTime)
		{
		  if(wLed->Type == Flashing)
		  {
		  	wLed->toggle();
			  wLed->NextEventTime = wActualTime + (FLASHINGDELAY);
		  }			  
		  else if (wLed->Type == Instance)
		    wLed->deactivateLED();	
		}
	}	
}