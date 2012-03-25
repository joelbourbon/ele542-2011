#include "includes.h"
#include "uart.h"
#include "motorControl.h"
#include "timer.h"
#include "ADCcontrol.h"
#include "ledsControl.h"
#include "sonars.h"
#include "portsControl.h"


//GLOBAL DEFINITIONS
//
// CPU clock speed = 16Mhz
//
#ifndef F_CPU
#define F_CPU  16000000UL  // Crystal 16.000 Mhz
#endif

//GLOBAL VARIABLES
// LEDS
extern void initLeds(void);
extern void hideALL(void);
extern void showALL(void);
extern void startFlashing(unsigned char ledNumber);
extern void stopFlashing(unsigned char ledNumber);
extern void flashLeds(void);
extern void hideLed(unsigned char ledNumber);
extern void showLed(unsigned char ledNumber);
extern void ledDeadState(void);

// TWI
extern void configureTWI(void);
extern void processTWI(void);




//Timer1 ready flag
//extern volatile unsigned char uartRxCompleteFlag;
extern volatile unsigned char timer1ReadyFlag;	//1 = end of cycle, 0 = still running
extern volatile unsigned char adcReadyFlag;		//1 = adc ready for read, 0 = still running


//FUNCTIONS DEFINITION
extern void sendDebugValue(char* dataToSend, unsigned char valueToSend);
extern void initTimer1(void);
extern void initMotors(void);
extern void setMotors(void);
extern void uartInit(void);
extern void uartReceive(void);

extern void initADC(void);
extern void calibrateMotors(void);
extern void readADC(void);
extern void startADC(void);


void dealWithMotors(void);

extern void checkForAdc(void);

volatile unsigned char robotState;

extern volatile struct int_motorSpeeds convertedMotorSpeed;

extern volatile struct MotorCommands requestedCommand;


//extern void uartReceiveByte(void);

extern void initPorts(void);
extern void checkControls(void);
extern void setWatchDog(void);
//unsigned char led;


int main(void)
{
	initPorts();
	
	
	uart_init();			//Set uart COM on RS-232

    sei();//enable interrupts
	
	initADC();				//initialize ADC
	initTimer1();			//initialize Timer1
	//calibrateMotors();

	configureTWI();

    while(1) // loop forever
	{
		checkControls();
		
		//also disable interrupts and everything ??? 
		//in that case, receiver state should also be reset and kept to its value
		if (robotState == STATE_ALIVE)
		{
			uartReceive();
			checkForAdc();
			dealWithMotors();
			processTWI();
			//TWI thingy
	    
		    //PORTB = 0xFF;        
			flashLeds();

		}
		else
		{
			ledDeadState();	//keep led ouptus for DEAD STATE
		}

		if (ReceiverState == STATE_ANGLE_RECEIVED)
		{
			ReceiverState = STATE_WAITING;		
		}
    }     
	return 1;       
}



