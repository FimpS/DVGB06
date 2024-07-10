#ifndef FONT_H
#define FONT_H
#define INFO_CORD_X 14
#define INO_CORD_Y 4
#define MAX_TEXT_SIZE 32

typedef struct color
{
	unsigned short red;
	unsigned short green;
	unsigned short blue;
} color;

struct message
{
	double x;
	double y;
	int timer;
	int limit;
	int size;
	int font_size;
	bool complete;
	bool tmp;
	char text[MAX_TEXT_SIZE];
	color col;
	SDL_Rect s_chars[MAX_TEXT_SIZE];

};

void add_message(dynList *msg_list, const char* text, double x, double y, int duration, int font_size);
size_t find_char(char c);
void render_messages();
void render_UI_texts();
#endif
