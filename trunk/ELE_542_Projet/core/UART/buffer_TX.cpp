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

void Buffer_Envoi::push(char iByte)
{
  ++countData;
	pile[ecriture] = iByte;
  
  ecriture++;
  if(ecriture >= TAILLE_TX_BUFFER)
    ecriture = 0;    
}	

char Buffer_Envoi::pull()
{
  --countData;
	uint8_t data =  pile[lecture];
  lecture++;
  if(lecture >= TAILLE_TX_BUFFER)
    lecture = 0;
	return data;
}

void Buffer_Envoi::sendData()
{
	UDR = pull();
}

void Buffer_Envoi::Print(const char* iString)
{
  cli();
  push(0xFE);
	while(*iString != 0)
		push(*iString++);
  push(0xFF);
  sei();
  sendData();
}	