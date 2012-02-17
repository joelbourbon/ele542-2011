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


#include "buffer_RX.h"

Buffer_Reception::Buffer_Reception():
	debut_derniere(100),
	pointeur_ecriture(0),
	compte_donnees(0),
	pile()
{
	
}

void Buffer_Reception::push(uint8_t iByte)
{
	if(iByte >= 0xF0 && compte_donnees !=0)
	  pointeur_ecriture -= compte_donnees;
	
	pile[pointeur_ecriture] = iByte;
	
	if(compte_donnees == 2)
	{
		compte_donnees = 0;
		debut_derniere = pointeur_ecriture - 2;
	}
	else
	  ++compte_donnees;
	  
	if(pointeur_ecriture >= 11)
	  pointeur_ecriture = 0;
	else
	  ++pointeur_ecriture;
}	

trame& Buffer_Reception::pull()
{
	trame derniereTrame;
	derniereTrame.Angle = 0;
	derniereTrame.Commande = 0;
	derniereTrame.Vitesse = 0;
	
	if(debut_derniere == 100)
	  return derniereTrame;
	
	derniereTrame.Commande = pile[debut_derniere];
	derniereTrame.Vitesse  = pile[debut_derniere + 1];
	derniereTrame.Angle    = pile[debut_derniere + 2];
	return derniereTrame;
}