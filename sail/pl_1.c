// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"
#include "player.h"
#include <sys/wait.h>
#include <signal.h>

// If we get here before a ship is chosen, then ms == 0 and
// we don't want to update the score file, or do any Write's either.
// We can assume the sync file is already created and may need
// to be removed.
// Of course, we don't do any more Sync()'s if we got here
// because of a Sync() failure.
void leave(int conditions)
{
    signal(SIGHUP, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    if (done_curses) {
	Msg("It looks like you've had it!");
	switch (conditions) {
	    case LEAVE_QUIT:
		break;
	    case LEAVE_CAPTURED:
		Msg("Your ship was captured.");
		break;
	    case LEAVE_HURRICAN:
		Msg("Hurricane!  All ships destroyed.");
		break;
	    case LEAVE_DRIVER:
		Msg("The driver died.");
		break;
	    case LEAVE_SYNC:
		Msg("Synchronization error.");
		break;
	    default:
		Msg("A funny thing happened (%d).", conditions);
	}
    } else {
	switch (conditions) {
	    case LEAVE_QUIT:
		break;
	    case LEAVE_DRIVER:
		printf("The driver died.\n");
		break;
	    case LEAVE_FORK:
		perror("fork");
		break;
	    case LEAVE_SYNC:
		printf("Synchronization error\n.");
		break;
	    default:
		printf("A funny thing happened (%d).\n", conditions);
	}
    }

    if (ms != 0) {
	logger(ms);
	if (conditions != LEAVE_SYNC) {
	    makemsg(ms, "Captain %s relinquishing.", mf->captain);
	    Write(W_END, ms, 0, 0, 0, 0);
	    Sync();
	}
    }
    sync_close(!hasdriver);
    sleep(5);
    cleanupscreen();
    exit(0);
}

void choke (int n UNUSED)
{
    leave(LEAVE_QUIT);
}

void child (int n UNUSED)
{
    int status;
    int pid;

    signal(SIGCHLD, SIG_IGN);
    do {
	pid = wait3(&status, WNOHANG, (struct rusage *) 0);
	if (pid < 0 || (pid > 0 && !WIFSTOPPED(status)))
	    hasdriver = 0;
    } while (pid > 0);
    signal(SIGCHLD, child);
}
