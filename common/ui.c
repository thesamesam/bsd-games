// Copyright (c) 2016 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT license.

#include "../config.h"
#include <signal.h>
#include <curses.h>

void cleanup_curses (void)
{
    if (isendwin())
	return;
    flushinp();
    endwin();
}

#define S(s)	(1<<(s))
enum {
    sigset_Quit = S(SIGINT)|S(SIGQUIT)|S(SIGTERM),
    exitcode_SignalBase = 128
};

static _Noreturn void on_fatal_signal (int sig)
{
    static volatile _Atomic(bool) s_bOnce = false;
    int exitcode = exitcode_SignalBase+sig;
    if (false == atomic_exchange (&s_bOnce, true)) {
	cleanup_curses();
	if (S(sig) & sigset_Quit)
	    exitcode = EXIT_SUCCESS;	// terminated by user
	else
	    psignal (sig, "Fatal error");
	exit (exitcode);
    }
    _Exit (exitcode);
}
#undef S

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

void init_pairs (const struct color_pair* cps, size_t ncps)
{
    for (unsigned i = 0; i < ncps; ++i)
	init_pair (i+1, cps[i].fg, cps[i].bg);
}
