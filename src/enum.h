#ifndef ENUM_HEADER
#define ENUM_HEADER

typedef enum 
{
	MAP_SPRITESHEET,
	POBJECT_SPRITESHEET,
	MOBJECT_SPRITESHEET,
	RUNES_SPRITESHEET,
	PLAYER_SPRITESHEET,
	FONT_SPRITESHEET,
	UI_SPRITESHEET,
} textureID;

typedef enum
{
	ST_P_NORMAL,
	ST_P_INVULN,
	ST_P_DASH,
	ST_P_ATTACK,
	ST_P_ATTACKING,
	ST_P_DEAD,
	ST_P_GONE,
	ST_P_KNOCKBACK
} player_global_state;

typedef enum
{
	PA_ROCK_DEBRIS,
	PA_MAGIC_DEBRIS,
} particle_type;

typedef enum
{
	MO_RUNNER,
	MO_CRAWLER,	
	MO_RUSHER,
	MO_BALISTA,
	MO_SUMMONER,
	MO_RUNE_SHARD,
	MO_PEAK_LOCAL,
	MO_PEAK_KNIGHT,
	MO_ROCK_WELL,
	MO_ROCK_ROLLER,
	MO_ROCK_VORTEX,
	MO_LOCAL_QUEEN,
	MO_ARCHER,
	MO_SWORDSMAN,
	MO_MAGUS,
	MO_CULTIST_CHIEFTAIN,
	MO_GOLEM,
	MO_RIDER_FIGHTER,
	MO_DRIDER_FIGHTER,
	MO_FIRE_ARCHER,
	MO_FIRE_TOWER,
	MO_FIRE_BOMBER,
	MO_RUNE_GUARD,
	MO_INTERACTABLE,
	MO_STARTRUN,	
	MO_STATSIGN,
	MO_ENDRUN,	
} mObject_type;

typedef enum
{
	STATUS_HEX,
	STATUS_STUN,
	STATUS_BOGGED,
	STATUS_BURN,
	STATUS_FROSTBITE,
	STATUS_ROT,
	STATUS_STASIS,
	STATUS_OMEN,
	STATUS_NONE
} object_status_effect;

typedef enum
{
	RN_HOLY,
	RN_UNHOLY,
	RN_BLOOD,
	RN_FROST,
	RN_ROT,
	RN_GRAVITY,
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
	PO_BALISTA_BOLT,
	PO_WRAITH,
	PO_BIG_WRAITH,
	PO_BLOOD_TAX,
	PO_SWAMP_POOL,
	PO_SPEAR_CAST,
	PO_ROCK_CAST,
	PO_SWAMP_BOLT,
	PO_SWORD_SHOCKWAVE,
	PO_SWORDSMAN_SWORD,
	PO_MAGIC_BOLT,
	PO_GOLEM_ROCK,
	PO_GOLEM_MELEE_WEAPON,
	PO_PLAYER_SPEAR,
	PO_GRAVITY_WELL,
	PO_GRAVITY_BOLT,
	PO_FROST_STORM,
	PO_FIRE_SLING,
	PO_LAVA_POOL,
	PO_HEX_ARROW,

	PO_BLOOD_SEEKER,
	PO_GRAVITY_VORTEX,
	PO_ROT_FLIES,
	PO_HOLY_OMEN,
	placeholder
} pObject_type;

typedef enum
{
	ST_MAP_RUN_TICK,
	ST_MAP_CINEMATIC,
	ST_MAP_CLEAR,
	ST_MAP_PAUSE,
	ST_MAP_TRANSITION,
	ST_MAP_FADE_IN,
	ST_MAP_DEFAULT,
} map_state;

typedef enum
{
	st_spawn,
	st_m1,
	ST_CRAWLER_DASH,
	ST_CRAWLER_IDLE,
	ST_BALISTA_IDLE,
	ST_PEAK_LOCAL_IDLE,
	ST_PEAK_LOCAL_AWARE,
	ST_PEAK_LOCAL_DASH,
	ST_PEAK_LOCAL_CAST,
	ST_PEAK_KNIGHT_IDLE,
	ST_PEAK_KNIGHT_AWARE,
	ST_PEAK_KNIGHT_DASH,
	ST_PEAK_KNIGHT_CAST,
	ST_ROCK_WELL_IDLE,
	ST_ROCK_WELL_AWARE,
	ST_ROCK_WELL_CAST,
	ST_ROCK_ROLLER_IDLE,
	ST_ROCK_ROLLER_AWARE,
	ST_ROCK_ROLLER_AROLL,
	ST_ROCK_ROLLER_DROLL,
	ST_ROCK_VORTEX_AWARE,
	ST_ROCK_VORTEX_STORM,
	ST_ROCK_VORTEX_PRESTORM,
	ST_ROCK_VORTEX_SUMMON,
	ST_ROCK_VORTEX_CAST,
	ST_ROCK_VORTEX_DECAST,
	ST_LOCAL_QUEEN_AWARE,
	ST_LOCAL_QUEEN_DASH,
	ST_LOCAL_QUEEN_CAST,
	ST_LOCAL_QUEEN_BOG,
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
	ST_RIDER_IDLE,
	ST_RIDER_AWARE,
	ST_RIDER_CHARGE,
	ST_DRIDER_IDLE,
	ST_DRIDER_AWARE,
	ST_DRIDER_DASH,
	ST_DRIDER_READY,
	ST_DRIDER_FIRE_CHARGE,
	ST_FIRE_BOMBER_IDLE,
	ST_FIRE_BOMBER_AWARE,
	ST_FIRE_TOWER_IDLE,
	ST_FIRE_TOWER_FIRE,
	ST_FIRE_ARCHER_IDLE,
	ST_FIRE_ARCHER_AWARE,
	ST_FIRE_ARCHER_DASH,
	ST_FIRE_ARCHER_DRAW,
	ST_FIRE_ARCHER_RITE,
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
	ST_RUNE_GUARD_SHOW,
	ST_RUNE_GUARD_AWARE,
	ST_RUNE_GUARD_UNHOLY,
	ST_RUNE_GUARD_GRAVITY,
	ST_RUNE_GUARD_GRAVITY_SECOND,
	ST_RUNE_GUARD_BLOOD,
	ST_RUNE_GUARD_FROST,
	ST_RUNE_GUARD_FROST_SECOND,
	ST_RUNE_GUARD_HOLY,
	ST_RUNE_GUARD_ROT,
	ST_ENEMYKNOCKBACK,
	ST_ENEMYDEAD,
	ST_DEATHRATTLE,
	ST_CLEAR,
	ST_INTERACTABLE,
	ST_STARTP_IDLE,
	ST_STARTP_OPEN,
	ST_STARTP_STAY,
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
	TYPE_EVENT_GOLEM,
	TYPE_EVENT_CHIEFTAIN,
	TYPE_EVENT_QUEEN,
	TYPE_EVENT_VORTEX,
	TYPE_EVENT_GUARD,
	TYPE_EVENT_START_RUN,
	TYPE_EVENT_END_RUN,
	TYPE_EVENT_BRIDGE,
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

typedef enum
{
	UI_CURRENT_HEALTH,
	UI_FULL_HEALTH,
	UI_HEALTH_BORDER,

	UI_RUNE_SLOT_1,
	UI_RUNE_SLOT_2,
	UI_RUNE_SLOT_3,

	UI_BLOOD,
	UI_FROST,
	UI_GRAVITY,
	UI_UNHOLY,
	UI_HOLY,
	UI_ROT,

	UI_MENU,
	UI_OPTION,
	UI_MENU_POINTER,

	UI_BOSS_BAR,
	UI_BOSS_FULL_BAR,
	UI_BOSS_DEC_BAR,
	
} UI_id;
#if 0
	UI_CURRENT_HEALTH_D,
	UI_FULL_HEALTH_D,
	UI_HEALTH_BORDER_D,
#endif

#endif
