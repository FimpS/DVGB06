#include "objects.h"
#include "runes.h"
#include "SDL2/SDL.h"
#include "font.h"


int get_rand_rune_type()
{
	return rand() % RUNE_AMOUNT;
}

int get_stage(struct map* map)
{
	return map->s_map.content[map->s_map.index][6] - '1';
}

struct rune_info get_rand_rune_info(struct map* map)
{
	//works
	core_type ct = get_rand_rune_type();
	core_stage cs = get_stage(map);
	const char *title = "rune";
	struct rune_info res = init_rune_info(ct, cs, title);
	return res;
}

//UNHOLY
void unholy_anchor_initial(struct player *player, struct map *map, struct rune* rune)
{
	player->kills = 0;
}

void unholy_anchor_ability(struct player *player, struct map *map, struct rune* rune)
{
	int rng = rand() % 2;
	if(rune->attribute > 0)
	{   
		if(rand() % 2 == 1 || 1)
		{
			printf("att %d\n", rune->attribute);
			spawn_pObject(map->pObject_list, player->x, player->y, PO_WRAITH, EAST, 50.0, 0.0, player);
		}
		rune->attribute --;
	}
}

void unholy_complete_initial(struct player *player, struct map* map, struct rune* rune)
{
	spawn_pObject(map->pObject_list, player->x, player->y, PO_BIG_WRAITH, EAST, 50.0, 0.0, player);
}

void unholy_support_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->maxhealth += 50;
	player->health = player->maxhealth;
	//printf("%f\n", player->health);
}

//HOLY
void holy_support_initial(struct player *player, struct map* map, struct rune* rune)
{

}

void holy_mending_initial(struct player *player, struct map* map, struct rune* rune)
{

}

//BLOOD
void blood_anchor_initial(struct player *player, struct map *map, struct rune* rune)
{
	player->maxhealth -= player->maxhealth * 0.25;
	player->sword_damage += player->sword_damage * 0.5;
}

void blood_support_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->sword_damage += player->sword_damage * 0.25;
}

void blood_mending_ability(struct player* player, struct map* map, struct rune* rune)
{

}

void blood_mending_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->maxhealth -= player->maxhealth * 0.25;
}

void blood_complete_initial(struct player* player, struct map* map, struct rune* rune)
{
	//delete prev ability
	struct rune* blood_mending = (struct rune*)dynList_get(player->rune_list, 2);
	
	if(blood_mending != NULL)
		blood_mending->ability = NULL;
}

void blood_complete_ability(struct player* player, struct map* map, struct rune* rune)
{
	//BRIMSTONE
	//other now but works
	
}
//Gravity

void gravity_anchor_ability(struct player* player, struct map *map, struct rune* rune)
{

}

void gravity_support_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->pObject_knockkoef += 0.5;
}


//Frost

void frost_anchor_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->sword_effect_type = STATUS_FROSTBITE;
	player->attack_speed -= player->attack_speed * 0.20;
}

void frost_support_initial(struct player* player, struct map *map, struct rune* rune)
{
	player->attack_speed -= player->attack_speed * 0.25;
}

void frost_mending_initial(struct player* player, struct map* map, struct rune* rune)
{
	//frost strom
	
}

void frost_mending_ability(struct player* player, struct map* map, struct rune* rune)
{

}

//rot

void rot_anchor_initial(struct player* player, struct map *map, struct rune* rune)
{
	player->sword_effect_type = STATUS_ROT;
	player->base_speed += 0.25;
	player->speed = player->base_speed / 5;
}

void rot_support_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->base_speed += 0.5;
	player->speed = player->base_speed / 5;
}

void rot_mending_ability(struct player* player, struct map* map, struct rune* rune)
{
	
}

//Generic
void print(struct player* player, struct map* map, struct rune* rune)
{
	printf("Rune ability *POW*\n");
}

void rune_abilities(struct player *player, struct map *map, struct rune* rune)
{
	if(dynList_is_empty(player->rune_list))
		return;

	for(int i = 0; i < player->rune_list->size; i++)
	{
		struct rune *curr_rune = (struct rune*)dynList_get(player->rune_list, i);
		if(curr_rune->ability != NULL)
			curr_rune->ability(player, map, curr_rune);
	}
}

struct rune init_rune(struct rune_info i, struct map* map)
{
	struct rune new = {0};
	new.info = init_rune_info(i.rune_type, i.rune_stage, i.title);
	switch(i.rune_type)
	{
		case RN_UNHOLY:
			switch(i.rune_stage)
			{
				case anchor:
					printf("#WISH GRANTED#\nunholy anchor rune acquired\n");
					add_message(map->msg_list, "UNHOLY ANCHOR RUNE ACQUIRED", 10.0, 2.0, 240, 1);
					new.attribute = 0;
					new.ability = unholy_anchor_ability;
					new.initial = unholy_anchor_initial;
					break;
				case support:
					printf("#WISH GRANTED#\nunholy suppot rune acquired\n");
					new.attribute = 0;
					new.ability = NULL;
					new.initial = unholy_support_initial;
					break;
				case mending:
					printf("#WISH GRANTED#\nunholy mending rune acquired\n");
					new.attribute = 0;
					new.ability = NULL;
					new.initial = NULL;
					break;
				case complete:
					new.attribute = 0;
					new.initial = unholy_complete_initial;
					new.ability = NULL;
					break;
			}
			break;
		case RN_HOLY:
			switch(i.rune_stage)
			{
				case anchor:
					printf("#WISH GRANTED#\nholy anchor rune acquired\n");
					new.attribute = true;
					new.ability = NULL;
					new.initial = NULL;
					break;
				case support:
					printf("#WISH GRANTED#\nholy anchor rune acquired\n");
					new.attribute = true;
					new.ability = NULL;
					new.initial = NULL;
					break;
				case mending:
					printf("#WISH GRANTED#\nholy anchor rune acquired\n");
					new.attribute = true;
					new.ability = NULL;
					new.initial = NULL;
					break;
				case complete:
					new.attribute = 3;
					new.ability = NULL;
					new.initial = NULL;
					break;
			}
			break;
		case RN_BLOOD:
			switch(i.rune_stage)
			{
				case anchor:
					printf("#WISH GRANTED#\nblood anchor rune acquired\n");
					new.ability = NULL;
					new.initial = blood_mending_initial;
					new.ability = blood_mending_ability;
					//new.initial = blood_anchor_initial;
					break;
				case support:
					printf("#WISH GRANTED#\nblood support rune acquired\n");
					new.ability = NULL;
					new.initial = blood_support_initial;
					break;
				case mending:
					printf("#WISH GRANTED#\nblood mending rune acquired\n");
					new.ability = blood_mending_ability;
					new.initial = blood_mending_initial;
					break;
				case complete:
					printf("#WISH GRANTED#\nblood complete rune acquired\n");
					new.attribute = 0;
					new.ability = blood_complete_ability;
					new.initial = blood_complete_initial;
					break;

			}
			break;
		case RN_GRAVITY:
			switch(i.rune_stage)
			{
				case anchor:
					printf("#WISH GRANTED#\nGravity anchor rune acquired\n");
					new.attribute = 0;
					new.ability = gravity_anchor_ability;
					new.initial = NULL;
					break;
				case support:
					printf("#WISH GRANTED#\nGravity suppot rune acquired\n");
					new.attribute = 0;
					new.ability = NULL;
					new.initial = gravity_support_initial;
					break;
				case mending:
					printf("#WISH GRANTED#\nGravity mending rune acquired\n");
					new.attribute = 0;
					new.ability = NULL;
					new.initial = NULL;
					break;
				case complete:
					printf("#WISH GRANTED#\nGravity complete rune acquired\n");
					new.attribute = 0;
					new.initial = NULL;
					new.ability = NULL;
					break;
			}
			break;

		case RN_FROST:
			switch(i.rune_stage)
			{
				case anchor:
					printf("#WISH GRANTED#\nfrost anchor rune acquired\n");
					new.ability = NULL;
					new.initial = frost_anchor_initial;
					break;
				case support:
					printf("#WISH GRANTED#\nfrost support rune acquired\n");
					new.ability = NULL;
					new.initial = frost_support_initial;
					break;
				case mending:
					printf("#WISH GRANTED#\nfrost mending rune acquired\n");
					new.ability = frost_mending_ability;
					new.initial = NULL;
					new.attribute = 0;
					break;
					printf("#WISH GRANTED#\nfrost complete rune acquired\n");
					new.ability = NULL;
					new.initial = NULL;
					new.attribute = 0;
					break;
			}
			break;
		case RN_ROT:
			switch(i.rune_stage)
			{
				case anchor:
					printf("#WISH GRANTED#\nrot anchor rune acquired\n");
					new.ability = NULL;
					new.initial = rot_anchor_initial;
					break;
				case support:
					printf("#WISH GRANTED#\nrot support rune acquired\n");
					new.ability = NULL;
					new.initial = rot_support_initial;
					break;
				case mending:
					printf("#WISH GRANTED#\nrot mending rune acquired\n");
					new.attribute = 0;
					new.ability = rot_mending_ability;
					new.initial = NULL;
					break;

			}
			break;
	}

	return new;
}

struct rune_info init_rune_info(core_type type, core_stage stage, const char *title)
{
	struct rune_info new = {0};
	new.rune_type = type;
	new.rune_stage = stage;
	strcpy(new.title, title);
	return new;
}

void add_rune(struct player *player, struct rune_info rinfo, struct map *map)
{
	struct rune* new = (struct rune*)malloc(sizeof(struct rune));
	*new = init_rune(rinfo, map);
	dynList_add(player->rune_list, (void*)new);
	if(new->initial != NULL)
		new->initial(player, map, new);
	int count = 0;
	if(player->rune_list->size != 3)
		return;
	printf("got here\n");
	struct rune *r1 = (struct rune*)dynList_get(player->rune_list, 0);
	struct rune *r2 = (struct rune*)dynList_get(player->rune_list, 1);
	struct rune *r3 = (struct rune*)dynList_get(player->rune_list, 2);
	//works
	if(r1->info.rune_type == r2->info.rune_type && r1->info.rune_type == r3->info.rune_type && player->rune_list->size <= 3)
	{
		struct rune_info new1 = init_rune_info(r1->info.rune_type, complete, "comp");
		add_rune(player, new1, map);
		printf("Rune mended\n");
	}

}
