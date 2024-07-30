#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "objects.h"
#include "global.h"
#include "font.h"
#include "gfx.h"
#include "event.h"

#define TEXTURE_AMOUNT 8

void gfx_init(SDL_Texture **textures, SDL_Renderer *renderer)
{
	SDL_Surface *surface;
	surface = IMG_Load("assets/map_spritesheet.png");
	textures[MAP_SPRITESHEET] = SDL_CreateTextureFromSurface(renderer, surface);
	surface = IMG_Load("assets/pObject_spritesheet.png");
	textures[POBJECT_SPRITESHEET] = SDL_CreateTextureFromSurface(renderer, surface);
	surface = IMG_Load("assets/mObject_spritesheet.png");
	textures[MOBJECT_SPRITESHEET] = SDL_CreateTextureFromSurface(renderer, surface);
	surface = IMG_Load("assets/runes_spritesheet.png");
	textures[RUNES_SPRITESHEET] = SDL_CreateTextureFromSurface(renderer, surface);
	surface = IMG_Load("assets/player_spritesheet.png");
	textures[PLAYER_SPRITESHEET] = SDL_CreateTextureFromSurface(renderer, surface);
	surface = IMG_Load("assets/font_spritesheet.png");
	textures[FONT_SPRITESHEET] = SDL_CreateTextureFromSurface(renderer, surface);
	surface = IMG_Load("assets/ui.png");
	textures[UI_SPRITESHEET] = SDL_CreateTextureFromSurface(renderer, surface);
}


const struct UI_element UI_sprite_info[] = {
	{UI_curr_health_update, {0,0,336,16}, {8,8,384,16}, 0}, //player-health
	{NULL, {0,16,336,16}, {8,8,384,16}, 1}, //player-maxhealth
	{NULL, {0,32,384,16}, {-8, 8, 428, 16}, 2}, //player-maxhealthdecor
	{NULL, {0,48,32,32}, {800 - 32 - 8, 40, 24, 24}, 3},
	{NULL, {0,48,32,32}, {800 - 32 - 8, 72 + 12, 24, 24}, 4},
	{NULL, {0,48,32,32}, {800 - 32 - 8, 104 + 24, 24, 24}, 5},

	{NULL, {32,48,32,32}, {800 - 32 - 8, 264, 32, 32}, 6},
	{NULL, {64,48,32,32}, {800 - 32 - 8, 264, 32, 32}, 7},
	{NULL, {96,48,32,32}, {800 - 32 - 8, 264, 32, 32}, 8},
	{NULL, {128,48,32,32}, {800 - 32 - 8, 264, 32, 32}, 9},
	{NULL, {160,48,32,32}, {800 - 32 - 8, 264, 32, 32}, 10},
	{NULL, {192,48,32,32}, {800 - 32 - 8, 264, 32, 32}, 11},

	{NULL, {0, 80, 64, 80}, {234, 100, 300, 400}, 12}, //menu
	{NULL, {0, 176, 64, 80}, {234, 100, 300, 400}, 13}, //option
	{NULL, {64, 80, 16, 16}, {280, 200, 32, 32}, 14}, //pointer

	{UI_boss_health_update, {0,0,336,16}, {100,550,600,32}, 15}, //boss-health
	{NULL, {0,16,336,16}, {100,550,600,32}, 16}, //boss-maxhealth
	{NULL, {0,160,336,16}, {100,550,600,32}, 17}, //boss-maxhealthdecor
};

void init_UI(dynList* ui_el_list)
{
	dynList_add(ui_el_list, (void*)init_UI_el(-1, -1, UI_FULL_HEALTH));
	dynList_add(ui_el_list, (void*)init_UI_el(-1, -1, UI_CURRENT_HEALTH));
	dynList_add(ui_el_list, (void*)init_UI_el(-1, -1, UI_HEALTH_BORDER));

	dynList_add(ui_el_list, (void*)init_UI_el(-1, -1, UI_RUNE_SLOT_1));
	dynList_add(ui_el_list, (void*)init_UI_el(-1, -1, UI_RUNE_SLOT_2));
	dynList_add(ui_el_list, (void*)init_UI_el(-1, -1, UI_RUNE_SLOT_3));
}

struct UI_element *init_UI_el(int x, int y, UI_id type)
{
	struct UI_element* new = (struct UI_element*)malloc(sizeof(struct UI_element));
	*new = UI_sprite_info[type];
	if(x != -1 && y != -1)
	{
		new->dest.x = x;
		new->dest.y = y;
	}
	return new;
}

void UI_curr_health_update(struct UI_element* el, struct player* player, struct map* map)
{
	if((el->dest.w > 384 * player->health / player->maxhealth)) (el->dest.w) -= 2;
	if((el->dest.w < 384 * player->health / player->maxhealth)) (el->dest.w) += 2;
}

void UI_boss_health_update(struct UI_element* el, struct player* player, struct map* map)
{
	struct mObject* boss;
	for(int i = 0; i < map->mObject_list->size; i++)
	{
		boss = (struct mObject*)dynList_get(map->mObject_list, i);
		if(boss->id == 'o' || boss->id == 'c' || boss->id == 'q' || boss->id == 'v')
			break;
	}
	if((el->dest.w > 600 * boss->health / boss->max_health)) (el->dest.w) -= 4;
	
}

void UI_rune0_display_update(struct UI_element* el, struct player* player)
{
	struct rune* rune = (struct rune*)dynList_get(player->rune_list, 0);

}

void render_UI_elements(dynList* ui_el_list, struct player* player, struct map* map, SDL_Renderer *renderer, SDL_Texture* tex)
{
	for(int i = 0; i < ui_el_list->size; i++)
	{
		struct UI_element *curr = (struct UI_element*)dynList_get(ui_el_list, i);	
		if(curr->UI_update != NULL) 
		{
			curr->UI_update(curr, player, map);
		}
		
		SDL_RenderCopy(renderer, tex, &curr->sprite, &curr->dest);
	}
}

void render_pObject_deathrattle(SDL_Renderer *renderer, SDL_Texture* tex, SDL_Rect R, SDL_Rect r)
{
	SDL_RenderCopy(renderer, tex, &R, &r);
}

void render_player_animation(struct player *player, SDL_Rect dR, SDL_Renderer *renderer, SDL_Texture *tex)
{
	const double conv = 57.29577;
	bool flip = false;
	if(player->theta < PI/2 && player->theta > -1 * PI/2)
	{
		flip = true;
	}
	const int fill = 150;
	SDL_SetTextureColorMod(tex, fill, fill, fill);
	SDL_RenderCopyEx(renderer, tex, &player->sprite, &dR, 0, NULL, flip);
	if(player->anim.timer >= player->anim.limit)
	{
		player->anim.timer = 0;
		player->sprite.x += player->anim.tile_length;
		player->sprite.x %= player->anim.frames * player->anim.tile_length;
		player->sprite.x += player->anim.start_frame;
		return;
	}
	player->anim.timer ++;

}

void render_mObject_animation(struct mObject *mObject, SDL_Rect dR, SDL_Renderer *renderer, SDL_Texture *tex, struct player* player)
{
	const double conv = 57.29577;
	bool flip = false;
	double theta = mObject->anim.rotatable ? mObject->theta : 0;
	int fill = 100;
	SDL_SetTextureColorMod(tex, fill, fill, fill);

	int red = 0, green = 0, blue = 0;
	
	if(mObject->status_effect.type == STATUS_FROSTBITE)
	{
		blue = 75;
	}
	if(mObject->status_effect.type == STATUS_ROT)
	{
		green = 75;
	}
	double dist = 0;
	double dx = (mObject->x - player->x);
	double dy = (mObject->y - player->y);
	dist = dx * dx + dy * dy;
	dist = 255 - dist*8;
	dist = dist <= 100 ? 100 : dist;
	dist = dist >= 180 ? 180 : dist;
	const int def = 100;
	//SDL_SetTextureColorMod(tex, dist + red, dist + green, dist+ blue);
	SDL_SetTextureColorMod(tex, def + red, def + green, def + blue);
	if(mObject->id == 'J')
	{
		const int j = 255;
		SDL_SetTextureColorMod(tex, j, j, j);
	}

	if(mObject->theta < PI/2 && mObject->theta > -1 * PI/2)
	{
		flip = theta == 0 ? true : false;
	}
	SDL_RenderCopyEx(renderer, tex, &mObject->sprite, &dR, theta * conv, NULL, flip);
	if(mObject->anim.timer >= mObject->anim.limit)
	{
		mObject->anim.timer = 0;
		mObject->sprite.x += mObject->anim.tile_length;
		mObject->sprite.x %= mObject->anim.frames * mObject->anim.tile_length;
		mObject->sprite.x += mObject->anim.start_frame;
		//X coordinaten måste vara delbar med 4 eller första x coordinaten måste % vara 0
		return;
	}
	mObject->anim.timer ++;
}
bool cond = false;
void render_animation(struct pObject* pObject, SDL_Texture *tex, SDL_Rect dR, SDL_Renderer *renderer)
{
	const double conv = 57.29577;
	const Uint8* cks = SDL_GetKeyboardState(NULL);

	if(cks[SDL_SCANCODE_P])
	{
		cond = !cond;
		sleep(1);
		//return;
	}
	if(cond)
	{
		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(renderer, &dR);
	}
	SDL_RenderCopyEx(renderer, tex, &pObject->sprite, &dR, pObject->theta*conv, NULL, 0);
	const int def = 150;
	SDL_SetTextureColorMod(tex, def, def, def);
	if(pObject->anim_timer >= pObject->anim_limit)
	{
		pObject->anim_timer = 0;
		pObject->sprite.x += pObject->anim_tile_length;
		pObject->sprite.x %= pObject->anim_frames * pObject->anim_tile_length;
		pObject->sprite.x += pObject->anim_start_frame;
		return;
	}
	pObject->anim_timer ++;
}

void render_message(struct message* msg, struct cam* cam, SDL_Renderer *renderer, SDL_Texture *tex)
{
	if(msg->timer ++ >= msg->limit)
	{
		msg->complete = true;
	}
	for(double i = 0; i < msg->size; i++)
	{
		//SDL_Rect dest = {((msg->x + i / 2) - cam->offset_x) * TILE_LENGTH, (msg->y - cam->offset_y) * TILE_LENGTH, 16, 16};
		SDL_SetTextureColorMod(tex, msg->col.red, msg->col.green, msg->col.blue);
		SDL_Rect dest = {(msg->x + msg->font_size*i) * 16, msg->y * 16, msg->font_size * 16, msg->font_size * 16};
		SDL_RenderCopyEx(renderer, tex, &msg->s_chars[(int)i], &dest, 0.0, NULL, false);
	}
}

void render_UI_text(struct message* msg, SDL_Renderer *renderer, SDL_Texture *tex)
{
	for(double i = 0; i < msg->size; i++)
	{
		SDL_SetTextureColorMod(tex, msg->col.red, msg->col.green, msg->col.blue);
		SDL_Rect dest = {(msg->x + msg->font_size*i) * 16, msg->y * 16, msg->font_size * 16, msg->font_size * 16};
		SDL_RenderCopyEx(renderer, tex, &msg->s_chars[(int)i], &dest, 0.0, NULL, false);
	}
}

void run_reset_UI(struct map* map)
{
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_BLOOD));
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_FROST));
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_GRAVITY));
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_UNHOLY));
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_HOLY));
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_ROT));
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_ROT));
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_ROT));
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_ROT));
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_BOSS_BAR));
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_BOSS_FULL_BAR));
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_BOSS_DEC_BAR));
}

#define lol 17
void fade_out(SDL_Renderer* renderer, struct screen_manager* sm, struct map* map, struct player* player)
{
	if(sm->tone < 255)
	{
		sm->tone += lol;
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, sm->tone);
		SDL_RenderFillRect(renderer, NULL);
	}
	else
	{
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, sm->tone);
		SDL_RenderFillRect(renderer, NULL);
		map_load_scene(map, map->s_map.content[map->s_map.index++], map->mObject_list, player);
		map->state = ST_MAP_FADE_IN;
	}
}


void fade_in(SDL_Renderer* renderer, struct screen_manager *sm, struct map* map, struct player* player)
{
	if(sm->tone > 0)
	{
		sm->tone -= lol;
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, sm->tone);
		SDL_RenderFillRect(renderer, NULL);
	}
	else
	{
		map->state = ST_MAP_RUN_TICK;
		map_start_events(map, player);
	}
}

SDL_Rect init_sprite(int x, int y, int w, int h)
{
	SDL_Rect new = {x, y, w, h};
	return new;
}

void load_textures()
{
	return;
}

size_t UI_el_index(dynList* UI_el_list, int id)
{
	for(int i = 0; i < UI_el_list->size; i++)
	{
		struct UI_element* curr = dynList_get(UI_el_list, i);
		if(curr->id == id)
			return i;
	}
	return -1;
}

void init_menu(struct map* map)
{
	dynList_add(map->UI_el_list, (void*)init_UI_el(-1.0, -1.0, UI_MENU));
	dynList_add(map->UI_el_list, (void*)init_UI_el(-1.0, -1.0, UI_MENU_POINTER));
	//add_message(map->msg_list_UI, "MENU", 20.0, 8.0, 0, 2);
}

void menu_put(struct map* map)
{
	const Uint8* c = SDL_GetKeyboardState(NULL);
	if(c[SDL_SCANCODE_ESCAPE])
	{
		init_menu(map);
		map->state = ST_MAP_PAUSE;
	}
}

static int menu_index = 0;
static int menu_timer = 0;

void menu_exit(struct map* map)
{
	map->state = ST_MAP_RUN_TICK;
	menu_timer = 0;
	menu_index = 0;
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_MENU));
	dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_MENU_POINTER));

}

void option_requests(struct map* map, SDL_Window *window)
{
	if(map->sm.request_fullscreen)
	{
		map->sm.request_fullscreen = false;
		map->sm.fullscreen = !map->sm.fullscreen;
		SDL_SetWindowFullscreen(window, map->sm.fullscreen);
	}
}

void map_leave_run(struct map* map)
{
	add_event(map->event_list, TYPE_EVENT_END_RUN, NULL, map, 0);
	//clear_seed_map(map);
	//strcpy(map->s_map.content[map->s_map.index], "res/ch0_maps/ch0_hubmap.txt");
	menu_index = 0;
	menu_action(map);

}

static void (*mcp)(struct map* map) = menu_action;

void option_action(struct map* map)
{
	switch(menu_index)
	{
		case 0:
			menu_index = 0;
			dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_MENU_POINTER));
			dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_OPTION));
			dynList_add(map->UI_el_list, (void*)init_UI_el(-1.0, -1.0, UI_MENU));
			dynList_add(map->UI_el_list, (void*)init_UI_el(-1.0, -1.0, UI_MENU_POINTER));
			mcp = menu_action;
			break;
		case 1:
			map->sm.request_fullscreen = true;
			break;
	}
}

void menu_action(struct map* map)
{
	switch(menu_index)
	{
		case 0:
			menu_exit(map);
			break;
		case 1:
			menu_index = 0;
			dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_MENU));
			dynList_del_index(map->UI_el_list, UI_el_index(map->UI_el_list, UI_MENU_POINTER));
			dynList_add(map->UI_el_list, (void*)init_UI_el(-1.0, -1.0, UI_OPTION));
			dynList_add(map->UI_el_list, (void*)init_UI_el(-1.0, -1.0, UI_MENU_POINTER));
			mcp = option_action;
			break;
		case 2:
			map_leave_run(map);
			break;
		case 3:
			map->quit = true;
			break;
		
	}
}

void run_menu(struct map* map)
{
	struct UI_element* pointer = dynList_get(map->UI_el_list, UI_el_index(map->UI_el_list, UI_MENU_POINTER));
	pointer->dest.y = 1 * menu_index * 60 + 200;
	if(menu_timer ++ >= 8)
	{
		const Uint8* c = SDL_GetKeyboardState(NULL);
		if(c[SDL_SCANCODE_W] || c[SDL_SCANCODE_UP]) 
		{
			menu_index <= 0 ? menu_index = 3: menu_index --;
			menu_timer = 0;
			//printf("%d\n", menu_index);
		}
		if(c[SDL_SCANCODE_S] || c[SDL_SCANCODE_DOWN])
		{
			//menu_index ++;
			menu_index >= 3 ? menu_index = 0: menu_index ++ ;
			menu_timer = 0;
			//printf("%d\n", menu_index);
		}


		if(c[SDL_SCANCODE_RETURN])
		{
			mcp(map);
			menu_timer = 0;
		}
#if 0
		if(c[SDL_SCANCODE_ESCAPE])
		{
			menu_exit(map);
		};
#endif
	}

}

#if 0
SDL_Rect dest = {8, 8, 384, 16};
SDL_Rect src1 = {0, 16, 336, 16};
SDL_RenderCopy(renderer, tex, &src1, &dest);
SDL_Rect src = {0, 0, 336, 16};
struct UI_element *curr = (struct UI_element*)dynList_get(ui_el_list, 0);	
//printf("%d %d %d %d\n", curr->sprite.x, curr->sprite.y, curr->sprite.w, curr->sprite.h);
dest.w = curr->reduce;
if((curr->reduce > 384 * player->health / player->maxhealth))
	(curr->reduce) -= 2;

	//	SDL_RenderCopyEx(renderer, tex, &curr->sprite, &dest, 0, NULL, false);
	SDL_RenderCopy(renderer, tex, &src, &dest);
	dest.w = 400;
	src1.y += 16;
	dest.x -= 16;
	SDL_RenderCopy(renderer, tex, &src1, &dest);
#endif
