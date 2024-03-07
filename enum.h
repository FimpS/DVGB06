#ifndef ENUM_HEADER
#define ENUM_HEADER

typedef enum 
{
	MAP_SPRITESHEET,
	POBJECT_SPRITESHEET,
	MOBJECT_SPRITESHEET,
	RUNES_SPRITESHEET,
	PLAYER_SPRITESHEET,
} textureID;

typedef enum
{
	st_p_normal,
	st_p_invuln,
	ST_P_DASH,
	st_p_knockbacked,
} player_global_state;

typedef enum
{
	runner,
	crawler,
	rusher,
	balista,
	summoner,
	rune_shard,
	MO_ARCHER,
	MO_SWORDSMAN,
	MO_MAGUS,
	interactable
} mObject_type;

typedef enum
{
	status_frostbite,
	status_rot,
	STATUS_STASIS,
	status_none
} mObject_status_effect;

typedef enum
{
	holy,
	unholy,
	blood,
	frost,
	rot,
	gravity,
	flame,
	rage
} core_type;

typedef enum
{
	anchor,
	support,
	mending,
	complete,
} core_stage;

typedef enum
{
	fire,
	sword,
	balista_bolt,
	wraith,
	wraith_big,
	blood_tax,
	brimstone,
	PO_SWORDSMAN_SWORD,
	PO_MAGIC_BOLT,
	gravity_well,
	gravity_bolt,
	frost_storm,
	rot_smog,
	placeholder
} pObject_type;

typedef enum
{
	st_map_default,
	st_map_clear,
} map_state;

typedef enum
{
	st_swinging,
	st_idle,
	st_zwei
} sword_state;

typedef enum
{
	st_f,
	st_b,
	st_l,
	st_r
} dir_state;

typedef enum
{
	st_tired,
	st_running,
	//st_dead
} mObject_state;

typedef enum
{
	st_spawn,
	st_m1,
	ST_CRAWLER_DASH,
	ST_CRAWLER_IDLE,
	ST_MAGUS_DASH,
	ST_MAGUS_AWARE,
	ST_MAGUS_IDLE,
	ST_MAGUS_READY,
	ST_MAGUS_KNOCKBACKED,
	ST_SUMMONER_IDLE,
	ST_SUMMONER_DASH,
	ST_SUMMONER_SUMMON,
	st_crawler_m1,
	st_crawler_m2,
	st_crawler_m3,
	st_runner_m1,
	st_runner_m2,
	st_runner_m3,
	st_summoner_idle,
	st_summoner_summon,
	st_enemyknockback,
	st_enemydead,
	st_deathrattle,
	st_clear,
	st_placeholder
} mObject_global_state;

typedef enum
{
	ST_PO_REAL,
	ST_PO_DEATHRATTLE,
	ST_PO_DEAD
} pObject_global_state;

typedef enum
{
	type_event_teleport,
	type_event_inmaptp,
	type_event_lock,
	type_event_rune_acquired,
	type_event_placeholder
} event_type;

typedef enum
{
	WEST,
	EAST,
	NORTH,
	SOUTH,
	NORTHEAST,
	SOUTHEAST,
	NORTHWEST,
	SOUTHWEST
} card_dir;

#endif
