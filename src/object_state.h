#ifndef OBJECT_STATE_HEADER
#define OBJECT_STATE_HEADER

#include "global.h"
#include "objects.h"
#include "map.h"

//p
#define PLAYER_DASH_COOLDOWN_LIMIT 20
#define PLAYER_DASH_LIMIT 8
#define PLAYER_DASH_INVULN_LIMIT 12
#define PLAYER_ATTACK_LIMIT 4
#define PLAYER_ATTACKING_LIMIT 20
#define PLAYER_DEATHRATTLE_LIMIT 64
//mObj
#define HOSTILE_MOBJ_WAKEUP_DIST 64
#define BALISTA_RANGE 49
#define PEAK_LOCAL_CAST_RANGE 16
#define PEAK_KNIGHT_CAST_RANGE 16
#define ROCK_WELL_CAST_RANGE 36
#define ROCK_ROLLER_ROLL_RANGE 49
#define VORTEX_CLOSE_CAST_RANGE 8
#define ARCHER_INRANGE 25
#define FIRE_ARCHER_INRANGE 42
#define SWORDSMAN_INRANGE 8
#define DEATHRATTLE_LIMIT 16
#define MAGUS_CAST_RANGE 49
#define MAGUS_CAST_COOLDOWN 32
#define SUMMONER_RANGE 64
#define CRAWLER_RANGE 144
#define RIDER_CHARGE_RANGE 49
#define DRIDER_FIRE_RANGE 25
#define FIRE_TOWER_RANGE 49
#define CRAWLER_CHARGE_COOLDOWN 32
//bosses
#define GOLEM_STOMP_RANGE 16
#define GOLEM_ATTACK_RANGE 64
#define GOLEM_ABILITY_COOLDOWN 64
#define CULTIST_RANGE_SUMMON 144
#define CULTIST_RANGE_CROSS 64

void set_mObject_state();
void set_pObject_state();
//for player
void check_attack_mods();
void player_deathrattle();
void input_attack();
void check_sword_dir();
void player_dash();
void player_dash_atk();
void dash_control();
void player_attack();
void player_attacking();
void player_inp_move();
void player_knockbacked();
void player_invuln();
//Generic for enemy
void state_enemy_knockbacked();
void state_deathrattle();
//peak local
void state_peak_local_idle();
void state_peak_local_aware();
void state_peak_local_dash();
void state_peak_local_cast();
//peak knight
void state_peak_knight_idle();
void state_peak_knight_aware();
void state_peak_knight_dash();
void state_peak_knight_cast();
//rock_well
void state_rock_well_idle();
void state_rock_well_aware();
void state_rock_well_cast();
//rock_roller
void state_rock_roller_idle();
void state_rock_roller_aware();
void state_rock_roller_aroll();
void state_rock_roller_droll();
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
void state_archer_draw();
void state_archer_dash();
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
//rider_nonranged
void state_rider_idle();
void state_rider_aware();
void state_rider_charge();
//rider_demounted
void state_drider_idle();
void state_drider_aware();
void state_drider_ready();
void state_drider_fire_charge();
void state_drider_dash();
//fire_tower
void state_fire_tower_idle();
void state_fire_tower_fire();
//fire_bomber
void state_fire_bomber_idle();
void state_fire_bomber_aware();
//fire_archer
void state_fire_archer_idle();
void state_fire_archer_aware();
void state_fire_archer_dash();
void state_fire_archer_draw();
void state_fire_archer_rite();
//bosses
//Vortex
void state_rock_vortex_aware();
void state_rock_vortex_storm();
void state_rock_vortex_summon();
void state_rock_vortex_cast();
void state_rock_vortex_decast();
//queen
void state_local_queen_aware();
void state_local_queen_dash();
void state_local_queen_cast();
void state_local_queen_bog();
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
//runeguard
void state_rune_guard_aware();
void state_rune_guard_unholy();
void state_rune_guard_gravity();
void state_rune_guard_blood();
void state_rune_guard_frost();
void state_rune_guard_holy();
void state_rune_guard_rot();
//for all enemies
void state_crawler_knockbacked();
void state_enemy_default();

//generic for pObjects
void state_pObject_deathrattle();
void check_pObject_mObject_hit();
//pObject
void state_player_spear_action();
void state_player_spear_dash_action();
void state_sword_shockwave();
void state_magic_bolt_travel();
void state_swamp_pool_action();
void state_spear_cast_action();
void state_rock_cast_action();
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
void state_fire_sling_action();
void state_lava_pool_action();
void state_hex_arrow_action();
void state_blood_seeker_action();
void state_gravity_vortex_action();
void state_rot_flies_action();
void state_rot_flies_action_fly();
void state_holy_omen_action();

#endif
