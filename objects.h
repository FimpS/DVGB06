#ifndef OBJECTS_HEADER_H
#define OBJECTS_HEADER_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "dynList.h"
#include "enum.h"
#include "map.h"
#include "runes.h"
#include "object_state.h"

struct rune_info;
struct mObject;
struct pObject;
struct player;

struct status_effect
{
	mObject_status_effect type;
	int timer;
	int limit;
};

struct pObj_state
{
	pObject_global_state type;
	void (*acp)(struct pObject*, struct player*, struct map*);
	int timer;
	int limit;
};

struct mObj_state
{
	mObject_state type; 
	void (*acp)(struct mObject*, struct player*, struct map*);
	void (*kcp)(struct mObject*, struct player*, struct map*);
	int timer;
	int limit;
};

struct render_info
{
	int start_frame;
	int tile_length;
	int frames;
	int limit;
	int timer;
	bool rotatable;
};

struct player
{
	double x;
	double y;
	double vel_x;
	double vel_y;
	double width;
	double height;
	double health;
	double maxhealth;
	double theta;
	double speed;
	double base_speed;
	double pObject_knockkoef;
	int timer;
	int attack_speed_timer;
	int attack_speed;
	int dash_timer;
	int invuln_limit;
	int dash_cooldown_timer;
	double sword_damage;
	mObject_status_effect sword_effect_type;
	int swingT;
	int cd;
	double thrust_decel;
	player_global_state global_state; 
	bool change_map;
	bool invuln;
	dynList *rune_list;
	card_dir dir;
	int anim1counter;
	int kills;

	SDL_Rect sprite;
	struct render_info anim;
};

struct pObject
{
	double x; //const
	double y; //const
	double vel_x; //const
	double vel_y; //const
	double width; //var
	double height; //var
	double knockkoef; //var
	double speed; //var?
	double damage; //var
	double theta; //const
	double thetaacc; //var
	bool transp; //var
	bool pen_wall; //var
	bool knockbacker; //var
	int penetration_index; //var
	mObject_status_effect status_effect; //const
	card_dir dir; //const
	pObject_type type; //const

	struct mObject *seeking_target; //var
	struct pObj_state st; //var

	int anim_start_frame; //var
	int anim_tile_length; //var
	int anim_frames; //var
	int anim_limit; //var
	int anim_timer; //var
	SDL_Rect sprite; //var
};

struct mObject
{
	double x;
	double y;
	double vel_x;
	double vel_y;
	double width;
	double height;
	bool hit;
	int inv_frames;
	char id;
	bool hittable;
	double contact_damage;	

	double base_speed;
	double speed;
	double theta;
	double mass;
	bool wall_collide;
	bool killable;
	bool hyperarmor;

	double health;

	struct mObj_state st;
	struct rune_info r_info;
	struct status_effect status_effect;

	mObject_type type;

	union 
	{
		int cheiftain_ticker;
		double lol;
	} atts;

	mObject_global_state type_reg;
	struct render_info anim;
	SDL_Rect sprite;
};



int getTick();
//struct rune init_pObject_state();

//s
void initShot();
void updateShot();
void drawShot();
void checkShot();
void destroyShot();

//general
bool AABB();
bool AABBpp();
bool AABBpm();
struct mObject* id_get_mObj(struct map* map, char id);
void set_status_effect();
void identify_status_effect();
//e
void tp_player_interaction();
void rune_player_interaction();
void spawn_mObject(struct map* map, int x, int y, mObject_type type, char id);
void delete_mObject();
void init_mObject(struct mObject* mObject, int x, int y, struct map* map);
void draw_mObject();
void draw_all_mObjects();
void update_all_mObjects();
void update_mObject();
void mObject_move();
void mObject_player_hitbox();
void mObject_damage();

//proj
void draw_pObject();
void update_pObject();
void draw_pObjects();
void update_pObjects();
void pObject_move();
struct pObject* spawn_pObject();
//p
void player_hit();
void player_move();
void initPlayer();
void drawPlayer();
void updatePlayer();
void updateSword();
void stateLogic();

#endif
