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
#include <util/delay.h>

#define F_CPU 16000000

void NormalMode::init()
{
  s.uart.TX_Buffer.push(0xFE);
  s.uart.LoopBackOn = false;
}

void NormalMode::loop()
{
	//trame wSend;
	//wSend = s.uart.RX_Buffer.pull();
	//
	//UDR = wSend.Commande;
	//UDR = wSend.Vitesse;
	//UDR = wSend.Angle;
  
  _delay_ms(2000);
  
  trame TEST;
  TEST = s.uart.RX_Buffer.pull(); 
  s.uart.TX_Buffer.push(TEST.Vitesse);
  s.uart.TX_Buffer.push(TEST.Angle);
  s.uart.TX_Buffer.sendData();
  
  
  
  /*//s.uart.TX_Buffer.push(TEST.Vitesse);
  //s.uart.TX_Buffer.push(TEST.Angle);
  s.uart.TX_Buffer.Print("CACA");
  s.uart.TX_Buffer.push(0xFF);


  s.uart.TX_Buffer.sendData();
  
  s.uart.LoopBackOn = true;*/
  
  
}		