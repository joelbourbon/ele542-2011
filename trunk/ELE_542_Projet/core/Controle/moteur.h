/**
	@file moteur.h
	@brief Header pour le fichier moteur.c
	@author Bruno De Kelper et Louis-Bernard Lagueux
	@date 13 Octobre 2006
*/
#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "core/Timer/timer1.h"

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

class moteur
{
	
private:
  timer1 Timer1;
	
public:
  moteur();
	void CalculPWM(float Vitesse_D, float Angle_D, float Vg, float Vd);
	
};

#endif
