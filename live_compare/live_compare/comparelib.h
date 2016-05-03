// comparelib.h

#ifndef _COMPARELIB_h
#define _COMPARELIB_h

#include "arduino.h"

#if defined(ARDUINO) && ARDUINO >= 100
#else
	#include "WProgram.h"
#endif


#endif

class Compare
{
private:
	byte key[126] = { 65, 53, 53, 56, 40, 37, 45, 68, 100, 100, 53, 38, 19, 24,
					40, 63, 68, 19, 0, 38, 30, 55, 68, 68, 30, 8, 32, 37,
					19, 59, 43, 27, 8, 38, 25, 37, 65, 59, 38, 0, 16, 24,
					0, 51, 30, 24, 33, 19, 27, 0, 8, 27, 0, 43, 24, 37,
					27, 44, 37, 41, 8, 30, 0, 27, 27, 25, 38, 33, 25, 0,
					19, 27, 32, 16, 19, 8, 35, 16, 16, 16, 0, 27, 0, 39,
					30, 38, 32, 33, 43, 8, 16, 33, 0, 0, 25, 24, 30, 27,
					25, 8, 0, 19, 0, 19, 30, 19, 16, 25, 8, 16, 30, 35,
					16, 0, 19, 19, 19, 8, 32, 24, 38, 8, 0, 30, 0, 0 };
	int data[512];		//hardcoded length in Compare::insert for memory
	byte index = 0;
	byte start = 0;
	byte prevStart = 0;

public:
	Compare();
	~Compare();
	void doCompare();
	void grabData();
	void fft();		//length of data = 2^(nn)
	void printData();
//	void insert(int value);
//	bool verifyCollected();

};