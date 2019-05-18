/*
Name:		RadioReciever.ino
Created:	12/22/2017 9:51 PM
Author:	Michael Langford
*/

#include "RadioReciever.h"

//pulse widths
volatile int time[CHANNELS]; 

//current offset into array, used by interrupt method
volatile int offset = 0;

//percentage point output
float percents[CHANNELS];

//last micros() read by interrupt method
volatile int last_time;

//smoothing banks
float a[CHANNELS];
float b[CHANNELS];
float c[CHANNELS];
float d[CHANNELS];
float e[CHANNELS];
float f[CHANNELS];
float g[CHANNELS];

//initialization code
void init_RadioReciever() {
	pinMode(RADIO_RX, INPUT);

	int delta = 0;

	while (true)
	{
		delta = micros();
		wait_for_pulse();
		delta = micros() - delta;
		if (delta > TIME_MINIMUM)
			break;
	}

	attachInterrupt(digitalPinToInterrupt(RADIO_RX), interrupt_method, RISING);

}

//returns percentage from -100.0f to 100.0f for specified channel
float GetChannel(int channel)
{
	return percents[channel];
}

//update reciever
void update_RadioReciever() {
	for (int i = 0; i < CHANNELS; i++)
	{
		//this strategy smooths out integer changes in the channels values. These changes, 
		//if left unfiltered, can jack up the derivative part of the PID loop
		percents[i] += (get_percent(time[i]) - percents[i])*RADIO_DELTA_SPEED;
	}
}

//wait for a single pulse to occur
void wait_for_pulse() {
	while (true)
	{
		if (digitalRead(RADIO_RX) == HIGH) break;
	}
	while (true)
	{
		if (digitalRead(RADIO_RX) == LOW) break;
	}
}

//convert pulse width into a percentage value
float get_percent(int pulse)
{
	pulse -= CENTER_PW;
	float new_percent = (float)pulse / DELTA_PW;
	new_percent = max(-100.0, new_percent);
	new_percent = min(100.0, new_percent);

	return new_percent;
}

//interrupt method, called each time a pulse occurs
void interrupt_method()
{
	//roll over
	if (offset == CHANNELS)
		offset = 0;

	time[offset] = micros() - last_time;
	last_time = micros();

	offset++;
	
	//check roll over again
	if (offset == CHANNELS)
		offset = 0;
}