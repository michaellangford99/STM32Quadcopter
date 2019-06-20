
// MPU9250

#ifndef _MPU9250_h
#define _MPU9250_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "wprogram.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>

void init_MPU9250();
void update_MPU9250();

//Gyroscope
void UpdateGyro();

float GetYaw();
float GetPitch();
float GetRoll();

float GetYawRate();
float GetPitchRate();
float GetRollRate();

//Accelerometer
float GetXAcc();
float GetYAcc();
float GetZAcc();

#endif