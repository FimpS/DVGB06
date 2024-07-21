#ifndef EVENT_HEADER_H
#define EVENT_HEADER_H

//handles localstates and events

#include "objects.h"
#include "dynList.h"

struct event
{
	event_type e_type;
	void (*event_tick)(struct player *player, struct event *event, struct map* map);
	void (*start_event)(struct player *player, struct event *event, struct map* map);
	int burst_time;
	int clock;
	bool complete;
};



void add_event();
void run_event();

#endif
