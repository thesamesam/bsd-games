// Copyright (c) 1987 by Ed James <edjames@berkeley.edu>
// This file is free software, distributed under the BSD license.

#include "include.h"
#include <getopt.h>
#include <signal.h>

extern FILE *yyin;

int main(int ac, char *av[])
{
    int f_usage = 0, f_list = 0, f_showscore = 0;
    const char *gamename = NULL;
    int ch;
    struct sigaction sa;
#ifdef BSD
    struct itimerval itv;
#endif

    // Open the score file then revoke setgid privileges
    open_score_file();
    setregid(getgid(), getgid());

    start_time = time(NULL);

    while ((ch = getopt(ac, av, "ulstpg:f:")) != -1) {
	switch (ch) {
	    case '?':
	    case 'u':
	    default:
		f_usage++;
		break;
	    case 'l':
		f_list++;
		break;
	    case 's':
	    case 't':
		f_showscore++;
		break;
	    case 'f':
	    case 'g':
		gamename = optarg;
		break;
	}
    }
    if (optind < ac)
	f_usage++;
    srandrand();

    if (f_usage)
	fprintf(stderr, "Usage: %s -[u?lstp] [-[gf] game_name]\n", av[0]);
    if (f_showscore)
	log_score(1);
    if (f_list)
	list_games();

    if (f_usage || f_showscore || f_list)
	return EXIT_SUCCESS;

    if (!gamename)
	gamename = default_game();
    else
	gamename = okay_game (gamename);

    if (!gamename || load_game (gamename) < 0)
	return EXIT_FAILURE;

    init_gr();
    setup_screen(sp);

    addplane();

    signal(SIGINT, quit);
    signal(SIGQUIT, quit);
#ifdef BSD
    signal(SIGTSTP, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);
#endif
    signal(SIGHUP, log_score_quit);
    signal(SIGTERM, log_score_quit);

    tcgetattr(fileno(stdin), &tty_start);
    tty_new = tty_start;
    tty_new.c_lflag &= ~(ICANON | ECHO);
    tty_new.c_iflag |= ICRNL;
    tty_new.c_cc[VMIN] = 1;
    tty_new.c_cc[VTIME] = 0;
    tcsetattr(fileno(stdin), TCSADRAIN, &tty_new);

    sa.sa_handler = update;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGALRM);
    sigaddset(&sa.sa_mask, SIGINT);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, (struct sigaction *) 0);

#ifdef BSD
    itv.it_value.tv_sec = 0;
    itv.it_value.tv_usec = 1;
    itv.it_interval.tv_sec = sp->update_secs;
    itv.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &itv, NULL);
#endif
#ifdef SYSV
    alarm(sp->update_secs);
#endif

    for (;;) {
	if (getcommand() != 1)
	    planewin();
	else {
#ifdef BSD
	    itv.it_value.tv_sec = 0;
	    itv.it_value.tv_usec = 0;
	    setitimer(ITIMER_REAL, &itv, NULL);
#endif
#ifdef SYSV
	    alarm(0);
#endif

	    update(0);

#ifdef BSD
	    itv.it_value.tv_sec = sp->update_secs;
	    itv.it_value.tv_usec = 0;
	    itv.it_interval.tv_sec = sp->update_secs;
	    itv.it_interval.tv_usec = 0;
	    setitimer(ITIMER_REAL, &itv, NULL);
#endif
#ifdef SYSV
	    alarm(sp->update_secs);
#endif
	}
    }
}
