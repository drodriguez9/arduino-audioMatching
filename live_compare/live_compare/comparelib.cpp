// 
// 
// 

#include "comparelib.h"
#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <../libraries/FFT/FFT.h> // include the library

Compare::Compare()
{

}

Compare::~Compare()
{

}

void Compare::doCompare()
{
	grabData();
}

void Compare::grabData()
{
	TIMSK0 = 0;			// turn off timer0 for lower jitter
	ADCSRA = 0xe5;		// set the adc to free running mode
	ADMUX = 0x47;		// use adc7

	while (1) {			// reduces jitter
		cli();			// UDRE interrupt slows this way down on arduino1.0
		for (int i = 0; i < 512; i += 2) {		 // save 256 samples
			while (!(ADCSRA & 0x10));		 // wait for adc to be ready
			ADCSRA = 0xf5;		// restart adc
			byte m = ADCL;		 // fetch adc data
			byte j = ADCH;
			int k = (j << 8) | m;		// form into an int
			k -= 0x0200;		// form into a signed int
			k <<= 6;		// form into a 16b signed int
			fft_input[i] = k;		// put real data into even bins
			fft_input[i + 1] = 0;		// set odd bins to 0
		}
		fft_window();		 // window the data for better frequency response
		fft_reorder();		 // reorder the data before doing the fft
		fft_run();		// process the data in the fft
		fft_mag_log();		// take the output of the fft
		sei();
		for (byte i = 0; i < FFT_N / 2; i++) {
			insert(fft_log_out[i]);			// send out the data
		}
		if (verifyCollected())
		{
			break;
		}
	}
}

void Compare::insert(int value)
{
	data[index] = value;
	if (index == 255)
	{
		data[index] = 0;
	}
	else
	{
		index++;
	}
}

bool Compare::verifyCollected()
{
	for (byte i = 0; i < 255; i++)
	{
		if (data[i] > 190 && data[i + 1] > 170)		//finds 1/f noise and sets the start
		{
			start = (i + 2) % 255;
			if (start != prevStart)		//making sure we grab new data
			{
				prevStart = start;		//storing for next check
				return true;
			}
		}
	}
	return false;
}