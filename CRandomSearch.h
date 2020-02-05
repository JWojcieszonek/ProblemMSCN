#pragma once
#include "CMscnProblem.h"
#include "COptimizer.h"
class CRandomSearch : public COptimizer
{
public:
	CRandomSearch();
	CRandomSearch(CProblem* problem);
	~CRandomSearch();
	double* getBest();
	double* generateRandSol();
	double* generateGoodRandSol();
private:
	
	static const int NUMBER_OF_ITERATIONS;
};

