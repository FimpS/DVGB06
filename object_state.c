#include "global.h"
#include "objects.h"
#include "object_state.h"
#include "dynList.h"
#include "map.h"

double sum_square(double x, double y)
{
	return x * x + y * y;
}

void find_higheset(double *dxs, double *dys, int size, double *res)
{
	double highest = *dxs * *dxs + *dys * *dys;
	res[0] = dxs[0];
	res[1] = dys[0];
	for(int i = 1; i < size; i++)
		if(highest > dxs[i] * dxs[i] + dys[i] * dys[i])
		{
			highest = dxs[i] * dxs[i] + dys[i] * dys[i];
			res[0] = dxs[i];
			res[1] = dys[i];
		}
}

void clueless(struct mObject *mObject, struct player *player, struct map *map)
{
	if(mObject->st.timer > mObject->st.limit)
	{
		mObject->st.acp(mObject, player, map);
	}
	mObject->st.timer ++;
}

void identify_mObject_sprite_location(struct mObject *mObject)
{
	mObject->anim.timer = 0;
	switch(mObject->st.type)
	{
		case ST_CRAWLER_IDLE:
			mObject->sprite.x = 0;
			mObject->anim.limit = 8;
			mObject->anim.start_frame = 0;
			break;
		case ST_CRAWLER_DASH:
			mObject->sprite.x = 64;
			mObject->anim.limit = 4;
			mObject->anim.start_frame = 64;
			break;
		case ST_BALISTA_IDLE:
			mObject->sprite.x = 0;
			mObject->anim.limit = 16;
			mObject->anim.frames = 1;
			mObject->anim.start_frame = 0;
			break;
		case ST_BALISTA_AWARE:
			mObject->sprite.x = 0;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.frames = 4;
			mObject->anim.start_frame = 0;
			break;
		case ST_ARCHER_IDLE:
			mObject->sprite.x = 0;
			mObject->sprite.y = 160;
			mObject->anim.limit = 16;
			mObject->anim.start_frame = 0;
			break;
		case ST_ARCHER_AWARE:
			mObject->sprite.x = 0;
			mObject->sprite.y = 160;
			mObject->anim.limit = 8;
			mObject->anim.start_frame = 0;
			break;
		case ST_ARCHER_DASH:
			mObject->sprite.x = 64;
			mObject->sprite.y = 160;
			mObject->anim.limit = 4;
			mObject->anim.start_frame = 64;
			break;
		case ST_ARCHER_DRAW:
			mObject->sprite.x = 0;
			mObject->sprite.y = 184;
			mObject->anim.limit = mObject->st.limit / 4 ;
			mObject->anim.start_frame = 0;
			break;
		case ST_SWORDSMAN_IDLE:
			mObject->sprite.x = 0;
			mObject->sprite.y = 112;
			mObject->anim.limit = 16;
			mObject->anim.start_frame = 0;
			break;
		case ST_SWORDSMAN_AWARE:
			mObject->sprite.x = 0;
			mObject->sprite.y = 112;
			mObject->anim.limit = 8;
			mObject->anim.start_frame = 0;
			break;
		case ST_SWORDSMAN_READY:
			mObject->sprite.x = 0;
			mObject->sprite.y = 136;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 0;
			break;
		case ST_SWORDSMAN_SLASH:

			mObject->sprite.y = 112;
			break;
		case ST_SWORDSMAN_DASH:
			mObject->sprite.x = 64;
			mObject->sprite.y = 112;
			mObject->anim.limit = 4;
			mObject->anim.start_frame = 64;
			break;
		case ST_MAGUS_IDLE:
			mObject->anim.limit = 12;
			mObject->sprite.x = 0;
			mObject->anim.start_frame = 0;
			break;
		case ST_MAGUS_AWARE:
			mObject->sprite.y = 0;
			mObject->anim.limit = 6;
			mObject->sprite.x = 0;
			mObject->anim.start_frame = 0;
			break;
		case ST_MAGUS_DASH:
			mObject->sprite.x = 64;
			mObject->sprite.y = 0;
			mObject->anim.limit = 48;
			mObject->anim.start_frame = 64;
			break;
		case ST_MAGUS_READY:
			mObject->sprite.x = 0;
			mObject->sprite.y = 24;
			mObject->anim.limit = 16;
			mObject->anim.start_frame = 0;
			break;
		case ST_SUMMONER_IDLE:
			mObject->sprite.x = 0;
			mObject->sprite.y = 48;
			mObject->anim.limit = 12;
			mObject->anim.start_frame = 0;
			break;
		case ST_SUMMONER_DASH:
			mObject->sprite.x = 64;
			mObject->sprite.y = 48;
			mObject->anim.limit = 6;
			mObject->anim.start_frame = 0;
			break;
		case ST_SUMMONER_SUMMON:
			mObject->sprite.x = 0;
			mObject->sprite.y = 72;
			mObject->anim.limit = 32;
			mObject->anim.start_frame = 0;
			break;
		case ST_CHIEFTAIN_AWARE:
			mObject->sprite.x = 0;
			mObject->sprite.y = 224;
			mObject->anim.limit = 8;
			mObject->anim.start_frame = 0;
			break;
		case ST_CHIEFTAIN_DASH:
			mObject->sprite.x = 64;
			mObject->sprite.y = 224;
			mObject->anim.limit = 4;
			mObject->anim.start_frame = 64;
			break;
		case ST_CHIEFTAIN_SUMMON:
			mObject->sprite.x = 64;
			mObject->sprite.y = 248;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 64;
			break;
		case ST_CHIEFTAIN_READY:
			mObject->sprite.x = 0;
			mObject->sprite.y = 248;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 0;
			break;
		case st_enemyknockback:
			switch(mObject->id)
			{
				case '6':
					mObject->sprite.y = 0;
					mObject->sprite.x = 128;
					mObject->anim.limit = mObject->st.limit + 124;
					mObject->anim.start_frame = 128;
					break;
				case '7':
					mObject->sprite.y = 48;
					mObject->sprite.x = 128;
					mObject->anim.limit = mObject->st.limit + 124;
					mObject->anim.start_frame = 128;
					break;
				case 'z':
				case '2':
					mObject->sprite.x = 128;
					mObject->anim.start_frame = 128;
					mObject->anim.limit = mObject->st.limit + 124;
					mObject->anim.limit = mObject->st.limit;
					break;
				case '4':
					mObject->sprite.x = 128;
					mObject->sprite.y = 160;
					mObject->anim.start_frame = 128;
					mObject->anim.limit = mObject->st.limit + 124;
					break;
				case '5':
					mObject->sprite.x = 128;
					mObject->sprite.y = 112;
					mObject->anim.start_frame = 128;
					mObject->anim.limit = mObject->st.limit + 124;
					break;
				}
			break;
		case st_deathrattle:
			switch(mObject->id)
			{
				case 'B':
					mObject->sprite.x = 192;
					mObject->anim.limit = mObject->st.limit / 4;
					mObject->anim.frames = 4;
					mObject->anim.start_frame = 192;
					break;
				case '4':
					mObject->sprite.y = 160;
					mObject->sprite.x = 192;
					mObject->anim.limit = mObject->st.limit / 4;
					mObject->anim.start_frame = 192;
					break;
				case '5':
					mObject->sprite.y = 112;
					mObject->sprite.x = 192;
					mObject->anim.limit = mObject->st.limit / 4;
					mObject->anim.start_frame = 192;
					break;
				case '6':	
					mObject->sprite.y = 0;
					mObject->sprite.x = 192;
					mObject->anim.limit = mObject->st.limit / 4 + 0;
					mObject->anim.start_frame = 192;
					break;
				case '7':
					mObject->sprite.y = 48;
					mObject->sprite.x = 192;
					mObject->anim.limit = mObject->st.limit / 4 + 0;
					mObject->anim.start_frame = 192;
					break;
				case 'R':
					mObject->anim.start_frame = 128;
					mObject->sprite.x = 128;
					mObject->anim.limit = mObject->st.limit / 4 - 0;
					break;
				case 'z':
				case '2':
					mObject->sprite.x = 192;
					mObject->anim.start_frame = 192;
					mObject->anim.limit = mObject->st.limit / 4;
					break;
				case 'c':
					mObject->sprite.x = 192;
					mObject->sprite.y = 224;
					mObject->anim.start_frame = 192;
					mObject->anim.limit = mObject->st.limit / 4;
					break;
			}
			break;

	}
}

void set_mObject_state(struct mObject *mObject, mObject_state type,
		void (*acp)(struct mObject*, 
			struct player*, 
			struct map *),
		int timer,
		int limit)
{
	mObject->st.type = type;
	mObject->st.acp = acp;
	mObject->st.timer = timer;
	mObject->st.limit = limit;
	identify_mObject_sprite_location(mObject);
}

void identify_pObject_sprite_location(struct pObject *pObject)
{
	switch(pObject->st.type)
	{
		case ST_PO_DEATHRATTLE:
			pObject->anim_timer = 0;
			pObject->anim_limit = 4;
			pObject->sprite.x = 64;
			pObject->anim_start_frame = 64;
			break;
		case ST_PO_DEAD:
			pObject->anim_timer = 0;
			pObject->anim_limit = 0;
			pObject->sprite.x = 0;
			pObject->anim_start_frame = 0;
			break;
	}
}

void set_pObject_state(struct pObject *pObject, pObject_global_state type,
		void (*acp)(struct pObject*,
			struct player*,
			struct map*),
		int timer,
		int limit)
{
	pObject->st.type = type;
	pObject->st.acp = acp;
	pObject->st.timer = timer;
	pObject->st.limit = limit;

	identify_pObject_sprite_location(pObject);
}

void state_crawler_idle(struct mObject *mObj, struct player *player, struct map *map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	if(sum_square(dy, dx) < CRAWLER_RANGE && mObj->st.timer > mObj->st.limit)
	{
		mObj->theta = atan2(dy, dx);
		mObj->speed = mObj->base_speed / 6;
		set_mObject_state(mObj, ST_CRAWLER_DASH, state_crawler_dash, 0, 48);

	}
	mObj->st.timer ++;
}

void state_crawler_dash(struct mObject* mObj, struct player *player, struct map *map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	mObject_move(mObj, player, map);
	mObject_player_hitbox(mObj, player);
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, ST_CRAWLER_IDLE, state_crawler_idle, 0, rand() % 40 + 20);
		mObj->theta = atan2(dy, dx);
	}
	mObj->st.timer ++;
}

void state_enemy_knockbacked(struct mObject* mObj, struct player *player, struct map *map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	if(mObj->st.timer > mObj->st.limit || mObj->speed <= 0)
	{
		set_mObject_state(mObj, mObj->type_reg, mObj->st.kcp, 0, 40);
		mObj->theta = atan2(dy, dx);
		mObj->speed = 0;
		//mObj->theta = atan2(dy, dx);
	}
	mObject_move(mObj, player, map);
	mObj->speed -= 0.01 * (mObj->mass / 40);

	mObj->st.timer ++;
}

void state_rusher_idle(struct mObject *mObj, struct player *player, struct map *map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	mObject_player_hitbox(mObj, player);
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, st_m1, state_rusher_idlewalk, 0, 40);
		mObj->theta = get_frand(TAU, 0.0); //((double)rand()/(double)(RAND_MAX)) * 3.1415 * 2;
	}
	if(( (mObj->y < player->y + player->height/TILE_LENGTH && 
					mObj->y + mObj->height/TILE_LENGTH > player->y) || 
				(mObj->x < player->x + player->width/TILE_LENGTH &&
				 mObj->x + mObj->width/TILE_LENGTH > player->x)) && dx * dx + dy * dy < 100)
	{
		set_mObject_state(mObj, st_m1, state_rusher_rush, 0, 80);
		mObj->theta = atan2(dy, dx);
		mObj->speed = mObj->base_speed/20;
	}
	mObj->st.timer ++;
}

void state_rusher_rush(struct mObject *mObj, struct player *player, struct map *map)
{
	//double dx = player->x - mObj->x, dy = player->y - mObj->y;
	mObj->speed += 0.005;
	mObject_player_hitbox(mObj, player);
	mObject_move(mObj, player, map);
	if(mObj->st.timer > mObj->st.limit || mObj->wall_collide == true)
	{
		set_mObject_state(mObj, st_m1, state_rusher_idle, 0, 40);
		mObj->wall_collide = false;
		mObj->speed = mObj->base_speed/100;
	}

	mObj->st.timer ++;
}

void state_rusher_idlewalk(struct mObject *mObj, struct player *player, struct map *map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	mObject_move(mObj, player, map);
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, st_m1, state_rusher_idle, 0, 80);
	}

	if(( (mObj->y < player->y + player->height/TILE_LENGTH && 
					mObj->y + mObj->height/TILE_LENGTH > player->y) || 
				(mObj->x < player->x + player->width/TILE_LENGTH &&
				 mObj->x + mObj->width/TILE_LENGTH > player->x)) && dx * dx + dy * dy < 100)
	{
		set_mObject_state(mObj, st_m1, state_rusher_rush, 0, 120);
		mObj->theta = atan2(dy, dx);
		mObj->speed = mObj->base_speed/6.67;
	}
	mObj->st.timer ++;
}

void state_balista_idle(struct mObject *mObj, struct player *player, struct map *map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	//mObj->theta = PI/2;
	if(sum_square(dy , dx) < 64 && mObj->st.timer > mObj->st.limit)
	{
		mObj->theta = atan2(dy ,dx);
		set_mObject_state(mObj, ST_BALISTA_AWARE, state_balista_aware, 0, 64);
	}
	mObj->st.timer ++;
}

void state_balista_aware(struct mObject *mObj, struct player *player, struct map *map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	if(getTick() % 24 == 0)
	{
		mObj->theta = atan2(dy ,dx);
	}

	if(mObj->st.timer > mObj->st.limit)
	{
		double t = atan2(dy, dx);
		spawn_pObject(map->pObject_list, mObj->x + 0.25, mObj->y + 0.25, balista_bolt, NORTH, 20.0, mObj->theta, player);
		set_mObject_state(mObj, ST_BALISTA_IDLE, state_balista_idle, 0, 64);
	}
	mObj->st.timer ++;

}

void state_archer_draw(struct mObject *mObj, struct player *player, struct map* map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	mObj->theta = atan2(dy, dx);
	if(mObj->st.timer > mObj->st.limit)
	{
		spawn_pObject(map->pObject_list, mObj->x + 0.25, mObj->y + 0.25, balista_bolt, EAST, 40.0, atan2(dy, dx), player);
		set_mObject_state(mObj, ST_ARCHER_AWARE, state_archer_aware, 0, 120);
		return;
	}
	mObj->st.timer ++;
}

void state_archer_idle(struct mObject *mObj, struct player *player, struct map* map)
{
	double dx = mObj->x - player->x, dy = mObj->y - player->y;
	if(sum_square(dx, dy) < HOSTILE_MOBJ_WAKEUP_DIST)
	{
		set_mObject_state(mObj, ST_ARCHER_AWARE, state_archer_aware, 0, 100);
		return;
	}
	mObj->st.timer ++;
}

void state_archer_dash(struct mObject *mObj, struct player *player, struct map* map)
{
	mObject_move(mObj, player, map);
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, ST_ARCHER_AWARE, state_archer_aware, 0, 120);
		return;
	}
	mObj->st.timer ++;
}

void state_archer_aware(struct mObject *mObj, struct player *player, struct map* map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	if(sum_square(dx, dy) < ARCHER_INRANGE)
	{
		set_mObject_state(mObj, ST_ARCHER_DRAW, state_archer_draw, 0, 64);
		return;
	}
	if(mObj->st.timer > mObj->st.limit)
	{
		mObj->speed = mObj->base_speed / 12;
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, ST_ARCHER_DASH, state_archer_dash, 0, 48);
		return;
	}
	mObj->st.timer ++;
}

//SWORDSMAN

void state_swordsman_slash(struct mObject *mObj, struct player *player, struct map* map)
{
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, ST_SWORDSMAN_AWARE, state_swordsman_aware, 0, 96);
		return;
	}
	mObj->st.timer ++;
}

void state_swordsman_ready(struct mObject *mObj, struct player *player, struct map* map)
{
	if(mObj->st.timer > mObj->st.limit)
	{
		double dx = player->x - mObj->x, dy = player->y - mObj->y;
		double theta = atan2(dy, dx);
		mObj->theta = theta;
		spawn_pObject(map->pObject_list,mObj->x + 0.2*cos(theta) - 0.2, 0.3 + mObj->y + 0.2*sin(theta), PO_SWORDSMAN_SWORD, EAST, 35.0, atan2(dy ,dx), player);
		set_mObject_state(mObj, ST_SWORDSMAN_SLASH, state_swordsman_slash, 0, 48);
		return;
	}
	mObj->st.timer ++;
}

void state_swordsman_dash(struct mObject *mObj, struct player *player, struct map* map)
{
	mObject_move(mObj, player, map);
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, ST_SWORDSMAN_AWARE, state_swordsman_aware, 0, 96);
		return;
	}
	mObj->st.timer ++;

}

void state_swordsman_aware(struct mObject *mObj, struct player *player, struct map* map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	if(sum_square(dx, dy) < SWORDSMAN_INRANGE)
	{
		set_mObject_state(mObj, ST_SWORDSMAN_READY, state_swordsman_ready, 0, 32);
		return;
	}
	if(mObj->st.timer > mObj->st.limit)
	{
		mObj->speed = mObj->base_speed / 6;
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, ST_SWORDSMAN_DASH, state_swordsman_dash, 0, 16);
		return;
	}
	mObj->st.timer ++;


}

void state_swordsman_idle(struct mObject *mObj, struct player *player, struct map* map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	if(sum_square(dx, dy) < HOSTILE_MOBJ_WAKEUP_DIST)
	{
		set_mObject_state(mObj, ST_SWORDSMAN_AWARE, state_swordsman_aware, 0, 100);
		return;
	}
	mObj->st.timer ++;
}
//MAGUS

void state_magus_ready(struct mObject *mObj, struct player *player, struct map* map)
{
	if(mObj->st.timer > mObj->st.limit)
	{
		spawn_pObject(map->pObject_list, player->x, player->y + 6, PO_MAGIC_BOLT, NORTH, 20.0, -PI/2, player);
		spawn_pObject(map->pObject_list, player->x, player->y - 6, PO_MAGIC_BOLT, SOUTH, 20.0, PI/2, player);
		set_mObject_state(mObj, ST_MAGUS_AWARE, state_magus_aware, 0, 96);
		return;
	}
	mObj->st.timer ++;
}

void state_magus_chase(struct mObject *mObj, struct player *player, struct map* map)
{
	mObject_move(mObj, player, map);
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, ST_MAGUS_AWARE, state_magus_aware, 0, 96);
		return;
	}
	mObj->st.timer ++;
}

void state_magus_aware(struct mObject *mObj, struct player *player, struct map* map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	if(sum_square(dx, dy) < MAGUS_CAST_RANGE && mObj->st.timer > MAGUS_CAST_COOLDOWN)
	{
		set_mObject_state(mObj, ST_MAGUS_READY, state_magus_ready, 0, 64);
	}
	if(mObj->st.timer > mObj->st.limit)
	{
		mObj->speed = mObj->base_speed / 8;
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, ST_MAGUS_DASH, state_magus_chase, 0, 16);
	}
	mObj->st.timer ++;
}

void state_magus_idle(struct mObject *mObj, struct player *player, struct map* map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	if(sum_square(dx, dy) < HOSTILE_MOBJ_WAKEUP_DIST)
	{
		set_mObject_state(mObj, ST_MAGUS_AWARE, state_magus_aware, 0, 100);
		return;
	}
	mObj->st.timer ++;
}


void state_summoner_idle(struct mObject *mObj, struct player *player, struct map *map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	if(sum_square(dy, dx) < SUMMONER_RANGE)
	{
		mObj->speed = mObj->base_speed / 25;
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, ST_SUMMONER_DASH, state_summoner_dash, 0, 64);	
	}
	mObj->st.timer ++;
}

void state_summoner_dash(struct mObject *mObj, struct player* player, struct map* map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	//mObj->theta = atan2(dy, dx);
	mObject_move(mObj, player, map);
	if(sum_square(dy, dx) > SUMMONER_RANGE + 4 && mObj->st.timer > mObj->st.limit)
	{
		mObj->speed = 0;
		set_mObject_state(mObj, ST_SUMMONER_IDLE, state_summoner_idle, 0, 0);
	}
	if(mObj->st.timer > mObj->st.limit && sum_square(dy, dx) < SUMMONER_RANGE - 9)
	{
		mObj->speed = 0;
		set_mObject_state(mObj, ST_SUMMONER_SUMMON, state_summoner_found, 0, 128);
	}
	mObj->st.timer ++;
}

void state_summoner_found(struct mObject *mObj, struct player *player, struct map *map)
{

	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	int count = 0;
	if(mObj->st.timer > mObj->st.limit)
	{
		mObj->speed = mObj->base_speed / 25;
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, ST_SUMMONER_DASH, state_summoner_dash, 0, 64);
		for(int i = 0; i < map->mObject_list->size; i++)
			if(((struct mObject*)dynList_get(map->mObject_list, i))->id == 'z')
				count++;
		printf("count: %d\n", count);
		if(count < 2)
			for(int i = -1; i <= 1; i += 2)
			{
				spawn_mObject(map, mObj->x + i, mObj->y - 1, crawler, 'z');
			}
		else
		{
			mObj->speed = mObj->base_speed / 16;
			mObj->theta = atan2(-dy, -dx);
			set_mObject_state(mObj, ST_SUMMONER_DASH, state_summoner_dash, 0, 196);
		}
	}
	mObj->st.timer ++;
}

//GOLEM /BOSS\

void state_golem_stomp(struct mObject *mObj, struct player *player, struct map* map)
{
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, 0, state_golem_aware, 0, GOLEM_ABILITY_COOLDOWN);
#if 0
		for(int i = 0; i < 4; i++)
			spawn_pObject(map->pObject_list, x + rand() % 9, y + rand() % 9, PO_GOLEM_ROCKS, EAST, 20.0, down, player);
#endif
		return;
	}
	mObj->st.timer ++;
}

void state_golem_hit(struct mObject *mObj, struct player *player, struct map* map)
{
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, 0, state_golem_aware, 0, GOLEM_ABILITY_COOLDOWN);
		return;
	}
	mObj->st.timer ++;
}

void state_golem_ready(struct mObject *mObj, struct player *player, struct map* map)
{
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, 0, state_golem_hit, 0, 64);
		//spawn_pObject(map->pObject_list, x, y, GOLEM_MELEE_WEAPON, EAST, 20.0, theta, player);
		return;
	}
	mObj->st.timer ++;
}

void state_golem_dash(struct mObject *mObj, struct player *player, struct map* map)
{
	mObject_move(mObj, player, map);
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, 0, state_golem_aware, 0, GOLEM_ABILITY_COOLDOWN);
		return;
	}
	mObj->st.timer ++;
}

void state_golem_aware(struct mObject *mObj, struct player *player, struct map* map)
{
	const double offx = mObj->width/TILE_LENGTH, offy = mObj->height/TILE_LENGTH;
	const double offx2 = player->width/TILE_LENGTH, offy2 = player->height/TILE_LENGTH;
	const double dx = player->x + offx/2 - (mObj->x + offx/2), dy = player->y + offy/2 - (mObj->y + offy/2);

#if 1
	mObj->st.timer ++;
	if(mObj->st.timer < mObj->st.limit)
	{
		return;
	}


	if(sum_square(dy, dx) > GOLEM_STOMP_RANGE && 1)
	{
		set_mObject_state(mObj, 0, state_golem_stomp, 0, 64);
	}
	else if(sum_square(dy, dx) < GOLEM_ATTACK_RANGE && 1)
	{
		set_mObject_state(mObj, 0, state_golem_ready, 0, 64);
	}
	else
	{
		mObj->speed = mObj->base_speed / 16;
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, 0, state_golem_dash, 0, 64);
	}
#endif 




}

//CHIEFTAIN /BOSS\

void state_chieftain_summon(struct mObject *mObj, struct player * player, struct map* map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	int count = 0;
	if(mObj->st.timer > mObj->st.limit)
	{
		mObj->speed = mObj->base_speed / 25;
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, ST_CHIEFTAIN_AWARE, state_chieftain_aware, 0, 24);
		for(int i = 0; i < map->mObject_list->size; i++)
			if(((struct mObject*)dynList_get(map->mObject_list, i))->id == 'z')
				count++;
		printf("count: %d\n", count);
		if(count < 2)
		{
			spawn_mObject(map, mObj->x + 1, mObj->y - 1, crawler, 'z');
			spawn_mObject(map, mObj->x - 1, mObj->y + 1, crawler, 'z');
			spawn_mObject(map, mObj->x + 1, mObj->y + 1, crawler, 'z');
			spawn_mObject(map, mObj->x - 1, mObj->y - 1, crawler, 'z');
		}
	}
	mObj->st.timer ++;

}

void state_chieftain_ready(struct mObject *mObj, struct player *player, struct map *map)
{
	if(mObj->st.timer > mObj->st.limit)
	{
		int spam_chance = rand() % 2;
		spawn_pObject(map->pObject_list, player->x, player->y + 6, PO_MAGIC_BOLT, NORTH, 20.0, -PI/2, player);
		spawn_pObject(map->pObject_list, player->x, player->y - 6, PO_MAGIC_BOLT, SOUTH, 20.0, PI/2, player);
		spawn_pObject(map->pObject_list, player->x + 6, player->y, PO_MAGIC_BOLT, EAST, 20.0, PI, player);
		spawn_pObject(map->pObject_list, player->x - 6, player->y, PO_MAGIC_BOLT, SOUTH, 20.0, 0.0, player);

		if(spam_chance)
			set_mObject_state(mObj, ST_CHIEFTAIN_READY, state_chieftain_ready, 0, 32);
		else
			set_mObject_state(mObj, ST_CHIEFTAIN_AWARE, state_chieftain_aware, 0, 24);
		return;
	}
	mObj->st.timer ++;

}


//void state_chieftain_tired ?? maybe good ??

void state_chieftain_dash(struct mObject *mObj, struct player *player, struct map* map)
{
	mObject_move(mObj, player, map);
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, ST_CHIEFTAIN_AWARE, state_chieftain_aware, 0, 32);
	}
	mObj->st.timer ++;
}

void state_chieftain_aware(struct mObject *mObj, struct player *player, struct map *map)
{
	//cooldown/resttime
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	mObj->st.timer ++;
	if(mObj->st.timer < mObj->st.limit)
	{
		return;
	}
	//skills

	//int spell = rand() % 5;

	//printf("got here\n");
	bool flip = mObj->atts.cheiftain_ticker ++ % 2;
	//printf("flip: %d\n", flip);
	if(flip)
		if(sum_square(dy, dx) > 9)
		{
			mObj->speed = mObj->base_speed / 4;
			mObj->theta = atan2(dy, dx);
			set_mObject_state(mObj, ST_CHIEFTAIN_DASH, state_chieftain_dash, 0, 16);
			return;
		}
		else
		{
			mObj->speed = mObj->base_speed / 4;
			mObj->theta = atan2(-dy, -dx);
			set_mObject_state(mObj, ST_CHIEFTAIN_DASH, state_chieftain_dash, 0, 16);
			return;
		}

	if(sum_square(dy, dx) < CULTIST_RANGE_SUMMON)
	{
		int count = 0;
		for(int i = 0; i < map->mObject_list->size; i++)
			if(((struct mObject*)dynList_get(map->mObject_list, i))->id == 'z')
				count++;
		if(count > 2)
		{
			set_mObject_state(mObj, ST_CHIEFTAIN_READY, state_chieftain_ready, 0, 32);
			return;
		}

		set_mObject_state(mObj, ST_CHIEFTAIN_SUMMON, state_chieftain_summon, 0, 80);
		return;
	}

}

void check_sword_dir(struct pObject *pObject, struct player *player)
{
	switch(pObject->dir)
	{   
		case NORTH:
			pObject->x = player->x;
			pObject->y = player->y - 1;
			pObject->theta = -PI/2;
			break;
		case SOUTH:
			pObject->x = player->x;
			pObject->y = player->y + 1;
			pObject->theta = PI/2;
			break;
		case EAST:
			pObject->x = player->x + 1;
			pObject->y = player->y;
			pObject->theta = 0;
			break;
		case WEST:
			pObject->x = player->x - 1;
			pObject->y = player->y;
			pObject->theta = PI;
			break;
	}
}

void state_pObject_deathrattle(struct pObject *pObject, struct player* player, struct map *map)
{
	if(pObject->st.timer > pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, 0);
		return;
	}
	pObject->st.timer ++;
}

void state_swordsman_sword_swing(struct pObject* pObject, struct player* player, struct map* map)
{
	double dx = player->x - pObject->x, dy = player->y - pObject->y;
	pObject->speed = 0.05;
	pObject_move(pObject, player, map);
	if(pObject->st.timer > pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, 16);
		return;
	}
	if(!player->invuln && AABBpp(pObject, player))
	{
		player_hit(player, pObject->damage, pObject->theta);
	}
	pObject->st.timer ++;
}

void state_magic_bolt_travel(struct pObject* pObject, struct player* player, struct map* map)
{
	pObject_move(pObject, player, map);
	if(pObject->st.timer > pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, 16);
		return;
	}
	pObject->st.timer ++;
	if(!player->invuln && AABB((struct mObject*)pObject, player))
	{
		player_hit(player, pObject->damage, pObject->theta);
	}
}

void state_balista_shot(struct pObject *pObject, struct player *player, struct map *map)
{
	if(pObject->st.timer > pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, 0);
		return;
	}
	double dx = player->x - pObject->x, dy = player->y - pObject->y;
	pObject_move(pObject, player, map);

	if(!player->invuln && AABBpp(pObject, player))
	{
		player_hit(player, pObject->damage, pObject->theta);
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, DEATHRATTLE_LIMIT);
	}
	pObject->st.timer ++;
}

//player

void player_knockbacked(struct player* player, struct cam* cam, struct map *map)
{
	if(player->global_state == st_p_knockbacked)
	{
		player->speed -= 0.03;
		//TODO Check this
		player->vel_x = player->speed*cos(player->theta);
		player->vel_y = player->speed*sin(player->theta);
		player_move(player, map, cam);
		player->timer++; 
		if(player->timer > 16 || player->speed <= 0)
		{   
			//player->timer = 0; 
			player->global_state = st_p_normal;
			player->speed = player->base_speed / 10;
			//player->invuln = false;
		}
	}
}

void player_invuln(struct player *player)
{
	if(player->invuln == true)
	{
		if(player->timer > player->invuln_limit)
		{
			player->global_state = st_p_normal;
			player->invuln = false;
		}
		player->timer++;
		//printf("%d\n", player->timer);
	}

}

float mysqrt(float numer)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = numer * 0.5F;
	y = numer;
	i = *(long*) &y;
	i = 0x5f3759df - (i >> 1);
	y = * ( float * ) &i;

	y = y * ( threehalfs - (x2 * y * y));
	return y;
}

void set_dash_vector(struct player* player)
{
	double speed = 0.4;
	double speed2 = 0.4;
	switch(player->dir)
	{
		case EAST:
			player->vel_x = speed * cos(0);
			player->vel_y = speed * sin(0);
			break;
		case WEST:
			player->vel_x = speed * cos(PI);
			player->vel_y = speed * sin(PI);
			break;
		case SOUTH:
			player->vel_x = speed * cos(PI/2);
			player->vel_y = speed * sin(PI/2);
			break;
		case NORTH:
			player->vel_x = speed * cos(-1 * PI/2);
			player->vel_y = speed * sin(-1 * PI/2);
			break;
			//normalize these
		case SOUTHEAST:
			player->vel_x = speed2 * cos(PI/4);
			player->vel_y = speed2 * sin(PI/4);
			break;
		case SOUTHWEST:
			player->vel_x = speed2 * cos(3*PI/4);
			player->vel_y = speed2 * sin(3*PI/4);
			break;
		case NORTHWEST:
			player->vel_x = speed2 * cos(-1 * 3*PI/4);
			player->vel_y = speed2 * sin(-1 * 3*PI/4);
			break;
		case NORTHEAST:
			player->vel_x = speed2 * cos(-1 * PI/4);
			player->vel_y = speed2 * sin(-1 * PI/4);
			break;
		default:
			break;

			player->vel_x = (player->vel_x/sqrt(player->vel_x * player->vel_x + player->vel_y * player->vel_y));
			player->vel_y = (player->vel_y/sqrt(player->vel_x * player->vel_x + player->vel_y * player->vel_y));
	}
}

void player_dash(struct player* player, struct map* map, struct cam* cam)
{
	if(player->global_state == ST_P_DASH)
	{
		set_dash_vector(player);
		player_move(player, map, cam);
		player->dash_timer ++;
		if(player->dash_timer > PLAYER_DASH_LIMIT)
		{
			player->global_state = st_p_normal;
			player->speed = player->base_speed / 10;
			player->dash_timer = 0;
		}
	}
}

void dash_control(struct player* player, const Uint8 *cks)
{
	if(cks[SDL_SCANCODE_SPACE] && player->dash_cooldown_timer > PLAYER_DASH_COOLDOWN_LIMIT)
	{
		player->global_state = ST_P_DASH;
		player->invuln = true;
		player->dash_timer = 0;
		player->timer = 0;
		player->invuln_limit = PLAYER_DASH_INVULN_LIMIT;
		player->dash_cooldown_timer = 0;
	}
	player->dash_cooldown_timer ++;
}

void player_inp_move(struct player* player, const Uint8 *currentKeyStates)
{
	if(player->global_state == ST_P_DASH)
		return;
	if(currentKeyStates[SDL_SCANCODE_D])
	{
		player->vel_x = player->speed;
		player->dir = EAST;
		player->theta = 0;
	}
	if(currentKeyStates[SDL_SCANCODE_A])
	{
		player->vel_x = -player->speed;
		player->dir = WEST;
		player->theta = PI;
	}
	if(currentKeyStates[SDL_SCANCODE_W])
	{
		player->vel_y = -player->speed;
		player->dir = NORTH;
	}
	if(currentKeyStates[SDL_SCANCODE_S])
	{
		player->vel_y = player->speed;
		player->dir = SOUTH;
	}
	if(player->vel_x > 0 && player->vel_y > 0)
	{
		player->dir = SOUTHEAST;
		player->theta = 0;
	} 
	else if(player->vel_x > 0 && player->vel_y < 0)
	{
		player->dir = NORTHEAST;
		player->theta = 0;
	}
	else if(player->vel_x < 0 && player->vel_y > 0)
	{
		player->dir = SOUTHWEST;
		player->theta = PI;
	}
	else if(player->vel_x < 0 && player->vel_y < 0)
	{
		player->dir = NORTHWEST;
		player->theta = PI;
	}
	if(player->vel_x == 0 && player->vel_y == 0)
	{
		player->dir = SOUTH;
	}
	const double tmp = player->vel_x;	
	//player->vel_x = player->vel_x != 0 || player->vel_y != 0 ? 0.075 : 0;
	//player->vel_y = player->vel_y != 0 || player->vel_y != 0 ? 0.075 : 0;
	//replace with just 1.414
	player->vel_x = player->vel_x != 0 || player->vel_y != 0 ? player->base_speed / 7 * (player->vel_x/sqrt(player->vel_x * player->vel_x + player->vel_y * player->vel_y)) : 0;
	player->vel_y = tmp != 0 || player->vel_y != 0 ? player->base_speed / 7 * (player->vel_y/sqrt(tmp * tmp + player->vel_y * player->vel_y)) : 0;
	//player->vel_x = player->vel_x != 0 || player->vel_y != 0 ? (player->base_speed / 5) * player->vel_y*0.707106 : 0;
	//player->vel_y = tmp != 0 || player->vel_y != 0 ? (player->base_speed / 5) * player->vel_x*0.707106 : 0;

}

void player_attack(struct player* player, struct map* map, const Uint8 *currentKeyStates)
{
	if(player->attack_speed_timer > player->attack_speed)
	{   
		if(currentKeyStates[SDL_SCANCODE_RIGHT])
		{   
			spawn_pObject(map->pObject_list, player->x + 1, player->y, sword, EAST, player->sword_damage, 0.0, player);
			player->attack_speed_timer = 0;
		}
		else if(currentKeyStates[SDL_SCANCODE_LEFT])
		{   
			spawn_pObject(map->pObject_list, player->x - 1, player->y, sword, WEST, player->sword_damage, 0.0, player);
			player->attack_speed_timer = 0;
		}
		else if(currentKeyStates[SDL_SCANCODE_DOWN])
		{   
			spawn_pObject(map->pObject_list, player->x, player->y + 1, sword, SOUTH, player->sword_damage, 0.0, player); 
			player->attack_speed_timer = 0;
		}
		else if(currentKeyStates[SDL_SCANCODE_UP])
		{   
			spawn_pObject(map->pObject_list, player->x, player->y - 1, sword, NORTH, player->sword_damage, 0.0, player); 
			player->attack_speed_timer = 0;
		}
	}
	player->attack_speed_timer++;
}

void state_blood_tax(struct pObject *pObject, struct player *player, struct map *map)
{
	if(pObject->st.timer > pObject->st.limit || pObject->penetration_index <= 0)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, 16);
		return;
	}
#if 1
	pObject_move(pObject, player, map);
	double dx, dy; 
	for(int i = 0; i < map->mObject_list->size; i++)
	{   
		struct mObject* target = ((struct mObject*)dynList_get(map->mObject_list, i));
		//fixed?
		if(!target->hittable)
			continue;
		dx = pObject->x - target->x;
		dy = pObject->y - target->y;
		if(!target->hit && AABB(target, (struct mObject*)pObject))
		{   
			dy = pObject->y - player->y;
			dx = pObject->x - player->x;
			target->theta = atan2(dy, dx);
			pObject->penetration_index --;
			target->inv_frames = 0;
			mObject_damage(target, pObject, player);
			target->hit = true;
		}
	}
#endif
	pObject->st.timer ++;
}


void check_pObject_mObject_hit(struct pObject *pObject, struct player* player, struct map* map)
{
	struct mObject* target;
	double dx, dy;
	for(int i = 0; i < map->mObject_list->size; i++)
	{
		target = (struct mObject*)dynList_get(map->mObject_list, i);
		if(!target->hittable)
			continue;
		if(!target->hit && AABB(target, (struct mObject*)pObject))
		{
			dx = pObject->x - target->x;
			dy = pObject->y - target->y;
			//target->theta = atan2(dy, dx);
			target->inv_frames = 0;
			mObject_damage(target, pObject, player);
			target->hit = true;

		}
	}
}

void state_brimstone_beam(struct pObject* pObject, struct player* player, struct map* map)
{
	if(pObject->st.timer > pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, 0);
		return;
	}
	pObject->st.timer ++;

	switch(pObject->dir)
	{
		case EAST:
			pObject->x = player->x + 1;
			pObject->y = player->y - 0.5;
			break;
		case WEST:
			pObject->x = player->x - pObject->width/TILE_LENGTH;
			pObject->y = player->y - 0.5;
			break;
		case SOUTH:
			pObject->x = player->x - 0.5;
			pObject->y = player->y + 1;
			break;
		case NORTH:
			pObject->x = player->x - 0.5;
			pObject->y = player->y - pObject->height/TILE_LENGTH;
			break;
	}
	check_pObject_mObject_hit(pObject, player, map);
}


//pObjects
void state_spell_fire(struct pObject *pObject, struct player *player, struct map *map)
{
	if(pObject->st.timer > pObject->st.limit || pObject->penetration_index <= 0)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, NULL, 0, 16);
		return;
	}
	pObject_move(pObject, player, map);
	double dx, dy; 
	for(int i = 0; i < map->mObject_list->size; i++)
	{   
		struct mObject* target = ((struct mObject*)dynList_get(map->mObject_list, i));
		dx = pObject->x - target->x;
		dy = pObject->y - target->y;
		if(dx * dx + dy * dy < 1 && !target->hit)
		{   
			dy = pObject->y - player->y;
			dx = pObject->x - player->x;
			target->theta = atan2(dy, dx);
			pObject->penetration_index --;
			target->inv_frames = 0;
			mObject_damage(target, pObject, player);
			target->hit = true;
		}
	}
	pObject->st.timer ++;
}

void state_wraith_found(struct pObject *pObject, struct player *player, struct map* map)
{
#if 0
	if(pObject->st.timer > pObject->st.limit)
	{
		pObject->global_state = ST_PO_DEAD;
	}
	pObject->st.timer ++;
	printf("founded\n");
	pObject_move(pObject, player, map);
#endif
}


void state_wraith_follow(struct pObject *pObject, struct player *player, struct map *map)
{
	double dx, dy;
	struct mObject *curr;
	bool found = false;
	//stick to a mObject maybe otherwise its gonna find [1] all the time
#if 1
	if(pObject->st.timer > pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, NULL, 0, 16);
		return;
	}
	pObject->st.timer ++;
	for(int i = 0; i < map->mObject_list->size; i++)
	{
		curr = (struct mObject*)dynList_get(map->mObject_list, i);
		if(curr->killable == false)
			continue;
		dx = curr->x - pObject->x;
		dy = curr->y - pObject->y;
		if(dx * dx + dy * dy < 16)
		{
			found = true;
			break;
		}
	}
	if(!found)
	{
		dx = player->x - pObject->x;
		dy = player->y - pObject->y;
	}
	if(AABB(curr, pObject))
	{
		if(!curr->hit && getTick() % 60 == 0)
		{
			curr->theta = PI + atan2(dy, dx);
			curr->inv_frames = 0;
			mObject_damage(curr, pObject, player);
			curr->hit = true;
		}
	}
	if(dx * dx + dy * dy < 0.15)
		return;
	pObject->theta = atan2(dy, dx); 
	pObject_move(pObject, player, map);
#endif
}
#if 0
void Estate_wraith_follow(struct pObject *pObject, struct player *player, struct map *map)
{
	double dx, dy;
	struct mObject *curr;
	bool found = false;
	int size = 0, index = 0;
	double dxs[128], dys[128], res[2];
	//stick to a mObject maybe otherwise its gonna find [1] all the time
#if 1
	if(pObject->st.timer > pObject->st.limit)
	{
		pObject->global_state = ST_PO_DEAD;
	}
	pObject->st.timer ++;
	for(int i = 0; i < map->mObject_list->size; i++)
	{
		curr = (struct mObject*)dynList_get(map->mObject_list, i);
		if(curr->killable == false)
			continue;
		dx = curr->x - pObject->x;
		dy = curr->y - pObject->y;
		if(dx * dx + dy * dy < 64)
		{
			dxs[index] = dx;
			dys[index++] = dy;
			size ++;
			found = true;
		}
	}
	printf("%d\n", size);
	find_higheset(dxs, dys, size, res);
	if(!found)
	{
		dx = player->x - pObject->x;
		dy = player->y - pObject->y;
	}
	if(AABB(curr, pObject))
	{
		if(!curr->hit && getTick() % 60 == 0)
		{
			curr->theta = PI + atan2(dy, dx);
			curr->inv_frames = 0;
			mObject_damage(curr, pObject, player);
			curr->hit = true;
		}
	}
	if(dx * dx + dy * dy < 0.15)
		return;
	pObject->theta = atan2(res[1], res[0]);
	pObject_move(pObject, player, map);
#endif
}
#endif


void state_gravity_well_travel(struct pObject *pObject, struct player* player, struct map* map)
{
	if(pObject->st.timer > pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, 0);
		return;
	}
	//pObject_move(pObject, player, map);
	if(getTick() % 20 == 0)
	{
		double angle = get_frand(2*PI);
		spawn_pObject(map->pObject_list, pObject->x, pObject->y, gravity_bolt, NORTH, player->sword_damage * 0.5, angle, player);
		//spawn_pObject(map->pObject_list, pObject->x, pObject->y, gravity_bolt, SOUTH, player->sword_damage * 0.5, angle + 2*PI/3, player);
		//spawn_pObject(map->pObject_list, pObject->x, pObject->y, gravity_bolt, NORTH, player->sword_damage * 0.5, angle - 2*PI/3, player);
	}
	pObject->st.timer ++;
}

void state_gravity_bolt_travel(struct pObject *pObject, struct player* player, struct map* map)
{
	if(pObject->st.timer > pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, NULL, 0, DEATHRATTLE_LIMIT);
		return;
	}
	pObject->thetaacc += 0.0011; //0.0011
	pObject->speed += 0.0025; //prev 0.0025 //0.0005
	pObject->theta += 0.05; //prev 0.05 //0.15 - thetaacc
	pObject_move(pObject, player, map);
	check_pObject_mObject_hit(pObject, player, map);
	//pObject->st.timer ++;

}

void state_rot_smog_flower(struct pObject *pObject, struct player* player, struct map* map)
{
	if(pObject->st.timer > pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, NULL, 0, DEATHRATTLE_LIMIT);
		return;
	}
	check_pObject_mObject_hit(pObject, player, map);
	pObject->st.timer ++;
}

void state_sword_swing(struct pObject *pObject, struct player *player, struct map *map)
{
	if(pObject->st.timer > pObject->st.limit || pObject->penetration_index <= 0)
	{   
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, 0);
	}   
	check_sword_dir(pObject, player);
	double dx, dy;
	//scary O(p*m) stuff
	//check_pObject_mObject_hit(pObject, player, map);

	for(int i = 0; i < map->mObject_list->size; i++)
	{
		struct mObject* target = ((struct mObject*)dynList_get(map->mObject_list, i));
		if(!target->hittable)
			continue;
		if(!target->hit && AABB(target, (struct mObject*)pObject)) 
		{
			dy = pObject->y - player->y;
			dx = pObject->x - player->x;
			target->theta = atan2(dy, dx);
			target->inv_frames = 0;
			pObject->penetration_index --;
			mObject_damage(target, pObject, player);
			target->hit = true;
		}
	}

	pObject->st.timer ++; 
}

void state_deathrattle(struct mObject *mObject, struct player *player, struct map *map)
{
	if(mObject->st.timer > mObject->st.limit)
	{
		set_mObject_state(mObject, st_clear, state_deathrattle, 0, 60);
		struct rune* rune = (struct rune*)dynList_get(player->rune_list, 0);
		if(rune != NULL && rune->info.rune_type == unholy)
			rune->attribute ++;

		rune = (struct rune*)dynList_get(player->rune_list, 2);
		if(rune != NULL && rune->info.rune_type == frost)
			rune->attribute ++;

		rune = (struct rune*)dynList_get(player->rune_list, 2);
		if(rune != NULL && rune->info.rune_type == rot)
			rune->attribute ++;

		rune = (struct rune*)dynList_get(player->rune_list, 2);
		if(rune != NULL && rune->info.rune_type == unholy)
			player->sword_damage += 3;
	}
	mObject->st.timer ++;
}

void state_enemy_default(struct mObject *mObject, struct player* player, struct map *map)
{
	/*
	   if(mObject->st.type == st_deathrattle)
	   {
	   if(mObject->st.timer > mObject->st.limit)
	   {
	   mObject->st.type = st_clear;
	   return;
	   }
	   mObject->st.timer++;
	   return;
	   }
	   */
	if(!mObject->hittable || mObject->st.type == st_deathrattle)
		return;
	identify_status_effect(mObject, player);
	if(mObject->st.type == st_enemydead)
	{
		if(mObject->killable)
			map->aggresive_mObj_count --;
		set_mObject_state(mObject, st_deathrattle, state_deathrattle, 0, 64);
		return;
	}
	if(mObject->inv_frames > 10)
	{
		mObject->hit = false;
	}
	mObject->inv_frames++;
	if(mObject->health <= 0)
	{
		mObject->st.type = st_enemydead;
	}
}
