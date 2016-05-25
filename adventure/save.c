// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "hdr.h"
#include "extern.h"
#include <sys/uio.h>
#include <sys/stat.h>

static struct iovec s_SaveArray[] = {
    {&abbnum, sizeof(abbnum)},
    {&attack, sizeof(attack)},
    {&blklin, sizeof(blklin)},
    {&bonus, sizeof(bonus)},
    {&chloc, sizeof(chloc)},
    {&chloc2, sizeof(chloc2)},
    {&clock1, sizeof(clock1)},
    {&clock2, sizeof(clock2)},
    {&closed, sizeof(closed)},
    {&closng, sizeof(closng)},
    {&daltlc, sizeof(daltlc)},
    {&detail, sizeof(detail)},
    {&dflag, sizeof(dflag)},
    {&dkill, sizeof(dkill)},
    {&dtotal, sizeof(dtotal)},
    {&foobar, sizeof(foobar)},
    {&gaveup, sizeof(gaveup)},
    {&holdng, sizeof(holdng)},
    {&iwest, sizeof(iwest)},
    {&k, sizeof(k)},
    {&k2, sizeof(k2)},
    {&knfloc, sizeof(knfloc)},
    {&kq, sizeof(kq)},
    {&limit, sizeof(limit)},
    {&lmwarn, sizeof(lmwarn)},
    {&loc, sizeof(loc)},
    {&maxdie, sizeof(maxdie)},
    {&mxscor, sizeof(mxscor)},
    {&newloc, sizeof(newloc)},
    {&numdie, sizeof(numdie)},
    {&obj, sizeof(obj)},
    {&oldlc2, sizeof(oldlc2)},
    {&oldloc, sizeof(oldloc)},
    {&panic, sizeof(panic)},
    {&scorng, sizeof(scorng)},
    {&spk, sizeof(spk)},
    {&stick, sizeof(stick)},
    {&tally, sizeof(tally)},
    {&tally2, sizeof(tally2)},
    {&turns, sizeof(turns)},
    {&verb, sizeof(verb)},
    {&wzdark, sizeof(wzdark)},
    {atloc, sizeof(atloc)},
    {dloc, sizeof(dloc)},
    {dseen, sizeof(dseen)},
    {fixed, sizeof(fixed)},
    {hinted, sizeof(hinted)},
    {links, sizeof(links)},
    {odloc, sizeof(odloc)},
    {place, sizeof(place)},
    {prop, sizeof(prop)},
    {tk, sizeof(tk)}
};

static uint16_t sum_save_array (void)
{
    uint16_t sum = 0;
    for (unsigned i = 0; i < ArraySize(s_SaveArray); ++i)
	sum = bsdsum ((const uint8_t*) s_SaveArray[i].iov_base, s_SaveArray[i].iov_len, sum);
    return sum;
}

// Creates all directories in path
static int mkpath (const char* path, mode_t mode)
{
    char pbuf [PATH_MAX], *pbe = pbuf;
    do {
	if (*path == '/' || !*path) {
	    *pbe = '\0';
	    if (pbuf[0] && 0 > mkdir (pbuf, mode) && errno != EEXIST)
		return -1;
	}
	*pbe++ = *path;
    } while (*path++);
    return 0;
}

struct SaveHeader {
    char magictext[6];
    uint16_t sum;
};

// to output the data using checksum to start random #s
static int save (const char* outfile)
{
    int fd = open (outfile, O_CREAT| O_EXCL| O_WRONLY, 0600);
    if (fd < 0)
	goto error;
    struct SaveHeader header = {{'a','d','v','e','n','t'}, sum_save_array()};
    if (write (fd, &header, sizeof(header)) != sizeof(header))
	goto error;
    if (writev (fd, s_SaveArray, ArraySize(s_SaveArray)) <= 0)
	goto error;
    if (close (fd) < 0)
	goto error;
    return 0;
error:
    fprintf (stderr, "Failed to write \"%s\"", outfile);
    perror ("");
    close (fd);
    return 1;
}

int restore (const char* infile)
{
    int fd = open (infile, O_RDONLY);
    if (fd < 0)
	goto error;
    struct SaveHeader header;
    if (read (fd, &header, sizeof(header)) != sizeof(header))
	goto error;
    if (memcmp (header.magictext, "advent", sizeof(header.magictext)) != 0)
	goto badfile;
    if (readv (fd, s_SaveArray, ArraySize(s_SaveArray)) <= 0)
	goto error;
    if (close (fd) < 0)
	goto error;
    if (header.sum != sum_save_array())
	goto badfile;
    return 0;
error:
    fprintf (stderr, "Failed to read \"%s\"", infile);
    perror ("");
    close (fd);
    return 1;
badfile:
    close (fd);
    return 2;
}

void ciao (void)
{
    const char* homedir = getenv ("HOME");
    if (!homedir) {
	printf ("You can not save because you have no home directory\n");
	return;
    }
    char savename [PATH_MAX];
    snprintf (savename, sizeof(savename), ADVENTURE_SAVE_DIR, homedir);
    if (0 != access (savename, R_OK))
	mkpath (savename, 0700);
    if (0 != access (savename, W_OK)) {
	printf ("You are not allowed to write to %s\n", savename);
	return;
    }
    snprintf (savename, sizeof(savename), ADVENTURE_SAVE_NAME, homedir);
    if (0 != save (savename))
	return;
    printf ("Game saved. Goodbye!\n");
    exit (EXIT_SUCCESS);
}
