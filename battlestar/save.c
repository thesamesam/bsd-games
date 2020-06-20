// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

void restore (const char *filename)
{
    if (filename == NULL)
	exit (EXIT_FAILURE);	       // Error determining save file name.
    FILE* fp = fopen (filename, "r");
    if (!fp) {
	perror (filename);
	exit (EXIT_FAILURE);
    }
    fread (&WEIGHT, sizeof WEIGHT, 1, fp);
    fread (&CUMBER, sizeof CUMBER, 1, fp);
    fread (&ourclock, sizeof ourclock, 1, fp);
    fread (&game_states, sizeof game_states, 1, fp);
    restore_saved_objects (fp);
    fread (injuries, sizeof injuries, 1, fp);
    fread (&direction, sizeof direction, 1, fp);
    fread (&position, sizeof position, 1, fp);
    fread (&ourtime, sizeof ourtime, 1, fp);
    fread (&fuel, sizeof fuel, 1, fp);
    fread (&torps, sizeof torps, 1, fp);
    fread (&carrying, sizeof carrying, 1, fp);
    fread (&encumber, sizeof encumber, 1, fp);
    fread (&rythmn, sizeof rythmn, 1, fp);
    fread (&followfight, sizeof followfight, 1, fp);
    fread (&ate, sizeof ate, 1, fp);
    fread (&snooze, sizeof snooze, 1, fp);
    fread (&followgod, sizeof followgod, 1, fp);
    fread (&godready, sizeof godready, 1, fp);
    fread (&win, sizeof win, 1, fp);
    fread (&wintime, sizeof wintime, 1, fp);
    fread (&matchcount, sizeof matchcount, 1, fp);
    fread (&loved, sizeof loved, 1, fp);
    fread (&pleasure, sizeof pleasure, 1, fp);
    fread (&power, sizeof power, 1, fp);
    // We must check the last read, to catch truncated save files
    if (fread (&ego, sizeof ego, 1, fp) < 1) {
	printf ("save file %s too short", filename);
	exit (EXIT_FAILURE);
    }
    fclose(fp);
}

void save (const char *filename)
{
    if (filename == NULL)
	return;		       // Error determining save file name.
    FILE* fp = fopen (filename, "w");
    if (!fp) {
	perror (filename);
	return;
    }
    printf ("Saved in %s.\n", filename);
    fwrite (&WEIGHT, sizeof WEIGHT, 1, fp);
    fwrite (&CUMBER, sizeof CUMBER, 1, fp);
    fwrite (&ourclock, sizeof ourclock, 1, fp);
    fwrite (&game_states, sizeof game_states, 1, fp);
    save_objects (fp);
    fwrite (injuries, sizeof injuries, 1, fp);
    fwrite (&direction, sizeof direction, 1, fp);
    fwrite (&position, sizeof position, 1, fp);
    fwrite (&ourtime, sizeof ourtime, 1, fp);
    fwrite (&fuel, sizeof fuel, 1, fp);
    fwrite (&torps, sizeof torps, 1, fp);
    fwrite (&carrying, sizeof carrying, 1, fp);
    fwrite (&encumber, sizeof encumber, 1, fp);
    fwrite (&rythmn, sizeof rythmn, 1, fp);
    fwrite (&followfight, sizeof followfight, 1, fp);
    fwrite (&ate, sizeof ate, 1, fp);
    fwrite (&snooze, sizeof snooze, 1, fp);
    fwrite (&followgod, sizeof followgod, 1, fp);
    fwrite (&godready, sizeof godready, 1, fp);
    fwrite (&win, sizeof win, 1, fp);
    fwrite (&wintime, sizeof wintime, 1, fp);
    fwrite (&matchcount, sizeof matchcount, 1, fp);
    fwrite (&loved, sizeof loved, 1, fp);
    fwrite (&pleasure, sizeof pleasure, 1, fp);
    fwrite (&power, sizeof power, 1, fp);
    fwrite (&ego, sizeof ego, 1, fp);
    fflush(fp);
    if (ferror(fp))
	perror (filename);
    fclose(fp);
}

// Given a save file name (possibly from getline, so without terminating NUL),
// determine the name of the file to be saved to by adding the HOME
// directory if the name does not contain a slash.  Name will be allocated
// with malloc(3).
char* save_file_name (const char* filename, size_t len)
{
    char *home;
    char *newname;
    size_t tmpl;

    if (memchr(filename, '/', len)) {
	newname = malloc(len + 1);
	if (newname == NULL) {
	    printf ("Out of memory");
	    return NULL;
	}
	memcpy(newname, filename, len);
	newname[len] = 0;
    } else {
	home = getenv("HOME");
	if (home != NULL) {
	    tmpl = strlen(home);
	    newname = malloc(tmpl + len + 2);
	    if (newname == NULL) {
		printf ("Out of memory");
		return NULL;
	    }
	    memcpy(newname, home, tmpl);
	    newname[tmpl] = '/';
	    memcpy(newname + tmpl + 1, filename, len);
	    newname[tmpl + len + 1] = 0;
	} else {
	    newname = malloc(len + 1);
	    if (newname == NULL) {
		printf ("Out of memory");
		return NULL;
	    }
	    memcpy(newname, filename, len);
	    newname[len] = 0;
	}
    }
    return newname;
}
