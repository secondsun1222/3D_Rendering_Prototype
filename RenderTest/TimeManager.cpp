#include "TimeManager.h"

TimeManager::TimeManager()
	:initCount{}
	, prevCount{}
	, curCount{}
	, frequency{}
	, DeltaTime(0.)
{

}

TimeManager::~TimeManager()
{

}

void TimeManager::init()
{
	QueryPerformanceCounter(&initCount);
	prevCount = initCount;
	curCount = initCount;
	QueryPerformanceFrequency(&frequency);
}

void TimeManager::update()
{
	QueryPerformanceCounter(&curCount);

	//time elapsed between the current update and the last update
	DeltaTime = double(curCount.QuadPart - prevCount.QuadPart) / (double)frequency.QuadPart;

	prevCount = curCount;
}