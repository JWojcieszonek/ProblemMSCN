#include "CTimer.h"




CTimer::CTimer()
{
}


CTimer::~CTimer()
{
}

void CTimer::startTimer()
{
	QueryPerformanceFrequency(&li_freq);
	QueryPerformanceCounter(&li_start);
}

void CTimer::endTimer()
{
	QueryPerformanceCounter(&li_end);
}

double CTimer::getTime()
{
	double seconds = li_end.QuadPart - li_start.QuadPart;
	return seconds/li_freq.QuadPart;
}
