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
	PA_ROCK_DEBRIS,
	PA_MAGIC_DEBRIS,
} particle_type;

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
	MO_CULTIST_CHIEFTAIN,
	MO_GOLEM,
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
	PO_GOLEM_ROCK,
	PO_GOLEM_MELEE_WEAPON,
	gravity_well,
	gravity_bolt,
	frost_storm,
	rot_smog,
	placeholder
} pObject_type;

typedef enum
{
	ST_MAP_RUN_TICK,
	ST_MAP_CINEMATIC,
	ST_MAP_CLEAR,
	ST_MAP_DEFAULT,
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
	ST_BALISTA_IDLE,
	ST_BALISTA_AWARE,
	ST_ARCHER_IDLE,
	ST_ARCHER_AWARE,
	ST_ARCHER_DASH,
	ST_ARCHER_DRAW,
	ST_SWORDSMAN_IDLE,
	ST_SWORDSMAN_AWARE,
	ST_SWORDSMAN_DASH,
	ST_SWORDSMAN_READY,
	ST_SWORDSMAN_SLASH,
	ST_MAGUS_DASH,
	ST_MAGUS_AWARE,
	ST_MAGUS_IDLE,
	ST_MAGUS_READY,
	ST_MAGUS_KNOCKBACKED,
	ST_SUMMONER_IDLE,
	ST_SUMMONER_DASH,
	ST_SUMMONER_SUMMON,
	ST_GOLEM_AWARE,
	ST_GOLEM_DASH,
	ST_GOLEM_READY,
	ST_GOLEM_HIT,
	ST_GOLEM_STOMP,
	ST_GOLEM_BUILD,
	ST_CHIEFTAIN_IDLE,
	ST_CHIEFTAIN_DASH,
	ST_CHIEFTAIN_AWARE,
	ST_CHIEFTAIN_READY,
	ST_CHIEFTAIN_SUMMON,
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
	type_event_golem,
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
