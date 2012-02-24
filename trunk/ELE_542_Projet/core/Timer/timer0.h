/************************************************************************/
/*  Title       : timer0.h                                              */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Class that creates the timer for PWN to control the   */
/*                motors of the µC                                      */
/************************************************************************/


#ifndef TIMER0_H_
#define TIMER0_H_

#include <avr/io.h>
#include <stdint.h>

class timer0
{
private:
  


public:
  timer0();
  void interupt_conversion();
  
  uint16_t mTop_Value;
  volatile uint32_t time_us;
  volatile uint32_t time_ms; 
  
};

#endif /* TIMER0_H_ */