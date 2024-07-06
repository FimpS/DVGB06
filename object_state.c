#include "global.h"
#include "objects.h"
#include "object_state.h"
#include "dynList.h"
#include "map.h"
#include "info.h"

void set_mObject_state(struct mObject *mObject, mObject_global_state type,
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
	if(sum_square(dy, dx) < CRAWLER_RANGE && mObj->st.timer >= mObj->st.limit)
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
	if(mObj->st.timer >= mObj->st.limit)
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
		spawn_pObject(map->pObject_list, mObj->x + 0.25, mObj->y + 0.25, PO_BALISTA_BOLT, NORTH, 20.0, mObj->theta, player);
		set_mObject_state(mObj, ST_BALISTA_IDLE, state_balista_idle, 0, 64);
	}
	mObj->st.timer ++;

}

void state_peak_local_idle(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(sum_square(dy, dx) <= HOSTILE_MOBJ_WAKEUP_DIST)
	{
		set_mObject_state(mObj, ST_PEAK_LOCAL_AWARE, state_peak_local_aware, 0, 144);
		return;
	}
}
void state_peak_local_aware(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(mObj->st.timer >= mObj->st.limit)
	{
		mObj->speed = mObj->base_speed / 24;
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, ST_PEAK_LOCAL_DASH, state_peak_local_dash, 0, 144);	
		return;
	}
	if(mObj->st.timer >= mObj->st.limit / 2 && sum_square(dy, dx) <= PEAK_LOCAL_CAST_RANGE)
	{
		set_mObject_state(mObj, ST_PEAK_LOCAL_CAST, state_peak_local_cast, 0, 144);	
		return;
	}
	mObj->st.timer ++;
}
void state_peak_local_dash(struct mObject* mObj, struct player* player, struct map* map)
{
	mObject_move(mObj, player, map);
	if(mObj->st.timer++ >= mObj->st.limit)
	{
		set_mObject_state(mObj, ST_PEAK_LOCAL_AWARE, state_peak_local_aware, 0, 144);
		return;
	}
}
void state_peak_local_cast(struct mObject* mObj, struct player* player, struct map* map)
{
	if(mObj->st.timer++ >= mObj->st.limit)
	{
		spawn_pObject(map->pObject_list, player->x, player->y, PO_SWAMP_POOL, EAST, 0.0, 0.0, player);	
		set_mObject_state(mObj, ST_PEAK_LOCAL_AWARE, state_peak_local_aware, 0, 144);
		return;
	}
	return;
}
//TODO WTF IS GOING ON WITH KNOCKBACK something that was done today
void state_peak_knight_idle(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(sum_square(dy, dx) <= HOSTILE_MOBJ_WAKEUP_DIST)
	{
		set_mObject_state(mObj, ST_PEAK_KNIGHT_AWARE, state_peak_knight_aware, 0, 144);
		return;
	}
	
}
void state_peak_knight_aware(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(mObj->st.timer >= mObj->st.limit && sum_square(dy, dx) <= PEAK_KNIGHT_CAST_RANGE /*&& !(rand() % 3*/)
	{
		set_mObject_state(mObj, ST_PEAK_KNIGHT_CAST, state_peak_knight_cast, 0, 96);	
		return;
	}
	if(mObj->st.timer >= mObj->st.limit)
	{
		set_mObject_state(mObj, ST_PEAK_KNIGHT_DASH, state_peak_knight_dash, 0, 64);	
		mObj->speed = mObj->base_speed / 16;
		mObj->theta = atan2(dy, dx);
		return;
	}
	mObj->st.timer ++;
}
void state_peak_knight_dash(struct mObject* mObj, struct player* player, struct map* map)
{
	mObject_move(mObj, player, map);
	if(mObj->st.timer++ >= mObj->st.limit)
	{
		set_mObject_state(mObj, ST_PEAK_KNIGHT_AWARE, state_peak_knight_aware, 0, 144);
		return;
	}
}
void state_peak_knight_cast(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(mObj->st.timer++ >= mObj->st.limit)
	{
		for(int i = 0; i < 6; i++)
			spawn_pObject(map->pObject_list, player->x + get_frand(6.0, -3.0), player->y + get_frand(6.0, -3.0), PO_SPEAR_CAST, EAST, 20.0, 0.0, player);
		set_mObject_state(mObj, ST_PEAK_KNIGHT_AWARE, state_peak_knight_aware, 0, 120);
		return;
	}
}

void state_rock_well_idle(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(sum_square(dy, dx) <= HOSTILE_MOBJ_WAKEUP_DIST)
	{
		set_mObject_state(mObj, ST_ROCK_WELL_AWARE, state_rock_well_aware, 0, 128);
		return;
	}
}
void state_rock_well_aware(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	mObj->speed = mObj->base_speed / 24;
	pObject_seek((struct pObject*)mObj, 0.05, atan2(dy, dx));
	mObject_move(mObj, player, map);
	if(sum_square(dy, dx) <= ROCK_WELL_CAST_RANGE && mObj->st.timer ++ >= mObj->st.limit)
	{
		set_mObject_state(mObj, ST_ROCK_WELL_CAST, state_rock_well_cast, 0, 64);
		return;
	}

}
void state_rock_well_cast(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	mObj->speed = mObj->base_speed / 16;
	pObject_seek((struct pObject*)mObj, 0.1, atan2(dy, dx));
	mObject_move(mObj, player, map);
	if(mObj->st.timer++ >= mObj->st.limit)
	{
		const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
		spawn_pObject(map->pObject_list, mObj->x, mObj->y, PO_ROCK_CAST, EAST, 100.0, atan2(dy, dx), player);
		set_mObject_state(mObj, ST_ROCK_WELL_AWARE, state_rock_well_aware, 0, 128);
		return;
	}

}

void state_rock_roller_idle(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(sum_square(dy, dx) <= HOSTILE_MOBJ_WAKEUP_DIST)
	{
		set_mObject_state(mObj, ST_ROCK_ROLLER_AWARE, state_rock_roller_aware, 0, 128);
	}
}
void state_rock_roller_aware(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(sum_square(dy, dx) <= ROCK_ROLLER_ROLL_RANGE && mObj->st.timer ++ >= mObj->st.limit)
	{
		mObj->speed = mObj->base_speed / 30;
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, ST_ROCK_ROLLER_AROLL, state_rock_roller_aroll, 0, 32);
	}
	return;
}
void state_rock_roller_aroll(struct mObject* mObj, struct player* player, struct map* map)
{
	if(mObj->st.timer ++ >= mObj->st.limit)
	{
		set_mObject_state(mObj, ST_ROCK_ROLLER_DROLL, state_rock_roller_droll, 0, 64);
	}
}
void state_rock_roller_droll(struct mObject* mObj, struct player* player, struct map* map)
{
	mObj->st.timer++ >= mObj->st.limit ? mObj->speed -= 0.003 : (mObj->speed += 0.002);
	mObject_player_hitbox(mObj, player);
	mObject_move(mObj, player, map);
	if(mObj->speed <= 0.025)
	{
		set_mObject_state(mObj, ST_ROCK_ROLLER_AWARE, state_rock_roller_aware, 0, 128);
	}
}

void state_archer_draw(struct mObject *mObj, struct player *player, struct map* map)
{
	double dx = player->x - mObj->x, dy = player->y - mObj->y;
	mObj->theta = atan2(dy, dx);
	if(mObj->st.timer > mObj->st.limit)
	{
		spawn_pObject(map->pObject_list, mObj->x + 0.25, mObj->y + 0.25, PO_BALISTA_BOLT, EAST, 40.0, atan2(dy, dx), player);
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
	if(mObj->st.timer >= mObj->st.limit)
	{
		double dx = player->x - mObj->x, dy = player->y - mObj->y;
		double theta = atan2(dy, dx);
		mObj->theta = theta;
		set_mObject_state(mObj, ST_SWORDSMAN_READY, state_swordsman_ready, 0, 32);
		spawn_pObject(map->pObject_list,mObj->x + 0.2*cos(theta) - 0.2, 0.3 + mObj->y + 0.2*sin(theta), PO_SWORDSMAN_SWORD, EAST, 35.0, atan2(dy ,dx), player);
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
		set_mObject_state(mObj, ST_SWORDSMAN_AWARE, state_swordsman_aware, 0, 96);
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
		set_mObject_state(mObj, ST_SWORDSMAN_SLASH, state_swordsman_slash, 0, 48);
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
				spawn_mObject(map, mObj->x + i, mObj->y - 1, MO_CRAWLER, 'z');
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

void state_rider_idle(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(sum_square(dy, dx) <= HOSTILE_MOBJ_WAKEUP_DIST)
	{
		set_mObject_state(mObj, ST_RIDER_AWARE, state_rider_aware, 0, 124);
		return;
	}
	mObj->st.timer ++;
}

void state_rider_aware(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	mObj->theta = atan2(dy, dx);
	mObj->speed = mObj->base_speed / 40;
	mObject_move(mObj, player, map);
	if(mObj->st.timer >= mObj->st.limit && sum_square(dy, dx) <= RIDER_CHARGE_RANGE)
	{
		set_mObject_state(mObj, ST_RIDER_CHARGE, state_rider_charge, 0, 32);	
		mObj->theta = atan2(dy, dx);
		return;
	}
	mObj->st.timer ++;
}
void state_rider_charge(struct mObject* mObj, struct player* player, struct map* map)
{
	mObj->speed = mObj->base_speed / 6;
	mObject_move(mObj, player, map);
	mObject_player_hitbox(mObj, player);
	if(mObj->st.timer >= mObj->st.limit)
	{
		set_mObject_state(mObj, ST_RIDER_AWARE, state_rider_aware, 0, 96);
		return;
	}
	mObj->st.timer ++;
}

void state_drider_idle(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(sum_square(dy, dx) <= HOSTILE_MOBJ_WAKEUP_DIST)
	{
		set_mObject_state(mObj, ST_DRIDER_AWARE, state_drider_aware, 0, 96);
	}
	return;
}
void state_drider_aware(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(mObj->st.timer >= mObj->st.limit && sum_square(dy, dx) <= DRIDER_FIRE_RANGE)
	{
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, ST_DRIDER_READY, state_drider_ready, 0, 32);
		return;
	}
	if(mObj->st.timer >= mObj->st.limit)
	{
		mObj->speed = mObj->base_speed / 6;
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, ST_DRIDER_DASH, state_drider_dash, 0, 16);
		return;
	}
	mObj->st.timer ++;
}
void state_drider_dash(struct mObject* mObj, struct player* player, struct map* map)
{
	mObject_move(mObj, player, map);
	if(mObj->st.timer >= mObj->st.limit)
	{
		set_mObject_state(mObj, ST_DRIDER_AWARE, state_drider_aware, 0, 96);
		return;
	}
	mObj->st.timer ++;

}
void state_drider_ready(struct mObject* mObj, struct player* player, struct map* map)
{
	if(mObj->st.timer >= mObj->st.limit)
	{
		const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
		mObj->theta = atan2(dy, dx);
		mObj->speed = mObj->base_speed / 4;
		set_mObject_state(mObj, ST_DRIDER_FIRE_CHARGE, state_drider_fire_charge, 0, 48);
		return;
	}
	mObj->st.timer ++;
}
void state_drider_fire_charge(struct mObject* mObj, struct player* player, struct map* map)
{
	mObject_move(mObj, player, map);
	mObject_player_hitbox(mObj, player, map);
	if(mObj->st.timer == mObj->st.limit / 2)
	{
		const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
		spawn_pObject(map->pObject_list, mObj->x, mObj->y, PO_FIRE_SLING, EAST, 40.0, -1 * PI / 2, player);
		//spawn projectile
	}
	if(mObj->st.timer >= mObj->st.limit)
	{
		const double dx = OBJDIFFX(mObj, player), dy = OBJDIFFY(mObj, player);
		set_mObject_state(mObj, ST_DRIDER_AWARE, state_drider_aware, 0, 32);
		return;
	}
	mObj->st.timer ++;
}

void state_fire_tower_idle(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(sum_square(dy, dx) <= FIRE_TOWER_RANGE)
	{
		if(mObj->st.timer >= mObj->st.limit)
		{
			set_mObject_state(mObj, ST_FIRE_TOWER_FIRE, state_fire_tower_fire, 0, 128);
		}
		mObj->st.timer ++;
		return;
	}
}

void state_fire_tower_fire(struct mObject* mObj, struct player* player, struct map* map)
{
	if((mObj->st.timer + 1) % 43 == 0)
	{
		spawn_pObject(map->pObject_list, mObj->x, mObj->y, PO_FIRE_SLING, EAST, 40.0, -1 * PI / 2, player);
	}
	if(mObj->st.timer >= mObj->st.limit)
	{
		set_mObject_state(mObj, ST_FIRE_TOWER_IDLE, state_fire_tower_idle, 0, 144);
		return;
	}
	mObj->st.timer ++;
}

void state_fire_bomber_idle(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(sum_square(dy, dx) <= HOSTILE_MOBJ_WAKEUP_DIST)
	{
		set_mObject_state(mObj, ST_FIRE_BOMBER_IDLE, state_fire_bomber_aware, 0, 144);
		return;
	}
}
void state_fire_bomber_aware(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	double theta = mObj->theta;
	if(sum_square(dy, dx) >= 1)
	{
		theta = atan2(dy, dx);
		//mObj->theta = atan2(dy, dx);
	}
	pObject_seek((struct pObject*)mObj, 0.05, theta);
	mObject_move(mObj, player, map);

}

void state_fire_archer_idle(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(sum_square(dy, dx) <= HOSTILE_MOBJ_WAKEUP_DIST)
	{
		set_mObject_state(mObj, ST_FIRE_ARCHER_IDLE, state_fire_archer_aware, 0, 144);
		return;
	}
}
void state_fire_archer_aware(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(sum_square(dx, dy) <= ARCHER_INRANGE && mObj->st.timer >= mObj->st.limit / 2)
	{
		if(rand() % 2) set_mObject_state(mObj, ST_FIRE_ARCHER_DRAW, state_fire_archer_draw, 0, 64);
		else set_mObject_state(mObj, ST_FIRE_ARCHER_RITE, state_fire_archer_rite, 0, 64);
		return;
	}
	if(mObj->st.timer >= mObj->st.limit)
	{
		mObj->speed = mObj->base_speed / 12;
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, ST_FIRE_ARCHER_DASH, state_fire_archer_dash, 0, 48);
		return;
	}
	mObj->st.timer ++;
}
void state_fire_archer_dash(struct mObject* mObj, struct player* player, struct map* map)
{
	mObject_move(mObj, player, map);
	if(mObj->st.timer >= mObj->st.limit)
	{
		set_mObject_state(mObj, ST_FIRE_ARCHER_AWARE, state_fire_archer_aware, 0, 120);
		return;
	}
	mObj->st.timer ++;
}
void state_fire_archer_draw(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	mObj->theta = atan2(dy, dx);
	if(mObj->st.timer >= mObj->st.limit)
	{
		spawn_pObject(map->pObject_list, mObj->x + 0.25, mObj->y + 0.25, PO_HEX_ARROW, EAST, 40.0, atan2(dy, dx), player);
		spawn_pObject(map->pObject_list, mObj->x + 0.25, mObj->y - 0.25, PO_HEX_ARROW, EAST, 40.0, atan2(dy, dx), player);
		set_mObject_state(mObj, ST_FIRE_ARCHER_AWARE, state_fire_archer_aware, 0, 120);
		return;
	}
	mObj->st.timer ++;
}
void state_fire_archer_rite(struct mObject* mObj, struct player* player, struct map* map)
{
	if(mObj->st.timer >= mObj->st.limit)
	{
		spawn_pObject(map->pObject_list, mObj->x, mObj->y, PO_FIRE_SLING, EAST, 10.0, -1 * PI / 2, player);
		set_mObject_state(mObj, ST_FIRE_ARCHER_AWARE, state_fire_archer_aware, 0, 120);
		return;
	}
	mObj->st.timer ++;
}

//LOCAL_QUEEN /BOSS\

void state_local_queen_aware(struct mObject* mObj, struct player* player, struct map* map)
{
	if(mObj->st.timer >= mObj->st.limit)
	{
		const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
		const int choice = rand() % 3;
		if(choice == 0 && 1)
		{
			printf("BOGGED\n");
			printf("%d %d\n", mObj->sprite.x, mObj->sprite.y);
			set_mObject_state(mObj, ST_LOCAL_QUEEN_BOG, state_local_queen_bog, 0, 120);
			printf("%d %d\n", mObj->sprite.x, mObj->sprite.y);
			return;
		}
		if(choice == 1)
		{
			printf("BOLT\n");
			mObj->theta = atan2(dy, dx);
			set_mObject_state(mObj, ST_LOCAL_QUEEN_CAST, state_local_queen_cast, 0, 120);
			return;
		}
		if(choice == 2)
		{
			printf("BOLT\n");
			mObj->speed = mObj->base_speed / 20;
			set_mObject_state(mObj, ST_LOCAL_QUEEN_DASH, state_local_queen_dash, 0, 120);
			return;
		}
	}
	mObj->st.timer ++;
}
void state_local_queen_dash(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	mObject_move(mObj, player, map);
	pObject_seek(mObj, 0.015, atan2(dy, dx));
	if((mObj->st.timer + 1) % (mObj->st.limit / 4) == 0)
	{
		spawn_pObject(map->pObject_list, MIDPOINTX(mObj), MIDPOINTY(mObj), PO_SWAMP_BOLT, EAST, 10.0, get_frand(2*PI, 0.0), player);

	}
	if(mObj->st.timer >= mObj->st.limit)
	{
		set_mObject_state(mObj, ST_LOCAL_QUEEN_AWARE, state_local_queen_aware, 0, 120);
		return;
	}
	mObj->st.timer ++;
}
void state_local_queen_cast(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	mObj->theta = atan2(dy, dx);
	if((mObj->st.timer + 1) % (mObj->st.limit / 8) == 0)
	{
		spawn_pObject(map->pObject_list, MIDPOINTX(mObj), MIDPOINTY(mObj), PO_SWAMP_BOLT, EAST, 10.0, atan2(dy, dx), player);

	}
	if(mObj->st.timer ++ >= mObj->st.limit)
	{
		set_mObject_state(mObj, ST_LOCAL_QUEEN_AWARE, state_local_queen_aware, 0, 120);
		return;
	}
}

void state_local_queen_bog(struct mObject* mObj, struct player* player, struct map* map)
{
	if(mObj->st.timer ++ >= mObj->st.limit)
	{
		const double dist = 6.0;
		for(int i = 0; i < 6; i++)
		{
			spawn_pObject(map->pObject_list, mObj->x + get_frand(dist * 2, -dist), mObj->y + get_frand(dist*2, -dist), PO_SWAMP_POOL, EAST, 0.0, 0.0, player);
		}
		set_mObject_state(mObj, ST_LOCAL_QUEEN_AWARE, state_local_queen_aware, 0, 120);
	}
}
//ROCK_VORTEX /BOSS\

void state_rock_vortex_aware(struct mObject* mObj, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
	if(mObj->st.timer >= mObj->st.limit)
	{
		int choice = rand() % 2, close = rand() % 5;
		if(sum_square(dy, dx) <= VORTEX_CLOSE_CAST_RANGE && choice >= 1)
		{
			set_mObject_state(mObj, ST_ROCK_VORTEX_CAST, state_rock_vortex_cast, 0, 120);
			return;
		}
		if(choice == 0)
		{
			set_mObject_state(mObj, ST_ROCK_VORTEX_SUMMON, state_rock_vortex_summon, 0, 120);
			return;
		}
		if(choice == 1)
		{
			set_mObject_state(mObj, ST_ROCK_VORTEX_STORM, state_rock_vortex_storm, 0, 360);
			return;
		}
	}

	mObj->st.timer ++;
}
void state_rock_vortex_summon(struct mObject* mObj, struct player* player, struct map* map)
{
	if(mObj->st.timer ++ >= mObj->st.limit)
	{
		spawn_mObject(map, mObj->x + 1, mObj->y, MO_ROCK_WELL, 'w');
		spawn_mObject(map, mObj->x - 1, mObj->y, MO_ROCK_WELL, 'w');
		set_mObject_state(mObj, ST_ROCK_VORTEX_AWARE, state_rock_vortex_aware, 0, 120);
	}
	return;
}

void state_rock_vortex_storm(struct mObject* mObj, struct player* player, struct map* map)
{
	if(mObj->st.timer == 60)
	{
		set_mObject_state(mObj, ST_ROCK_VORTEX_PRESTORM, mObj->st.acp, mObj->st.timer, mObj->st.limit);
	}
	if((mObj->st.timer + 1) % (mObj->st.limit / 12) == 0 && mObj->st.timer >= 60)
	{
		const double start_theta = get_frand(-PI/2, PI/2), offset = 0.6;
		spawn_pObject(map->pObject_list, MIDPOINTX(mObj) - offset, MIDPOINTY(mObj) - offset, PO_ROCK_CAST, EAST, 20.0, start_theta, player);
		spawn_pObject(map->pObject_list, MIDPOINTX(mObj) - offset, MIDPOINTY(mObj) - offset, PO_ROCK_CAST, EAST, 20.0, start_theta + 2*PI/3, player);
		spawn_pObject(map->pObject_list, MIDPOINTX(mObj) - offset, MIDPOINTY(mObj) - offset, PO_ROCK_CAST, EAST, 20.0, start_theta + 4*PI/3, player);
	}
	if(mObj->st.timer >= mObj->st.limit)
	{
		set_mObject_state(mObj, ST_ROCK_VORTEX_AWARE, state_rock_vortex_aware, 0, 120);
		return;
	}
	mObj->st.timer ++;
}
void state_rock_vortex_cast(struct mObject* mObj, struct player* player, struct map* map)
{
	if(mObj->st.timer ++ >= mObj->st.limit)
	{
		//vortex explosion
		set_mObject_state(mObj, ST_ROCK_VORTEX_DECAST, state_rock_vortex_decast, 0, 120);
	}
	return;
}

void state_rock_vortex_decast(struct mObject* mObj, struct player* player, struct map* map)
{
	mObject_player_hitbox(mObj, player, map);	
	if(mObj->st.timer ++ >= mObj->st.limit)
	{
		set_mObject_state(mObj, ST_ROCK_VORTEX_AWARE, state_rock_vortex_aware, 0, 120);
	}
}

//GOLEM /BOSS\

void state_golem_stomp(struct mObject *mObj, struct player *player, struct map* map)
{
	if(mObj->st.timer > mObj->st.limit)
	{
		int stones = 4 + rand() % 4;
		set_mObject_state(mObj, ST_GOLEM_AWARE, state_golem_aware, 0, GOLEM_ABILITY_COOLDOWN*2.5);
		for(int i = 0; i < stones; i++)
			spawn_pObject(map->pObject_list, mObj->x - 6 + rand() % 12, mObj->y - 4 - rand() % 8, PO_GOLEM_ROCK, SOUTH, 20.0, PI/2, player);
		return;
	}
	if(mObj->st.timer ==  mObj->st.limit / 2 + 4)
	{
		set_screen_shake(map, 0.4, 96);
	}

	mObj->st.timer ++;
}

void state_golem_hit(struct mObject *mObj, struct player *player, struct map* map)
{
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, ST_GOLEM_AWARE, state_golem_aware, 0, GOLEM_ABILITY_COOLDOWN);
		return;
	}
	mObj->st.timer ++;
}

void state_golem_ready(struct mObject *mObj, struct player *player, struct map* map)
{
	if(mObj->st.timer > mObj->st.limit)
	{
		const double dx = OBJDIFF(player, mObj, 'X'), dy = OBJDIFF(player, mObj, 'Y');
		const double theta = atan2(dy, dx);
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, ST_GOLEM_AWARE, state_golem_aware, 0, GOLEM_ABILITY_COOLDOWN);
		spawn_pObject(map->pObject_list, mObj->x + 0.5*cos(theta), mObj->y + 0.5*sin(theta), PO_GOLEM_MELEE_WEAPON, EAST, 20.0, theta, player);
		return;
	}
	mObj->st.timer ++;
}

void state_golem_dash(struct mObject *mObj, struct player *player, struct map* map)
{
	mObject_move(mObj, player, map);
	if(mObj->st.timer > mObj->st.limit)
	{
		set_mObject_state(mObj, ST_GOLEM_AWARE, state_golem_aware, 0, GOLEM_ABILITY_COOLDOWN);
		return;
	}
	mObj->st.timer ++;
}

void state_golem_aware(struct mObject *mObj, struct player *player, struct map* map)
{
	//const double dx = player->x + player->width/TILE_LENGTH/2 - (mObj->x + mObj->width/TILE_LENGTH/2), dy = player->y + player->height/TILE_LENGTH/2 - (mObj->y + mObj->height/TILE_LENGTH/2);
	const double dx = OBJDIFF(player, mObj, 'X'), dy = OBJDIFF(player, mObj, 'Y');

#if 1
	mObj->st.timer ++;
	if(mObj->st.timer < mObj->st.limit)
	{
		return;
	}


	if(sum_square(dy, dx) < GOLEM_STOMP_RANGE || rand() % 6 == 0) 
	{
		set_mObject_state(mObj, ST_GOLEM_STOMP, state_golem_stomp, 0, 128);
	}
	else if(sum_square(dy, dx) < GOLEM_ATTACK_RANGE && rand() % 6 != 0)
	{
		set_mObject_state(mObj, ST_GOLEM_READY, state_golem_ready, 0, 32);
	}
	else
	{
		mObj->speed = mObj->base_speed / 24;
		mObj->theta = atan2(dy, dx);
		set_mObject_state(mObj, ST_GOLEM_DASH, state_golem_dash, 0, 96);
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
			spawn_mObject(map, mObj->x + 1, mObj->y - 1, MO_CRAWLER, 'z');
			spawn_mObject(map, mObj->x - 1, mObj->y + 1, MO_CRAWLER, 'z');
			spawn_mObject(map, mObj->x + 1, mObj->y + 1, MO_CRAWLER, 'z');
			spawn_mObject(map, mObj->x - 1, mObj->y - 1, MO_CRAWLER, 'z');
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
	if(!player->invuln && AABB(pObject, player))
	{
		player_hit(player, pObject->damage, pObject->theta);
	}
	pObject->st.timer ++;
}

void state_magic_bolt_travel(struct pObject* pObject, struct player* player, struct map* map)
{
	pObject_move(pObject, player, map);
	pObject_player_hitbox(pObject, player);
	if(pObject->st.timer >= pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, 16);
		return;
	}
	pObject->st.timer ++;
#if 0
	if(!player->invuln && AABB((struct mObject*)pObject, player))
	{
		player_hit(player, pObject->damage, pObject->theta);
	}
#endif
}

void state_swamp_pool_action(struct pObject* pObject, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, pObject), dy = OBJDIFFY(player, pObject);
	if(pObject->st.timer ++ >= pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, 16);
		player->base_speed = 1;
		return;
	}
	if(AABB(pObject, player))
	{
		player->base_speed = 0.5;
	}
	else player->base_speed = 1;
}

void state_spear_cast_action2(struct pObject* pObject, struct player* player, struct map* map)
{
	//pObject_player_hitbox(pObject, player);
	if(!player->invuln && AABB((struct mObject*)pObject, (struct mObject*)player))
	{
		player_hit(player, pObject->damage, pObject->theta);
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, 0);
	}
	if(pObject->st.timer ++ >= pObject->st.limit)
	{
		pObject->anim_limit = 4;
		pObject->anim_start_frame = 64;
		pObject->anim_frames = 4;
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, 16);
		return;
	}
}

void state_spear_cast_action(struct pObject* pObject, struct player* player, struct map* map)
{
	if(pObject->st.timer ++ >= pObject->st.limit)
	{
		pObject->anim_start_frame = 48;
		pObject->anim_frames = 1;
		set_pObject_state(pObject, pObject->st.type, state_spear_cast_action2, 0, 60 + rand() % 40);
	}
}

void state_rock_cast_action(struct pObject* pObject, struct player* player, struct map* map)
{
	pObject_move(pObject, player, map);
	pObject_player_hitbox(pObject, player);
	if(pObject->st.timer ++ >= pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, 16);
	}
}

void state_swamp_bolt(struct pObject* pObject, struct player* player, struct map* map)
{

}


void state_golem_weapon_swing(struct pObject *pObject, struct player *player,struct map *map)
{

	const double dx = player->x + player->width/TILE_LENGTH/2 - (pObject->x + pObject->width/TILE_LENGTH/2), dy = player->y + player->height/TILE_LENGTH/2 - (pObject->y + pObject->height/TILE_LENGTH/2);

	if(pObject->st.timer > pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, 0);
		return;
	}
	pObject_move(pObject, player, map);
	pObject_seek(pObject, 0.03, atan2(dy,dx));
	//ugly but cool
#if 0
	if(pObject->theta < PI/2 && pObject->theta > -1 * PI/2)
		pObject->theta += 0.025;
	else
		pObject->theta -= 0.025;
#endif

	//half seeker pObject->theta += 0.055;
	pObject->st.timer ++;
	if(!player->invuln && AABB(pObject, player))
	{
		player_hit(player, pObject->damage, pObject->theta);
	}
}

void state_golem_rock_travel(struct pObject *pObject, struct player *player, struct map *map)
{
	if(pObject->st.timer > pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, 0);
		return;
	}
	pObject_move(pObject, player, map);
	if(!player->invuln && AABB(pObject, player))
	{
		player_hit(player, pObject->damage, pObject->theta);
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, DEATHRATTLE_LIMIT);
	}
	pObject->st.timer ++;
}

void state_balista_shot(struct pObject *pObject, struct player *player, struct map *map)
{
	if(pObject->st.timer >= pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, 0);
		return;
	}
	double dx = player->x - pObject->x, dy = player->y - pObject->y;
	pObject_move(pObject, player, map);

	if(!player->invuln && AABB(pObject, player))
	{
		player_hit(player, pObject->damage, pObject->theta);
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, DEATHRATTLE_LIMIT);
	}
	pObject->st.timer ++;
}

void state_fire_sling_action(struct pObject *pObject, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, pObject), dy = OBJDIFFY(player, pObject);
	if(pObject->st.timer >= pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, 16);
		return;
	}
	pObject_player_hitbox(pObject, player);
	pObject_seek(pObject, 0.05, atan2(dy, dx));
	pObject_move(pObject, player, map);
	pObject->st.timer ++;
}

void state_lava_pool_action(struct pObject* pObject, struct player* player, struct map* map)
{
	if(pObject->st.timer >= pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, 16);
		return;
	}
	pObject_player_hitbox(pObject, player);
	pObject->st.timer ++;
}

void state_hex_arrow_action(struct pObject* pObject, struct player* player, struct map* map)
{
	const double dx = OBJDIFFX(player, pObject), dy = OBJDIFFY(player, pObject);
	if(pObject->st.timer >= pObject->st.limit)
	{
		//spawn_pObject(map->pObject_list, pObject->x, pObject->y, PO_LAVA_POOL, EAST, 25.0, 0.0, player);
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, 16);
		return;
	}
	pObject_seek(pObject, 0.01, atan2(dy, dx));
	pObject_move(pObject, player, map);
	pObject_player_hitbox(pObject, player);
	pObject->st.timer ++;
}

//player

void check_hit_abilities(struct player* player, struct map* map)
{
	struct rune* rune = (struct rune*)dynList_get(player->rune_list, 0);
	if(rune != NULL && rune->info.rune_type == RN_GRAVITY)
	{
		spawn_pObject(map->pObject_list, player->x, player->y, PO_GRAVITY_WELL, EAST, player->sword_damage, 0.0, player);
	}

	rune = (struct rune*)dynList_get(player->rune_list, 2);
	if(rune != NULL && rune->info.rune_type == RN_ROT)
	{
		for(int i = 0; i < map->mObject_list->size; i++)
		{
			struct mObject* curr = (struct mObject*)dynList_get(map->mObject_list, i);
			if(curr->killable == true)
				set_status_effect(curr, 0, 360, STATUS_ROT);
		}
	}
}


void player_knockbacked(struct player* player, struct cam* cam, struct map *map)
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
		player->global_state = ST_P_NORMAL;
		player->speed = player->base_speed / 20;
		check_hit_abilities(player, map);
		//player->invuln = false;
	}
}

void player_invuln(struct player *player, struct map* map)
{
	if(player->invuln == true)
	{
		if(player->timer >= player->invuln_limit)
		{
			player->global_state = ST_P_NORMAL;
			identify_player_sprite_location(player);
			player->invuln = false;
		}
		player->timer++;
		//printf("%d\n", player->timer);
	}

}

void set_dash_vector(struct player* player)
{
	double speed = 0.45;
	double speed2 = 0.45;
	speed = player->speed * 8;
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

			player->vel_x = norm(player->vel_x, player->vel_y);//(player->vel_x/sqrt(player->vel_x * player->vel_x + player->vel_y * player->vel_y));
			player->vel_y = norm(player->vel_y, player->vel_x);//(player->vel_y/sqrt(player->vel_x * player->vel_x + player->vel_y * player->vel_y));
	}
}

void player_dash(struct player* player, struct map* map, struct cam* cam)
{
	set_dash_vector(player);
	player_move(player, map, cam);
	if(player->dash_timer >= PLAYER_DASH_LIMIT)
	{
		player->global_state = ST_P_NORMAL;
		identify_player_sprite_location(player);
		player->speed = player->base_speed / 20;
		player->dash_timer = 0;
	}
	player->dash_timer ++;
}

void dash_control(struct player* player, const Uint8 *cks)
{
	if(cks[SDL_SCANCODE_SPACE] && player->dash_cooldown_timer >= PLAYER_DASH_COOLDOWN_LIMIT && player->global_state != ST_P_KNOCKBACK)
	{
		player->global_state = ST_P_DASH;
		identify_player_sprite_location(player);
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
	bool cond = false;
	if(player->global_state == ST_P_DASH)
		return;
	if(currentKeyStates[SDL_SCANCODE_D])
	{
		player->vel_x = player->speed;
		cond = true;
		player->dir = EAST;
		player->theta = 0;
	}
	if(currentKeyStates[SDL_SCANCODE_A])
	{
		player->vel_x = -player->speed;
		cond = true;
		player->dir = WEST;
		player->theta = PI;
	}
	if(currentKeyStates[SDL_SCANCODE_W])
	{
		player->vel_y = -player->speed;
		cond = true;
		player->dir = NORTH;
	}
	if(currentKeyStates[SDL_SCANCODE_S])
	{
		player->vel_y = player->speed;
		cond = true;
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
	if(cond)
	{
		player->anim.limit = 6;
	} else player->anim.limit = 16;
	const double tmp = player->vel_x;	
	//player->vel_x = player->vel_x != 0 || player->vel_y != 0 ? 0.075 : 0;
	//player->vel_y = player->vel_y != 0 || player->vel_y != 0 ? 0.075 : 0;
	//replace with just 1/1.414
	player->vel_x = player->vel_x != 0 || player->vel_y != 0 ? player->base_speed / 12 * (player->vel_x/sqrt(player->vel_x * player->vel_x + player->vel_y * player->vel_y)) : 0;
	player->vel_y = tmp != 0 || player->vel_y != 0 ? player->base_speed / 12 * (player->vel_y/sqrt(tmp * tmp + player->vel_y * player->vel_y)) : 0;
	//player->vel_x = player->vel_x != 0 || player->vel_y != 0 ? (player->base_speed / 5) * player->vel_y*0.707106 : 0;
	//player->vel_y = tmp != 0 || player->vel_y != 0 ? (player->base_speed / 5) * player->vel_x*0.707106 : 0;

}

void input_attack(struct player* player, struct map* map, const Uint8 *currentKeyStates)
{
	if(currentKeyStates[SDL_SCANCODE_Q] && player->attack_speed_timer >= player->attack_speed)
	{
		player->global_state = ST_P_ATTACK;
		identify_player_sprite_location(player);
		player->timer = 0;
		return;
	}
	player->attack_speed_timer ++;
}

void player_attacking(struct player* player, struct map* map, const Uint8 *currentKeyStates)
{
	if(player->timer >= PLAYER_ATTACKING_LIMIT)
	{
		player->global_state = ST_P_NORMAL;
		identify_player_sprite_location(player);
		player->timer = 0;
		return;
	}
	player->timer ++;

}

void player_deathrattle(struct player* player, struct map* map)
{
	if(player->timer >= PLAYER_DEATHRATTLE_LIMIT)
	{
		player->global_state = ST_P_GONE;
		identify_player_sprite_location(player);
		player->timer = 0;
		return;
	}
	player->timer ++;
}
void check_attack_mods(struct player* player, struct map* map, double theta)
{
	struct rune* rune = (struct rune*)dynList_get(player->rune_list, 2);
	if(rune != NULL && rune->info.rune_type == RN_BLOOD)
	{
		spawn_pObject(map->pObject_list, player->x + 0.3*cos(theta) + 0.2, player->y + 0.2*sin(theta) + 0.4, PO_BLOOD_TAX, EAST, player->sword_damage * 0.5, theta, player);
	}
}
void player_attack(struct player* player, struct map* map, const Uint8 *currentKeyStates)
{
	if(player->timer >= PLAYER_ATTACK_LIMIT) 
	{   
		player->global_state = ST_P_ATTACKING;
		//identify_player_sprite_location(player);
		int mx1, my1;
		SDL_GetMouseState(&mx1, &my1);
		double mx = (double)mx1, my = (double)my1;
		mx /= TILE_LENGTH;
		my /= TILE_LENGTH;

		const double dx = mx + (int)map->cam.offset_x - player->x;
		const double dy = my + (int)map->cam.offset_y - player->y;
		const double theta = atan2(dy, dx);

		spawn_pObject(map->pObject_list, player->x + 0.2*cos(theta) - 0.8, player->y + 0.2*sin(theta) + 0.4, PO_PLAYER_SPEAR, EAST, player->sword_damage, theta, player);
		check_attack_mods(player, map, theta);
		player->attack_speed_timer = 0;
		player->timer = 0;
		return;
	}
	player->timer++;
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
			pObject->penetration_index --;
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
			mObject_damage(target, pObject, player);
			target->hit = true;

		}
	}
}

//pObjects

void state_wraith_follow(struct pObject *pObject, struct player *player, struct map *map)
{
	double dx, dy;
	struct mObject *curr;
	bool found = false;
	//stick to a mObject maybe otherwise its gonna find [1] all the time
	if(pObject->st.timer >= pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, 16);
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
		if(sum_square(dx, dy) < 16)
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
		if(!curr->hit && getTick() % 30 == 0)
		{
			curr->theta = PI + atan2(dy, dx);
			curr->inv_frames = 0;
			mObject_damage(curr, pObject, player);
			curr->hit = true;
		}
	}
	if(sum_square(dx, dy) < 0.15 && found)
	{
		return;
	}
	if(getTick() % 30 == 0)
		pObject->theta = atan2(dy, dx);
	pObject_move(pObject, player, map);
}

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
		spawn_pObject(map->pObject_list, pObject->x, pObject->y, PO_GRAVITY_BOLT, NORTH, player->sword_damage * 0.5, angle, player);
		//spawn_pObject(map->pObject_list, pObject->x, pObject->y, PO_GRAVITY_BOLT, SOUTH, player->sword_damage * 0.5, angle + 2*PI/3, player);
		//spawn_pObject(map->pObject_list, pObject->x, pObject->y, PO_GRAVITY_BOLT, NORTH, player->sword_damage * 0.5, angle - 2*PI/3, player);
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
	if(pObject->st.timer >= pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, DEATHRATTLE_LIMIT);
		return;
	}
	check_pObject_mObject_hit(pObject, player, map);
	pObject->st.timer ++;
}

void state_player_spear_action(struct pObject *pObject, struct player* player, struct map* map)
{
	if(pObject->st.timer > pObject->st.limit)
	{
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, 0);
		return;
	}
	pObject->speed = 0.10;
	pObject_move(pObject, player, map);
	check_pObject_mObject_hit(pObject, player, map);
	pObject->st.timer ++;
}

void state_sword_shockwave(struct pObject *pObject, struct player* player, struct map* map)
{
	if(pObject->st.timer >= pObject->st.limit)
	{
		if(player->shock_counter == 0)
		{
			player->shock_counter = 6;
			player->hit_counter = 0;
			set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, 0);
			return;
		}
		spawn_pObject(map->pObject_list, pObject->x + cos(pObject->theta), pObject->y + sin(pObject->theta), PO_SWORD_SHOCKWAVE, EAST, player->sword_damage, pObject->theta, player);
		player->shock_counter --;
		set_pObject_state(pObject, ST_PO_DEAD, NULL, 0, 0);
		return;
	}
	check_pObject_mObject_hit(pObject, player, map);
	pObject->st.timer ++;
}

void check_deathrattle_abilities(struct mObject* mObject, struct player* player, struct map* map)
{
	switch(mObject->id)
	{
		case 'a':
			spawn_mObject(map, mObject->x, mObject->y, MO_FIRE_BOMBER, 's');
			break;
		case '8':
			spawn_mObject(map, mObject->x + 1, mObject->y + 0.75, MO_DRIDER_FIGHTER, '9');
			break;
		case 's':
			spawn_pObject(map->pObject_list, mObject->x - 0.0, mObject->y - 0.0, PO_LAVA_POOL, EAST, 10.0, 0.0, player);
			spawn_pObject(map->pObject_list, mObject->x, mObject->y, PO_FIRE_SLING, EAST, 20.0, 4*PI / 3, player);
			spawn_pObject(map->pObject_list, mObject->x, mObject->y, PO_FIRE_SLING, EAST, 20.0, 2*PI / 3, player);
			spawn_pObject(map->pObject_list, mObject->x, mObject->y, PO_FIRE_SLING, EAST, 20.0, 6*PI / 3, player);
			spawn_pObject(map->pObject_list, mObject->x, mObject->y, PO_FIRE_SLING, EAST, 20.0, 4*PI / 3, player);
			break;
	}
}

void state_deathrattle(struct mObject *mObject, struct player *player, struct map *map)
{
	if(mObject->st.timer >= mObject->st.limit)
	{
		check_deathrattle_abilities(mObject, player, map);

		set_mObject_state(mObject, ST_CLEAR, state_deathrattle, 0, 60);
		if(mObject->killable == false)
			return;
		struct rune* rune = (struct rune*)dynList_get(player->rune_list, 0);
		if(rune != NULL && rune->info.rune_type == RN_UNHOLY)
			rune->attribute ++;

		rune = (struct rune*)dynList_get(player->rune_list, 2);
		if(rune != NULL && rune->info.rune_type == RN_FROST)
		{
			spawn_pObject(map->pObject_list, mObject->x - 1, mObject->y - 1, PO_FROST_STORM, EAST, 0.0, 0.0, player);
		}

		rune = (struct rune*)dynList_get(player->rune_list, 2);
		if(rune != NULL && rune->info.rune_type == RN_ROT)
			rune->attribute ++;

		rune = (struct rune*)dynList_get(player->rune_list, 2);
		if(rune != NULL && rune->info.rune_type == RN_UNHOLY)
			player->sword_damage += 3;

		rune = (struct rune*)dynList_get(player->rune_list, 3);
		if(rune != NULL && rune->info.rune_type == RN_BLOOD)
		{
			//change list to map for fun results (C feature)
			for(int i = 0; i < 3; i++)
				spawn_pObject(map->pObject_list, MIDPOINTX(mObject), MIDPOINTY(mObject), PO_BLOOD_TAX, EAST, player->sword_damage, get_frand(-PI/2, PI/2), player);
		}
	}
	mObject->st.timer ++;
}

void state_enemy_default(struct mObject *mObject, struct player* player, struct map *map)
{

	if(mObject->st.type == ST_DEATHRATTLE)
	{
		if(mObject->st.timer > mObject->st.limit)
		{
			mObject->st.type = ST_CLEAR;
			return;
		}
		//mObject->st.timer++;
		return;
	}

	if(!mObject->hittable || mObject->st.type == ST_DEATHRATTLE)
		return;
	identify_status_effect(mObject, player);
	if(mObject->st.type == ST_ENEMYDEAD)
	{
		if(mObject->killable)
			map->aggresive_mObj_count --;
		set_mObject_state(mObject, ST_DEATHRATTLE, state_deathrattle, 0, 64);
		return;
	}
	if(mObject->inv_frames >= 24)
	{
		mObject->hit = false;
	}
	mObject->inv_frames++;
	if(mObject->health <= 0)
	{
		mObject->st.type = ST_ENEMYDEAD;
	}
}
