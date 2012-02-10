/*
 * uart.h
 *
 * Created: 2012-01-27 12:31:06
 *  Author: Joel
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

extern "C" void __vector_13(); // ISR(USART_RXC_vect)

class Uart
{
private:
	// Interrupt for the USART_RXC_vect
	friend void __vector_13();	

public:
  void Uart;
  void uart_transmit_byte(uint8_t iByte);

}




#endif /* UART_H_ */