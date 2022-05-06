#pragma once
#include <Windows.h>
#include "Utils.h"
class Timer
{
	ULONGLONG startTime;
	ULONGLONG endTime;
public:
	Timer(int endTime) { this->endTime = endTime; startTime = 0; }

	void Start() { startTime = GetTickCount64(); }
	void Reset() { startTime = GetTickCount64(); }
	void Stop() { startTime = 0; }

	bool IsTimeUp() {

		return (GetTickCount64() - startTime) > endTime;
	}
	ULONGLONG Timeleft() {
		return GetTickCount64() - startTime;
	}
	ULONGLONG GetStartTime() { return startTime; }
	ULONGLONG GetEndTime() { return endTime; }
	void SetEndTime(int temp) { endTime = temp; }
};