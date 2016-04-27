// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

char* getcom (char* buf, int size, const char* prompt, const char* error)
{
    for (;;) {
	fputs(prompt, stdout);
	if (fgets(buf, size, stdin) == 0) {
	    if (feof(stdin))
		die();
	    clearerr(stdin);
	    continue;
	}
	while (isspace((unsigned char) *buf))
	    buf++;
	if (*buf)
	    break;
	if (error)
	    puts(error);
    }
    // If we didn't get to the end of the line, don't read it in next time.
    if (buf[strlen(buf) - 1] != '\n') {
	int i;
	while ((i = getchar()) != '\n' && i != EOF)
	    continue;
    }
    return buf;
}

// shifts to UPPERCASE if flag > 0, lowercase if flag < 0,
// and leaves it unchanged if flag = 0
char *getword(char *buf1, char *buf2, int flag)
{
    int cnt = 1;
    while (isspace((unsigned char) *buf1))
	buf1++;
    if (*buf1 != ',') {
	if (!*buf1) {
	    *buf2 = 0;
	    return 0;
	}
	while (cnt < WORDLEN && *buf1 && !isspace((unsigned char) *buf1) && *buf1 != ',')
	    if (flag < 0) {
		if (isupper((unsigned char) *buf1)) {
		    *buf2++ = tolower((unsigned char) *buf1++);
		    cnt++;
		} else {
		    *buf2++ = *buf1++;
		    cnt++;
		}
	    } else if (flag > 0) {
		if (islower((unsigned char) *buf1)) {
		    *buf2++ = toupper((unsigned char) *buf1++);
		    cnt++;
		} else {
		    *buf2++ = *buf1++;
		    cnt++;
		}
	    } else {
		*buf2++ = *buf1++;
		cnt++;
	    }
	if (cnt == WORDLEN)
	    while (*buf1 && !isspace((unsigned char) *buf1))
		buf1++;
    } else
	*buf2++ = *buf1++;
    *buf2 = '\0';
    while (isspace((unsigned char) *buf1))
	buf1++;
    return *buf1 ? buf1 : NULL;
}
