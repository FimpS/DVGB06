#ifndef RUNES_HEADER
#define RUNES_HEADER
#define RUNE_AMOUNT 6


#include "map.h"
#include "objects.h"

struct player;
struct map;



struct rune_info
{
    core_type rune_type;
    core_stage rune_stage;
    char title[128];
};

struct rune
{   
    void (*ability)(struct player*, struct map*, struct rune*);
    void (*initial)(struct player*, struct map*, struct rune*);
	int attribute;
	struct rune_info info;
};



struct rune init_rune();
struct rune_info init_rune_info();
struct rune_info get_rand_rune_info();
int get_rand_rune_type();
int get_stage();
void add_rune();
void rune_abilities();

#endif
