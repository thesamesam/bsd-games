// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "pathnames.h"
#include "deck.h"
#include "cribbage.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <curses.h>

void instructions (void)
{
    int pstat;
    pid_t pid = fork();
    switch (pid = fork()) {
	default: waitpid (pid, &pstat, 0); break;
	case -1: perror ("fork"); break;
	case 0:
	    execlp (_PATH_PAGER, _PATH_PAGER, _PATH_INSTR, NULL);
	    perror ("execlp");
	    exit (EXIT_FAILURE);
    }
}
