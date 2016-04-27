// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum { LINE = 70 };

static char buf[257];

static int comp(const char *);

int getinp(const char *prompt, const char *const list[])
{
    int i, n_match, match = 0;
    char *sp;
    int c;

    for (;;) {
      inter:
	printf("%s", prompt);
	for (sp = buf; (c = getchar()) != '\n';) {
	    *sp = c;
	    if (c == -1)       // check for interrupted system call
		goto inter;
	    else if (sp != buf || *sp != ' ')
		sp++;
	}
	*sp = c;
	if (buf[0] == '?' && buf[1] == '\n') {
	    printf("Valid inputs are: ");
	    for (i = 0, match = 18; list[i]; i++) {
		if ((match += (n_match = strlen(list[i]))) > LINE) {
		    printf("\n\t");
		    match = n_match + 8;
		}
		if (*list[i] == '\0') {
		    match += 8;
		    printf("<RETURN>");
		} else
		    printf("%s", list[i]);
		if (list[i + 1])
		    printf(", ");
		else
		    putchar('\n');
		match += 2;
	    }
	    continue;
	}
	*sp = '\0';
	for (sp = buf; *sp; sp++)
	    *sp = tolower((unsigned char) *sp);
	for (i = n_match = 0; list[i]; i++)
	    if (comp(list[i])) {
		n_match++;
		match = i;
	    }
	if (n_match == 1)
	    return match;
	else if (buf[0] != '\0')
	    printf("Illegal response: \"%s\".  " "Use '?' to get list of valid answers\n", buf);
    }
}

static int comp(const char *s1)
{
    const char *sp, *tsp;
    char c;

    if (buf[0] != '\0')
	for (sp = buf, tsp = s1; *sp;) {
	    c = tolower((unsigned char) *tsp);
	    tsp++;
	    if (c != *sp++)
		return 0;
    } else if (*s1 != '\0')
	return 0;
    return 1;
}
