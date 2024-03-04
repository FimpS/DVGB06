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

#define MY_SCREEN_HEIGHT 1280
#define MY_SCREEN_WIDTH 960

int main() {

	srand(time(NULL));

	struct map map = map_init();
	struct cam cam = cam_init();
	double reduce;
	gfx_init();

	//char maps[1024][32];
	struct player player;
	//dynList *maps = arrayList_create();
	//dynList *mObject_list = arrayList_create();
	printf("*************************\n");
	printf("RUNNING GAME IN %dx%d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
	printf("*************************\n");

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

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if(renderer == NULL)
	{
		SDL_DestroyWindow(window);
		printf("Error: %s", SDL_GetError());
		return 1;
	}

	SDL_Event e;
	int quit = 0; 

	//INIT
	initPlayer(&player, 20, 20);
	reduce = player.health / player.maxhealth * 8 * TILE_LENGTH;
	//map_load_scene(&map, "res/testmap.txt", map.mObject_list, &player);
	map_load_scene(&map, map.s_map.content[map.s_map.index], map.mObject_list, &player);
	//INIT
	SDL_Surface *sur[32];
	*sur = IMG_Load("assets/Untitled.png");
	SDL_Texture *tex[32];
	*tex = SDL_CreateTextureFromSurface(renderer, sur[0]);
	int start = SDL_GetTicks();
	int framecount = 0;
	while(!quit)
	{
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
					quit = 1;
					break;
			}
		}
		const Uint8* cdr = SDL_GetKeyboardState(NULL);
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		

		//update
		//
		//	map_update(map);
		//  Actual visualization
		//		
		run_event(map.event_list, &map, &player);
		logic_manager(&map, map.mObject_list);
		//spawnEnemy(mObject_list);
		delete_mObject(map.mObject_list);
		map_update(&map, &player, map.event_list);
		updatePlayer(&player, &map, &cam, map.mObject_list, map.event_list, map.pObject_list);
		update_pObjects(map.pObject_list, &player, &map);
		cam_update(&cam, &map, &player);
		update_all_mObjects(map.mObject_list, &player, &map, &cam, map.event_list);
		
		if(cdr[SDL_SCANCODE_V])
			return 1;
		//update





		//draw
		map_draw(&map, &cam, renderer, *tex);
		draw_all_mObjects(renderer, map.mObject_list, &cam, *tex);
		draw_pObjects(renderer, map.pObject_list, &cam, *tex);
		drawPlayer(renderer, &player, &cam, *tex);

		render_hpbar(renderer, &player, &cam, &reduce);
		//draw
		update_tick();
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/70);
#if 0
		framecount++;
		int end = SDL_GetTicks();
		if(end - start >= 1000)
		{
			printf("FPS: %d\n", framecount);
			framecount = 0;
			start = SDL_GetTicks();
		}
#endif

	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;

}
