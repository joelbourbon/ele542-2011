/************************************************************************/
/*  Title       : normalmode.cpp                                        */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Contains the init and loop for the normal mode        */
/*                                                                      */
/************************************************************************/

#include "normalmode.h"

void NormalMode::init()
{
	
}

void NormalMode::loop()
{
	//trame wSend;
	//wSend = s.uart.RX_Buffer.pull();
	//
	//UDR = wSend.Commande;
	//UDR = wSend.Vitesse;
	//UDR = wSend.Angle;
	
	s.uart.uart_loopback('a');
	
}		