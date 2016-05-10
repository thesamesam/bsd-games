// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "board.h"
#include <signal.h>

static void getplayers(void);
static void init_players(void);
static void init_monops(void);
static void do_quit(int);
void *heapstart;

// This program implements a monopoly game
int main(int ac, char *av[])
{
    // Revoke setgid privileges
    setregid(getgid(), getgid());

    srand(getpid());
    heapstart = sbrk(0);
    if (ac > 1) {
	if (!rest_f(av[1]))
	    restore();
    } else {
	getplayers();
	init_players();
	init_monops();
    }
    num_luck = sizeof lucky_mes / sizeof(char *);
    init_decks();
    signal(SIGINT, do_quit);
    for (;;) {
	printf("\n%s (%d) (cash $%d) on %s\n", cur_p->name, player + 1, cur_p->money, board[cur_p->loc].name);
	printturn();
	force_morg();
	execute(getinp("-- Command: ", comlist));
    }
}

static void do_quit (int n UNUSED)
{
    quit();
}

// This routine gets the names of the players
static void getplayers(void)
{
    char *sp;
    int i, j;
    char buf[257];

  blew_it:
    for (;;) {
	if ((num_play = get_int("How many players? ")) <= 0 || num_play > MAX_PL)
	    printf("Sorry. Number must range from 1 to 9\n");
	else
	    break;
    }
    cur_p = play = (PLAY *) calloc(num_play, sizeof(PLAY));
    if (play == NULL)
	err(1, NULL);
    for (i = 0; i < num_play; i++) {
      over:
	printf("Player %d's name: ", i + 1);
	for (sp = buf; (*sp = getchar()) != '\n'; sp++)
	    continue;
	if (sp == buf)
	    goto over;
	*sp++ = '\0';
	name_list[i] = play[i].name = (char *) calloc(1, sp - buf);
	if (name_list[i] == NULL)
	    err(1, NULL);
	strcpy(play[i].name, buf);
	play[i].money = 1500;
    }
    name_list[i++] = "done";
    name_list[i] = 0;
    for (i = 0; i < num_play; ++i) {
	for (j = i + 1; j < num_play; ++j) {
	    if (strcasecmp(name_list[i], name_list[j]) == 0) {
		if (i != num_play - 1)
		    printf("Hey!!! Some of those are " "IDENTICAL!!  Let's try that " "again....\n");
		else
		    printf("\"done\" is a reserved word.  " "Please try again\n");
		for (i = 0; i < num_play; i++)
		    free(play[i].name);
		free(play);
		goto blew_it;
	    }
	}
    }
}

// This routine figures out who goes first
static void init_players(void)
{
    int i, rl, cur_max;
    bool over = 0;
    int max_pl = 0;

  again:
    putchar('\n');
    for (cur_max = i = 0; i < num_play; i++) {
	printf("%s (%d) rolls %d\n", play[i].name, i + 1, rl = roll(2, 6));
	if (rl > cur_max) {
	    over = false;
	    cur_max = rl;
	    max_pl = i;
	} else if (rl == cur_max)
	    over++;
    }
    if (over) {
	printf("%d people rolled the same thing, so we'll try again\n", over + 1);
	goto again;
    }
    player = max_pl;
    cur_p = &play[max_pl];
    printf("%s (%d) goes first\n", cur_p->name, max_pl + 1);
}

// This routine initializes the monopoly structures.
static void init_monops(void)
{
    for (MON* mp = mon; mp < &mon[N_MON]; ++mp) {
	mp->name = mp->not_m;
	for (int i = 0; i < mp->num_in; ++i)
	    mp->sq[i] = &board[mp->sqnums[i]];
    }
}
