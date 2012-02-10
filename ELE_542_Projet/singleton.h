/************************************************************************/
/*  Title       : Singleton.h                                           */
/*                                                                      */
/*  Class       : ELE-542                                               */  
/*                                                                      */
/*  Written by  : Joel Bourbonnais & Olivier Massé                      */
/*	                                                                    */
/*  Summary     : The main object to be instantiated to init everything */
/*                                                                      */
/************************************************************************/

#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <init/initcore.h>

class Singleton : public CoreInit
{	
	public:
		Singleton() : CoreInit()
		{
		
		}		
};

extern Singleton s;

#endif /* SINGLETON_H_ */