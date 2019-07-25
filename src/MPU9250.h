
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
void update_Gyro();

void calibrate_Gyro();

float GetYaw();
float GetPitch();
float GetRoll();

float GetYawRate();
float GetPitchRate();
float GetRollRate();

void ClearAngles(float yaw, float pitch, float roll);

//Accelerometer
void update_Accelerometer();

void calibrate_Accelerometer();

float GetPitchAcc();
float GetRollAcc();

float GetXAcc();
float GetYAcc();
float GetZAcc();

#endif