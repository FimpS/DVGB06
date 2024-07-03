#ifndef MAP_HEADER
#define MAP_HEADER

#include <stdbool.h>
#include "dynList.h"
#include "enum.h"
#define MAP_WIDTH 64
#define MAP_HEIGHT 16
#define TILE_LENGTH 40
#define SEED_CHAPTER_AMOUNT 3
#define SEED_CHAPTER_SIZE 5
#define SEED_MAP_CHAR_MAX_LENGTH 128
#define CONTENT_SIZE 4096
#define MAP_CARD 6

struct seed_map
{
	char content[SEED_CHAPTER_AMOUNT * SEED_CHAPTER_SIZE + SEED_CHAPTER_AMOUNT][SEED_MAP_CHAR_MAX_LENGTH];
	int index;
};

struct vec2d
{
	double x;
	double y;	
};


struct m_anim_info
{
	int timer;
	int limit;
	int frame;
};

struct cam
{
	double x;
	double y;
	int vis_tile_x;
	int vis_tile_y;

	double shake_x;
	double shake_y;
	int shake_timer;
	int shake_limit;
	double shake_koef;

	bool move;

	int zoom;

	double step_x;
	double step_y;

	double offset_x;
	double offset_y;
	double tile_offset_x;
	double tile_offset_y;

	union
	{
		struct vec2d s_coord;
	} cin_info;
};

struct lightmap
{
	int content[CONTENT_SIZE];
	unsigned short red;
	unsigned short green;
	unsigned short blue;
};

struct map
{
	int width;
	int height;
	int aggresive_mObj_count;
	int current_chapter;
	struct lightmap lightmap;
	//int lightmap[CONTENT_SIZE];
	char content[CONTENT_SIZE];
	bool solid_content[CONTENT_SIZE];
	struct m_anim_info anim;
	struct seed_map s_map;
	struct cam cam;
	dynList *mObject_list;
	dynList *pObject_list;
	dynList *event_list;
	map_state state;
};

struct cam cam_init();
void set_screen_shake();
void cam_update();
void set_cam(struct cam *cam, bool t);
void control_cam_update();
void cam_move_to();
void screen_shake();

void map_get_coord(struct map* m, char key, int* dest);
void get_lightmap();
struct map map_init();
//void get_rand_mapID();
void get_chapterlight();
void map_load_scene();
void map_set_tile(struct map *m, int x, int y, char key);
bool map_get_solid();
char map_get_tile();
void map_spawn_enemies();
void map_update();
void map_draw();

#endif
