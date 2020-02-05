#include "CRandom.h"

CRandom::CRandom(int seed)
{
	generator = mt19937(seed);
}

CRandom::CRandom()
{
	random_device rd;
	generator = mt19937(rd());
}

int CRandom::getInt(int min, int max)
{
	uniform_int_distribution<> distribution(min, max);
	return distribution(generator);
}

double CRandom::getDouble(double min, double max)
{
	uniform_real_distribution<> distribution(min, max);
	return distribution(generator);
}