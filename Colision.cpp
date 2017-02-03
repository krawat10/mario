#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include<math.h>
#include<stdio.h>
#include<string.h>
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

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

#define OBSTACLE_WIDTH 70
//inicjali
void start( char* mapCollision)  
{
	
	for (int i = 400; i < MAP_WIDTH; i++)
	{
		if (i % 300 == 0 && i != 0)
		{
			for (int j = 0; j<OBSTACLE_WIDTH;j++, i++)
			{
				*(mapCollision + i) = STEP;
			}
		}
		else
		{
			*(mapCollision + i) = GROUND;
		}
	}	
	for (int j = 0; j<OBSTACLE_WIDTH; j++)
	{
		*(mapCollision + j + 720) = HOLE;
	}
	*(mapCollision + MAP_WIDTH) = '\0';
}


//sprawdzanie czy nie ma przeszkód na drodze
bool check_way(double position, char* mapCollision, double Yposition, char direction)
{
	if (direction == LEFT)
	{
		if (*(mapCollision + (-(int)(position * 10) + SCREEN_WIDTH / 2 - 4)) == STEP && Yposition < ONE_STEP_HEIGHT) return false;  //wykrywanie przeszkody
		else if ((-position * 10 + SCREEN_WIDTH / 2)  < SCREEN_WIDTH/2) return false;						 //wykrywanie pocz¹tku mapy
		else return true;
	}
	else if (direction == RIGHT)
	{
		if (*(mapCollision + (-(int)(position * 10) + SCREEN_WIDTH / 2) + 4) == STEP && Yposition < ONE_STEP_HEIGHT) return false;
		else if ((-position * 10 + SCREEN_WIDTH) > MAP_WIDTH) return false;
		else return true;
	}	
	return false;
}


