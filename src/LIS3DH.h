/*
Name:		LIS3DH.h
Created:	3/2/2018 4:50 PM
Author:	Michael Langford
*/

#ifndef _LIS3DH_h
#define _LIS3DH_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "wprogram.h"
#else
	#include "WProgram.h"
#endif

#include <SPI.h>

float Get_raw_X();
float Get_raw_Y();
float Get_raw_Z();

float Get_Acc_Pitch();
float Get_Acc_Roll();

void update_LIS3DH();

void Calibrate_Accelerometer();

void init_LIS3DH();

#endif

