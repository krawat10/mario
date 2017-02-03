#ifndef Timer_H
#define Timer_H
#include "Timer.cpp"
class Timer
{
public:
	Timer();
	~Timer();
	void InitalizeTime();
	double ElapsedTime();
	double TimeToEnd();
	double WorldTime = 0;
	double animation_time = 0;
private:
	double TimeOne = 0;
	double TimeTwo;
	double buff;
};


#endif 