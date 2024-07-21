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
	//printf("map:%ld, mObject:%ld, pObject:%ld, player:%ld\n", sizeof(struct map), sizeof(struct mObject), sizeof(struct pObject), sizeof(struct player));
	//printf("%d\n", init_UI_el(1.0, 2.0).sprite.w);
#if 1
	srand(time(NULL));
	int mode = SDL_WINDOW_BORDERLESS;
	int nmode = 0;
	if(argc == 2 && !strcmp(argv[1], "full"))
	{
		mode = SDL_WINDOW_FULLSCREEN;
	}
	if(argc == 3)
		nmode = SDL_RENDERER_PRESENTVSYNC;

	struct map map = map_init();
	//dynList* ui_el_list = dynList_create();
	init_UI(map.UI_el_list);
	add_message(map.msg_list_UI, "TEST123", 0.5, 3.0, 0, 1);
	double reduce;

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

	SDL_Window *window = SDL_CreateWindow("main", 500, 120, SCREEN_WIDTH, SCREEN_HEIGHT, mode);
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
	const char* tmp = "res/ch1_maps/ch1_1.txt";
	map_load_scene(&map, tmp/*map.s_map.content[map.s_map.index]*/, map.mObject_list, &player);
	//INIT
	SDL_Texture *tex[32];
	gfx_init(tex, renderer);
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
		
		if(cdr[SDL_SCANCODE_V] || map.quit)
			goto out;
		run_tick(&map, &player);
#if 1
		SDL_MouseMotionEvent m;
		//draw
		map_draw(&map, &map.cam, renderer, *tex, &player);
		drawPlayer(renderer, &player, &map.cam, tex[PLAYER_SPRITESHEET]);
		draw_all_mObjects(renderer, map.mObject_list, &map.cam, tex[MOBJECT_SPRITESHEET], &player);
		draw_pObjects(renderer, map.pObject_list, &map.cam, tex[POBJECT_SPRITESHEET]);

		render_hpbar(renderer, &player, &map.cam, &reduce);
		render_UI_elements(map.UI_el_list, &player, renderer, tex[UI_SPRITESHEET]);
		render_messages(map.msg_list, &map.cam, renderer, tex[FONT_SPRITESHEET]);
		render_UI_texts(map.msg_list_UI, renderer, tex[FONT_SPRITESHEET]);
		//render_message(((struct message*)dynList_get(map.msg_list, 0)), renderer, tex[FONT_SPRITESHEET]);
		//printf("%d\n", ((struct message*)dynList_get(map.msg_list, 0))->size);
		//draw
		update_tick();
		SDL_RenderPresent(renderer);
		//SDL_Delay(1000/70);
#endif
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
out:
	SDL_DestroyWindow(window);
	SDL_Quit();

#endif
	return 0;
}