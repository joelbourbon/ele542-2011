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
	//, DIR_G1(PORTD, DDRD, PIND, PIND2, PinOutput)
	//, DIR_G2(PORTD, DDRD, PIND, PIND3, PinOutput)
	//, DIR_D1(PORTD, DDRD, PIND, PIND6, PinOutput)
	//, DIR_D2(PORTD, DDRD, PIND, PIND7, PinOutput)
{
  PORTD = (1 << PIND2) | (1 << PIND3) | (1 << PIND6) | (1 << PIND7);
  DDRD  = 0x00;
}	  