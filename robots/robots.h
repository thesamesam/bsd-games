// Copyright (c) 1980 The Regents of the University of California.
// Copyright (c) 1999 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include <sys/ttydefaults.h>
#include <curses.h>
#include <err.h>
#include <pwd.h>
#include <setjmp.h>
#include <signal.h>
#include <termios.h>

// miscellaneous constants

enum {
    Y_FIELDSIZE		= 23,
    X_FIELDSIZE		= 60,
    Y_SIZE		= 24,
    X_SIZE		= 80,
    MAXLEVELS		= 4,
    MAXROBOTS		= (MAXLEVELS * 10),
    ROB_SCORE		= 10,
    S_BONUS		= (60 * ROB_SCORE),
    Y_SCORE		= 21,
    X_SCORE		= (X_FIELDSIZE + 9),
    Y_PROMPT		= (Y_FIELDSIZE - 1),
    X_PROMPT		= (X_FIELDSIZE + 2),
    MAXSCORES		= (Y_SIZE - 2),
    MAXNAME		= 16,
    MAX_PER_UID		= 5
};

#define	MS_NAME		"Ten"
#define	_PATH_SCORE	_PATH_GAME_STATE "robots.scores"

// characters on screen
enum {
    ROBOT	= '+',
    HEAP	= '*',
    PLAYER	= '@'
};

// type definitions
typedef struct {
    int y, x;
} COORD;

typedef struct {
    u_int32_t s_uid;
    u_int32_t s_score;
    u_int32_t s_auto;
    u_int32_t s_level;
    char s_name[MAXNAME];
} SCORE;

typedef struct passwd PASSWD;

// global variables

extern bool Dead, Full_clear, Jump, Newscore, Real_time, Running, Teleport, Waiting, Was_bonus, Auto_bot;

#ifdef	FANCY
extern bool Pattern_roll, Stand_still;
#endif

extern char Cnt_move, Field[Y_FIELDSIZE][X_FIELDSIZE], Run_ch;
extern const char *Next_move, *Move_list;

extern int Count, Level, Num_robots, Num_scrap, Num_scores, Start_level, Wait_bonus, Num_games;

extern u_int32_t Score;

extern COORD Max, Min, My_pos, Robots[], Scrap[];

extern jmp_buf End_move;

// functions types

void add_score(int);
bool another(void);
char automove(void);
int cmp_sc(const void *, const void *);
bool do_move(int, int);
bool eaten(const COORD *);
void flush_in(void);
void get_move(void);
void init_field(void);
bool jumping(void);
void make_level(void);
void move_robots(int);
bool must_telep(void);
void play_level(void);
int query(const char *);
void quit(int) NORETURN;
void reset_count(void);
int rnd(int);
COORD *rnd_pos(void);
void score(int);
void set_name(SCORE *);
void show_score(void);
void telmsg(int);
