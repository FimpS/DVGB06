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

double get_frand();
int getTick();
void update_tick();

#endif
