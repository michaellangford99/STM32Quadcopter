// MMA7455L.h

#ifndef _MMA7455L_h
#define _MMA7455L_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "wprogram.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>
/*
#define MMA_7455_ADDRESS 0x1D //I2C Adsress for the sensor
#define MMA_7455_MODE_CONTROLL 0x16 //Call the sensors Mode Control

#define MMA_7455_2G_MODE 0x05 //Set Sensitivity to 2g
#define MMA_7455_4G_MODE 0x09 //Set Sensitivity to 4g
#define MMA_7455_8G_MODE 0x01 //Set Sensitivity to 8g

#define X_OUT 0x06 //Register for reading the X-Axis
#define Y_OUT 0x07 //Register for reading the Y-Axis
#define Z_OUT 0x08 //Register for reading the Z-Axis

#define X_ACC_OFFSET -4.2789f
#define Y_ACC_OFFSET -17.826401f
#define Z_ACC_OFFSET -115.9207f

#define MMA_7455L_CALIBRATE_TIME 500

void init_MMA7455L();
void update_MMA7455L();
void Calibrate_Accelerometer();

float GetAxis(int reg);

float Get_raw_X();
float Get_raw_Y();
float Get_raw_Z();

float Get_Acc_Pitch();
float Get_Acc_Roll();
*/
#endif

