// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "pathnames.h"
#include "deck.h"
#include "cribbage.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <curses.h>
#include <err.h>

void instructions (void)
{
    int pstat;
    int fd;
    pid_t pid;
    const char *path;

    switch (pid = vfork()) {
	case -1:
	    err(1, "vfork");
	case 0:
	    // Follow the same behaviour for pagers as defined in POSIX.2
	    // for mailx and man.  We only use a pager if stdout is
	    // a terminal, and we pass the file on stdin to sh -c pager.
	    if (!isatty(1))
		path = "cat";
	    else {
		if (!(path = getenv("PAGER")) || (*path == 0))
		    path = _PATH_PAGER;
	    }
	    if ((fd = open(_PATH_INSTR, O_RDONLY)) == -1) {
		warn("open %s", _PATH_INSTR);
		_exit(1);
	    }
	    if (dup2(fd, 0) == -1) {
		warn("dup2");
		_exit(1);
	    }
	    execl("/bin/sh", "sh", "-c", path, (char *) NULL);
	    warn(NULL);
	    _exit(1);
	default:
	    do {
		pid = waitpid(pid, &pstat, 0);
	    } while (pid == -1 && errno == EINTR);
	    if (pid == -1 || WEXITSTATUS(pstat))
		exit(1);
    }
}
