// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
// Re-coding of advent in C: file i/o and user i/o

#include "hdr.h"
#include "extern.h"

//----------------------------------------------------------------------

void getin(char **wrd1, char **wrd2)	// no prompt, usually
{
    char *s;
    static char wd1buf[MAXSTR], wd2buf[MAXSTR];
    int first, numch;

    *wrd1 = wd1buf;	       // return ptr to internal str
    *wrd2 = wd2buf;
    wd2buf[0] = 0;	       // in case it isn't set here
    for (s = wd1buf, first = 1, numch = 0;;) {
	if ((*s = getchar()) >= 'A' && *s <= 'Z')
	    *s = *s - ('A' - 'a');
	// convert to upper case
	switch (*s) {	       // start reading from user
	    case '\n':
		*s = 0;
		return;
	    case ' ':
		if (s == wd1buf || s == wd2buf)	// initial blank
		    continue;
		*s = 0;
		if (first) {   // finished 1st wd; start 2nd
		    first = numch = 0;
		    s = wd2buf;
		    break;
		} else {       // finished 2nd word
		    FLUSHLINE;
		    *s = 0;
		    return;
		}
	    case EOF:
		printf("user closed input stream, quitting...\n");
		exit(0);
	    default:
		if (++numch >= MAXSTR) {	// string too long
		    printf("Give me a break!!\n");
		    wd1buf[0] = wd2buf[0] = 0;
		    FLUSHLINE;
		    return;
		}
		s++;
	}
    }
}

bool yes (int prompt, int yesmsg, int nomsg)
{
    bool result = false;
    for (;;) {
	rspeak (prompt);	// tell him what we want
	int ch = getchar();
	if (ch == 'y')
	    result = true;
	else if (ch == 'n')
	    result = false;
	else if (ch == EOF) {
	    printf ("User closed input stream, quitting...\n");
	    exit (EXIT_SUCCESS);
	}
	FLUSHLINE;
	if (ch == 'y' || ch == 'n')
	    break;
	printf ("Please answer the question.\n");
    }
    if (result == true)
	rspeak (yesmsg);
    if (result == false)
	rspeak (nomsg);
    return result;
}

bool yesm (int prompt, int yesmsg, int nomsg)
{
    bool result = false;
    for (;;) {
	mspeak (prompt);
	int ch = getchar();
	if (ch == 'y')
	    result = true;
	else if (ch == 'n')
	    result = false;
	else if (ch == EOF) {
	    printf ("User closed input stream, quitting...\n");
	    exit (EXIT_SUCCESS);
	}
	FLUSHLINE;
	if (ch == 'y' || ch == 'n')
	    break;
	printf ("Please answer the question.\n");
    }
    if (result == true)
	mspeak (yesmsg);
    if (result == false)
	mspeak (nomsg);
    return result;
}

void rspeak (int msg)
{
    if (msg != 0)
	speak (rtext[msg]);
}

void mspeak (int msg)
{
    if (msg != 0)
	speak (mtext[msg]);
}

void speak (const char* msg)
{
    printf ("\n%s\n", msg);
}

// Multiple object descriptions exist based on conditions; set skip to select sub-message
void pspeak (int m, int skip)
{
    const char* msg = ptext[m];
    for (int i = 0; i <= skip && *msg; ++i)
	msg += strlen(msg)+1;
    speak (msg);
}
