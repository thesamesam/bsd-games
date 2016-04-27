// Copyright (c) 1980 The Regents of the University of California.
// Copyright (c) 1999 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

#include "robots.h"

bool Dead;			// Player is now dead
bool Full_clear = true;		// Lots of junk for init_field to clear
bool Jump = false;		// Jump while running, counting, or waiting
bool Newscore;			// There was a new score added
#ifdef	FANCY
bool Pattern_roll = false;	// Auto play for YHBJNLUK pattern
#endif
bool Real_time = false;		// Play in real time?
bool Auto_bot = false;		// Automatic mover
bool Running = false;		// Currently in the middle of a run
#ifdef	FANCY
bool Stand_still = false;	// Auto play for standing still pattern
#endif
bool Teleport = false;		// Teleport automatically when player must
bool Waiting;			// Player is waiting for end
bool Was_bonus = false;		// Was a bonus last level

char Cnt_move;			// Command which has preceded the count
char Field[Y_FIELDSIZE][X_FIELDSIZE];	// the playing field itslef
const char *Next_move;		// Next move to be used in the pattern
const char *Move_list = "YHBJNLUK";	// List of moves in the pattern
char Run_ch;			// Character for the direction we are running

int Count = 0;			// Command count
int Level;			// Current level
int Num_robots;			// Number of robots left
int Num_scrap;			// Number of scrap heaps
int Num_scores;			// Number of scores posted
int Num_games;			// Number of games to play
u_int32_t Score;		// Current score
int Start_level = 1;		// Level on which to start
int Wait_bonus;			// bonus for waiting

COORD Max;			// Max area robots take up
COORD Min;			// Min area robots take up
COORD My_pos;			// Player's current position
COORD Robots[MAXROBOTS];	// Robots' current positions
COORD Scrap[MAXROBOTS];		// ScrapHeap' current position

jmp_buf End_move;		// Jump to on Real_time
