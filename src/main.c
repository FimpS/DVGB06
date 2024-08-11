#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "objects.h"
#include "state.h"
#include "global.h"
#include "map.h"
#include "event.h"
#include "gfx.h"
#include "info.h"
#include "font.h"

#define MY_SCREEN_HEIGHT 1280
#define MY_SCREEN_WIDTH 960

int main(int argc, char **argv) {
	srand(time(NULL));
	int mode = SDL_WINDOW_BORDERLESS;

	struct map map = map_init();
	init_UI(map.UI_el_list);

	struct player player;
#if 0
	printf("*************************\n");
	printf("RUNNING GAME IN %dx%d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
	printf("*************************\n");
#endif

	if(SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Error %s", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("main", 500, 120, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
	if(window == NULL)
	{
		printf("Error: %s", SDL_GetError());
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if(renderer == NULL)
	{
		SDL_DestroyWindow(window);
		printf("Error: %s", SDL_GetError());
		return 1;
	}

	SDL_Event e;
	int quit = 0; 

	//INIT
	initPlayer(&player, &map);
	const char* start_map = "res/ch0_maps/ch0_hubmap.txt";
	map_load_scene(&map, start_map, map.mObject_list, &player);
	//INIT
	SDL_Texture *tex[32];
	gfx_init(tex, renderer);
	SDL_ShowCursor(false);
	int start = SDL_GetTicks();
	int framecount = 0;
	bool boola = false;
	while(!quit)
	{
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if(e.button.button == SDL_BUTTON_LEFT) map.mouse_clicked = true;
					break;
			}
		}
		const Uint8* cdr = SDL_GetKeyboardState(NULL);
		int start_frame = SDL_GetTicks();
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		if(map.quit)
			goto out;
#if 0
		if(cdr[SDL_SCANCODE_C])
		{
			map_load_scene(&map, map.map_name, map.mObject_list, &player);
			sleep(1);
		}
#endif
		run_tick(&map, &player, window);
		SDL_MouseMotionEvent m;
		render_tick(&map, &player, renderer, tex);
		update_tick();
		SDL_RenderPresent(renderer);
		int end_frame = SDL_GetTicks() - start_frame;
		if(1000/60 > end_frame)
		{
			SDL_Delay(1000/60 - end_frame);
		}

	}
out:
	add_recent_run_save(&map.save);
	save_game(&map);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
