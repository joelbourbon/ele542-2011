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
#include <avr/interrupt.h>

#define TAILLE_TX_BUFFER 128

class Buffer_Envoi{

private:
  uint8_t pile[TAILLE_TX_BUFFER];
  uint8_t lecture;
  uint8_t ecriture;  
  
public:
  void Print(const char* iString, uint8_t inDEBUG);
  char pull();
  void push(char iByte);
  void sendData();
  uint8_t countData;
};




#endif /* BUFFER_RCPT_H_ */