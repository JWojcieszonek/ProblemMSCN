#pragma once
#include <vector>
#include <iostream>
#include"CRandom.h"
#include "CProblem.h"
using namespace std;

class CMscnProblem: public CProblem
{
public:
	CMscnProblem();
	CMscnProblem(int d, int f, int m, int s);
	~CMscnProblem();

	void initialize();
	void setMinMax();

	bool constraintsSatisfied(double* solution);
	double getQuality(double* solution);
	double calculateTransCost();
	double calculateDealsCost();
	double calculateProfit();

	void changeSolutionToMatrix(double* solution);
	void fixSolution(double* solution);

	void generateInstance(int seed);
	void generateInstance();

	bool loadProblem(string fileName);
	bool saveProblem(string fileName);
	bool saveSolution(string fileName);

	bool setNumOfSup(int d);
	bool setNumOfFac(int f);
	bool setNumOFWar(int m);
	bool setNumOfShop(int s);

	bool setValueVector(int index, vector<double> &vector, double val);
	bool setValueMatrix(int row, int col, vector<vector<double>> &matrix, double val);
	vector<vector<double>> &getSupMinMax();
	vector<vector<double>> &getFacMinMax();
	vector<vector<double>> &getWarMinMax();
	
	vector<double> flattenMatrix( vector<vector<double>> matrix);
	double* generateRandomSolution();
	double* generateGoodRandomSolution();

	int getNumOfSup();
	int getNumOfFac();
	int getNumOfWar();
	int getNumOfShops();

	double getSupProd(int index);
	double getFacProd(int index);
	double getWarProd(int index);
	double getShopProd(int index);


	int getSolutionSize();
	bool isPositive(double* solution);


private:
	int numOfSup; //liczba dostawców, fabryk itd.
	int numOfFac;
	int numOfWar;
	int numOfShops;

	vector<double> supProductivity; // sumaryczne iloœæ surowca, produktów
	vector<double> facProductivity;
	vector<double> warProductivity;
	vector<double> shopProductivity;

	vector<vector<double>> supFacTransCost; // koszty transportu
	vector<vector<double>> facWarTransCost;
	vector<vector<double>> warShopTransCost;

	vector<double> supDealCost; //koszty umów
	vector<double> facDealCost;
	vector<double> warDealCost;

	vector<double> shopProfit; //profit per product

	vector<vector<double>> supMinMax; //ograniczenia
	vector<vector<double>> facMinMax;
	vector<vector<double>> warMinMax;

	vector<vector<double>> xSupFac;// do wczytania rozwi¹zania
	vector<vector<double>> xFacWar;
	vector<vector<double>> xWarShop;

	static const double DEFAULT_NUM_OF_SUP;
	static const double DEFAULT_NUM_OF_FAC;
	static const double DEFAULT_NUM_OF_WAR;
	static const double DEFAULT_NUM_OF_SHOPS;

	static const int MAX_LENGTH;

	static const double MIN_SD;
	static const double MAX_SD;
	static const double MIN_SF;
	static const double MAX_SF;
	static const double MIN_SM;
	static const double MAX_SM;
	static const double MIN_SS;
	static const double MAX_SS;

	static const double MIN_CD;
	static const double MAX_CD;
	static const double MIN_CF;
	static const double MAX_CF;
	static const double MIN_CM;
	static const double MAX_CM;

	static const double MIN_UD;
	static const double MAX_UD;
	static const double MIN_UF;
	static const double MAX_UF;
	static const double MIN_UM;
	static const double MAX_UM;
};

