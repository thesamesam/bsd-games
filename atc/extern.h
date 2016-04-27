// Copyright (c) 1987 by Ed James <edjames@berkeley.edu>
// This file is free software, distributed under the BSD license.

extern char GAMES[];
extern const char *file;

extern int clck, safe_planes, start_time, test_mode;

extern FILE *filein, *fileout;

extern C_SCREEN screen, *sp;

extern LIST air, ground;

extern struct termios tty_start, tty_new;

extern DISPLACEMENT displacement[MAXDIR];

int addplane(void);
void append(LIST *, PLANE *);
void check_adir(int, int, int);
void check_edge(int, int);
void check_edir(int, int, int);
void check_line(int, int, int, int);
void check_linepoint(int, int);
void check_point(int, int);
int checkdefs(void);
int compar(const void *, const void *);
void delete(LIST *, PLANE *);
int dir_deg(int);
int dir_no(char);
void done_screen(void);
void draw_all(void);
void draw_line(WINDOW *, int, int, int, int, const char *);
void erase_all(void);
int getAChar(void);
int getcommand(void);
int gettoken(void);
void init_gr(void);
void ioaddstr(int, const char *);
void ioclrtobot(void);
void ioclrtoeol(int);
void ioerror(int, int, const char *);
void iomove(int);
int list_games(void);
int log_score(int);
void log_score_quit(int) NORETURN;
void loser(const PLANE *, const char *) NORETURN;
int main(int, char *[]);
char name(const PLANE *);
int next_plane(void);
void noise(void);
int number(char);
void open_score_file(void);
void planewin(void);
int pop(void);
void push(int, int);
void quit(int);
int read_file(const char *);
void redraw(void);
void rezero(void);
void setup_screen(const C_SCREEN *);
int too_close(const PLANE * p1, const PLANE * p2, int);
void update(int);
int yyerror(const char *);
int yylex(void);
#ifndef YYEMPTY
int yyparse(void);
#endif
const char *Left(char);
const char *Right(char);
const char *airport(char);
const char *beacon(char);
const char *benum(char);
const char *circle(char);
const char *climb(char);
const char *command(const PLANE *);
const char *default_game(void);
const char *delayb(char);
const char *descend(char);
const char *ex_it(char);
PLANE *findplane(int);
const char *ignore(char);
const char *left(char);
const char *mark(char);
PLANE *newplane(void);
const char *okay_game(const char *);
const char *rel_dir(char);
const char *right(char);
const char *setalt(char);
const char *setplane(char);
const char *setrelalt(char);
const char *timestr(int);
const char *to_dir(char);
const char *turn(char);
const char *unmark(char);
