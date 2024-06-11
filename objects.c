#include "objects.h"
#include "global.h"
#include "map.h"
#include "event.h"
#include "object_state.h"
#include "runes.h"
#include "gfx.h"
#include "info.h"

static int tick = 0;

//this only works if all elements are in the same memory field (C is the language of gods)
bool AABB(struct mObject *s, struct mObject *t)
{
	return s->x < t->x + t->width/TILE_LENGTH &&
		   s->x + s->width/TILE_LENGTH > t->x &&
		   s->y < t->y + t->height/TILE_LENGTH &&
		   s->y + s->height/TILE_LENGTH > t->y;
}


bool AABBpp(struct pObject *pObject, struct player *player)
{
	return pObject->x < player->x + player->width/TILE_LENGTH &&
		   pObject->x + pObject->width/TILE_LENGTH > player->x &&
		   pObject->y < player->y + player->height/TILE_LENGTH &&
		   pObject->y + pObject->width/TILE_LENGTH > player->y;
}

bool AABBpm(struct pObject *pObject, struct mObject *target)
{
	return pObject->x < target->x + target->width/TILE_LENGTH &&
		pObject->x + pObject->width/TILE_LENGTH > target->x &&
		pObject->y < target->y + target->height/TILE_LENGTH &&
		pObject->y + pObject->width/TILE_LENGTH > target->y;

}

struct mObject* id_get_mObj(struct map* map, char id)
{
	struct mObject* res;
	for(int i = 0; i < map->mObject_list->size; i++)
	{
		res = (struct mObject*)dynList_get(map->mObject_list, i);
		if(res->id == id)
			return res;
	}
	return NULL;
}


void set_status_effect(struct mObject *mObject, int timer, int limit, mObject_status_effect type)
{
	mObject->status_effect.timer = timer;
	mObject->status_effect.limit = limit;
	mObject->status_effect.type = type;
}


struct pObject* spawn_pObject(dynList *pObject_list, double x, double y, pObject_type type, card_dir dir, double dmg, double theta, struct player* player)
{
	struct pObject* new = (struct pObject*)malloc(sizeof(struct pObject));
	new->type = type;
	init_pObject(new, x, y, dir, dmg, theta, player);
	dynList_add(pObject_list, (void*)new);
	return new;
}

void spawn_mObject(struct map* map, int x, int y, mObject_type type, char id)
{
	struct mObject* new = (struct mObject*)malloc(sizeof(struct mObject));
	new->type = type;
	new->id = id;
	init_mObject(new, x, y, map);
	dynList_add(map->mObject_list, (void*)new);
	new->killable == true ? map->aggresive_mObj_count++ : 1;
}

void check_damage_modifiers(struct mObject *target, struct pObject *source, struct player* player)
{	
	struct rune* rune = (struct rune*)dynList_get(player->rune_list, 0);
	if(rune != NULL && rune->info.rune_type == blood)
	{
		player->health += player->sword_damage * 0.05;
	}

	rune = (struct rune*)dynList_get(player->rune_list, 2);
	if(rune != NULL && rune->info.rune_type == rot && target->status_effect.type == status_rot)
	{
		target->health -= 0.5 * source->damage;
	}
}

void stasis_status_effect(struct mObject *mObject, struct player* player)
{
	mObject->base_speed = 0;
	//printf("IM STUNNED\n");
	if(mObject->status_effect.timer > mObject->status_effect.limit)
	{
		mObject->base_speed = 1;
		set_status_effect(mObject, 0, 0, status_none);
	}
	mObject->status_effect.timer ++;
}

void frozen_status_effect(struct mObject *mObject, struct player* player)
{
	//change to basespeed when able
	mObject->base_speed = 0.25;
	if(mObject->status_effect.timer > mObject->status_effect.limit)
	{
		struct rune* rune = (struct rune*)dynList_get(player->rune_list, 3);
		if(rune != NULL && rune->info.rune_type == frost)
		{
			mObject->health -= mObject->health * 0.5;
			printf("frost procced %f\n", mObject->health);
		}
		set_status_effect(mObject, 0, 0, status_none);
		mObject->base_speed = 1;
	}
	mObject->status_effect.timer ++;
}

void rot_status_effect(struct mObject *mObject)
{
	if(getTick() % 60 == 0)
	{
		mObject->health -= mObject->health * 0.05;
		printf("%f\n", mObject->health);
	}
	if(mObject->status_effect.timer > mObject->status_effect.limit)
	{
		set_status_effect(mObject, 0, 0, status_none);
	}
	mObject->status_effect.timer ++;
}

void identify_status_effect(struct mObject *mObject, struct player* player)
{
	switch(mObject->status_effect.type)
	{
		case status_frostbite:
			frozen_status_effect(mObject, player);
			break;
		case status_rot:
			rot_status_effect(mObject);
			break;
		case STATUS_STASIS:
			stasis_status_effect(mObject, player);
			break;
		default:
			break;
	}
}

void delete_mObject(dynList *mObject_list, int index)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);	
	if(currentKeyStates[SDL_SCANCODE_F])
	{
		dynList_del_index(mObject_list, 0);
	}
	if(currentKeyStates[SDL_SCANCODE_H])
	{
		for(int i = 0; i < mObject_list->size; i++)
		{
			struct mObject *target = (struct mObject*)dynList_get(mObject_list, i);
			if(target->hittable == true)
				target->st.type = st_enemydead;
		}
		//dynList_destroy(mObject_list);
		//sleep(2);
	}
}
void update_all_mObjects(dynList *mObject_list, struct player *player, struct map *map, struct cam *cam, dynList *ev_list)
{
	if(dynList_is_empty(mObject_list))
		return;
	for(int i = 0; i < mObject_list->size; i++)
	{
		struct mObject *curr_mObj = (struct mObject*)dynList_get(mObject_list, i);
		if(curr_mObj->st.type == st_clear)
		{
			dynList_del_index(mObject_list, i);
		}
		update_mObject(curr_mObj, player, map, mObject_list, ev_list);	
	}

}

void update_pObjects(dynList *pObject_list, struct player *player, struct map* map)
{
	if(dynList_is_empty(pObject_list))
		return;
	for(int i = 0; i < pObject_list->size; i++)
	{
		struct pObject *curr_pObj = (struct pObject*)dynList_get(pObject_list, i);

		update_pObject(curr_pObj, player, map);
		if(curr_pObj->st.type == ST_PO_DEAD)
		{
			dynList_del_index(pObject_list, i);
		}
	}
}

void draw_all_mObjects(SDL_Renderer *renderer, dynList *mObject_list, struct cam *cam, SDL_Texture *tex, struct player *player)
{
	if(dynList_is_empty(mObject_list))
		return;
	for(int i = 0; i < mObject_list->size; i++)
	{
		struct mObject* curr_mObj = (struct mObject*)dynList_get(mObject_list, i);
		draw_mObject(renderer, curr_mObj, cam, tex, player);
	}
}

void draw_pObjects(SDL_Renderer *renderer, dynList *pObject_list, struct cam *cam, SDL_Texture*tex)
{
	if(dynList_is_empty(pObject_list))
		return;
	for(int i = 0; i < pObject_list->size; i++)
	{
		struct pObject* curr_pObj = (struct pObject*)dynList_get(pObject_list, i);
		draw_pObject(renderer, curr_pObj, cam, tex);
	}
}


void rune_player_interaction(struct mObject *mObject, struct player* player, struct map *map)
{
	const Uint8* cks = SDL_GetKeyboardState(NULL);
	if(cks[SDL_SCANCODE_E])
	{
		if(AABB((struct mObject*)player, mObject))
		{
			add_rune(player, mObject->r_info, map);
			set_mObject_state(mObject, st_deathrattle, NULL, 0, 32);
			for(int i = 0; i < map->mObject_list->size; i++)
			{
				struct mObject* curr = (struct mObject*)dynList_get(map->mObject_list, i);
				if(curr->id == 'R')
				{
					set_mObject_state(curr, st_deathrattle, state_deathrattle, 0, 32);
				}
			}
		}
	}
}

void tp_player_interaction(struct mObject *mObject, struct player* player, struct map *map)
{

	if(mObject->killable)
		return;
	dynList *ev_list = map->event_list;

	double dx = player->x - mObject->x, dy = player->y - mObject->y;
	const Uint8* currentKeyState = SDL_GetKeyboardState(NULL);
	if(currentKeyState[SDL_SCANCODE_E])
	{
		if(AABB((struct mObject*)player, mObject) && mObject->id == 'E')
		{
			if(map->aggresive_mObj_count <= 0)
				add_event(ev_list, type_event_teleport, player, map, 0);
			else
				printf("Enemies left: %d!\n", map->aggresive_mObj_count);
			//add_event(ev_list, type_event_lock);
		}
		if(AABB((struct mObject*)player, mObject) && mObject->id == 'T')
		{
			add_event(ev_list, type_event_inmaptp, player, map, 0);
			//add_event(ev_list, type_event_lock);
		}
	}
}

void player_move(struct player *player, struct map *map, struct cam *cam)
{
	double new_x = 0, new_y = 0;
	bool flight = false;
	new_x = player->x + player->vel_x;
	new_y = player->y + player->vel_y;

	//struct rune* rune = (struct rune*)dynList_get(player->rune_list, 2);
	/*	
		if(rune == NULL)
		{
		flight = false;	
		}
		else if(!(rune->info.rune_type == holy))
		{
		flight = true;
		}
		*/
	double offw = player->width/TILE_LENGTH;
	double offh = player->height/TILE_LENGTH;
#if 1
	int fuckx = (int)(player->width/TILE_LENGTH);
	int fuck = (int)(player->height/TILE_LENGTH);
// 3/2 -> (1-offw) 5/2 ->
	double wunderkindw = ((int)player->width <= TILE_LENGTH) ? 1 - (offw) : (offw - (int)offw != 0 ? -1 * (fuckx-offw) : 0.0);
	double wunderkindh = ((int)player->height <= TILE_LENGTH) ? 1 - (offh) : (offh - (int)offh != 0 ? -1 * (fuck-offh) : 0.0);
	if(1)
	{
		if(player->vel_x <= 0)
		{
			if(map_get_solid(map, (int)new_x, (int)player->y) || map_get_solid(map, (int)new_x, (int)(player->y + player->height/TILE_LENGTH - 0.1)))
			{
				new_x = (int)new_x + player->width/TILE_LENGTH * TILE_LENGTH/player->width;
				player->vel_x = 0;
			}
		}
		else
		{
			if(map_get_solid(map, (int)(new_x + player->width/TILE_LENGTH), (int)player->y) || map_get_solid(map, (int)(new_x + player->width/TILE_LENGTH), (int)(player->y + player->height/TILE_LENGTH - 0.1)))
			{
				new_x = (int)new_x + wunderkindw;
				player->vel_x = 0;
			}
		}
		if(player->vel_y <= 0)
		{
			if(map_get_solid(map, (int)new_x, (int)(new_y)) || map_get_solid(map, (int)(new_x + player->width/TILE_LENGTH - 0.1), (int)(new_y)))
			{
				new_y = (int)new_y + 1;
				player->vel_y = 0;
			}
		}
		else
		{
			if(map_get_solid(map, (int)(new_x), (int)(new_y + player->height/TILE_LENGTH - 0.1)) || map_get_solid(map, (int)(new_x + player->width/TILE_LENGTH - 0.1), (int)(new_y + player->height/TILE_LENGTH - 0.1)))
			{
				new_y = (int)new_y + wunderkindh;
				player->vel_y = 0;
			}
		}
	}
#endif
#if 0
	if(map_get_solid(map, (int)new_x, (int) new_y))
		return;
#endif
	player->x = new_x;
	player->y = new_y;

	//extra clamp check
#if 0
	if(player->x >= (double)map->width-2)
		player->x = (double)map->width-2;
	if(player->y >= (double)map->height-2)
		player->y = (double)map->height-2;
	if(player->x < 1)
		player->x = 1;
	if(player->y < 1)
		player->y = 1;
#endif
}

#if 0
	if(player->health <= 0)
	{
		struct rune *rune = (struct rune*)dynList_get(player->rune_list, 3);
		if(rune != NULL && rune->info.rune_type == holy && rune->attribute > 0)
		{
			player->health = player->maxhealth*rune->attribute/4;
			rune->attribute --;
			return;
		}
		player->health = 0;
		return;
	}
#endif

void updatePlayer(struct player *player, struct map *map, struct cam *cam, dynList *e_list, dynList *ev_list, dynList *pObject_list)
{
	//always exec
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	player->vel_x = 0;
	player->vel_y = 0;
	//tmp
	if(player->maxhealth < player->health)
		player->health = player->maxhealth;
	if(currentKeyStates[SDL_SCANCODE_9] || 0)
		player->health = player->maxhealth;
	if(player->health == 0)
		return;
	rune_abilities(player, map);
	//always exec
	//tmp
	//
	//
	switch(player->global_state)
	{
		case st_p_normal:
			player_invuln(player);
			dash_control(player, currentKeyStates);
			player_inp_move(player, currentKeyStates);
			player_move(player, map, cam);
			input_attack(player, map, currentKeyStates);
			break;
		case st_p_knockbacked:
			player_knockbacked(player, cam, map);
			break;
		case ST_P_DASH:
			player_dash(player, map, cam);
			break;
		case ST_P_ATTACKING:
			player_attack(player, map, currentKeyStates);
		default:
			break;
	}
	if(player->global_state == st_p_knockbacked || player->global_state == ST_P_ATTACKING)
		return;

}

void updateSword(struct player *sword, struct player *player)
{


}


void pObject_move(struct pObject *pObject, struct player *player, struct map *map)
{
	//double dx = player->x - mObject->x, dy = player->y - mObject->y;
	double new_x = 0, new_y = 0;
	bool hit_wall = false;

	pObject->vel_x = pObject->speed*cos(pObject->theta);
	pObject->vel_y = pObject->speed*sin(pObject->theta);
	new_x = pObject->x + pObject->vel_x;
	new_y = pObject->y + pObject->vel_y;

	double f = 0.1;
	double offw = pObject->width/TILE_LENGTH;
	double offh = pObject->height/TILE_LENGTH;

	double wunderkindw = ((int)pObject->width <= TILE_LENGTH) ? 1 - offw : 0.0;
	double wunderkindh = ((int)pObject->height <= TILE_LENGTH) ? 1 - offh : 0.0;
#if 1
	if(pObject->transp || pObject->pen_wall == true)
	{
		if(pObject->vel_x <= 0)
		{
			if(map_get_solid(map, (int)new_x, (int)pObject->y) || map_get_solid(map, (int)new_x, (int)(pObject->y + (offh - f))))
			{
				new_x = (int)new_x + 1;
				pObject->vel_x = 0;
				hit_wall = true;
			}
		}
		else
		{
			if(map_get_solid(map, (int)(new_x + offw), (int)pObject->y) || map_get_solid(map, (int)(new_x + offw), (int)(pObject->y + (offh - f))))
			{
				new_x = (int)new_x + wunderkindw;
				pObject->vel_x = 0;
				hit_wall = true;
			}
		}
		if(pObject->vel_y <= 0)
		{
			if(map_get_solid(map, (int)new_x, (int)(new_y)) || map_get_solid(map, (int)(new_x + (offw - f)), (int)(new_y)))
			{
				new_y = (int)new_y + 1;
				pObject->vel_y = 0;
				hit_wall = true;
			}
		}
		else
		{
			if(map_get_solid(map, (int)(new_x), (int)(new_y + offh)) || map_get_solid(map, (int)(new_x + (offw - f)), (int)(new_y + offh)))
			{
				new_y = (int)new_y + wunderkindh;
				pObject->vel_y = 0;
				hit_wall = true;
			}
		}
	}
#endif

	if(hit_wall /* &&? */)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, 16);
		//pObject->st.acp = NULL;
		/*
		   pObject->timer = 0;
		   pObject->limit = 16;
		   pObject->global_state = ST_PO_DEATHRATTLE;
		   */
	}

	pObject->x = new_x;
	pObject->y = new_y;


}

void set_status_effect_area(struct mObject* mObject, struct map *map, int distancesquared, mObject_status_effect effect)
{
	for(int i = 0; i < map->mObject_list->size; i++)
	{
		struct mObject* mObj_cond = (struct mObject*)dynList_get(map->mObject_list, i);
		double dx = mObject->x - mObj_cond->x, dy = mObject->y - mObj_cond->y;
		if(dx * dx + dy * dy < distancesquared)
		{
			set_status_effect(mObj_cond, 0, 30, effect);
		}
	}

}

void mObject_move(struct mObject *mObject, struct player *player, struct map *map)
{
	//double dx = player->x - mObject->x, dy = player->y - mObject->y;
	double new_x = 0, new_y = 0;
	//write a paragraph about why this didnt work in reverse order
	mObject->vel_x = mObject->speed*cos(mObject->theta);
	mObject->vel_y = mObject->speed*sin(mObject->theta);
	new_x = mObject->x + mObject->vel_x;
	new_y = mObject->y + mObject->vel_y;



	const double offw = mObject->width/TILE_LENGTH;
	const double offh = mObject->height/TILE_LENGTH;
	int fuckx = (int)(mObject->width/TILE_LENGTH);
	int fuck = (int)(mObject->height/TILE_LENGTH);
	double wunderkindw = ((int)mObject->width <= TILE_LENGTH) ? 1 - (offw) : (offw - (int)offw != 0 ? -1 * (fuckx-offw) : 0.0);
	double wunderkindh = ((int)player->height <= TILE_LENGTH) ? 1 - (offh) : (offh - (int)offh != 0 ? -1 * (fuck-offh) : 0.0);

	double f = 0.1;
	bool hit_wall = false;

	//TODO 
	if(mObject->vel_x <= 0.0)
	{
		if(map_get_solid(map, (int)(new_x + 0), (int)(mObject->y + 0)) || map_get_solid(map, (int)(new_x + 0), (int)(mObject->y + (offh - f))))
		{
			new_x = (int)new_x + 1;
			mObject->vel_x = 0;
			hit_wall = true;
		}
	}
	else
	{
		if(map_get_solid(map, (int)(new_x + offw), (int)(mObject->y + 0)) || map_get_solid(map, (int)(new_x + offw), (int)(mObject->y + (offh - f))))
		{
			new_x = (int)(new_x) + wunderkindw;
			mObject->vel_x = 0;
			hit_wall = true;
		}
	}
	if(mObject->vel_y <= 0.0)
	{
		if(map_get_solid(map, (int)(new_x + 0), (int)(new_y + 0)) || map_get_solid(map, (int)(new_x + (offw - f)), (int)(new_y + 0)))
		{
			new_y = (int)new_y + 1;
			mObject->vel_y = 0;
			hit_wall = true;
		}
	}
	else
	{
		if(map_get_solid(map, (int)(new_x), (int)(new_y + offh)) || map_get_solid(map, (int)(new_x + (offw-f)), (int)(new_y + offh)))
		{
			new_y = (int)(new_y) + wunderkindh;
			mObject->vel_y = 0;
			hit_wall = true;
		}
	}


	if(mObject->st.type == st_enemyknockback && hit_wall == true)
	{
		struct rune* rune = (struct rune*)dynList_get(player->rune_list, 2);
		if(rune != NULL && rune->info.rune_type == gravity)
		{
			rune = (struct rune*)dynList_get(player->rune_list, 3);
			if(rune != NULL && rune->info.rune_type == gravity)
			{
				set_status_effect_area(mObject, map, 4, STATUS_STASIS);
			}
			mObject->health -= player->sword_damage * 1;
			hit_wall = false;
			mObject->st.type = 0;
		}
	}

	mObject->wall_collide = hit_wall;
	mObject->x = new_x;
	mObject->y = new_y;

}

bool divine_shield(struct player *player)
{
	//maybe use div_shield as a counter instead
	//and at tick 1800 it recharges seems underwhelming now
	struct rune* rune = (struct rune*)dynList_get(player->rune_list, 0);
	if(!rune)
		return false;
	bool res = rune->info.rune_type == holy && rune->attribute == true ? true : false;
	rune->attribute = false;
	return res;
}

void player_hit(struct player* player, double damage, double theta) 
{
	player->invuln = true;
	player->timer = 0;
	player->invuln_limit = 48;
	if(divine_shield(player) == true)
		return;
	player->health -= damage;
	player->speed = 0.3;
	player->theta = theta;
	player->global_state = st_p_knockbacked;
}

void mObject_damage(struct mObject* target, struct pObject *source, struct player *player)
{
	int healthbefore = target->health;
	target->health -= source->damage;
	target->inv_frames = 0;
	check_damage_modifiers(target, source, player);
	//this segfaulted for no reason before be careful
	set_status_effect(target, 0, 360, source->status_effect);
	printf("damage dealt %f dmg: %f\n", healthbefore - target->health, source->damage);
	if(!source->knockbacker || target->hyperarmor)
		return;
	const double dx = OBJDIFF(target, source, 'X'), dy = OBJDIFF(target, source, 'Y');
	target->theta = atan2(dy, dx);
	target->speed = 0.2 * source->knockkoef - target->mass / 1024;
	target->speed = target->speed <= 0 ? 0 : target->speed;
	set_mObject_state(target, st_enemyknockback, state_enemy_knockbacked, 0, 20);
}

void mObject_player_hitbox(struct mObject *mObject, struct player *player)
{
	double dx = player->x - mObject->x, dy = player->y - mObject->y;
	if(!player->invuln && AABB(mObject, (struct mObject*)player))
	{
		player_hit(player, mObject->contact_damage, mObject->theta);
	}
}

void mObject_active_chase(struct mObject *mObject, struct player *player, struct map *map, struct cam *cam)
{
	double dx = player->x - mObject->x, dy = player->y - mObject->y;
	mObject->speed = 0.05;
	mObject->theta = atan2(dy, dx);
	if(dx * dx + dy * dy > 48)
	{
		//printf("I'm hecking Done chasing\n");
		mObject->st.type = st_spawn;
	}
	mObject_move(mObject, player, map);
	//mObject_player_hitbox(mObject, player);

}

void mObject_idle_moving(struct mObject *mObject, struct player *player, struct map *map, struct cam *cam)
{
	double dx = player->x - mObject->x, dy = player->y - mObject->y;
	if(mObject->st.timer > mObject->st.limit)
	{
		mObject->speed = 0.01;
		if(rand() % 2 == 1)
		{	
			mObject->speed = 0;
		}
		mObject->theta = get_frand(PI * 2, 0);//((double)rand()/(double)(RAND_MAX)) * 3.1415 * 2;
		mObject->st.timer = 0;
		mObject->st.limit = rand() % 80 + 80;
	}
	if(dx * dx + dy * dy < 16)
	{
		mObject->st.type = st_runner_m2;
	}

	//mObject_player_hitbox(mObject, player);
	mObject_move(mObject, player, map);
	mObject->st.timer ++;
}

void update_mObject(struct mObject *mObject, struct player *player, struct map *map, struct cam *cam, dynList *ev_list)
{
	state_enemy_default(mObject, player, map);
	if(mObject->st.acp == NULL)
		return;		
	mObject->st.acp(mObject, player, map);
}

void update_pObject(struct pObject *pObject, struct player *player, struct map *map)
{
	if(pObject->st.acp == NULL)
		return;
	pObject->st.acp(pObject, player, map);
}

void drawPlayer(SDL_Renderer *renderer, struct player *player, struct cam *cam, SDL_Texture *tex)
{
	int lul = 0;
	SDL_Rect s = {0, 0, 16, 24};
	//SDL_Rect s1 = {80, 80, 32, 32};
	SDL_Rect R = {(player->x - cam->offset_x) * TILE_LENGTH, (player->y - cam->offset_y) * TILE_LENGTH - lul, player->width, player->height + lul}; //20


	//render_player_animation(player, R, renderer, tex);

	//SDL_RenderCopy(renderer, tex, &s, &R);
	//SDL_RenderCopy(renderer, tex, &s1, &R);
	SDL_SetRenderDrawColor(renderer, 0xDC, 0xA0, 0x22, 0xFF);
	SDL_RenderFillRect(renderer, &R);
}

void draw_pObject(SDL_Renderer *renderer, struct pObject *pObject, struct cam *cam, SDL_Texture* tex)
{
	SDL_Rect r = {(pObject->x - cam->offset_x) * TILE_LENGTH, (pObject->y - cam->offset_y) * TILE_LENGTH, pObject->width, pObject->height};


	render_animation(pObject, tex, r, renderer);
}

void draw_mObject(SDL_Renderer *renderer, struct mObject *mObject, struct cam *cam, SDL_Texture *tex, struct player* player)
{
	SDL_Rect r = {(mObject->x - cam->offset_x) * TILE_LENGTH - 0/*(mObject->width - TILE_LENGTH)*/, (mObject->y - cam->offset_y) * TILE_LENGTH - (mObject->sprite.h * 0), mObject->width, mObject->height};
	//0.8 , 2.5*mObject->sprite.h
	if(mObject->id != '6' && mObject->id != '7' && mObject->id != 'R' && mObject->id != '2' && mObject->id != 'z' && mObject->id != '5' && mObject->id != '4' && mObject->id != 'B' && mObject->id != 'c' && mObject->id != 'o')
		return;


	if(mObject->id == '7' && 0) 
	{

		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(renderer, &r);
		return;
	}
	render_mObject_animation(mObject, r, renderer, tex, player);

}
#if 0
mobj_t*
	P_SpawnMobj
( fixed_t	x,
  fixed_t	y,
  fixed_t	z,
  mobjtype_t	type )
{
	mobj_t*	mobj;
	state_t*	st;
	mobjinfo_t*	info;

	mobj = Z_Malloc (sizeof(*mobj), PU_LEVEL, NULL);
	memset (mobj, 0, sizeof (*mobj));
	info = &mobjinfo[type];

	mobj->type = type;
	mobj->info = info;
	mobj->x = x;
	mobj->y = y;
	mobj->radius = info->radius;
	mobj->height = info->height;
	mobj->flags = info->flags;
	mobj->health = info->spawnhealth;


#endif

#if 0
	switch(mObject->st.type)
	{
		case st_spawn:
			state_crawler_idle(mObject, player, map);
			break;
		case st_crawler_m2:
			state_crawler_move(mObject, player, map);
			break;
		case st_m1:
		case st_crawler_m3:
			state_crawler_stay(mObject, player, map);
			break;
		case st_enemyknockback:
			state_crawler_knockbacked(mObject, player, map);
			break;
	}
#endif

