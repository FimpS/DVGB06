#include "objects.h"
#include "runes.h"
#include "SDL2/SDL.h"
#include "font.h"
#include "gfx.h"
#include "info.h"

#define FIRST 36
#define SECOND 80
#define THIRD 124
#define ALWAYS 800 - 32 - 12

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
	player->maxhealth += 60;
}

void unholy_anchor_ability(struct player *player, struct map *map, struct rune* rune)
{
	if(rune->attribute > 0)
	{   
			spawn_pObject(map->pObject_list, player->x, player->y, PO_WRAITH, EAST, 20.0, 0.0, player);
		rune->attribute --;
	}
}

void unholy_complete_initial(struct player *player, struct map* map, struct rune* rune)
{
	spawn_pObject(map->pObject_list, player->x, player->y, PO_BIG_WRAITH, EAST, player->sword_damage / 2, 0.0, player);
}

void unholy_mending_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->maxhealth += 40;
	player->health = player->maxhealth;
}


void unholy_support_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->maxhealth += 120;
}

//HOLY
void holy_anchor_initial(struct player *player, struct map* map, struct rune* rune)
{
	player->dash_invuln += 2;
}

void holy_support_initial(struct player *player, struct map* map, struct rune* rune)
{
	player->dash_invuln += 4;
}

void holy_mending_initial(struct player *player, struct map* map, struct rune* rune)
{
	player->dash_invuln += 2;
}

//BLOOD
void blood_anchor_initial(struct player *player, struct map *map, struct rune* rune)
{
	player->maxhealth -= 25;
	player->sword_damage += PLAYER_START_DMG * 0.1;
}

void blood_support_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->maxhealth -= 35;
	player->sword_damage += PLAYER_START_DMG * 0.15;
}

void blood_mending_ability(struct player* player, struct map* map, struct rune* rune)
{

}

void blood_mending_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->maxhealth -= 25;
	player->sword_damage += PLAYER_START_DMG * 0.1;
}

void blood_complete_initial(struct player* player, struct map* map, struct rune* rune)
{

}

void blood_complete_ability(struct player* player, struct map* map, struct rune* rune)
{
	
}
//Gravity

void gravity_anchor_initial(struct player* player, struct map *map, struct rune* rune)
{
	player->pObject_knockkoef += 0.125;
}

void gravity_anchor_ability(struct player* player, struct map *map, struct rune* rune)
{

}

void gravity_support_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->pObject_knockkoef += 0.175;
}

void gravity_mending_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->pObject_knockkoef += 0.125;
}

//Frost

void frost_anchor_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->sword_effect_type = STATUS_FROSTBITE;
	player->attack_speed -= 8;
}

void frost_support_initial(struct player* player, struct map *map, struct rune* rune)
{
	player->attack_speed -= 12;
}

void frost_mending_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->attack_speed -= 8;	
}

void frost_mending_ability(struct player* player, struct map* map, struct rune* rune)
{

}

//rot

void rot_anchor_initial(struct player* player, struct map *map, struct rune* rune)
{
	player->sword_effect_type = STATUS_ROT;
	player->base_speed += 0.025;
	player->speed = player->base_speed / 16;
}

void rot_support_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->base_speed += 0.05;
	player->speed = player->base_speed / 16;
}

void rot_mending_initial(struct player* player, struct map* map, struct rune* rune)
{
	player->base_speed += 0.025;
	player->speed = player->base_speed / 16;
}

void rot_mending_ability(struct player* player, struct map* map, struct rune* rune)
{

}

//Generic

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
	add_message(map->msg_list, "RUNE ACQUIRED", X_MIDDLE_FONT, 29.6, 360, 2);
	switch(i.rune_type)
	{
		case RN_UNHOLY:
			switch(i.rune_stage)
			{
				case anchor:
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, FIRST, UI_UNHOLY));
					add_message(map->msg_list, "UNHOLY ANCHOR", X_MIDDLE_FONT, 32.0, 360, 2);
					new.attribute = 0;
					new.ability = unholy_anchor_ability;
					new.initial = unholy_anchor_initial;
					break;
				case support:
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, SECOND, UI_UNHOLY));
					add_message(map->msg_list, "UNHOLY SUPPORT", X_MIDDLE_FONT, 32.0, 360, 2);
					new.attribute = 0;
					new.ability = NULL;
					new.initial = unholy_support_initial;
					break;
				case mending:
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, THIRD, UI_UNHOLY));
					add_message(map->msg_list, "UNHOLY MENDING", X_MIDDLE_FONT, 32.0, 360, 2);
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
					add_message(map->msg_list, "HOLY ANCHOR", X_MIDDLE_FONT, 32.0, 360, 2);
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, FIRST, UI_HOLY));
					new.attribute = true;
					new.ability = NULL;
					new.initial = holy_anchor_initial;
					break;
				case support:
					add_message(map->msg_list, "HOLY SUPPORT", X_MIDDLE_FONT, 32.0, 360, 2);
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, SECOND, UI_HOLY));
					new.attribute = true;
					new.ability = NULL;
					new.initial = holy_support_initial;
					break;
				case mending:
					add_message(map->msg_list, "HOLY MENDING", X_MIDDLE_FONT, 32.0, 360, 2);
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, THIRD, UI_HOLY));
					new.attribute = true;
					new.ability = NULL;
					new.initial = holy_mending_initial;
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
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, FIRST, UI_BLOOD));
					add_message(map->msg_list, "BLOOD ANCHOR", X_MIDDLE_FONT, 32.0, 360, 2);
					new.ability = NULL;
					new.initial = blood_anchor_initial;
					break;
				case support:
					add_message(map->msg_list, "BLOOD SUPPORT", X_MIDDLE_FONT, 32.0, 360, 2);
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, SECOND, UI_BLOOD));
					new.ability = NULL;
					new.initial = blood_support_initial;
					break;
				case mending:
					add_message(map->msg_list, "BLOOD MENDING", X_MIDDLE_FONT, 32.0, 360, 2);
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, THIRD, UI_BLOOD));
					new.ability = blood_mending_ability;
					new.initial = blood_mending_initial;
					break;
				case complete:
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
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, FIRST, UI_GRAVITY));
					add_message(map->msg_list, "GRAVITY ANCHOR", X_MIDDLE_FONT, 32.0, 360, 2);
					new.attribute = 0;
					new.ability = gravity_anchor_ability;
					new.initial = gravity_anchor_initial;
					break;
				case support:
					add_message(map->msg_list, "GRAVITY SUPPORT", X_MIDDLE_FONT, 32.0, 360, 2);
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, SECOND, UI_GRAVITY));
					new.attribute = 0;
					new.ability = NULL;
					new.initial = gravity_support_initial;
					break;
				case mending:
					add_message(map->msg_list, "GRAVITY MENDING", X_MIDDLE_FONT, 32.0, 360, 2);
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, THIRD, UI_GRAVITY));
					new.attribute = 0;
					new.ability = NULL;
					new.initial = gravity_mending_initial;
					break;
				case complete:
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
					add_message(map->msg_list, "FROST ANCHOR", X_MIDDLE_FONT, 32.0, 360, 2);
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, FIRST, UI_FROST));
					new.ability = NULL;
					new.initial = frost_anchor_initial;
					break;
				case support:
					add_message(map->msg_list, "FROST SUPPORT", X_MIDDLE_FONT, 32.0, 360, 2);
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, SECOND, UI_FROST));
					new.ability = NULL;
					new.initial = frost_support_initial;
					break;
				case mending:
					add_message(map->msg_list, "FROST MENDING", X_MIDDLE_FONT, 32.0, 360, 2);
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, THIRD, UI_FROST));
					new.ability = frost_mending_ability;
					new.initial = frost_mending_initial;
					new.attribute = 0;
					break;
				case complete:
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
					add_message(map->msg_list, "ROT ANCHOR", X_MIDDLE_FONT, 32.0, 360, 2);
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, FIRST, UI_ROT));
					new.ability = NULL;
					new.initial = rot_anchor_initial;
					break;
				case support:
					add_message(map->msg_list, "ROT SUPPORT", X_MIDDLE_FONT, 32.0, 360, 2);
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, SECOND, UI_ROT));
					new.ability = NULL;
					new.initial = rot_support_initial;
					break;
				case mending:
					add_message(map->msg_list, "ROT MENDING", X_MIDDLE_FONT, 32.0, 360, 2);
					dynList_add(map->UI_el_list, (void*)init_UI_el(ALWAYS, THIRD, UI_ROT));
					new.attribute = 0;
					new.ability = rot_mending_ability;
					new.initial = rot_mending_initial;
					break;

			}
			break;
		default:
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
	player->sword_damage += 10;
	struct rune* new = (struct rune*)malloc(sizeof(struct rune));
	*new = init_rune(rinfo, map);
	dynList_add(player->rune_list, (void*)new);
	if(new->initial != NULL)
		new->initial(player, map, new);
	int count = 0;
	if(player->rune_list->size != 3)
		return;
	struct rune *r1 = (struct rune*)dynList_get(player->rune_list, 0);
	struct rune *r2 = (struct rune*)dynList_get(player->rune_list, 1);
	struct rune *r3 = (struct rune*)dynList_get(player->rune_list, 2);
	if(r1->info.rune_type == r2->info.rune_type && r1->info.rune_type == r3->info.rune_type && player->rune_list->size <= 3)
	{
		struct rune_info new1 = init_rune_info(r1->info.rune_type, complete, "comp");
		add_rune(player, new1, map);
	}
}
