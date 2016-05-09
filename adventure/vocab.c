// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
// Re-coding of advent in C: data structure routines

#include "hdr.h"
#include "extern.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

struct vocmap {		// word table for vocabulary
    unsigned short val;	// word type &index (ktab)
    char word [6];	// pointer to actual string
};

enum { VOCABSZ = 299 };
static const struct vocmap s_Vocab [VOCABSZ];

void dstroy (int object)
{
    move (object, 0);
}

void juggle (int object)
{
    int i = place[object];
    int j = fixed[object];
    move(object, i);
    move(object + 100, j);
}

void move (int object, int where)
{
    int from = (object <= 100 ? place[object] : fixed[object - 100]);
    if (from > 0 && from <= 300)
	carry (object, from);
    drop (object, where);
}

int put (int object, int where, int pval)
{
    move (object, where);
    return -1 - pval;
}

void carry (int object, int where)
{
    if (object <= 100) {
	if (place[object] == -1)
	    return;
	place[object] = -1;
	holdng++;
    }
    if (atloc[where] == object) {
	atloc[where] = links[object];
	return;
    }
    int temp;
    for (temp = atloc[where]; links[temp] != object; temp = links[temp]) {}
    links[temp] = links[object];
}

void drop (int object, int where)
{
    if (object > 100)
	fixed[object - 100] = where;
    else {
	if (place[object] == -1)
	    holdng--;
	place[object] = where;
    }
    if (where <= 0)
	return;
    links[object] = atloc[where];
    atloc[where] = object;
}

int vocab (const char *word, int type)	// used for storing only
{
    for (unsigned i = 0; i < ArraySize(s_Vocab); ++i) {
	const struct vocmap* h = &s_Vocab[i];
	if (0 != strncmp (word, h->word, sizeof(h->word)-1))
	    continue;
	if (type == -1)
	    return h->val;
	if (h->val / 1000 == type)
	    return h->val % 1000;
    }
    return -1;
}

static const struct vocmap s_Vocab [VOCABSZ] = {
    {wrd_Road,"road"},
    {wrd_Road,"hill"},
    {wrd_Enter,"enter"},
    {wrd_Upstream,"upstr"},
    {wrd_Downstream,"downs"},
    {wrd_Forest,"fores"},
    {wrd_Forward,"forwa"},
    {wrd_Forward,"conti"},
    {wrd_Forward,"onwar"},
    {wrd_Back,"back"},
    {wrd_Back,"retur"},
    {wrd_Back,"retre"},
    {wrd_Valley,"valle"},
    {wrd_Stairs,"stair"},
    {wrd_Out,"out"},
    {wrd_Out,"outsi"},
    {wrd_Out,"exit"},
    {wrd_Out,"leave"},
    {wrd_Build,"build"},
    {wrd_Build,"house"},
    {wrd_Gully,"gully"},
    {wrd_Stream,"strea"},
    {wrd_Rock,"rock"},
    {wrd_Bed,"bed"},
    {wrd_Crawl,"crawl"},
    {wrd_Cobbl,"cobbl"},
    {wrd_Inward,"inwar"},
    {wrd_Inward,"insid"},
    {wrd_Inward,"in"},
    {wrd_Surface,"surfa"},
    {wrd_Null,"null"},
    {wrd_Null,"nowhe"},
    {wrd_Dark,"dark"},
    {wrd_Passage,"passa"},
    {wrd_Passage,"tunne"},
    {wrd_Low,"low"},
    {wrd_Canyon,"canyo"},
    {wrd_Awkward,"awkwa"},
    {wrd_Giant,"giant"},
    {wrd_View,"view"},
    {wrd_Up,"upwar"},
    {wrd_Up,"up"},
    {wrd_Up,"u"},
    {wrd_Up,"above"},
    {wrd_Up,"ascen"},
    {wrd_Down,"d"},
    {wrd_Down,"downw"},
    {wrd_Down,"down"},
    {wrd_Down,"desce"},
    {wrd_Pit,"pit"},
    {wrd_Outdoors,"outdo"},
    {wrd_Crack,"crack"},
    {wrd_Steps,"steps"},
    {wrd_Dome,"dome"},
    {wrd_Left,"left"},
    {wrd_Right,"right"},
    {wrd_Hall,"hall"},
    {wrd_Jump,"jump"},
    {wrd_Barren,"barre"},
    {wrd_Over,"over"},
    {wrd_Across,"acros"},
    {wrd_East,"east"},
    {wrd_East,"e"},
    {wrd_West,"west"},
    {wrd_West,"w"},
    {wrd_North,"north"},
    {wrd_North,"n"},
    {wrd_South,"south"},
    {wrd_South,"s"},
    {wrd_NE,"ne"},
    {wrd_SE,"se"},
    {wrd_SW,"sw"},
    {wrd_NW,"nw"},
    {wrd_Debris,"debri"},
    {wrd_Hole,"hole"},
    {wrd_Wall,"wall"},
    {wrd_Broke,"broke"},
    {wrd_Y2,"y2"},
    {wrd_Climb,"climb"},
    {wrd_Look,"look"},
    {wrd_Look,"exami"},
    {wrd_Look,"touch"},
    {wrd_Look,"descr"},
    {wrd_Floor,"floor"},
    {wrd_Room,"room"},
    {wrd_Slit,"slit"},
    {wrd_Slab,"slab"},
    {wrd_Slab,"slabr"},
    {wrd_Xyzzy,"xyzzy"},
    {wrd_Depression,"depre"},
    {wrd_Entrance,"entra"},
    {wrd_Plugh,"plugh"},
    {wrd_Secret,"secre"},
    {wrd_Cave,"cave"},
    {wrd_Cross,"cross"},
    {wrd_Bedquilt,"bedqu"},
    {wrd_Plover,"plove"},
    {wrd_Oriental,"orien"},
    {wrd_Cavern,"caver"},
    {wrd_Shell,"shell"},
    {wrd_Reservoir,"reser"},
    {wrd_Main,"main"},
    {wrd_Main,"offic"},
    {wrd_Fork,"fork"},
    {item_Keys,"keys"},
    {item_Keys,"key"},
    {item_Lamp,"lamp"},
    {item_Lamp,"headl"},
    {item_Lamp,"lante"},
    {item_Grate,"grate"},
    {item_Cage,"cage"},
    {item_Wand,"wand"},
    {item_Wand,"rod"},
    {item_Wand2,"wand"},
    {item_Wand2,"rod"},
    {item_Steps,"steps"},
    {item_Bird,"bird"},
    {item_Door,"door"},
    {item_Pillow,"pillo"},
    {item_Pillow,"velve"},
    {item_Snake,"snake"},
    {item_Fissure,"fissu"},
    {item_Table,"table"},
    {item_Clam,"clam"},
    {item_Oyster,"oyste"},
    {item_Magazine,"magaz"},
    {item_Magazine,"issue"},
    {item_Magazine,"spelu"},
    {item_Magazine,"\"spel"},
    {item_Dwarf,"dwarf"},
    {item_Dwarf,"dwarv"},
    {item_Knife,"knife"},
    {item_Knife,"knive"},
    {item_Food,"food"},
    {item_Food,"ratio"},
    {item_Bottle,"bottl"},
    {item_Bottle,"jar"},
    {item_Water,"water"},
    {item_Water,"h2o"},
    {item_Oil,"oil"},
    {item_Mirror,"mirro"},
    {item_Plant,"plant"},
    {item_Plant,"beans"},
    {item_Plant2,"plant"},
    {item_Stalactite,"stala"},
    {item_Shadow,"shado"},
    {item_Shadow,"figur"},
    {item_Axe,"axe"},
    {item_Drawing,"drawi"},
    {item_Pirate,"pirat"},
    {item_Dragon,"drago"},
    {item_Chasm,"chasm"},
    {item_Troll,"troll"},
    {item_Troll2,"troll"},
    {item_Bear,"bear"},
    {item_Message,"messa"},
    {item_Volcano,"volca"},
    {item_Volcano,"geyse"},
    {item_Machine,"machi"},
    {item_Machine,"vendi"},
    {item_Battery,"batte"},
    {item_Carpet,"carpe"},
    {item_Carpet,"moss"},
    {item_Gold,"gold"},
    {item_Gold,"nugge"},
    {item_Diamond,"diamo"},
    {item_Silver,"silve"},
    {item_Silver,"bars"},
    {item_Jewels,"jewel"},
    {item_Coins,"coins"},
    {item_Chest,"chest"},
    {item_Chest,"box"},
    {item_Chest,"treas"},
    {item_Eggs,"eggs"},
    {item_Eggs,"egg"},
    {item_Eggs,"nest"},
    {item_Trident,"tride"},
    {item_Vase,"vase"},
    {item_Vase,"ming"},
    {item_Vase,"shard"},
    {item_Vase,"potte"},
    {item_Emerald,"emera"},
    {item_Pyramid,"plati"},
    {item_Pyramid,"pyram"},
    {item_Pearl,"pearl"},
    {item_Rug,"rug"},
    {item_Rug,"persi"},
    {item_Spice,"spice"},
    {item_Chain,"chain"},
    {cmd_Carry,"carry"},
    {cmd_Carry,"take"},
    {cmd_Carry,"keep"},
    {cmd_Carry,"catch"},
    {cmd_Carry,"steal"},
    {cmd_Carry,"captu"},
    {cmd_Carry,"get"},
    {cmd_Carry,"tote"},
    {cmd_Drop,"drop"},
    {cmd_Drop,"relea"},
    {cmd_Drop,"free"},
    {cmd_Drop,"disca"},
    {cmd_Drop,"dump"},
    {cmd_Say,"say"},
    {cmd_Say,"chant"},
    {cmd_Say,"sing"},
    {cmd_Say,"utter"},
    {cmd_Say,"mumbl"},
    {cmd_Unlock,"unloc"},
    {cmd_Unlock,"open"},
    {cmd_Nothing,"nothi"},
    {cmd_Lock,"lock"},
    {cmd_Lock,"close"},
    {cmd_Light,"light"},
    {cmd_Light,"on"},
    {cmd_Extinguish,"extin"},
    {cmd_Extinguish,"off"},
    {cmd_Wave,"wave"},
    {cmd_Wave,"shake"},
    {cmd_Wave,"swing"},
    {cmd_Calm,"calm"},
    {cmd_Calm,"placa"},
    {cmd_Calm,"tame"},
    {cmd_Walk,"walk"},
    {cmd_Walk,"run"},
    {cmd_Walk,"trave"},
    {cmd_Walk,"go"},
    {cmd_Walk,"proce"},
    {cmd_Walk,"conti"},
    {cmd_Walk,"explo"},
    {cmd_Walk,"goto"},
    {cmd_Walk,"follo"},
    {cmd_Walk,"turn"},
    {cmd_Attack,"attac"},
    {cmd_Attack,"kill"},
    {cmd_Attack,"slay"},
    {cmd_Attack,"fight"},
    {cmd_Attack,"hit"},
    {cmd_Attack,"strik"},
    {cmd_Pour,"pour"},
    {cmd_Eat,"eat"},
    {cmd_Eat,"devou"},
    {cmd_Drink,"drink"},
    {cmd_Rub,"rub"},
    {cmd_Throw,"throw"},
    {cmd_Throw,"toss"},
    {cmd_Quit,"quit"},
    {cmd_Find,"find"},
    {cmd_Find,"where"},
    {cmd_Inventory,"inven"},
    {cmd_Inventory,"inv"},
    {cmd_Feed,"feed"},
    {cmd_Fill,"fill"},
    {cmd_Blast,"blast"},
    {cmd_Blast,"deton"},
    {cmd_Blast,"ignit"},
    {cmd_Blast,"blowu"},
    {cmd_Score,"score"},
    {cmd_Fee,"fee"},
    {cmd_Fee,"fie"},
    {cmd_Fee,"foe"},
    {cmd_Fee,"foo"},
    {cmd_Fee,"fum"},
    {cmd_Brief,"brief"},
    {cmd_Read,"read"},
    {cmd_Read,"perus"},
    {cmd_Break,"break"},
    {cmd_Break,"shatt"},
    {cmd_Break,"smash"},
    {cmd_Wake,"wake"},
    {cmd_Wake,"distu"},
    {cmd_Save,"suspe"},
    {cmd_Save,"pause"},
    {cmd_Save,"save"},
    {cmd_Hours,"hours"},
    {magic_Fee,"fee"},
    {magic_Fie,"fie"},
    {magic_Foe,"foe"},
    {magic_Foo,"foo"},
    {magic_Fum,"fum"},
    {magic_Sesame,"sesam"},
    {magic_Sesame,"opens"},
    {magic_Sesame,"abra"},
    {magic_Sesame,"abrac"},
    {magic_Sesame,"shaza"},
    {magic_Sesame,"hocus"},
    {magic_Sesame,"pocus"},
    {magic_Help,"help"},
    {magic_Help,"?"},
    {magic_Tree,"tree"},
    {magic_Tree,"trees"},
    {magic_Dig,"dig"},
    {magic_Dig,"excav"},
    {magic_Lost,"lost"},
    {magic_Mist,"mist"},
    {magic_Fuck,"fuck"},
    {magic_Stop,"stop"},
    {magic_Info,"info"},
    {magic_Info,"infor"},
    {magic_Swim,"swim"}
};
