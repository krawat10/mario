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

//#ifdef __cplusplus
//extern "C"
//#endif
int lifes = 3;

void initcontrolers(bool* active_jump, int* quit, int* frames, int* wait, char* direction, double* fpsTimer, double* fps, double* distance, double* map_speed,
	double* current_jump_speed, double* current_mario_y_pos, bool* loss)
{
	*loss = false;  //czy postac spad³a/koniec czasu
	*active_jump = false; //czy postac skacze
	*frames = 0; 
	*fpsTimer = 0;
	*fps = 0;
	*quit = 0;  //czy nacisnieto ESC
	*distance = 0; // po³o¿enie postaci na mapie (pozycja x)
	*current_jump_speed = 0; //aktualna predkosc skoku
	*map_speed = 0;  //predkosc poruszania sie
	*current_mario_y_pos = 0; //aktualne po³ozenie mario w osi y
	*direction = 0;  //kierunek poruszania sie mario (lewo, prawo, skok)
	*wait = 0;  //zmienna potrzebna do animacji
}

//Restartowanie zmiennych (po stracie ¿ycia/ nowej gry)
void RestartMap(Timer *time, double *distance, double* current_jump_speed, double* current_mario_y_pos, bool* loss, bool* active_jump, double *map_speed)
{
	(*time).WorldTime = 0; 
	(*time).animation_time = 0;
	lifes--;
	*distance = 0;
	*current_jump_speed = 0;
	*current_mario_y_pos = 0;
	*map_speed = 0;
	*active_jump = false;
	*loss = false;	
}

//Zmiana kontrolek po ka¿dej pêtli 
bool changecontrols(double *delta, Timer *time, double *distance, double *map_speed,
	double* current_jump_speed, double* current_mario_y_pos, double* fpsTimer, bool* loss, bool* active_jump)
{
	*delta = (*time).ElapsedTime();
	*distance += *map_speed * (*delta);
	*current_mario_y_pos += *current_jump_speed * (*delta);
	*fpsTimer += (*delta);
	if (*current_mario_y_pos < -60) *loss = true;
	else if ((*time).TimeToEnd() < 0) *loss = true;
	if (*loss)
	{
		RestartMap(time, distance, current_jump_speed, current_mario_y_pos, loss, active_jump, map_speed);
		if (lifes < 0)
		{
			return true;
		}
	}
	else if ((-*distance * 10 + SCREEN_WIDTH) > MAP_WIDTH)  //gdy dotrze do koñca mapy
	{
		RestartMap(time, distance, current_jump_speed, current_mario_y_pos, loss, active_jump, map_speed);
		return true;
	}
	return false;  //jesli false nie w³¹czy menu
}

//kontrola fpsów  -  niepotrzebne do dzia³ania gry
void FpsControls(double* fps, int* frames, double *fpsTimer)
{
	if (*fpsTimer > 0.5) {
		*fps = *frames * 2;
		*frames = 0;
		*fpsTimer -= 0.5;
	};
}

//tablica z ¿yciem itp. (podczas gry)
void InfoText(char *text, Timer time, double fps, double distance, SDL_Surface *screen, SDL_Surface *charset)
{
	sprintf(text, "Czas = %.1lf s  %d zycia, n - menu,  ESC - wyjscie, posirion %.3lf ", time.TimeToEnd(), lifes, (-distance * 10 + SCREEN_WIDTH / 2));
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
}

//Po nacisnieciu ESC czysci pamiec
void FreeSurface(SDL_Surface *charset, SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer, SDL_Window *window)
{
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//renderowanie
void Render(SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer)
{
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}

//inicjacja elementów biblioteki SDL
void InitSDL(int *rc, SDL_Window* window, SDL_Renderer *renderer, SDL_Surface *screen, SDL_Texture *scrtex)
{
	*rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, "Szablon do zdania drugiego 2014");
	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);  //stworzenie screenu o wymiarach 640-480 w czarnym kolorze
	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);
}

//edycja kontrolek pod koniec pêtli
void ChangeEndControls(int* frames, double* current_mario_y_pos, bool *active_jump, char* CollisionBuffer, double distance, char* direction, double* current_jump_speed)
{
	(*frames)++;
	*current_jump_speed = jump(current_mario_y_pos, active_jump, CollisionBuffer, distance, direction);

}

//Sprawdza czy nacisniêto klawisz
void KeyHandler(SDL_Event *event, double *speed, char *dir, int *wait, bool *active_jump, int *quit, double y_pos, double distance, char *CollisionBuffer, bool* loss)
{
	if (y_pos >= 0)  //jeœli nie wpadnie w przepaœæ
	{
		while (SDL_PollEvent(event))
		{
			EventHandler(*event, speed, dir, wait, active_jump, quit, y_pos, distance, CollisionBuffer, loss);
		}
	}
}

// obs³uga klawiszy
void EventHandler(SDL_Event event, double *speed, char *dir, int *wait, bool *active_jump, int *quit, double y_pos, double distance, char *CollisionBuffer, bool* loss)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) *quit = 1;
		else if (event.key.keysym.sym == SDLK_LEFT && check_way(distance, CollisionBuffer, y_pos, LEFT)) { *speed = 100.0; *wait = 20; *dir = LEFT; }
		else if (event.key.keysym.sym == SDLK_RIGHT&& check_way(distance, CollisionBuffer, y_pos, RIGHT)) { *speed = -100.0; *wait = 20; *dir = RIGHT; }
		else if (event.key.keysym.sym == SDLK_UP) *active_jump = true;
		//Jeœli nacisnie n to nowa gra (symuluj przegran¹ gre)
		if ((event).key.keysym.sym == SDLK_n)
				{	
					*loss = true;
					lifes = -1;					
				}
		break;
	case SDL_KEYUP:
		*speed = 0.0;
		break;
	case SDL_QUIT:
		*quit = 1;
		break;
	};
}


//Menu wraz z jego obs³ug¹
void Menu(char *text, SDL_Surface *screen, SDL_Surface *charset, SDL_Texture *scrtex,
	SDL_Renderer *renderer, SDL_Event *event, int *quit, Timer *time)
{
	sprintf(text, "n - nowa gra                ESC - wyjscie");
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, SCREEN_HEIGHT / 2, text, charset);
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
	while (!*quit)
	{
		while (SDL_PollEvent(event))
		{
			switch ((*event).type)
			{
			case SDL_KEYDOWN:
				if ((*event).key.keysym.sym == SDLK_ESCAPE) *quit = 1;
				if ((*event).key.keysym.sym == SDLK_n)  //nowa gra
				{
					lifes = 3;
					*time = Timer::Timer(); 
					return;
				}
				break;
			};
		}
	}
	*time = Timer::Timer();
}
