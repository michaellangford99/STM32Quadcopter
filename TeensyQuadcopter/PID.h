// PID.h

#ifndef _PID_h
#define _PID_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "wprogram.h"
#else
	#include "WProgram.h"
#endif

class PIDClass
{
 protected:
	 float kP = 0.0f;
	 float kI = 0.0f;
	 float kD = 0.0f;

	 float Error = 0.0f;
	 float Last_Error = 0.0f;

	 float P_Error = 0.0f, I_Error = 0.0f, D_Error = 0.0f;

	 float Output = 0.0f, Last_Output = 0.0f, Delta_Output = 0.0f;


 public:
	void init_PID();
	float update_PID(float real, float desired);
	void Set_Constants(float KP, float KI, float KD);
	//void clear_D_Error(float real, float desired);
};

#endif

