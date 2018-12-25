// Kalman.h

#ifndef _KALMAN_h
#define _KALMAN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "wprogram.h"
#else
	#include "WProgram.h"
#endif

class Kalman
{
protected:
	float Q_gyro_trust;
	float Q_acc_trust;
	float R_angle;
	float y_bias;
	float p[2][2];
	float KFangle;

public:
	void init_Kalman();
	float Filter(float acc_angle, float gyro_rate);
};

#endif

