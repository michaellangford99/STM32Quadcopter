// 
// 
// 

#include "Debug.h"
#include "Metro.h"

#define DEBUG_LED  0

void init_Debug()
{
	//init LED
	pinMode(DEBUG_LED, OUTPUT);
}

void update_Debug_SAFE()
{
	//debug LED for SAFE
	digitalWrite(DEBUG_LED, LOW);
}

void update_Debug_ARMED()
{
	//Debug LED for ARM
	digitalWrite(DEBUG_LED, HIGH);
}