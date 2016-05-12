// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"
#include <signal.h>

// This routine executes a truncated set of commands until a "yes or "no" answer is gotten.
int getyn (const char* prompt)
{
    for (;;) {
	int com = getinp(prompt, yncoms);
	if (com < 2)
	    return com;
	else
	    (*func[com - 2])();
    }
}

// This routine tells the player if he's out of money.
void notify (void)
{
    if (cur_p->money < 0)
	printf("That leaves you $%d in debt\n", -cur_p->money);
    else if (cur_p->money == 0)
	printf("that leaves you broke\n");
    else if (fixing && !told_em && cur_p->money > 0) {
	printf("-- You are now Solvent ---\n");
	told_em = true;
    }
}

// This routine switches to the next player
void next_play(void)
{
    player = (player + 1) % num_play;
    cur_p = &play[player];
    num_doub = 0;
}

// This routine gets an integer from the keyboard after the given prompt.
int get_int(const char *prompt)
{
    int num;
    char *sp;
    int c;
    char buf[257];

    for (;;) {
      inter:
	printf(prompt);
	num = 0;
	for (sp = buf; (c = getchar()) != '\n'; *sp++ = c)
	    if (c == -1)       // check for interrupted system call
		goto inter;
	*sp = c;
	if (sp == buf)
	    continue;
	for (sp = buf; isspace((unsigned char) *sp); sp++)
	    continue;
	for (; isdigit((unsigned char) *sp); sp++)
	    num = num * 10 + *sp - '0';
	if (*sp == '\n')
	    return num;
	else
	    printf("I can't understand that\n");
    }
}

// This routine sets the monopoly flag from the list given.
void set_ownlist(int pl)
{
    int num;			// general counter
    MON *orig;			// remember starting monop ptr
    OWN *op;			// current owned prop
    OWN *orig_op;		// origianl prop before loop

    op = play[pl].own_list;
#ifndef NDEBUG
    printf("op [%s] = play[pl [%d] ].own_list;\n", op->sqr->name, pl);
#endif
    while (op) {
#ifndef NDEBUG
	printf("op->sqr->type = %d\n", op->sqr->type);
#endif
	switch (op->sqr->type) {
	    case UTIL:
#ifndef NDEBUG
		printf("  case UTIL:\n");
#endif
		for (num = 0; op && op->sqr->type == UTIL; op = op->next)
		    num++;
		play[pl].num_util = num;
#ifndef NDEBUG
		printf("play[pl].num_util = num [%d];\n", num);
#endif
		break;
	    case RR:
#ifndef NDEBUG
		printf("  case RR:\n");
#endif
		for (num = 0; op && op->sqr->type == RR; op = op->next) {
#ifndef NDEBUG
		    printf("iter: %d\n", num);
		    printf("op = %p, op->sqr = %s, op->sqr->type = %d\n", op, op->sqr->name, op->sqr->type);
#endif
		    num++;
		}
		play[pl].num_rr = num;
#ifndef NDEBUG
		printf("play[pl].num_rr = num [%d];\n", num);
#endif
		break;
	    case PRPTY:
#ifndef NDEBUG
		printf("  case PRPTY:\n");
#endif
		orig = op->sqr->desc->mon_desc;
		orig_op = op;
		num = 0;
		while (op && op->sqr->desc->mon_desc == orig) {
#ifndef NDEBUG
		    printf("iter: %d\n", num);
#endif
		    num++;
#ifndef NDEBUG
		    printf("op = op->next ");
#endif
		    op = op->next;
#ifndef NDEBUG
		    printf("[%s];\n", op->sqr->name);
#endif
		}
#ifndef NDEBUG
		printf("num = %d\n", num);
#endif
		if (orig == 0) {
		    printf("panic:  bad monopoly descriptor: " "orig = %p\n", orig);
		    printf("player # %d\n", pl + 1);
		    printhold(pl);
		    printf("orig_op = %p\n", orig_op);
		    printf("orig_op->sqr->type = %d (PRPTY)\n", op->sqr->type);
		    printf("orig_op->next = %p\n", op->next);
		    printf("orig_op->sqr->desc = %p\n", op->sqr->desc);
		    printf("op = %p\n", op);
		    printf("op->sqr->type = %d (PRPTY)\n", op->sqr->type);
		    printf("op->next = %p\n", op->next);
		    printf("op->sqr->desc = %p\n", op->sqr->desc);
		    printf("num = %d\n", num);
		}
#ifndef NDEBUG
		printf("orig->num_in = %d\n", orig->num_in);
#endif
		if (num == orig->num_in)
		    is_monop(orig, pl);
		else
		    is_not_monop(orig);
		break;
	}
    }
}

// This routine sets things up as if it is a new monopoly
void is_monop(MON * mp, int pl)
{
    int i;

    mp->owner = pl;
    mp->num_own = mp->num_in;
    for (i = 0; i < mp->num_in; i++)
	mp->sq[i]->desc->monop = true;
    mp->name = mp->mon_n;
}

// This routine sets things up as if it is no longer a monopoly
void is_not_monop(MON * mp)
{
    int i;

    mp->owner = -1;
    for (i = 0; i < mp->num_in; i++)
	mp->sq[i]->desc->monop = false;
    mp->name = mp->not_m;
}

// This routine gives a list of the current player's routine
void list(void)
{
    printhold(player);
}

// This routine gives a list of a given players holdings
void list_all(void)
{
    unsigned pl;
    while ((pl = getinp("Whose holdings do you want to see? ", name_list)) < num_play)
	printhold(pl);
}

// This routine gives the players a chance before it exits.
void quit(void)
{
    putchar('\n');
    if (getyn("Do you all really want to quit? ") == 0)
	exit(0);
}
