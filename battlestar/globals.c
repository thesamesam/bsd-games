// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

int WEIGHT = MAXWEIGHT;
int CUMBER = MAXCUMBER;

const char* const ouch[NUMOFINJURIES] = {
    "some minor abrasions",
    "some minor lacerations",
    "a minor puncture wound",
    "a minor amputation",
    "a sprained wrist",
    "a fractured ankle and shattered kneecap",
    "a broken arm and dislocated shoulder",
    "a few broken ribs",
    "a broken leg and torn ligaments",
    "a broken back and ruptured spleen",
    "some deep incisions and a loss of blood",
    "a fractured skull and mashed face",
    "a broken neck"
};

int win = 1;
int matchcount = 20;
int followgod = -1;
int followfight = -1;

// current input line
char words [NWORD][WORDLEN] = {};
int wordvalue [NWORD] = {};
int wordtype [NWORD] = {};
int wordcount = 0;
int wordnumber = 0;

// state of the game
int ourtime = 0;
int position = 0;
int direction = 0;
int left = 0;
int right = 0;
int ahead = 0;
int back = 0;
int fuel = 0;
int torps = 0;
int carrying = 0;
int encumber = 0;
int rythmn = 0;
int ate = 0;
int snooze = 0;
int godready = 0;
int wintime = 0;
int loved = 0;
int pleasure = 0;
int power = 0;
int ego = 0;
uint16_t game_states = 0;
char beenthere [NUMOFROOMS + 1] = {};
char injuries [NUMOFINJURIES] = {};

const char* username = "player";
