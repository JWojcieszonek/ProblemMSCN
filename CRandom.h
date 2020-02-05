#pragma once
#include <random>
using namespace std;
class CRandom
{
public:
public:
	CRandom(int seed);
	CRandom();
	~CRandom() {};

	int getInt(int iMin, int iMax);
	double getDouble(double dMin, double dMax);

private:
	mt19937 generator;
};

