/************************************************************************/
/*  Title       : watchdog.cpp                                        */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Contains the init and loop for the normal mode        */
/*                                                                      */
/************************************************************************/

#include <singleton.h>
#include <avr/wdt.h>
#include "watchdog.h"


watchdog::watchdog()
{
	WDTCR = ( 1<< WDE)  // Watchdog Enabled
	        | (1<<WDP2) // WDP are used to set the period of the watchdog timer-> 1s
			    | (1<<WDP1)
			    | (0<<WDP0);
}

void watchdog::reset()
{
	wdt_reset();
}