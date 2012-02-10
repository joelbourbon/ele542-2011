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
	pile[countData] = iByte;
	++countData;
	
	if (countData == 1)
	  sendData();
}	

char Buffer_Envoi::pull()
{
	uint8_t data =  pile[countData];
	--countData;
	return data;
}

void Buffer_Envoi::sendData()
{
	UDR = pull();
}

void Buffer_Envoi::Print(const char* iString)
{
	while(*iString != 0)
		push(*iString++); 
		  
}	