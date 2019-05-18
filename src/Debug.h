// Debug.h

#ifndef _DEBUG_h
#define _DEBUG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "wprogram.h"
#else
	#include "WProgram.h"
#endif

void init_Debug();
void update_Debug_SAFE();
void update_Debug_ARMED();

#endif

