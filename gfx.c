#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "objects.h"
#include "global.h"
#include "font.h"
#include "gfx.h"

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


#define DD 32
const struct UI_element UI_sprite_info[] = {
	{UI_curr_health_update, {0,0,336,16}, {8,8,384,16}, 0},
	{NULL, {0,16,336,16}, {8,8,384,16}, 1},
	{NULL, {0,32,384,16}, {-8, 8, 428, 16}, 2},
	{NULL, {0,48,32,32}, {800 - 32 - 8, 40, 24, 24}, 3},
	{NULL, {0,48,32,32}, {800 - 32 - 8, 72 + 12, 24, 24}, 4},
	{NULL, {0,48,32,32}, {800 - 32 - 8, 104 + 24, 24, 24}, 5},

	{NULL, {32,48,32,32}, {800 - 32 - 8, 264, DD, DD}, 6},
	{NULL, {64,48,32,32}, {800 - 32 - 8, 264, DD, DD}, 7},
	{NULL, {96,48,32,32}, {800 - 32 - 8, 264, DD, DD}, 8},
	{NULL, {128,48,32,32}, {800 - 32 - 8, 264, DD, DD}, 9},
	{NULL, {160,48,32,32}, {800 - 32 - 8, 264, DD, DD}, 10},
	{NULL, {192,48,32,32}, {800 - 32 - 8, 264, DD, DD}, 11},

	{NULL, {0, 80, 64, 80}, {234, 100, 300, 400}, 12},
	{NULL, {64, 80, 16, 16}, {280, 100, 32, 32}, 13},
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
	printf("%d %d\n", new->dest.x, new->dest.w + new->dest.x);
	return new;
}

void UI_curr_health_update(struct UI_element* el, struct player* player)
{
	if((el->dest.w > 384 * player->health / player->maxhealth)) (el->dest.w) -= 2;
}

void UI_rune0_display_update(struct UI_element* el, struct player* player)
{
	struct rune* rune = (struct rune*)dynList_get(player->rune_list, 0);

}

void render_UI_elements(dynList* ui_el_list, struct player* player, SDL_Renderer *renderer, SDL_Texture* tex)
{
	for(int i = 0; i < ui_el_list->size; i++)
	{
		struct UI_element *curr = (struct UI_element*)dynList_get(ui_el_list, i);	
		if(curr->UI_update != NULL) 
		{
			curr->UI_update(curr, player);
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
	int fill = 10;
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

	//SDL_SetTextureColorMod(tex, dist + red, dist + green, dist+ blue);
	SDL_SetTextureColorMod(tex, 150 + red, 150 + green, 150 + blue);

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
		SDL_Rect dest = {((msg->x + i / 2) - cam->offset_x) * TILE_LENGTH, (msg->y - cam->offset_y) * TILE_LENGTH, 16, 16};
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

void process_symmetric_animation(SDL_Renderer *renderer, SDL_Texture *tex, SDL_Rect sR, SDL_Rect dR, struct pObject *pObject)
{
	const double conv = 57.29577;

	if(pObject->anim_timer < 4)
	{
		SDL_RenderCopyEx(renderer, tex, &sR, &dR, pObject->theta*conv, NULL, 0);
	}
	else if(pObject->anim_timer >= 4 && pObject->anim_timer < 8)
	{
		sR.x += 16;
		SDL_RenderCopyEx(renderer, tex, &sR, &dR, pObject->theta*conv, NULL, 0);
	}
	else if(pObject->anim_timer >= 8 && pObject->anim_timer < 12)
	{
		sR.x += 32;
		SDL_RenderCopyEx(renderer, tex, &sR, &dR, pObject->theta*conv, NULL, 0);
	}
	else if(pObject->anim_timer >= 12 && pObject->anim_timer < 16)
	{
		sR.x += 48;
		SDL_RenderCopyEx(renderer, tex, &sR, &dR, pObject->theta*conv, NULL, 0);
	}
	else if(pObject->anim_timer >= 16)
	{
		SDL_RenderCopyEx(renderer, tex, &sR, &dR, pObject->theta*conv, NULL, 0);
		pObject->anim_timer = -1;	
	}
	pObject->anim_timer ++;
}

void render_hpbar(SDL_Renderer *renderer, struct player* player, struct cam *cam, double *reduce)
{
	//TODO good enough for now but should be altererd prob
	//Prob health should be integer so +1/-1 will fix everything
#if 0
	for(int i = 0; i < 6; i++)
	{
		if((*reduce <= 320 * player->health / player->maxhealth))
		{
			(*reduce) += 1.0;
		}
	}
	for(int i = 0; i < 6; i++)
	{
		if((*reduce > 320 * player->health / player->maxhealth))
			(*reduce) -= 1.0;
	}


	SDL_Rect curr_health = {(0.5) * TILE_LENGTH, (0.5) * TILE_LENGTH, *reduce, TILE_LENGTH / 2};
	SDL_Rect full_health = {0.5 * TILE_LENGTH, 0.5 * TILE_LENGTH, 8 * TILE_LENGTH, TILE_LENGTH / 2};
	SDL_Rect surround = {0.25 * TILE_LENGTH, 0.35 * TILE_LENGTH, 10.75 * 32, 32};

	SDL_SetRenderDrawColor(renderer, 0x60, 0x60, 0x60, 0xFF);
	SDL_RenderFillRect(renderer, &surround);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &full_health);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &curr_health);
#endif 
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

void menu_action(struct map* map)
{
	switch(menu_index)
	{
		case 0:
			menu_exit(map);
			break;
		case 1:
			printf("leave\n"); //TODO
			break;
		case 2:
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
			menu_index <= 0 ? menu_index : menu_index --;
			menu_timer = 0;
			printf("menu_index: %d\n", menu_index);
		}
		if(c[SDL_SCANCODE_S] || c[SDL_SCANCODE_DOWN])
		{
			//menu_index ++;
			menu_index >= 2 ? menu_index : menu_index ++ ;
			menu_timer = 0;
			printf("menu_index: %d\n", menu_index);
		}


		if(c[SDL_SCANCODE_RETURN])
		{
			menu_action(map);
			menu_timer = 0;
		}
#if 0
		if(c[SDL_SCANCODE_ESCAPE])
		{
			menu_exit(map);
		};
#endif
		//printf("menu_index: %d\n", menu_index);
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
