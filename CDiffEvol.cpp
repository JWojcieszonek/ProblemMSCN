#include "CDiffEvol.h"
#include "CRandom.h"
#include <string>

const double CDiffEvol::CROSS_PROB = 0.66;
const double CDiffEvol::DIFF_WEIGHT = 1.33;
const int CDiffEvol::DEFAULT_POP_SIZE = 10;
const int CDiffEvol::NUMBER_OF_EVALUATIONS = 200;

CDiffEvol::CDiffEvol()
{
	
	//prob.generateInstance();
	problem = new CMscnProblem();
	problem->generateInstance();
	population.resize(DEFAULT_POP_SIZE);
	for (int i = 0; i < population.size(); i++)
	{
		population[i] = problem->generateGoodRandomSolution();
	}
}

CDiffEvol::CDiffEvol(CProblem * prob)
{
	problem = prob;
	population.resize(DEFAULT_POP_SIZE);
	for (int i = 0; i < population.size(); i++)
	{
		population[i] = problem->generateGoodRandomSolution();
	}
}

CDiffEvol::CDiffEvol(int numOfIndividualsInPop)
{
	problem = new CMscnProblem();
	problem->generateInstance();
	population.resize(numOfIndividualsInPop);
	for (int i = 0; i < population.size(); i++)
	{
		population[i] = problem->generateGoodRandomSolution();
	}
}

CDiffEvol::CDiffEvol(int iNumberOfIndInPop, CProblem* prob)
{
	problem = prob;
	
	population.resize(iNumberOfIndInPop);
	for (int i = 0; i < population.size(); i++)
	{
		population[i] = problem->generateGoodRandomSolution();
	}
}

CDiffEvol::~CDiffEvol()
{
}

void CDiffEvol::reinitializePop(int iNumberOfIndInPop)
{
	population.resize(iNumberOfIndInPop);
	for (int i = 0; i < iNumberOfIndInPop; i++)
	{
		population[i] = problem->generateGoodRandomSolution();
	}
}

void  CDiffEvol::search() 
{
	int counter = 0;
	while (counter < NUMBER_OF_EVALUATIONS)
	{
		for (int i = 0; i < population.size(); i++)
		{
			int j = 0;
			double*  ind = new double[problem->getSolutionSize()];
			double* indNew;
			
			while (j < (NUMBER_OF_EVALUATIONS / population.size()))
			{
				ind = population[i];
				double* baseInd = problem->generateGoodRandomSolution(); 
				double* addInd0 = problem->generateGoodRandomSolution();
				double* addInd1 = problem->generateGoodRandomSolution();
				 indNew = new double[problem->getSolutionSize()];

				if (individualsAreDifferent(ind, baseInd, addInd0, addInd1))
				{

					for (int geneOffset = 0; geneOffset < problem->getSolutionSize(); geneOffset++)
					{

						CRandom cRand;
						if (cRand.getDouble(0, 1) < CROSS_PROB)
						{
							if ((addInd0[geneOffset] - addInd1[geneOffset] < 0))
								indNew[geneOffset] = baseInd[geneOffset] + DIFF_WEIGHT * (addInd1[geneOffset] - addInd0[geneOffset]);
							else
								indNew[geneOffset] = baseInd[geneOffset] + DIFF_WEIGHT * (addInd0[geneOffset] - addInd1[geneOffset]);
						}
						else
						{
							indNew[geneOffset] = (ind)[geneOffset];
						}
					}
					//cout << "quality indNEW: " << problem->getQuality(indNew) << endl;
					//cout << "quality ind: " << problem->getQuality(ind) << endl;
					counter += 2;
					j += 2;
					if (problem->getQuality(indNew) >= problem->getQuality(ind))
					{
						if (problem->constraintsSatisfied(indNew))
						{
							//c_problem.fixSolution(indNew, c_problem.iGetSizeOfSolution());
							
							
							copySolution(&population[i], &indNew, problem->getSolutionSize());
							delete indNew;
						}
						else
						{
							problem->fixSolution(indNew);
							copySolution(&population[i], &indNew, problem->getSolutionSize());
							delete indNew;
						}
					}
					
				}
			}
		}
	}
}

double * CDiffEvol::getBest()
{
	search();
	double* best = population[0];
	for (int i = 1; i < population.size(); i++)
	{
		if(problem->getQuality(population[i])>problem->getQuality(best))
			best = population[i];
	}
	return best;
}

bool CDiffEvol::individualsAreDifferent(double * ind, double * baseInd, double * addInd0, double * addInd1)
{

	return areDiff(ind, baseInd) && areDiff(baseInd, addInd0) && areDiff(addInd0, addInd1) && areDiff(addInd1, ind);
}

bool CDiffEvol::areDiff(double * dArr0, double * dArr1)
{
	for (int i = 0; i < problem->getSolutionSize(); i++)
	{
		if (dArr0[i] != dArr1[i])
			return true;
	}
	return false;
}

void CDiffEvol::copySolution(double** dSol, double ** dOther, int iSize)
{
	delete[] * dSol;
	*dSol = new double[iSize];
	for (int i = 0; i < iSize; i++)
	{
		(*dSol)[i] = (*dOther)[i];
	}
}
