/*
 Name:		live_compare.ino
 Created:	4/17/2016 5:45:57 PM
 Author:	David

Takes in data on ADC7 (Analog0) and processes them
with the fft. the data is sent out over the serial
port at 115.2kb.
*/
#define LOG_OUT 1
#define FHT_N 256

#include <FHT.h>


int sensorPin = A7;
byte sensorValue = 0;
byte thresholdMin = 120;
byte thresholdMax = 140;

byte key[126] = { 65, 53, 53, 56, 40, 37, 45, 68, 100, 100, 53, 38, 19, 24,
40, 63, 68, 19, 0, 38, 30, 55, 68, 68, 30, 8, 32, 37,
19, 59, 43, 27, 8, 38, 25, 37, 65, 59, 38, 0, 16, 24,
0, 51, 30, 24, 33, 19, 27, 0, 8, 27, 0, 43, 24, 37,
27, 44, 37, 41, 8, 30, 0, 27, 27, 25, 38, 33, 25, 0,
19, 27, 32, 16, 19, 8, 35, 16, 16, 16, 0, 27, 0, 39,
30, 38, 32, 33, 43, 8, 16, 33, 0, 0, 25, 24, 30, 27,
25, 8, 0, 19, 0, 19, 30, 19, 16, 25, 8, 16, 30, 35,
16, 0, 19, 19, 19, 8, 32, 24, 38, 8, 0, 30, 0, 0 };

void setup() 
{
	Serial.begin(115200);
}

void loop() 
{
	sensorValue = analogRead(sensorPin) / 4;	//keep values 0-255
//	if (sensorValue < thresholdMin || sensorValue > thresholdMax)
//	{
	TIMSK0 = 0;			// turn off timer0 for lower jitter
	ADCSRA = 0xe5;		// set the adc to free running mode
	ADMUX = 0x47;		// use adc7

	int passes = 10;

	while (passes > 0) {			// reduces jitter
		cli();			// UDRE interrupt slows this way down on arduino1.0
		for (int i = 0; i < 512; i = +2) {		 // save 256 samples
			while (!(ADCSRA & 0x10));		 // wait for adc to be ready
			ADCSRA = 0xf5;		// restart adc
			byte m = ADCL;		 // fetch adc data
			byte j = ADCH;
			int k = (j << 8) | m;		// form into an int
			k -= 0x0200;		// form into a signed int
			k <<= 6;		// form into a 16b signed int
			fht_input[i] = fht_input[i] + k;		// put real data into even bins
		}
		sei();
		passes--;
	}
	for (int i = 0; i < 512; i = +2) {
		fht_input[i] = fht_input[i] / passes;
	}

//	}
}
