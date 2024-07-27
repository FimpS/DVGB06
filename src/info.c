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
	new.type = STATUS_NONE;
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
	pObject->penetration_index = 1;

	pObject->anim_tile_length = 16;
	pObject->anim_frames = 4;
	pObject->anim_limit = 4;
	pObject->anim_start_frame = 0;
	pObject->status_effect = STATUS_NONE;

	switch(pObject->type)
	{
		case PO_BALISTA_BOLT:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH/2;
			pObject->speed = 0.1;
			pObject->damage = dmg;
			pObject->transp = false;
			pObject->penetration_index = 1;
			pObject->anim_frames = 1;
			pObject->sprite = init_sprite(0, 16, 16, 16);
			pObject->st = init_pObject_state(state_balista_shot, 0, 1200);
			break;
		case PO_BIG_WRAITH:
		case PO_WRAITH:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH;

			pObject->speed = 0.2;
			pObject->damage = dmg;
			pObject->transp = false;
			pObject->penetration_index = 1;
			
			pObject->sprite = init_sprite(0, 32, 16, 16);
			pObject->st = init_pObject_state(state_wraith_follow, 0, player->maxhealth * 0.4);
			if(pObject->type == PO_BIG_WRAITH)
			{
				pObject->speed = 0.15;
				pObject->st.limit = 2 << 21;
			}
			break;
		case PO_FROST_STORM:
			pObject->width = TILE_LENGTH * 3;
			pObject->height = TILE_LENGTH * 3;
			pObject->penetration_index = 0;
			pObject->damage = dmg;
			pObject->transp = false;
			
			pObject->knockbacker = false;
			pObject->status_effect = STATUS_FROSTBITE;
			pObject->sprite = init_sprite(0, 128, 16, 16);
			pObject->st = init_pObject_state(state_rot_smog_flower, 0, 128);
			break;
		case PO_GRAVITY_WELL:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH;
			pObject->speed = 0.05;
			pObject->damage = 0;
			pObject->transp = true;
			pObject->sprite = init_sprite(0, 80, 16, 16);
			pObject->st = init_pObject_state(state_gravity_well_travel, 0, 300);
			break;
		case PO_GRAVITY_BOLT:
			pObject->width = TILE_LENGTH / 1.5;
			pObject->height = TILE_LENGTH / 1.5;
			pObject->speed = 0.05; //prev 0.05
			pObject->damage = dmg;
			pObject->transp = true;
			pObject->thetaacc = 0;
			pObject->sprite = init_sprite(0, 96, 16, 16);
			pObject->st = init_pObject_state(state_gravity_bolt_travel, 0, 120);
			break;
		case PO_BLOOD_TAX:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH;
			pObject->speed = 0.25;
			pObject->damage = dmg;
			pObject->transp = true;
			pObject->penetration_index = 1;
			pObject->sprite = init_sprite(0, 0, 16, 16);
			pObject->st = init_pObject_state(state_blood_tax, 0, 48);
			break;
		case PO_SWAMP_POOL:
			pObject->width = TILE_LENGTH * 2;
			pObject->height = TILE_LENGTH * 2;
			pObject->speed = 0;
			pObject->knockbacker = false;
			pObject->damage = dmg;
			pObject->transp = true;
			pObject->status_effect = STATUS_BOGGED;
			pObject->sprite = init_sprite(0, 288, 16, 16);
			pObject->st = init_pObject_state(state_swamp_pool_action, 0, 640);
			pObject->anim_tile_length = 16;
			pObject->anim_frames = 4;
			break;
		case PO_SPEAR_CAST:
			pObject->width = TILE_LENGTH * 1;
			pObject->height = TILE_LENGTH * 1;
			pObject->speed = 0;
			pObject->knockbacker = false;
			pObject->damage = dmg;
			pObject->transp = true;
			pObject->sprite = init_sprite(0, 304, 16, 16);
			int tmp = rand() % 16 + 32;
			pObject->st = init_pObject_state(state_spear_cast_action, 0, tmp);
			pObject->anim_limit = tmp / 4 + 1;
			pObject->anim_tile_length = 16;
			pObject->anim_frames = 4;
			break;
		case PO_ROCK_CAST:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH;
			pObject->speed = 0.15;
			pObject->damage = dmg;
			pObject->transp = true;
			pObject->sprite = init_sprite(0, 320, 16, 16);
			pObject->st = init_pObject_state(state_rock_cast_action, 0, 128);
			pObject->anim_tile_length = 16;
			pObject->anim_frames = 4;
			break;
		case PO_SWAMP_BOLT:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH;
			pObject->speed = 0.15;
			pObject->damage = dmg;
			pObject->transp = true;
			pObject->sprite = init_sprite(0, 336, 16, 16);
			pObject->st = init_pObject_state(state_magic_bolt_travel, 0, 128);
			pObject->anim_tile_length = 16;
			pObject->anim_frames = 4;
			break;
		case PO_SWORDSMAN_SWORD:
			pObject->width = TILE_LENGTH * 2;
			pObject->height = TILE_LENGTH;
			pObject->speed = 0;
			pObject->damage = dmg;
			pObject->transp = false;
			
			pObject->sprite = init_sprite(0, 192, 32, 16);
			pObject->st = init_pObject_state(state_swordsman_sword_swing, 0, 16);
			pObject->anim_tile_length = 32;
			pObject->anim_frames = 1;
			break;
		case PO_FIRE_SLING:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH;
			pObject->speed = 0.15;
			pObject->damage = dmg;
			pObject->transp = true;
			pObject->status_effect = STATUS_BURN;	
			pObject->sprite = init_sprite(0, 240, 16, 16);
			pObject->st = init_pObject_state(state_fire_sling_action, 0, 64);
			pObject->anim_tile_length = 16;
			pObject->anim_frames = 4;
			break;
		case PO_LAVA_POOL:
			pObject->width = TILE_LENGTH * 1;
			pObject->height = TILE_LENGTH * 1;
			pObject->speed = 0.0;
			pObject->damage = dmg;
			pObject->transp = false;
			pObject->sprite = init_sprite(0, 256, 16, 16);
			pObject->st = init_pObject_state(state_lava_pool_action, 0, 256);
			pObject->anim_tile_length = 16;
			pObject->anim_frames = 4;
			break;
		case PO_HEX_ARROW:
			pObject->width = TILE_LENGTH;
			pObject->height = TILE_LENGTH / 2;
			pObject->speed = 0.15;
			pObject->damage = dmg;
			pObject->transp = false;
			pObject->penetration_index = 1;
			pObject->status_effect = STATUS_HEX;	
			pObject->sprite = init_sprite(0, 272, 16, 16);
			pObject->st = init_pObject_state(state_hex_arrow_action, 0, 16);
			pObject->anim_tile_length = 16;
			pObject->anim_frames = 4;
			break;
		case PO_GOLEM_ROCK:
			pObject->speed = get_frand(0.1, 0.05);
			int fuck_you = rand() % 20;
			pObject->width = TILE_LENGTH + fuck_you;
			pObject->height = TILE_LENGTH + fuck_you;
			pObject->damage = dmg;
			pObject->transp = false;
			
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
			pObject->transp = true;
			pObject->sprite = init_sprite(0, 176, 16, 16);
			pObject->st = init_pObject_state(state_magic_bolt_travel, 0, 48);
			break;
		case PO_PLAYER_SPEAR:
			pObject->width = TILE_LENGTH * 3;
			pObject->height = TILE_LENGTH * 1;
			pObject->speed = 0;
			pObject->damage = dmg;
			pObject->transp = true;
			pObject->status_effect = player->sword_effect_type;
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
	player->base_speed = 1.0;
	player->speed = player->base_speed / 20;
	player->theta = 0;
	player->maxhealth = 1000;
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
	player->sword_damage = 600;
	player->pObject_knockkoef = 1;
	player->kills = 0;
	player->se_list = dynList_create();
	player->rune_list = dynList_create();
	player->anim = init_render_info(0, 16, 4, 0, 16);
	player->sprite = init_sprite(0, 0, 16, 24);
	player->sword_effect_type = STATUS_NONE;
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
		case ST_P_DEAD:
			player->sprite = init_sprite(64, 24, 16, 24);
			player->anim = init_render_info(64, 16, 4, 0, 16);
			break;
		case ST_P_GONE:
			player->sprite = init_sprite(0, 48, 16, 24);
			player->anim = init_render_info(0, 16, 4, 0, 16);
			break;
		default:
			break;
	}
}

SDL_Rect identify_rune_sprite(struct rune_info ri)
{
	SDL_Rect new = {0};
	switch(ri.rune_type)
	{
		case RN_UNHOLY:
			new = init_sprite(0, 320, 32, 32);
			break;
		case RN_HOLY:
			new = init_sprite(0, 352, 32, 32);
			break;
		case RN_ROT:
			new = init_sprite(0, 384, 32, 32);
			break;
		case RN_BLOOD:
			new = init_sprite(0, 416, 32, 32);
			break;
		case RN_GRAVITY:
			new = init_sprite(0, 448, 32, 32);
			break;
		case RN_FROST:
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
	mObject->transp = false;
	mObject->anim.rotatable = false;
	mObject->hyperarmor = false;
	mObject->max_health = 1;
	switch(mObject->type)
	{
		case MO_RUNNER:
			mObject->x = x;
			mObject->y = y;
			mObject->vel_x = 0;
			mObject->vel_y = 0;
			mObject->max_health = 100;
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
		case MO_CRAWLER:
			mObject->max_health = 100;
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
		case MO_RUSHER:
			mObject->max_health = 100;
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
				mObject->max_health = 500;
			}
			break;
		case MO_BALISTA:
			//TODO weird stuff with balista and blood_tax maybe target_hit problem
			//mObject->base_speed = 0;
			mObject->speed = mObject->base_speed;
			mObject->max_health = 500;
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
		case MO_PEAK_LOCAL:
			mObject->speed = mObject->base_speed / 24;
			mObject->max_health = 150;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH * 3/2;
			mObject->hit = false;
			mObject->mass = 50;
			mObject->wall_collide = false;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->sprite = init_sprite(256, 320, 16, 24);
			mObject->anim = init_render_info(256, 16, 4, 0, 16);
			mObject->type_reg = ST_PEAK_LOCAL_AWARE;
			mObject->st = init_mObject_state(state_peak_local_idle, 0, 40, state_peak_local_aware);
			break;
		case MO_PEAK_KNIGHT:
			mObject->speed = mObject->base_speed / 16;
			mObject->max_health = 150;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH * 1.5;
			mObject->hit = false;
			mObject->mass = 40;
			mObject->wall_collide = false;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->hyperarmor = false;
			mObject->sprite = init_sprite(256, 368, 16, 24);
			mObject->anim = init_render_info(256, 16, 4, 0, 16);
			mObject->type_reg = ST_PEAK_KNIGHT_AWARE;
			mObject->st = init_mObject_state(state_peak_knight_idle, 0, 40, state_peak_knight_aware);
			break;
		case MO_ROCK_WELL:
			mObject->speed = mObject->base_speed / 24;
			mObject->max_health = 150;
			mObject->width = TILE_LENGTH * 1.5;
			mObject->height = TILE_LENGTH * 1.5;
			mObject->hit = false;
			mObject->mass = 60;
			mObject->transp = true;
			mObject->wall_collide = false;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->hyperarmor = false;
			mObject->sprite = init_sprite(256, 416, 16, 16);
			mObject->anim = init_render_info(256, 16, 4, 0, 8);
			mObject->type_reg = ST_ROCK_WELL_AWARE;
			mObject->st = init_mObject_state(state_rock_well_idle, 0, 40, state_rock_well_aware);
			break;
		case MO_ROCK_ROLLER:
			mObject->speed = mObject->base_speed / 24;
			mObject->max_health = 150;
			mObject->width = TILE_LENGTH * 1.5;
			mObject->height = TILE_LENGTH * 1.5;
			mObject->hit = false;
			mObject->mass = 60;
			mObject->wall_collide = false;
			mObject->contact_damage = 50.0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->hyperarmor = false;
			mObject->sprite = init_sprite(256, 448, 16, 16);
			mObject->anim = init_render_info(256, 16, 8, 0, 16);
			mObject->type_reg = ST_ROCK_ROLLER_AWARE;
			mObject->st = init_mObject_state(state_rock_roller_idle, 0, 40, state_rock_roller_aware);
			break;
		case MO_ROCK_VORTEX:
			mObject->speed = mObject->base_speed / 20;
			mObject->max_health = 2000;
			mObject->width = TILE_LENGTH * 4;
			mObject->height = TILE_LENGTH * 4;
			mObject->hit = false;
			mObject->mass = 999;
			mObject->wall_collide = false;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->anim = init_render_info(256, 32, 4, 0, 12);
			mObject->sprite = init_sprite(256, 480, 32, 32);
			mObject->type_reg = 0;
			mObject->hyperarmor = true;
			mObject->st = init_mObject_state(state_rock_vortex_aware, 0, 48, NULL);
			break;
		case MO_LOCAL_QUEEN:
			mObject->speed = mObject->base_speed / 20;
			mObject->max_health = 2000;
			mObject->width = TILE_LENGTH * 1;
			mObject->height = TILE_LENGTH * 2;
			mObject->hit = false;
			mObject->mass = 999;
			mObject->wall_collide = false;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->anim = init_render_info(256, 16, 4, 0, 16);
			mObject->sprite = init_sprite(256, 672, 16, 32);
			mObject->type_reg = 0;
			mObject->hyperarmor = true;
			mObject->st = init_mObject_state(state_local_queen_aware, 0, 120, NULL);
			break;
		case MO_ARCHER:
			mObject->speed = mObject->base_speed;
			mObject->max_health = 150;
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
			mObject->max_health = 250;
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
			mObject->max_health = 75;
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
		case MO_SUMMONER:
			mObject->speed = mObject->base_speed / 20;
			mObject->max_health = 200;
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
			mObject->max_health = 2000;
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
			mObject->max_health = 200;
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
		case MO_RIDER_FIGHTER:
			mObject->speed = mObject->base_speed / 20;
			mObject->max_health = 400;
			mObject->width = TILE_LENGTH * 2;
			mObject->height = TILE_LENGTH * 2;
			mObject->hit = false;
			mObject->mass = 75;
			mObject->wall_collide = false;
			mObject->contact_damage = 50;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->anim = init_render_info(256, 32, 4, 0, 8);
			mObject->sprite = init_sprite(256, 0, 32, 32);
			mObject->type_reg = ST_RIDER_AWARE;
			mObject->hyperarmor = false;
			mObject->st = init_mObject_state(state_rider_idle, 0, 102, state_rider_idle);
			break;
		case MO_DRIDER_FIGHTER:
			mObject->speed = mObject->base_speed / 20;
			mObject->max_health = 200;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH * 1.5;
			mObject->hit = false;
			mObject->mass = 40;
			mObject->wall_collide = false;
			mObject->contact_damage = 50;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->anim = init_render_info(256, 16, 4, 0, 8);
			mObject->sprite = init_sprite(256, 96, 16, 24);
			mObject->type_reg = ST_DRIDER_AWARE;
			mObject->hyperarmor = false;
			mObject->st = init_mObject_state(state_drider_idle, 0, 102, state_drider_idle);
			break;
		case MO_FIRE_TOWER:
			mObject->speed = mObject->base_speed / 20;
			mObject->max_health = 200;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH * 3;
			mObject->hit = false;
			mObject->mass = 500;
			mObject->wall_collide = false;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->anim = init_render_info(256, 16, 4, 0, 8);
			mObject->sprite = init_sprite(256, 144, 16, 48);
			mObject->type_reg = ST_FIRE_TOWER_IDLE;
			mObject->hyperarmor = true;
			mObject->st = init_mObject_state(state_fire_tower_idle, 0, 144, state_fire_tower_idle);

			break;
		case MO_FIRE_BOMBER:
			mObject->speed = mObject->base_speed / 24;
			mObject->max_health = 100;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH;
			mObject->hit = false;
			mObject->mass = 20;
			mObject->wall_collide = true;
			mObject->transp = true;
			mObject->contact_damage = 0;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->anim = init_render_info(256, 16, 4, 0, 8);
			mObject->sprite = init_sprite(256, 192, 16, 16);
			mObject->type_reg = ST_FIRE_BOMBER_IDLE;
			mObject->hyperarmor = true;
			mObject->st = init_mObject_state(state_fire_bomber_idle, 0, 144, state_fire_bomber_idle);
			break;
		case MO_FIRE_ARCHER:
			mObject->speed = mObject->base_speed / 20;
			mObject->max_health = 400;
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH * 1.5;
			mObject->hit = false;
			mObject->mass = 40;
			mObject->wall_collide = false;
			mObject->contact_damage = 50;
			mObject->hittable = true;
			mObject->killable = true;
			mObject->anim = init_render_info(256, 16, 4, 0, 8);
			mObject->sprite = init_sprite(256, 208, 16, 24);
			mObject->type_reg = ST_FIRE_ARCHER_AWARE;
			mObject->hyperarmor = false;
			mObject->st = init_mObject_state(state_fire_archer_idle, 0, 102, state_fire_archer_idle);
			break;
		case MO_INTERACTABLE:
			mObject->width = TILE_LENGTH;
			mObject->height = TILE_LENGTH;
			mObject->hittable = false;
			mObject->st.type = st_placeholder;
			mObject->killable = false;
			mObject->sprite = init_sprite(256, 192, 16, 16);
			mObject->anim = init_render_info(256, 16, 4, 0, 8);
			mObject->st.acp = tp_player_interaction;
			mObject->type_reg = st_placeholder;
			mObject->st.kcp = NULL;
			break;
		case MO_RUNE_SHARD:
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
	mObject->health = mObject->max_health;
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
		case ST_PEAK_LOCAL_AWARE:
			mObject->sprite.x = 256;
			mObject->sprite.y = 320;
			mObject->anim.limit = 8;
			mObject->anim.start_frame = 256;
			break;
		case ST_PEAK_LOCAL_DASH:
			mObject->sprite.x = 256;
			mObject->sprite.y = 344;
			mObject->anim.limit = 6;
			mObject->anim.start_frame = 256;
			break;
		case ST_PEAK_LOCAL_CAST:
			mObject->sprite.x = 320;
			mObject->sprite.y = 320;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 320;
			break;
		case ST_PEAK_KNIGHT_AWARE:
			mObject->sprite.x = 256;
			mObject->sprite.y = 368;
			mObject->anim.limit = 8;
			mObject->anim.start_frame = 256;
			break;
		case ST_PEAK_KNIGHT_DASH:
			mObject->sprite.x = 320;
			mObject->sprite.y = 368;
			mObject->anim.limit = 6;
			mObject->anim.start_frame = 320;
			break;
		case ST_PEAK_KNIGHT_CAST:
			mObject->sprite.x = 256;
			mObject->sprite.y = 392;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 256;
			break;
		case ST_ROCK_WELL_AWARE:
			mObject->sprite.x = 256;
			mObject->sprite.y = 416;
			mObject->anim.limit = 8;
			mObject->anim.start_frame = 256;
			break;
		case ST_ROCK_WELL_CAST:
			mObject->sprite.x = 256;
			mObject->sprite.y = 432;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 256;
			break;
		case ST_ROCK_ROLLER_AWARE:
			mObject->sprite.x = 256;
			mObject->sprite.y = 448;
			mObject->anim.limit = 16;
			mObject->anim.start_frame = 256;
			mObject->anim.frames = 8;
			break;
		case ST_ROCK_ROLLER_AROLL:
			mObject->sprite.x = 256;
			mObject->sprite.y = 464;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 256;
			break;
		case ST_ROCK_ROLLER_DROLL:
			mObject->sprite.x = 320;
			mObject->sprite.y = 464;
			mObject->anim.limit = 6;
			mObject->anim.start_frame = 320;
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
		case ST_RIDER_AWARE:
			mObject->sprite.x = 256;
			mObject->sprite.y = 64;
			mObject->anim.limit = 16;
			mObject->anim.start_frame = 256;
			break;
		case ST_RIDER_CHARGE:
			mObject->sprite.x = 256;
			mObject->sprite.y = 32;
			mObject->anim.limit = 4;
			mObject->anim.start_frame = 256;
			break;
		case ST_DRIDER_AWARE:
			mObject->sprite.x = 256;
			mObject->sprite.y = 96;
			mObject->anim.limit = 8;
			mObject->anim.start_frame = 256;
			break;
		case ST_DRIDER_READY:
			mObject->sprite.x = 320;
			mObject->sprite.y = 96;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 320;
			break;
		case ST_DRIDER_FIRE_CHARGE:
			mObject->sprite.x = 256;
			mObject->sprite.y = 120;
			mObject->anim.limit = 4;
			mObject->anim.start_frame = 256;
			break;
		case ST_FIRE_TOWER_IDLE:
			mObject->sprite.x = 256;
			mObject->sprite.y = 144;
			mObject->anim.limit = 8;
			mObject->anim.start_frame = 256;
			break;
		case ST_FIRE_TOWER_FIRE:
			mObject->sprite.x = 320;
			mObject->sprite.y = 144;
			mObject->anim.limit = (mObject->st.limit / 3) / 4;
			mObject->anim.start_frame = 320;
			break;
		case ST_FIRE_ARCHER_AWARE:
			mObject->sprite.x = 256;
			mObject->sprite.y = 208;
			mObject->anim.limit =  8;
			mObject->anim.start_frame = 256;
			break;
		case ST_FIRE_ARCHER_DASH:
			mObject->sprite.x = 320;
			mObject->sprite.y = 208;
			mObject->anim.limit = 4;
			mObject->anim.start_frame = 320;
			break;
		case ST_FIRE_ARCHER_DRAW:
			mObject->sprite.x = 256;
			mObject->sprite.y = 232;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 256;
			break;
		case ST_FIRE_ARCHER_RITE:
			mObject->sprite.x = 320;
			mObject->sprite.y = 232;
			mObject->anim.limit =  mObject->st.limit / 4;
			mObject->anim.start_frame = 320;
			break;
		case ST_ROCK_VORTEX_AWARE:
			mObject->sprite.x = 256;
			mObject->sprite.y = 480;
			mObject->anim.limit = 12;
			mObject->anim.start_frame = 256;
			break;
		case ST_ROCK_VORTEX_CAST:
			mObject->sprite.x = 256;
			mObject->sprite.y = 512;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 256;
			break;
		case ST_ROCK_VORTEX_DECAST:
			mObject->sprite.x = 384;
			mObject->sprite.y = 512;
			mObject->anim.limit = 6;
			mObject->anim.start_frame = 384;
			break;
		case ST_ROCK_VORTEX_STORM:
			mObject->sprite.x = 384;
			mObject->sprite.y = 480;
			mObject->anim.limit = 15;
			mObject->anim.start_frame = 384;
			break;
		case ST_ROCK_VORTEX_SUMMON:
			mObject->sprite.x = 384;
			mObject->sprite.y = 544;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 384;
			break;
		case ST_LOCAL_QUEEN_AWARE:
			mObject->sprite.x = 256;
			mObject->sprite.y = 672;
			mObject->anim.limit = 16;
			mObject->anim.start_frame = 256;
			break;
		case ST_LOCAL_QUEEN_DASH:
			mObject->sprite.x = 256;
			mObject->sprite.y = 704;
			mObject->anim.limit = 8;
			mObject->anim.start_frame = 256;
			break;
		case ST_LOCAL_QUEEN_CAST:
			mObject->sprite.x = 320;
			mObject->sprite.y = 672;
			mObject->anim.limit = 8;
			mObject->anim.start_frame = 320;
			break;
		case ST_LOCAL_QUEEN_BOG:
			mObject->sprite.x = 320;
			mObject->sprite.y = 704;
			mObject->anim.limit = mObject->st.limit / 4;
			mObject->anim.start_frame = 320;
			break;
		case ST_ROCK_VORTEX_PRESTORM:
			mObject->sprite.x = 256;
			mObject->sprite.y = 544;
			mObject->anim.limit = mObject->st.limit;
			mObject->anim.start_frame = 256;
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
		case ST_ENEMYKNOCKBACK:
			switch(mObject->id)
			{

				case 'f':
					mObject->sprite.y = 368;
					mObject->sprite.x = 384;
					mObject->anim.limit = mObject->st.limit + 124;
					mObject->anim.start_frame = 320;
					break;
				case 'r':
					mObject->sprite.y = 448;
					mObject->sprite.x = 384;
					mObject->anim.limit = mObject->st.limit + 124;
					mObject->anim.start_frame = 384;
					break;
				case 'l':
					mObject->sprite.y = 320;
					mObject->sprite.x = 384;
					mObject->anim.limit = mObject->st.limit + 124;
					mObject->anim.start_frame = 128;
					break;
				case 'w':
					mObject->sprite.x = 320;
					mObject->sprite.y = 416;
					mObject->anim.limit = mObject->st.limit + 124;
					mObject->anim.start_frame = 320;
					break;
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
				case '8': 
					mObject->sprite.x = 384;
					mObject->sprite.y = 0;
					mObject->anim.start_frame = 384;
					mObject->anim.limit = mObject->st.limit + 124;
					break;
				case '9':
					mObject->sprite.x = 384;
					mObject->sprite.y = 96;
					mObject->anim.start_frame = 384;
					mObject->anim.limit = mObject->st.limit + 124;
					break;
				case 'a':
					mObject->sprite.x = 384;
					mObject->sprite.y = 208;
					mObject->anim.start_frame = 384;
					mObject->anim.limit = mObject->st.limit + 124;
					break;
			}
			break;
		case ST_DEATHRATTLE:
			switch(mObject->id)
			{
				case 'B':
					mObject->sprite.x = 192;
					mObject->anim.limit = mObject->st.limit / 4;
					mObject->anim.frames = 4;
					mObject->anim.start_frame = 192;
					break;
				case 'r':
					mObject->sprite.x = 448;
					mObject->sprite.y = 448;
					mObject->anim.limit = mObject->st.limit / 4;
					mObject->anim.start_frame = 448;
					break;
				case 'f':
					mObject->sprite.x = 448;
					mObject->sprite.y = 368;
					mObject->anim.limit = mObject->st.limit / 4;
					mObject->anim.start_frame = 448;
					break;
				case 'l':
					mObject->sprite.x = 448;
					mObject->sprite.y = 320;
					mObject->anim.limit = mObject->st.limit / 4;
					mObject->anim.start_frame = 448;
					break;
				case 'w':
					mObject->sprite.x = 448;
					mObject->sprite.y = 416;
					mObject->anim.limit = mObject->st.limit / 4;
					mObject->anim.start_frame = 448;
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
				case '8':
					mObject->sprite.y = 32;
					mObject->sprite.x = 384;
					mObject->anim.limit = mObject->st.limit / 4;
					mObject->anim.start_frame = 384;
					break;
				case '9':
					mObject->sprite.y = 96;
					mObject->sprite.x = 448;
					mObject->anim.limit = mObject->st.limit / 4;
					mObject->anim.start_frame = 448;
					break;
				case 't':
					mObject->sprite.y = 144;
					mObject->sprite.x = 448;
					mObject->anim.limit = mObject->st.limit / 4;
					mObject->anim.start_frame = 448;
					break;
				case 's':
					mObject->sprite.y = 192;
					mObject->sprite.x = 320;
					mObject->anim.limit = mObject->st.limit / 4;
					mObject->anim.start_frame = 320;
					break;
				case 'a':
					mObject->sprite.x = 448;
					mObject->sprite.y = 208;
					mObject->anim.limit = mObject->st.limit / 4;
					mObject->anim.start_frame = 448;
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
