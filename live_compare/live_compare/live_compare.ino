/*
 Name:		live_compare.ino
 Created:	4/17/2016 5:45:57 PM
 Author:	David

Takes in data on ADC7 (Analog0) and processes them
with the fft. the data is sent out over the serial
port at 115.2kb.
*/

#include "comparelib.h"

Compare myCompare;
int sensorPin = A7;
byte sensorValue = 0;
byte thresholdMin = 120;
byte thresholdMax = 140;

void setup() 
{
	Serial.begin(115200);
}

void loop() 
{
	sensorValue = analogRead(sensorPin) / 4;	//keep values 0-255
//	if (sensorValue < thresholdMin || sensorValue > thresholdMax)
//	{
		myCompare.doCompare();
//	}
}
