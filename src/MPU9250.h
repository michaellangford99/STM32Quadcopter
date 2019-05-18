
// L3G4200D

#ifndef _MPU9250_h
#define _MPU9250_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "wprogram.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>

#define CLOCK_SPEED                1000000
#define CALIBRATE_TIME             1000

void init_MPU9250();
void update_MPU9250_h();

#endif