#include "global.h"

double get_frand(double end, double start)
{
	return ((double)rand()/(double)(RAND_MAX)) * end + start;
}

double sum_square(double x, double y)
{
	return x * x + y * y;
}

double norm(double top, double bot)
{
	return top/(sqrt(sum_square(top, bot)));
}

bool mouse_clicked(struct map* map)
{
	bool tmp = map->mouse_clicked;
	map->mouse_clicked = false;
	return tmp;
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
