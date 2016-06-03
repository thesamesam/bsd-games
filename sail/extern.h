// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include <sys/types.h>
#include "machdep.h"

	// program mode
extern int mode;
enum {
    MODE_PLAYER = 1,
    MODE_DRIVER,
    MODE_LOGGER
};

// command line flags
extern int randomize;		// -x, give first available ship
extern int longfmt;		// -l, print score in long format
extern int nobells;		// -b, don't ring bell before Signal

// other initial modes
extern gid_t gid;
extern gid_t egid;

#define dieroll()	(nrand(6)+1)
#define sqr(a)		((a) * (a))
#define min(a,b)	((a) < (b) ? (a) : (b))

#define grappled(a)	((a)->file->ngrap)
#define fouled(a)	((a)->file->nfoul)
#define snagged(a)	(grappled(a) + fouled(a))

#define grappled2(a, b)	((a)->file->grap[(b)->file->index].sn_count)
#define fouled2(a, b)	((a)->file->foul[(b)->file->index].sn_count)
#define snagged2(a, b)	(grappled2(a, b) + fouled2(a, b))

#define Xgrappled2(a, b) ((a)->file->grap[(b)->file->index].sn_turn < turn-1 ? grappled2(a, b) : 0)
#define Xfouled2(a, b)	((a)->file->foul[(b)->file->index].sn_turn < turn-1 ? fouled2(a, b) : 0)
#define Xsnagged2(a, b)	(Xgrappled2(a, b) + Xfouled2(a, b))

#define cleangrapple(a, b, c)	Cleansnag(a, b, c, 1)
#define cleanfoul(a, b, c)	Cleansnag(a, b, c, 2)
#define cleansnag(a, b, c)	Cleansnag(a, b, c, 3)

#define sterncolour(sp)	((sp)->file->stern+'0'-((sp)->file->captured?10:0))
#define sternrow(sp)	((sp)->file->row + dr[(sp)->file->dir])
#define sterncol(sp)	((sp)->file->col + dc[(sp)->file->dir])

#define capship(sp)	((sp)->file->captured?(sp)->file->captured:(sp))

#define readyname(r)	((r) & R_LOADING ? '*' : ((r) & R_INITIAL ? '!' : ' '))

// loadL and loadR, should match loadname[]
enum {
    L_EMPTY,
    L_GRAPE,
    L_CHAIN,
    L_ROUND,
    L_DOUBLE,
    L_EXPLODE
};

// readyL and readyR, these are bits, except R_EMPTY
enum {
    R_EMPTY,	// not loaded and not loading
    R_LOADING,	// loading
    R_DOUBLE,	// loading double
    R_LOADED	= 4,	// loaded
    R_INITIAL	= 8	// loaded initial
};

enum {
    HULL,
    RIGGING
};

enum {
    W_CAPTAIN = 1,
    W_CAPTURED,
    W_CLASS,
    W_CREW,
    W_DBP,
    W_DRIFT,
    W_EXPLODE,
    W_FILE,
    W_FOUL,
    W_GUNL,
    W_GUNR,
    W_HULL,
    W_MOVE,
    W_OBP,
    W_PCREW,
    W_UNFOUL,
    W_POINTS,
    W_QUAL,
    W_UNGRAP,
    W_RIGG,
    W_COL,
    W_DIR,
    W_ROW,
    W_SIGNAL,
    W_SINK,
    W_STRUCK,
    W_TA,
    W_ALIVE,
    W_TURN,
    W_WIND,
    W_FS,
    W_GRAP,
    W_RIG1,
    W_RIG2,
    W_RIG3,
    W_RIG4,
    W_BEGIN,
    W_END,
    W_DDEAD
};

enum { NLOG = 10 };

struct logs {
    char l_name[20];
    int l_uid;
    int l_shipnum;
    int l_gamenum;
    int l_netpoints;
};

struct BP {
    short turnsent;
    struct ship *toship;
    short mensent;
};

struct snag {
    short sn_count;
    short sn_turn;
};

#define NSCENE	nscene
enum {
    NSHIP = 10,
    NBP = 3
};

enum {
    N_A,
    N_B,
    N_S,
    N_F,
    N_J,
    N_D,
    N_K,
    N_O,
    NNATION
};

struct File {
    int index;
    char captain[20];		// 0
    short points;		// 20
    unsigned char loadL;	// 22
    unsigned char loadR;	// 24
    unsigned char readyL;	// 26
    unsigned char readyR;	// 28
    struct BP OBP[NBP];		// 30
    struct BP DBP[NBP];		// 48
    char struck;		// 66
    struct ship *captured;	// 68
    short pcrew;		// 70
    char movebuf[10];		// 72
    char drift;			// 82
    short nfoul;
    short ngrap;
    struct snag foul[NSHIP];	// 84
    struct snag grap[NSHIP];	// 124
    char RH;			// 224
    char RG;			// 226
    char RR;			// 228
    char FS;			// 230
    char explode;		// 232
    char sink;			// 234
    unsigned char dir;
    short col;
    short row;
    char loadwith;
    char stern;
};

struct ship {
    const char *shipname;	// 0
    struct shipspecs *specs;	// 2
    unsigned char nationality;	// 4
    short shiprow;		// 6
    short shipcol;		// 8
    char shipdir;		// 10
    struct File *file;		// 12
};

struct scenario {
    char winddir;		// 0
    char windspeed;		// 2
    char windchange;		// 4
    unsigned char vessels;	// 12
    const char *name;		// 14
    struct ship ship[NSHIP];	// 16
};
extern struct scenario scene[];
extern int nscene;

struct shipspecs {
    char bs;
    char fs;
    char ta;
    short guns;
    unsigned char class;
    char hull;
    unsigned char qual;
    char crew1;
    char crew2;
    char crew3;
    char gunL;
    char gunR;
    char carL;
    char carR;
    int rig1;
    int rig2;
    int rig3;
    int rig4;
    short pts;
};
extern struct shipspecs specs[];

extern struct scenario *cc;	// the current scenario
extern struct ship *ls;		// &cc->ship[cc->vessels]

#define SHIP(s)		(&cc->ship[s])
#define foreachship(sp)	for ((sp) = cc->ship; (sp) < ls; (sp)++)

struct windeffects {
    char A, B, C, D;
};
extern const struct windeffects WET[7][6];

struct Tables {
    char H, G, C, R;
};
extern const struct Tables RigTable[11][6];
extern const struct Tables HullTable[11][6];

extern const char AMMO[9][4];
extern const char HDT[9][10];
extern const char HDTrake[9][10];
extern const char QUAL[9][5];
extern const char MT[9][3];

extern const char *const countryname[];
extern const char *const classname[];
extern const char *const directionname[];
extern const char *const qualname[];
extern const char loadname[];

extern const char rangeofshot[];

extern const char dr[], dc[];

extern int winddir;
extern int windspeed;
extern int turn;
extern int game;
extern int alive;
extern int people;
extern int hasdriver;

// assorted.c
void table(struct ship *, struct ship *, int, int, int, int);
void Cleansnag(struct ship *, struct ship *, int, int);

// dr_1.c
void unfoul(void);
void boardcomp(void);
void resolve(void);
void compcombat(void);
int next(void);

// dr_2.c
void thinkofgrapples(void);
void checkup(void);
void prizecheck(void);
void closeon(struct ship *, struct ship *, char *, int, int, int);

// dr_3.c
void moveall(void);
void sendbp(struct ship *, struct ship *, int, int);
int is_toughmelee(struct ship *, struct ship *, int, int);
void reload(void);
void checksails(void);

// dr_4.c
void ungrap(struct ship *, struct ship *);
void grap(struct ship *, struct ship *);

// dr_5.c
void subtract(struct ship *, struct ship *, int, int[3], int);
int mensent(struct ship *, struct ship *, int[3], struct ship **, int *, int);

// dr_main.c
int dr_main(void);

// game.c
int maxturns(struct ship *, char *);
int maxmove(struct ship *, int, int);

// lo_main.c
int lo_main(void);

// misc.c
int range(struct ship *, struct ship *);
struct ship *closestenemy(struct ship *, int, int);
int gunsbear(struct ship *, struct ship *);
int portside(struct ship *, struct ship *, int);
int colours(struct ship *);
void logger(struct ship *);

// parties.c
int meleeing(struct ship *, struct ship *);
int boarding(struct ship *, int);
void unboard(struct ship *, struct ship *, int);

// pl_1.c
void leave(int) NORETURN;
void choke(int) NORETURN;
void child(int);

// pl_2.c
void play(void) NORETURN;

// pl_3.c
void acceptcombat(void);
void grapungrap(void);
void unfoulplayer(void);

// pl_4.c
void changesail(void);
void acceptsignal(void);
void lookout(void);
const char *saywhat(struct ship *, int);
void eyeball(struct ship *);

// pl_5.c
void acceptmove(void);
void acceptboard(void);

// pl_6.c
void repair(void);
void loadplayer(void);

// pl_7.c
void initscreen(void);
void cleanupscreen(void);
void newturn(int);
void Signal(const char *, struct ship *, ...) PRINTFLIKE(1,3);
void Msg(const char *, ...) PRINTFLIKE(1,2);
void prompt(const char *, struct ship *);
int sgetch(const char *, struct ship *, int);
void sgetstr(const char *, char *, int);
void draw_screen(void);
void draw_view(void);
void draw_turn(void);
void draw_stat(void);
void draw_slot(void);
void draw_board(void);
void centerview(void);
void upview(void);
void downview(void);
void leftview(void);
void rightview(void);

// pl_main.c
int pl_main(void);

// sync.c
void fmtship(char *, size_t, const char *, struct ship *);
void makesignal(struct ship *, const char *, struct ship *, ...) PRINTFLIKE(2,4);
void makemsg(struct ship *, const char *, ...) PRINTFLIKE(2,3);
int sync_exists(int);
int sync_open(void);
void sync_close(int);
void Write(int, struct ship *, long, long, long, long);
void Writestr(int, struct ship *, const char *);
int Sync(void);
