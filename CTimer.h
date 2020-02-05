#pragma once
#include <windows.h>
class CTimer
{
public:
	CTimer();
	~CTimer();

	void startTimer();
	void endTimer();
	double getTime();
private:
	LARGE_INTEGER li_start;
	LARGE_INTEGER li_end;
	LARGE_INTEGER li_freq;
	
};

