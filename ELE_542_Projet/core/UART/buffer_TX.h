/************************************************************************/
/*  Title       : buffer.h                                              */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Reception of data byte per byte plus organized pull   */
/*                                                                      */
/************************************************************************/

#ifndef BUFFER_RCPT_H_
#define BUFFER_RCPT_H_

#include <stdint.h>
#include <avr/io.h>
#include <string.h>

class Buffer_Envoi{

private:
  uint8_t pile[128];
  
  void sendData();
  
public:
  void push(char iByte);
  void Print(const char* iString);
  char pull();
  uint8_t countData;
};




#endif /* BUFFER_RCPT_H_ */