#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "gfx.h"
#include "map.h"
#include "objects.h"
#include "global.h"
#include "enum.h"
#include "event.h"
#include "runes.h"
#include "info.h"

#define BOSS_CUTSCENE_TIME 512

static int content_size = 0;

char map_get_tile(struct map *m, int x, int y)
{
	return x >= 0 && x < m->width && y >= 0 && y < m->height ? m->content[y*m->width + x] : ' ';
}


//finds coords and puts them in array dest[0] = x, dest[1] = y
void map_get_coord(struct map* m, char key, int *dest)
{
	for(int i = 0; i < content_size; i++)
		if(m->content[i] == key)
		{
			dest[1] = i/m->width;
			dest[0] = i - dest[1]*m->width;
		}
}

bool map_get_solid(struct map *m, int x, int y)
{
	return x >= 0 && x < m->width && y >= 0 && y < m->height ? m->solid_content[y*m->width + x] : false;
}

int map_get_light(struct map* m, int x, int y)
{
	return x >= 0 && x < m->width && y >= 0 && y < m->height ? m->lightmap.content[y*m->width + x] : 0;
}

void map_set_tile(struct map *m, int x, int y, char key)
{
	if(x >= 0 && x < m->width && y >= 0 && y < m->height)
		m->content[y * m->width + x] = key;
}

char* get_rand_mapID(char *fn, const char* chapter)
{
	int id = rand() % MAP_CARD + 1;
	char magic[16];
	sprintf(magic, "%d", id);

	strcpy(fn, "res/");
	strcat(fn, chapter);
	strcat(fn, "_maps/");
	strcat(fn, chapter);
	strcat(fn, "_");
	strcat(fn, magic);
	strcat(fn, ".txt");

	return fn;
}

char* get_rand_bossID(char *fn, const char* chapter)
{
	int id = rand() % 2 + 1;
	char magic[16];
	magic[0] = '0' + id;
	magic[1] = '\0';
	strcpy(fn, "res/");
	strcat(fn, chapter);
	strcat(fn, "_maps/");
	strcat(fn, chapter);
	strcat(fn, "_boss");
	strcat(fn, magic);
	strcat(fn, ".txt");
	return fn;
}

char* get_rand_itemmapID(char *fn, const char* chapter)
{
	int id = rand() % 2 + 1;
	char magic[16];
	magic[0] = '0' + id;
	magic[1] = '\0';
	strcpy(fn, "res/");
	strcat(fn, chapter);
	strcat(fn, "_maps/");
	strcat(fn, chapter);
	strcat(fn, "_item");
	strcat(fn, magic);
	strcat(fn, ".txt");
	return fn;
}

bool check_map_dupe(struct map* m, int stop)
{
	int i;
	for(i = 0; i < stop; i++)
	{
		if(!strcmp(m->s_map.content[stop], m->s_map.content[i]))
		{
			return true;
		}	
	}
	return false;
}

void gen_seed_map(struct map* m)
{
	size_t current = 0;
	for(int j = 0; j < SEED_CHAPTER_AMOUNT; j++)
	{
		char curr_chapter[32] = "ch", magic[4];
		magic[0] = '0' + j + 1;
		magic[1] = '\0';
		strcat(curr_chapter, (char*)magic);
		for(int i = 0; i < SEED_CHAPTER_SIZE; i++)
		{
			if(i == SEED_CHAPTER_SIZE / 2)
			{
				get_rand_itemmapID(m->s_map.content[current++], curr_chapter);
			}
			bool dupe = true;
			while(dupe)
			{
				//break somewhere to add itemrooms?
				get_rand_mapID(m->s_map.content[current], curr_chapter);
				if(!check_map_dupe(m, current))
				{
					dupe = false;	
				}
			}
			current++;
		}
		get_rand_bossID(m->s_map.content[current++], curr_chapter); //generate specific boss layer
	}
}

struct screen_manager sm_init()
{
	struct screen_manager new = {0};
	new.tone = 255;
	new.request_fullscreen = false;
	new.fullscreen = false;
	return new;
}
struct map map_init()
{
	struct map m = {0};

	memset(m.content, 0, CONTENT_SIZE);
	memset(m.solid_content, 0, CONTENT_SIZE);
	gen_seed_map(&m);
	for(int i = 0; i < SEED_CHAPTER_AMOUNT * SEED_CHAPTER_SIZE + SEED_CHAPTER_AMOUNT + SPECIAL_MAPS; i++)
		printf("%s\n", m.s_map.content[i]);
	m.s_map.index = 0;
	m.quit = false;
	m.width = MAP_WIDTH;
	m.height = MAP_HEIGHT;
	m.sm = sm_init();
	m.cam = cam_init();
	m.mObject_list = dynList_create();
	m.pObject_list = dynList_create();
	m.event_list = dynList_create();
	m.msg_list = dynList_create();
	m.msg_list_UI = dynList_create();
	m.UI_el_list = dynList_create();
	init_UI(m.UI_el_list);
	m.state = ST_MAP_RUN_TICK;
	m.aggresive_mObj_count = 0;
	m.anim.limit = 12;

	return m;
}

void map_run_init(dynList *maps)
{
	char fn[1024];
	for(int i = 0; i < MAP_CARD; i++)
	{
		struct map *new = (struct map*)malloc(sizeof(struct map));
		*new = map_init();
		//get_rand_mapID(fn, "ch1");
		memset(fn, 0, strlen(fn));
	}
}

struct cam cam_init()
{
	struct cam new_cam;
	new_cam.x = 0;
	new_cam.y = 0;
	new_cam.vis_tile_x = SCREEN_WIDTH / TILE_LENGTH;
	new_cam.vis_tile_y = SCREEN_HEIGHT / TILE_LENGTH;

	new_cam.shake_x = 0;
	new_cam.shake_y = 0;
	new_cam.shake_timer = 0;
	new_cam.shake_limit = 0;
	new_cam.shake_koef = 0.0;

	new_cam.zoom = 0;
	new_cam.step_x = 0;
	new_cam.step_y = 0;

	new_cam.move = false;

	new_cam.offset_x = 0;
	new_cam.offset_y = 0;

	new_cam.tile_offset_x = 0;
	new_cam.tile_offset_y = 0;

	return new_cam;
}

void spawn_mObjects(struct map *m, dynList *eList, struct player* player)
{
	for(int x = 0; x < m->width; x++)
		for(int y = 0; y < m->height; y++)
		{
			switch(m->content[x + y * m->width])
			{
				case 'p':
					player->x = x;
					player->y = y;
					m->content[x+y * m->width] = '.';
					break;
				case '1':
					spawn_mObject(m, x, y, MO_RUNNER, '1');
					m->content[x+y * m->width] = '.';
					break;
				case '2':
					spawn_mObject(m, x, y, MO_CRAWLER, '2');
					m->content[x+y * m->width] = '.';
					break;
				case '3':
					spawn_mObject(m, x, y, MO_RUSHER, '3');
					m->content[x+y * m->width] = '.';
					break;
				case 'l':
					spawn_mObject(m, x, y, MO_PEAK_LOCAL, 'l');
					m->content[x+y * m->width] = '.';
					break;
				case 'f':
					spawn_mObject(m, x, y, MO_PEAK_KNIGHT, 'f');
					m->content[x+y * m->width] = '.';
					break;
				case 'w':
					spawn_mObject(m, x, y, MO_ROCK_WELL, 'w');
					m->content[x+y * m->width] = '.';
					break;
				case 'r':
					spawn_mObject(m, x, y, MO_ROCK_ROLLER, 'r');
					m->content[x+y * m->width] = '.';
					break;
				case '4':
					spawn_mObject(m, x, y, MO_ARCHER, '4');
					m->content[x+y * m->width] = '.';
					break;
				case '5':
					spawn_mObject(m, x, y, MO_SWORDSMAN, '5');
					m->content[x+y * m->width] = '.';
					break;
				case '6':
					spawn_mObject(m, x, y, MO_MAGUS, '6');
					m->content[x+y * m->width] = '.';
					break;
				case '7':
					spawn_mObject(m, x, y, MO_SUMMONER, '7');
					m->content[x+y * m->width] = '.';
					break;
				case '8':
					spawn_mObject(m, x, y, MO_RIDER_FIGHTER, '8');
					m->content[x+y * m->width] = '.';
					break;
				case '9':
					spawn_mObject(m, x, y, MO_DRIDER_FIGHTER, '9');
					m->content[x+y * m->width] = '.';
					break;
				case 't':
					spawn_mObject(m, x, y, MO_FIRE_TOWER, 't');
					m->content[x+y * m->width] = '.';
					break;
				case 's':
					spawn_mObject(m, x, y, MO_FIRE_BOMBER, 's');
					m->content[x+y * m->width] = '.';
					break;
				case 'a':
					spawn_mObject(m, x, y, MO_FIRE_ARCHER, 'a');
					m->content[x+y * m->width] = '.';
					break;
				case 'B':
					spawn_mObject(m, x, y, MO_BALISTA, 'B');
					m->content[x+y * m->width] = '.';
					break;
				case 'E':
					spawn_mObject(m, x, y, MO_INTERACTABLE, 'E');
					m->content[x+y * m->width] = 'E';
					break;
				case 'D':
					spawn_mObject(m, x, y, MO_INTERACTABLE, 'D');
					m->content[x+y * m->width] = 'D';
					break;
				case 'T':
					spawn_mObject(m, x, y, MO_INTERACTABLE, 'T');
					m->content[x+y * m->width] = 'T';
					break;
				case 'g':
					spawn_mObject(m, x, y, MO_RUSHER, 'g');
					m->content[x+y * m->width] = '.';
					break;
				case 'v':
					spawn_mObject(m, x, y, MO_ROCK_VORTEX, 'v');
					m->content[x+y * m->width] = '.';
					break;
				case 'q':
					spawn_mObject(m, x, y, MO_LOCAL_QUEEN, 'q');
					m->content[x+y * m->width] = '.';
					break;
				case 'c':
					spawn_mObject(m, x, y, MO_CULTIST_CHIEFTAIN, 'c');
					m->content[x+y * m->width] = '.';
					break;
				case 'o':
					spawn_mObject(m, x, y, MO_GOLEM, 'o');
					m->content[x+y * m->width] = '.';
					break;
				case 'R':
					spawn_mObject(m, x, y, MO_RUNE_SHARD, 'R'); //disable when real
					m->content[x+y * m->width] = '.';
					break;
			}

		}

}

void init_killcount(struct map *m)
{
	for(int i = 0; i < m->mObject_list->size; i++)
	{
		struct mObject* tmp = (struct mObject*)dynList_get(m->mObject_list, i);
		if(tmp->killable == true)
		{
			m->aggresive_mObj_count ++;
		}
	}
	printf("killables: %d\n", m->aggresive_mObj_count);
}

void reset_player(struct player *player)
{
	if(!dynList_is_empty(player->rune_list))
	{
		struct rune* rune = (struct rune*)dynList_get(player->rune_list, 0);
		if(rune != NULL && rune->info.rune_type == RN_HOLY)
		{
			rune->attribute = true;
		}
	}
}

bool check_rune_dupe(struct rune_info *map_runes, int stop)
{
	int i;
	for(i = 0; i < stop; i++)
	{
		if(map_runes[stop].rune_type == map_runes[i].rune_type)
				return true;
	}
	return false;
}

void get_rune_coords(struct map* m, int *coords)
{
	int index = 0;
	//y1,x1,y2,x2,y3,x3
	for(int i = 0; i < m->width * m->height; i++)
	{
		if(m->content[i] == 'R')
		{
			coords[index++] = i/m->width;
			coords[index++] = i - coords[index - 1] * m->width;
		}
	}
}

bool check_rune_room(struct map *m)
{
	for(int i = 0; i < m->width * m->height; i++)
		if(m->content[i] == 'R')
			return true;
	return false;
}


void spawn_runes(struct map* m, struct rune_info *map_runes)
{

	if(!check_rune_room(m))
		return;
	size_t current = 0;
	for(int j = 0; j < 3; j++)
	{
		bool dupe = true;
		int res = 0;
		while(dupe)
		{
			map_runes[current].rune_type = get_rand_rune_type();
			//rune_type = blood, curr = 0
			//rune_tpye = blood, curr = 1
			if(!check_rune_dupe(map_runes, current))
			{
				dupe = false;
			}
		}
		current ++;
	}
	int coords[6];
	get_rune_coords(m, coords);
	for(int i = 0; i < 3; i++)
	{
		//cant run init here so we do it manually
		struct mObject *new = (struct mObject*)malloc(sizeof(struct mObject));
		new->x = coords[2*i + 1];
		new->y = coords[2*i];
		new->type = MO_RUNE_SHARD;
		new->id = 'R';
		new->width = TILE_LENGTH * 2;
		new->height = TILE_LENGTH * 2;
		new->hittable = false;
		new->killable = false;
		new->st.type = st_placeholder;
		new->st.acp = NULL;
		new->r_info = map_runes[i];
		dynList_add(m->mObject_list, (void*)new);
	}
}

int hash_map_name(const char *map_name)
{
	int fuck_you = 0;
	while(*map_name != '\0')
	{
		fuck_you +=  101 * *map_name++ + 40;
		//*map_name ++;
	}
	return fuck_you;
}

void map_start_events(struct map *m, struct player *player)
{
	//tmp solutions maybe an & check for this
		printf("m->c %s\nhash %d\n", m->s_map.content[m->s_map.index], hash_map_name(m->s_map.content[m->s_map.index]));
	switch(hash_map_name(m->s_map.content[m->s_map.index]))
	{
		case 247177:
		case 247480:
		case 247379:
		case 247278:
			dynList_add(m->UI_el_list, (void*)init_UI_el(-1, -1, UI_BOSS_FULL_BAR));
			dynList_add(m->UI_el_list, (void*)init_UI_el(-1, -1, UI_BOSS_BAR));
			dynList_add(m->UI_el_list, (void*)init_UI_el(-1, -1, UI_BOSS_DEC_BAR));
			break;
	}

	switch(hash_map_name(m->s_map.content[m->s_map.index]))
	{
		case 247278:
			add_event(m->event_list, TYPE_EVENT_VORTEX, player, m, BOSS_CUTSCENE_TIME / 2);
			break;
		case 247177:
			add_event(m->event_list, TYPE_EVENT_QUEEN, player, m, BOSS_CUTSCENE_TIME / 2);
			break;
		case 247480:
			add_event(m->event_list, TYPE_EVENT_GOLEM, player, m, BOSS_CUTSCENE_TIME);
			break;
		case 247379:
			add_event(m->event_list, TYPE_EVENT_CHIEFTAIN, player, m, BOSS_CUTSCENE_TIME / 2);
			break;
		default:
			return;
			break;
	}
}

static const char solids[] = {"# MWS!P-%|C+-*/_{[]}H"};

bool solid_chars(char match)
{
	for(int i = 0; solids[i]; i++)
		if(solids[i] == match)
			return true;
	return false;
}

void load_map_file(struct map *m, char* filename)
{
	FILE *f;
	char file[64];
	printf("file: %s\n", filename);
	m->current_chapter = (int)filename[6] - '0';
	f = fopen(filename, "r");
	if(!f)
	{
		printf("Could Not Open File/File does not exist\n");
		printf("filename %s\n", filename);
		return;
	}
	int i = 0;
	fscanf(f, "%d %d\n", &m->width, &m->height);
	content_size = m->width * m->height;
	while(!feof(f))
	{
		fscanf(f, "%c", &m->content[i]);
		if(m->content[i] == '\n')
			m->content[i--] = '\0';
		i++;
		//printf("%c", m->content[i++]);
	}
	for(int i = 0; i < content_size; i++)
	{
		if(solid_chars(m->content[i]))
		{
			m->solid_content[i] = true;
		}
		else
		{
			m->solid_content[i] = false;
		}
	}
	fclose(f);
}

void replace_boss(struct map* m)
{
	for(int i = 0; i < m->mObject_list->size; i++)
	{
		struct mObject* boss = dynList_get(m->mObject_list, i);
		switch(boss->id)
		{
			case 'o':
				spawn_mObject(m, boss->x, boss->y, MO_GOLEM, boss->id);
				m->aggresive_mObj_count--;
				dynList_del_index(m->mObject_list, i);
				break;
			default:
				;
				break;
		}
	}
}

void map_load_scene(struct map *m, char *filename, dynList* eList, struct player *player)
{
	dynList_clear(m->mObject_list);
	dynList_clear(m->pObject_list);
	//m->sm.tone = 255;
	m->aggresive_mObj_count = 0;
	load_map_file(m, filename);
	strcpy(m->map_name, filename);
	struct rune_info map_runes[3];
	m->state = ST_MAP_FADE_IN;
	get_lightmap(m, m->cam);
	get_chapterlight(m);
	//spawn_runes(m, map_runes); enable when real
	spawn_mObjects(m, eList, player);
	//replace_boss(m); what is this function??
	reset_player(player);
	//printf("%d\n", m->aggresive_mObj_count);
	//printf("%d\n", m->mObject_list->si
	cam_update(&m->cam, m, player);
	//map_start_events(m, player);
	printf("chap:%d\n", m->current_chapter);
}

void default_screen_shake(struct map *m)
{
	m->cam.shake_x = 0;
	m->cam.shake_y = 0;
	m->cam.shake_limit = 0;
	m->cam.shake_koef = 0;
}

void set_screen_shake(struct map *m, const double shake_koef, const int limit)
{
	m->cam.shake_timer = 0;
	m->cam.shake_limit = limit;
	m->cam.shake_koef = shake_koef;
}

void screen_shake(struct map *m)
{
#if 1
	if(getTick() % 4 != 0)
		return;
	//cool effect
#endif 
	//tmp sol should implement a cam_state
	if(m->cam.shake_timer >= m->cam.shake_limit / 4)
	{
		m->cam.shake_timer = 0;
		default_screen_shake(m);	
		return;
	}
	m->cam.shake_timer ++;
	m->cam.shake_x = get_frand(m->cam.shake_koef, -0.5 * m->cam.shake_koef);
	m->cam.shake_y = get_frand(m->cam.shake_koef, -0.5 * m->cam.shake_koef);
}

void set_cam(struct cam *cam, bool t)
{
	cam->move = t;
}

void cam_move_to(struct cam *cam, double dsttheta)
{
	if(!cam->move)
		return;
	cam->shake_x += 0.05;
	//cam->shake_y += 0.05;
}

void control_cam_update(struct cam* cam, struct map *map, struct player *player)
{
	cam->vis_tile_x = SCREEN_WIDTH / (TILE_LENGTH);
	cam->vis_tile_y = SCREEN_HEIGHT / (TILE_LENGTH);

	cam->offset_x = cam->x - (double)cam->vis_tile_x / 2.0;
	cam->offset_y = cam->y - (double)cam->vis_tile_y / 2.0;

	if(cam->offset_x < cam->shake_x) cam->offset_x = cam->shake_x;
	if(cam->offset_y < cam->shake_y) cam->offset_y = cam->shake_y;
	if(cam->offset_x - cam->shake_x > map->width - cam->vis_tile_x) cam->offset_x = map->width - cam->vis_tile_x + cam->shake_x;
	if(cam->offset_y - cam->shake_y > map->height - cam->vis_tile_y) cam->offset_y = map->height - cam->vis_tile_y + cam->shake_y;

	cam->tile_offset_x = (cam->offset_x - (int)(cam->offset_x)) * TILE_LENGTH;
	cam->tile_offset_y = (cam->offset_y - (int)(cam->offset_y)) * TILE_LENGTH;

}

void cam_update(struct cam *cam, struct map *map, struct player *player)
{

	screen_shake(map);

	cam->x = player->x + cam->shake_x;
	cam->y = player->y + cam->shake_y; //LRU

	cam->vis_tile_x = SCREEN_WIDTH / (TILE_LENGTH);
	cam->vis_tile_y = SCREEN_HEIGHT / (TILE_LENGTH);

	cam->offset_x = cam->x - (double)cam->vis_tile_x / 2.0;
	cam->offset_y = cam->y - (double)cam->vis_tile_y / 2.0;

	if(cam->offset_x < cam->shake_x) cam->offset_x = cam->shake_x;
	if(cam->offset_y < cam->shake_y) cam->offset_y = cam->shake_y;
	if(cam->offset_x - cam->shake_x > map->width - cam->vis_tile_x) cam->offset_x = map->width - cam->vis_tile_x + cam->shake_x;
	if(cam->offset_y - cam->shake_y > map->height - cam->vis_tile_y) cam->offset_y = map->height - cam->vis_tile_y + cam->shake_y;

	cam->tile_offset_x = (cam->offset_x - (int)(cam->offset_x)) * TILE_LENGTH;
	cam->tile_offset_y = (cam->offset_y - (int)(cam->offset_y)) * TILE_LENGTH;
}

static int tmptimer = 0;

void map_update(struct map *map, struct player *player, dynList *event_list)
{
	const Uint8 *kinp = SDL_GetKeyboardState(NULL);
	if(kinp[SDL_SCANCODE_O] && tmptimer >= 16)
	{
		add_event(event_list, type_event_teleport);
		tmptimer = 0;
	}
	tmptimer++;
}

void map_switch_frame(struct map* map, struct cam *cam)
{
	if(map->anim.timer >= map->anim.limit)
	{
		map->anim.frame += 16;
		map->anim.frame %= 64;
		map->anim.timer = 0;
	}
	map->anim.timer ++;
}

bool is_valid(int *m, int x, int y, int x1, int y1, int w, int h)
{
	//printf("x1:%d y1:%d\n", x1, y1);
	//printf("m*%d\n", m[x1 + y1 * w]);
	return x1 >= 0 && y1 >= 0 /*&& x1 / 10 == x / 10 && y / 10 == y1 / 10 */&& m[x1 + y1 * w] != 3 && m[x1 + y1 * w] != 2;
	//care
}

void populate(int *m, int x, int y, int w, int h)
{
	if(is_valid(m, x, y, x + 1, y, w, h)) m[(x + 1) + (y)* w] = 2;
	if(is_valid(m, x, y, x - 1, y, w, h)) m[(x - 1) + (y)* w] = 2;
	if(is_valid(m, x, y, x, y + 1, w, h)) m[(x) + (y + 1)* w] = 2;
	if(is_valid(m, x, y, x, y - 1, w, h)) m[(x) + (y - 1)* w] = 2;


	if(is_valid(m, x, y, x + 1, y + 1, w, h)) m[(x + 1) + (y + 1)* w] = 1;
	if(is_valid(m, x, y, x - 1, y - 1, w, h)) m[(x - 1) + (y - 1)* w] = 1;
	if(is_valid(m, x, y, x + 1, y - 1, w, h)) m[(x + 1) + (y - 1)* w] = 1;
	if(is_valid(m, x, y, x - 1, y + 1, w, h)) m[(x - 1) + (y + 1)* w] = 1;

	if(is_valid(m, x, y, x + 2, y, w, h)) m[(x + 2) + (y)* w] = 1;
	if(is_valid(m, x, y, x - 2, y, w, h)) m[(x - 2) + (y)* w] = 1;
	if(is_valid(m, x, y, x, y + 2, w, h)) m[(x) + (y + 2)* w] = 1;
	if(is_valid(m, x, y, x, y - 2, w, h)) m[(x) + (y - 2)* w] = 1;
}

void make_lightmap(int *m, int w, int h)
{

	for(int i = 0; i < w; i++)
	{
		for(int j = 0; j < h; j++)
			if(m[i + j * w] == 3)
			{
				populate(m, i, j, w, h);
			}
	}
}

void get_chapterlight(struct map* map)
{
#if 1
	switch(map->current_chapter)
	{
		case 1:
			map->lightmap.red = 0;
			map->lightmap.green = 1;
			map->lightmap.blue= 0;
			break;
		case 2:
			map->lightmap.red = 0;
			map->lightmap.green = 1;
			map->lightmap.blue= 1;
			break;
		case 3:
			map->lightmap.red = 1;
			map->lightmap.green = 0.35;
			map->lightmap.blue= 0.35;
			break;
	}
#endif
}

void get_lightmap(struct map* map, struct cam *cam)
{
	for(int i = 0; i < CONTENT_SIZE; i++)
	{
		map->lightmap.content[i] = 0;
	}
	for(int i = 0; i < CONTENT_SIZE; i++)
	{
		switch(map->content[i])
		{
			case 'L':
				map->lightmap.content[i] = 3;
				break;
			case 'S':
				map->lightmap.content[i] = 3;
				break;
			case 'A':
				map->lightmap.content[i] = 3;
				break;
			case 'C':
				map->lightmap.content[i] = 3;
				break;
			case 't':
				map->lightmap.content[i] = 3;
				break;
		}
	}
	make_lightmap(map->lightmap.content, map->width, map->height);
#if 0
	for(int i = 0; i < map->height; i++)
	{
		for(int j = 0; j < map->width; j++)
		{
			printf("%d ", map->lightmap.content[j +  i * map->width]);
		}
		printf("\n");
	}
#endif
}

void player_lighting(struct map* map, int i, int j, struct cam *cam, SDL_Texture *tex, struct player* player)
{
	double dist = 0;
	double dx = ((i + (int)cam->offset_x) - player->x);
	double dy = ((j + (int)cam->offset_y) - player->y);
	dist = dx * dx + dy * dy;
	dist = 255 - dist*12;
	//dist += rand() % 8;
	dist = dist <= 100 ? 100 : dist;
	dist = dist >= 160 ? 160 : dist;
	SDL_SetTextureColorMod(tex, dist, dist, dist);
}


static const SDL_Rect tile_info[] = 
{
	['.'] = {0, 32, 16, 16},
	['#'] = {16, 32, 16, 16},
	['H'] = {32, 0, 16, 16},
	['S'] = {0, 128, 16, 16},
	['M'] = {48, 0, 16, 16},
	['A'] = {0, 144, 16, 16},
	['C'] = {0, 160, 16, 16},
	['<'] = {64, 112, 16, 16},
	['>'] = {80, 112, 16, 16},
	['_'] = {80, 48, 16, 16},
	['|'] = {48, 48, 16, 16},
	['%'] = {64, 48, 16, 16},
	['+'] = {80, 16, 16, 16},
	['-'] = {96, 16, 16, 16},
	['*'] = {80, 32, 16, 16},
	['/'] = {96, 32, 16, 16},
	['{'] = {64, 64, 16, 16},
	['['] = {48, 64, 16, 16},
	[']'] = {48, 80, 16, 16},
	['}'] = {64, 80, 16, 16},
	['L'] = {0, 112, 16, 16},
	['W'] = {0, 96, 16, 16},
	['G'] = {48, 32, 16, 16},
	['P'] = {32, 32, 16, 16},
	['F'] = {0, 64, 16, 16},
	['^'] = {64, 16, 16, 16},
	['V'] = {64, 32, 16, 16},
	['E'] = {128, 128, 16, 16},
	['T'] = {128, 128, 16, 16},
	['D'] = {128, 128, 16, 16},
	['!'] = {128, 128, 16, 16},
	[' '] = {128, 128, 16, 16},
};

static char tile_animations[] = {"LWSAC"};

bool tile_anim_req(char tile)
{
	for(int i = 0; tile_animations[i]; i++)
		if(tile == tile_animations[i])
			return true;
	return false;
}

void map_draw(struct map *map, struct cam *cam, SDL_Renderer *renderer, SDL_Texture *tex, struct player* player)
{
	//SDL_Surface *sur = IMG_Load("assets/Untitled.png");
	//SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, sur);
	//tmp stuff
	//
	map_switch_frame(map, cam);
	double dx = 0;
	double dy = 0;
	double dist = 0;
	for(int i = -1; i < cam->vis_tile_x + 1; i++)
	{
		for(int j = -1; j < cam->vis_tile_y + 1; j++)
		{
			char tile = map_get_tile(map, i + (int)cam->offset_x, j + (int)cam->offset_y);	
			SDL_Rect r_tile = {i * TILE_LENGTH - cam->tile_offset_x, j * TILE_LENGTH - cam->tile_offset_y, TILE_LENGTH, TILE_LENGTH};

			const int light = map_get_light(map, i + (int)cam->offset_x, j + (int)cam->offset_y) * 40 + rand() % 10;
			const int defR = 120 * map->lightmap.red;
			const int defG = 120 * map->lightmap.green;
			const int defB = 120 * map->lightmap.blue;

			SDL_SetTextureColorMod(tex, 0 + defR + light * map->lightmap.red, defG + light * map->lightmap.green, defB + light * map->lightmap.blue);
			//SDL_Rect R = {0, 0, 16, 16};
			SDL_Rect R = tile_info[tile];
			if(tile_anim_req(tile))
			{
				R.x = map->anim.frame;
			}
			SDL_RenderCopy(renderer, tex, &R, &r_tile);
		}
	}
}

#if 0
#if 0
switch(tile)
{
	case '.':
		//player_lighting(map, i, j, cam, tex, player);
		R = tile_info['.'];
		//R.y += 32;
		//R.x += 0;
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case '_':
		R = tile_info[tile];
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case '|':
		R.x += 48;
		R.y += 48;
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case '%':
		R.x += 64;
		R.y += 48;
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case '+':
		R.x += 80;
		R.y += 16;
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case '-':
		R.x += 96;
		R.y += 16;
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case '*':
		R.x += 80;
		R.y += 32;
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case '/':
		R.x += 96;
		R.y += 32;
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case '#':
		R.y += 32;
		R.x += 16;
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case 'L':
		//SDL_SetTextureColorMod(tex, 200, 200, 200);
		R.y += 48;
		R.x = 16 * (getTick() % 16 >= 8 && getTick() % 16 < 16 ? 1 : 0);
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case 'G':
		R.y += 32;
		R.x += 48;
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case 'W':
		R.x += 32;
		R.y += 48;
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case 'P':
		R.x += 32;
		R.y += 32;
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case '^':
		R.x += 64;
		R.y += 16;
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case 'V':
		R.x += 64;
		R.y += 32;
		SDL_RenderCopy(renderer, tex, &R, &r_tile);
		break;
	case 'E':
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(renderer, &r_tile);
		break;
	case 'T':
		//SDL_RenderCopy(renderer, tex, &img_tile4, &r_tile);
		break;
	case 'D':
		//SDL_RenderCopy(renderer, tex, &img_tile4, &r_tile);
		break;
	case '!':
	case ' ':
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(renderer, &r_tile);
		break;
}
//printf("d: %f\n", dist);
#endif
}
}
#endif

