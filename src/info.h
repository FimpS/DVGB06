#ifndef INFO_H
#define INFO_H

#define PLAYER_START_HP 200
#define PLAYER_START_AS 48
#define PLAYER_START_MS 0.75
#define PLAYER_START_DMG 40
#define PLAYER_START_DASH_INVULN 10
#define PLAYER_START_DMG_INVULN 40

void init_pObject();
void init_mObject();
void initPlayer();


struct render_info init_render_info();
struct SDL_Rect identify_rune_sprite();
void identify_mObject_sprite_location();
void identify_pObject_sprite_location();
void identify_player_sprite_location();


#endif
