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

#include "singleton.h"

class NormalMode 
{
public:
	void init();
	void loop();
};

#endif /* NORMALMOD_H_ */