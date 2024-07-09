#include <string.h>
#include "global.h"
#include "font.h"
#include "gfx.h"

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
struct message init_message(double x, double y, int limit, const char *text, int* pos)
{
	struct message new = {0};
	new.limit = limit;
	new.size = strlen(text);
	new.x = x - strlen(text) / 4;
	new.y = y;
	new.complete = false;
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

void add_message(dynList *msg_list, const char* text, double x, double y, int duration)
{
	const size_t s = strlen(text);
	int spos[s];
	for(int i = 0; i < s; i++)
	{
		spos[i] = (text[i] - 65) * 16;	
		if(spos[i] < 0)
			spos[i] = 520;
	}
	print_res(text, spos);
	struct message* msg = (struct message*)malloc(sizeof(struct message));
	*msg = init_message(x, y, duration, text, spos);
	print_sprites(*msg);
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

