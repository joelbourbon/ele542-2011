/************************************************************************/
/*  Title       : buffer.cpp                                            */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Reception of data byte per byte plus organized pull   */
/*                                                                      */
/************************************************************************/


#include "buffer_TX.h"

//
//  Push a byte in the buffer
//
void Buffer_Envoi::push(char iByte)
{
  ++countData;
	pile[ecriture] = iByte;
  
  ecriture++;
  if(ecriture >= TAILLE_TX_BUFFER)
    ecriture = 0;    
}

//
//  Pull the next byte from the buffer to send
//
char Buffer_Envoi::pull()
{
  --countData;
	uint8_t data =  pile[lecture];
  lecture++;
  if(lecture >= TAILLE_TX_BUFFER)
    lecture = 0;
	return data;
}

//
//  Simplified function to send the next byte in the UART
//
void Buffer_Envoi::sendData()
{
	UDR = pull();
}

//
//  Simplified function to send a string in the UART async
//  Once started, it's interrupt managed
//
void Buffer_Envoi::Print(const char* iString, uint8_t inDEBUG = 0)
{
  cli();
  
  if(1 == inDEBUG)
    push(0xFE);
	
	while(*iString != 0)
		push(*iString++);
		
  if(1 == inDEBUG)
    push(0xFF);
	
  sei();
  sendData();
}	