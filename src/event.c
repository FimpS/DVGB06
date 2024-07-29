#include <stdlib.h>
#include "event.h"
#include "dynList.h"
#include "map.h"
#include "global.h"
#include "objects.h"
#include "info.h"
#include "font.h"
//go change arraylist to free the array

void end_run_event(struct player* player, struct event* event, struct map* map)
{
	//gen_seed_map(map);
	//map_leave_run(map);
	clear_seed_map(map);

	reset_player_run(player, map);
	strcpy(map->s_map.content[map->s_map.index], "res/ch0_maps/ch0_hubmap.txt");
	//printf("Lecuck%d\n", player->rune_list->size);
	add_event(map->event_list, type_event_teleport, player, map, 0);
	event->complete = true;
	//for(int i = 0; i < SEED_CHAPTER_AMOUNT * SEED_CHAPTER_SIZE + SEED_CHAPTER_AMOUNT + SPECIAL_MAPS; i++)
	//	printf("%s\n", map->s_map.content[i]);
	//generate a seed
	//go to first map
}

void start_run_event(struct player* player, struct event* event, struct map* map)
{
	gen_seed_map(map);
	map->s_map.index = 0;
	event->complete = true;
	add_event(map->event_list, type_event_teleport, player, map, 0);
	for(int i = 0; i < SEED_CHAPTER_AMOUNT * SEED_CHAPTER_SIZE + SEED_CHAPTER_AMOUNT + SPECIAL_MAPS; i++)
		printf("%s\n", map->s_map.content[i]);
	//generate a seed
	//go to first map
}

void teleport_event(struct player *player, struct event* event, struct map* map)
{	
	map->state = ST_MAP_TRANSITION;
	map->sm.tone = 0;
	//map_load_scene(map, map->s_map.content[++map->s_map.index], map->mObject_list, player);
	event->complete = true;
}

void inmap_teleport_event(struct player* player, struct event* event, struct map* map)
{
	int coords[2];
	map_get_coord(map, 'D', coords);
	player->x = coords[0];
	player->y = coords[1];
	event->complete = true;
}
void golem_start(struct player* player, struct event* event, struct map* map)
{
	map->state = ST_MAP_CINEMATIC;
	size_t index = 0;
	
	struct mObject *golem = id_get_mObj(map, 'o');
	for(int i = 0; i < map->mObject_list->size; i++)
	{
		struct mObject *magus = (struct mObject*)dynList_get(map->mObject_list, i);
		if(magus->id == '6')
		{
			map->cam.cin_info.s_coord.x = -map->cam.x + golem->x + golem->width/TILE_LENGTH/2,
			map->cam.cin_info.s_coord.y = -map->cam.y + golem->y+ golem->height/TILE_LENGTH/2;
			double dx = magus->x - golem->x, dy = magus->y - golem->y;
			magus->theta = atan2(-dy, -dx);
		}
	}
	//map->cam.x = golem->x + golem->width/TILE_LENGTH/2;
	//map->cam.y = golem->y + golem->width/TILE_LENGTH/2;
	printf("in golem_start %c %lf\n", golem->id, map->cam.cin_info.s_coord.x);
}

void golem_cutscene2(struct player* player, struct event* event, struct map* map)
{
	struct mObject* golem = id_get_mObj(map, 'o');
	if(event->clock >= event->burst_time)
	{
		map->state = ST_MAP_RUN_TICK;
		set_mObject_state(golem, ST_GOLEM_AWARE, state_golem_aware, 0, 60);
		event->complete = true;
		return;
	}
	event->clock ++;
}

void golem_cutscene(struct player* player, struct event* event, struct map* map)
{
	struct mObject *golem = id_get_mObj(map, 'o');
	int time = 64;
	if(event->clock >= time)
	{
		add_message(map->msg_list, "DEFEAT THE ANIMATED GOLEM", MIDPOINTX(golem), MIDPOINTY(golem) - 2, 460, 1);
		for(int i = 0; i < map->mObject_list->size; i++)
		{
			struct mObject *magus = (struct mObject*)dynList_get(map->mObject_list, i);
			if(magus->id == '6')
			{
				double dx = magus->x - golem->x, dy = magus->y - golem->y;
				magus->theta = atan2(-dy, -dx);
				magus->st.type = ST_MAGUS_READY;
				identify_mObject_sprite_location(magus);
			}
		}


		map->cam.x = golem->x + golem->width/TILE_LENGTH/2;
		map->cam.y = golem->y + golem->height/TILE_LENGTH/2;
		set_mObject_state(golem, ST_GOLEM_BUILD, NULL, 0, 60*8 - time);
		event->event_tick = golem_cutscene2;
		return;
	}

	map->cam.x += map->cam.cin_info.s_coord.x/time;
	map->cam.y += map->cam.cin_info.s_coord.y/time;
	event->clock ++;
}

void chieftain_start(struct player* player, struct event* event, struct map* map)
{
	map->state = ST_MAP_CINEMATIC;
	struct mObject *golem = id_get_mObj(map, 'c');
	map->cam.cin_info.s_coord.x = -map->cam.x + golem->x + golem->width/TILE_LENGTH/2;
	map->cam.cin_info.s_coord.y = -map->cam.y + golem->y+ golem->height/TILE_LENGTH/2;
	printf("in witch start %c\n", golem->id);
}

void chieftain_cutscene2(struct player* player, struct event* event, struct map* map)
{
	if(event->clock >= event->burst_time)
	{
		struct mObject* chieftain = id_get_mObj(map, 'c');
		map->state = ST_MAP_RUN_TICK;
		set_mObject_state(chieftain, ST_CHIEFTAIN_SUMMON, state_chieftain_summon, 0, 80);
		event->complete = true;
		return;
	}
	event->clock ++;
}

void chieftain_cutscene(struct player* player, struct event* event, struct map* map)
{
	int time = 168;
	if(event->clock >= time)
	{
		struct mObject* chieftain = id_get_mObj(map, 'c');
		event->event_tick = chieftain_cutscene2;
		SET_CAM_MID_CORDS(map->cam, chieftain);
		return;
	}
	map->cam.x += map->cam.cin_info.s_coord.x/time;
	map->cam.y += map->cam.cin_info.s_coord.y/time;
	event->clock ++;
}

void queen_start(struct player* player, struct event* event, struct map* map)
{
	map->state = ST_MAP_CINEMATIC;
	struct mObject *queen = id_get_mObj(map, 'q');
	map->cam.cin_info.s_coord.x = -map->cam.x + MIDPOINTX(queen);
	map->cam.cin_info.s_coord.y = -map->cam.y + MIDPOINTY(queen);
}

void queen_cutscene2(struct player* player, struct event* event, struct map* map)
{
	if(event->clock ++ >= event->burst_time)
	{
		struct mObject* queen = id_get_mObj(map, 'q');
		map->state = ST_MAP_RUN_TICK;
		set_mObject_state(queen, ST_LOCAL_QUEEN_AWARE, state_local_queen_aware, 0, 124);
		event->complete = true;
		return;
	}
}

void queen_cutscene(struct player* player, struct event* event, struct map* map)
{
	const int time = 60;
	if(event->clock ++ >= time)
	{
		struct mObject* queen = id_get_mObj(map, 'q');
		event->event_tick = queen_cutscene2;
		SET_CAM_MID_CORDS(map->cam, queen);
	}
	map->cam.x += map->cam.cin_info.s_coord.x/time;
	map->cam.y += map->cam.cin_info.s_coord.y/time;
	
}

void vortex_start(struct player* player, struct event* event, struct map* map)
{
	map->state = ST_MAP_CINEMATIC;
	struct mObject *vort = id_get_mObj(map, 'v');
	map->cam.cin_info.s_coord.x = -map->cam.x + MIDPOINTX(vort);
	map->cam.cin_info.s_coord.y = -map->cam.y + MIDPOINTY(vort);
}

void vortex_cutscene2(struct player* player, struct event* event, struct map* map)
{
	if(event->clock ++ >= event->burst_time)
	{
		struct mObject* vort = id_get_mObj(map, 'v');
		map->state = ST_MAP_RUN_TICK;
		set_mObject_state(vort, ST_ROCK_VORTEX_AWARE, state_rock_vortex_aware, 0, 124);
		event->complete = true;
		return;
	}
}

void vortex_cutscene(struct player* player, struct event* event, struct map* map)
{
	const int time = 60;
	if(event->clock ++ >= time)
	{
		struct mObject* vort = id_get_mObj(map, 'v');
		event->event_tick = vortex_cutscene2;
		SET_CAM_MID_CORDS(map->cam, vort);
	}
	map->cam.x += map->cam.cin_info.s_coord.x/time;
	map->cam.y += map->cam.cin_info.s_coord.y/time;
	
}

void lock(struct player *player, struct event* event, struct map* map)
{
	if(event->clock < event->burst_time)
	{
		if(rand() % 2)
			player->width++;
		else
			player->height++;
		event->clock++;
		return;
	}
	event->complete = true;;
}


void identify_start_tick(struct event* event)
{
	switch(event->e_type)
	{
		case TYPE_EVENT_GOLEM:
			event->start_event = golem_start;
			event->event_tick = golem_cutscene;
			break;
		case TYPE_EVENT_CHIEFTAIN:
			event->start_event = chieftain_start;
			event->event_tick = chieftain_cutscene;
			break;
		case TYPE_EVENT_QUEEN:
			event->start_event = queen_start;
			event->event_tick = queen_cutscene;
			break;
		case TYPE_EVENT_VORTEX:
			event->start_event = vortex_start;
			event->event_tick = vortex_cutscene;
			break;
		case TYPE_EVENT_START_RUN:
			event->start_event = NULL;
			event->event_tick = start_run_event;
			break;
		case TYPE_EVENT_END_RUN:
			event->start_event = NULL;
			event->event_tick = end_run_event;
			break;
		case type_event_teleport:
			event->start_event = NULL;
			event->event_tick = teleport_event;
			break;
		case type_event_lock:
			event->start_event = NULL;
			event->event_tick = lock;
			break;
		case type_event_inmaptp:
			event->start_event = NULL;
			event->event_tick = inmap_teleport_event;
			break;
		case type_event_rune_acquired:
			event->start_event = NULL;
			event->event_tick = NULL;
			break;
		case type_event_placeholder:
			return;
			break;
	}
}

void init_event(struct event* new, event_type e_type, int burst)
{
	//add acp to skip gigaswitch
	new->e_type = e_type;
	new->complete = false;
	new->burst_time = burst;
	new->clock = 0;
	identify_start_tick(new);
}

void add_event(dynList* ev_list, event_type type, struct player *player, struct map* map, int burst)
{
	struct event* new_event = (struct event*)malloc(sizeof(struct event));
	init_event(new_event, type, burst);
	dynList_add(ev_list, (void*)new_event);
	if(new_event->start_event == NULL)
		return;
	new_event->start_event(player, new_event, map);
}



void run_event(dynList *ev_list, struct map* map, struct player *player)
{
	//THIS THING WORKS AS A QUENE AND SHOULD BE TREATED AS ONE foremanInsane
	//DONT TRY TO RUN THIS AS ANIMATION HANDLER
	if(dynList_is_empty(ev_list))
		return;
	//this will segfault soon
	for(int i = 0; i < map->event_list->size; i++)
	{
		struct event* curr_event = ((struct event*)dynList_get(ev_list, i));
		if(!curr_event->complete)
		{
			curr_event->event_tick(player, curr_event, map);
		}
		else
		{
			dynList_del_index(ev_list, i);	
			i--;
		}
	}



}

#if 0
make a copy of identify and run for animation just with one argument which would be the mObject i think this is the only way to do it without over-argumenting everything
maybe make a new file or do it here but this is not working

this probably means that we need to do as the guy told us and make a seperate thingy for items and mObjects and projectiles, further analysis required
#endif
