#include "global.h"
#include "state.h"
#include "map.h"
#include "event.h"
#include "runes.h"
#include "gfx.h"
#include "font.h"
#include "dynList.h"

void run_tick(struct map *map, struct player *player, SDL_Window* window)
{
	switch(map->state)
	{
		case ST_MAP_RUN_TICK:
			map_update(map, player, map->event_list);
			cam_update(&map->cam, map, player);
			updatePlayer(player, map, &map->cam, map->mObject_list, map->event_list, map->pObject_list);
			update_pObjects(map->pObject_list, player, map);
			update_all_mObjects(map->mObject_list, player, map, &map->cam, map->event_list);
			run_event(map->event_list, map, player);
			menu_put(map);
			break;
		case ST_MAP_CINEMATIC:
			run_event(map->event_list, map, player);
			control_cam_update(&map->cam, map, player);
			break;
		case ST_MAP_PAUSE:
			run_menu(map); //very weird works without args, since run_tick is map first? prob A C exclusive
			option_requests(map, window);
			break;
		case ST_MAP_TRANSITION:
		case ST_MAP_FADE_IN:
			break;
		case ST_MAP_DEFAULT:
			
			break;
	}
}

void render_tick(struct map* map, struct player* player, SDL_Renderer *renderer, SDL_Texture **tex)
{

	switch(map->state)
	{
		case ST_MAP_RUN_TICK:
		case ST_MAP_CINEMATIC:
		case ST_MAP_PAUSE:
			map_draw(map, &map->cam, renderer, *tex, player);
			draw_all_mObjects(renderer, map->mObject_list, &map->cam, tex[MOBJECT_SPRITESHEET], player);
			drawPlayer(renderer, player, &map->cam, tex[PLAYER_SPRITESHEET]);
			draw_pObjects(renderer, map->pObject_list, &map->cam, tex[POBJECT_SPRITESHEET]);
			render_UI_elements(map->UI_el_list, player, map, renderer, tex[UI_SPRITESHEET]);
			render_messages(map->msg_list, &map->cam, renderer, tex[FONT_SPRITESHEET]);
			render_UI_texts(map->msg_list_UI, renderer, tex[FONT_SPRITESHEET]);
			//fade_in(renderer, &map->sm, map);
			//fade_out(renderer, &map->sm, map);
			break;
		case ST_MAP_TRANSITION:
			map_draw(map, &map->cam, renderer, *tex, player);
			draw_all_mObjects(renderer, map->mObject_list, &map->cam, tex[MOBJECT_SPRITESHEET], player);
			drawPlayer(renderer, player, &map->cam, tex[PLAYER_SPRITESHEET]);
			fade_out(renderer, &map->sm, map, player);
			break;
		case ST_MAP_FADE_IN:
			map_draw(map, &map->cam, renderer, *tex, player);
			draw_all_mObjects(renderer, map->mObject_list, &map->cam, tex[MOBJECT_SPRITESHEET], player);
			drawPlayer(renderer, player, &map->cam, tex[PLAYER_SPRITESHEET]);
			fade_in(renderer, &map->sm, map, player);
			break;
	}
}
