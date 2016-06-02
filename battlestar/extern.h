// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include <pwd.h>
#include <signal.h>
#include <time.h>

#define BITS (8 * sizeof (int))

#define OUTSIDE		(position > 68 && position < 246 && position != 218)
#define max(a,b)	((a) < (b) ? (b) : (a))
#define testbit(array, index)	(array[index/BITS] & (1 << (index % BITS)))
#define setbit(array, index)	(array[index/BITS] |= (1 << (index % BITS)))
#define clearbit(array, index)	(array[index/BITS] &= ~(1 << (index % BITS)))

#define	_PATH_SCORE	_PATH_GAME_STATE "battlestar.scores"
#define DEFAULT_SAVE_FILE	".Bstar"

// well known rooms
enum {
    FINAL	= 275,
    GARDEN	= 197,
    POOLS	= 126,
    DOCK	= 93
};

// word types
enum {
    VERB,
    OBJECT,
    NOUNS,
    PREPS,
    ADJS,
    CONJ
};

// words numbers
enum {
    KNIFE,
    SWORD,
    LAND,
    WOODSMAN,
    TWO_HANDED,
    CLEAVER,
    BROAD,
    MAIL,
    HELM,
    SHIELD,
    MAID,
    BODY = MAID,
    VIPER,
    LAMPON,
    SHOES,
    CYLON,
    PAJAMAS,
    ROBE,
    AMULET,
    MEDALION,
    TALISMAN,
    DEADWOOD,
    MALLET,
    LASER,
    BATHGOD,
    NORMGOD,
    GRENADE,
    CHAIN,
    ROPE,
    LEVIS,
    MACE,
    SHOVEL,
    HALBERD,
    COMPASS,
    CRASH,
    ELF,
    FOOT,
    COINS,
    MATCHES,
    MAN,
    PAPAYAS,
    PINEAPPLE,
    KIWI,
    COCONUTS,
    MANGO,
    RING,
    POTION,
    BRACELET,
    GIRL,
    GIRLTALK,
    DARK,
    TIMER,
    CHAR = TIMER+3,
    BOMB,
    DEADGOD,
    DEADTIME,
    DEADNATIVE,
    NATIVE,
    HORSE,
    CAR,
    POT,
    BAR,
    BLOCK,
    NUMOFOBJECTS
};

// non-objects below
enum {
    UP = 1000,
    DOWN,
    AHEAD,
    BACK,
    RIGHT,
    LEFT,
    TAKE,
    USE,
    LOOK,
    QUIT,
    NORTH,
    SOUTH,
    EAST,
    WEST,
    SU,
    DROP,
    TAKEOFF,
    DRAW,
    PUTON,
    WEARIT,
    PUT,
    INVEN,
    EVERYTHING,
    AND,
    KILL,
    RAVAGE,
    UNDRESS,
    THROW,
    LAUNCH,
    LANDIT,
    LIGHT,
    FOLLOW,
    KISS,
    LOVE,
    GIVE,
    SMITE,
    SHOOT,
    ON,
    OFF,
    TIME,
    SLEEP,
    DIG,
    EAT,
    SWIM,
    DRINK,
    DOOR,
    SAVE,
    RIDE,
    DRIVE,
    SCORE,
    BURY,
    JUMP,
    KICK,
    OPEN,
    VERBOSE,
    BRIEF,
    AUXVERB
};

// injuries
enum {
    ARM		= 6,	// broken arm
    RIBS	= 7,	// broken ribs
    SPINE	= 9,	// broken back
    INCISE	= 10,	// deep incisions
    SKULL	= 11,	// fractured skull
    NECK	= 12,	// broken NECK
    NUMOFINJURIES
};

// notes
enum {
    CANTLAUNCH,
    LAUNCHED,
    CANTSEE,
    CANTMOVE,
    JINXED,
    DUG,
    NUMOFNOTES
};

// Number of times room description shown.
enum { ROOMDESC = 3 };

// Fundamental constants
enum {
    NUMOFROOMS	= 275,
    NUMOFWORDS	= ((NUMOFOBJECTS + BITS - 1) / BITS),
    LINELENGTH	= 81,

    TODAY	= 0,
    TONIGHT	= 1,
    CYCLE	= 100,

    // Initial variable values
    TANKFULL	= 250,
    TORPEDOES	= 10,
    MAXWEIGHT	= 60,
    MAXCUMBER	= 10
};

// These are flags for objects in the objflags array.  OBJ_PLURAL means
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

struct room {
    const char *name;
    int link[8];
#define north	link[0]
#define south	link[1]
#define east	link[2]
#define west	link[3]
#define up	link[4]
#define access	link[5]
#define down	link[6]
#define flyhere	link[7]
    const char *desc;
    unsigned int objects[NUMOFWORDS];
};
extern struct room dayfile[];
extern struct room nightfile[];
extern struct room *location;

 // object characteristics
extern const char *const objdes [NUMOFOBJECTS];
extern const char *const objsht [NUMOFOBJECTS];
extern const char *const ouch [NUMOFINJURIES];
extern const int objwt [NUMOFOBJECTS];
extern const int objcumber [NUMOFOBJECTS];
extern const int objflags [NUMOFOBJECTS];
#define is_plural_object(n)	(objflags[(n)] & OBJ_PLURAL)
//
// These macros yield words to use with objects (followed but not preceded
// by spaces, or with no spaces if the expansion is the empty string).
#define A_OR_AN(n)		(objflags[(n)] & OBJ_AN ? "an " : "a ")
#define A_OR_AN_OR_THE(n)	(is_plural_object((n)) ? "the " : A_OR_AN((n)))
#define A_OR_AN_OR_BLANK(n)	(is_plural_object((n)) ? "" : A_OR_AN((n)))
#define IS_OR_ARE(n)		(is_plural_object((n)) ? "are " : "is ")

 // current input line
enum {
    WORDLEN	= 15,
    NWORD	= 20	       // words per line
};
extern char words[NWORD][WORDLEN];
extern int wordvalue[NWORD];
extern int wordtype[NWORD];
extern int wordcount, wordnumber;

 // state of the game
extern int ourtime;
extern int position;
extern int direction;
extern int left, right, ahead, back;
extern int ourclock, fuel, torps;
extern int carrying, encumber;
extern int rythmn;
extern int followfight;
extern int ate;
extern int snooze;
extern int meetgirl;
extern int followgod;
extern int godready;
extern int win;
extern int wintime;
extern int wiz;
extern int tempwiz;
extern int matchlight, matchcount;
extern int loved;
extern int pleasure, power, ego;
extern int WEIGHT;
extern int CUMBER;
extern int notes[NUMOFNOTES];
extern unsigned int inven[NUMOFWORDS];
extern unsigned int wear[NUMOFWORDS];
extern char beenthere[NUMOFROOMS + 1];
extern char injuries[NUMOFINJURIES];
extern int verbose;

extern const char *username;

struct wlist {
    const char *string;
    int value, article;
    struct wlist *next;
};
extern struct wlist wlist[];

struct objs {
    short room;
    short obj;
};
extern const struct objs dayobjs[];
extern const struct objs nightobjs[];

void bury(void);
int card(const char *, int);
void chime(void);
void convert(int);
void crash(void);
int cypher(void);
void die(void) NORETURN;
void diesig(int) NORETURN;
void dig(void);
void dooropen(void);
int draw(void);
void drink(void);
int drive(void);
int drop(const char *);
int eat(void);
int fight(int, int);
int follow(void);
char *getcom(char *, int, const char *, const char *);
char *getword(char *, char *, int);
int give(void);
void initialize(const char *);
int jump(void);
void kiss(void);
int land(void);
int launch(void);
void light(void);
void live(void) NORETURN;
void love(void);
int moveplayer(int, int);
void murder(void);
void news(void);
void newway(int);
void open_score_file(void);
void parse(void);
void post(char);
void printobjs(void);
int put(void);
int puton(void);
const char *rate(void);
void ravage(void);
void restore(const char *);
int ride(void);
void save(const char *);
char *save_file_name(const char *, size_t);
int shoot(void);
int take(unsigned int[]);
int takeoff(void);
int throw(const char *);
const char *truedirec(int, char);
int ucard(const unsigned int *);
int use(void);
int visual(void);
int wearit(void);
void whichway(struct room);
void wordinit(void);
void writedes(void);
int zzz(void);
