/************************************************************************/
/*  Title       : leddriver.cpp                                               */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     :        */
/*                                                                      */
/************************************************************************/

#include "leddriver.h"
#include "singleton.h"

leddriver::leddriver():
    Led1(&PORTB, &DDRB, PINB0, Solid)
  , Led2(&PORTB, &DDRB, PINB1, Flashing)
  , Led3(&PORTB, &DDRB, PINB2, Instance)
  , Led4(&PORTB, &DDRB, PINB3, Flashing)
  , Led5(&PORTB, &DDRB, PINB4, Instance)
  , Led6(&PORTB, &DDRB, PINB5, Flashing)
  , Led7(&PORTB, &DDRB, PINB6, Solid)
  , Led8(&PORTB, &DDRB, PINB7, Solid)
{
  LedList[0] = &Led1;
  LedList[1] = &Led7;
  LedList[2] = &Led8;
  LedList[3] = &Led2;
  LedList[4] = &Led3;
  LedList[5] = &Led4;
  LedList[6] = &Led5;
  LedList[7] = &Led6;
}

void leddriver::refreshLEDsState()
{
	for (int i = 3; i < 7; i++)
	{
		led* wLed= LedList[i];
		uint32_t wActualTime = s.Timer0.get_us_time();

		if (wLed->Active == 1 && wLed->NextEventTime < wActualTime)
		{
		  if(wLed->Type == Flashing)
		  {
		  	wLed->toggle();
			  wLed->NextEventTime = wActualTime + (FLASHINGDELAY * 1000);
		  }			  
		  else if (wLed->Type == Instance)
		    wLed->deactivateLED();	
		}
	}	
}