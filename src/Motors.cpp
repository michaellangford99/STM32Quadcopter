/*
Name:		Motors.cpp
Created:	12/23/2017 8:32 AM
Author:	Michael Langford
*/

#include "Motors.h"

#define MOTOR_MIN 0.0f
#define MOTOR_MAX 100.0f

#define PWM_BITS 16
#define PWM_MULTIPLIER 65535.0f

#define M1_PIN    6
#define M2_PIN    3
#define M3_PIN    5
#define M4_PIN    4

float m1, m2, m3, m4;

void init_motors()
{
	m1 = m2 = m3 = m4 = 0.0f;
	
	analogWriteRes(PWM_BITS);

	pinMode(M1_PIN, OUTPUT);
	pinMode(M2_PIN, OUTPUT);
	pinMode(M3_PIN, OUTPUT);
	pinMode(M4_PIN, OUTPUT);
}

void update_motors(float a, float b, float c, float d)
{
	a = max(MOTOR_MIN, a);
	b = max(MOTOR_MIN, b);
	c = max(MOTOR_MIN, c);
	d = max(MOTOR_MIN, d);

	a = min(MOTOR_MAX, a);
	b = min(MOTOR_MAX, b);
	c = min(MOTOR_MAX, c);
	d = min(MOTOR_MAX, d);

	analogWrite(M1_PIN, (int)((a / 100.0f)*(PWM_MULTIPLIER)));
	analogWrite(M2_PIN, (int)((b / 100.0f)*(PWM_MULTIPLIER)));
	analogWrite(M3_PIN, (int)((c / 100.0f)*(PWM_MULTIPLIER)));
	analogWrite(M4_PIN, (int)((d / 100.0f)*(PWM_MULTIPLIER)));
}