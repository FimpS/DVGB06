#include <stdlib.h>
#include "event.h"
#include "dynList.h"
#include "map.h"
#include "global.h"
//go change arraylist to free the array



void teleport_event(struct player *player, struct event* event, struct map* map)
{	
	map_load_scene(map, map->s_map.content[++map->s_map.index], map->mObject_list, player);
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

}

void golem_cutscene(struct player* player, struct event* event, struct map* map)
{
	map->state = ST_MAP_CINEMATIC;
	map->cam.x += 0.1;

	if(event->clock > event->burst_time)
	{
		map->state = ST_MAP_RUN_TICK;
		event->complete = true;
	}
	event->clock ++;
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
		case type_event_golem:
			event->start_event = NULL;
			event->event_tick = golem_cutscene;
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
	init_event(new_event, type, 120);
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
