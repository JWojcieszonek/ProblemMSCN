#include "CRandomSearch.h"

const int CRandomSearch::NUMBER_OF_ITERATIONS = 100;

CRandomSearch::CRandomSearch()
{
	problem = new CMscnProblem();
	problem->generateInstance();
}

CRandomSearch::CRandomSearch(CProblem* prob)
{
	problem = prob;
}


CRandomSearch::~CRandomSearch()
{
}

double* CRandomSearch::getBest()
{
	
	double* solution = problem->generateGoodRandomSolution();
	int counter = 0;
	
	for (int i = 0; i < NUMBER_OF_ITERATIONS; i++)
	{
		double* newSolution = problem->generateGoodRandomSolution();
		double solQ = problem->getQuality(solution);
		double newQ = problem->getQuality(newSolution);
		if (problem->constraintsSatisfied(newSolution))
		{
			if (newQ > solQ)
			{
				*solution = *(newSolution);
				delete newSolution;
			}
		}
		else i++;
	}
	return solution;
}


