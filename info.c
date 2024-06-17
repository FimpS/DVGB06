#include "objects.h"
#include "global.h"
#include "map.h"
#include "event.h"
#include "object_state.h"
#include "runes.h"
#include "gfx.h"
#include "info.h"

struct mObj_state init_mObject_state(void (*acp)(struct mObject*,
			struct player*,
			struct map*),
		int timer,
		int limit,
		void (*kcp)(struct mObject*,
			struct player*,
			struct map*)
		)
{
	struct mObj_state new = {0};
	new.type = st_spawn;
	new.acp = acp;
	new.timer = timer;
	new.limit = limit;
	new.kcp = kcp;

	return new;
}

struct render_info init_render_info(int s, int ti, int f, int t, int l)
{
	struct render_info new = {0};
	new.start_frame = s;
	new.tile_length= ti;
	new.frames = f;
	new.timer = t;
	new.limit = l;
	return new;
}

struct pObj_state init_pObject_state(void (*acp)(struct pObject*,
			struct player*,
			struct map*),
		int timer,
		int limit)
{
	struct pObj_state new = {0};
	new.type = ST_PO_REAL;
	new.acp = acp;
	new.timer = timer;
	new.limit = limit;

	return new;
}
struct status_effect init_status_effect()
{
	struct status_effect new = {0};
	new.timer = 0;
	new.limit = 0;
	new.type = status_none;
	return new;
}
void init_pObject(struct pObject *pObject, double x, double y, card_dir dir, double dmg, double theta, struct player *player)
{
	pObject->x = x;
	pObject->y = y;
	pObject->vel_x = 0;
	pObject->vel_y = 0;
	pObject->theta = theta;
	pObject->anim_timer = 0;
	pObject->knockbacker = true;
	pObject->knockkoef = 1;
	pObject->dir = dir;

	pObject->anim_tile_length = 16;
	pObject->anim_frames = 4;
	pObject->anim_limit = 4;
	pObject->anim_start_frame = 0;

	pObject->status_effect = player->sword_effect_type;
	switch(pObject->type)
	{
		case fire:
			pObject->width = TILE_LENGTH / 2;
			pObject->height = TILE_LENGTH / 2;
			pObject->speed = 0.3;
			pObject->damage = dmg;
			pObject->transp = false;
			pObject->penetration_index = 2;
			pObject->pen_wall = true;
			pObject->sprite = init_sprite(0, 0, 16, 16);
			break;
		case balista_bolt:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH/2;
			pObject->speed = 0.1;
			pObject->damage = dmg;
			pObject->transp = false;
			pObject->penetration_index = 1;
			pObject->pen_wall = true;
			pObject->status_effect = status_none;
			pObject->anim_frames = 1;
			pObject->sprite = init_sprite(0, 16, 16, 16);
			pObject->st = init_pObject_state(state_balista_shot, 0, 1200);
			break;
		case wraith_big:
		case wraith:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH;

			pObject->speed = 0.25;
			pObject->damage = dmg;
			pObject->transp = false;
			pObject->penetration_index = 1;
			pObject->pen_wall = false;
			pObject->sprite = init_sprite(0, 32, 16, 16);
			pObject->st = init_pObject_state(state_wraith_follow, 0, player->health * 2.4);
			if(pObject->type == wraith_big)
			{
				pObject->st.limit = 2 << 21;
			}
			break;
		case frost_storm:
			pObject->width = TILE_LENGTH * 3;
			pObject->height = TILE_LENGTH * 3;
			pObject->penetration_index = 0;
			pObject->transp = false;
			pObject->pen_wall = false;
			pObject->knockbacker = false;
			pObject->status_effect = status_frostbite;
			pObject->sprite = init_sprite(0, 48, 16, 16);
			pObject->st = init_pObject_state(state_rot_smog_flower, 0, 240);
			break;
		case rot_smog:
			pObject->width = TILE_LENGTH * 3;
			pObject->height = TILE_LENGTH * 3;
			pObject->damage = dmg;
			pObject->penetration_index = 100;
			pObject->transp = false;
			pObject->pen_wall = false;
			pObject->knockbacker = false;
			pObject->status_effect = status_none;
			pObject->sprite = init_sprite(0, 64, 16, 16);
			pObject->st = init_pObject_state(state_rot_smog_flower, 0, 240);
			break;
		case gravity_well:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH;
			pObject->speed = 0.05;
			pObject->damage = 0;
			pObject->transp = true;
			pObject->pen_wall = true;
			pObject->sprite = init_sprite(0, 80, 16, 16);
			pObject->st = init_pObject_state(state_gravity_well_travel, 0, 300);
			break;
		case gravity_bolt:
			pObject->width = TILE_LENGTH / 1.5;
			pObject->height = TILE_LENGTH / 1.5;
			pObject->speed = 0.05; //prev 0.05
			pObject->damage = dmg;
			pObject->transp = true;
			pObject->pen_wall = true;
			pObject->thetaacc = 0;
			pObject->sprite = init_sprite(0, 96, 16, 16);
			pObject->st = init_pObject_state(state_gravity_bolt_travel, 0, 120);
			break;
		case blood_tax:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH;
			pObject->speed = 0.25;
			pObject->damage = dmg;
			pObject->transp = true;
			pObject->penetration_index = 1;
			pObject->pen_wall = true;
			pObject->sprite = init_sprite(0, 0, 16, 16);
			pObject->st = init_pObject_state(state_blood_tax, 0, 48);
			break;
		case brimstone:
			pObject->dir = dir;
			switch(pObject->dir)
			{
				case EAST:
					pObject->x = x;
					pObject->y = y;
					pObject->width = TILE_LENGTH * 12;
					pObject->height = TILE_LENGTH * 2;
					break;
				case WEST:
					pObject->width = TILE_LENGTH * 12;
					pObject->height = TILE_LENGTH * 2;
					pObject->x = x - pObject->width;
					pObject->y = y;
					break;
				case SOUTH:
					pObject->width = TILE_LENGTH * 2;
					pObject->height = TILE_LENGTH * 12;
					pObject->x = x;
					pObject->y = y;
					break;
				case NORTH:
					pObject->width = TILE_LENGTH * 2;
					pObject->height = TILE_LENGTH * 12;
					pObject->x = x;
					pObject->y = y - pObject->height;
					break;
			}
			pObject->speed = 0;
			pObject->damage = dmg;
			pObject->transp = false;
			pObject->pen_wall = false;
			pObject->penetration_index = 0;
			pObject->knockbacker = false;
			pObject->sprite = init_sprite(0, 128, 16, 16);
			pObject->st = init_pObject_state(state_brimstone_beam, 0, 48);
			break;
		case PO_SWORDSMAN_SWORD:
			pObject->width = TILE_LENGTH * 2;
			pObject->height = TILE_LENGTH;
			pObject->speed = 0;
			pObject->damage = dmg;
			pObject->transp = false;
			pObject->pen_wall = false;
			pObject->status_effect = status_none;
			pObject->sprite = init_sprite(0, 192, 32, 16);
			pObject->st = init_pObject_state(state_swordsman_sword_swing, 0, 16);
			pObject->anim_tile_length = 32;
			pObject->anim_frames = 1;
			break;
		case PO_GOLEM_ROCK:
			pObject->speed = get_frand(0.1, 0.05);
			int fuck_you = rand() % 20;
			pObject->width = TILE_LENGTH + fuck_you;
			pObject->height = TILE_LENGTH + fuck_you;
			pObject->damage = dmg;
			pObject->transp = false;
			pObject->pen_wall = false;
			pObject->status_effect = status_none;
			pObject->sprite = init_sprite(0, 208, 16, 16);
			pObject->st = init_pObject_state(state_golem_rock_travel, 0, 128);
			pObject->anim_tile_length = 16;
			pObject->anim_frames = 4;
			break;
		case PO_GOLEM_MELEE_WEAPON:
			pObject->width = TILE_LENGTH * 5/2;
			pObject->height = TILE_LENGTH * 5/2;
			pObject->speed = 0.15;
			pObject->damage = dmg;
			pObject->transp = false;
			pObject->pen_wall = false;
			pObject->status_effect = status_none;
			pObject->sprite = init_sprite(0, 224, 16, 16);
			pObject->st = init_pObject_state(state_golem_weapon_swing, 0, 128);
			pObject->anim_tile_length = 16;
			pObject->anim_frames = 4;
			pObject->anim_limit = 4;
			break;
		case PO_MAGIC_BOLT:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH;
			pObject->speed = 0.2;
			pObject->damage = dmg;
			pObject->transp = false;
			pObject->pen_wall = false;
			pObject->status_effect = status_none;
			pObject->sprite = init_sprite(0, 176, 16, 16);
			pObject->st = init_pObject_state(state_magic_bolt_travel, 0, 48);
			break;
		case sword:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH;
			pObject->speed = 0.3;
			pObject->damage = dmg;
			pObject->transp = true;
			pObject->penetration_index = 100;
			pObject->pen_wall = false;
			pObject->knockkoef = player->pObject_knockkoef;
			pObject->anim_limit = 4;
			pObject->sprite = init_sprite(0, 160, 16, 16);
			pObject->st = init_pObject_state(state_sword_swing, 0, 16);
			break;
		case PO_PLAYER_SPEAR:
			pObject->width = TILE_LENGTH * 3;
			pObject->height = TILE_LENGTH;
			pObject->speed = 0;
			pObject->damage = dmg;
			pObject->transp = false;
			pObject->pen_wall = false;
			pObject->sprite = init_sprite(0, 48, 48, 16);
			pObject->st = init_pObject_state(state_player_spear_action, 0, 8);
			pObject->anim_tile_length = 32;
			pObject->anim_frames = 1;
			break;
		case PO_SWORD_SHOCKWAVE:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH;
			pObject->speed = 0.15;
			pObject->damage = dmg;
			pObject->transp = true;
			pObject->penetration_index = 100;
			pObject->pen_wall = false;
			pObject->knockkoef = player->pObject_knockkoef;
			pObject->anim_limit = 2;
			pObject->sprite = init_sprite(0, 160, 16, 16);
			pObject->st = init_pObject_state(state_sword_shockwave, 0, 8);
			break;
		case placeholder:
			break;
	}
}
void initPlayer(struct player *player, int width, int height)
{ 
	player->x = 2;
	player->y = 2;
	player->vel_x = 0;
	player->vel_y = 0;
	player->width = TILE_LENGTH * 1;
	player->height = TILE_LENGTH * 6/4;
	player->base_speed = 1;
	player->speed = player->base_speed / 10;
	player->theta = 0;
	player->maxhealth = 100;
	player->health = player->maxhealth;
	player->global_state = ST_P_NORMAL;
	player->invuln = false;
	player->cd = 0;
	player->dash_timer = 0;
	player->dash_cooldown_timer = 0;
	player->timer = 0;
	player->invuln_limit = 0;
	player->attack_speed = 32;
	player->attack_speed_timer = 0;
	player->change_map = false;
	player->shock_counter = 6;
	player->anim1counter = 0;
	player->sword_damage = 25;
	player->pObject_knockkoef = 1;
	player->kills = 0;
	player->rune_list = dynList_create();
	player->anim = init_render_info(0, 16, 4, 0, 16);
	player->sprite = init_sprite(0, 0, 16, 24);
	player->sword_effect_type = status_none;
}
void identify_player_sprite_location(struct player* player)
{
	switch(player->global_state)
	{
		case ST_P_NORMAL:
			player->sprite = init_sprite(0, 0, 16, 24);
			player->anim = init_render_info(0, 16, 4, 0, 16);
			break;

		case ST_P_ATTACK:
			player->sprite = init_sprite(0, 24, 16, 24);
			player->anim = init_render_info(0, 16, 1, 0, (PLAYER_ATTACK_LIMIT + PLAYER_ATTACKING_LIMIT) / 4);
			break;
		case ST_P_DASH:
			player->sprite = init_sprite(96, 0, 16, 24);
			player->anim = init_render_info(0, 16, 1, 0, (PLAYER_ATTACK_LIMIT + PLAYER_ATTACKING_LIMIT) / 4);
			break;
	}
}

SDL_Rect identify_rune_sprite(struct rune_info ri)
{
	SDL_Rect new = {0};
	switch(ri.rune_type)
	{
		case unholy:
			new = init_sprite(0, 320, 32, 32);
			break;
		case holy:
			new = init_sprite(0, 352, 32, 32);
			break;
		case rot:
			new = init_sprite(0, 384, 32, 32);
			break;
		case blood:
			new = init_sprite(0, 416, 32, 32);
			break;
		case gravity:
			new = init_sprite(0, 448, 32, 32);
			break;
		case frost:
			new = init_sprite(0, 480, 32, 32);
			break;
	}

	return new;
}

void init_mObject(struct mObject *mObject, int x, int y, struct map *map)
{
	mObject->x = x;
	mObject->y = y;
	mObject->vel_x = 0;
	mObject->vel_y = 0;
	mObject->inv_frames = 0;
	mObject->base_speed = 1;
	mObject->theta = PI;
	mObject->type_reg = ST_MAGUS_IDLE;
	mObject->status_effect = init_status_effect();
	mObject->anim.rotatable = false;
	mObject->hyperarmor = false;
	switch(mObject->type)
	{
		case runner:
			mObject->x = x;
			mObject->y = y;
			mObject->vel_x = 0;
			mObject->vel_y = 0;
			mObject->health = 100;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH;
			mObject->speed = 0.01;
			mObject->theta = ((double)rand()/(double)(RAND_MAX)) * 3.1415 * 2;
			mObject->hit = false;
			mObject->st.type = st_spawn;
			mObject->st.timer = 0;
			mObject->st.limit = 120;
			mObject->inv_frames = 0;
			mObject->mass = 80;
			mObject->wall_collide = false;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->st.acp = NULL;
			mObject->contact_damage = 20;
			mObject->anim = init_render_info(0, TILE_LENGTH, 4, 0, 4);
			mObject->st = init_mObject_state(state_crawler_idle, 0, 40, NULL);
			break;
		case crawler:
			mObject->health = 100;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH;
			mObject->speed = mObject->base_speed / 6.67;
			mObject->mass = 30;
			mObject->wall_collide = false;
			mObject->hit = false;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->contact_damage = 22;
			mObject->sprite = init_sprite(0, 96, 16, 16);
			mObject->anim = init_render_info(0, 16, 4, 0, 8);
			mObject->type_reg = ST_CRAWLER_IDLE;
			mObject->st = init_mObject_state(state_crawler_idle, 0, 64, state_crawler_idle);
			break;
		case rusher:
			mObject->health = 100;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH;
			mObject->speed = mObject->base_speed / 50;
			mObject->hit = false;
			mObject->mass = 30;
			mObject->wall_collide = false;
			mObject->contact_damage = 20;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->anim = init_render_info(64, TILE_LENGTH, 16, 0, 4);
			mObject->st = init_mObject_state(state_rusher_idle, 0, 40, state_rusher_idle);
			if(mObject->id == 'g')
			{
				mObject->width = TILE_LENGTH * 2;
				mObject->height = TILE_LENGTH * 2;
				mObject->mass = 90;
				mObject->health = 500;
			}
			break;
		case balista:
			//TODO weird stuff with balista and blood_tax maybe target_hit problem
			//mObject->base_speed = 0;
			mObject->speed = mObject->base_speed;
			mObject->health = 500;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH;
			mObject->theta = PI/2;
			mObject->hit = false;
			mObject->mass = 999;
			mObject->wall_collide = false;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->sprite = init_sprite(0, 208, 16, 16);
			mObject->anim = init_render_info(0, 16, 1, 0, 1600);
			mObject->type_reg = ST_BALISTA_IDLE;
			mObject->st = init_mObject_state(state_balista_idle, 0, 40, state_balista_idle);
			mObject->anim.rotatable = true;
			break;
		case MO_ARCHER:
			mObject->speed = mObject->base_speed;
			mObject->health = 150;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH * 3/2;
			mObject->hit = false;
			mObject->mass = 50;
			mObject->wall_collide = false;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->sprite = init_sprite(0, 160, 16, 24);
			mObject->anim = init_render_info(0, 16, 4, 0, 16);
			mObject->type_reg = ST_ARCHER_AWARE;
			mObject->st = init_mObject_state(state_archer_idle, 0, 40, state_archer_aware);
			break;
		case MO_SWORDSMAN:
			mObject->speed = mObject->base_speed;
			mObject->health = 250;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH * 1.5;
			mObject->hit = false;
			mObject->mass = 40;
			mObject->wall_collide = false;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->sprite = init_sprite(0, 112, 16, 24);
			mObject->anim = init_render_info(0, 16, 4, 0, 32);
			mObject->type_reg = ST_SWORDSMAN_AWARE;
			mObject->st = init_mObject_state(state_swordsman_idle, 0, 40, state_swordsman_aware);
			break;		
		case MO_MAGUS:
			mObject->speed = mObject->base_speed;
			mObject->health = 75;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH * 1.5;
			mObject->hit = false;
			mObject->mass = 25;
			mObject->wall_collide = false;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->sprite = init_sprite(0, 0, 16, 24);
			mObject->anim = init_render_info(0, 16, 4, 0, 16);
			mObject->type_reg = ST_MAGUS_AWARE;
			mObject->st = init_mObject_state(state_magus_idle, 0, 40, state_magus_aware);
			break;
		case summoner:
			mObject->speed = mObject->base_speed / 20;
			mObject->health = 200;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH * 1.5;
			mObject->hit = false;
			mObject->mass = 30;
			mObject->wall_collide = false;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->anim = init_render_info(0, 16, 4, 0, 16);
			mObject->sprite = init_sprite(0, 48, 16, 24);
			mObject->type_reg = ST_SUMMONER_IDLE;
			mObject->st = init_mObject_state(state_summoner_idle, 0, 40, state_summoner_idle);
			break;
		case MO_GOLEM:
			mObject->speed = mObject->base_speed / 20;
			mObject->health = 2000;
			mObject->width = TILE_LENGTH * 4;
			mObject->height = TILE_LENGTH * 4;
			mObject->hit = false;
			mObject->mass = 999;
			mObject->wall_collide = false;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->anim = init_render_info(0, 32, 1, 0, 128);
			mObject->sprite = init_sprite(0, 566, 32, 27);
			mObject->type_reg = 0;
			mObject->hyperarmor = true;
			mObject->st = init_mObject_state(state_golem_aware, 0, 48, NULL);
			break;
		case MO_CULTIST_CHIEFTAIN:
			mObject->speed = mObject->base_speed / 20;
			mObject->health = 200;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH * 4/2;
			mObject->hit = false;
			mObject->mass = 999;
			mObject->wall_collide = false;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->atts.cheiftain_ticker = 0;
			mObject->anim = init_render_info(0, 16, 4, 0, 8);
			mObject->sprite = init_sprite(0, 224, 16, 24);
			mObject->type_reg = 0;
			mObject->hyperarmor = true;
			mObject->st = init_mObject_state(state_chieftain_aware, 0, 48, NULL);
			break;
		case interactable:
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH;
			mObject->hittable = false;
			mObject->st.type = st_placeholder;
			mObject->killable = false;
			mObject->st.acp = tp_player_interaction;
			break;
		case rune_shard:
			mObject->width = TILE_LENGTH * 2;
			mObject->height = TILE_LENGTH * 2;
			mObject->hittable = false;
			mObject->killable = false;
			mObject->st.type = st_placeholder;
			mObject->r_info = get_rand_rune_info(map);
			mObject->anim = init_render_info(0, 32, 4, 0, 12);
			mObject->st = init_mObject_state(rune_player_interaction, 0, 0, NULL);
			mObject->sprite = identify_rune_sprite(mObject->r_info);
			mObject->st.acp = rune_player_interaction;
			break;
	}
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

void identify_mObject_sprite_location(struct mObject *mObject)
{
	mObject->anim.timer = 0;
	mObject->anim.frames = 4;
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
		case ST_GOLEM_AWARE:
			mObject->sprite.x = 0;
			mObject->sprite.y = 512;
			mObject->anim.limit = 16;
			mObject->anim.start_frame = 0;
			break;
		case ST_GOLEM_DASH:
			mObject->sprite.x = 128;
			mObject->sprite.y = 512;
			mObject->anim.limit = 16;
			mObject->anim.start_frame = 128;
			break;
		case ST_GOLEM_STOMP:
			mObject->sprite.x = 0;
			mObject->sprite.y = 539;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 0;
			break;
		case ST_GOLEM_READY:
			mObject->sprite.x = 128;
			mObject->sprite.y = 539;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 128;
			break;
		case ST_GOLEM_HIT:
			mObject->sprite.x = 0;
			mObject->sprite.y = 512;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 0;
			break;
		case ST_GOLEM_BUILD:
			mObject->sprite.x = 0;
			mObject->sprite.y = 566;
			//why does this happen???
			mObject->anim.limit = mObject->st.limit / 8 + 4;
			mObject->anim.start_frame = 0;
			mObject->anim.frames = 8;
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
