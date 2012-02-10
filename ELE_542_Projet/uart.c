/*
 * uart.c
 *
 * Created: 2012-01-27 12:30:55
 *  Author: Joel
 */ 

#include "uart.h"

Uart::Uart()
{
	//
	// Complete configuration of the UART
	// Note : This is too much but education oriented
	//
	
	// USART Control and Status Register C
	UCSRC |=(1 << URSEL)  // Programming UCSRC
	      | (0 << UMSEL)  // Asynchronous mode
		    | (0 << UPM1 )
		    | (0 << UPM0 )  // Parity mode set to : Enabled, Even Parity
		    | (0 << USBS )  // 1 stop bit
		    | (1 << UCSZ1)
		    | (1 << UCSZ0)  // Character size set to 8 bit
		    | (0 << UCPOL); // Not used on asynchronous

	// USART Baud Rate Registers
	UBRRL |= 103;          // 9600 Baud rate with U2X = 1 and Oscillator of 16 MHz ==> 2Mbps
  
	// USART Control and Status Register B
	UCSRB |=(1 << RXCIE)  // Reception Interrupt Enabled
		    | (1 << TXCIE)   // Transmission Interrupt Enabled
		    | (1 << UDRIE)   // USART Data Register Empty Interrupt Enable
		    | (1 << RXEN )   // USART Reception Enabled
		    | (1 << TXEN )   // USART Transmission Enabled
		    | (0 << UCSZ2);  // Character size set to 8 bit (See UCSRC UCSZ0 & UCSZ1) 
  // RXB8 & TXB8 are used as Read/Write register for 9 bits long messages (With UDR)
}	

ISR(USART_RXC_vect)
{
	unsigned char received;
	received = UDR;
	uart_transmit(UDR);
}

void Uart::uart_transmit_byte(uint8_t iByte)
{
	UDR = iByte;
}