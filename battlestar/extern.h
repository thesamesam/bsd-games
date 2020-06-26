// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include <pwd.h>
#include <signal.h>
#include <time.h>

#define	_PATH_SCORE	_PATH_GAME_STATE "battlestar.scores"
#define DEFAULT_SAVE_FILE	".Bstar"

#define BOLD_ON		"\033[1m"
#define BOLD_OFF	"\033[22m"

// well known rooms
enum LocationId {
    MAIN_HANGAR		= 1,
    HANGAR_GALLERY	= 3,
    LAUNCH_ROOM		= 5,
    STARFIGHTER_ROOM	= 7,
    STATEROOM_CLOSET	= 8,
    AMULET_ROOM		= 13,
    LASER_ROOM		= 20,
    LUXURIOUS_STATEROOM	= 22,
    KITCHEN_STAIRWELL	= 25,
    KITCHEN_DOOR	= 30,
    DINING_ROOM_DOOR	= 31,
    OUTSIDE_BATTLESTAR	= 32,
    ORBITING_PLANET	= 68,
    FIRST_ISLAND_LAND	= 89,
    DOCK		= 93,
    ABOVE_SEA_CAVE	= 114,
    POOLS		= 126,
    WIDER_CANYON	= 133,
    SECRET_THICKET	= 144,
    TIDE_POOLS		= 145,
    ALONG_THE_SHORE	= 159,
    SEA_CAVE_ENTRANCE	= 160,
    COAST_ROAD_TURN	= 162,
    BRIDGE_OVER_LAGOON	= 172,
    CAVE_DOOR		= 189,
    GARDEN		= 197,
    IN_THE_LAGOON	= 201,
    ALONG_HIGH_CLIFFS	= 214,
    ROAD_TURNAROUND	= 215,
    BUNGALOW_PORCH	= 217,
    BUNGALOW_BEDROOM	= 218,
    END_OF_THE_ROAD	= 224,
    CAVE_STREAM_BANK	= 229,
    CAVE_ENTRANCE	= 231,
    HUGE_CHASM		= 232,
    CLIFF_WATERFALL	= 242,
    GODDESS_THRONE_ROOM	= 268,
    FINAL		= 275,
    NUMOFROOMS		= FINAL,
    // Non-world locations
    INVENTORY,
    WEARING
};

// word types
enum WordType {
    VERB,
    OBJECT,
    NOUNS,
    PREPS,
    ADJS,
    CONJ
};

// words numbers
enum ObjectId {
    KNIFE, SWORD, LAND, WOODSMAN, TWO_HANDED,
    CLEAVER, BROAD, MAIL, HELM, SHIELD,
    MAID, STARFIGHTER, LAMPON, SHOES, DRENIAN,
    PAJAMAS, ROBE, AMULET, MEDALION, TALISMAN,
    DEADWOOD, MALLET, LASER, BATHGOD, GODDESS,
    GRENADE, CHAIN, ROPE, LEVIS, MACE,
    SHOVEL, HALBERD, COMPASS, CRASH, ELF,
    FOOT, COINS, MATCHES, MAN, PAPAYAS,
    PINEAPPLE, KIWI, COCONUTS, MANGO, RING,
    POTION, BRACELET, GIRL, GIRLTALK, DARK_LORD,
    TIMER, TIMER1, TIMER2, CHAR, BOMB,
    NATIVE_GIRL, HORSE, CAR, POT, BAR,
    BLOCK, NUMOFOBJECTS
};

// non-objects below
enum {
    UP = 1000, DOWN, AHEAD, BACK, RIGHT,
    LEFT, TAKE, USE, LOOK, QUIT,
    NORTH, SOUTH, EAST, WEST, SU,
    DROP, TAKEOFF, DRAW, PUTON, WEARIT,
    PUT, INVEN, EVERYTHING, AND, KILL,
    THROW_OBJECT, LAUNCH, LANDIT, LIGHT, FOLLOW,
    KISS, LOVE, GIVE, SMITE, SHOOT,
    ON, OFF, TIME, SLEEP, DIG,
    EAT, SWIM, DRINK, DOOR, SAVE,
    RIDE, DRIVE, SCORE, BURY, JUMP,
    KICK, OPEN, VERBOSE, BRIEF, AUXVERB
};

// injuries
enum {
    ABRASIONS, LACERATIONS, MINOR_PUNCTURE, MINOR_AMPUTATION, SPRAINED_WRIST,
    BROKEN_ANKLE, BROKEN_ARM, BROKEN_RIBS, BROKEN_LEG, BROKEN_BACK,
    HEAVY_BLEEDING, FRACTURED_SKULL, BROKEN_NECK, NUMOFINJURIES
};

// game_states bits
enum EGameState {
    CANTLAUNCH, LAUNCHED, CANTSEE, CANTMOVE, DUG,
    OPENED_KITCHEN_DOOR, OPENED_CAVE_DOOR, UNCOVERED_SEA_CAVE, ROPE_IN_PIT, IS_NIGHT,
    MET_GIRL, IS_WIZARD, MATCH_LIGHT, IS_VERBOSE
};

// Number of times room description shown.
enum { ROOMDESC = 3 };

// Fundamental constants
enum {
    LINELENGTH	= 81,
    CYCLE	= 100,
    // Initial variable values
    TANKFULL	= 250,
    TORPEDOES	= 10,
    MAXWEIGHT	= 60,
    MAXCUMBER	= 10
};

enum EDayOrNight { TODAY, TONIGHT };

// These are flags for objects in ObjectInfo. OBJ_PLURAL means
// that the object short name is plural; OBJ_AN that it begins with a
// vowel sound so should be preceded by "an" instead of "a"; OBJ_PERSON
// that it is a living person; OBJ_NONOBJ that it is not an object (to
// which any game action can be applied) at all (e.g. footsteps, asteroids).
// Any individual object has at most one of OBJ_PERSON and OBJ_NONOBJ.
enum {
    OBJ_PLURAL	= 1<<0,
    OBJ_AN	= 1<<1,
    OBJ_PERSON	= 1<<2,
    OBJ_NONOBJ	= 1<<3,
};

struct ObjectInfo {
    const char*	shdesc;
    const char*	desc;
    uint16_t	weight;
    uint8_t	cumber;
    uint8_t	flags;
};

struct room {
    const char*	name;
    const char*	day_desc;
    const char*	night_desc;
    struct {
	uint16_t north;
	uint16_t east;
	uint16_t south;
	uint16_t west;
	uint16_t up;
	uint16_t down;
	uint16_t flyhere;
	uint16_t access;
    }		dir;
};
extern const struct room location [NUMOFROOMS+1];

// object characteristics
extern const struct ObjectInfo c_objinfo [NUMOFOBJECTS];
extern const char ouch [NUMOFINJURIES][24];

// current input line
enum {
    WORDLEN	= 13,
    NWORDS	= 8	       // words per line
};
struct Word {
    char	word [WORDLEN];
    uint8_t	type;
    uint16_t	value;
};

//----------------------------------------------------------------------

extern struct Word words [NWORDS];
extern uint8_t wordcount;
extern uint8_t wordnumber;

// state of the game
extern int ourtime;
extern int position;
extern int direction;
extern int left;
extern int right;
extern int ahead;
extern int back;
extern int ourclock;
extern int fuel;
extern int torps;
extern int carrying;
extern int encumber;
extern int rythmn;
extern int followfight;
extern int ate;
extern int snooze;
extern int followgod;
extern int godready;
extern int win;
extern int wintime;
extern int matchcount;
extern int loved;
extern int pleasure;
extern int power;
extern int ego;
extern int WEIGHT;
extern int CUMBER;
extern uint16_t game_states;
extern char beenthere [NUMOFROOMS+1];
extern char injuries [NUMOFINJURIES];

extern const char *username;

//----------------------------------------------------------------------

// game_states access
inline static bool game_state (enum EGameState f)
    { return game_states & (1u << f); }
inline static void set_game_state (enum EGameState f)
    { game_states |= (1u << f); }
inline static void clear_game_state (enum EGameState f)
    { game_states &= ~(1u << f); }

// These macros yield words to use with objects (followed but not preceded
// by spaces, or with no spaces if the expansion is the empty string).

inline static bool is_plural_object (uint16_t n)
    { return c_objinfo[n].flags & OBJ_PLURAL; }
inline static const char* A_OR_AN (uint16_t n)
    { return c_objinfo[n].flags & OBJ_AN ? "an " : "a "; }
inline static const char* A_OR_AN_OR_THE (uint16_t n)
    { return is_plural_object(n) ? "the " : A_OR_AN(n); }
inline static const char* A_OR_AN_OR_BLANK (uint16_t n)
    { return is_plural_object(n) ? "" : A_OR_AN(n); }
inline static const char* IS_OR_ARE (uint16_t n)
    { return is_plural_object(n) ? "are " : "is "; }

inline static bool is_outside (void)
    { return position > ORBITING_PLANET && position < 246 && position != BUNGALOW_BEDROOM; }

//----------------------------------------------------------------------

void bury(void);
int card(const char *, int);
void chime(void);
void crash(void);
int process_command(void);
_Noreturn void die(void);
void dig(void);
void dooropen(void);
int draw(void);
void drink(void);
int drive(void);
int drop(const char *);
int eat(void);
void fight (enum ObjectId enemy, unsigned strength);
int follow(void);
void get_player_command (const char* prompt);
int give(void);
int jump(void);
void kiss(void);
bool land (void);
bool launch (void);
void light(void);
_Noreturn void live(void);
void love(void);
bool moveplayer(int, int);
void murder(void);
void news(void);
void newway(int);
int put(void);
int puton(void);
const char *rate(void);
int ride(void);
void save(const char *);
char *save_file_name(const char *, size_t);
int shoot(void);
int take (uint16_t fromloc);
int takeoff(void);
int throw_object (const char *);
const char *truedirec(int, char);
int use(void);
int visual(void);
int wearit(void);
void update_relative_directions (void);
void writedes(void);
bool zzz(void);

// obj.c
void cleanup_objects (void);
bool object_is_at (enum ObjectId oid, uint16_t l);
void create_object_at (enum ObjectId oid, uint16_t l);
void remove_object_from (enum ObjectId oid, uint16_t l);
unsigned count_objects_at (uint16_t l);
void printobjs (void);
void convert (enum EDayOrNight to);
void place_default_objects (void);
void restore_saved_objects (FILE* fp);
void save_objects (FILE* fp);
unsigned player_melee_damage (void);
const char* melee_damage_message (unsigned dmg);
unsigned player_melee_damage_taken (void);
