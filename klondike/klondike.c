#include "../config.h"
#include <curses.h>

//----------------------------------------------------------------------

typedef uint8_t card_t;
enum { suit_Spades, suit_Diamonds, suit_Clubs, suit_Hearts, NSUITS };
enum {
    rank_Ace, rank_2, rank_3, rank_4, rank_5,
    rank_6, rank_7, rank_8, rank_9, rank_Ten,
    rank_Jack, rank_Queen, rank_King, NRANKS
};
enum {
    NCARDS	= NSUITS*NRANKS,
    NSORTING	= 7,
    NFINAL	= NSUITS,
    WLINES	= 22,
    WCOLS	= 60
};
enum {
    color_None,
    color_HiddenCard,
    color_BlackSuit,
    color_RedSuit,
    color_SelectedBlackSuit,
    color_SelectedRedSuit
};

struct Pile {
    card_t	c [NCARDS];
    uint8_t	sz;
    uint8_t	nHidden;
};
enum {
    PDECK,
    PSORTING,
    PDEALT = PSORTING+NSORTING,
    PFINAL,
    NPILES = PFINAL+NSUITS,
};
struct Selection {
    uint8_t p;	// Selected pile
    uint8_t c;	// Selected card, counted from the back (0 = sz-1)
    uint8_t dest[3];	// Possible destinations 0,1 in sorting piles, 2 final
};

//----------------------------------------------------------------------

static struct Pile	_pile [NPILES]	= { {{0},0,0}};
static struct {
    struct Selection	m [NRANKS];
    uint8_t		sz;
    uint8_t		selected;
} _moveable = { {{PDECK,0,{0}}},0,0};

static WINDOW*	_w = NULL;

//----------------------------------------------------------------------

static void initialize_window (void);
static void move_cards (unsigned f, unsigned c, unsigned t);
static void deal_one_card (void);
static void deal_cards (void);
static unsigned card_rank (card_t c);
static unsigned card_suit (card_t c);
static card_t selected_card (unsigned p, unsigned c);
static bool update_move_targets (struct Selection *m);
static void find_moveable_cards (void);
static void move_selection (int ds);
static void move_selected_to_dest (unsigned d);
static bool has_won (void);
static void draw_card (unsigned l, unsigned c, card_t v, bool selected);
static void draw_hidden_card (unsigned l, unsigned c);
static void draw_empty_target (unsigned l, unsigned c);
static void draw_window (void);

//----------------------------------------------------------------------
// Initialization

static void initialize_window (void)
{
    if (_w) {
	mvwin (_w, LINES-WLINES, (COLS-WCOLS)/2);
	erase();
	refresh();
	return;
    }
    _w = newwin (WLINES, WCOLS, LINES-WLINES, (COLS-WCOLS)/2);
    keypad (_w, true);
    init_pair (color_HiddenCard,	COLOR_WHITE,	COLOR_BLUE);
    init_pair (color_BlackSuit,		COLOR_BLACK,	COLOR_WHITE);
    init_pair (color_RedSuit,		COLOR_RED,	COLOR_WHITE);
    init_pair (color_SelectedBlackSuit,	COLOR_BLACK,	COLOR_CYAN);
    init_pair (color_SelectedRedSuit,	COLOR_RED,	COLOR_CYAN);
}

// Move cards from pile f starting from card c to pile t
static void move_cards (unsigned f, unsigned c, unsigned t)
{
    for (unsigned i = _pile[f].sz-1-c; i < _pile[f].sz; ++i)
	_pile[t].c[_pile[t].sz++] = _pile[f].c[i];
    _pile[f].sz -= c+1;
    if (f >= PSORTING && _pile[f].nHidden >= _pile[f].sz)
	_pile[f].nHidden = _pile[f].sz - !!_pile[f].sz;
    find_moveable_cards();
}

static void deal_one_card (void)
{
    if (!_pile[PDECK].sz) {
	// Flip the dealt pile, becoming the deck
	while (_pile[PDEALT].sz)
	    _pile[PDECK].c[_pile[PDECK].sz++] = _pile[PDEALT].c[--_pile[PDEALT].sz];
	_pile[PDECK].nHidden = _pile[PDECK].sz;
    }
    move_cards (PDECK, 0, PDEALT);
}

static void deal_cards (void)
{
    // Empty all piles
    for (unsigned i = 0; i < NPILES; ++i)
	_pile[i].sz = _pile[i].nHidden = 0;
    // Fill and shuffle the deck
    struct Pile* p = &_pile[PDECK];
    p->sz = p->nHidden = NCARDS;
    for (unsigned i = 0; i < NCARDS; ++i)
	p->c[i] = i;
    for (unsigned i = 0; i < NCARDS; ++i) {
	unsigned r = nrand(NCARDS-i);
	card_t t = p->c[i];
	p->c[i] = p->c[r];
	p->c[r] = t;
    }

    // Deal sorting piles
    for (unsigned i = 0; i < NSORTING; ++i) {
	move_cards (PDECK, NSORTING-1-i, PSORTING+i);
	_pile[PSORTING+i].nHidden = _pile[PSORTING+i].sz-1;
    }
    // Deal one card to dealt pile
    deal_one_card();
}

//----------------------------------------------------------------------
// Selection and movement

static unsigned card_rank (card_t c)
    { return c/NSUITS; }
static unsigned card_suit (card_t c)
    { return c%NSUITS; }
static card_t selected_card (unsigned p, unsigned c)
    { return _pile[p].c[_pile[p].sz-1-c]; }

static bool update_move_targets (struct Selection* m)
{
    assert (_pile[m->p].sz);
    bool can = false;
    card_t v = selected_card (m->p, m->c);
    memset (m->dest, 0, sizeof(m->dest));
    // Check tops of the sorting piles
    for (unsigned i = 0; i < NSORTING; ++i) {
	const struct Pile* psp = &_pile[PSORTING+i];
	card_t dc = psp->sz ? psp->c[psp->sz-1] : NCARDS;
	if ((card_rank(v)+1 == card_rank(dc) && card_suit(v)%2 != card_suit(dc)%2)
		|| (!psp->sz && card_rank(v) == rank_King)) {
	    m->dest[can] = PSORTING+i;
	    can = true;
	}
    }
    // Check tops of the final piles
    if (!m->c) {	// only the top card can move to final
	for (unsigned i = 0; i < NFINAL; ++i) {
	    const struct Pile* psp = &_pile[PFINAL+i];
	    card_t dc = psp->sz ? psp->c[psp->sz-1] : NCARDS;
	    if ((card_rank(v) == card_rank(dc)+1 && card_suit(v) == card_suit(dc))
		    || (!psp->sz && card_rank(v) == rank_Ace)) {
		m->dest[2] = PFINAL+i;
		can = true;
	    }
	}
    }
    return can;
}

static void find_moveable_cards (void)
{
    _moveable.selected = 0;
    _moveable.sz = 1;
    for (unsigned p = PDECK+1; p < NPILES; ++p) {
	const struct Pile* pp = &_pile[p];
	unsigned nvis = pp->sz - pp->nHidden;
	if (p >= PDEALT && nvis > 1)	// Dealt and final piles only move top card
	    nvis = 1;
	for (unsigned c = 0; c < nvis; ++c) {
	    _moveable.m[_moveable.sz].p = p;
	    _moveable.m[_moveable.sz].c = c;
	    if (update_move_targets (&_moveable.m[_moveable.sz]))
		++_moveable.sz;
	}
    }
    if (_moveable.sz > 1)	// Select non-deck, if available
	_moveable.selected = 1;
}

static void move_selection (int ds)
{
    assert (ds == -1 || ds == 0 || ds == 1);
    _moveable.selected = (_moveable.selected + ds) % _moveable.sz;
    if (!_moveable.selected && _moveable.sz > 1 && ds)
	move_selection (ds);	// skip PDECK
}

static void move_selected_to_dest (unsigned d)
{
    const struct Selection* sel = &_moveable.m[_moveable.selected];
    if (sel->dest[d])
	move_cards (sel->p, sel->c, sel->dest[d]);
}

static bool has_won (void)
{
    for (unsigned p = 0; p < NFINAL; ++p)
	if (_pile[PFINAL+p].sz < NRANKS)
	    return false;
    // Animate the great achievement
    for (unsigned t = 0; t < 9; ++t) {
	draw_window();
	wattrset (_w, A_BOLD| COLOR_PAIR(color_HiddenCard));
	if (!(t%2))
	    mvwaddstr (_w, getmaxy(_w)-1, 1, "*** YOU WON! ***");
	wrefresh (_w);
	usleep (1000000/8);
    }
    mvwaddstr (_w, getmaxy(_w)-1, 17, "Press any key to exit");
    flushinp();
    wgetch (_w);
    return true;
}

//----------------------------------------------------------------------
// Drawing

static void draw_card (unsigned l, unsigned c, card_t v, bool selected)
{
    unsigned rank = v/NSUITS, suit = v%NSUITS;
    if (selected)
	mvwaddch (_w, l, c+2, A_BOLD| COLOR_PAIR(color_HiddenCard)|'*');
    wattrset (_w, COLOR_PAIR(color_BlackSuit + suit%2 + selected*2));
    mvwaddch (_w, l, c, rank < NRANKS ? "A23456789TJQK"[rank] : (int)ACS_CKBOARD);
    mvwaddch (_w, l, c+1, ' ');
    mvwaddch (_w, l+1, c, ' ');
    mvwaddch (_w, l+1, c+1, "SDCH"[suit]);
}

static void draw_hidden_card (unsigned l, unsigned c)
{
    wattrset (_w, COLOR_PAIR(color_HiddenCard));
    mvwaddch (_w, l, c, ACS_ULCORNER);
    mvwaddch (_w, l, c+1, ACS_URCORNER);
    mvwaddch (_w, l+1, c, ACS_LLCORNER);
    mvwaddch (_w, l+1, c+1, ACS_LRCORNER);
}

static void draw_empty_target (unsigned l, unsigned c)
{
    wattrset (_w, COLOR_PAIR(color_HiddenCard));
    mvwaddch (_w, l, c, ACS_CKBOARD);
    mvwaddch (_w, l, c+1, ACS_CKBOARD);
    mvwaddch (_w, l+1, c, ACS_CKBOARD);
    mvwaddch (_w, l+1, c+1, ACS_CKBOARD);
}

static void draw_window (void)
{
    wattrset (_w, 0);
    werase (_w);

    // Draw title
    box (_w, 0, 0);
    static const char c_Title[] = " Klondike ";
    mvwaddstr (_w, 0, (getmaxx(_w)-strlen(c_Title))/2, c_Title);

    const struct Selection* sel = &_moveable.m[_moveable.selected];
    // Draw deck
    if (!_pile[PDECK].sz)
	draw_empty_target (1, getmaxx(_w)-3);
    else
	draw_hidden_card (1, getmaxx(_w)-3);
    // Draw dealt
    if (!_pile[PDEALT].sz)
	draw_empty_target (4, getmaxx(_w)-3);
    else
	draw_card (4, getmaxx(_w)-3, _pile[PDEALT].c[_pile[PDEALT].sz-1], sel->p == PDEALT);

    // Draw final piles
    for (unsigned i = 0; i < NSUITS; ++i) {
	const struct Pile* p = &_pile[PFINAL+i];
	unsigned l = getmaxy(_w)-3*i-3, c = getmaxx(_w)-3;
	if (!p->sz)
	    draw_empty_target (l, c);
	else
	    draw_card (l, c, p->c[p->sz-1], sel->p == PFINAL+i);
	if (sel->dest[2] == PFINAL+i) {
	    wattrset (_w, 0);
	    mvwaddch (_w, l+1, c-1, A_BOLD|'f');
	}
    }

    // Draw sorting piles
    for (unsigned i = 0; i < NSORTING; ++i) {
	const struct Pile* p = &_pile[PSORTING+i];
	unsigned l = 3*i+1, c = 1;
	if (!p->sz)
	    draw_empty_target (l, c);
	else {
	    for (unsigned j = 0; j < p->sz; ++j, c += 3) {
		if (j < p->nHidden)
		    draw_hidden_card (l, c);
		else
		    draw_card (l, c, p->c[j], sel->p == PSORTING+i && sel-> c == p->sz-1-j);
	    }
	}
	wattrset (_w, 0);
	if (sel->dest[0] == PSORTING+i)
	    mvwaddch (_w, l, c-1, A_BOLD|'s');
	if (sel->dest[1] == PSORTING+i)
	    mvwaddch (_w, l, c-1, A_BOLD|'a');
    }
}

//----------------------------------------------------------------------

int main (void)
{
    initialize_curses();
    initialize_window();
    deal_cards();
    do {
	draw_window();
	int k = wgetch (_w);
	if (k == 'q' || k == KEY_F(10))	break;
	else if (k == KEY_RESIZE)	initialize_window();
	else if (k == 'R')	deal_cards();
	else if (k == 'd')	deal_one_card();
	else if (k == 'j')	move_selection (1);
	else if (k == 'k')	move_selection (-1);
	else if (k == 's')	move_selected_to_dest (0);
	else if (k == 'a')	move_selected_to_dest (1);
	else if (k == 'f')	move_selected_to_dest (2);
    } while (!has_won());
    return EXIT_SUCCESS;
}
