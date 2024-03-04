#include "global.h"
#include "state.h"
#include "map.h"



void map_manager(struct map *map, dynList* eList)
{
	const Uint8* key_code = SDL_GetKeyboardState(NULL);

	if(key_code[SDL_SCANCODE_C] && 1 == 2)
	{
		sleep(1);
		dynList_clear(eList);
		map_load_scene(map, "res/ch1_maps/ch1_2.txt", eList);
	}

}

void logic_manager(struct map *map, dynList* eList)
{
	map_manager(map, eList);
}

void m_update(struct map *map)
{
	switch(map->state)
	{
		case st_map_default:
			if(map->aggresive_mObj_count == 0)
			{
		//		map->state = st_map_clear;
			}
			break;
		case st_map_clear:
			
			break;
	}
}
