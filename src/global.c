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

int tick = 0;
int getTick()
{
	return tick;
}

void update_tick()
{
	tick++;
}
