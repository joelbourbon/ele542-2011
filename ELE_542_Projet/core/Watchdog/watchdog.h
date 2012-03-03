//************************************************************************/
/*  Title       : watchdog.h                                             */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Include file for the normal mode                      */
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
};
#endif /* watchdog_H_ */