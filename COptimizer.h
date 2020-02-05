#pragma once
#include "CProblem.h"

class COptimizer
{
public:
	virtual double* getBest() = 0;
	virtual ~COptimizer() {  };
	virtual CProblem* getProblem() { return problem; };
protected:
	CProblem* problem = nullptr;

};
