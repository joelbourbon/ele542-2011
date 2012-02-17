/************************************************************************/
/*  Title       : timer1.h                                              */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Class that creates the timer for PWN to control the   */
/*                motors of the µC                                      */
/************************************************************************/


#ifndef TIMER1_H_
#define TIMER1_H_

#include <avr/io.h>
#include <stdint.h>

class timer1
{

public:
	void setCompareValueLeft(uint16_t iValue );
	void setCompareValueRight(uint16_t iValue );
  timer1();

};

#endif /* TIMER1_H_ */