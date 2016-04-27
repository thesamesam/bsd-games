// Copyright (c) 1982 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <curses.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

// Miscellaneous constants

typedef short CARD;

enum {
    HAND_SZ		= 7,	// number of cards in a hand
    DECK_SZ		= 101,	// number of cards in decks
    NUM_SAFE		= 4,	// number of saftey cards
    NUM_MILES		= 5,	// number of milestones types
    NUM_CARDS		= 20,	// number of types of cards
    BOARD_Y		= 17,	// size of board screen
    BOARD_X		= 40,
    MILES_Y		= 7,	// size of mileage screen
    MILES_X		= 80,
    SCORE_Y		= 17,	// size of score screen
    SCORE_X		= 40,
    MOVE_Y		= 10,	// Where to print move prompt
    MOVE_X		= 20,
    ERR_Y		= 15,	// Where to print errors
    ERR_X		= 5,
    EXT_Y		= 4,	// Where to put Extension
    EXT_X		= 9
};
enum {
    PLAYER,
    COMP
};
enum {
    W_SMALL,	// Small (initial) window
    W_FULL	// Full (final) window
};

// Move types
enum {
    M_DISCARD,
    M_DRAW,
    M_PLAY,
    M_ORDER
};

// Scores
enum {
    SC_SAFETY		= 100,
    SC_ALL_SAFE		= 300,
    SC_COUP		= 300,
    SC_TRIP		= 400,
    SC_SAFE		= 300,
    SC_DELAY		= 300,
    SC_EXTENSION	= 200,
    SC_SHUT_OUT		= 500
};


// safety descriptions
enum {
    S_UNKNOWN		= 0,	// location of safety unknown
    S_IN_HAND		= 1,	// safety in player's hand
    S_PLAYED		= 2,	// safety has been played
    S_GAS_SAFE		= 0,	// Gas safety card index
    S_SPARE_SAFE	= 1,	// Tire safety card index
    S_DRIVE_SAFE	= 2,	// Driveing safety card index
    S_RIGHT_WAY		= 3,	// Right-of-Way card index
    S_CONV		= 15,	// conversion from C_ to S_
};

// card numbers
enum {
    C_INIT = -1,
    C_25,
    C_50,
    C_75,
    C_100,
    C_200,
    C_EMPTY,
    C_FLAT,
    C_CRASH,
    C_STOP,
    C_LIMIT,
    C_GAS,
    C_SPARE,
    C_REPAIRS,
    C_GO,
    C_END_LIMIT,
    C_GAS_SAFE,
    C_SPARE_SAFE,
    C_DRIVE_SAFE,
    C_RIGHT_WAY
};

// prompt types
enum {
    MOVEPROMPT,
    REALLYPROMPT,
    ANOTHERHANDPROMPT,
    ANOTHERGAMEPROMPT,
    SAVEGAMEPROMPT,
    SAMEFILEPROMPT,
    FILEPROMPT,
    EXTENSIONPROMPT,
    OVERWRITEFILEPROMPT
};

typedef struct {
    bool coups[NUM_SAFE];
    bool can_go;
    bool new_battle;
    bool new_speed;
    short safety[NUM_SAFE];
    short sh_safety[NUM_SAFE];
    short nummiles[NUM_MILES];
    short sh_nummiles[NUM_MILES];
    CARD hand[HAND_SZ];
    CARD sh_hand[HAND_SZ];
    CARD battle;
    CARD sh_battle;
    CARD speed;
    CARD sh_speed;
    int mileage;
    int sh_mileage;
    int hand_tot;
    int sh_hand_tot;
    int safescore;
    int sh_safescore;
    int coupscore;
    int total;
    int sh_total;
    int games;
    int sh_games;
    int was_finished;
} PLAY;

// macros
#define	other(x)	(1 - x)
#define	nextplay()	(Play = other(Play))
#define	nextwin(x)	(1 - x)
#define	opposite(x)	(Opposite[x])
#define	is_safety(x)	(x >= C_GAS_SAFE)

// externals

extern bool Debug, Finished, Next, On_exit, Order, Saved;

extern char Initstr[];
extern const char *C_fmt, *const *C_name, *Fromfile;

extern int Card_no, End, Handstart, Movetype, Numgos, Numneed[], Numseen[NUM_CARDS], Play, Window;
extern const int Numcards[], Value[];

extern CARD Deck[DECK_SZ], Discard, Sh_discard, *Topcard;
extern const CARD Opposite[NUM_CARDS];

extern FILE *outf;

extern PLAY Player[2];

extern WINDOW *Board, *Miles, *Score;

// functions
void account(CARD);
void calcmove(void);
int canplay(const PLAY *, const PLAY *, CARD);
int check_ext(bool);
void check_go(void);
void check_more(void);
void die(int) NORETURN;
void domove(void);
bool error(const char *, ...);
void extrapolate(PLAY *);
void finalscore(PLAY *);
CARD getcard(void);
void getmove(void);
int getyn(int);
int haspicked(const PLAY *);
void init(void);
int is_repair(CARD);
int main(int, char **);
void newboard(void);
void newscore(void);
int onecard(const PLAY *);
int playcard(PLAY *);
void prboard(void);
void prompt(int);
void prscore(bool);
int readch(void);
bool rest_f(const char *);
int roll(int, int);
void rub(int);
int safety(CARD);
bool save(void);
void show_card(int, int, CARD, CARD *);
void show_score(int, int, int, int *);
void shuffle(void);
void sort(CARD *);
void undoex(int);
#if defined(__linux__) && !defined(__GLIBC__)
bool varpush(int, ssize_t(int, const struct iovec *, size_t));
#else
bool varpush(int, ssize_t(int, const struct iovec *, int));
#endif
