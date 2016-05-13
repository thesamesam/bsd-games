// Copyright (c) 1980 The Regents of the University of California.
// Copyright (c) 1999 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

#include "robots.h"

extern const char *Scorefile;
extern int Max_per_uid;

int main (int ac, char** av)
{
    const char *sp;
    bool bad_arg;
    bool show_only;
    int score_wfd;		// high score writable file descriptor
    int score_err = 0;		// hold errno from score file open

    score_wfd = open(Scorefile, O_RDWR);
    if (score_wfd < 0)
	score_err = errno;
    else if (score_wfd < 3)
	exit(1);

    // Revoke setgid privileges
    setregid(getgid(), getgid());

    show_only = false;
    Num_games = 1;
    if (ac > 1) {
	bad_arg = false;
	for (++av; ac > 1 && *av[0]; av++, ac--)
	    if (av[0][0] != '-')
		if (isdigit((unsigned char) av[0][0]))
		    Max_per_uid = atoi(av[0]);
		else {
		    Scorefile = av[0];
		    if (score_wfd >= 0)
			close(score_wfd);
		    score_wfd = open(Scorefile, O_RDWR);
		    if (score_wfd < 0)
			score_err = errno;
#ifdef	FANCY
		    sp = strrchr(Scorefile, '/');
		    if (sp == NULL)
			sp = Scorefile;
		    if (strcmp(sp, "pattern_roll") == 0)
			Pattern_roll = true;
		    else if (strcmp(sp, "stand_still") == 0)
			Stand_still = true;
		    if (Pattern_roll || Stand_still)
			Teleport = true;
#endif
	    } else
		for (sp = &av[0][1]; *sp; sp++)
		    switch (*sp) {
			case 'A':
			    Auto_bot = true;
			    break;
			case 's':
			    show_only = true;
			    break;
			case 'r':
			    Real_time = true;
			    break;
			case 'a':
			    Start_level = 4;
			    break;
			case 'n':
			    Num_games++;
			    break;
			case 'j':
			    Jump = true;
			    break;
			case 't':
			    Teleport = true;
			    break;

			default:
			    fprintf(stderr, "robots: unknown option: %c\n", *sp);
			    bad_arg = true;
			    break;
		    }
	if (bad_arg) {
	    exit(1);
	    // NOTREACHED
	}
    }

    if (show_only) {
	show_score();
	exit(0);
	// NOTREACHED
    }

    if (score_wfd < 0) {
	errno = score_err;
	warn("%s", Scorefile);
	warnx("High scores will not be recorded!");
	sleep(2);
    }

    initscr();
    signal(SIGINT, quit);
    cbreak();
    noecho();
    nonl();
    if (LINES != Y_SIZE || COLS != X_SIZE) {
	if (LINES < Y_SIZE || COLS < X_SIZE) {
	    endwin();
	    printf("Need at least a %dx%d screen\n", Y_SIZE, X_SIZE);
	    exit(1);
	}
	delwin(stdscr);
	stdscr = newwin(Y_SIZE, X_SIZE, 0, 0);
    }

    srandrand();
    if (Real_time)
	signal(SIGALRM, move_robots);
    do {
	while (Num_games--) {
	    init_field();
	    for (Level = Start_level; !Dead; Level++) {
		make_level();
		play_level();
		if (Auto_bot)
		    sleep(1);
	    }
	    move(My_pos.y, My_pos.x);
	    printw("AARRrrgghhhh....");
	    refresh();
	    if (Auto_bot)
		sleep(1);
	    score(score_wfd);
	    if (Auto_bot)
		sleep(1);
	    refresh();
	}
	Num_games = 1;
    } while (!Auto_bot && another());
    quit(0);
    // NOTREACHED
    return 0;
}

// quit:
//      Leave the program elegantly.
void quit (int dummy UNUSED)
{
    endwin();
    exit(0);
    // NOTREACHED
}

//
// another:
//      See if another game is desired
bool another (void)
{
    int y;

#ifdef	FANCY
    if ((Stand_still || Pattern_roll) && !Newscore)
	return true;
#endif

    if (query("Another game?")) {
	if (Full_clear) {
	    for (y = 1; y <= Num_scores; y++) {
		move(y, 1);
		clrtoeol();
	    }
	    refresh();
	}
	return true;
    }
    return false;
}
