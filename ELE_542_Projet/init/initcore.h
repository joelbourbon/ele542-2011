/************************************************************************/
/*  Title       : initcore.h                                            */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Inits all the core systems used in modes              */
/*                                                                      */
/************************************************************************/


#ifndef INITCORE_H_
#define INITCORE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "core/UART/uart.h"
#include "core/Timer/timer0.h"
#include "core/Timer/timer1.h"
#include "core/ADC/adc.h"
#include "core/Controle/moteur.h"
#include "core/datastore.h"
#include "core/watchdog/watchdog.h"
#include "core/PIN/pin.h"
#include "core/LED/led.h"
//#include "tasks/AverageADC.h"

class CoreInit
{

public:
	CoreInit();

public:	
	uart Uart;
	timer0 Timer0;
	timer1 Timer1;
	moteur Moteur;
	datastore DataStore;
	adc ADC1;
	watchdog Watchdog;
	
	pin StartButton;
	pin StopButton;
};

#endif /* INITCORE_H_ */