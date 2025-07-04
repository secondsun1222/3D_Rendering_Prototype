#pragma once

#include "pch.h"

class TimeManager
{
	SINGLETON(TimeManager)

public:
	double			DeltaTime;

private:
	LARGE_INTEGER	initCount;	//start time of the program
	LARGE_INTEGER	prevCount;	//last update of the program
	LARGE_INTEGER	curCount;	//current update of the program
	LARGE_INTEGER	frequency;	//frequency of QueryPerformanceCounter

public:
	void init();
	void update();

};