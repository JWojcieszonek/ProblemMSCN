#pragma once
#include "CMscnProblem.h"
#include "COptimizer.h"

class CDiffEvol :public COptimizer
{
public:
	
	CDiffEvol();
	CDiffEvol(CProblem* prob);
	CDiffEvol(int numOfIndividualsInPop);
	CDiffEvol(int numOfIndividualsInPop, CProblem* prob);
	~CDiffEvol();

	void reinitializePop(int numOfInd);
	void search();
	double* getBest() ;
	bool individualsAreDifferent(double* ind, double* baseInd, double* addInd0, double* addInd1);
	bool areDiff(double *dArr0, double *dArr1);
	void copySolution(double** sol, double ** other,int size);

	vector<double*> population;
private:
	static const double CROSS_PROB;
	static const double DIFF_WEIGHT;
	static const int DEFAULT_POP_SIZE;
	static const int NUMBER_OF_EVALUATIONS;
	
	
};