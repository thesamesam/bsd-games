// Copyright (c) 1982 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "mille.h"
#ifndef	unctrl
#include "unctrl.h"
#endif

typedef struct stat STAT;

// This routine saves the current game for use at a later date
// Returns false if it couldn't be done.
bool save (void)
{
    char *sp;
    int outf;
    time_t *tp;
    char buf[80];
    time_t tme;
    STAT junk;
    bool rv;

    sp = NULL;
    tp = &tme;
    if (Fromfile && getyn(SAMEFILEPROMPT))
	strcpy(buf, Fromfile);
    else {
      over:
	prompt(FILEPROMPT);
	leaveok(Board, false);
	refresh();
	sp = buf;
	while ((*sp = readch()) != '\n' && *sp != '\r') {
	    if (*sp == killchar())
		goto over;
	    else if (*sp == erasechar()) {
		if (--sp < buf)
		    sp = buf;
		else {
		    addch('\b');
		    // if the previous char was a control
		    // char, cover up two characters.
		    if (*sp < ' ')
			addch('\b');
		    clrtoeol();
		}
	    } else {
		addstr(unctrl(*sp));
		++sp;
	    }
	    refresh();
	}
	*sp = '\0';
	leaveok(Board, true);
    }

    // check for existing files, and confirm overwrite if needed
    if (sp == buf || (!Fromfile && stat(buf, &junk) > -1 && getyn(OVERWRITEFILEPROMPT) == false))
	return false;

    if ((outf = creat(buf, 0644)) < 0) {
	error(strerror(errno));
	return false;
    }
    mvwaddstr(Score, ERR_Y, ERR_X, buf);
    wrefresh(Score);
    time(tp);		       // get current time
    rv = varpush(outf, writev);
    close(outf);
    if (rv == false) {
	unlink(buf);
    } else {
	strcpy(buf, ctime(tp));
	for (sp = buf; *sp != '\n'; sp++)
	    continue;
	*sp = '\0';
	wprintw(Score, " [%s]", buf);
    }
    wclrtoeol(Score);
    wrefresh(Score);
    return rv;
}

// This does the actual restoring.  It returns true if the
// backup was made on exiting, in which case certain things must
// be cleaned up before the game starts.
bool rest_f (const char* file)
{
    char *sp;
    int inf;
    char buf[80];
    STAT sbuf;

    if ((inf = open(file, O_RDONLY)) < 0) {
	warn("%s", file);
	exit(1);
    }
    if (fstat(inf, &sbuf) < 0) {	// get file stats
	warn("%s", file);
	exit(1);
    }
    varpush(inf, readv);
    close(inf);
    strcpy(buf, ctime(&sbuf.st_mtime));
    for (sp = buf; *sp != '\n'; sp++)
	continue;
    *sp = '\0';
    // initialize some necessary values
    sprintf(Initstr, "%s [%s]\n", file, buf);
    Fromfile = file;
    return !On_exit;
}
