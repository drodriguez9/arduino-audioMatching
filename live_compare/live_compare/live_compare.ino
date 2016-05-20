/*
 Name:		Sketch1.ino
 Created:	5/11/2016 5:03:21 PM
 Author:	David
*/

#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht
#define PASSES 10

#include <FHT.h>
#include "CosSim.h"

int data[126];
CosSim myCos;

void setup() {
	Serial.begin(115200); 
}

void loop() {
	int sensorValue = analogRead(A7)/4;		//using a voltage threshold to trigger a comnparison against the key
	Serial.println(sensorValue);
	if (sensorValue > 95 || sensorValue < 94) {
		while (1) { // reduces jitter
			for (int j = PASSES; j >= 0; j--)		//taking avg of 10 passes
			{
				for (int i = 0; i < FHT_N; i++) {		//save 256 samples
					while (!(ADCSRA & 0x10));		// wait for adc to be ready
					ADCSRA = 0xf5;		//restart adc
					byte m = ADCL;		//fetch adc data
					byte j = ADCH;
					int k = (j << 8) | m;		//form into an int
					k -= 0x0200;		// form into a signed int
					k <<= 6;		// form into a 16b signed int
					fht_input[i] = fht_input[i] + k;		// put real data into bins
				}
			}
			for (int i = 0; i < FHT_N; i++)
			{
				fht_input[i] = fht_input[i] / PASSES;		//average
			}
			fht_window();		//window the data for better frequency response
			fht_reorder();		//reorder the data before doing the fht
			fht_run();		//process the data in the fht
			fht_mag_log();		//take the output of the fht
			for (int i = 0; i < 128; i++) {		//type issues when calling c++ functions
				data[i] = fht_log_out[i + 2];		//removing the 1/f noise
			}
			myCos.doCompare(data);
		}
	}
}
