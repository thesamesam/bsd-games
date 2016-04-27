// Copyright (c) 1987 by Ed James <edjames@berkeley.edu>
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/wait.h>

#include <ctype.h>
#include <curses.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <paths.h>

#include "def.h"
#include "struct.h"
#include "extern.h"
#include "tunable.h"

#define BSD 1
#define YY_NO_UNPUT 1

#define	_PATH_GAMES	_PATH_GAME_DATA "atc/"
#define	_PATH_SCORE	_PATH_GAME_STATE "atc_score"
