#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
	//#ifdef BIT64
	//#include"./sdl64/include/SDL.h"
	//#include"./sdl64/include/SDL_main.h"
	//#else
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
	//#endif
}


#include "Header.h"
bool up_jump = true;  // okreœlenie czy opada czy unosi sie 
double current_y_position = 0;  //okreœlenie czy w danym miejscu jest przeszkoda
int animation_sequences = 0;
double oldTime = 0;
char last_direction = 0;

//animacja postaci
void animation(SDL_Surface *screen, SDL_Surface *sprite[6], int x, int y, Timer *time, char direction)
{
	if (direction == RIGHT)
	{
		switch (animation_sequences)
		{
		case 0:DrawSurface(screen, sprite[1], x, y); break;
		case 1:DrawSurface(screen, sprite[2], x, y); break;
		case 2:DrawSurface(screen, sprite[3], x, y); break;
		case 3:DrawSurface(screen, sprite[4], x, y); break;
		case 4:DrawSurface(screen, sprite[5], x, y); break;
		default:
			break;
		}
	}
	if (direction == LEFT)
	{
		switch (animation_sequences)
		{
		case 0:DrawSurface(screen, sprite[6], x, y); break;
		case 1:DrawSurface(screen, sprite[7], x, y); break;
		case 2:DrawSurface(screen, sprite[8], x, y); break;
		case 3:DrawSurface(screen, sprite[9], x, y); break;
		case 4:DrawSurface(screen, sprite[10], x, y); break;
		default:
			break;
		}
	}
	if ((*time).WorldTime - (*time).animation_time > 0.1)
	{
		animation_sequences++;
		(*time).animation_time = (*time).WorldTime;
	}

	if (animation_sequences == 5) animation_sequences = 0; //zmiana grafik (animacja)


}

//za³adowanie grafik mario
void InitMario(int size, SDL_Surface *mario[])
{
	mario[0] = SDL_LoadBMP("./mario.bmp");
	mario[1] = (SDL_LoadBMP("./mario1.bmp"));
	mario[2] = (SDL_LoadBMP("./mario2.bmp"));
	mario[3] = (SDL_LoadBMP("./mario3.bmp"));
	mario[4] = (SDL_LoadBMP("./mario4.bmp"));
	mario[5] = (SDL_LoadBMP("./mario5.bmp"));
	mario[6] = (SDL_LoadBMP("./mario6.bmp"));
	mario[7] = (SDL_LoadBMP("./mario7.bmp"));
	mario[8] = (SDL_LoadBMP("./mario8.bmp"));
	mario[9] = (SDL_LoadBMP("./mario9.bmp"));
	mario[10] = (SDL_LoadBMP("./mario10.bmp"));
	mario[11] = (SDL_LoadBMP("./jumpL.bmp"));
	mario[12] = (SDL_LoadBMP("./jumpR.bmp"));
}

//rysowanie mario
void DrawMario(SDL_Surface * screen, SDL_Surface * sprite[6], int x, int y, Timer *time, double *map_speed, int* freez, char direction)
{
	if (direction == UP)  //podczas skoku
	{
		switch (last_direction)
		{
		case(LEFT) : DrawSurface(screen, sprite[11], x, y); break;
		case(RIGHT) : DrawSurface(screen, sprite[12], x, y); break;
		default:DrawSurface(screen, sprite[12], x, y); break;
		}
	}
	else if (*map_speed != 0 || *freez > 1)  //poruszanie sie
	{
		animation(screen, sprite, x, y, time, direction);
		*freez = *freez - 1;
		last_direction = direction;  //zapamietanie pozycji po skoku
	}
	else  //stanie w miejscu
	{
		switch (last_direction) //zapamiêtana pozycja po skoku
		{
		case(LEFT) : DrawSurface(screen, sprite[6], x, y); break;
		case(RIGHT) : DrawSurface(screen, sprite[0], x, y); break;
		default:DrawSurface(screen, sprite[0], x, y); break;
		}
	}
	*map_speed = 0;
}


//funkcja ogólna odpowiedzialna za skok
int jump(double* heigth, bool* active_jump, char* Collision, double location, char* direction)
{
	//okreœlenie czy w danej lokalizacji jest przeszkoda/pod³o¿e/dziura i podanie jej wysokosci
	current_y_position = -ONE_STEP_HEIGHT*(CHAR_TO_INT - *(Collision + (-(int)(location * 10) + SCREEN_WIDTH / 2)));


	if (*active_jump && current_y_position > 0) Jump_On_Obstacle(heigth, active_jump, direction); //skoki na przeszkodach

	if (*active_jump)return Normal_Jump(heigth, active_jump, direction, current_y_position);  //normalny skok
	else if (current_y_position == -ONE_STEP_HEIGHT)  //spadanie do dziury jeœli nie jest w trakcie skoku
	{
		*direction = UP;
		return -JUMP_SPEED;
	}
	else if (*heigth > 0 && current_y_position == 0)  //spadanie z przeskód bez u¿ycia skoku
	{
		*direction = UP;
		*active_jump = true;
		up_jump = false;
		return -JUMP_SPEED;
	}
	else
	{
		return 0;
	}
}

int Normal_Jump(double *heigth, bool *active_jump, char* direction, double current_y_position) {

	if (*heigth < 0)  //gdy  opada i jest tu¿ przy powierzchni
	{
		if (current_y_position == -ONE_STEP_HEIGHT)
		{
			*direction = UP;
			return -JUMP_SPEED;
		}
		up_jump = true;
		*heigth = 0;
		*active_jump = false;
		*direction = last_direction;
		return 0;
	}
	else if (*heigth < MAX_JUMP && up_jump)  // gdy skacze do góry
	{
		*direction = UP;
		return JUMP_SPEED;
	}
	else if (*heigth >= MAX_JUMP)  // gdy osi¹gnie max wysokosc skoku
	{
		*direction = UP;
		*heigth = MAX_JUMP;
		up_jump = false;
		return -JUMP_SPEED;
	}
	else if (*heigth < MAX_JUMP && !up_jump)  // gdy opada
	{
		*direction = UP;
		return -JUMP_SPEED;
	}
	else
	{
		up_jump = true;
		*heigth = 0;
		*active_jump = false;

		return 0;
	}
}

//skoki na przeszkodach
int Jump_On_Obstacle(double *heigth, bool *active_jump, char* direction) {

	if (*heigth > MAX_JUMP) //gdy ju¿ sie wzniesie ponad maksymaln¹ wysokoœæ
	{
		up_jump = false;
		return -JUMP_SPEED;
	}
	else if (*heigth >= 0 && up_jump) // wyskakiwanie w góre (na pocz¹tku)
	{
		*direction = UP;
		return JUMP_SPEED;
	}
	else if (*heigth < current_y_position && !up_jump)  //gdy ju¿ opadnie na przeszkode
	{
		*heigth = current_y_position;
		up_jump = true;
		*active_jump = false;
		*direction = last_direction;
		return 0;
	}
	else
	{
		return 0;
	}
}