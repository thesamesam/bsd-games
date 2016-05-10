// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "bog.h"

char *nextword(FILE *, char *, int *, int *);

int main(void)
{
    int clen, rlen, prev, i;
    long off, start;
    char buf[MAXWORDLEN + 1];

    prev = '\0';
    off = start = 0L;
    while (nextword(stdin, buf, &clen, &rlen) != NULL) {
	if (*buf != prev) {
	    // Boggle expects a full index even if the dictionary had no words
	    // beginning with some letters. So we write out entries for every
	    // letter from prev to *buf.
	    if (prev != '\0')
		printf("%c %6ld %6ld\n", prev, start, off - 1);
	    for (i = (prev ? prev + 1 : 'a'); i < *buf; i++)
		printf("%c %6ld %6ld\n", i, off, off - 1);
	    prev = *buf;
	    start = off;
	}
	off += clen + 1;
    }
    printf("%c %6ld %6ld\n", prev, start, off - 1);
    for (i = prev + 1; i <= 'z'; i++)
	printf("%c %6ld %6ld\n", i, off, off - 1);
    fflush(stdout);
    if (ferror(stdout)) {
	perror("error writing standard output");
	exit(1);
    }
    exit(0);
}

// Return the next word in the compressed dictionary in 'buffer' or
// NULL on end-of-file
// Also set clen to the length of the compressed word (for mkindex) and
// rlen to the strlen() of the real word
char* nextword (FILE* fp, char* buffer, int* clen, int* rlen)
{
    int ch, pcount;
    char *p, *q;
    static char buf[MAXWORDLEN + 1];
    static int first = 1;
    static int lastch = 0;

    if (first) {
	if ((pcount = getc(fp)) == EOF)
	    return NULL;
	first = 0;
    } else if ((pcount = lastch) == EOF)
	return NULL;

    p = buf + (*clen = pcount);

    while ((ch = getc(fp)) != EOF && ch >= 'a')
	*p++ = ch;
    lastch = ch;
    *p = '\0';

    *rlen = (int) (p - buf);
    *clen = *rlen - *clen;

    p = buf;
    q = buffer;
    while ((*q++ = *p) != '\0')
	if (*p++ == 'q')
	    *q++ = 'u';
    return buffer;
}
