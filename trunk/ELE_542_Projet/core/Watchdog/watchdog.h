/************************************************************************/
/*  Title       : watchdog.h                                            */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Include file for the watchdog                         */
/*                                                                      */
/************************************************************************/


#ifndef watchdog_H_
#define watchdog_H_

#include <avr/io.h>
#include <avr/interrupt.h>

class watchdog{
private:
	
public:
	watchdog();
	void reset();
	void enable();
	void disable();
};
#endif /* watchdog_H_ */