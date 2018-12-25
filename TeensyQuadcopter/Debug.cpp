// 
// 
// 

#include "Debug.h"
#include "Metro.h"

#define DEBUG_LED  0
#define DEBUG_FLASH_DURATION 250

int LEDstate = HIGH;
bool waiting = false;
/*Metro ledmetro = Metro(DEBUG_FLASH_DURATION);*/

void init_Debug()
{
	//init LED
	//pinMode(DEBUG_LED, OUTPUT);

}

void update_Debug_SAFE()
{
	//debug LED for SAFE
	//digitalWrite(DEBUG_LED, LOW);
}

void update_Debug_ARMED()
{
	/*if (ledmetro.check() == 1) waiting = false;

	if (waiting == false)
	{
		LEDstate = HIGH;
	}
	else
	{
		LEDstate = LOW;
	}*/

	//Debug LED for ARM
	//digitalWrite(DEBUG_LED, LEDstate);
}

void debug_flash()
{
	/*ledmetro.interval(DEBUG_FLASH_DURATION);
	waiting = true;*/
}