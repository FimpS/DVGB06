#ifndef OBJECT_STATE_HEADER
#define OBJECT_STATE_HEADER

#include "global.h"
#include "objects.h"
#include "map.h"

#define PLAYER_DASH_COOLDOWN_LIMIT 24
#define PLAYER_DASH_LIMIT 5
#define PLAYER_DASH_INVULN_LIMIT 16
#define PLAYER_ATTACK_LIMIT 8
#define HOSTILE_MOBJ_WAKEUP_DIST 64
#define BALISTA_RANGE 49
#define ARCHER_INRANGE 49
#define SWORDSMAN_INRANGE 4
#define DEATHRATTLE_LIMIT 16
#define MAGUS_CAST_RANGE 16
#define MAGUS_CAST_COOLDOWN 32
#define SUMMONER_RANGE 49
#define CRAWLER_RANGE 144
#define CRAWLER_CHARGE_COOLDOWN 32
//bosses
#define GOLEM_STOMP_RANGE 16
#define GOLEM_ATTACK_RANGE 64
#define GOLEM_ABILITY_COOLDOWN 48
#define CULTIST_RANGE_SUMMON 144
#define CULTIST_RANGE_CROSS 64

void set_mObject_state();
void set_pObject_state();
//for player
void input_attack();
void check_sword_dir();
void player_dash();
void dash_control();
void player_attack();
void player_inp_move();
void player_knockbacked();
void player_invuln();
//Generic for enemy
void state_enemy_knockbacked();
void state_deathrattle();
//crawler
void state_crawler_idle();
void state_crawler_dash();
//rusher
void state_rusher_idle();
void state_rusher_idlewalk();
void state_rusher_rush();
//balita
void state_balista_idle();
void state_balista_aware();
void state_balista_shot();
//archer
void state_archer_idle();
void state_archer_aware();
//swordsman
void state_swordsman_slash();
void state_swordsman_aware();
void state_swordsman_idle();
void state_swordsman_dash();
void state_swordsman_ready();
//magus 
void state_magus_idle();
void state_magus_aware();
void state_magus_chase();
void state_magus_ready();
//summoner
void state_summoner_idle();
void state_summoner_dash();
void state_summoner_found();
//bossas
//Golem
void state_golem_aware();
void state_golem_stomp();
void state_golem_dash();
void state_golem_ready();
void state_golem_hit();
//cheiftain
void state_chieftain_summon();
void state_chieftain_aware();
void state_chieftain_ready();
//for all enemies
void state_crawler_knockbacked();
void state_enemy_default();

//generic for pObjects
void state_pObject_deathrattle();
//pObject
void state_sword_shockwave();
void state_magic_bolt_travel();
void state_swordsman_sword_swing();
void state_golem_rock_travel();
void state_golem_weapon_swing();
void state_brimstone_beam();
void state_gravity_well_travel();
void state_gravity_bolt_travel();
void state_rot_smog_flower();
void state_spell_fire();
void state_sword_swing();
void state_wraith_follow();
void state_blood_tax();

#endif
