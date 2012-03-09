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

extern "C" void __vector_10(); // ISR(TIMER0_COMP_vect)

class timer0
{
private:
  friend void __vector_10();
  void interupt_conversion();

public:
  timer0();  
  
  volatile uint16_t mTop_Value;
  volatile uint32_t time_us;
  volatile uint64_t time_ms; 
  
  uint64_t get_ms_time();
  
};

#endif /* TIMER0_H_ */