// Copyright (c) 1990 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <err.h>

#define	_PATH_INSTR	_PATH_GAME_DATA "fish.instr"

enum {
    COMPUTER,
    USER,
    RANKS	= 13,
    CARDS	= 4,
    HANDSIZE	= 7,
    TOTCARDS	= RANKS * CARDS
};
#define OTHER(a)	(1 - (a))

static const char *const cards[] = {
    "A", "2", "3", "4", "5", "6", "7",
    "8", "9", "10", "J", "Q", "K", NULL,
};

#define	PRC(card)	printf(" %s", cards[card])

static int promode;
static int asked[RANKS], comphand[RANKS], deck[TOTCARDS];
static int userasked[RANKS], userhand[RANKS];
static int curcard = TOTCARDS;

static void chkwinner(int, const int *);
static int compmove(void);
static int countbooks(const int *);
static int countcards(const int *);
static int drawcard(int, int *);
static int gofish(int, int, int *);
static void goodmove(int, int, int *, int *);
static void init(void);
static void instructions(void);
static int nrandom(int);
static void printhand(const int *);
static void printplayer(int);
static int promove(void);
static void usage(void) ;
static int usermove(void);

int main (int argc, char **argv)
{
    // Revoke setgid privileges
    setregid(getgid(), getgid());

    for (int ch; (ch = getopt(argc, argv, "p")) != -1;) {
	switch (ch) {
	    case 'p':	promode = 1; break;
	    case '?':
	    default:	usage();
	}
    }

    srand (time (NULL));
    instructions();
    init();

    if (nrandom(2) == 1) {
	printplayer(COMPUTER);
	printf("get to start.\n");
	goto istart;
    }
    printplayer(USER);
    printf("get to start.\n");

    for (;;) {
	int move = usermove();
	if (!comphand[move]) {
	    if (gofish(move, USER, userhand))
		continue;
	} else {
	    goodmove(USER, move, userhand, comphand);
	    continue;
	}

      istart:for (;;) {
	    move = compmove();
	    if (!userhand[move]) {
		if (!gofish(move, COMPUTER, comphand))
		    break;
	    } else
		goodmove(COMPUTER, move, comphand, userhand);
	}
    }
    return EXIT_SUCCESS;
}

static int usermove (void)
{
    int n;
    const char *const *p;
    char buf[256];

    printf("\nYour hand is:");
    printhand(userhand);

    for (;;) {
	printf("You ask me for: ");
	fflush(stdout);
	if (fgets(buf, sizeof(buf), stdin) == NULL)
	    exit (EXIT_SUCCESS);
	if (buf[0] == '\0')
	    continue;
	if (buf[0] == '\n') {
	    printf("%d cards in my hand, %d in the pool.\n", countcards(comphand), curcard);
	    printf("My books:");
	    countbooks(comphand);
	    continue;
	}
	buf[strlen(buf) - 1] = '\0';
	if (!strcasecmp(buf, "p") && !promode) {
	    promode = 1;
	    printf("Entering pro mode.\n");
	    continue;
	}
	if (!strcasecmp(buf, "quit"))
	    exit (EXIT_SUCCESS);
	for (p = cards; *p; ++p)
	    if (!strcasecmp(*p, buf))
		break;
	if (!*p) {
	    printf("I don't understand!\n");
	    continue;
	}
	n = p - cards;
	if (userhand[n]) {
	    userasked[n] = 1;
	    return n;
	}
	if (nrandom(3) == 1)
	    printf("You don't have any of those!\n");
	else
	    printf("You don't have any %s's!\n", cards[n]);
	if (nrandom(4) == 1)
	    printf("No cheating!\n");
	printf("Guess again.\n");
    }
    // NOTREACHED
}

static int compmove (void)
{
    static int lmove;

    if (promode)
	lmove = promove();
    else {
	do {
	    lmove = (lmove + 1) % RANKS;
	} while (!comphand[lmove] || comphand[lmove] == CARDS);
    }
    asked[lmove] = 1;

    printf("I ask you for: %s.\n", cards[lmove]);
    return lmove;
}

static int promove (void)
{
    int i, max;

    for (i = 0; i < RANKS; ++i) {
	if (userasked[i] && comphand[i] > 0 && comphand[i] < CARDS) {
	    userasked[i] = 0;
	    return i;
	}
    }
    if (nrandom(3) == 1) {
	for (i = 0;; ++i) {
	    if (comphand[i] && comphand[i] != CARDS) {
		max = i;
		break;
	    }
	}
	while (++i < RANKS)
	    if (comphand[i] != CARDS && comphand[i] > comphand[max])
		max = i;
	return max;
    }
    if (nrandom(1024) == 0723) {
	for (i = 0; i < RANKS; ++i)
	    if (userhand[i] && comphand[i])
		return i;
    }
    for (;;) {
	for (i = 0; i < RANKS; ++i)
	    if (comphand[i] && comphand[i] != CARDS && !asked[i])
		return i;
	for (i = 0; i < RANKS; ++i)
	    asked[i] = 0;
    }
    // NOTREACHED
}

static int drawcard (int player, int *hand)
{
    int card = deck[--curcard];
    ++hand[card];
    if (player == USER || hand[card] == CARDS) {
	printplayer(player);
	printf("drew %s", cards[card]);
	if (hand[card] == CARDS) {
	    printf(" and made a book of %s's!\n", cards[card]);
	    chkwinner(player, hand);
	} else
	    printf(".\n");
    }
    return card;
}

static int gofish (int askedfor, int player, int *hand)
{
    printplayer(OTHER(player));
    printf("say \"GO FISH!\"\n");
    if (askedfor == drawcard(player, hand)) {
	printplayer(player);
	printf("drew the guess!\n");
	printplayer(player);
	printf("get to ask again!\n");
	return 1;
    }
    return 0;
}

static void goodmove (int player, int move, int *hand, int *opphand)
{
    printplayer(OTHER(player));
    printf("have %d %s%s.\n", opphand[move], cards[move], opphand[move] == 1 ? "" : "'s");

    hand[move] += opphand[move];
    opphand[move] = 0;

    if (hand[move] == CARDS) {
	printplayer(player);
	printf("made a book of %s's!\n", cards[move]);
	chkwinner(player, hand);
    }

    chkwinner(OTHER(player), opphand);

    printplayer(player);
    printf("get another guess!\n");
}

static void chkwinner (int player, const int *hand)
{
    int cb, i, ub;

    for (i = 0; i < RANKS; ++i)
	if (hand[i] > 0 && hand[i] < CARDS)
	    return;
    printplayer(player);
    printf("don't have any more cards!\n");
    printf("My books:");
    cb = countbooks(comphand);
    printf("Your books:");
    ub = countbooks(userhand);
    printf("\nI have %d, you have %d.\n", cb, ub);
    if (ub > cb) {
	printf("\nYou win!!!\n");
	if (nrandom(1024) == 0723)
	    printf("Cheater, cheater, pumpkin eater!\n");
    } else if (cb > ub) {
	printf("\nI win!!!\n");
	if (nrandom(1024) == 0723)
	    printf("Hah!  Stupid peasant!\n");
    } else
	printf("\nTie!\n");
    exit (EXIT_SUCCESS);
}

static void printplayer (int player)
{
    switch (player) {
	case COMPUTER:	printf("I "); break;
	case USER:	printf("You "); break;
    }
}

static void printhand (const int *hand)
{
    int book, i, j;
    for (book = i = 0; i < RANKS; i++)
	if (hand[i] < CARDS)
	    for (j = hand[i]; --j >= 0;)
		PRC(i);
	else
	    ++book;
    if (book) {
	printf(" + Book%s of", book > 1 ? "s" : "");
	for (i = 0; i < RANKS; i++)
	    if (hand[i] == CARDS)
		PRC(i);
    }
    putchar('\n');
}

static int countcards (const int *hand)
{
    int count = 0;
    for (int i = 0; i < RANKS; ++i)
	count += *hand++;
    return count;
}

static int countbooks (const int *hand)
{
    int count = 0;
    for (int i = 0; i < RANKS; ++i) {
	if (hand[i] == CARDS) {
	    ++count;
	    PRC(i);
	}
    }
    if (!count)
	printf(" none");
    putchar('\n');
    return count;
}

static void init (void)
{
    for (int i = 0; i < TOTCARDS; ++i)
	deck[i] = i % RANKS;
    for (int i = 0; i < TOTCARDS - 1; ++i) {
	int j = nrandom(TOTCARDS - i);
	if (j == 0)
	    continue;
	int temp = deck[i];
	deck[i] = deck[i + j];
	deck[i + j] = temp;
    }
    for (int i = 0; i < HANDSIZE; ++i) {
	++userhand[deck[--curcard]];
	++comphand[deck[--curcard]];
    }
}

static int nrandom (int n)
{
    return rand() % n;
}

static void instructions (void)
{
    int input;
    pid_t pid;
    int fd;
    const char *pager;
    int status;

    printf("Would you like instructions (y or n)? ");
    input = getchar();
    while (getchar() != '\n');
    if (input != 'y')
	return;

    switch (pid = fork()) {
	case 0:	       // child
	    if (!isatty(1))
		pager = "cat";
	    else {
		if (!(pager = getenv("PAGER")) || (*pager == 0))
		    pager = _PATH_PAGER;
	    }
	    if ((fd = open(_PATH_INSTR, O_RDONLY)) == -1)
		err(1, "open %s", _PATH_INSTR);
	    if (dup2(fd, 0) == -1)
		err(1, "dup2");
	    execl("/bin/sh", "sh", "-c", pager, (char *) NULL);
	    err(1, "exec sh -c %s", pager);
	    // NOTREACHED
	case -1:
	    err(1, "fork");
	    // NOTREACHED
	default:
	    waitpid(pid, &status, 0);
	    break;
    }
    printf ("Hit return to continue...\n");
    while ((input = getchar()) != EOF && input != '\n') {}
}

static void usage (void)
{
    fprintf (stderr, "usage: fish [-p]\n");
    exit (EXIT_FAILURE);
}
