// 
// 
// 

#include "Kalman.h"

void Kalman::init_Kalman()
{
	Q_gyro_trust = 0.9f;
	Q_acc_trust = 0.1f;
	R_angle = 0.1f;

	p[0][0] = 0.0f;
	p[0][1] = 0.0f;
	p[1][0] = 0.0f;
	p[1][1] = 0.0f;
	KFangle = 0.0f;
	y_bias = 0.0f;
}

float Kalman::Filter(float acc_angle, float gyro_rate)
{
	float y = 0.0f;
	float s = 0.0f;

	float k[2];

	k[0] = 0.0f;
	k[1] = 0.0f;

	KFangle = KFangle + (gyro_rate - y_bias);
	p[0][0] = p[0][0] + (p[1][1] - (p[0][1] + p[1][0] - Q_gyro_trust));

	p[0][1] = p[0][1] - p[1][1];
	p[1][0] = p[1][0] - p[1][1];
	p[1][1] = p[1][1] + Q_acc_trust;
	y = acc_angle - KFangle;
	s = p[0][0] + R_angle;
	k[0] = p[0][0] / s;
	k[1] = p[1][0] / s;
	KFangle = KFangle + (k[0] * y);
	y_bias = y_bias + (k[1] * y);
	p[0][0] -= k[0] * p[0][0];
	p[0][1] -= k[0] * p[0][1];
	p[1][0] -= k[1] * p[0][0];
	p[1][1] -= k[1] * p[0][1];

	return KFangle;
}