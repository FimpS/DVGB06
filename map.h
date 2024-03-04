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

struct map
{
	int width;
	int height;
	int aggresive_mObj_count;
	int current_chapter;
	char content[CONTENT_SIZE];
	bool solid_content[CONTENT_SIZE];
	struct seed_map s_map;
	dynList *mObject_list;
	dynList *pObject_list;
	dynList *event_list;
	map_state state;
};

struct cam
{
	double x;
	double y;
	int vis_tile_x;
	int vis_tile_y;
	double offset_x;
	double offset_y;
	double tile_offset_x;
	double tile_offset_y;
};


struct cam cam_init();
void cam_update();

void map_get_coord(struct map* m, char key, int* dest);

struct map map_init();
//void get_rand_mapID();
void map_load_scene();
void map_set_tile(struct map *m, int x, int y, char key);
bool map_get_solid();
char map_get_tile();
void map_spawn_enemies();
void map_update();
void map_draw();

#endif
