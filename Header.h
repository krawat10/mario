#ifndef TIMER_H
#define TIMER_H
#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#define STEP '1' // przeszkoda (do buffera)
#define HOLE 47  // -1 (do buffera)
#define GROUND '0' //zwyk³e pod³o¿e (do buffera)
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define MAX_JUMP	150
#define ONE_STEP_HEIGHT 50 //wysokoœæ przeszkody lub -(dziury)
#define CHAR_TO_INT 48
#define JUMP_SPEED 150
#define LEFT 'L'
#define RIGHT 'R'
#define UP 'U'
#define MAP_WIDTH 3000
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

void DrawString(SDL_Surface *screen, int x, int y, const char *text,
	SDL_Surface *charset);
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color);
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor);
class Timer
{
public:
	double WorldTime;
	double animation_time;
	Timer();
	~Timer();
	void InitalizeTime();
	double ElapsedTime();
	double TimeToEnd();
private:
	double TimeOne;
	double TimeTwo;
	double buff;
};

void KeyHandler(SDL_Event *event, double *speed, char *dir, int *wait, bool *active_jump,
	int *quit, double y_pos, double distance, char *CollisionBuffer, bool* loss);
void EventHandler(SDL_Event event, double *speed, char *dir, int *wait, bool *active_jump,
	int *quit, double y_pos, double distance, char *CollisionBuffer, bool* loss);

void InfoText(char *text, Timer time, double fps, double distance, SDL_Surface *screen, SDL_Surface *charset);
void Menu(char *text, SDL_Surface *screen, SDL_Surface *charset, SDL_Texture *scrtex, SDL_Renderer *renderer, SDL_Event *event, int *quit, Timer *time);

void FreeSurface(SDL_Surface *charset, SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer, SDL_Window *window);
void Render(SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer);
void InitSDL(int *rc, SDL_Window* window, SDL_Renderer *renderer, SDL_Surface *screen, SDL_Texture *scrtex);

void start(char* mapCollision);
int jump(double* heigth, bool *active, char* Collision, double location, char* direction);
bool check_way(double position, char* mapCollision, double Yposition, char direction);
int Normal_Jump(double *heigth, bool *active_jump, char* direction, double current_y_position);
int Jump_On_Obstacle(double *heigth, bool *active_jump, char* direction);
void ChangeEndControls(int* frames, double* current_mario_y_pos, bool *active_jump, char* CollisionBuffer, double distance, char* direction, double* current_jump_speed);

void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);
void initcontrolers(bool* active_jump, int* quit, int* frames, int* wait, char* direction, double* fpsTimer, double* fps, double* distance, double* map_speed,
	double* current_jump_speed, double* current_mario_y_pos, bool* loss);
bool changecontrols(double *delta, Timer *time, double *distance, double* map_speed, double* current_jump_speed, double* current_mario_y_pos,
	double* fpsTimer, bool* loss, bool* active_jump);
void FpsControls(double* fps, int* frames, double *fpsTimer);

void animation(SDL_Surface *screen, SDL_Surface *sprite[6], int x, int y, Timer* time, char direction);
void DrawMario(SDL_Surface * screen, SDL_Surface * sprite[6], int x, int y, Timer* time, double *map_speed, int* freez, char direction);
void InitMario(int size, SDL_Surface *mario[]);
#endif 