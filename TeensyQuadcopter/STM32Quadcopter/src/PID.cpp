// 
// 
// 

#include "PID.h"


void PIDClass::init_PID()
{
	
}

float PIDClass::update_PID(float real, float desired)
{
	Error = desired - real;

	P_Error = Error;
	I_Error = I_Error + Error;
	D_Error = Error - Last_Error;

	Output = P_Error*kP;
	Output += I_Error*kI;
	Output += D_Error*kD;

	//Delta_Output = Output - Last_Output;

	//Delta_Output */

	Last_Error = Error;
	Last_Output = Output;

	return Output;
}
/*
void PIDClass::clear_D_Error(float real, float desired)
{
	Last_Error = desired - real;
}*/

void PIDClass::Set_Constants(float KP, float KI, float KD)
{
	kP = KP;
	kI = KI;
	kD = KD;
}

