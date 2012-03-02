/************************************************************************/
/*  Title       : uart.h                                                */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Class that creates the Uart of the µC                 */
/*                                                                      */
/************************************************************************/


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "buffer_RX.h"
#include "buffer_TX.h"

extern "C" void __vector_13(); // ISR(USART_RXC_vect)
extern "C" void __vector_15(); // ISR(USART_RXC_vect)

class uart
{
	
private:
	// Interrupt for the USART_RXC_vect
	friend void __vector_13();
	friend void __vector_15();
	volatile uint8_t wReceivedByte;
	
public:
  // Constructor
  bool LoopBackOn;
  uart();
  Buffer_Reception  RX_Buffer;
  Buffer_Envoi      TX_Buffer;
  
  // Transmit byte
  void uart_loopback(uint8_t iByte);
  void printDebug(const char* iString);
  void printString(const char* iString);

};

#endif /* UART_H_ */