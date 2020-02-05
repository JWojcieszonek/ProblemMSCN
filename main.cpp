#include "CMscnProblem.h"
#include "CRandom.h"
#include "CRandomSearch.h"
#include "CDiffEvol.h"
#include "CTimer.h"
int main()
{
	CMscnProblem test = CMscnProblem();
	test.loadProblem("problem0.txt");
	test.changeSolutionToMatrix(test.generateGoodRandomSolution());
	test.saveSolution("solution0.txt");
	CMscnProblem test2 = CMscnProblem();
	test2.generateInstance();
	test2.saveProblem("problem1.txt");
	
	
	CDiffEvol testEvol = CDiffEvol(&test);
	CRandomSearch testRand = CRandomSearch(&test);

	CTimer timer = CTimer();
	timer.startTimer();
	cout << "evol: " << testEvol.getProblem()->getQuality(testEvol.getBest()) <<endl;
	timer.endTimer();
	cout << " evol time: " << timer.getTime() << endl;
	timer.startTimer();
	cout << "rand: " << testRand.getProblem()->getQuality(testRand.getBest()) << endl;
	timer.endTimer();
	cout << " rand time: " << timer.getTime() << endl;

	return 0;
}