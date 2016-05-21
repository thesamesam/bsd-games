// Copyright (c) 2016 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT license.

#include "../config.h"
#include <signal.h>
#include <curses.h>

static void cleanup_curses (void)
{
    if (isendwin())
	return;
    flushinp();
    endwin();
}

static void on_fatal_signal (int sig)
{
    cleanup_curses();
    if (sig != SIGINT && sig != SIGQUIT && sig != SIGTERM)
	psignal (sig, "Fatal error");
    exit (EXIT_FAILURE);
}

void initialize_curses (void)
{
    static const uint8_t c_FatalSignals[] = {
	SIGINT, SIGQUIT, SIGTERM, SIGILL, SIGBUS,
	SIGABRT, SIGFPE, SIGSYS, SIGSEGV, SIGHUP
    };
    for (unsigned i = 0; i < ArraySize(c_FatalSignals); ++i)
	signal (c_FatalSignals[i], on_fatal_signal);
    if (!initscr()) {
	puts ("Error: unable to initialize terminal graphics");
	exit (EXIT_FAILURE);
    }
    atexit (cleanup_curses);
    start_color();
    use_default_colors();
    noecho();
    cbreak();
    curs_set (0);
    keypad (stdscr, true);

    signal (SIGTSTP, SIG_IGN);	// Disable Ctrl+z
    srandrand();
}
