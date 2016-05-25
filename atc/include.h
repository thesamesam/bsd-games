// Copyright (c) 1987 by Ed James <edjames@berkeley.edu>
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include "def.h"
#include "struct.h"
#include "extern.h"
#include "tunable.h"
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <curses.h>
#include <math.h>
#include <pwd.h>
#include <signal.h>
#include <termios.h>
#include <time.h>

#define BSD 1
#define YY_NO_UNPUT 1

#define	_PATH_SCORE	_PATH_GAME_STATE "atc.scores"
