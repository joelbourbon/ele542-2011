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
#include "core/Timer/timer1.h"

class CoreInit
{

public:
	CoreInit();

public:	
	uart Uart;
	timer1 Timer1;
	
	 
	
};

#endif /* INITCORE_H_ */