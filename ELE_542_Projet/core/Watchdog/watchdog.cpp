/************************************************************************/
/*  Title       : watchdog.cpp                                          */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Contains the init watchdog setup                      */
/*                                                                      */
/************************************************************************/

#include <singleton.h>
#include <avr/wdt.h>
#include "watchdog.h"


watchdog::watchdog()
{
	WDTCR = ( 0<< WDE)  // Watchdog Enabled
	        | (1<<WDP2) // WDP are used to set the period of the watchdog timer-> 1s
			    | (1<<WDP1)
			    | (0<<WDP0);
}

//
//  Reset the timer counting down to reset
//
void watchdog::reset()
{
	wdt_reset();
}

//
//  Enable the watchdog
//
void watchdog::enable()
{
	WDTCR = 1 << WDE;
}

//
//  Disable the watchdog
//
void watchdog::disable()
{
  wdt_disable();
}  