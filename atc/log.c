// Copyright (c) 1987 by Ed James <edjames@berkeley.edu>
// This file is free software, distributed under the BSD license.

#include "include.h"

static FILE *score_fp;

int compar(const void *va, const void *vb)
{
    const SCORE *a, *b;

    a = (const SCORE *) va;
    b = (const SCORE *) vb;
    if (b->planes == a->planes)
	return b->time - a->time;
    else
	return b->planes - a->planes;
}

#define SECAMIN		60
#define MINAHOUR	60
#define HOURADAY	24
#define SECAHOUR	(SECAMIN * MINAHOUR)
#define SECADAY		(SECAHOUR * HOURADAY)
#define DAY(t)		((t) / SECADAY)
#define HOUR(t)		(((t) % SECADAY) / SECAHOUR)
#define MIN(t)		(((t) % SECAHOUR) / SECAMIN)
#define SEC(t)		((t) % SECAMIN)

const char *timestr(int t)
{
    static char s[80];

    if (DAY(t) > 0)
	(void) sprintf(s, "%dd+%02dhrs", DAY(t), HOUR(t));
    else if (HOUR(t) > 0)
	(void) sprintf(s, "%d:%02d:%02d", HOUR(t), MIN(t), SEC(t));
    else if (MIN(t) > 0)
	(void) sprintf(s, "%d:%02d", MIN(t), SEC(t));
    else if (SEC(t) > 0)
	(void) sprintf(s, ":%02d", SEC(t));
    else
	*s = '\0';

    return s;
}

void open_score_file(void)
{
    mode_t old_mask;
    int score_fd;
    int flags;

    old_mask = umask(0);
    score_fd = open(_PATH_SCORE, O_CREAT | O_RDWR, 0664);
    umask(old_mask);
    if (score_fd < 0) {
	warn("open %s", _PATH_SCORE);
	return;
    }
    if (score_fd < 3)
	exit(1);
    // Set the close-on-exec flag.  If this fails for any reason, quit
    // rather than leave the score file open to tampering.
    flags = fcntl(score_fd, F_GETFD);
    if (flags < 0)
	err(1, "fcntl F_GETFD");
    flags |= FD_CLOEXEC;
    if (fcntl(score_fd, F_SETFD, flags) == -1)
	err(1, "fcntl F_SETFD");
    // This is done to take advantage of stdio, while still
    // allowing a O_CREAT during the open(2) of the log file.
    score_fp = fdopen(score_fd, "r+");
    if (score_fp == NULL) {
	warn("fdopen %s", _PATH_SCORE);
	return;
    }
}

int log_score(int list_em)
{
    int i, num_scores = 0, good, changed = 0, found = 0;
    struct passwd *pw;
    char *cp;
    SCORE score[100], thisscore;
    struct utsname name;
    long offset;

    if (score_fp == NULL) {
	warnx("no score file available");
	return -1;
    }
#ifdef BSD
    if (flock(fileno(score_fp), LOCK_EX) < 0)
#endif
#ifdef SYSV
	while (lockf(fileno(score_fp), F_LOCK, 1) < 0)
#endif
	{
	    warn("flock %s", _PATH_SCORE);
	    return -1;
	}
    for (;;) {
	good = fscanf(score_fp, SCORE_SCANF_FMT, score[num_scores].name, score[num_scores].host, score[num_scores].game, &score[num_scores].planes, &score[num_scores].time, &score[num_scores].real_time);
	if (good != 6 || ++num_scores >= NUM_SCORES)
	    break;
    }
    if (!test_mode && !list_em && sp && sp->name) {
	if ((pw = (struct passwd *) getpwuid(getuid())) == NULL) {
	    fprintf(stderr, "getpwuid failed for uid %d.  Who are you?\n", (int) getuid());
	    return -1;
	}
	strcpy(thisscore.name, pw->pw_name);
	uname(&name);
	snprintf(thisscore.host, sizeof(thisscore.host), "%s", name.nodename);
	strcpy(thisscore.game, sp->name);

	thisscore.time = clck;
	thisscore.planes = safe_planes;
	thisscore.real_time = time(0) - start_time;

	for (i = 0; i < num_scores; i++) {
	    if (strcmp(thisscore.name, score[i].name) == 0 && strcmp(thisscore.host, score[i].host) == 0 && strcmp(thisscore.game, score[i].game) == 0) {
		if (thisscore.time > score[i].time) {
		    score[i].time = thisscore.time;
		    score[i].planes = thisscore.planes;
		    score[i].real_time = thisscore.real_time;
		    changed++;
		}
		found++;
		break;
	    }
	}
	if (!found) {
	    for (i = 0; i < num_scores; i++) {
		if (thisscore.time > score[i].time) {
		    if (num_scores < NUM_SCORES)
			num_scores++;
		    memcpy(&score[num_scores - 1], &score[i], sizeof(score[i]));
		    memcpy(&score[i], &thisscore, sizeof(score[i]));
		    changed++;
		    break;
		}
	    }
	}
	if (!found && !changed && num_scores < NUM_SCORES) {
	    memcpy(&score[num_scores], &thisscore, sizeof(score[num_scores]));
	    num_scores++;
	    changed++;
	}

	if (changed) {
	    if (found)
		puts("You beat your previous score!");
	    else
		puts("You made the top players list!");
	    qsort(score, num_scores, sizeof(*score), compar);
	    rewind(score_fp);
	    for (i = 0; i < num_scores; i++)
		fprintf(score_fp, "%s %s %s %d %d %d\n", score[i].name, score[i].host, score[i].game, score[i].planes, score[i].time, score[i].real_time);
	    fflush(score_fp);
	    if (ferror(score_fp))
		warn("error writing %s", _PATH_SCORE);
	    // It is just possible that updating an entry could
	    // have reduced the length of the file, so we
	    // truncate it.  The seeks are required for stream/fd
	    // synchronisation by POSIX.1.
	    offset = ftell(score_fp);
	    lseek(fileno(score_fp), 0, SEEK_SET);
	    ftruncate(fileno(score_fp), offset);
	    rewind(score_fp);
	} else {
	    if (found)
		puts("You didn't beat your previous score.");
	    else
		puts("You didn't make the top players list.");
	}
	putchar('\n');
    }
#ifdef BSD
    flock(fileno(score_fp), LOCK_UN);
#endif
#ifdef SYSV
    // lock will evaporate upon close
#endif
    fclose(score_fp);
    printf("%2s:  %-8s  %-8s  %-18s  %4s  %9s  %4s\n", "#", "name", "host", "game", "time", "real time", "planes safe");
    puts("-------------------------------------------------------------------------------");
    for (i = 0; i < num_scores; i++) {
	cp = strchr(score[i].host, '.');
	if (cp != NULL)
	    *cp = '\0';
	printf("%2d:  %-8s  %-8s  %-18s  %4d  %9s  %4d\n", i + 1, score[i].name, score[i].host, score[i].game, score[i].time, timestr(score[i].real_time), score[i].planes);
    }
    putchar('\n');
    return 0;
}

void log_score_quit(int dummy UNUSED)
{
    log_score(0);
    exit(0);
}
