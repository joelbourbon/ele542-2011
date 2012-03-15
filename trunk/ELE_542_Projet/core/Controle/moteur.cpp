/************************************************************************/
/*  Title       : moteur.cpp                                            */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Management of the motors (PWM)                        */
/*                                                                      */
/************************************************************************/

#include "moteur.h"
#include "math.h"

moteur::moteur():
    Timer1()
  , PIN_G1(&PORTD, &DDRD, &PIND, PIND2, PinOutput)
	, PIN_G2(&PORTD, &DDRD, &PIND, PIND3, PinOutput)
	, PIN_D1(&PORTD, &DDRD, &PIND, PIND6, PinOutput)
	, PIN_D2(&PORTD, &DDRD, &PIND, PIND7, PinOutput)
  {	  
  }

//
//  Already giver function to calculate PWM but modified to fit our needs
//  
void moteur::CalculPWM(uint8_t iVitesse_D, uint8_t iAngle_D, float Vg, float Vd)
{
	float Vitesse_D = ((float)(iVitesse_D - 100)) / 100.f;
	float Angle_D   = ((float)(iAngle_D >> 1)) * M_PI; 
	      Angle_D  /= 180.f;

	/**
		Dans cette fonction, la valeur des duty cycle pour chaque moteur est calculé.  
		Ce calcul est effectué à l`aide de la vitesse désirée, de l`angle désriré ainsi 
		que de la vitesse (mesurée avec CalculVitesses) et l`angle actuel.
	*/
	static float Angle = 0.0;
	static float ErreurAngle = 0.0;
	static float W = 0.0;
	static float Old_W = 0.0;
	static float Vt     = 0.0;
	static float Ut     = 0.0;
	static float Ua     = 0.0;
	static float Duty_G = 0.0;
	static float Duty_D = 0.0;
	static int 	 Signe_Ua = 0;
	static int 	 Signe_Ut = 0;


	Vg = (Vg > 1.0) ? 1.0 : ((Vg < -1.0) ? -1.0 : Vg);  /* Regarde les limites (-1.0 à 1.0) */
	Vd = (Vd > 1.0) ? 1.0 : ((Vd < -1.0) ? -1.0 : Vd);  /* Regarde les limites (-1.0 à 1.0) */
		
	Old_W = W;
	W     = 0.5*(Vmax/RAYON)*(Vd-Vg);
	Vt    = 0.5*(Vd+Vg);

	Angle = Angle + (0.5)*TS*(W+Old_W);
	Angle = (Angle > 2.0*Pi) ? Angle - 2*Pi : ((Angle < 0.0) ? Angle + 2*Pi : Angle); /* Angle entre 0 et 2 pi */
	ErreurAngle = ((Angle_D >= Pi + Angle) ? Angle_D - 2*Pi : ((Angle_D <= -Pi + Angle) ? Angle_D + 2*Pi : Angle_D))-Angle;

	Ut = -H11*Vt + H12*Vitesse_D;
	Ua = H21*ErreurAngle - H22*W;
			
	Signe_Ua = (Ua >= 0.0)   ? 1 : -1;
	Signe_Ut = (Ut >= 0.0) ? 1 : -1;
	
	Ua = (Signe_Ua*Ua > 1.0) ? Signe_Ua*1.0 : ((Signe_Ua*Ua <= 0.05) ? 0.0 : Ua);
	Ut = (Signe_Ut*Ut > 1.0) ? Signe_Ut*1.0 : Ut;
	Ut = ((Signe_Ut*Ut) > (1.0 - Signe_Ua*Ua)) ? Signe_Ut*(1.0 - Signe_Ua*Ua) : Ut;


	Duty_D = (Ut+Ua);
	Duty_G = (Ut-Ua);

	Duty_D = (Duty_D > 0.99) ? 0.99 : ((Duty_D < -0.99) ? -0.99 : Duty_D);
	Duty_G = (Duty_G > 0.99) ? 0.99 : ((Duty_G < -0.99) ? -0.99 : Duty_G);
	
	//MOTEUR DROIT
	if(Duty_D > 0)
	  ChangeMotorAction(MoteurDroit, MarcheAvant);
	else if(Duty_D < 0)
	  ChangeMotorAction(MoteurDroit, MarcheArriere);
	else
	  ChangeMotorAction(MoteurDroit, Neutre);
	  
	//MOTEUR GAUCHE
	if(Duty_G > 0)
	  ChangeMotorAction(MoteurGauche, MarcheAvant);
	else if(Duty_D < 0)
	  ChangeMotorAction(MoteurGauche, MarcheArriere);
	else
	  ChangeMotorAction(MoteurGauche, Neutre);
	  
	// Absolute value of the duties
	Duty_D = (Duty_D < 0.0) ? -Duty_D : Duty_D;
	Duty_G = (Duty_G < 0.0) ? -Duty_G : Duty_G;
	
	// Change the duty cycle of the PWM for each motor
	Timer1.setCompareValueRight(Duty_D * (float)Timer1.mTop_Value);
	Timer1.setCompareValueLeft( Duty_G * (float)Timer1.mTop_Value);
}

//
//  Function to choose direction of the motor (simplified)
//
void moteur::ChangeMotorAction(Moteurs iMoteur, ActionMoteur iAction)
{
	pin* PinMoteur1 = &PIN_G1;
	pin* PinMoteur2 = &PIN_G2;
	
	if(iMoteur == MoteurDroit)
	{
		PinMoteur1 = &PIN_D1;
		PinMoteur2 = &PIN_D2;
	}	  
	
	if (iAction == Neutre)
	{
		PinMoteur1->clearPIN();
		PinMoteur2->clearPIN();
	}
	else if (iAction == MarcheAvant)
	{
		PinMoteur1->setPIN();
		PinMoteur2->clearPIN();
	}
	else if (iAction == MarcheArriere)
	{
		PinMoteur1->clearPIN();
		PinMoteur2->setPIN();
	}
	else
	{
		PinMoteur1->setPIN();
		PinMoteur2->setPIN();
	}
}

