// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

static int checkout(const char *);
static const char *getutmp(void);
static int wizard(const char *);

void initialize(const char *filename)
{
    const struct objs *p;
    char *savefile;

    puts("Version 4.2, fall 1984.");
    puts("First Adventure game written by His Lordship, the honorable");
    puts("Admiral D.W. Riggle\n");
    location = dayfile;
    srand(getpid());
    username = getutmp();
    wordinit();
    if (filename == NULL) {
	direction = NORTH;
	ourtime = 0;
	snooze = CYCLE * 1.5;
	position = 22;
	setbit(wear, PAJAMAS);
	fuel = TANKFULL;
	torps = TORPEDOES;
	for (p = dayobjs; p->room != 0; p++)
	    setbit(location[p->room].objects, p->obj);
    } else {
	savefile = save_file_name(filename, strlen(filename));
	restore(savefile);
	free(savefile);
    }
    wiz = wizard(username);
    signal(SIGINT, diesig);
}

static const char *getutmp(void)
{
    struct passwd *ptr;

    ptr = getpwuid(getuid());
    if (ptr == NULL)
	return "";
    else
	return strdup(ptr->pw_name);
}

// Hereditary wizards.  A configuration file might make more sense.
static const char *const list[] = {
    "riggle",
    "chris",
    "edward",
    "comay",
    "yee",
    "dmr",
    "ken",
    0
};

static const char *const badguys[] = {
    "wnj",
    "root",
    "ted",
    0
};

static int wizard(const char *uname)
{
    int flag;

    if ((flag = checkout(uname)) != 0)
	printf("You are the Great wizard %s.\n", uname);
    return flag;
}

static int checkout(const char *uname)
{
    const char *const *ptr;

    for (ptr = list; *ptr; ptr++)
	if (strcmp(*ptr, uname) == 0)
	    return 1;
    for (ptr = badguys; *ptr; ptr++)
	if (strcmp(*ptr, uname) == 0) {
	    printf("You are the Poor anti-wizard %s.  Good Luck!\n", uname);
	    CUMBER = 3;
	    WEIGHT = 9;	       // that'll get him!
	    ourclock = 10;
	    setbit(location[7].objects, WOODSMAN);	// viper room
	    setbit(location[20].objects, WOODSMAN);	// laser "
	    setbit(location[13].objects, DARK);	// amulet "
	    setbit(location[8].objects, ELF);	// closet
	    return 0;	       // anything else, Chris?
	}
    return 0;
}
