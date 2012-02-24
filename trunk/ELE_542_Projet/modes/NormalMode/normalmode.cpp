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
  //s.Uart.TX_Buffer.push(0xFE);
  //s.Uart.LoopBackOn = false;
}

void NormalMode::loop()
{
	// TEST THE CIRCULAR BUFFER  
  //_delay_ms(2000);
  //
  //trame TEST;
  //TEST = s.Uart.RX_Buffer.pull(); 
  //s.Uart.TX_Buffer.push(TEST.Commande);
  //s.Uart.TX_Buffer.push(TEST.Vitesse);
  //s.Uart.TX_Buffer.push(TEST.Angle);
  //s.Uart.TX_Buffer.sendData();

  //s.uart.LoopBackOn = true;
  
  trame TEST;
  TEST = s.Uart.RX_Buffer.pull();
  
  s.Moteur.CalculPWM(TEST.Vitesse, TEST.Angle, 0, 0);  
}		