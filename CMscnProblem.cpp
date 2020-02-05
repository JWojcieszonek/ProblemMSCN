#include "CMscnProblem.h"

const int CMscnProblem::MAX_LENGTH = 100;

const double CMscnProblem::DEFAULT_NUM_OF_SUP=1;
const double CMscnProblem::DEFAULT_NUM_OF_FAC=2;
const double CMscnProblem::DEFAULT_NUM_OF_WAR=1;
const double CMscnProblem::DEFAULT_NUM_OF_SHOPS=4;

const double CMscnProblem::MIN_SD = 50;
const double CMscnProblem::MAX_SD = 100;
const double CMscnProblem::MIN_SF = 10;
const double CMscnProblem::MAX_SF = 50;
const double CMscnProblem::MIN_SM = 5;
const double CMscnProblem::MAX_SM = 40;
const double CMscnProblem::MIN_SS = 30;
const double CMscnProblem::MAX_SS = 100;

const double CMscnProblem::MIN_CD = 0;
const double CMscnProblem::MAX_CD = 20;
const double CMscnProblem::MIN_CF = 0;
const double CMscnProblem::MAX_CF = 20;
const double CMscnProblem::MIN_CM = 0;
const double CMscnProblem::MAX_CM = 20;

const double CMscnProblem::MIN_UD = 0;
const double CMscnProblem::MAX_UD = 15;
const double CMscnProblem::MIN_UF = 0;
const double CMscnProblem::MAX_UF = 35;
const double CMscnProblem::MIN_UM = 0;
const double CMscnProblem::MAX_UM = 25;

CMscnProblem::CMscnProblem()
{
	numOfSup = DEFAULT_NUM_OF_SUP;
	numOfFac = DEFAULT_NUM_OF_FAC;
	numOfWar = DEFAULT_NUM_OF_WAR;
	numOfShops = DEFAULT_NUM_OF_SHOPS;
	initialize();
}

CMscnProblem::CMscnProblem(int d, int f, int m, int s)
{
	numOfSup = d;
	numOfFac = f;
	numOfWar = m;
	numOfShops = s;

	initialize();
}

CMscnProblem::~CMscnProblem()
{
}

void CMscnProblem::initialize()
{
	supProductivity.resize(numOfSup);
	facProductivity.resize(numOfFac);
	warProductivity.resize(numOfWar);
	shopProductivity.resize(numOfShops);

	supFacTransCost.resize(numOfSup);
	for (int i = 0; i < numOfSup; i++)
		supFacTransCost[i].resize(numOfFac);

	facWarTransCost.resize(numOfFac);
	for (int i = 0; i < numOfFac; i++)
		facWarTransCost[i].resize(numOfWar);

	warShopTransCost.resize(numOfWar);
	for (int i = 0; i < numOfWar; i++)
		warShopTransCost[i].resize(numOfShops);

	supDealCost.resize(numOfSup);
	facDealCost.resize(numOfFac);
	warDealCost.resize(numOfWar);
	shopProfit.resize(numOfShops);

	supMinMax .resize(numOfSup);
	for (int i = 0; i < numOfSup; i++)
		supMinMax[i].resize(2 * numOfFac);

	facMinMax.resize(numOfFac);
	for (int i = 0; i < numOfFac; i++)
		facMinMax[i].resize(2 * numOfWar);

	warMinMax.resize(numOfWar);
	for (int i = 0; i < numOfWar; i++)
		warMinMax[i].resize(2 * numOfShops);

	xSupFac.resize(numOfSup);
	for (int i = 0; i < numOfSup; i++)
		xSupFac[i].resize(numOfFac);

	xFacWar.resize(numOfFac);
	for (int i = 0; i < numOfFac; i++)
		xFacWar[i].resize(numOfWar);

	xWarShop.resize(numOfWar);
	for (int i = 0; i < numOfWar; i++)
		xWarShop[i].resize(numOfShops);

	setMinMax();

}

void CMscnProblem::setMinMax()
{
	for (int i = 0; i < numOfSup; i++)
	{
		for (int j = 0; j <2* numOfFac; j++)
		{
			if (j % 2 == 0)
				supMinMax[i][j] = 0;
			else
				supMinMax[i][j] = supProductivity[i]; // productivity/numOfFac - zawsze spe³nione ograniczenia chyba?
		}
	}

	for (int i = 0; i < numOfFac; i++)
	{
		for (int j = 0; j < 2 * numOfWar; j++)
		{
			if (j % 2 == 0)
				facMinMax[i][j] = 0;
			else
				facMinMax[i][j] = facProductivity[i];
		}
	}

	for (int i = 0; i < numOfWar; i++)
	{
		for (int j = 0; j < 2 * numOfShops; j++)
		{
			if (j % 2 == 0)
				warMinMax[i][j] = 0;
			else
				warMinMax[i][j] = facProductivity[i];
		}
	}
}

bool CMscnProblem::constraintsSatisfied(double * solution)
{
	if (solution == NULL && !isPositive(solution) )
		return false;
	else
	{
		double sumOfProducts = 0;
		changeSolutionToMatrix(solution);

		// sprawdzenie z maksymaln¹ sum¹ surowców
		for (int i = 0; i < numOfSup; i++)
		{
			for (int j = 0; j < numOfFac; j++)
			{
				sumOfProducts += xSupFac[i][j];
			}
			if (sumOfProducts > supProductivity[i])
			{
				//cout << "suma XDF[" << i << "] > SD[" << i << "]" << endl;
				return false;
			}

			sumOfProducts = 0;
		}

		for (int i = 0; i < numOfFac; i++)
		{
			for (int j = 0; j < numOfWar; j++)
			{
				sumOfProducts += xFacWar[i][j];
			}
			if (sumOfProducts > facProductivity[i])
			{
				//cout << "suma XFM[" << i << "] > SF[" << i << "]" << endl;
				return false;

			}
			sumOfProducts = 0;
		}
		for (int i = 0; i < numOfWar; i++)
		{
			for (int j = 0; j < numOfShops; j++)
			{
				sumOfProducts += xWarShop[i][j];
			}

			if (sumOfProducts > warProductivity[i])
			{
				//cout << "suma XMS[" << i << "] > SM[" << i << "]" << endl;
				return false;

			}
			sumOfProducts = 0;
		}

		for (int i = 0; i < numOfShops; i++)
		{
			for (int j = 0; j < numOfWar; j++)
			{
				sumOfProducts += xWarShop[j][i];
			}
			if (sumOfProducts > shopProductivity[i])
			{
				//cout << "suma XMS[" << i << "] > SS[" << i << "]" << endl;
				return false;

			}
			sumOfProducts = 0;
		}
		//sprawdzenie czy nie przyjmowanie wiecej niz wysy³ane
		double produced = 0;
		double received = 0;
		for (int i = 0; i < numOfFac; i++)
		{
			for (int j = 0; j < numOfSup; j++)
			{
				produced += xSupFac[j][i];
			}
			for (int j = 0; j < numOfWar; j++)
			{
				received += xFacWar[i][j];
			}

			if (received > produced)
			{
				//cout << "received XDF[" << i << "] > produced[" << i << "]" << endl;
				return false;

			}
			produced = 0.0;
			received = 0.0;
		}
		for (int i = 0; i < numOfWar; i++)
		{
			for (int j = 0; j < numOfFac; j++)
			{
				produced += xFacWar[j][i];
			}
			for (int j = 0; j < numOfShops; j++)
			{
				received += xWarShop[i][j];
			}
			if (received > produced)
			{
				//cout << "received XFM[" << i << "] > produced XMS[" << i << "]" << endl;
				return false;

			}
		}
		
		return true;
	}
}

double CMscnProblem::getQuality(double * solution)
{
	if (solution != NULL && isPositive(solution))
	{
		
		if (constraintsSatisfied(solution))
		{
			changeSolutionToMatrix(solution);
		}
		else
		{
			fixSolution(solution);
			changeSolutionToMatrix(solution);
		}

	}
	return calculateProfit() - (calculateTransCost() + calculateDealsCost());
}

double CMscnProblem::calculateTransCost()
{
	double transCost = 0;
	for (int i = 0; i < numOfSup; i++)
	{
		for (int j = 0; j < numOfFac; j++)
		{
			transCost += (supFacTransCost[i][j] * xSupFac[i][j]);
		}
	}

	for (int i = 0; i < numOfFac; i++)
	{
		for (int j = 0; j < numOfWar; j++)
		{
			transCost += (facWarTransCost[i][j] * xFacWar[i][j]);
		}
	}

	for (int i = 0; i < numOfWar; i++)
	{
		for (int j = 0; j < numOfShops; j++)
		{
			transCost += (warShopTransCost[i][j] * xWarShop[i][j]);
		}
	}
	return transCost;
}

double CMscnProblem::calculateDealsCost()
{
	double dealsCost = 0;
	
	for (int i = 0; i < numOfSup; i++)
	{
		for (int j = 0; j < numOfFac; j++)
		{
			if (xSupFac[i][j] > 0)
				dealsCost += supDealCost[i];

		}
	}
	for (int i = 0; i < numOfFac; i++)
	{
		for (int j = 0; j < numOfWar; j++)
		{
			if (xFacWar[i][j] > 0)
				dealsCost += facDealCost[i];

		}
	}
	for (int i = 0; i < numOfWar; i++)
	{
		for (int j = 0; j < numOfShops; j++)
		{
			if (xWarShop[i][j] > 0)
				dealsCost += warDealCost[i];

		}
	}
	return dealsCost;
}

double CMscnProblem::calculateProfit()
{
	double profit = 0;
	for (int i = 0; i < numOfWar; i++)
	{
		for (int j = 0; j < numOfShops; j++)
		{
			profit += xWarShop[i][j] * shopProfit[j];

		}
	}
	return profit;
}

void CMscnProblem::changeSolutionToMatrix(double * solution)
{
	int iIndex = 0;
	for (int i = 0; i < numOfSup; i++)
	{
		for (int j = 0; j < numOfFac; j++)
		{
			xSupFac[i][j] = solution[iIndex];
			iIndex++;
		}
	}

	for (int i = 0; i < numOfFac; i++)
	{
		for (int j = 0; j < numOfWar; j++)
		{
			xFacWar[i][j] = solution[iIndex];
			iIndex++;
		}
	}

	for (int i = 0; i < numOfWar; i++)
	{
		for (int j = 0; j < numOfShops; j++)
		{
			xWarShop[i][j] = solution[iIndex];
			iIndex++;
		}
	}
}

void CMscnProblem::fixSolution(double * solution)
{
	//cout << "correct: " << isPositive(solution) << endl;
	//cout << "cosntrints0: " << constraintsSatisfied(solution) << endl;
	
	//stworzenie macierzy na rozwiaznie
	vector < vector<double>> xSupFacMat = vector < vector<double>>(getNumOfSup());
	for (int i = 0; i < getNumOfSup(); i++)
	{
		xSupFacMat[i].resize(getNumOfFac());
	}
	vector < vector<double>> xFacWarMat = vector < vector<double>>(getNumOfFac());
	for (int i = 0; i < getNumOfFac(); i++)
	{
		xFacWarMat[i].resize(getNumOfWar());
	}
	vector < vector<double>> xWarShopMat = vector < vector<double>>(getNumOfWar());
	for (int i = 0; i < getNumOfWar(); i++)
	{
		xWarShopMat[i].resize(getNumOfShops());
	}
	//zmiana rozwiazania w macierze
	int iIndex = 0;
	for (int i = 0; i < numOfSup; i++)
	{
		for (int j = 0; j < numOfFac; j++)
		{
			xSupFacMat[i][j] = solution[iIndex];
			iIndex++;
		}
	}

	for (int i = 0; i < numOfFac; i++)
	{
		for (int j = 0; j < numOfWar; j++)
		{
			xFacWarMat[i][j] = solution[iIndex];
			iIndex++;
		}
	}

	for (int i = 0; i < numOfWar; i++)
	{
		for (int j = 0; j < numOfShops; j++)
		{
			xWarShopMat[i][j] = solution[iIndex];
			iIndex++;
		}
	}
	//naprawienie xd
	double sumSent;;// suma od jednego dostawcy
	double sumReceived;
	for (int i = 0; i < getNumOfSup(); i++)
	{
		sumSent = 0;
		for (int j = 0; j < getNumOfFac(); j++) // suma wys³ana przez danego dostawce
		{
			sumSent += xSupFacMat[i][j];
		}
		if (sumSent > supProductivity[i])
		{
			double surplus = sumSent - supProductivity[i];
			for (int j = 0; j < getNumOfFac(); j++) // odjecie z kazdej fabryki takiej samej czesci nadwyzki
			{
				xSupFacMat[i][j] = (xSupFacMat[i][j] - xSupFacMat[i][j]*(surplus / sumSent));
			}
		}
	}
	//naprawienie xf czy przekracza liczbe dostarczon
	for (int i = 0; i < getNumOfFac(); i++)
	{
		sumReceived = 0;
		sumSent = 0;
		for (int j = 0; j < getNumOfSup(); j++) // zliczenie towaru od danego dostawcy
		{
			sumReceived += xSupFacMat[j][i];
		}
		for (int j = 0; j < getNumOfWar(); j++) // zliczenie towaru do danego magazynu
		{
			sumSent += xFacWarMat[i][j];
		}
		if (sumSent > sumReceived)
		{
			double surplus = sumSent - sumReceived;
			for (int j = 0; j < getNumOfWar(); j++)
			{
				xFacWarMat[i][j] = (xFacWarMat[i][j] - xFacWarMat[i][j]*(surplus / sumSent));
			}
		}
	}
	//naprawienie xf gdy przekracza moc produkcyjna
	for (int i = 0; i < getNumOfFac(); i++)
	{
		sumSent = 0;
		for (int j = 0; j < getNumOfWar(); j++)
		{
			sumSent += xFacWarMat[i][j];
		}
		if (sumSent > facProductivity[i])
		{
			double surplus = sumSent - facProductivity[i];
			for (int j = 0; j < getNumOfWar(); j++) // odjecie z kazdej fabryki takiej samej czesci nadwyzki
			{
				xFacWarMat[i][j] = (xFacWarMat[i][j] - xFacWarMat[i][j] * (surplus / sumSent));
			}

		}
	}
	//naprawa xm 
	for (int i = 0; i < getNumOfWar(); i++)
	{
		sumReceived = 0;
		sumSent = 0;
		for (int j = 0; j < getNumOfFac(); j++) // zliczenie towaru z danej fabryki
		{
			sumReceived += xFacWarMat[j][i];
		}
		for (int j = 0; j < getNumOfShops(); j++) // zliczenie towaru do danego sklepu
		{
			sumSent += xWarShopMat[i][j];
		}
		if (sumSent > sumReceived)
		{
			double surplus = sumSent - sumReceived;
			for (int j = 0; j < getNumOfShops(); j++)
			{
				xWarShopMat[i][j] = (xWarShopMat[i][j] - xWarShopMat[i][j]*(surplus / sumSent));
			}
		}
	}
	//naprawienie xm gdy przekracza moc produkcyjna magazynów
	for (int i = 0; i < getNumOfWar(); i++)
	{
		sumSent = 0;
		for (int j = 0; j < getNumOfShops(); j++)
		{
			sumSent += xWarShopMat[i][j];
		}
		if (sumSent > warProductivity[i])
		{
			double surplus = sumSent - warProductivity[i];
			for (int j = 0; j < getNumOfWar(); j++) // odjecie z kazdej fabryki takiej samej czesci nadwyzki
			{
				xWarShopMat[i][j] = (xWarShopMat[i][j] - xWarShopMat[i][j] * (surplus / sumSent));
			}

		}
	}
	// naprawienie xm gdy przekracza moc produkcyjna sklepów
	for (int i = 0; i < getNumOfShops(); i++)
	{
		sumReceived = 0;
		for (int j = 0; j < getNumOfWar(); j++)
		{
			sumReceived += xWarShopMat[j][i];
		}
		if (sumReceived > shopProductivity[i])
		{
			double surplus = sumReceived - shopProductivity[i];
			for (int j = 0; j < getNumOfWar(); j++) 
			{
				xWarShopMat[j][i] = (xWarShopMat[j][i] - xWarShopMat[j][i] * (surplus / sumReceived));
			}

		}
	}
	vector<double> solution1;
	solution1.resize(getSolutionSize());
	int index = 0;
	for (int i = 0; i < getNumOfSup(); i++)
	{
		for (int j = 0; j < getNumOfFac(); j++)
		{
			solution[index] = xSupFacMat[i][j];
			index++;
		}
	}
	for (int i = 0; i < getNumOfFac(); i++)
	{
		for (int j = 0; j < getNumOfWar(); j++)
		{
			solution[index] = xFacWarMat[i][j];
			index++;
		}
	}
	for (int i = 0; i < getNumOfWar(); i++)
	{
		for (int j = 0; j < getNumOfShops(); j++)
		{
			solution[index] = xWarShopMat[i][j];
			index++;
		}
	}
	//cout << "correct: " << isPositive(solution) << endl;
	//cout << "cosntrints1: " << constraintsSatisfied(solution) << endl;
	
}

void CMscnProblem::generateInstance(int seed)
{
	CRandom rand(seed);

	//sumy surowców
	for (int i = 0; i < numOfSup; i++)
		supProductivity[i] = rand.getDouble(MIN_SD, MAX_SD);

	for (int i = 0; i < numOfFac; i++)
		facProductivity[i] = rand.getDouble(MIN_SF, MAX_SF);

	for (int i = 0; i < numOfWar; i++)
		warProductivity[i] = rand.getDouble(MIN_SM, MAX_SM);

	for (int i = 0; i < numOfShops; i++)
		shopProductivity[i] = rand.getDouble(MIN_SS, MAX_SS);
	//koszty transportu
	for (int i = 0; i < numOfSup; i++)
	{
		for (int j = 0; j < numOfFac; j++)
			supFacTransCost[i][j] = rand.getDouble(MIN_CD, MAX_CD);
	}

	for (int i = 0; i < numOfFac; i++)
	{
		for (int j = 0; j < numOfWar; j++)
			facWarTransCost[i][j] = rand.getDouble(MIN_CF, MAX_CF);
	}

	for (int i = 0; i < numOfWar; i++)
	{
		for (int j = 0; j < numOfShops; j++)
			warShopTransCost[i][j] = rand.getDouble(MIN_CM, MAX_CM);
	}
	//koszty umów
	for (int i = 0; i < numOfSup; i++)
		supDealCost[i] = rand.getDouble(MIN_UD, MAX_UD);

	for (int i = 0; i < numOfFac; i++)
		facDealCost[i] = rand.getDouble(MIN_UF, MAX_UF);

	for (int i = 0; i < numOfWar; i++)
		warDealCost[i] = rand.getDouble(MIN_UM, MAX_UM);
	//profit
	for (int i = 0; i < numOfShops; i++)
		shopProfit[i] = rand.getDouble(MIN_UM, MAX_UM);
}

void CMscnProblem::generateInstance()
{
	CRandom rand;
	//sumy surowców
	for (int i = 0; i < numOfSup; i++)
		supProductivity[i] = rand.getDouble(MIN_SD, MAX_SD);

	for (int i = 0; i < numOfFac; i++)
		facProductivity[i] = rand.getDouble(MIN_SF, MAX_SF);

	for (int i = 0; i < numOfWar; i++)
		warProductivity[i] = rand.getDouble(MIN_SM, MAX_SM);

	for (int i = 0; i < numOfShops; i++)
		shopProductivity[i] = rand.getDouble(MIN_SS, MAX_SS);
	//koszty transportu
	for (int i = 0; i < numOfSup; i++)
	{
		for (int j = 0; j < numOfFac; j++)
			supFacTransCost[i][j] = rand.getDouble(MIN_CD, MAX_CD);
	}

	for (int i = 0; i < numOfFac; i++)
	{
		for (int j = 0; j < numOfWar; j++)
			facWarTransCost[i][j] = rand.getDouble(MIN_CF, MAX_CF);
	}

	for (int i = 0; i < numOfWar; i++)
	{
		for (int j = 0; j < numOfShops; j++)
			warShopTransCost[i][j] = rand.getDouble(MIN_CM, MAX_CM);
	}
	//koszty umów
	for (int i = 0; i < numOfSup; i++)
		supDealCost[i] = rand.getDouble(MIN_UD, MAX_UD);

	for (int i = 0; i < numOfFac; i++)
		facDealCost[i] = rand.getDouble(MIN_UF, MAX_UF);

	for (int i = 0; i < numOfWar; i++)
		warDealCost[i] = rand.getDouble(MIN_UM, MAX_UM);
	//profit
	for (int i = 0; i < numOfShops; i++)
		shopProfit[i] = rand.getDouble(MIN_UM, MAX_UM);
	setMinMax();
}

bool CMscnProblem::loadProblem(string fileName)
{
	FILE* file = fopen(fileName.c_str(), "r");

	if (file != nullptr)
	{
		char charBuffer[MAX_LENGTH];
		int intBuffer = 0;
		float floatBuffer;

		//Ustawienie liczby dostawców, fabryk itd
		fscanf(file, "%s", charBuffer);
		fscanf(file, "%d", &intBuffer);
		numOfSup = intBuffer;

		fscanf(file, "%s", charBuffer);
		fscanf(file, "%d", &intBuffer);
		numOfFac = intBuffer;

		fscanf(file, "%s", charBuffer);
		fscanf(file, "%d", &intBuffer);
		numOfWar = intBuffer;

		fscanf(file, "%s", charBuffer);
		fscanf(file, "%d", &intBuffer);
		numOfShops = intBuffer;
		initialize();

		//ustawienie liczby surowców
		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfSup; i++)
		{
			fscanf(file, "%g", &floatBuffer);
			supProductivity[i] = floatBuffer;
		}

		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfFac; i++)
		{
			fscanf(file, "%g", &floatBuffer);
			facProductivity[i] = floatBuffer;
		}

		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfWar; i++)
		{
			fscanf(file, "%g", &floatBuffer);
			warProductivity[i] = floatBuffer;
		}

		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfShops; i++)
		{
			fscanf(file, "%g", &floatBuffer);
			shopProductivity[i] = floatBuffer;
		}
		//ustawienie kosztów transportu
		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfSup; i++) {
			for (int j = 0; j < numOfFac; j++) {
				fscanf(file, "%g", &floatBuffer);
				supFacTransCost[i][j] = floatBuffer;
			}
		}
		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfFac; i++) {
			for (int j = 0; j < numOfWar; j++) {
				fscanf(file, "%g", &floatBuffer);
				facWarTransCost[i][j] = floatBuffer;
			}
		}
		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfWar; i++) {
			for (int j = 0; j < numOfShops; j++) {
				fscanf(file, "%g", &floatBuffer);
				warShopTransCost[i][j] = floatBuffer;
			}
		}
		//ustawienie kosztów umów
		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfSup; i++) {
			fscanf(file, "%g", &floatBuffer);
			supDealCost[i] = floatBuffer;
		}

		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfFac; i++) {
			fscanf(file, "%g", &floatBuffer);
			facDealCost[i] = floatBuffer;
		}

		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfWar; i++) {
			fscanf(file, "%g", &floatBuffer);
			warDealCost[i] = floatBuffer;
		}
		//ustawienie przychodów
		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfShops; i++) {
			fscanf(file, "%g", &floatBuffer);
			shopProfit[i] = floatBuffer;
		}
		//ustawienie min max
		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfSup; i++) {
			for (int j = 0; j < numOfFac * 2; j++) {
				fscanf(file, "%g", &floatBuffer);
				supMinMax[i][j] = floatBuffer;
			}
		}
		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfFac; i++) {
			for (int j = 0; j < numOfWar * 2; j++) {
				fscanf(file, "%g", &floatBuffer);
				facMinMax[i][j] = floatBuffer;
			}
		}
		fscanf(file, "%s", charBuffer);
		for (int i = 0; i < numOfWar; i++) {
			for (int j = 0; j < numOfShops * 2; j++) {
				fscanf(file, "%g", &floatBuffer);
				warMinMax[i][j] = floatBuffer;
			}
		}
		return true;
	}
	else
		return false;
}

bool CMscnProblem::saveProblem(string fileName)
{
	FILE *file = fopen(fileName.c_str(), "w+");
	if (file != nullptr) {
		fprintf(file, "D %d\n", numOfSup);
		fprintf(file, "F %d\n", numOfFac);
		fprintf(file, "M %d\n", numOfWar);
		fprintf(file, "S %d\n", numOfShops);

		fprintf(file, "sd\n");
		for (int i = 0; i < supProductivity.size(); i++)
			fprintf(file, "%g ", supProductivity[i]);

		fprintf(file, "\nsf\n");
		for (int i = 0; i < facProductivity.size(); i++)
			fprintf(file, "%g ", facProductivity[i]);

		fprintf(file, "\nsm\n");
		for (int i = 0; i < warProductivity.size(); i++)
			fprintf(file, "%g ", warProductivity[i]);

		fprintf(file, "\nss\n");
		for (int i = 0; i < shopProductivity.size(); i++)
			fprintf(file, "%g ", shopProductivity[i]);

		fprintf(file, "\ncd\n");
		for (int i = 0; i < numOfSup; i++) {
			for (int j = 0; j < numOfFac; j++)
				fprintf(file, "%g ", supFacTransCost[i][j]);
			fprintf(file, "\n");
		}

		fprintf(file, "cf\n");
		for (int i = 0; i < numOfFac; i++) {
			for (int j = 0; j < numOfWar; j++)
				fprintf(file, "%g ", facWarTransCost[i][j]);
			fprintf(file, "\n");
		}

		fprintf(file, "cm\n");
		for (int i = 0; i < numOfWar; i++) {
			for (int j = 0; j < numOfShops; j++)
				fprintf(file, "%g ", warShopTransCost[i][j]);
			fprintf(file, "\n");
		}

		fprintf(file, "ud\n");
		for (int i = 0; i < numOfSup; i++)
			fprintf(file, "%g ", supDealCost[i]);

		fprintf(file, "\nuf\n");
		for (int i = 0; i < numOfFac; i++)
			fprintf(file, "%g ", facDealCost[i]);

		fprintf(file, "\num\n");
		for (int i = 0; i < numOfWar; i++)
			fprintf(file, "%g ", warDealCost[i]);

		fprintf(file, "\np\n");
		for (int i = 0; i < numOfShops; i++)
			fprintf(file, "%g ", shopProfit[i]);

		fprintf(file, "\nxdminmax\n");
		for (int i = 0; i < numOfSup; i++) {
			for (int j = 0; j < numOfFac * 2; j++)
				fprintf(file, "%g ", supMinMax[i][j]);
			fprintf(file, "\n");
		}

		fprintf(file, "xfminmax\n");
		for (int i = 0; i < numOfFac; i++) {
			for (int j = 0; j < numOfWar * 2; j++)
				fprintf(file, "%g ", facMinMax[i][j]);
			fprintf(file, "\n");
		}

		fprintf(file, "xmminmax\n");
		for (int i = 0; i < numOfWar; i++) {
			for (int j = 0; j < numOfShops * 2; j++)
				fprintf(file, "%g ", warMinMax[i][j]);
			fprintf(file, "\n");
		}

		fclose(file);
		return true;
	}
	return false;
}

bool CMscnProblem::saveSolution(string fileName)
{
	FILE *file = fopen(fileName.c_str(), "w+");
	if (file != nullptr) {
		fprintf(file, "D %d\n", numOfSup);
		fprintf(file, "F %d\n", numOfFac);
		fprintf(file, "M %d\n", numOfWar);
		fprintf(file, "S %d\n", numOfShops);

		fprintf(file, "xd\n");
		for (int i = 0; i < numOfSup; i++) {
			for (int j = 0; j < numOfFac; j++)
				fprintf(file, "%g ", xSupFac[i][j]);
			fprintf(file, "\n");
		}

		fprintf(file, "xf\n");
		for (int i = 0; i < numOfFac; i++) {
			for (int j = 0; j < numOfWar; j++)
				fprintf(file, "%g ", xFacWar[i][j]);
			fprintf(file, "\n");
		}

		fprintf(file, "xm\n");
		for (int i = 0; i < numOfWar; i++) {
			for (int j = 0; j < numOfShops; j++)
				fprintf(file, "%g ", xWarShop[i][j]);
			fprintf(file, "\n");
		}
		fclose(file);
		return true;
	}
	return false;
}

bool CMscnProblem::setNumOfSup(int d)
{
	if (d > 0 && d!=numOfSup)
	{
		numOfSup = d;
		initialize();
		return true;
	}
	else
		return false;
}

bool CMscnProblem::setNumOfFac(int f)
{
	if (f > 0 && f != numOfSup)
	{
		numOfFac = f;
		initialize();
		return true;
	}
	else
		return false;
}

bool CMscnProblem::setNumOFWar(int m)
{
	if (m > 0 && m != numOfSup)
	{
		numOfWar = m;
		initialize();
		return true;
	}
	else
		return false;
}

bool CMscnProblem::setNumOfShop(int s)
{
	if (s > 0 && s != numOfSup)
	{
		numOfShops = s;
		initialize();
		return true;
	}
	else
		return false;
}

bool CMscnProblem::setValueVector(int index, vector<double>& vector, double val)
{

	if (!vector.empty() && index >= 0 && index < vector.size())
	{
		vector[index] = val;
		return true;
	}
	else return false;
	return false;
}

bool CMscnProblem::setValueMatrix(int row, int col, vector<vector<double>>& matrix, double val)
{
	if (!matrix.empty() && row >= 0 && row < matrix.size() && !matrix[row].empty() && col >= 0 && col < matrix[row].size())
	{
		matrix[row][col] = val;
		return true;
	}
	else return false;
	return false;
}

vector<vector<double>>& CMscnProblem::getSupMinMax()
{
	return supMinMax;
}

vector<vector<double>>& CMscnProblem::getFacMinMax()
{
	return facMinMax;
}

vector<vector<double>>& CMscnProblem::getWarMinMax()
{
	return warMinMax;
}

vector<double> CMscnProblem::flattenMatrix(vector<vector<double>> matrix)
{
	vector<double> vector;
	int index = 0;
	vector.resize(matrix.size()*matrix[0].size());
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			vector[index] = matrix[i][j];
			index++;
		}
	}
	return vector;
}

double * CMscnProblem::generateRandomSolution()
{
	CRandom rand;
	bool constraints = false;
	double* solution = new double[getSolutionSize()];
	vector<double> supMinMax = flattenMatrix(getSupMinMax());
	vector<double> facMinMax = flattenMatrix(getFacMinMax());
	vector<double> warMinMax = flattenMatrix(getWarMinMax());


	int index = 0;
	for (int i = 0; i < supMinMax.size(); i++)
	{
		solution[index] = rand.getDouble(supMinMax[i], supMinMax[i + 1]);
		i++;
		index++;
	}
	for (int i = 0; i < facMinMax.size(); i++)
	{
		solution[index] = rand.getDouble(facMinMax[i], facMinMax[i + 1]); // zle bo rozne ksztaly macierzy
		i++;
		index++;

	}
	for (int i = 0; i < warMinMax.size(); i++)
	{
		solution[index] = rand.getDouble(warMinMax[i], warMinMax[i + 1]);
		i++;
		index++;
	}

	return solution;
}

double * CMscnProblem::generateGoodRandomSolution()
{
	CRandom rand;
	vector < vector<double>> xSupFacRand = vector < vector<double>>(getNumOfSup());
	for (int i = 0; i < getNumOfSup(); i++)
	{
		xSupFacRand[i].resize(getNumOfFac());
	}
	vector < vector<double>> xFacWarRand = vector < vector<double>>(getNumOfFac());
	for (int i = 0; i < getNumOfFac(); i++)
	{
		xFacWarRand[i].resize(getNumOfWar());
	}
	vector < vector<double>> xWarShopRand = vector < vector<double>>(getNumOfWar());
	for (int i = 0; i < getNumOfWar(); i++)
	{
		xWarShopRand[i].resize(getNumOfShops());
	}

	double sumOfXSupFac;
	double supProd;

	for (int i = 0; i < getNumOfSup(); i++)
	{
		sumOfXSupFac = 0;
		supProd = getSupProd(i);
		for (int j = 0; j < getNumOfFac(); j++)
		{
			double xdf;
			if (supProd - sumOfXSupFac > 0)
				xdf = rand.getDouble(0, supProd - sumOfXSupFac);
			else
				xdf = 0;
			sumOfXSupFac += xdf;
			xSupFacRand[i][j] = xdf;
		}
	}

	double sumOfXFacWar;
	double maxProdFromSup;
	double facProductivity;
	for (int i = 0; i < getNumOfFac(); i++)
	{
		facProductivity = getFacProd(i);
		sumOfXFacWar = 0;
		maxProdFromSup = 0;
		for (int j = 0; j < getNumOfSup(); j++)
		{
			maxProdFromSup += xSupFacRand[j][i];

		}
		for (int k = 0; k < getNumOfWar(); k++)
		{

			double xfm;
			if (maxProdFromSup < facProductivity)
			{
				if (maxProdFromSup - sumOfXFacWar > 0)
					xfm = rand.getDouble(0, maxProdFromSup - sumOfXFacWar);
				else
					xfm = 0;

			}
			else
			{
				if (facProductivity - sumOfXFacWar > 0)
					xfm = rand.getDouble(0, facProductivity - sumOfXFacWar);
				else
					xfm = 0;
			}
			sumOfXFacWar += xfm;
			xFacWarRand[i][k] = xfm;

		}

	}


	double sumOfWarShop = 0;
	double maxProdFromFac;
	double warProductivity;
	double shopProductivity;
	for (int i = 0; i < getNumOfWar(); i++)
	{
		warProductivity = getFacProd(i);
		sumOfWarShop = 0;
		maxProdFromFac = 0;
		for (int j = 0; j < getNumOfFac(); j++)
		{
			maxProdFromFac += xFacWarRand[j][i];

		}
		for (int k = 0; k < getNumOfShops(); k++)
		{
			//shopProductivity = problem.getShopProd(k);
			double xms;
			if (maxProdFromFac < warProductivity)
			{
				if (maxProdFromFac - sumOfWarShop > 0)
					xms = rand.getDouble(0, maxProdFromFac - sumOfWarShop);
				else
					xms = 0;

			}
			else
			{
				if (warProductivity - sumOfWarShop > 0)
					xms = rand.getDouble(0, warProductivity - sumOfWarShop);
				else
					xms = 0;
			}
			sumOfWarShop += xms;
			xWarShopRand[i][k] = xms;

		}

	}

	double* solution = new double[getSolutionSize()];
	int index = 0;
	for (int i = 0; i < getNumOfSup(); i++)
	{
		for (int j = 0; j < getNumOfFac(); j++)
		{
			solution[index] = xSupFacRand[i][j];
			index++;
		}
	}
	for (int i = 0; i < getNumOfFac(); i++)
	{
		for (int j = 0; j < getNumOfWar(); j++)
		{
			solution[index] = xFacWarRand[i][j];
			index++;
		}
	}
	for (int i = 0; i < getNumOfWar(); i++)
	{
		for (int j = 0; j < getNumOfShops(); j++)
		{
			solution[index] = xWarShopRand[i][j];
			index++;
		}
	}


	return solution;
}

int CMscnProblem::getNumOfSup()
{
	return numOfSup;
}

int CMscnProblem::getNumOfFac()
{
	return numOfFac;
}

int CMscnProblem::getNumOfWar()
{
	return numOfWar;
}

int CMscnProblem::getNumOfShops()
{
	return numOfShops;
}

double CMscnProblem::getSupProd(int index)
{
	return supProductivity[index];
}

double CMscnProblem::getFacProd(int index)
{
	return facProductivity[index];
}

double CMscnProblem::getWarProd(int index)
{
	return warProductivity[index];
}

double CMscnProblem::getShopProd(int index)
{
	return shopProductivity[index];
}

int CMscnProblem::getSolutionSize()
{
	return numOfSup*numOfFac+numOfFac*numOfWar+numOfWar*numOfShops;
}

bool CMscnProblem::isPositive(double * solution)
{
	for (int i = 0; i < getSolutionSize(); i++)
	{
		if (solution[i] < 0)
			return false;
	}

	return true;;
}
