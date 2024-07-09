#ifndef FONT_H
#define FONT_H
#define INFO_CORD_X 14
#define INO_CORD_Y 4
#define MAX_TEXT_SIZE 32

struct message
{
	double x;
	double y;
	int timer;
	int limit;
	int size;
	bool complete;
	char text[MAX_TEXT_SIZE];
	SDL_Rect s_chars[MAX_TEXT_SIZE];

};

void add_message();
void render_messages();
#endif
