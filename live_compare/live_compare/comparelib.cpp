#include "comparelib.h"

Compare::Compare()
{
	for (int i = 0; i < 512; i++) {
		data[i] = 0;
	}
}

Compare::~Compare()
{

}

void Compare::doCompare()
{
	Serial.println("doCompare Called.");
	grabData();
	fft();
}

void Compare::grabData()
{
	Serial.println("grabData Called.");
	TIMSK0 = 0;			// turn off timer0 for lower jitter
	ADCSRA = 0xe5;		// set the adc to free running mode
	ADMUX = 0x47;		// use adc7

	int passes = 10;

	while (passes > 0) {			// reduces jitter
		cli();			// UDRE interrupt slows this way down on arduino1.0
		for (int i = 0; i < 512; i =+ 2) {		 // save 256 samples
			while (!(ADCSRA & 0x10));		 // wait for adc to be ready
			ADCSRA = 0xf5;		// restart adc
			byte m = ADCL;		 // fetch adc data
			byte j = ADCH;
			int k = (j << 8) | m;		// form into an int
			k -= 0x0200;		// form into a signed int
			k <<= 6;		// form into a 16b signed int
			data[i] = data[i] + k;		// put real data into even bins
		}
		sei();
		passes--;
	}
	for (int i = 0; i < 512; i =+ 2) {
		data[i] = data[i] / passes;
	}

}

void Compare::fft()
{
	Serial.println("fft Called.");
	unsigned int n, mmax, m, j, istep, i;
	int nn = 9;
	double wtemp, wr, wpr, wpi, wi, theta;
	double tempr, tempi;

	//reindexing
	n = nn << 1;
	j = 1;
	for (i = 1; i<n; i += 2) {
		if (j>i) {
			double temp = data[j - 1];
			data[j - 1] = data[i - 1];
			data[i - 1] = temp;

			temp = data[j];
			data[j] = data[i];
			data[i] = temp;
		}
		m = nn;
		while (m >= 2 && j>m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	};

	mmax = 2;
	while (n>mmax) {
		istep = mmax << 1;
		theta = -(2 * M_PI / mmax);
		wtemp = sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
		for (m = 1; m < mmax; m += 2) {
			for (i = m; i <= n; i += istep) {
				j = i + mmax;
				tempr = wr*data[j - 1] - wi*data[j];
				tempi = wr * data[j] + wi*data[j - 1];

				data[j - 1] = data[i - 1] - tempr;
				data[j] = data[i] - tempi;
				data[i - 1] += tempr;
				data[i] += tempi;
			}
			wtemp = wr;
			wr += wr*wpr - wi*wpi;
			wi += wi*wpr + wtemp*wpi;
		}
		mmax = istep;
	}
}


void Compare::printData() {
	Serial.println("printData Called.");
	for (int i = 0; i < 512; i++) {
		Serial.println(data[i]);
	}
}

/*
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
	for (int i = 0; i < 255; i++)
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
}*/