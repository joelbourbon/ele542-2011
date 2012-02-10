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

#ifndef BUFFER_RX_H_
#define BUFFER_RX_H_

#include <stdint.h>

struct trame
{
	uint8_t Commande;
	uint8_t Vitesse;
	uint8_t Angle;
};

class Buffer_Reception{

private:
  uint8_t debut_derniere;
  uint8_t pointeur_ecriture;
  uint8_t compte_donnees;
  uint8_t pile[12];
  
public:
  void push(uint8_t iByte);
  trame& pull();
};


#endif /* BUFFER_RX_H_ */