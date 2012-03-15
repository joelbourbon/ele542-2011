/************************************************************************/
/*  Title       : normalmode.cpp                                        */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Contains the init and loop for the normal mode        */
/*                                                                      */
/************************************************************************/

#include "normalmode.h"
#include "singleton.h"

#define F_CPU 16000000

//
//  Main init (only called once upon reset)
//  
void NormalMode::init()
{
	// Essential inits
	mActiveTask = 1;
	mStopWatchingStartButton = 0;
	mStopWatchingStopButton  = 0;
  s.LedDriver.Led1.activateLED();
  s.Watchdog.disable();
  s.LedDriver.Led8.ON();
  State = Stop;
  
  // TESTING PURPOSE
  
  s.Uart.printString("\r\nUART IS WORKING\r\n");}


//
//  Main loop (only called once after the init)
//
void NormalMode::loop()
{
	checkButtons();
	
	// Start state loop
	if(State == Start)
  {	  
	  // TO BE REMOVED FROM HERE TO UART
	  s.Watchdog.reset();
	  
	  // START HERE
	  trame& wRecu = s.Uart.RX_Buffer.pull();
	  s.Moteur.CalculPWM(wRecu.Vitesse, wRecu.Angle, s.ADC1.getAvgLeftMotor(), s.ADC1.getAvgRightMotor());
	  processTasks();
  }
	
	// Stop state loop
	else
	{
	}
}

//
//  Call all the task that needs to be executed
//
void NormalMode::processTasks()
{
	if(1 == mActiveTask)
	{
		s.ADC1.processAverageADC();
		s.LedDriver.refreshLEDsState();
	}		
}

//
//  Check if a button is pressed (ON/OFF)
//
void NormalMode::checkButtons()
{
	if (s.StartButton.getState() == Low && mStopWatchingStartButton != 1)
	  initStart();
	else if(s.StopButton.getState() == Low && mStopWatchingStopButton != 1)
	  initStop();
}	

//
//  Init the stop state
//
void NormalMode::initStop()
{
	s.LedDriver.Led7.OFF();
	s.LedDriver.Led8.ON();
	// Disable the watchdog for the stop mode
	s.Watchdog.disable();
	
	// Debouncing the buttons
	mStopWatchingStartButton = 0;
	mStopWatchingStopButton  = 1;
	
	// Stop everything
	s.Moteur.ChangeMotorAction(MoteurDroit,  Neutre);
  s.Moteur.ChangeMotorAction(MoteurGauche, Neutre);  
  s.Timer1.setCompareValueLeft(0);
  s.Timer1.setCompareValueRight(0);
  
  // Stay in the main loop in Stop mode
  State = Stop;
}

//
//  Init the start state
//
void NormalMode::initStart()
{
	s.LedDriver.Led7.ON();
	s.LedDriver.Led8.OFF();
	// Debouncing the buttons
	mStopWatchingStartButton = 1;
	mStopWatchingStopButton  = 0;
  
  // Start calibration of the ADC
  s.ADC1.calibrate();
  
  // Stay in the main loop in start mode
  s.Watchdog.enable();  
  State = Start;
}