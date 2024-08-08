#include <string.h>
#include "global.h"
#include "font.h"
#include "gfx.h"

static const char ch[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.!?=+-(); "};

size_t find_char(char c)
{
	int res = 0;
	while(ch[res ++] != c)
		if(ch[res] == '\0') return 64;
	
	return res - 1;
}

void print_res(const char* text, int* arr)
{
	size_t s = strlen(text);
	for(int i = 0; i < s; i++)
		printf("%c: %d\n", text[i], arr[i]);
}
void print_sprites(struct message msg)
{
	for(int i = 0; i < msg.size; i++)
	{
		printf("char: %c -> x:%d y:%d w:%d h:%d\n", msg.text[i], msg.s_chars[i].x, msg.s_chars[i].y, msg.s_chars[i].w, msg.s_chars[i].h);
	}
}
struct message init_message(double x, double y, int limit, const char *text, int* pos, int font_size)
{
	struct message new = {0};
	new.limit = limit;
	new.size = strlen(text);
	int tile_length_of_string = strlen(text) * 16 / TILE_LENGTH;
	new.x = x - ((strlen(text) / 2) * 0.0);
	const double d = strlen(text) / 2;
	new.x = x - (ceil((d + 1.0)) * 2.0);
	new.y = y;
	new.complete = false;
	new.font_size = font_size;
	new.col.red = 255;
	new.col.blue = 100;
	new.col.green = 100;
	strcpy(new.text, text);
	for(int i = 0; i < new.size; i++)
	{
		new.s_chars[i].x = pos[i];
		new.s_chars[i].y = 0;
		new.s_chars[i].w = 16;
		new.s_chars[i].h = 16;
	}
	return new;
}

void add_message(dynList *msg_list, const char* text, double x, double y, int duration, int font_size)
{
	const size_t s = strlen(text);
	int spos[s];
	for(int i = 0; i < s; i++)
	{
		spos[i] = find_char(text[i]) * 16;
	}
	struct message* msg = (struct message*)malloc(sizeof(struct message));
	*msg = init_message(x, y, duration, text, spos, font_size);
	dynList_add(msg_list, (void*)msg);
}

void render_messages(dynList* msg_list, struct cam* cam, SDL_Renderer *renderer, SDL_Texture *tex)
{
	if(dynList_is_empty(msg_list))
		return;
	for(int i = 0; i < msg_list->size; i++)
	{
		struct message* curr = (struct message*)dynList_get(msg_list, i);
		render_message(curr, cam, renderer, tex);
		if(curr->complete)
		{
			dynList_del_index(msg_list, i);
		}
	}
}

void render_UI_texts(dynList* msg_list_UI, SDL_Renderer *renderer, SDL_Texture *tex)
{
	if(dynList_is_empty(msg_list_UI))
		return;
	for(int i = 0; i < msg_list_UI->size;  i++)
	{
		struct message* curr = (struct message*)dynList_get(msg_list_UI, i);
		render_UI_text(curr, renderer, tex);
	}
}

