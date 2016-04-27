// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "pathnames.h"
#include <sys/types.h>
#include <err.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct betinfo {
    long hand;			// cost of dealing hand
    long inspection;		// cost of inspecting hand
    long game;			// cost of buying game
    long runs;			// cost of running through hands
    long information;		// cost of information
    long thinktime;		// cost of thinking time
    long wins;			// total winnings
    long worth;			// net worth after costs
};

int dbfd;

int main(int, char *[]);
void printuser(const struct passwd *, int);

int main(int argc, char *argv[])
{
    struct passwd *pw;
    int uid;

    // Revoke setgid privileges
    setregid(getgid(), getgid());

    if (argc > 2) {
	printf("Usage: cfscores [user]\n");
	exit(1);
    }
    dbfd = open(_PATH_SCORE, O_RDONLY);
    if (dbfd < 0)
	err(2, "open %s", _PATH_SCORE);
    setpwent();
    if (argc == 1) {
	uid = getuid();
	pw = getpwuid(uid);
	if (pw == 0) {
	    printf("You are not listed in the password file?!?\n");
	    exit(2);
	}
	printuser(pw, 1);
	exit(0);
    }
    if (strcmp(argv[1], "-a") == 0) {
	while ((pw = getpwent()) != 0)
	    printuser(pw, 0);
	exit(0);
    }
    pw = getpwnam(argv[1]);
    if (pw == 0) {
	printf("User %s unknown\n", argv[1]);
	exit(3);
    }
    printuser(pw, 1);
    exit(0);
}

// print out info for specified password entry
void printuser(const struct passwd *pw, int printfail)
{
    int i = lseek(dbfd, pw->pw_uid * sizeof(struct betinfo), SEEK_SET);
    if (i < 0)
	warn("lseek %s", _PATH_SCORE);
    struct betinfo total;
    i = read(dbfd, &total, sizeof(total));
    if (i < 0)
	warn("read %s", _PATH_SCORE);
    if (i == 0 || total.hand == 0) {
	if (printfail)
	    printf("%s has never played canfield.\n", pw->pw_name);
	return;
    }
    printf("*----------------------*\n");
    if (total.worth >= 0)
	printf("* Winnings for %-8s*\n", pw->pw_name);
    else
	printf("* Losses for %-10s*\n", pw->pw_name);
    printf("*======================*\n");
    printf("|Costs           Total |\n");
    printf("| Hands       %8ld |\n", total.hand);
    printf("| Inspections %8ld |\n", total.inspection);
    printf("| Games       %8ld |\n", total.game);
    printf("| Runs        %8ld |\n", total.runs);
    printf("| Information %8ld |\n", total.information);
    printf("| Think time  %8ld |\n", total.thinktime);
    printf("|Total Costs  %8ld |\n", total.wins - total.worth);
    printf("|Winnings     %8ld |\n", total.wins);
    printf("|Net Worth    %8ld |\n", total.worth);
    printf("*----------------------*\n\n");
}
