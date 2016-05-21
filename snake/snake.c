// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
//
// snake - crt hack game.
//
// You move around the screen with arrow keys trying to pick up money
// without getting eaten by the snake. hjkl work as in vi in place of
// arrow keys. You can leave at the exit any time.

#include "../config.h"
#include <sys/param.h>
#include <curses.h>
#include <pwd.h>
#include <err.h>
#include <math.h>
#include <signal.h>
#include <termios.h>
#include <sys/uio.h>
#include <sys/file.h>

#define _PATH_SCOREFILE	_PATH_GAME_STATE "snakerawscores"
#define SCOREFILE_MAGIC	"snake"

enum {
    SPACEWARP_PENALTY	= 10,
    MAXSCORES		= 10,
    MAXSCORE		= INT_MAX
};
enum {
    color_None,
    color_Field,
    color_Snake,
    color_Me,
    color_Money,
    color_Goal
};
enum {
    ME		= A_BOLD| COLOR_PAIR(color_Me)| '@',
    SNAKEHEAD	= A_BOLD| COLOR_PAIR(color_Snake)| '@',
    SNAKETAIL	= A_BOLD| COLOR_PAIR(color_Snake)| 's',
    TREASURE	= A_BOLD| COLOR_PAIR(color_Money)| '$',
    GOAL	= A_BOLD| COLOR_PAIR(color_Goal)| '#'
};

struct point {
    unsigned short	col;
    unsigned short	line;
};

struct ScorefileHeader {
    char	magic[6];
    uint16_t	sum;
};
struct Score {
    unsigned	score;
    char	name [16-sizeof(unsigned)];
};

//----------------------------------------------------------------------

static struct point _you = {0};
static struct point _money = {0};
static struct point _finish = {0};
static struct point _snake[6] = {{0}};

static unsigned _loot = 0;
static unsigned _moves = 0;

static unsigned short _penalty = 0;	// current spacewarp penalty
static unsigned short _chunk = 0;	// amount of money given at a time

static WINDOW* _wscore = NULL;		// top line with money displayed
static WINDOW* _wgame = NULL;		// main game field

static struct Score _scores [MAXSCORES+1] = {{0,""}};

//----------------------------------------------------------------------

static void calculate_screen_size (unsigned short rcols, unsigned short rlines);
static struct point find_empty_spot (void);
static void setup (void);

static void chase (struct point *, struct point *);
static void length (void);
static bool pushsnake (void);
static void spacewarp (bool bonus);
static void stop (int);
static void surround (struct point *);
static void win (const struct point *);
static void winnings (unsigned won);

static int compare_scores (const void *x, const void *y);
static bool read_scores (void);
static void write_scores (void);
static void show_scores (void);
static bool save_score (unsigned score, bool won);

//----------------------------------------------------------------------

static inline bool same (const struct point* p1, const struct point* p2)
    { return p1->line == p2->line && p1->col == p2->col; }
static void pchar (const struct point* p, int c)
    { mvwaddch (_wgame, p->line+1, p->col+1, c); }
static inline void delay (unsigned t)
    { usleep (t*50000); }
static inline int cashvalue (void)
    { return _chunk*(_loot-_penalty)/25; }

//----------------------------------------------------------------------

int main (int argc, const char* const* argv)
{
    unsigned short rlines = 0, rcols = 0;	// user requested field size
    if (argc == 3) {
	rcols = atoi (argv[1]);
	rlines = atoi (argv[2]);
    } else if (argc == 2) {
	show_scores();
	return EXIT_SUCCESS;
    } else if (argc != 1) {
	puts ("Usage: snake [width height] [-s]");
	return EXIT_SUCCESS;
    }
    srandrand();

    if (!initscr())
	return EXIT_FAILURE;
    start_color();
    use_default_colors();
    init_pair (color_Field,	COLOR_YELLOW, COLOR_BLACK);
    init_pair (color_Snake,	COLOR_GREEN, COLOR_BLACK);
    init_pair (color_Me,	COLOR_RED, COLOR_BLACK);
    init_pair (color_Money,	COLOR_YELLOW, COLOR_BLACK);
    init_pair (color_Goal,	COLOR_CYAN, COLOR_BLACK);
    cbreak();
    noecho();
    curs_set (0);
    calculate_screen_size (rcols, rlines);

    signal (SIGINT, stop);
    signal (SIGQUIT, stop);
    signal (SIGTERM, stop);
    signal (SIGTSTP, SIG_IGN);

    box (_wgame, 0, 0);
    _finish = find_empty_spot();
    _you = find_empty_spot();
    _money = find_empty_spot();
    _snake[0] = find_empty_spot();
    for (unsigned i = 1; i < ArraySize(_snake); ++i)
	chase (&_snake[i], &_snake[i-1]);
    setup();

    for (int lastc = 0;;) {
	wrefresh (_wgame);
	int c = wgetch (_wgame);
	if (c == ERR)
	    c = lastc;
	if (c == 'q' || c == KEY_F(10))
	    break;
	else if (c == 'w' || c == ';') {
	    spacewarp (false);
	    continue;
	}
	pchar (&_you, ' ');
	if ((c == 'h' || c == KEY_LEFT) && _you.col > 0)
	    --_you.col;
	else if ((c == 'l' || c == KEY_RIGHT) && _you.col < getmaxx(_wgame)-3)
	    ++_you.col;
	else if ((c == 'k' || c == KEY_UP) && _you.line > 0)
	    --_you.line;
	else if ((c == 'j' || c == KEY_DOWN) && _you.line < getmaxy(_wgame)-3)
	    ++_you.line;
	pchar (&_you, ME);
	lastc = c;
	++_moves;
	if (same (&_you, &_money)) {
	    _loot += 25;
	    _money = find_empty_spot();
	    pchar (&_money, TREASURE);
	    winnings (cashvalue());
	    continue;
	}
	if (same (&_you, &_finish)) {
	    win (&_finish);
	    flushinp();
	    endwin();
	    printf ("You have won with $%d.\n", cashvalue());
	    save_score (cashvalue(), true);
	    break;
	}
	if (pushsnake())
	    break;
    }
    flushinp();
    endwin();
    length();
    return EXIT_SUCCESS;
}

static void calculate_screen_size (unsigned short rcols, unsigned short rlines)
{
    enum { c_MinFairFieldSize = 12 };
    if (!rlines || rlines < c_MinFairFieldSize || rlines > LINES-1)
	rlines = LINES-1;
    if (!rcols || rcols < c_MinFairFieldSize || rcols > COLS)
	rcols = COLS;
    unsigned mindim = rlines;
    if (rcols < mindim)
	mindim = rcols;
    // chunk is the amount of money the user gets for each $.
    // The formula below tries to be fair for various screen sizes.
    // We only pay attention to the smaller of the 2 edges, since
    // that seems to be the bottleneck.
    // This formula is a hyperbola which includes the following points:
    //      (24, $25)       (original scoring algorithm)
    //      (12, $40)       (experimentally derived by the "feel")
    //      (48, $15)       (a guess)
    // This will give a 4x4 screen $99/shot.  We don't allow anything
    // smaller than 4x4 because there is a 3x3 game where you can win
    // an infinite amount of money.
    //
    // Compensate for border.  This really changes the game since
    // the screen is two squares smaller but we want the default
    // to be $25, and the high scores on small screens were a bit
    // much anyway.
    mindim += 2;
    _chunk = (675.0 / (mindim + 6)) + 2.5;	// min screen edge

    // Create the score and game field windows
    _wscore = newwin (1, rcols, LINES-rlines-1, 0);
    _wgame = newwin (rlines, rcols, LINES-rlines, 0);
    keypad (_wgame, true);
    wbkgdset (_wgame, COLOR_PAIR(color_Field));
}

// setup the board
static void setup(void)
{
    werase (_wgame);
    pchar(&_you, ME);
    pchar(&_finish, GOAL);
    pchar(&_money, TREASURE);
    for (unsigned i = 1; i < ArraySize(_snake); ++i)
	pchar(&_snake[i], SNAKETAIL);
    pchar(&_snake[0], SNAKEHEAD);
    box (_wgame, 0, 0);
    wrefresh (_wgame);
}

static struct point find_empty_spot (void)
{
    struct point p;
    do {
	p.col = nrand (getmaxx(_wgame)-2);
	p.line = nrand (getmaxy(_wgame)-2);
    } while ((char) mvwinch (_wgame, p.line+1, p.col+1) != ' ');
    return p;
}

static void chase (struct point *np, struct point *sp)
{
    static const int8_t mx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    static const int8_t my[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
    static const float absv[8] = { 1, 1.4, 1, 1.4, 1, 1.4, 1, 1.4 };
    static unsigned oldw = 0;

    // this algorithm has bugs; otherwise the snake would get too good
    struct point d = { _you.col - sp->col, _you.line - sp->line };
    double v1 = sqrt((double) (d.col * d.col + d.line * d.line));
    unsigned w = 0;
    double max = 0;
    double v2, vp;
    for (unsigned i = 0; i < 8; ++i) {
	vp = d.col * mx[i] + d.line * my[i];
	v2 = absv[i];
	if (v1 > 0)
	    vp = ((double) vp) / (v1 * v2);
	else
	    vp = 1.0;
	if (vp > max) {
	    max = vp;
	    w = i;
	}
    }
    int wt[8] = {0};
    for (unsigned i = 0; i < 8; ++i) {
	d.col = sp->col + mx[i];
	d.line = sp->line + my[i];
	if (d.col >= getmaxx(_wgame)-2 || d.line >= getmaxy(_wgame)-2)
	    continue;
	// Change to allow snake to eat you if you're on the money,
	// otherwise, you can just crouch there until the snake goes
	// away. Not positive it's right.
	// if (d.line == 0 && d.col < 5) continue;
	if (same(&d, &_money))
	    continue;
	if (same(&d, &_finish))
	    continue;
	wt[i] = i == w ? _loot / 10 : 1;
	if (i == oldw)
	    wt[i] += _loot / 20;
    }
    w = 0;
    for (unsigned i = 0; i < 8; ++i)
	w += wt[i];
    vp = nrand(w);
    unsigned i = 0;
    for (; i < 8; ++i) {
	if (vp < wt[i])
	    break;
	else
	    vp -= wt[i];
    }
    if (i == 8) {
	wprintw (_wgame, "failure\n");
	i = 0;
	while (wt[i] == 0)
	    ++i;
    }
    oldw = w = i;
    np->col = sp->col + mx[w];
    np->line = sp->line + my[w];
}

static void spacewarp (bool bonus)
{
    _you = find_empty_spot();
    const char* str = "SPACE WARP!!!";
    if (bonus) {
	str = "BONUS!!!";
	_loot -= _penalty;
	_penalty = 0;
    } else
	_penalty += _loot / SPACEWARP_PENALTY;
    // Flash the message in the center of the empty screen
    wattrset (_wgame, A_BOLD|COLOR_PAIR(color_Goal));
    for (unsigned j = 0; j < 3; ++j) {
	werase (_wgame);
	box (_wgame, 0, 0);
	wrefresh (_wgame);
	delay(5);
	mvwaddstr (_wgame, getmaxy(_wgame)/2, (getmaxx(_wgame)-strlen(str))/2, str);
	wrefresh (_wgame);
	delay(10);
    }
    wattrset (_wgame, COLOR_PAIR(color_Field));
    setup();
    winnings (cashvalue());
}

static void draw_snake_head_frame (struct point* ps, const char (*f)[4])
{
    for (unsigned i = 0; i < 3; ++i)
	mvwaddstr (_wgame, ps->line+i,   ps->col, f[i]);
}

static void surround(struct point *ps)
{
    // Adjust snake to not leave the window
    if (ps->col == 0)
	++ps->col;
    if (ps->line == 0)
	++ps->line;
    if (ps->line == LINES-1)
	--ps->line;
    if (ps->col == COLS-1)
	--ps->col;

    // Eating animation frames
    enum { frame_BigMouth, frame_ClosedMouth, frame_Wink, frame_NFrames};
    static const char c_SnakeHeadFrames [frame_NFrames][3][4] = {
	{"/*\\",
	 "* *",
	 "\\*/"},	// big mouth
	{"   ",
	 "o.o",
	 "\\_/"},	// closed mouth, eyes
	{"   ",
	 "o.-",
	 "\\_/"}	// wink
    };

    draw_snake_head_frame (ps, c_SnakeHeadFrames[frame_BigMouth]);
    for (unsigned j = 0; j < 20; ++j) {		// chewing :)
	pchar (ps, '@');
	wrefresh (_wgame);
	delay(1);
	pchar (ps, ' ');
	wrefresh (_wgame);
	delay(1);
    }
    if (save_score (cashvalue(), false)) {	// wink if better score
	draw_snake_head_frame (ps, c_SnakeHeadFrames[frame_ClosedMouth]);
	wrefresh (_wgame);
	delay(6);
	draw_snake_head_frame (ps, c_SnakeHeadFrames[frame_Wink]);
	wrefresh (_wgame);
	delay(6);
    }
    draw_snake_head_frame (ps, c_SnakeHeadFrames[frame_ClosedMouth]);
    wrefresh (_wgame);
    delay(6);
}

static void win (const struct point* ps)
{
    enum { c_BoxSize = 10 };	// The size of box at full expansion
    struct point x = *ps;
    wattrset (_wgame, A_BOLD|COLOR_PAIR(color_Goal));
    for (unsigned j = 1; j < c_BoxSize; ++j) {
	for (unsigned k = 0; k < j; ++k) {
	    pchar(&x, '#');
	    --x.line;
	}
	for (unsigned k = 0; k < j; ++k) {
	    pchar(&x, '#');
	    --x.col;
	}
	j++;
	for (unsigned k = 0; k < j; ++k) {
	    pchar(&x, '#');
	    ++x.line;
	}
	for (unsigned k = 0; k < j; ++k) {
	    pchar(&x, '#');
	    ++x.col;
	}
	wrefresh (_wgame);
	delay(1);
    }
}

static bool pushsnake (void)
{
    bool issame = false;
    for (int i = 4; i >= 0; --i)
	if (same(&_snake[i], &_snake[5]))
	    issame = true;
    if (!issame)
	pchar(&_snake[5], ' ');
    // Need the following to catch you if you step on the snake's tail
    struct point tmp = _snake[5];
    for (int i = 4; i >= 0; --i)
	_snake[i+1] = _snake[i];
    chase(&_snake[0], &_snake[1]);
    pchar(&_snake[1], SNAKETAIL);
    pchar(&_snake[0], SNAKEHEAD);
    for (unsigned i = 0; i < 6; ++i) {
	if (same(&_snake[i], &_you) || same(&tmp, &_you)) {
	    surround(&_you);
	    i = cashvalue() % 10;
	    unsigned bonus = nrand(10);
	    mvwprintw (_wgame, getmaxy(_wgame)-2, 2, "Bonus %u!\n", bonus);
	    wrefresh (_wgame);
	    delay(30);
	    if (bonus == i) {
		spacewarp (true);
		return true;
	    }
	    flushinp();
	    endwin();
	    if (_loot >= _penalty)
		printf("You and your $%d have been eaten\n", cashvalue());
	    else
		printf("The snake ate you. You owe $%d.\n", -cashvalue());
	    length();
	    exit (EXIT_SUCCESS);
	}
    }
    return false;
}

static void winnings (unsigned won)
{
    if (won > 0) {
	mvwprintw (_wscore, 0, 1, "$%u ", won);
	wrefresh (_wscore);
    }
}

static void stop (int dummy UNUSED)
{
    flushinp();
    endwin();
    length();
    exit (EXIT_SUCCESS);
}

static void length (void)
{
    printf ("You made %u moves.\n", _moves);
}

//----------------------------------------------------------------------
// Scorefile reading and writing

// Score comparator for qsort, in reverse order
static int compare_scores (const void *x, const void *y)
{
    const struct Score* a = (const struct Score*) x;
    const struct Score* b = (const struct Score*) y;
    return a->score < b->score ? 1 : a->score == b->score ? 0 : -1;
}

enum { SCORESSIZE = MAXSCORES * sizeof(_scores[0]) };

static bool read_scores (void)
{
    int fd = open (_PATH_SCOREFILE, O_RDONLY);
    if (fd < 0)
	return false;
    struct ScorefileHeader header;
    struct iovec iov[2] = {{&header,sizeof(header)},{_scores,SCORESSIZE}};
    while (0 != flock (fd, LOCK_SH))
	sleep (1);
    ssize_t br = readv (fd, iov, ArraySize(iov));
    close (fd);
    // Check that score list appears valid
    bool r = true;
    if (br != sizeof(header)+SCORESSIZE
	|| memcmp (header.magic, SCOREFILE_MAGIC, sizeof(header.magic)) != 0
	|| header.sum != bsdsum (_scores, SCORESSIZE, 0))
	r = false;
    // Check each score and zero if invalid
    for (unsigned i = 0; i < MAXSCORES; ++i)
	if (!r || !_scores[i].name[0] || _scores[i].name[sizeof(_scores[i].name)-1] || _scores[i].score > MAXSCORE)
	    memset (&_scores[i], 0, sizeof(_scores[i]));
    // Resort to account for the above zeroing
    qsort (_scores, MAXSCORES, sizeof(_scores[0]), compare_scores);
    return r;
}

static void write_scores (void)
{
    int fd = open (_PATH_SCOREFILE, O_WRONLY);
    if (fd < 0)
	return;
    struct ScorefileHeader header = { SCOREFILE_MAGIC, bsdsum (_scores, SCORESSIZE, 0) };
    struct iovec iov[2] = {{&header,sizeof(header)},{_scores,SCORESSIZE}};
    while (0 != flock (fd, LOCK_EX))
	sleep (1);
    writev (fd, iov, ArraySize(iov));
    ftruncate (fd, sizeof(header)+SCORESSIZE);
    close (fd);
}

static void show_scores (void)
{
    if (read_scores()) {
	printf("Snake players scores to date:\n");
	for (unsigned i = 0; i < MAXSCORES && _scores[i].score; ++i)
	    printf("%u:\t$%u\t%s\n", i+1, _scores[i].score, _scores[i].name);
    } else
	puts ("No scores recorded yet!");
}

static bool save_score (unsigned score, bool won)
{
    read_scores();
    // Save the scores only if the new score is better than the lowest
    if (_scores[MAXSCORES-1].score >= score)
	return false;
    else if (!won)
	return true;	// Return true to wink if the score would have been saved
    // Get user name
    const char* name = getlogin();
    if (!name)
	return false;
    _scores[MAXSCORES].score = score;
    strncpy (_scores[MAXSCORES].name, name, sizeof(_scores[MAXSCORES].name)-1);
    _scores[MAXSCORES].name[sizeof(_scores[MAXSCORES].name)-1] = 0;
    // Find user's previous best score
    unsigned prevbest = 0;
    for (unsigned i = 0; i < MAXSCORES && _scores[i].score >= prevbest; ++i)
	if (0 == strcmp (_scores[i].name, _scores[MAXSCORES].name))
	    prevbest = _scores[i].score;
    // Tell him how good he is
    if (prevbest) {
	if (prevbest < score)
	    printf ("You bettered your previous best of $%u\n", prevbest);
	else if (prevbest > score)
	    printf ("Your best to date is $%d\n", prevbest);
    }
    if (_scores[0].score) {
	if (_scores[0].score > score)
	    printf ("The highest is %s with $%u\n", _scores[0].name, _scores[0].score);
	else if (_scores[0].score < score) {
	    if (0 == strcmp (_scores[0].name, _scores[MAXSCORES].name))
		printf ("You set a new record!\n");
	    else
		printf ("You beat %s's old record of $%u!\n", _scores[0].name, _scores[0].score);
	}
    }
    // Save the scores if the new score is better than the lowest
    qsort (_scores, ArraySize(_scores), sizeof(_scores[0]), compare_scores);
    write_scores();
    return true;
}
