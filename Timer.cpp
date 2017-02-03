#include"Header.h"
extern "C" {
	//#ifdef BIT64
	//#include"./sdl64/include/SDL.h"
	//#include"./sdl64/include/SDL_main.h"
	//#else
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
	//#endif
}
#define TIME 30


Timer::Timer()
{
	InitalizeTime();
};
Timer::~Timer() {};

//rozpoczêcie wyliczania
void Timer::InitalizeTime()
{
	this->WorldTime = 0;
	this->TimeOne = 0;
	this->TimeTwo = 0;
	this->TimeOne = SDL_GetTicks();  
}

//zwrócenie aktualnego czasu
double Timer::ElapsedTime() 
{
	this->TimeTwo = SDL_GetTicks();
	this->buff = (this->TimeTwo - this->TimeOne)* 0.001;
	this->TimeOne = this->TimeTwo;
	WorldTime += buff;
	return buff;
}

//zwraca czas do konca gry
double Timer::TimeToEnd()
{
	return TIME - this->WorldTime;
}

