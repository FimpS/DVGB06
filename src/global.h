#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "dynList.h"
#include "objects.h"
#include "object_state.h"
//#include "event.h"
//#include "enum.h"
//


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PI 3.141592653689
#define TAU 6.283185307178

#define MIDPOINTX(o) (o->x + o->width/TILE_LENGTH/2)
#define MIDPOINTY(o) (o->y + o->height/TILE_LENGTH/2)

#define OBJDIFFX(o1, o2) (o1->x + o1->width/TILE_LENGTH/2) - (o2->x + o2->width/TILE_LENGTH/2)
#define OBJDIFFY(o1, o2) (o1->y + o1->height/TILE_LENGTH/2) - (o2->y + o2->height/TILE_LENGTH/2)


#define OBJDIFF(o1, o2, m) (m == 'X' ? ((o1->x + o1->width/TILE_LENGTH/2) - (o2->x + o2->width/TILE_LENGTH/2)) : ((o1->y + o1->height/TILE_LENGTH/2) - (o2->y + o2->height/TILE_LENGTH/2)))

#define SET_CAM_MID_CORDS(o1, o2) (o1.x = o2->x + o2->width/TILE_LENGTH/2, o1.y = o2->y + o2->height/TILE_LENGTH/2)


double get_frand();
int getTick();
void update_tick();
double sum_square(double, double);
double norm(double, double);

#endif
