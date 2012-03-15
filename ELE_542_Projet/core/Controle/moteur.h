/************************************************************************/
/*  Title       : moteur.h                                              */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Include file for the moteur                           */
/*                                                                      */
/************************************************************************/

#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "core/Timer/timer1.h"
#include "core/PIN/pin.h"

#define Pi      (3.1415926535897932384626433832795)
#define RAYON   (9.525)
#define TS      (0.005)
#define Vmax    (80.0)
#define Tau     (0.5)

#define H11		(3.90148347975678) 
#define H12		(4.90148347975678)

#define H21		(1.1613504)
#define H22		(0.5806746734)

#define Fpwm (10000)

#define DIR_G1 2
#define DIR_G2 3
#define DIR_D1 6
#define DIR_D2 7

enum ActionMoteur{
	Neutre        = 0,
	MarcheAvant   = 1,
	MarcheArriere = 2,
	Frein         = 3
};

enum Moteurs{
	MoteurGauche = 0,
	MoteurDroit  = 1
};

class moteur
{
	
private:
  timer1 Timer1;
  pin PIN_G1;
	pin PIN_G2;
	pin PIN_D1;
	pin PIN_D2;
	
public:
  moteur();
	void CalculPWM(uint8_t iVitesse_D, uint8_t iAngle_D, float Vg, float Vd);
	void ChangeMotorAction(Moteurs iMoteur, ActionMoteur iAction);	
};

#endif
