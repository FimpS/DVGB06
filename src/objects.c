#include "objects.h"
#include "global.h"
#include "map.h"
#include "event.h"
#include "object_state.h"
#include "runes.h"
#include "gfx.h"
#include "info.h"
#include "font.h"

static int tick = 0;

bool AABB(struct mObject *s, struct mObject *t)
{
	return s->x < t->x + t->width/TILE_LENGTH &&
		   s->x + s->width/TILE_LENGTH > t->x &&
		   s->y < t->y + t->height/TILE_LENGTH &&
		   s->y + s->height/TILE_LENGTH > t->y;
}


bool POLARBB(struct mObject* target, struct mObject* source)
{
	const double r = source->width > source->height ? source->width / TILE_LENGTH / 2 : source->height / TILE_LENGTH / 2;
	return MIDPOINTX(source) + r*cos(source->theta) >= target->x &&
		   MIDPOINTX(source) + r*cos(source->theta) < target->x + target->width/TILE_LENGTH &&
		   MIDPOINTY(source) + r*sin(source->theta) < target->y + target->height/TILE_LENGTH&&
		   MIDPOINTY(source) + r*sin(source->theta) >= target->y;
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

static int effect_limits[] = 
{
	64, //HEX
	64, //STUN
	564, //BOG
	128, //BURN
	360, //FROSTBITE
	200, //ROT
	128, //STASIS
	360, //OMEN
};

void set_status_effect(struct mObject *mObject, int timer, int limit, object_status_effect type)
{
	mObject->status_effect.timer = timer;
	mObject->status_effect.limit = limit;
	mObject->status_effect.type = type;
}


void spawn_pObject(dynList *pObject_list, double x, double y, pObject_type type, card_dir dir, double dmg, double theta, struct player* player)
{
	struct pObject* new = (struct pObject*)malloc(sizeof(struct pObject));
	new->type = type;
	init_pObject(new, x, y, dir, dmg, theta, player);
	dynList_add(pObject_list, (void*)new);
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
	rune = (struct rune*)dynList_get(player->rune_list, 2);
	if(rune != NULL && rune->info.rune_type == RN_ROT && target->status_effect.type == STATUS_ROT)
	{
		target->health -= 0.5 * source->damage;
	}
	rune = (struct rune*)dynList_get(player->rune_list, 2);
	if(rune != NULL && rune->info.rune_type == RN_GRAVITY && target->hyperarmor)
	{
		target->health -= (source->knockkoef - 1) * source->damage;
	}
}

void stasis_status_effect(struct mObject *mObject, struct player* player)
{
	mObject->base_speed = 0;
	if(mObject->status_effect.timer >= mObject->status_effect.limit)
	{
		mObject->base_speed = 1;
		set_status_effect(mObject, 0, 0, STATUS_NONE);
	}
	mObject->status_effect.timer ++;
}

void frozen_status_effect(struct mObject *mObject, struct player* player)
{
	mObject->base_speed = 0.25;
	if(mObject->status_effect.timer >= mObject->status_effect.limit)
	{
		struct rune* rune = (struct rune*)dynList_get(player->rune_list, 3);
		if(rune != NULL && rune->info.rune_type == RN_FROST)
		{
			mObject->health -= player->sword_damage * 3; //mObject->health * 0.5;
		}
		set_status_effect(mObject, 0, 0, STATUS_NONE);
		mObject->base_speed = 1;
	}
	mObject->status_effect.timer ++;
}

void rot_status_effect(struct mObject *mObject)
{
	if(getTick() % 60 == 0)
	{
		if(mObject->health > 10)
		{
			mObject->health -= 10;
		}
	}
	if(mObject->status_effect.timer ++ >= mObject->status_effect.limit)
	{
		set_status_effect(mObject, 0, 0, STATUS_NONE);
	}
}

void identify_status_effect(struct mObject *mObject, struct player* player)
{
	switch(mObject->status_effect.type)
	{
		case STATUS_FROSTBITE:
			frozen_status_effect(mObject, player);
			break;
		case STATUS_ROT:
			rot_status_effect(mObject);
			break;
		case STATUS_STASIS:
			stasis_status_effect(mObject, player);
			break;
		default:
			break;
	}
}

void player_status_effect(struct player* player)
{

}

//misc
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
				target->st.type = ST_ENEMYDEAD;
		}
	}
}

void update_all_mObjects(dynList *mObject_list, struct player *player, struct map *map, struct cam *cam, dynList *ev_list)
{
	if(dynList_is_empty(mObject_list))
		return;
	for(int i = 0; i < mObject_list->size; i++)
	{
		struct mObject *curr_mObj = (struct mObject*)dynList_get(mObject_list, i);
		if(curr_mObj->st.type == ST_CLEAR)
		{
			dynList_del_index(mObject_list, i --);
			continue;
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
			set_mObject_state(mObject, ST_DEATHRATTLE, NULL, 0, 32);
			for(int i = 0; i < map->mObject_list->size; i++)
			{
				struct mObject* curr = (struct mObject*)dynList_get(map->mObject_list, i);
				if(curr->id == 'R')
				{
					map->aggresive_mObj_count --;
					set_mObject_state(curr, ST_DEATHRATTLE, state_deathrattle, 0, 32);
				}
			}
		}
	}
}
void tut_player_interaction(struct mObject* mObject, struct player* player, struct map* map)
{
	if(AABB((struct mObject*)player, mObject))
	{
		if(mObject->st.timer >= 128)
		{
			display_tutorial(map);
			mObject->st.timer = 0;
		}
	}
	mObject->st.timer ++;
} 

void stat_player_interaction(struct mObject* mObject, struct player* player, struct map* map)
{
	if(AABB((struct mObject*)player, mObject))
	{
		if(mObject->st.timer >= 128)
		{
			display_all_stats(map);
			mObject->st.timer = 0;
		}
	}
	mObject->st.timer ++;
}

void endp_player_whiten(struct mObject* mObject, struct player* player, struct map* map)
{
	if(mObject->st.timer ++ >= mObject->st.limit)
	{
		add_event(map->event_list, TYPE_EVENT_END_RUN, player, map, 0);
	}
}

void endp_player_interaction(struct mObject* mObject, struct player* player, struct map* map)
{
	const Uint8* cks = SDL_GetKeyboardState(NULL);
	if(cks[SDL_SCANCODE_E])
	{
		if(AABB((struct mObject*)player, mObject))
		{
			add_message(map->msg_list, "RUN COMPLETED", X_MIDDLE_FONT, 4.0, 360, 2);
			set_mObject_state(mObject, st_placeholder, endp_player_whiten, 0, 128);
		}
	}

}


void state_startp_stay(struct mObject* mObject, struct player* player, struct map* map)
{
	if(mObject->st.timer ++ >= mObject->st.limit && AABB(mObject, (struct mObject*)player))
	{
		add_event(map->event_list, TYPE_EVENT_START_RUN, player, map, 0);
	}
}


void state_startp_open(struct mObject* mObject, struct player* player, struct map* map)
{
	if(mObject->st.timer ++ >= mObject->st.limit)
	{
		set_mObject_state(mObject, ST_STARTP_STAY, state_startp_stay, 0, 64);
	}
}

void startp_player_interaction(struct mObject* mObject, struct player* player, struct map* map)
{
	const Uint8* cks = SDL_GetKeyboardState(NULL);
	if(cks[SDL_SCANCODE_E])
	{
		if(AABB((struct mObject*)player, mObject))
		{
			set_screen_shake(map, 0.1, 180);
			//add_message(map->msg_list, "FRUIT JUMPSCARE", X_MIDDLE_FONT, 4.0, 256, 2);
			set_mObject_state(mObject, ST_STARTP_OPEN, state_startp_open, 0, 204);
			//add_event(map->event_list, TYPE_EVENT_START_RUN, player, map, 0);
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
		if(AABB((struct mObject*)player, mObject))
		{
			add_event(ev_list, type_event_teleport, player, map, 0);
		}
	}
}

static char *stat_strs[] =
{
	"MAX HEALTH",
	"DAMAGE",
	"ATTACK SPEED",
	"DAMAGE INVULNERABILITY",
	"DASH INVULNERABILITY",
	"MOVEMENT SPEED",
};

void add_map_end_stat(struct map* map, struct player* player)
{
	int stat_upgrade = rand() % 6;
	player->health += 20;
	switch(stat_upgrade)
	{
		case 0: player->maxhealth += 40; player->health += 40; break;
		case 1: player->sword_damage += 5.0; break;
		case 2: player->attack_speed -= 2; break;
		case 3: player->dmg_invuln += 8; break;
		case 4: player->dash_invuln += 2; break;
		case 5: player->base_speed += 0.0125; break;
	}
	const char *stat_str = stat_strs[stat_upgrade];
	add_message(map->msg_list, "UPGRADE;", X_MIDDLE_FONT, 4.0, 280, 2);
	add_message(map->msg_list, stat_str, X_MIDDLE_FONT, 6.0, 280, 2);
}

void tp_player_undone(struct mObject* mObject, struct player* player, struct map* map)
{
	if(map->aggresive_mObj_count <= 0)
	{
		map->save.recent_rooms_completed ++;
		add_message(map->msg_list, "!NO ENEMIES LEFT!", X_MIDDLE_FONT, 2.0, 128, 2);
		set_mObject_state(mObject, ST_INTERACTABLE, tp_player_interaction, 0, 0);
		add_map_end_stat(map, player);
	}
}

void player_move(struct player *player, struct map *map, struct cam *cam)
{
	double new_x = 0, new_y = 0;
	bool flight = false;
	new_x = player->x + player->vel_x;
	new_y = player->y + player->vel_y;

	double offw = player->width/TILE_LENGTH;
	double offh = player->height/TILE_LENGTH;
	int fx = (int)(player->width/TILE_LENGTH);
	int fy = (int)(player->height/TILE_LENGTH);
	double wunderkindw = ((int)player->width <= TILE_LENGTH) ? 1 - (offw) : (offw - (int)offw != 0 ? -1 * (fx-offw) : 0.0);
	double wunderkindh = ((int)player->height <= TILE_LENGTH) ? 1 - (offh) : (offh - (int)offh != 0 ? -1 * (fy-offh) : 0.0);
	if(!flight)
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
			if(map_get_solid(map, (int)(new_x), (int)(new_y + offh)) || map_get_solid(map, (int)(new_x + player->width/TILE_LENGTH - 0.1), (int)(new_y + player->height/TILE_LENGTH)))
			{
				new_y = (int)new_y + wunderkindh;
				player->vel_y = 0;
			}
		}
	}
	player->x = new_x;
	player->y = new_y;

	if(player->x >= (double)map->width-2)
		player->x = (double)map->width-2;
	if(player->y >= (double)map->height-2)
		player->y = (double)map->height-2;
	if(player->x < 1)
		player->x = 1;
	if(player->y < 1)
		player->y = 1;
}
#if 0
void player_move(struct player *player, struct map *map, struct cam *cam)
{
	double new_x = 0, new_y = 0;
	bool flight = false;
	new_x = player->x + player->vel_x;
	new_y = player->y + player->vel_y;

	const double offw = player->width/TILE_LENGTH;
	const double offh = player->height/TILE_LENGTH;
	const int fx = (int)(player->width/TILE_LENGTH);
	const int fy = (int)(player->height/TILE_LENGTH);
	const double wunderkindw = ((int)player->width <= TILE_LENGTH) ? 1 - (offw) : (offw - (int)offw != 0 ? -1 * (fx-offw) : 0.0);
	const double wunderkindh = ((int)player->height <= TILE_LENGTH) ? 1 - (offh) : (offh - (int)offh != 0 ? -1 * (fy-offh) : 0.0);
	const double f = 0.1;
	if(!flight)
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
			if(map_get_solid(map, (int)(new_x), (int)(new_y + offh)) || map_get_solid(map, (int)(new_x + (offw - f)), (int)(new_y + offh)))
			{
				new_y = (int)new_y + wunderkindh;
				player->vel_y = 0;
			}
		}
	}
	player->x = new_x;
	player->y = new_y;

	if(player->x >= (double)map->width-2)
		player->x = (double)map->width-2;
	if(player->y >= (double)map->height-2)
		player->y = (double)map->height-2;
	if(player->x < 1)
		player->x = 1;
	if(player->y < 1)
		player->y = 1;
}
#endif
void check_player_state(struct player* player, struct map* map, struct cam* cam, const Uint8* currentKeyStates)
{
	switch(player->global_state)
	{
		case ST_P_NORMAL:
			dash_control(player, currentKeyStates);
			player_inp_move(player, currentKeyStates);
			player_move(player, map, cam);
			input_attack(player, map, currentKeyStates);
			break;
		case ST_P_KNOCKBACK:
			player_knockbacked(player, cam, map);
			break;
		case ST_P_DASH:
			player_dash_atk(player, map);
			player_dash(player, map, cam);
			break;
		case ST_P_ATTACK:
			player_attack(player, map, currentKeyStates);
			break;
		case ST_P_ATTACKING:
			player_attacking(player, map);
			break;
		case ST_P_DEAD:
			player_deathrattle(player, map);
			return;
			break;
		case ST_P_GONE:
			return;
			break;
		default:
			break;
	}
	player_invuln(player, map);

}

void reset_player_run(struct player* player, struct map* map)
{
	player->maxhealth = PLAYER_START_HP; //TODO;
	player->health = player->maxhealth;
	player->base_speed = PLAYER_START_MS;
	player->attack_speed = PLAYER_START_AS;
	player->sword_damage = PLAYER_START_DMG;
	player->kills = 0;
	player->dash_invuln = PLAYER_START_DASH_INVULN;
	player->dmg_invuln = PLAYER_START_DMG_INVULN;
	player->sword_effect_type = STATUS_NONE;
	player->global_state = ST_P_NORMAL;
	dynList_clear(player->rune_list);
	run_reset_UI(map);
}

void display_tutorial(struct map* map)
{
	add_message(map->msg_list, "TUTORIAL;", X_MIDDLE_FONT, 2.0, 128, 2);
	add_message(map->msg_list, "SPACE TO DASH.", X_MIDDLE_FONT, 4.0, 128, 2);
	add_message(map->msg_list, "M1 TO ATTACK.", X_MIDDLE_FONT, 6.0, 128, 2);
	add_message(map->msg_list, "M1 WHILE DASHING", X_MIDDLE_FONT, 8.0, 128, 2);
	add_message(map->msg_list, "TO DASH-ATTACK.", X_MIDDLE_FONT, 10.0, 128, 2);
	add_message(map->msg_list, "E TO INTERACT.", X_MIDDLE_FONT, 12.0, 128, 2);
	add_message(map->msg_list, "ESC FOR MENU.", X_MIDDLE_FONT, 14.0, 128, 2);
}

void display_stat(struct map* map, int stat, const char* str, double posx, double posy)
{
	char buff[64];
	char tmp[16];
	strcpy(buff, str); 
	sprintf(tmp, "%d", stat);
	for(int i = 1; i < strlen(tmp); i++)
	{
		buff[strlen(buff) - 1] = '\0';
	}
	strcat(buff, tmp);
	add_message(map->msg_list, buff, posx, posy, 128, 1);
}

void display_all_stats(struct map* map)
{
	const double x = 21.0;
	const double y = 3.0;
	add_message(map->msg_list, "TOTAL;            ", x, y, 128, 1);
	display_stat(map, map->save.monsters_slain, "MONSTERS SLAIN    ", x, y + 1);	
	display_stat(map, map->save.rooms_completed, "ROOMS COMPLETED   ", x, y + 2);	
	display_stat(map, map->save.bosses_killed, "BOSSES KILLED     ", x, y + 3);	
	display_stat(map, map->save.runs_completed, "RUNS COMPLETED    ", x, y + 4);	
	
	add_message(map->msg_list, "RECENT;           ", x, y + 6, 128, 1);
	display_stat(map, map->save.recent_slain, "MONSTERS SLAIN    ", x, y + 7);	
	display_stat(map, map->save.recent_rooms_completed, "ROOMS COMPLETED   ", x, y + 8);	
	display_stat(map, map->save.recent_bosses_killed, "BOSSES KILLED     ", x, y + 9);	
	display_stat(map, map->save.recent_runs_completed, "RUNS COMPLETED    ", x, y + 10);
}

static int tocker = 0;

void updatePlayer(struct player *player, struct map *map, struct cam *cam, dynList *e_list, dynList *ev_list, dynList *pObject_list)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	player->vel_x = 0;
	player->vel_y = 0;
	if(player->maxhealth < player->health)
		player->health = player->maxhealth;
	tocker ++;
	if(currentKeyStates[SDL_SCANCODE_B] && tocker >= 60)
	{
		display_all_stats(map);
		tocker = 0;
	}

	if(player->health <= 0 && (player->global_state != ST_P_DEAD && player->global_state != ST_P_GONE))
	{
		struct rune* rune = (struct rune*)dynList_get(player->rune_list, 3);
		if(rune != NULL && rune->info.rune_type == RN_HOLY)
		{
			player->health += 50 * rune->attribute --;
			return;
		}
		player->global_state = ST_P_DEAD;
		identify_player_sprite_location(player);
	}
	rune_abilities(player, map);
	run_player_status_effects(player);
	check_player_state(player, map, cam, currentKeyStates);
	if(player->global_state == ST_P_KNOCKBACK || player->global_state == ST_P_ATTACKING)
		return;

}

void pObject_seek(struct pObject* pObject, const double s_koef, const double dsttheta)
{
	double dtheta = dsttheta - pObject->theta;

	if(dtheta > PI)
		dtheta -= 2 * PI;
	else if(dtheta < -PI)
		dtheta += 2 * PI;

	pObject->theta += s_koef * dtheta;

	while(pObject->theta >= 2*PI)
		pObject->theta -= 2*PI;
	while(pObject->theta < 0.0)
		pObject->theta += 2*PI;
}

void pObject_move(struct pObject *pObject, struct player *player, struct map *map)
{
	double new_x = 0, new_y = 0;
	bool hit_wall = false;

	pObject->vel_x = pObject->speed*cos(pObject->theta);
	pObject->vel_y = pObject->speed*sin(pObject->theta);
	new_x = pObject->x + pObject->vel_x;
	new_y = pObject->y + pObject->vel_y;

	const double f = 0.1;
	const double offw = pObject->width/TILE_LENGTH;
	const double offh = pObject->height/TILE_LENGTH;

	double wunderkindw = ((int)pObject->width <= TILE_LENGTH) ? 1 - offw : 0.0;
	double wunderkindh = ((int)pObject->height <= TILE_LENGTH) ? 1 - offh : 0.0;
#if 1
	if(!pObject->transp)
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
	if(hit_wall)
	{
		set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, 16);
		return;
	}

	pObject->x = new_x;
	pObject->y = new_y;

}
void set_status_effect_area(struct player* player, struct map *map, int distancesquared, object_status_effect effect)
{
	for(int i = 0; i < map->mObject_list->size; i++)
	{
		struct mObject* mObj = (struct mObject*)dynList_get(map->mObject_list, i);
		const double dx = OBJDIFFX(player, mObj), dy = OBJDIFFY(player, mObj);
		if(sum_square(dy, dx) <= distancesquared && mObj->hittable)
		{
			set_status_effect(mObj, 0, effect_limits[effect], effect);
		}
	}

}

void mObject_move(struct mObject *mObject, struct player *player, struct map *map)
{
	double new_x = 0, new_y = 0;
	//write a paragraph about why this didnt work in reverse order
	mObject->vel_x = mObject->speed*cos(mObject->theta);
	mObject->vel_y = mObject->speed*sin(mObject->theta);
	new_x = mObject->x + mObject->vel_x;
	new_y = mObject->y + mObject->vel_y;



	const double offw = mObject->width/TILE_LENGTH;
	const double offh = mObject->height/TILE_LENGTH;
	const int fx = (int)(mObject->width/TILE_LENGTH);
	const int fy = (int)(mObject->height/TILE_LENGTH);
	double wunderkindw = ((int)mObject->width <= TILE_LENGTH) ? 1 - (offw) : (offw - (int)offw != 0 ? -1 * (fx-offw) : 0.0);
	double wunderkindh = ((int)mObject->height <= TILE_LENGTH) ? 1 - (offh) : (offh - (int)offh != 0 ? -1 * (fy-offh) : 0.0);

	double f = 0.1;
	bool hit_wall = false;
	if(!mObject->transp)
	{
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


		if(mObject->st.type == ST_ENEMYKNOCKBACK && hit_wall == true)
		{
			struct rune* rune = (struct rune*)dynList_get(player->rune_list, 2);
			if(rune != NULL && rune->info.rune_type == RN_GRAVITY)
			{
				rune = (struct rune*)dynList_get(player->rune_list, 3);
				if(rune != NULL && rune->info.rune_type == RN_GRAVITY)
				{
					set_status_effect(mObject, 0, effect_limits[STATUS_STASIS], STATUS_STASIS);
				}
				mObject->health -= player->sword_damage * 1;
				hit_wall = false;
				mObject->st.type = 0;
			}
		}
	}
	mObject->wall_collide = hit_wall;
	mObject->x = new_x;
	mObject->y = new_y;

}

bool divine_shield(struct player *player)
{
	struct rune* rune = (struct rune*)dynList_get(player->rune_list, 0);
	if(!rune) return false;
	bool res = rune->info.rune_type == RN_HOLY && rune->attribute == true ? true : false;
	rune->attribute = false;
	return res;
}

void player_hit(struct player* player, double damage, double theta) 
{
	player->invuln = true;
	player->timer = 0;
	player->invuln_limit = player->dmg_invuln;
	if(divine_shield(player) == true)
		return;
	player->health -= damage;
	player->speed = 0.3;
	player->theta = theta;
	player->global_state = ST_P_KNOCKBACK;
}

void mObject_damage(struct mObject* target, struct pObject *source, struct player *player)
{
	int healthbefore = target->health;
	target->health -= source->damage;
	target->inv_frames = 0;
	check_damage_modifiers(target, source, player);
	set_status_effect(target, 0, effect_limits[source->status_effect], source->status_effect);
	if(!source->knockbacker || target->hyperarmor)
		return;
	const double dx = OBJDIFF(target, source, 'X'), dy = OBJDIFF(target, source, 'Y');
	target->theta = atan2(dy, dx);
	target->speed = 0.2 * source->knockkoef - target->mass / 1024;
	target->speed = target->speed <= 0 ? 0 : target->speed;
	set_mObject_state(target, ST_ENEMYKNOCKBACK, state_enemy_knockbacked, 0, 20);
}

void mObject_player_hitbox(struct mObject *mObject, struct player *player)
{
	double dx = player->x - mObject->x, dy = player->y - mObject->y;
	if(!player->invuln && AABB(mObject, (struct mObject*)player))
	{
		player_hit(player, mObject->contact_damage, mObject->theta);
	}
}

void omen_status_effect(struct player* player, struct status_effect *effect)
{
	if(effect->timer == 0)
	{
		player->sword_damage -= 40.0;
	}
	if(effect->timer >= effect->limit)
	{
		player->sword_damage += 40.0;
	}
}

void burn_status_effect(struct player* player, struct status_effect *effect)
{
	if(effect->timer % 40 == 0)
	{
		player->health -= 3.0;
	}
}
void bogged_status_effect(struct player* player, struct status_effect *effect)
{
	if(effect->timer == 0)
	{
		player->base_speed -= 0.35;
	}
	if(effect->timer == effect->limit)
	{
		player->base_speed += 0.35;
		player->speed = player->base_speed / 16;
	}
}
void stun_status_effect(struct player* player, struct status_effect *effect)
{
	player->base_speed = 0.0;
	if(effect->timer >= effect->limit)
	{
		player->base_speed = 1.0;
	}
}

void hex_status_effect(struct player* player, struct status_effect *effect)
{
	player->dash_cooldown_timer = 0;
}

bool has_player_status_effect(struct player* player, object_status_effect effect)
{
	if(dynList_is_empty(player->se_list))
		return false;
	return effect == ((struct status_effect*)dynList_get(player->se_list, 0))->type;
}

void run_player_status_effects(struct player* player)
{
	if(dynList_is_empty(player->se_list))
		return;
	for(int i = 0; i < player->se_list->size; i++)
	{
		struct status_effect *effect = dynList_get(player->se_list, i);
		switch(effect->type)
		{
			case STATUS_OMEN:
				omen_status_effect(player, effect);
				break;
			case STATUS_BOGGED:
				bogged_status_effect(player, effect);
				break;
			case STATUS_BURN:
				burn_status_effect(player, effect);
				break;
			case STATUS_STUN:
				stun_status_effect(player, effect);
				break;
			case STATUS_HEX:
				hex_status_effect(player, effect);
				break;
		}

		if(effect->timer ++ >= effect->limit)
		{
			dynList_del_index(player->se_list, i); 
		}
	}
}
bool effect_exists(struct player* player, object_status_effect effect)
{
	for(int i = 0; i < player->se_list->size; i++)
	{
		struct status_effect *peffect = dynList_get(player->se_list, i);
		if(effect == peffect->type)
			return true;
	}
	return false;
}



void apply_player_status_effect(struct player* player, object_status_effect effect)
{

	struct rune* rune = (struct rune*)dynList_get(player->rune_list, 2);
	if(rune != NULL && rune->info.rune_type == RN_HOLY)
		return;
	if(effect == STATUS_NONE || effect_exists(player, effect))
		return;
	struct status_effect *new = (struct status_effect*)malloc(sizeof(struct status_effect));
	new->type = effect;
	new->timer = 0;
	new->limit = effect_limits[effect]; 
	dynList_add(player->se_list, (void*)new);
}

void pObject_player_hitbox(struct pObject* pObject, struct player *player)
{
	if(!player->invuln && AABB((struct mObject*)pObject, (struct mObject*)player))
	{
		player_hit(player, pObject->damage, pObject->theta);
		apply_player_status_effect(player, pObject->status_effect);
		if(--pObject->penetration_index == 0)
		{
			set_pObject_state(pObject, ST_PO_DEATHRATTLE, state_pObject_deathrattle, 0, 16);
		}
	}
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
	SDL_Rect R = {(player->x - cam->offset_x) * TILE_LENGTH, (player->y - cam->offset_y) * TILE_LENGTH, player->width, player->height};

	render_player_animation(player, R, renderer, tex);
}

void draw_pObject(SDL_Renderer *renderer, struct pObject *pObject, struct cam *cam, SDL_Texture* tex)
{
	SDL_Rect r = {(pObject->x - cam->offset_x) * TILE_LENGTH, (pObject->y - cam->offset_y) * TILE_LENGTH, pObject->width, pObject->height};

	render_animation(pObject, tex, r, renderer);
}

void draw_mObject(SDL_Renderer *renderer, struct mObject *mObject, struct cam *cam, SDL_Texture *tex, struct player* player)
{
	SDL_Rect r = {(mObject->x - cam->offset_x) * TILE_LENGTH - 0, (mObject->y - cam->offset_y) * TILE_LENGTH - (mObject->sprite.h * 0), mObject->width, mObject->height};
	render_mObject_animation(mObject, r, renderer, tex, player);

}
