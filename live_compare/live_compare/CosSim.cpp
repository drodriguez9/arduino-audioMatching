#include "CosSim.h"
#include <math.h>
#include "Arduino.h"

CosSim::CosSim()
{
}


CosSim::~CosSim()
{
}

void CosSim::doCompare(int compare[])
{
	double magKey = magnitude(key);
	double magToCompare = magnitude(compare);
	Serial.print("CosSim: ");
	Serial.println(dot(compare) / (magKey*magToCompare));
}

double CosSim::magnitude(int mag[])
{
	double sum = 0;

	for (int i = 0; i < 126; i++)
	{
		sum += mag[i] * mag[i];
	}
	
	return sqrt(sum);
}

double CosSim::dot(int compare[])
{
	double sum = 0;

	for (int i = 0; i < 126; i++)
	{
		sum += key[i] * compare[i];
	}

	return sum;
}