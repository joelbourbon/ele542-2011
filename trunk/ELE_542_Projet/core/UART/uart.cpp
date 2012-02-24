/************************************************************************/
/*  Title       : uart.cpp                                              */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Class that creates the Uart of the µC                 */
/*                                                                      */
/************************************************************************/


#include <singleton.h>
#include "uart.h"

uart::uart()
{
	//
	// Complete configuration of the UART
	// Note : This is too much but education oriented
	//
	LoopBackOn = true;
	
	// USART Control and Status Register C
	UCSRC = (1 << URSEL)  // Programming UCSRC
	      | (0 << UMSEL)  // Asynchronous mode
		    | (0 << UPM1 )
		    | (0 << UPM0 )  // Parity mode set to : Enabled, Even Parity
		    | (0 << USBS )  // 1 stop bit
		    | (1 << UCSZ1)
		    | (1 << UCSZ0)  // Character size set to 8 bit
		    | (0 << UCPOL); // Not used on asynchronous

	// USART Baud Rate Registers
	UBRRL = 103;          // 9600 Baud rate with U2X = 1 and Oscillator of 16 MHz ==> 2Mbps
  
	// USART Control and Status Register B
	UCSRB = (1 << RXCIE)  // Reception Interrupt Enabled
		    | (1 << TXCIE)   // Transmission Interrupt Enabled
		    | (0 << UDRIE)   // USART Data Register Empty Interrupt Enable
		    | (1 << RXEN )   // USART Reception Enabled
		    | (1 << TXEN )   // USART Transmission Enabled
		    | (0 << UCSZ2);  // Character size set to 8 bit (See UCSRC UCSZ0 & UCSZ1) 
  // RXB8 & TXB8 are used as Read/Write register for 9 bits long messages (With UDR)
}

void uart::uart_loopback(uint8_t iByte)
{
	RX_Buffer.push(iByte);  // Store dans le buffer du UART
  if(LoopBackOn)
	  UDR = iByte;            // Loop back directly to UDR to TX
}

ISR(USART_RXC_vect)
{
	s.Uart.wReceivedByte = UDR;
	s.Uart.uart_loopback(s.Uart.wReceivedByte);
}

ISR(USART_TXC_vect)
{
	if(s.Uart.TX_Buffer.countData > 0)
	  UDR = s.Uart.TX_Buffer.pull();
}	