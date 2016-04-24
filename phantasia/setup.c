// setup.c - set up all files for Phantasia
// Creates the binary monsters file for the game state dir

#include "include.h"
#include <sys/param.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

static void Error (const char*, const char*) __attribute__ ((__noreturn__));

int main (int argc, const char *const *argv)
{
    if (argc < 3) {
	printf ("Usage: setup <monsters.asc> <monsters>\n");
	return EXIT_FAILURE;
    }
    FILE* srcfp = fopen (argv[1], "r");
    if (!srcfp)
	Error ("cannot open %s source file to create monster database\n", argv[1]);
    FILE* destfp = fopen (argv[2], "w");
    if (!destfp) {
	fclose (srcfp);
	Error ("cannot write to destination file %s\n", argv[2]);
    }
    // read in text file, convert to binary
    char buf [128];
    while (fgets (buf, SZ_DATABUF, srcfp)) {
	struct monster m;
	memset (&m, 0, sizeof(m));
	sscanf (&buf[24], "%lf%lf%lf%lf%lf%d%d%lf", &m.m_strength, &m.m_brains, &m.m_speed, &m.m_energy, &m.m_experience, &m.m_treasuretype, &m.m_type, &m.m_flock);
	buf[24] = '\0';
	strcpy (m.m_name, buf);
	fwrite ((const char*) &m, sizeof(m), 1, destfp);
	//printf ("% 24s%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", m.m_name, (int) m.m_strength, (int) m.m_brains, (int) m.m_speed, (int) m.m_energy, (int) m.m_experience, (int) m.m_treasuretype, (int) m.m_type, (int) m.m_flock);
    }
    fclose(srcfp);
    if (0 != fclose (destfp))
	Error("writing %s\n", argv[2]);
    return EXIT_SUCCESS;
}

static void Error (const char* str, const char* file)
{
    fprintf (stderr, "Error: ");
    fprintf (stderr, str, file);
    perror (file);
    exit (EXIT_FAILURE);
}
