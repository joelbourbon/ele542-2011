/************************************************************************/
/*  Title       : initcore.cpp                                          */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Inits all the core systems used in modes              */
/*                                                                      */
/************************************************************************/


#include "initcore.h"

CoreInit::CoreInit() :
    Uart()
	, Timer0()
	, Timer1()
	, Moteur()
	, DataStore()
	, ADC1()
	, Watchdog()
	, StartButton(&PORTA, &DDRA, &PINA, PINA6, PinInput)
	, StopButton(&PORTA, &DDRA, &PINA, PINA7, PinInput)
{
}	  