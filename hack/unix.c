// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.
// This file collects some Unix dependencies; hack.pager.c contains some more

//
// The time is used for:
//      - seed for rand()
//      - year on tombstone and yymmdd in record file
//      - phase of the moon (various monsters react to NEW_MOON or FULL_MOON)
//      - night and midnight (the undead are dangerous at midnight)
//      - determination of what files are "very old"

#include "hack.h"
#include "extern.h"
#include <sys/stat.h>
#include <time.h>
#include <signal.h>

static struct tm *getlt(void)
{
    time_t date;
    time (&date);
    return localtime(&date);
}

int getyear(void)
{
    return 1900 + getlt()->tm_year;
}

const char* getdatestr (void)
{
    static char datestr[12];
    struct tm *lt = getlt();
    snprintf(ArrayBlock(datestr), "%02hhd%02hhd%02hhd", lt->tm_year % 100, lt->tm_mon + 1, lt->tm_mday);
    return datestr;
}

int phase_of_the_moon(void)
{
    // 0-7, with 0: new, 4: full moon period: 29.5306 days. year: 365.2422 days
    struct tm *lt = getlt();
    int epact, diy, golden;

    diy = lt->tm_yday;
    golden = (lt->tm_year % 19) + 1;
    epact = (11 * golden + 18) % 30;
    if ((epact == 25 && golden > 11) || epact == 24)
	++epact;

    return (((((diy + epact) * 6) + 11) % 177) / 22) & 7;
}

int night(void)
{
    int hour = getlt()->tm_hour;

    return hour < 6 || hour > 21;
}

int midnight(void)
{
    return getlt()->tm_hour == 0;
}
