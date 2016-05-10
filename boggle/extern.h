// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include <time.h>

void addword(const char *);
void badword(void);
char *batchword(FILE *);
void checkdict(void);
int checkword(const char *, int, int *);
void cleanup(void);
void delay(int);
long dictseek(FILE *, long, int);
void findword(void);
void flushin(FILE *);
char *boggle_getline(char *);
void getword(char *);
int help(void);
int inputch(void);
int loaddict(FILE *);
int loadindex(const char *);
void newgame(const char *);
char *nextword(FILE *);
FILE *opendict(const char *);
void playgame(void);
void prompt(const char *);
void prtable(const char *const[], int, int, int, void (*)(const char *const[], int), int (*)(const char *const[], int));
void putstr(const char *);
void redraw(void);
void results(void);
int setup(int, time_t);
void showboard(const char *);
void showstr(const char *, int);
void showword(int);
void starttime(void);
void startwords(void);
void stoptime(void);
int timerch(void);
void usage(void) NORETURN;
int validword(const char *);
