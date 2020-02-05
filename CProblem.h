#pragma once

class CProblem
{
public:
	virtual double getQuality(double* solution)=0;
	virtual int getSolutionSize() = 0;
	virtual void generateInstance() = 0;
	virtual double* generateGoodRandomSolution() = 0;
	virtual bool constraintsSatisfied(double* solution) = 0;

	virtual int getNumOfSup()=0;
	virtual int getNumOfFac()=0;
	virtual int getNumOfWar()=0;
	virtual int getNumOfShops()=0;

	virtual double getSupProd(int index)=0;
	virtual double getFacProd(int index)=0;
	virtual double getWarProd(int index)=0;
	virtual double getShopProd(int index)=0;
	virtual vector<vector<double>> &getSupMinMax()=0;
	virtual vector<vector<double>> &getFacMinMax()=0;
	virtual vector<vector<double>> &getWarMinMax()=0;
	virtual vector<double> flattenMatrix(vector<vector<double>> matrix)=0;
	virtual void fixSolution(double* solution) = 0;
};
