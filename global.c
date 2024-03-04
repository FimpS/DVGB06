#include "global.h"

double get_frand(double end, double start)
{
	return ((double)rand()/(double)(RAND_MAX)) * end + start;
}
int tick = 0;
int getTick()
{
	return tick;
}

void update_tick()
{
	tick++;
}
