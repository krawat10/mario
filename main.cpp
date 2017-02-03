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

#ifdef __cplusplus
extern "C"
#endif
char *CollisionBuffer = (char*)malloc(sizeof(char*) * 3000);

int main(int argc, char **argv) {
	start(CollisionBuffer);
	bool active_jump, loss;	
	int quit, frames, rc, wait;
	char direction, text[128];
	double delta, fpsTimer, fps, distance, map_speed, current_jump_speed, current_mario_y_pos;
	Timer time;
	SDL_Event event;
	SDL_Surface *screen, *charset, *eti, *mario[13];
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;
	//inicjacja SDL'ów
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, "Szablon do zdania drugiego 2014");
	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);
	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./cs8x8.bmp");
	eti = SDL_LoadBMP("./eti.bmp");	
	// inizjacja zmiennych (¿ycie, pocz¹tkowe po³o¿enie itp) oraz w³¹czenie menu
	initcontrolers(&active_jump, &quit, &frames, &wait, &direction, &fpsTimer, &fps, &distance, &map_speed,	&current_jump_speed, &current_mario_y_pos, &loss);
	Menu(text, screen, charset, scrtex, renderer, &event, &quit, &time);	
	InitMario(13, mario);		
	while (!quit) {
		//kontrolki dystansu, pozycji itp
		if (changecontrols(&delta, &time, &distance, &map_speed, &current_jump_speed, &current_mario_y_pos, &fpsTimer, &loss, &active_jump))
		{
			Menu(text, screen, charset, scrtex, renderer, &event, &quit, &time);
		}
		//wyswietlanie postaci i mapy
		DrawSurface(screen, eti, 1500 + distance * 10, SCREEN_HEIGHT / 2);
		DrawMario(screen, mario, SCREEN_WIDTH / 2, SCREEN_HEIGHT - SCREEN_HEIGHT / 6 - current_mario_y_pos, &time, &map_speed, &wait, direction);
		//kontorla fpsów
		FpsControls(&fps, &frames, &fpsTimer);
		//wyœwietlanie ¿ycia, czasu
		InfoText(text, time, fps, distance, screen, charset);
		//Renderowanie
		Render(screen, scrtex, renderer);
		//obsluga klawiszy		
		KeyHandler(&event, &map_speed, &direction, &wait, &active_jump, &quit, current_mario_y_pos, distance, CollisionBuffer, &loss);		
		ChangeEndControls(&frames, &current_mario_y_pos, &active_jump, CollisionBuffer, distance, &direction, &current_jump_speed);
	};
	FreeSurface(charset, screen, scrtex, renderer, window);	
	return 0;
};
