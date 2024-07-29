#include "objects.h"


struct UI_element
{
	void (*UI_update)(struct UI_element*, struct player* player, struct map* map);
	SDL_Rect sprite;
	SDL_Rect dest;
	UI_id id;
};
struct UI_element *init_UI_el();
void init_UI();
SDL_Rect init_sprite();
void render_UI_elements();
void UI_curr_health_update();
void UI_boss_health_update();
size_t UI_el_index();
void run_reset_UI();

void option_requests();
void menu_action();
void menu_put();
void run_menu();

void fade_out();
void fade_in();

void render_animation();
void render_player_animation();
void render_mObject_animation();
void gfx_init();
void load_textures();
void process_symmetric_animation();
void render_hpbar();
void render_message();
void render_UI_text();
void render_pObject_deathrattle();
