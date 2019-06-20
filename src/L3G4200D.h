
// L3G4200D

#ifndef _L3G4200D_h
#define _L3G4200D_h

//#define L3G4200D_IN_USE

#ifdef L3G4200D_IN_USE

#if defined(ARDUINO) && ARDUINO >= 100
#include "wprogram.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>

#define CLOCK_SPEED                200000
#define CALIBRATE_TIME             1000

float GetYaw();
float GetPitch();
float GetRoll();

float GetYawRate();
float GetPitchRate();
float GetRollRate();

float GetGyroElapsedTime();

void ClearAngles(float yaw, float pitch, float roll);
void SetGyroAngles(float yaw, float pitch, float roll);

float get_cal_x();
float get_cal_y();
float get_cal_z();

void init_L3G4200D();
void update_L3G4200D();
void Calibrate_Gyro();

int16_t getX();
int16_t getY();
int16_t getZ();

#endif
#endif