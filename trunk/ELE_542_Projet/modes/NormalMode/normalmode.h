/************************************************************************/
/*  Title       : mormalmode.h                                          */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : Include file for the normal mode                      */
/*                                                                      */
/************************************************************************/

#ifndef NORMALMODE_H_
#define NORMALMODE_H_

#include <stdint.h>

enum state{
	  Start   = 0
	, Stop    = 1
};

class NormalMode 
{
private:
  uint8_t mActiveTask;
  uint8_t mStopWatchingStartButton;
  uint8_t mStopWatchingStopButton;
  state State;
  void processTasks();
  void checkButtons();
	
public:
	void init();
	void loop();
	void initStart();
	void initStop();
};

#endif /* NORMALMOD_H_ */