// Copyright (c) 1992 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
//
// Tetris (or however it is spelled).

#include "tetris.h"
#include "scores.h"
#include <signal.h>
#include <time.h>

// A shape is the fundamental thing that makes up the game.  There
// are 7 basic shapes, comprising all possible combinations of 4 blocks.
//
//      X.X       X.X           X.X
//        X.X   X.X     X.X.X   X.X     X.X.X   X.X.X   X.X.X.X
//                        X             X           X
//
//        0       1       2       3       4       5       6
//
// Except for 3 and 6, the center of each shape is one of the blocks.
// This block is designated (0,0).  The other three blocks can then be
// described as offsets from the center.  Shape 3 is the same under
// rotation, so its center is effectively irrelevant; it has been chosen
// so that it `sticks out' upward and leftward.  Except for shape 6,
// all the blocks are contained in a box going from (-1,-1) to (+1,+1);
// shape 6's center `wobbles' as it rotates, so that while it `sticks out'
// rightward, its rotation---a vertical line---`sticks out' downward.
// The containment box has to include the offset (2,0), making the overall
// containment box range from offset (-1,-1) to (+2,+1).  (This is why
// there is only one row above, but two rows below, the display area.)
//
// The game works by choosing one of these shapes at random and putting
// its center at the middle of the first display row (row 1, column 5).
// The shape is moved steadily downward until it collides with something:
// either  another shape, or the bottom of the board.  When the shape can
// no longer be moved downwards, it is merged into the current board.
// At this time, any completely filled rows are elided, and blocks above
// these rows move down to make more room.  A new random shape is again
// introduced at the top of the board, and the whole process repeats.
// The game ends when the new shape will not fit at (1,5).
//
// While the shapes are falling, the user can rotate them counterclockwise
// 90 degrees (in addition to moving them left or right), provided that the
// rotation puts the blocks in empty spaces.  The table of shapes is set up
// so that each shape contains the index of the new shape obtained by
// rotating the current shape.  Due to symmetry, each shape has exactly
// 1, 2, or 4 rotations total; the first 7 entries in the table represent
// the primary shapes, and the remaining 12 represent their various
// rotated forms.

struct shape {
    uint8_t rot;	// index of rotated version of this shape
    struct {
	int8_t	dy:4;
	int8_t	dx:4;
    } off[3];		// offsets to other blocks if center is at (0,0)
};

//----------------------------------------------------------------------
// Local variables

static uint8_t _board [B_ROWS][B_COLS] = {{0}};	// 1 => occupied, 0 => empty

// Scoring is as follows:
//
// When the shape comes to rest, and is integrated into the board,
// we score one point.  If the shape is high up (at a low-numbered row),
// and the user hits the space bar, the shape plummets all the way down,
// and we score a point for each row it falls (plus one more as soon as
// we find that it is at rest and integrate it---until then, it can
// still be moved or rotated).
static unsigned _score = 0;

static unsigned _movedelay = 500;	// Delay between movements in us
static struct timespec _nextmove = {0,0};

static bool _paused = false;

static const struct shape* _curshape = NULL;
static const struct shape* _nextshape = NULL;

static WINDOW* _win = NULL;

//----------------------------------------------------------------------
// Local prototypes

static void onintr(int);
static unsigned ms_to_next_move_time (void);
static void set_next_move_time (void);
static struct timespec get_current_time (void);
static unsigned get_level (void);
static void set_level (unsigned l);

static void gameloop (void);
static void remove_full_rows(void);

static const struct shape* rotateshape (void);
static const struct shape* randshape (void);
static void place (const struct shape *shape, unsigned y, unsigned x, bool onoff);
static bool fits_in (const struct shape* shape, unsigned y, unsigned x);

static void scr_start (void);
static void scr_create_window (void);
static void scr_end (void);
static void scr_update (void);
static void showscores (unsigned level);

//----------------------------------------------------------------------
// main

int main (int argc, const char* const* argv)
{
    if (argc > 2) {
	printf ("Usage: tetris [level]\n");
	return EXIT_SUCCESS;
    } else if (argc == 2) {
	int level = atoi(argv[1]);
	if (level < MINLEVEL || level > MAXLEVEL) {
	    printf ("Level must be from %u to %u", MINLEVEL, MAXLEVEL);
	    return EXIT_FAILURE;
	}
	set_level (level);
    }

    signal (SIGINT, onintr);
    signal (SIGSEGV, onintr);
    signal (SIGABRT, onintr);
    signal (SIGTSTP, SIG_IGN);	// Disable Ctrl+z
    srandrand();

    scr_start();
    gameloop();
    savescore (_score, get_level());
    showscores (get_level());
    scr_end();
    return EXIT_SUCCESS;
}

static void onintr (int signo)
{
    scr_end();
    psignal (signo, "Fatal error");
    exit (EXIT_SUCCESS);
}

static struct timespec get_current_time (void)
{
    struct timespec now;
    if (0 > clock_gettime (CLOCK_REALTIME, &now)) {
	perror ("clock_gettime");
	exit (EXIT_FAILURE);
    }
    return now;
}

static void set_next_move_time (void)
{
    _nextmove = get_current_time();
    _nextmove.tv_nsec += _movedelay * 1000000;
    if (_nextmove.tv_nsec > 1000000000) {
	_nextmove.tv_nsec -= 1000000000;
	++_nextmove.tv_sec;
    }
}

static unsigned ms_to_next_move_time (void)
{
    const struct timespec now = get_current_time();
    int dsec = _nextmove.tv_sec - now.tv_sec;
    int dmsec = dsec*1000 + (_nextmove.tv_nsec - now.tv_nsec)/1000000;
    return dmsec < 0 ? 0 : dmsec;
}

static unsigned get_level (void)
{
    return 1000/_movedelay;
}

static void set_level (unsigned l)
{
    _movedelay = 1000/l;
}

//----------------------------------------------------------------------
// Game loop

static void gameloop (void)
{
    set_next_move_time();
    _nextshape = randshape();
    _curshape = randshape();

    for (int y = B_STARTROW, x = B_STARTCOL;;) {
	place (_curshape, y, x, 1);
	scr_update();
	place (_curshape, y, x, 0);
	wtimeout (_win, ms_to_next_move_time());
	int c = wgetch (_win);
	if (c == 'q' || c == KEY_F(10) || c == KEY_END)	// quit
	    break;
	else if (c == KEY_RESIZE)			// screen resized, recreate window
	    scr_create_window();
	else if (c == '>' && get_level() < MAXLEVEL)	// go faster
	    set_level (get_level()+1);
	else if ((c == 'j' || c == KEY_LEFT) && fits_in(_curshape, y, x-1))	// move left
	    --x;
	else if ((c == 'l' || c == KEY_RIGHT) && fits_in(_curshape, y, x+1))	// move right
	    ++x;
	else if (c == ' ' || c == KEY_DOWN) {		// move to bottom
	    while (fits_in (_curshape, y+1, x))
		++y, ++_score;
	    c = ERR;	// End turn
	} else if (c == 'k' || c == KEY_UP || c == KEY_B2) {	// turn
	    const struct shape* rotshape = rotateshape();
	    if (fits_in (rotshape, y, x))
		_curshape = rotshape;
	} else if (c == 'p' || c == KEY_BACKSPACE) {	// pause
	    _paused = true;
	    scr_update();
	    wtimeout (_win, -1);
	    if (KEY_RESIZE == wgetch (_win))
		scr_create_window();			// handle window resize while paused
	    _paused = false;
	}
	if (c == ERR) {	// Timeout.  Move down if possible.
	    set_next_move_time();
	    if (fits_in (_curshape, y+1, x))
		++y;
	    else {		// Current shape has reached the bottom
		place (_curshape, y, x, 1);
		++_score;
		remove_full_rows();

		// Choose a new shape.  If it does not fit, the game is over.
		_curshape = _nextshape;
		_nextshape = randshape();
		if (!fits_in(_curshape, y = B_STARTROW, x = B_STARTCOL))
		    break;
	    }
	}
    }
}

static void remove_full_rows (void)
{
    unsigned firstNonempty = B_ROWS;
    for (unsigned r = 0; r < B_ROWS; ++r) {
	bool rowEmpty = true, rowFull = true;
	for (unsigned c = 0; c < B_COLS; ++c) {
	    if (_board[r][c])
		rowEmpty = false;
	    else
		rowFull = false;
	}
	if (!rowEmpty && firstNonempty > r)
	    firstNonempty = r;
	if (rowFull && r > firstNonempty) {
	    memset (_board[r], 0, B_COLS);
	    scr_update();
	    usleep (250000);
	    memmove (_board[firstNonempty+1], _board[firstNonempty], (r-firstNonempty)*sizeof(_board[0]));
	    memset (_board[firstNonempty], 0, B_COLS);
	    ++firstNonempty;
	    scr_update();
	    usleep (250000);
	    // Bump level every N rows
	    static unsigned s_RowsRemoved = 0;
	    unsigned level = get_level();
	    if (level < MAXLEVEL && ++s_RowsRemoved >= ROWS_PER_LEVEL) {
		s_RowsRemoved = 0;
		set_level (get_level()+1);
	    }
	}
    }
}

//----------------------------------------------------------------------
// Shapes and operations on them

static const struct shape shapes[] = {
    { 7, {{-1,-1}, {-1, 0}, {0, 1}}},
    { 8, {{-1, 0}, {-1, 1}, {0,-1}}},
    { 9, {{ 0,-1}, { 0, 1}, {1, 0}}},
    { 3, {{-1,-1}, {-1, 0}, {0,-1}}},
    {12, {{ 0,-1}, { 1,-1}, {0, 1}}},
    {15, {{ 0,-1}, { 1, 1}, {0, 1}}},
    {18, {{ 0,-1}, { 0, 1}, {0, 2}}},
    { 0, {{-1, 0}, { 0,-1}, {1,-1}}},
    { 1, {{-1, 0}, { 0, 1}, {1, 1}}},
    {10, {{-1, 0}, { 0, 1}, {1, 0}}},
    {11, {{-1, 0}, { 0,-1}, {0, 1}}},
    { 2, {{-1, 0}, { 0,-1}, {1, 0}}},
    {13, {{-1, 0}, { 1, 0}, {1, 1}}},
    {14, {{-1, 1}, { 0,-1}, {0, 1}}},
    { 4, {{-1,-1}, {-1, 0}, {1, 0}}},
    {16, {{-1, 1}, {-1, 0}, {1, 0}}},
    {17, {{-1,-1}, { 0, 1}, {0,-1}}},
    { 5, {{-1, 0}, { 1, 0}, {1,-1}}},
    { 6, {{-1, 0}, { 1, 0}, {2, 0}}}
};

//----------------------------------------------------------------------

static inline const struct shape* rotateshape (void)
    { return &shapes[_curshape->rot]; }
static inline const struct shape* randshape (void)
    { return &shapes[rand() % 7]; }

// Return true iff the given shape fits in the given position,
// taking the current board into account.
static bool fits_in (const struct shape* shape, unsigned y, unsigned x)
{
    unsigned py = y, px = x, ip = 0;
    for (;;) {
	if (py >= B_ROWS || px >= B_COLS || _board[py][px])
	    return false;
	if (ip < ArraySize(shape->off)) {
	    py = y + shape->off[ip].dy;
	    px = x + shape->off[ip++].dx;
	} else break;
    }
    return true;
}

// Write the given shape into the current board, turning it on
// if onoff is true, and off if onoff is false.
static void place (const struct shape* shape, unsigned y, unsigned x, bool onoff)
{
    unsigned py = y, px = x, ip = 0;
    for (;;) {
	assert (py < B_ROWS && px < B_COLS);
	_board[py][px] = onoff;
	if (ip < ArraySize(shape->off)) {
	    py = y + shape->off[ip].dy;
	    px = x + shape->off[ip++].dx;
	} else break;
    }
}

//----------------------------------------------------------------------
// Game window drawing

enum {
    color_None,
    color_Default,
    color_Background,
    color_Text,
    color_HelpText,
    color_Borders,
    color_Shapes,
    color_Last
};

static void scr_start (void)
{
    if (!initscr()) {
	printf ("Error: unable to initialize this terminal for graphics.");
	exit (EXIT_FAILURE);
    }
    atexit (scr_end);

    scr_create_window();
    if (has_colors()) {
	start_color();
	use_default_colors();
	init_pair (color_Default, COLOR_DEFAULT, COLOR_DEFAULT);
	init_pair (color_Background, COLOR_DEFAULT, COLOR_DEFAULT);
	init_pair (color_Text, COLOR_CYAN, COLOR_BLACK);
	init_pair (color_HelpText, COLOR_BLACK, COLOR_DEFAULT);
	init_pair (color_Borders, COLOR_MAGENTA, COLOR_BLACK);
	init_pair (color_Shapes, COLOR_CYAN, COLOR_BLACK);
    }
    noecho();
    nodelay (_win, true);
    keypad (_win, true);
    cbreak();
    curs_set (0);
}

static void scr_create_window (void)
{
    if (_win)
	delwin (_win);
    _win = newwin (S_ROWS, S_COLS, LINES-S_ROWS, 0);
}

static void scr_end (void)
{
    if (isendwin())
	return;
    delwin (_win);
    _win = NULL;
    endwin();
}

static void scr_drawbar (unsigned y, unsigned x, unsigned h, unsigned w)
{
    for (unsigned i = 0; i < h; ++i)
	mvwhline (_win, y+i, x, ' ', w);
}

static void scr_update(void)
{
    werase (_win);
    wattrset (_win, COLOR_PAIR(color_Background));
    scr_drawbar (0, 0, S_ROWS, S_COLS);

    // Explicitly draw black boxes under
    unsigned brdx = (S_COLS-B_COLS*2)/2;
    wattrset (_win, COLOR_PAIR(color_Shapes));
    scr_drawbar (0, brdx, B_ROWS, B_COLS*2);
    scr_drawbar (1, 2, 8, 15);

    // Draw score and level
    wattrset (_win, COLOR_PAIR(color_Text));
    mvwprintw (_win, 2, 4, "Score: %d", _score);
    mvwprintw (_win, 3, 4, "Level: %u", get_level());

    if (_paused)
	mvwaddstr (_win, S_ROWS/2, (S_COLS-strlen("Paused"))/2, "Paused");
    else {
	// draw preview of nextpattern
	wattrset (_win, A_REVERSE| COLOR_PAIR(color_Shapes));
	unsigned py = 1, px = 1, ip = 0;
	for (;;) {
	    mvwaddstr (_win, 5+py, 4+px*2, "  ");
	    if (ip < ArraySize(_nextshape->off)) {
		py = 1 + _nextshape->off[ip].dy;
		px = 1 + _nextshape->off[ip++].dx;
	    } else break;
	}

	// Draw the board
	for (unsigned y = 0; y < B_ROWS; ++y)
	    for (unsigned x = 0; x < B_COLS; ++x)
		if (_board[y][x])
		    mvwaddstr (_win, y, brdx + 2*x, "  ");
    }

    // Draw the board border
    wattrset (_win, A_REVERSE| COLOR_PAIR(color_Borders));
    mvwvline (_win, 0, brdx-1, ' ', B_ROWS+1);
    mvwvline (_win, 0, brdx-2, ' ', B_ROWS+1);
    mvwvline (_win, 0, brdx+B_COLS*2, ' ', B_ROWS+1);
    mvwvline (_win, 0, brdx+B_COLS*2+1, ' ', B_ROWS+1);
    mvwhline (_win, B_ROWS, brdx, ' ', B_COLS*2);
    wattroff (_win, A_REVERSE);
    mvwvline (_win, 0, brdx-3, ACS_VLINE, B_ROWS+1);
    mvwvline (_win, 0, brdx+B_COLS*2+2, ACS_VLINE, B_ROWS+1);

    wattrset (_win, COLOR_PAIR(color_Default));
    wrefresh (_win);
}

// Show current scores.
static void showscores (unsigned level)
{
    const char* username = getlogin();
    werase (_win);
    mvwaddstr (_win, 0, 0, "Tetris High Scores");
    mvwaddstr (_win, 1, 0, "Rank  Score   Name             (points/level)");
    for (unsigned i = 0; i < MAXHISCORES && _scores[i].score; ++i) {
	const struct highscore* sp = &_scores[i];
	bool bMeOnThisRow = (sp->level == level && sp->score == _score && username && !strcmp(sp->name, username));
	if (bMeOnThisRow)
	    wstandout (_win);
	mvwprintw (_win, 2+i, 0, "%3u  %6u   %-16s (%7u on %u)", i+1, sp->score * sp->level, sp->name, sp->score, sp->level);
	if (bMeOnThisRow)
	    wstandend (_win);
    }
    mvwprintw (_win, 22, 0, "Your score is %u", _score * level);
    mvwaddstr (_win, 23, 0, "Hit RETURN to continue.");
    wrefresh (_win);
    wtimeout (_win, -1);
    wgetch (_win);
}
