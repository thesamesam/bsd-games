// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "hdr.h"

// Object descriptions
const char* const ptext [OBJSIZ] = {
	// 0
	"",
	// 1
	"Set of keys\0"
	"There are some keys on the ground here.",
	// 2
	"Brass lantern\0"
	"There is a shiny brass lamp nearby.\0"
	"There is a lamp shining nearby.",
	// 3
	"*Grate\0"
	"The grate is locked.\0"
	"The grate is open.",
	// 4
	"Wicker cage\0"
	"There is a small wicker cage discarded nearby.",
	// 5
	"Black rod\0"
	"A three foot black rod with a rusty star on an end lies nearby.",
	// 6
	"Black rod\0"
	"A three foot black rod with a rusty mark on an end lies nearby.",
	// 7
	"*Steps\0"
	"Rough stone steps lead down the pit.\0"
	"Rough stone steps lead up the dome.",
	// 8
	"Little bird in cage\0"
	"A cheerful little bird is sitting here singing.\0"
	"There is a little bird in the cage.",
	// 9
	"*Rusty door\0"
	"The way north is barred by a massive, rusty, iron door.\0"
	"The way north leads through a massive, rusty, iron door.",
	// 10
	"Velvet pillow\0"
	"A small velvet pillow lies on the floor.",
	// 11
	"*Snake\0"
	"A huge green fierce snake bars the way!\0"
	"",
	// 12
	"*Fissure\0"
	"""\0"
	"A crystal bridge now spans the fissure.\0"
	"The crystal bridge has vanished!",
	// 13
	"*Stone tablet\0"
	"A massive stone tablet embedded in the wall reads:\n"
	"\"Congratulations on bringing light into the Dark-Room!\"",
	// 14
	"Giant clam  >grunt!<\0"
	"There is an enormous clam here with its shell tightly closed.",
	// 15
	"Giant oyster  >groan!<\0"
	"There is an enormous oyster here with its shell tightly closed.\0"
	"Interesting.  There seems to be something written on the underside of\n"
	"The oyster.",
	// 16
	"\"Spelunker Today\"\0"
	"There are a few recent issues of \"Spelunker Today\" magazine here.",
	// 17
	"",
	// 18
	"",
	// 19
	"Tasty food\0"
	"There is food here.",
	// 20
	"Small bottle\0"
	"There is a bottle of water here.\0"
	"There is an empty bottle here.\0"
	"There is a bottle of oil here.",
	// 21
	"Water in the bottle",
	// 22
	"Oil in the bottle",
	// 23
	"*Mirror\0"
	"",
	// 24
	"*Plant\0"
	"There is a tiny little plant in the pit, murmuring \"Water, water, ...\"\0"
	"The plant spurts into furious growth for a few seconds.\0"
	"There is a 12-foot-tall beanstalk stretching up out of the pit,\n"
	"bellowing \"WATER!! WATER!!\"\0"
	"The plant grows explosively, almost filling the bottom of the pit.\0"
	"There is a gigantic beanstalk stretching all the way up to the hole.\0"
	"You've over-watered the plant!  It's shriveling up!  It's, it's...",
	// 25
	"*Phony plant (seen in twopit room only when tall enough)\0"
	"""\0"
	"The top of a 12-foot-tall beanstalk is poking out of the west pit.\0"
	"There is a huge beanstalk growing out of the west pit up to the hole.",
	// 26
	"*Stalactite\0"
	"",
	// 27
	"*Shadowy figure\0"
	"The shadowy figure seems to be trying to attract your attention.",
	// 28
	"Dwarf's axe\0"
	"There is a little axe here.\0"
	"There is a little axe lying beside the bear.",
	// 29
	"*Cave drawings\0"
	"",
	// 30
	"*Pirate\0"
	"",
	// 31
	"*Dragon\0"
	"A huge green fierce dragon bars the way!\0"
	"Congratulations!  You have just vanquished a dragon with your bare\n"
	"hands!  (Unbelievable, isn't it?)\0"
	"The body of a huge green dead dragon is lying off to one side.",
	// 32
	"*Chasm\0"
	"A rickety wooden bridge extends across the chasm, vanishing into the\n"
	"mist.  A sign posted on the bridge reads, \"Stop! Pay troll!\"\0"
	"The wreckage of a bridge (and a dead bear) can be seen at the bottom\n"
	"of the chasm.",
	// 33
	"*Troll\0"
	"A burly troll stands by the bridge and insists you throw him a\n"
	"treasure before you may cross.\0"
	"The troll steps out from beneath the bridge and blocks your way.\0"
	"",
	// 34
	"*Phony troll\0"
	"The troll is nowhere to be seen.",
	// 35
	"""\0"
	"There is a ferocious cave bear eying you from the far end of the room!\0"
	"There is a gentle cave bear sitting placidly in one corner.\0"
	"There is a contented-looking bear wandering about nearby.\0"
	"",
	// 36
	"*Message in second maze\0"
	"There is a message scrawled in the dust in a flowery script, reading:\n"
	"\"This is not the maze where the pirate leaves his treasure chest.\"",
	// 37
	"*Volcano and/or geyser\0"
	"",
	// 38
	"*Vending machine\0"
	"There is a massive vending machine here.  The instructions on it read:\n"
	"\"Drop coins here to receive fresh batteries.\"",
	// 39
	"Batteries\0"
	"There are fresh batteries here.\0"
	"Some worn-out batteries have been discarded nearby.",
	// 40
	"*Carpet and/or moss\0"
	"",
	// 41
	"",
	// 42
	"",
	// 43
	"",
	// 44
	"",
	// 45
	"",
	// 46
	"",
	// 47
	"",
	// 48
	"",
	// 49
	"",
	// 50
	"Large gold nugget\0"
	"There is a large sparkling nugget of gold here!",
	// 51
	"Several diamonds\0"
	"There are diamonds here!",
	// 52
	"Bars of silver\0"
	"There are bars of silver here!",
	// 53
	"Precious jewelry\0"
	"There is precious jewelry here!",
	// 54
	"Rare coins\0"
	"There are many coins here!",
	// 55
	"Treasure chest\0"
	"The pirate's treasure chest is here!",
	// 56
	"Golden eggs\0"
	"There is a large nest here, full of golden eggs!\0"
	"The nest of golden eggs has vanished!\0"
	"Done!",
	// 57
	"Jeweled trident\0"
	"There is a jewel-encrusted trident here!",
	// 58
	"Ming vase\0"
	"There is a delicate, precious, Ming vase here!\0"
	"The vase is now resting, delicately, on a velvet pillow.\0"
	"The floor is littered with worthless shards of pottery.\0"
	"The Ming vase drops with a delicate crash.",
	// 59
	"Egg-sized emerald\0"
	"There is an emerald here the size of a plover's egg!",
	// 60
	"Platinum pyramid\0"
	"There is a platinum pyramid here, 8 inches on a side!",
	// 61
	"Glistening pearl\0"
	"Off to one side lies a glistening pearl!",
	// 62
	"Persian rug\0"
	"There is a Persian rug spread out on the floor!\0"
	"The dragon is sprawled out on a Persian rug!!",
	// 63
	"Rare spices\0"
	"There are rare spices here!",
	// 64
	"Golden chain\0"
	"There is a golden chain lying in a heap on the floor!\0"
	"The bear is locked to the wall with a golden chain!\0"
	"There is a golden chain locked to the wall!",
	// 65
	"",
	// 66
	"",
	// 67
	"",
	// 68
	"",
	// 69
	"",
	// 70
	"",
	// 71
	"",
	// 72
	"",
	// 73
	"",
	// 74
	"",
	// 75
	"",
	// 76
	"",
	// 77
	"",
	// 78
	"",
	// 79
	"",
	// 80
	"",
	// 81
	"",
	// 82
	"",
	// 83
	"",
	// 84
	"",
	// 85
	"",
	// 86
	"",
	// 87
	"",
	// 88
	"",
	// 89
	"",
	// 90
	"",
	// 91
	"",
	// 92
	"",
	// 93
	"",
	// 94
	"",
	// 95
	"",
	// 96
	"",
	// 97
	"",
	// 98
	"",
	// 99
	"",
	// 100
	""
};

// Random messages
const char* const rtext [RTXSIZ] = {
	// 0
	"",
	// 1
	"Somewhere nearby is Colossal Cave, where others have found fortunes in\n"
	"treasure and gold, though it is rumored that some who enter are never\n"
	"seen again.  Magic is said to work in the cave.  I will be your eyes\n"
	"and hands.  Direct me with commands of 1 or 2 words.  I should warn\n"
	"you that I look at only the first five letters of each word, so you'll\n"
	"have to enter \"northeast\" as \"ne\" to distinguish it from \"north\".\n"
	"(Should you get stuck, type \"help\" for some general hints.  For\n"
	"information on how to end your adventure, etc., type \"info\".)\n"
	"			      - - -\n"
	"This program was originally developed by Will Crowther.  Most of the\n"
	"features of the current program were added by Don Woods.  Address\n"
	"complaints about the UNIX version to Jim Gillogly (jim@rand.org).",
	// 2
	"A little dwarf with a big knife blocks your way.",
	// 3
	"A little dwarf just walked around a corner, saw you, threw a little\n"
	"axe at you (which missed), cursed, and ran away.",
	// 4
	"There is a threatening little dwarf in the room with you!",
	// 5
	"One sharp nasty knife is thrown at you!",
	// 6
	"None of them hit you!",
	// 7
	"One of them gets you!",
	// 8
	"A hollow voice says \"Plugh\".",
	// 9
	"There is no way to go that direction.",
	// 10
	"I am unsure how you are facing.  Use compass points or nearby objects.",
	// 11
	"I don't know in from out here.  Use compass points or name something\n"
	"in the general direction you want to go.",
	// 12
	"I don't know how to apply that word here.",
	// 13
	"I don't understand that!",
	// 14
	"I'm game.  Would you care to explain how?",
	// 15
	"Sorry, but I am not allowed to give more detail.  I will repeat the\n"
	"long description of your location.",
	// 16
	"It is now pitch dark.  If you proceed you will likely fall into a pit.",
	// 17
	"If you prefer, simply type w rather than west.",
	// 18
	"Are you trying to catch the bird?",
	// 19
	"The bird is frightened right now and you cannot catch it no matter\n"
	"what you try.  Perhaps you might try later.",
	// 20
	"Are you trying to somehow deal with the snake?",
	// 21
	"You can't kill the snake, or drive it away, or avoid it, or anything\n"
	"like that.  There is a way to get by, but you don't have the necessary\n"
	"resources right now.",
	// 22
	"Do you really want to quit now?",
	// 23
	"You fell into a pit and broke every bone in your body!",
	// 24
	"You are already carrying it!",
	// 25
	"You can't be serious!",
	// 26
	"The bird was unafraid when you entered, but as you approach it becomes\n"
	"disturbed and you cannot catch it.",
	// 27
	"You can catch the bird, but you cannot carry it.",
	// 28
	"There is nothing here with a lock!",
	// 29
	"You aren't carrying it!",
	// 30
	"The little bird attacks the green snake, and in an astounding flurry\n"
	"drives the snake away.",
	// 31
	"You have no keys!",
	// 32
	"It has no lock.",
	// 33
	"I don't know how to lock or unlock such a thing.",
	// 34
	"It was already locked.",
	// 35
	"The grate is now locked.",
	// 36
	"The grate is now unlocked.",
	// 37
	"It was already unlocked.",
	// 38
	"You have no source of light.",
	// 39
	"Your lamp is now on.",
	// 40
	"Your lamp is now off.",
	// 41
	"There is no way to get past the bear to unlock the chain, which is\n"
	"probably just as well.",
	// 42
	"Nothing happens.",
	// 43
	"Where?",
	// 44
	"There is nothing here to attack.",
	// 45
	"The little bird is now dead.  Its body disappears.",
	// 46
	"Attacking the snake both doesn't work and is very dangerous.",
	// 47
	"You killed a little dwarf.",
	// 48
	"You attack a little dwarf, but he dodges out of the way.",
	// 49
	"With what?  Your bare hands?",
	// 50
	"Good try, but that is an old worn-out magic word.",
	// 51
	"I know of places, actions, and things.  Most of my vocabulary\n"
	"describes places and is used to move you there.  To move, try words\n"
	"like forest, building, downstream, enter, east, west, north, south,\n"
	"up, or down.  I know about a few special objects, like a black rod\n"
	"hidden in the cave.  These objects can be manipulated using some of\n"
	"the action words that I know.  Usually you will need to give both the\n"
	"object and action words (in either order), but sometimes I can infer\n"
	"the object from the verb alone.  Some objects also imply verbs; in\n"
	"particular, \"inventory\" implies \"take inventory\", which causes me to\n"
	"give you a list of what you're carrying.  The objects have side\n"
	"effects; for instance, the rod scares the bird.  Usually people having\n"
	"trouble moving just need to try a few more words.  Usually people\n"
	"trying unsuccessfully to manipulate an object are attempting something\n"
	"beyond their (or my!) capabilities and should try a completely\n"
	"different tack.  To speed the game you can sometimes move long\n"
	"distances with a single word.  For example, \"building\" usually gets\n"
	"you to the building from anywhere above ground except when lost in the\n"
	"forest.  Also, note that cave passages turn a lot, and that leaving a\n"
	"room to the north does not guarantee entering the next from the south.\n"
	"Good luck!",
	// 52
	"It misses!",
	// 53
	"It gets you!",
	// 54
	"OK",
	// 55
	"You can't unlock the keys.",
	// 56
	"You have crawled around in some little holes and wound up back in the\n"
	"main passage.",
	// 57
	"I don't know where the cave is, but hereabouts no stream can run on\n"
	"the surface for long.  I would try the stream.",
	// 58
	"I need more detailed instructions to do that.",
	// 59
	"I can only tell you what you see as you move about and manipulate\n"
	"things.  I cannot tell you where remote things are.",
	// 60
	"I don't know that word.",
	// 61
	"What?",
	// 62
	"Are you trying to get into the cave?",
	// 63
	"The grate is very solid and has a hardened steel lock.  You cannot\n"
	"enter without a key, and there are no keys nearby.  I would recommend\n"
	"looking elsewhere for the keys.",
	// 64
	"The trees of the forest are large hardwood oak and maple, with an\n"
	"occasional grove of pine or spruce.  There is quite a bit of under-\n"
	"growth, largely birch and ash saplings plus nondescript bushes of\n"
	"various sorts.  This time of year visibility is quite restricted by\n"
	"all the leaves, but travel is quite easy if you detour around the\n"
	"spruce and berry bushes.",
	// 65
	"Welcome to Adventure!!  Would you like instructions?",
	// 66
	"Digging without a shovel is quite impractical.  Even with a shovel\n"
	"progress is unlikely.",
	// 67
	"Blasting requires dynamite.",
	// 68
	"I'm as confused as you are.",
	// 69
	"Mist is a white vapor, usually water, seen from time to time in\n"
	"caverns.  It can be found anywhere but is frequently a sign of a deep\n"
	"pit leading down to water.",
	// 70
	"Your feet are now wet.",
	// 71
	"I think I just lost my appetite.",
	// 72
	"Thank you, it was delicious!",
	// 73
	"You have taken a drink from the stream.  The water tastes strongly of\n"
	"minerals, but is not unpleasant.  It is extremely cold.",
	// 74
	"The bottle of water is now empty.",
	// 75
	"Rubbing the electric lamp is not particularly rewarding.  Anyway,\n"
	"nothing exciting happens.",
	// 76
	"Peculiar.  Nothing unexpected happens.",
	// 77
	"Your bottle is empty and the ground is wet.",
	// 78
	"You can't pour that.",
	// 79
	"Watch it!",
	// 80
	"Which way?",
	// 81
	"Oh dear, you seem to have gotten yourself killed.  I might be able to\n"
	"help you out, but I've never really done this before.  Do you want me\n"
	"to try to reincarnate you?",
	// 82
	"All right.  But don't blame me if something goes wr......\n"
	"                    --- Poof!! ---\n"
	"You are engulfed in a cloud of orange smoke.  Coughing and gasping,\n"
	"you emerge from the smoke and find....",
	// 83
	"You clumsy oaf, you've done it again!  I don't know how long I can\n"
	"keep this up.  Do you want me to try reincarnating you again?",
	// 84
	"Okay, now where did I put my orange smoke?....  >poof!<\n"
	"Everything disappears in a dense cloud of orange smoke.",
	// 85
	"Now you've really done it!  I'm out of orange smoke!  You don't expect\n"
	"me to do a decent reincarnation without any orange smoke, do you?",
	// 86
	"Okay, if you're so smart, do it yourself!  I'm leaving!",
	// 87
	"",
	// 88
	"",
	// 89
	"",
	// 90
	">>> Messages 81 thru 90 are reserved for \"obituaries\". <<<",
	// 91
	"Sorry, but I no longer seem to remember how it was you got here.",
	// 92
	"You can't carry anything more.  You'll have to drop something first.",
	// 93
	"You can't go through a locked steel grate!",
	// 94
	"I believe what you want is right here with you.",
	// 95
	"You don't fit through a two-inch slit!",
	// 96
	"I respectfully suggest you go across the bridge instead of jumping.",
	// 97
	"There is no way across the fissure.",
	// 98
	"You're not carrying anything.",
	// 99
	"You are currently holding the following:",
	// 100
	"It's not hungry (it's merely pinin' for the fjords).  Besides, you\n"
	"have no bird seed.",
	// 101
	"The snake has now devoured your bird.",
	// 102
	"There's nothing here it wants to eat (except perhaps you).",
	// 103
	"You fool, dwarves eat only coal!  Now you've made him *really* mad!!",
	// 104
	"You have nothing in which to carry it.",
	// 105
	"Your bottle is already full.",
	// 106
	"There is nothing here with which to fill the bottle.",
	// 107
	"Your bottle is now full of water.",
	// 108
	"Your bottle is now full of oil.",
	// 109
	"You can't fill that.",
	// 110
	"Don't be ridiculous!",
	// 111
	"The door is extremely rusty and refuses to open.",
	// 112
	"The plant indignantly shakes the oil off its leaves and asks, \"water?\"",
	// 113
	"The hinges are quite thoroughly rusted now and won't budge.",
	// 114
	"The oil has freed up the hinges so that the door will now move,\n"
	"although it requires some effort.",
	// 115
	"The plant has exceptionally deep roots and cannot be pulled free.",
	// 116
	"The dwarves' knives vanish as they strike the walls of the cave.",
	// 117
	"Something you're carrying won't fit through the tunnel with you.\n"
	"You'd best take inventory and drop something.",
	// 118
	"You can't fit this five-foot clam through that little passage!",
	// 119
	"You can't fit this five-foot oyster through that little passage!",
	// 120
	"I advise you to put down the clam before opening it.  >strain!<",
	// 121
	"I advise you to put down the oyster before opening it.  >wrench!<",
	// 122
	"You don't have anything strong enough to open the clam.",
	// 123
	"You don't have anything strong enough to open the oyster.",
	// 124
	"A glistening pearl falls out of the clam and rolls away.  Goodness,\n"
	"this must really be an oyster.  (I never was very good at identifying\n"
	"bivalves.)  Whatever it is, it has now snapped shut again.",
	// 125
	"The oyster creaks open, revealing nothing but oyster inside.  It\n"
	"promptly snaps shut again.",
	// 126
	"You have crawled around in some little holes and found your way\n"
	"blocked by a recent cave-in.  You are now back in the main passage.",
	// 127
	"There are faint rustling noises from the darkness behind you.",
	// 128
	"Out from the shadows behind you pounces a bearded pirate!  \"Har, har,\"\n"
	"he chortles, \"I'll just take all this booty and hide it away with me\n"
	"chest deep in the maze!\"  He snatches your treasure and vanishes into\n"
	"the gloom.",
	// 129
	"A sepulchral voice reverberating through the cave, says, \"Cave closing\n"
	"soon.  All adventurers exit immediately through Main Office.\"",
	// 130
	"A mysterious recorded voice groans into life and announces:\n"
	"\"This exit is closed.  Please leave via Main Office.\"",
	// 131
	"It looks as though you're dead.  Well, seeing as how it's so close to\n"
	"closing time anyway, I think we'll just call it a day.",
	// 132
	"The sepulchral voice intones, \"The cave is now closed.\"  As the echoes\n"
	"fade, there is a blinding flash of light (and a small puff of orange\n"
	"smoke). . . .    As your eyes refocus, you look around and find...",
	// 133
	"There is a loud explosion, and a twenty-foot hole appears in the far\n"
	"wall, burying the dwarves in the rubble.  You march through the hole\n"
	"and find yourself in the Main Office, where a cheering band of\n"
	"friendly elves carry the conquering adventurer off into the sunset.",
	// 134
	"There is a loud explosion, and a twenty-foot hole appears in the far\n"
	"wall, burying the snakes in the rubble.  A river of molten lava pours\n"
	"in through the hole, destroying everything in its path, including you!",
	// 135
	"There is a loud explosion, and you are suddenly splashed across the\n"
	"walls of the room.",
	// 136
	"The resulting ruckus has awakened the dwarves.  There are now several\n"
	"threatening little dwarves in the room with you!  Most of them throw\n"
	"knives at you!  All of them get you!",
	// 137
	"Oh, leave the poor unhappy bird alone.",
	// 138
	"I dare say whatever you want is around here somewhere.",
	// 139
	"I don't know the word \"stop\".  Use \"quit\" if you want to give up.",
	// 140
	"You can't get there from here.",
	// 141
	"You are being followed by a very large, tame bear.",
	// 142
	"If you want to end your adventure early, say \"quit\".  To suspend your\n"
	"adventure such that you can continue later, say \"suspend\" (or \"pause\"\n"
	"or \"save\").  To see what hours the cave is normally open, say \"hours\".\n"
	"To see how well you're doing, say \"score\".  To get full credit for a\n"
	"treasure, you must have left it safely in the building, though you get\n"
	"partial credit just for locating it.  You lose points for getting\n"
	"killed, or for quitting, though the former costs you more.  There are\n"
	"also points based on how much (if any) of the cave you've managed to\n"
	"explore; in particular, there is a large bonus just for getting in (to\n"
	"distinguish the beginners from the rest of the pack), and there are\n"
	"other ways to determine whether you've been through some of the more\n"
	"harrowing sections.  If you think you've found all the treasures, just\n"
	"keep exploring for a while.  If nothing interesting happens, you\n"
	"haven't found them all yet.  If something interesting *does* happen,\n"
	"it means you're getting a bonus and have an opportunity to garner many\n"
	"more points in the Master's Section.  I may occasionally offer hints\n"
	"if you seem to be having trouble.  If I do, I'll warn you in advance\n"
	"how much it will affect your score to accept the hints.  Finally, to\n"
	"save paper, you may specify \"brief\", which tells me never to repeat\n"
	"the full description of a place unless you explicitly ask me to.",
	// 143
	"Do you indeed wish to quit now?",
	// 144
	"There is nothing here with which to fill the vase.",
	// 145
	"The sudden change in temperature has delicately shattered the vase.",
	// 146
	"It is beyond your power to do that.",
	// 147
	"I don't know how.",
	// 148
	"It is too far up for you to reach.",
	// 149
	"You killed a little dwarf.  The body vanishes in a cloud of greasy\n"
	"black smoke.",
	// 150
	"The shell is very strong and is impervious to attack.",
	// 151
	"What's the matter, can't you read?  Now you'd best start over.",
	// 152
	"The axe bounces harmlessly off the dragon's thick scales.",
	// 153
	"The dragon looks rather nasty.  You'd best not try to get by.",
	// 154
	"The little bird attacks the green dragon, and in an astounding flurry\n"
	"gets burnt to a cinder.  The ashes blow away.",
	// 155
	"On what?",
	// 156
	"Okay, from now on I'll only describe a place in full the first time\n"
	"you come to it.  To get the full description, say \"look\".",
	// 157
	"Trolls are close relatives with the rocks and have skin as tough as\n"
	"that of a rhinoceros.  The troll fends off your blows effortlessly.",
	// 158
	"The troll deftly catches the axe, examines it carefully, and tosses it\n"
	"back, declaring, \"Good workmanship, but it's not valuable enough.\"",
	// 159
	"The troll catches your treasure and scurries away out of sight.",
	// 160
	"The troll refuses to let you cross.",
	// 161
	"There is no longer any way across the chasm.",
	// 162
	"Just as you reach the other side, the bridge buckles beneath the\n"
	"weight of the bear, which was still following you around.  You\n"
	"scrabble desperately for support, but as the bridge collapses you\n"
	"stumble back and fall into the chasm.",
	// 163
	"The bear lumbers toward the troll, who lets out a startled shriek and\n"
	"scurries away.  The bear soon gives up the pursuit and wanders back.",
	// 164
	"The axe misses and lands near the bear where you can't get at it.",
	// 165
	"With what?  Your bare hands?  Against *his* bear hands??",
	// 166
	"The bear is confused; he only wants to be your friend.",
	// 167
	"For crying out loud, the poor thing is already dead!",
	// 168
	"The bear eagerly wolfs down your food, after which he seems to calm\n"
	"down considerably and even becomes rather friendly.",
	// 169
	"The bear is still chained to the wall.",
	// 170
	"The chain is still locked.",
	// 171
	"The chain is now unlocked.",
	// 172
	"The chain is now locked.",
	// 173
	"There is nothing here to which the chain can be locked.",
	// 174
	"There is nothing here to eat.",
	// 175
	"Do you want the hint?",
	// 176
	"Do you need help getting out of the maze?",
	// 177
	"You can make the passages look less alike by dropping things.",
	// 178
	"Are you trying to explore beyond the Plover Room?",
	// 179
	"There is a way to explore that region without having to worry about\n"
	"falling into a pit.  None of the objects available is immediately\n"
	"useful in discovering the secret.",
	// 180
	"Do you need help getting out of here?",
	// 181
	"Don't go west.",
	// 182
	"Gluttony is not one of the troll's vices.  Avarice, however, is.",
	// 183
	"Your lamp is getting dim.  You'd best start wrapping this up, unless\n"
	"You can find some fresh batteries.  I seem to recall there's a vending\n"
	"machine in the maze.  Bring some coins with you.",
	// 184
	"Your lamp has run out of power.",
	// 185
	"There's not much point in wandering around out here, and you can't\n"
	"explore the cave without a lamp.  So let's just call it a day.",
	// 186
	"There are faint rustling noises from the darkness behind you.  As you\n"
	"turn toward them, the beam of your lamp falls across a bearded pirate.\n"
	"He is carrying a large chest.  \"Shiver me timbers!\" he cries, \"I've\n"
	"been spotted!  I'd best hie meself off to the maze to hide me chest!\"\n"
	"With that, he vanishes into the gloom.",
	// 187
	"Your lamp is getting dim.  You'd best go back for those batteries.",
	// 188
	"Your lamp is getting dim.  I'm taking the liberty of replacing the\n"
	"batteries.",
	// 189
	"Your lamp is getting dim, and you're out of spare batteries.  You'd\n"
	"best start wrapping this up.",
	// 190
	"I'm afraid the magazine is written in Dwarvish.",
	// 191
	"\"This is not the maze where the pirate leaves his treasure chest.\"",
	// 192
	"Hmmm, this looks like a clue, which means it'll cost you 10 points to\n"
	"read it.  Should I go ahead and read it anyway?",
	// 193
	"It says, \"there is something strange about this place, such that one\n"
	"of the words I've always known now has a new effect.\"",
	// 194
	"It says the same thing it did before.",
	// 195
	"I'm afraid I don't understand.",
	// 196
	"\"Congratulations on bringing light into the Dark-Room!\"",
	// 197
	"You strike the mirror a resounding blow, whereupon it shatters into a\n"
	"myriad tiny fragments.",
	// 198
	"You have taken the vase and hurled it delicately to the ground.",
	// 199
	"You prod the nearest dwarf, who wakes up grumpily, takes one look at\n"
	"you, curses, and grabs for his axe.",
	// 200
	"Is this acceptable?",
	// 201
	"There's no point in suspending a demonstration game.",
	// 202
	"You awaken only to discover your bits have been dissolving while you\n"
	"slept.  You disappear in a cloud of greasy black smoke.",
	// 203
	"",
	// 204
	""
};

// Magic messages
const char* const mtext [MAGSIZ] = {
	// 0
	"",
	// 1
	"A large cloud of green smoke appears in front of you.  It clears away\n"
	"to reveal a tall wizard, clothed in grey.  He fixes you with a steely\n"
	"glare and declares, \"This adventure has lasted too long.\"  With that\n"
	"he makes a single pass over you with his hands, and everything around\n"
	"you fades away into a grey nothingness.",
	// 2
	"Even wizards have to wait longer than that!",
	// 3
	"I'm terribly sorry, but Colossal Cave is closed.  Our hours are:",
	// 4
	"Only wizards are permitted within the cave right now.",
	// 5
	"We do allow visitors to make short explorations during our off hours.\n"
	"Would you like to do that?",
	// 6
	"Colossal Cave is open to regular adventurers at the following hours:",
	// 7
	"Very well.",
	// 8
	"Only a wizard may continue an adventure this soon.",
	// 9
	"I suggest you resume your adventure at a later time.",
	// 10
	"Do you wish to see the hours?",
	// 11
	"Do you wish to change the hours?",
	// 12
	"New magic word (null to leave unchanged):",
	// 13
	"New magic number (null to leave unchanged):",
	// 14
	"Do you wish to change the message of the day?",
	// 15
	"Okay.  You can save this version now.",
	// 16
	"Are you a wizard?",
	// 17
	"Prove it!  Say the magic word!",
	// 18
	"That is not what I thought it was.  Do you know what I thought it was?",
	// 19
	"Oh dear, you really *are* a wizard!  Sorry to have bothered you . . .",
	// 20
	"Foo, you are nothing but a charlatan!",
	// 21
	"New hours specified by defining \"prime time\".  Give only the hour\n"
	"(E.g. 14, not 14:00 or 2pm).  Enter a negative number after last pair.",
	// 22
	"New hours for Colossal Cave:",
	// 23
	"Limit lines to 70 chars.  End with null line.",
	// 24
	"Line too long, retype:",
	// 25
	"Not enough room for another line.  Ending message here.",
	// 26
	"Do you wish to (re)schedule the next holiday?",
	// 27
	"To begin how many days from today?",
	// 28
	"To last how many days (zero if no holiday)?",
	// 29
	"To be called what (up to 20 characters)?",
	// 30
	"Too small!  Assuming minimum value (45 minutes).",
	// 31
	"Break out of this and save your core-image.",
	// 32
	"Be sure to save your core-image...",
	// 33
	"",
	// 34
	""
};

// Various condition bits
const short int cond [LOCSIZ] = {
	0,0x5,0x1,0x5,0x5,0x1,0x1,0x5,0x11,0x1,
	0x1,0,0,0x20,0,0,0,0,0,0x40,
	0,0,0,0,0x6,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0x4,0,
	0,0,0x80,0x80,0x80,0x80,0x88,0x88,0x88,0x80,
	0x80,0x80,0x80,0x80,0x88,0x80,0x88,0,0x8,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0x80,0x80,0x88,0,0,0x8,0x88,0x80,0,0,
	0,0,0,0,0,0x4,0,0,0,0x100,
	0x101,0x100,0,0,0,0,0,0,0x200,0,
	0,0,0,0x4,0,0x1,0x1,0,0,0,
	0,0,0x8,0x8,0x8,0x8,0x9,0x8,0x8,0x8,
	0x8,0,0,0,0,0,0,0,0,0,
	0
};

// Info on hints
const short int hints[10][5] = {
	{ 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
	{ 0, 9999, 10, 0, 0 },
	{ 0, 9999, 5, 0, 0 },
	{ 0, 4, 2, 62, 63 },
	{ 0, 5, 2, 18, 19 },
	{ 0, 8, 2, 20, 21 },
	{ 0, 75, 4, 176, 177 },
	{ 0, 25, 5, 178, 179 },
	{ 0, 20, 3, 180, 181 }
};

// rtext msg for verb <n>
const short int actspk[35] = {
	0,24,29,0,33,0,33,38,38,42,
	14,43,110,29,110,73,75,29,13,59,
	59,174,109,67,13,147,155,195,146,110,
	13,13,0,0,0
};

// Initial object placement
const short int plac[OBJSIZ] = {
	0,3,3,8,10,11,0,14,13,94,
	96,19,17,101,103,0,106,0,0,3,
	3,0,0,109,25,23,111,35,0,97,
	0,119,117,117,0,130,0,126,140,0,
	96,0,0,0,0,0,0,0,0,0,
	18,27,28,29,30,0,92,95,97,100,
	101,0,119,127,130,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0
};
const short int fixd[OBJSIZ] = {
	0,0,0,9,0,0,0,15,0,-1,
	0,-1,27,-1,0,0,0,-1,0,0,
	0,0,0,-1,-1,67,-1,110,0,-1,
	-1,121,122,122,0,-1,-1,-1,-1,0,
	-1,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,121,0,-1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0
};

// Map of the game
const struct MapEntry c_Map [LOCSIZ] = {
    { // 0
	"",
	"",
	(const struct MapDestEntry[]) {
	    {0,0,0}},
    },
    { // 1
	"You're at end of road again.",
	"You are standing at the end of a road before a small brick building.\n"
	"Around you is a forest. A small stream flows out of the building and\n"
	"down a gully.",
	(const struct MapDestEntry[]) {
	    {wrd_Road,0,2},{wrd_West,0,2},{wrd_Up,0,2},{wrd_Enter,0,3},
	    {wrd_Build,0,3},{wrd_Inward,0,3},{wrd_East,0,3},{wrd_Downstream,0,4},
	    {wrd_Gully,0,4},{wrd_Stream,0,4},{wrd_South,0,4},{wrd_Down,0,4},
	    {wrd_Forest,0,5},{wrd_North,0,5},{wrd_East,0,5},{wrd_Depression,0,8},{0,0,0}},
    },
    { // 2
	"You're at hill in road.",
	"You have walked up a hill, still in the forest. The road slopes back\n"
	"down the other side of the hill. There is a building in the distance.",
	(const struct MapDestEntry[]) {
	    {wrd_Road,0,1},{wrd_Build,0,1},{wrd_Forward,0,1},{wrd_East,0,1},
	    {wrd_North,0,1},{wrd_Down,0,1},{wrd_Forest,0,5},{wrd_North,0,5},
	    {wrd_South,0,5},{0,0,0}},
    },
    { // 3
	"You're inside building.",
	"You are inside a building, a well house for a large spring.",
	(const struct MapDestEntry[]) {
	    {wrd_Enter,0,1},{wrd_Out,0,1},{wrd_Outdoors,0,1},{wrd_West,0,1},
	    {wrd_Xyzzy,0,11},{wrd_Plugh,0,33},{wrd_Downstream,0,79},{wrd_Stream,0,79},{0,0,0}},
    },
    { // 4
	"You're in valley.",
	"You are in a valley in the forest beside a stream tumbling along a\n"
	"rocky bed.",
	(const struct MapDestEntry[]) {
	    {wrd_Upstream,0,1},{wrd_Build,0,1},{wrd_North,0,1},{wrd_Forest,0,5},
	    {wrd_East,0,5},{wrd_West,0,5},{wrd_Up,0,5},{wrd_Downstream,0,7},
	    {wrd_South,0,7},{wrd_Down,0,7},{wrd_Depression,0,8},{0,0,0}},
    },
    { // 5
	"You're in forest.",
	"You are in open forest, with a deep valley to one side.",
	(const struct MapDestEntry[]) {
	    {wrd_Valley,0,4},{wrd_East,0,4},{wrd_Down,0,4},{wrd_Forest,50,5},
	    {wrd_Forward,50,5},{wrd_North,50,5},{wrd_Forest,0,6},{wrd_West,0,5},
	    {wrd_South,0,5},{0,0,0}},
    },
    { // 6
	"You're in forest.",
	"You are in open forest near both a valley and a road.",
	(const struct MapDestEntry[]) {
	    {wrd_Road,0,1},{wrd_North,0,1},{wrd_Valley,0,4},{wrd_East,0,4},
	    {wrd_West,0,4},{wrd_Down,0,4},{wrd_Forest,0,5},{wrd_South,0,5},{0,0,0}},
    },
    { // 7
	"You're at slit in streambed.",
	"At your feet all the water of the stream splashes into a 2-inch slit\n"
	"in the rock. Downstream the streambed is bare rock.",
	(const struct MapDestEntry[]) {
	    {wrd_Build,0,1},{wrd_Upstream,0,4},{wrd_North,0,4},{wrd_Forest,0,5},
	    {wrd_East,0,5},{wrd_West,0,5},{wrd_Downstream,0,8},{wrd_Rock,0,8},
	    {wrd_Bed,0,8},{wrd_South,0,8},{wrd_Slit,0,595},{wrd_Stream,0,595},
	    {wrd_Down,0,595},{0,0,0}},
    },
    { // 8
	"You're outside grate.",
	"You are in a 20-foot depression floored with bare dirt. Set into the\n"
	"dirt is a strong steel grate mounted in concrete. A dry streambed\n"
	"leads into the depression.",
	(const struct MapDestEntry[]) {
	    {wrd_Forest,0,5},{wrd_East,0,5},{wrd_West,0,5},{wrd_South,0,5},
	    {wrd_Build,0,1},{wrd_Upstream,0,7},{wrd_Gully,0,7},{wrd_North,0,7},
	    {wrd_Enter,303,9},{wrd_Inward,303,9},{wrd_Down,303,9},{wrd_Enter,0,593},{0,0,0}},
    },
    { // 9
	"You're below the grate.",
	"You are in a small chamber beneath a 3x3 steel grate to the surface.\n"
	"A low crawl over cobbles leads inward to the west.",
	(const struct MapDestEntry[]) {
	    {wrd_Out,303,8},{wrd_Up,303,8},{wrd_Out,0,593},{wrd_Crawl,0,10},
	    {wrd_Cobbl,0,10},{wrd_Inward,0,10},{wrd_West,0,10},{wrd_Pit,0,14},
	    {wrd_Debris,0,11},{0,0,0}},
    },
    { // 10
	"You're in Cobble Crawl.",
	"You are crawling over cobbles in a low passage. There is a dim light\n"
	"at the east end of the passage.",
	(const struct MapDestEntry[]) {
	    {wrd_Out,0,9},{wrd_Surface,0,9},{wrd_Null,0,9},{wrd_East,0,9},
	    {wrd_Inward,0,11},{wrd_Dark,0,11},{wrd_West,0,11},{wrd_Debris,0,11},
	    {wrd_Pit,0,14},{0,0,0}},
    },
    { // 11
	"You're in Debris Room.",
	"You are in a debris room filled with stuff washed in from the surface.\n"
	"A low wide passage with cobbles becomes plugged with mud and debris\n"
	"here, but an awkward canyon leads upward and west. A note on the wall\n"
	"says \"Magic word XYZZY\".",
	(const struct MapDestEntry[]) {
	    {wrd_Depression,303,8},{wrd_Entrance,0,9},{wrd_Crawl,0,10},{wrd_Cobbl,0,10},
	    {wrd_Passage,0,10},{wrd_Low,0,10},{wrd_East,0,10},{wrd_Canyon,0,12},
	    {wrd_Inward,0,12},{wrd_Up,0,12},{wrd_West,0,12},{wrd_Xyzzy,0,3},
	    {wrd_Pit,0,14},{0,0,0}},
    },
    { // 12
	"",
	"You are in an awkward sloping east/west canyon.",
	(const struct MapDestEntry[]) {
	    {wrd_Depression,303,8},{wrd_Entrance,0,9},{wrd_Down,0,11},{wrd_East,0,11},
	    {wrd_Debris,0,11},{wrd_Inward,0,13},{wrd_Up,0,13},{wrd_West,0,13},
	    {wrd_Pit,0,14},{0,0,0}},
    },
    { // 13
	"You're in Bird Chamber.",
	"You are in a splendid chamber thirty feet high. The walls are frozen\n"
	"rivers of orange stone. An awkward canyon and a good passage exit\n"
	"from east and west sides of the chamber.",
	(const struct MapDestEntry[]) {
	    {wrd_Depression,303,8},{wrd_Entrance,0,9},{wrd_Debris,0,11},{wrd_Canyon,0,12},
	    {wrd_East,0,12},{wrd_Passage,0,14},{wrd_Pit,0,14},{wrd_West,0,14},{0,0,0}},
    },
    { // 14
	"You're at top of small pit.",
	"At your feet is a small pit breathing traces of white mist. An east\n"
	"passage ends here except for a small crack leading on.",
	(const struct MapDestEntry[]) {
	    {wrd_Depression,303,8},{wrd_Entrance,0,9},{wrd_Debris,0,11},{wrd_Passage,0,13},
	    {wrd_East,0,13},{wrd_Down,150,20},{wrd_Pit,150,20},{wrd_Steps,150,20},
	    {wrd_Down,0,15},{wrd_Crack,0,16},{wrd_West,0,16},{0,0,0}},
    },
    { // 15
	"You're in Hall of Mists.",
	"You are at one end of a vast hall stretching forward out of sight to\n"
	"the west. There are openings to either side. Nearby, a wide stone\n"
	"staircase leads downward. The hall is filled with wisps of white mist\n"
	"swaying to and fro almost as if alive. A cold wind blows up the\n"
	"staircase. There is a passage at the top of a dome behind you.",
	(const struct MapDestEntry[]) {
	    {wrd_Left,0,18},{wrd_South,0,18},{wrd_Forward,0,17},{wrd_Hall,0,17},
	    {wrd_West,0,17},{wrd_Stairs,0,19},{wrd_Down,0,19},{wrd_North,0,19},
	    {wrd_Up,150,22},{wrd_Pit,150,22},{wrd_Steps,150,22},{wrd_Dome,150,22},
	    {wrd_Passage,150,22},{wrd_East,150,22},{wrd_Up,0,14},{wrd_Y2,0,34},{0,0,0}},
    },
    { // 16
	"",
	"The crack is far too small for you to follow.",
	(const struct MapDestEntry[]) {
	    {1,0,14},{0,0,0}},
    },
    { // 17
	"You're on east bank of fissure.",
	"You are on the east bank of a fissure slicing clear across the hall.\n"
	"The mist is quite thick here, and the fissure is too wide to jump.",
	(const struct MapDestEntry[]) {
	    {wrd_Hall,0,15},{wrd_East,0,15},{wrd_Jump,312,596},{wrd_Forward,412,21},
	    {wrd_Over,412,597},{wrd_Across,412,597},{wrd_West,412,597},{wrd_Cross,412,597},
	    {wrd_Over,0,27},{0,0,0}},
    },
    { // 18
	"You're in Nugget of Gold Room.",
	"This is a low room with a crude note on the wall. The note says,\n"
	"\"You won't get it up the steps\".",
	(const struct MapDestEntry[]) {
	    {wrd_Hall,0,15},{wrd_Out,0,15},{wrd_North,0,15},{0,0,0}},
    },
    { // 19
	"You're in Hall of Mt King.",
	"You are in the Hall of the Mountain King, with passages off in all\n"
	"directions.",
	(const struct MapDestEntry[]) {
	    {wrd_Stairs,0,15},{wrd_Up,0,15},{wrd_East,0,15},{wrd_North,311,28},
	    {wrd_Left,311,28},{wrd_South,311,29},{wrd_Right,311,29},{wrd_West,311,30},
	    {wrd_Forward,311,30},{wrd_North,0,32},{wrd_SW,35,74},{wrd_SW,211,32},
	    {wrd_Secret,0,74},{0,0,0}},
    },
    { // 20
	"",
	"You are at the bottom of the pit with a broken neck.",
	(const struct MapDestEntry[]) {
	    {1,0,0},{0,0,0}},
    },
    { // 21
	"",
	"You didn't make it.",
	(const struct MapDestEntry[]) {
	    {1,0,0},{0,0,0}},
    },
    { // 22
	"",
	"The dome is unclimbable.",
	(const struct MapDestEntry[]) {
	    {1,0,15},{0,0,0}},
    },
    { // 23
	"You're at west end of Twopit Room.",
	"You are at the west end of the Twopit Room. There is a large hole in\n"
	"the wall above the pit at this end of the room.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,67},{wrd_Across,0,67},{wrd_West,0,68},{wrd_Slab,0,68},
	    {wrd_Down,0,25},{wrd_Pit,0,25},{wrd_Hole,0,648},{0,0,0}},
    },
    { // 24
	"You're in east pit.",
	"You are at the bottom of the eastern pit in the Twopit Room. There is\n"
	"a small pool of oil in one corner of the pit.",
	(const struct MapDestEntry[]) {
	    {wrd_Up,0,67},{wrd_Out,0,67},{0,0,0}},
    },
    { // 25
	"You're in west pit.",
	"You are at the bottom of the western pit in the Twopit Room. There is\n"
	"a large hole in the wall about 25 feet above you.",
	(const struct MapDestEntry[]) {
	    {wrd_Up,0,23},{wrd_Out,0,23},{wrd_Climb,724,31},{wrd_Climb,0,26},{0,0,0}},
    },
    { // 26
	"",
	"You clamber up the plant and scurry through the hole at the top.",
	(const struct MapDestEntry[]) {
	    {1,0,88},{0,0,0}},
    },
    { // 27
	"",
	"You are on the west side of the fissure in the Hall of Mists.",
	(const struct MapDestEntry[]) {
	    {wrd_Jump,312,596},{wrd_Forward,412,21},{wrd_Over,412,597},{wrd_Across,412,597},
	    {wrd_East,412,597},{wrd_Cross,412,597},{wrd_Over,0,17},{wrd_North,0,40},
	    {wrd_West,0,41},{0,0,0}},
    },
    { // 28
	"",
	"You are in a low N/S passage at a hole in the floor. The hole goes\n"
	"down to an E/W passage.",
	(const struct MapDestEntry[]) {
	    {wrd_Hall,0,19},{wrd_Out,0,19},{wrd_South,0,19},{wrd_North,0,33},
	    {wrd_Y2,0,33},{wrd_Down,0,36},{wrd_Hole,0,36},{0,0,0}},
    },
    { // 29
	"",
	"You are in the south side chamber.",
	(const struct MapDestEntry[]) {
	    {wrd_Hall,0,19},{wrd_Out,0,19},{wrd_North,0,19},{0,0,0}},
    },
    { // 30
	"",
	"You are in the west side chamber of the Hall of the Mountain King.\n"
	"A passage continues west and up here.",
	(const struct MapDestEntry[]) {
	    {wrd_Hall,0,19},{wrd_Out,0,19},{wrd_East,0,19},{wrd_West,0,62},
	    {wrd_Up,0,62},{0,0,0}},
    },
    { // 31
	"",
	"",
	(const struct MapDestEntry[]) {
	    {1,524,89},{1,0,90},{0,0,0}},
    },
    { // 32
	"",
	"You can't get by the snake.",
	(const struct MapDestEntry[]) {
	    {1,0,19},{0,0,0}},
    },
    { // 33
	"You're at \"Y2\".",
	"You are in a large room, with a passage to the south, a passage to the\n"
	"west, and a wall of broken rock to the east. There is a large \"Y2\" on\n"
	"a rock in the room's center.",
	(const struct MapDestEntry[]) {
	    {wrd_Plugh,0,3},{wrd_South,0,28},{wrd_East,0,34},{wrd_Wall,0,34},
	    {wrd_Broke,0,34},{wrd_West,0,35},{wrd_Plover,159,302},{wrd_Plover,0,100},{0,0,0}},
    },
    { // 34
	"",
	"You are in a jumble of rock, with cracks everywhere.",
	(const struct MapDestEntry[]) {
	    {wrd_Down,0,33},{wrd_Y2,0,33},{wrd_Up,0,15},{0,0,0}},
    },
    { // 35
	"You're at window on pit.",
	"You're at a low window overlooking a huge pit, which extends up out of\n"
	"sight. A floor is indistinctly visible over 50 feet below. Traces of\n"
	"white mist cover the floor of the pit, becoming thicker to the right.\n"
	"Marks in the dust around the window would seem to indicate that\n"
	"someone has been here recently. Directly across the pit from you and\n"
	"25 feet away there is a similar window looking into a lighted room. A\n"
	"shadowy figure can be seen there peering back at you.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,33},{wrd_Y2,0,33},{wrd_Jump,0,20},{0,0,0}},
    },
    { // 36
	"You're in dirty passage.",
	"You are in a dirty broken passage. To the east is a crawl. To the\n"
	"west is a large passage. Above you is a hole to another passage.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,37},{wrd_Crawl,0,37},{wrd_Up,0,28},{wrd_Hole,0,28},
	    {wrd_West,0,39},{wrd_Bedquilt,0,65},{0,0,0}},
    },
    { // 37
	"",
	"You are on the brink of a small clean climbable pit. A crawl leads\n"
	"west.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,36},{wrd_Crawl,0,36},{wrd_Down,0,38},{wrd_Pit,0,38},
	    {wrd_Climb,0,38},{0,0,0}},
    },
    { // 38
	"",
	"You are in the bottom of a small pit with a little stream, which\n"
	"enters and exits through tiny slits.",
	(const struct MapDestEntry[]) {
	    {wrd_Climb,0,37},{wrd_Up,0,37},{wrd_Out,0,37},{wrd_Slit,0,595},
	    {wrd_Stream,0,595},{wrd_Down,0,595},{wrd_Upstream,0,595},{wrd_Downstream,0,595},{0,0,0}},
    },
    { // 39
	"You're in Dusty Rock room.",
	"You are in a large room full of dusty rocks. There is a big hole in\n"
	"the floor. There are cracks everywhere, and a passage leading east.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,36},{wrd_Passage,0,36},{wrd_Down,0,64},{wrd_Hole,0,64},
	    {wrd_Floor,0,64},{wrd_Bedquilt,0,65},{0,0,0}},
    },
    { // 40
	"",
	"You have crawled through a very low wide passage parallel to and north\n"
	"of the Hall of Mists.",
	(const struct MapDestEntry[]) {
	    {1,0,41},{0,0,0}},
    },
    { // 41
	"You're at west end of Hall of Mists.",
	"You are at the west end of Hall of Mists. A low wide crawl continues\n"
	"west and another goes north. To the south is a little passage 6 feet\n"
	"off the floor.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,42},{wrd_Up,0,42},{wrd_Passage,0,42},{wrd_Climb,0,42},
	    {wrd_East,0,27},{wrd_North,0,59},{wrd_West,0,60},{wrd_Crawl,0,60},{0,0,0}},
    },
    { // 42
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_Up,0,41},{wrd_North,0,42},{wrd_East,0,43},{wrd_South,0,45},
	    {wrd_West,0,80},{0,0,0}},
    },
    { // 43
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,42},{wrd_South,0,44},{wrd_East,0,45},{0,0,0}},
    },
    { // 44
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,43},{wrd_Down,0,48},{wrd_South,0,50},{wrd_North,0,82},{0,0,0}},
    },
    { // 45
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,42},{wrd_North,0,43},{wrd_East,0,46},{wrd_South,0,47},
	    {wrd_Up,0,87},{wrd_Down,0,87},{0,0,0}},
    },
    { // 46
	"",
	"Dead end",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,45},{wrd_Out,0,45},{0,0,0}},
    },
    { // 47
	"",
	"Dead end",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,45},{wrd_Out,0,45},{0,0,0}},
    },
    { // 48
	"",
	"Dead end",
	(const struct MapDestEntry[]) {
	    {wrd_Up,0,44},{wrd_Out,0,44},{0,0,0}},
    },
    { // 49
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,50},{wrd_West,0,51},{0,0,0}},
    },
    { // 50
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,44},{wrd_West,0,49},{wrd_Down,0,51},{wrd_South,0,52},{0,0,0}},
    },
    { // 51
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,49},{wrd_Up,0,50},{wrd_East,0,52},{wrd_South,0,53},{0,0,0}},
    },
    { // 52
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,50},{wrd_East,0,51},{wrd_South,0,52},{wrd_Up,0,53},
	    {wrd_North,0,55},{wrd_Down,0,86},{0,0,0}},
    },
    { // 53
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,51},{wrd_North,0,52},{wrd_South,0,54},{0,0,0}},
    },
    { // 54
	"",
	"Dead end",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,53},{wrd_Out,0,53},{0,0,0}},
    },
    { // 55
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,52},{wrd_North,0,55},{wrd_Down,0,56},{wrd_East,0,57},{0,0,0}},
    },
    { // 56
	"",
	"Dead end",
	(const struct MapDestEntry[]) {
	    {wrd_Up,0,55},{wrd_Out,0,55},{0,0,0}},
    },
    { // 57
	"You're at brink of pit.",
	"You are on the brink of a thirty foot pit with a massive orange column\n"
	"down one wall. You could climb down here but you could not get back\n"
	"up. The maze continues at this level.",
	(const struct MapDestEntry[]) {
	    {wrd_Down,0,13},{wrd_Climb,0,13},{wrd_West,0,55},{wrd_South,0,58},
	    {wrd_North,0,83},{wrd_East,0,84},{0,0,0}},
    },
    { // 58
	"",
	"Dead end",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,57},{wrd_Out,0,57},{0,0,0}},
    },
    { // 59
	"",
	"You have crawled through a very low wide passage parallel to and north\n"
	"of the Hall of Mists.",
	(const struct MapDestEntry[]) {
	    {1,0,27},{0,0,0}},
    },
    { // 60
	"You're at east end of Long Hall.",
	"You are at the east end of a very long hall apparently without side\n"
	"chambers. To the east a low wide crawl slants up. To the north a\n"
	"round two foot hole slants down.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,41},{wrd_Up,0,41},{wrd_Crawl,0,41},{wrd_West,0,61},
	    {wrd_North,0,62},{wrd_Down,0,62},{wrd_Hole,0,62},{0,0,0}},
    },
    { // 61
	"You're at west end of Long Hall.",
	"You are at the west end of a very long featureless hall. The hall\n"
	"joins up with a narrow north/south passage.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,60},{wrd_North,0,62},{wrd_South,100,107},{0,0,0}},
    },
    { // 62
	"",
	"You are at a crossover of a high N/S passage and a low E/W one.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,60},{wrd_North,0,63},{wrd_East,0,30},{wrd_South,0,61},{0,0,0}},
    },
    { // 63
	"",
	"Dead end",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,62},{wrd_Out,0,62},{0,0,0}},
    },
    { // 64
	"You're at Complex Junction.",
	"You are at a complex junction. A low hands and knees passage from the\n"
	"north joins a higher crawl from the east to make a walking passage\n"
	"going west. There is also a large room above. The air is damp here.",
	(const struct MapDestEntry[]) {
	    {wrd_Up,0,39},{wrd_Climb,0,39},{wrd_Room,0,39},{wrd_West,0,65},
	    {wrd_Bedquilt,0,65},{wrd_North,0,103},{wrd_Shell,0,103},{wrd_East,0,106},{0,0,0}},
    },
    { // 65
	"",
	"You are in Bedquilt, a long east/west passage with holes everywhere.\n"
	"To explore at random select north, south, up, or down.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,64},{wrd_West,0,66},{wrd_South,80,556},{wrd_Slab,0,68},
	    {wrd_Up,80,556},{wrd_Up,50,70},{wrd_Up,0,39},{wrd_North,60,556},
	    {wrd_North,75,72},{wrd_North,0,71},{wrd_Down,80,556},{wrd_Down,0,106},{0,0,0}},
    },
    { // 66
	"You're in Swiss Cheese room.",
	"You are in a room whose walls resemble Swiss cheese. Obvious passages\n"
	"go west, east, NE, and NW. Part of the room is occupied by a large\n"
	"bedrock block.",
	(const struct MapDestEntry[]) {
	    {wrd_NE,0,65},{wrd_West,0,67},{wrd_South,80,556},{wrd_Canyon,0,77},
	    {wrd_East,0,96},{wrd_NW,50,556},{wrd_Oriental,0,97},{0,0,0}},
    },
    { // 67
	"You're at east end of Twopit Room.",
	"You are at the east end of the Twopit Room. The floor here is\n"
	"littered with thin rock slabs, which make it easy to descend the pits.\n"
	"There is a path here bypassing the pits to connect passages from east\n"
	"and west. There are holes all over, but the only big one is on the\n"
	"wall directly over the west pit where you can't get to it.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,66},{wrd_West,0,23},{wrd_Across,0,23},{wrd_Down,0,24},
	    {wrd_Pit,0,24},{0,0,0}},
    },
    { // 68
	"You're in Slab Room.",
	"You are in a large low circular chamber whose floor is an immense slab\n"
	"fallen from the ceiling (Slab Room). East and west there once were\n"
	"large passages, but they are now filled with boulders. Low small\n"
	"passages go north and south, and the south one quickly bends west\n"
	"around the boulders.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,23},{wrd_Up,0,69},{wrd_Climb,0,69},{wrd_North,0,65},{0,0,0}},
    },
    { // 69
	"",
	"You are in a secret N/S canyon above a large room.",
	(const struct MapDestEntry[]) {
	    {wrd_Down,0,68},{wrd_Slab,0,68},{wrd_South,331,120},{wrd_South,0,119},
	    {wrd_North,0,109},{wrd_Reservoir,0,113},{0,0,0}},
    },
    { // 70
	"",
	"You are in a secret N/S canyon above a sizable passage.",
	(const struct MapDestEntry[]) {
	    {wrd_North,0,71},{wrd_Down,0,65},{wrd_Passage,0,65},{wrd_South,0,111},{0,0,0}},
    },
    { // 71
	"You're at junction of three secret canyons.",
	"You are in a secret canyon at a junction of three canyons, bearing\n"
	"north, south, and SE. The north one is as tall as the other two\n"
	"combined.",
	(const struct MapDestEntry[]) {
	    {wrd_SE,0,65},{wrd_South,0,70},{wrd_North,0,110},{0,0,0}},
    },
    { // 72
	"",
	"You are in a large low room. Crawls lead north, SE, and SW.",
	(const struct MapDestEntry[]) {
	    {wrd_Bedquilt,0,65},{wrd_SW,0,118},{wrd_North,0,73},{wrd_SE,0,97},
	    {wrd_Oriental,0,97},{0,0,0}},
    },
    { // 73
	"",
	"Dead end crawl.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,72},{wrd_Crawl,0,72},{wrd_Out,0,72},{0,0,0}},
    },
    { // 74
	"You're in secret E/W canyon above tight canyon.",
	"You are in a secret canyon which here runs E/W. It crosses over a\n"
	"very tight canyon 15 feet below. If you go down you may not be able\n"
	"to get back up.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,19},{wrd_West,331,120},{wrd_West,0,121},{wrd_Down,0,75},{0,0,0}},
    },
    { // 75
	"",
	"You are at a wide place in a very tight N/S canyon.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,76},{wrd_North,0,77},{0,0,0}},
    },
    { // 76
	"",
	"The canyon here becomes too tight to go further south.",
	(const struct MapDestEntry[]) {
	    {wrd_North,0,75},{0,0,0}},
    },
    { // 77
	"",
	"You are in a tall E/W canyon. A low tight crawl goes 3 feet north and\n"
	"seems to open up.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,75},{wrd_West,0,78},{wrd_North,0,66},{wrd_Crawl,0,66},{0,0,0}},
    },
    { // 78
	"",
	"The canyon runs into a mass of boulders -- dead end.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,77},{0,0,0}},
    },
    { // 79
	"",
	"The stream flows out through a pair of 1 foot diameter sewer pipes.\n"
	"It would be advisable to use the exit.",
	(const struct MapDestEntry[]) {
	    {1,0,3},{0,0,0}},
    },
    { // 80
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_North,0,42},{wrd_West,0,80},{wrd_South,0,80},{wrd_East,0,81},{0,0,0}},
    },
    { // 81
	"",
	"Dead end",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,80},{wrd_Out,0,80},{0,0,0}},
    },
    { // 82
	"",
	"Dead end",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,44},{wrd_Out,0,44},{0,0,0}},
    },
    { // 83
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,57},{wrd_East,0,84},{wrd_West,0,85},{0,0,0}},
    },
    { // 84
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_North,0,57},{wrd_West,0,83},{wrd_NW,0,114},{0,0,0}},
    },
    { // 85
	"",
	"Dead end",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,83},{wrd_Out,0,83},{0,0,0}},
    },
    { // 86
	"",
	"Dead end",
	(const struct MapDestEntry[]) {
	    {wrd_Up,0,52},{wrd_Out,0,52},{0,0,0}},
    },
    { // 87
	"",
	"You are in a maze of twisty little passages, all alike.",
	(const struct MapDestEntry[]) {
	    {wrd_Up,0,45},{wrd_Down,0,45},{0,0,0}},
    },
    { // 88
	"You're in narrow corridor.",
	"You are in a long, narrow corridor stretching out of sight to the\n"
	"west. At the eastern end is a hole through which you can see a\n"
	"profusion of leaves.",
	(const struct MapDestEntry[]) {
	    {wrd_Down,0,25},{wrd_Climb,0,25},{wrd_East,0,25},{wrd_Jump,0,20},
	    {wrd_West,0,92},{wrd_Giant,0,92},{0,0,0}},
    },
    { // 89
	"",
	"There is nothing here to climb. Use \"up\" or \"out\" to leave the pit.",
	(const struct MapDestEntry[]) {
	    {1,0,25},{0,0,0}},
    },
    { // 90
	"",
	"You have climbed up the plant and out of the pit.",
	(const struct MapDestEntry[]) {
	    {1,0,23},{0,0,0}},
    },
    { // 91
	"You're at steep incline above large room.",
	"You are at the top of a steep incline above a large room. You could\n"
	"climb down here, but you would not be able to climb up. There is a\n"
	"passage leading back to the north.",
	(const struct MapDestEntry[]) {
	    {wrd_North,0,95},{wrd_Cavern,0,95},{wrd_Passage,0,95},{wrd_Down,0,72},
	    {wrd_Climb,0,72},{0,0,0}},
    },
    { // 92
	"You're in Giant Room.",
	"You are in the Giant Room. The ceiling here is too high up for your\n"
	"lamp to show it. Cavernous passages lead east, north, and south. On\n"
	"the west wall is scrawled the inscription, \"Fee fie foe foo\" [sic].",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,88},{wrd_East,0,93},{wrd_North,0,94},{0,0,0}},
    },
    { // 93
	"",
	"The passage here is blocked by a recent cave-in.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,92},{wrd_Giant,0,92},{wrd_Out,0,92},{0,0,0}},
    },
    { // 94
	"",
	"You are at one end of an immense north/south passage.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,92},{wrd_Giant,0,92},{wrd_Passage,0,92},{wrd_North,309,95},
	    {wrd_Enter,309,95},{wrd_Cavern,309,95},{wrd_North,0,611},{0,0,0}},
    },
    { // 95
	"You're in cavern with waterfall.",
	"You are in a magnificent cavern with a rushing stream, which cascades\n"
	"over a sparkling waterfall into a roaring whirlpool which disappears\n"
	"through a hole in the floor. Passages exit to the south and west.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,94},{wrd_Out,0,94},{wrd_Giant,0,92},{wrd_West,0,91},{0,0,0}},
    },
    { // 96
	"You're in Soft Room.",
	"You are in the Soft Room. The walls are covered with heavy curtains,\n"
	"the floor with a thick pile carpet. Moss covers the ceiling.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,66},{wrd_Out,0,66},{0,0,0}},
    },
    { // 97
	"You're in Oriental Room.",
	"This is the Oriental Room. Ancient oriental cave drawings cover the\n"
	"walls. A gently sloping passage leads upward to the north, another\n"
	"passage leads SE, and a hands and knees crawl leads west.",
	(const struct MapDestEntry[]) {
	    {wrd_SE,0,66},{wrd_West,0,72},{wrd_Crawl,0,72},{wrd_Up,0,98},
	    {wrd_North,0,98},{wrd_Cavern,0,98},{0,0,0}},
    },
    { // 98
	"You're in Misty Cavern.",
	"You are following a wide path around the outer edge of a large cavern.\n"
	"Far below, through a heavy white mist, strange splashing noises can be\n"
	"heard. The mist rises up through a fissure in the ceiling. The path\n"
	"exits to the south and west.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,97},{wrd_Oriental,0,97},{wrd_West,0,99},{0,0,0}},
    },
    { // 99
	"You're in Alcove.",
	"You are in an alcove. A small NW path seems to widen after a short\n"
	"distance. An extremely tight tunnel leads east. It looks like a very\n"
	"tight squeeze. An eerie light can be seen at the other end.",
	(const struct MapDestEntry[]) {
	    {wrd_NW,0,98},{wrd_Cavern,0,98},{wrd_East,0,301},{wrd_Passage,0,301},
	    {wrd_East,0,100},{0,0,0}},
    },
    { // 100
	"You're in Plover Room.",
	"You're in a small chamber lit by an eerie green light. An extremely\n"
	"narrow tunnel exits to the west. A dark corridor leads NE.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,301},{wrd_Passage,0,301},{wrd_Out,0,301},{wrd_West,0,99},
	    {wrd_Plover,159,302},{wrd_Plover,0,33},{wrd_NE,0,101},{wrd_Dark,0,101},{0,0,0}},
    },
    { // 101
	"You're in Dark-Room.",
	"You're in the Dark-Room. A corridor leading south is the only exit.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,100},{wrd_Plover,0,100},{wrd_Out,0,100},{0,0,0}},
    },
    { // 102
	"You're in Arched Hall.",
	"You are in an arched hall. A coral passage once continued up and east\n"
	"from here, but is now blocked by debris. The air smells of sea water.",
	(const struct MapDestEntry[]) {
	    {wrd_Down,0,103},{wrd_Shell,0,103},{wrd_Out,0,103},{0,0,0}},
    },
    { // 103
	"You're in Shell Room.",
	"You're in a large room carved out of sedimentary rock. The floor and\n"
	"walls are littered with bits of shells embedded in the stone. A\n"
	"shallow passage proceeds downward, and a somewhat steeper one leads\n"
	"up. A low hands and knees passage enters from the south.",
	(const struct MapDestEntry[]) {
	    {wrd_Up,0,102},{wrd_Hall,0,102},{wrd_Down,0,104},{wrd_South,114,618},
	    {wrd_South,115,619},{wrd_South,0,64},{0,0,0}},
    },
    { // 104
	"",
	"You are in a long sloping corridor with ragged sharp walls.",
	(const struct MapDestEntry[]) {
	    {wrd_Up,0,103},{wrd_Shell,0,103},{wrd_Down,0,105},{0,0,0}},
    },
    { // 105
	"",
	"You are in a cul-de-sac about eight feet across.",
	(const struct MapDestEntry[]) {
	    {wrd_Up,0,104},{wrd_Out,0,104},{wrd_Shell,0,103},{0,0,0}},
    },
    { // 106
	"You're in Anteroom.",
	"You are in an anteroom leading to a large passage to the east. Small\n"
	"passages go west and up. The remnants of recent digging are evident.\n"
	"A sign in midair here says \"Cave under construction beyond this point.\n"
	"Proceed at own risk. [Witt construction company]\"",
	(const struct MapDestEntry[]) {
	    {wrd_Up,0,64},{wrd_West,0,65},{wrd_East,0,108},{0,0,0}},
    },
    { // 107
	"",
	"You are in a maze of twisty little passages, all different.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,131},{wrd_SW,0,132},{wrd_NE,0,133},{wrd_SE,0,134},
	    {wrd_Up,0,135},{wrd_NW,0,136},{wrd_East,0,137},{wrd_West,0,138},
	    {wrd_North,0,139},{wrd_Down,0,61},{0,0,0}},
    },
    { // 108
	"You're at Witt's End.",
	"You are at Witt's End. Passages lead off in *all* directions.",
	(const struct MapDestEntry[]) {
	    {wrd_East,95,556},{wrd_North,95,556},{wrd_South,95,556},{wrd_NE,95,556},
	    {wrd_SE,95,556},{wrd_SW,95,556},{wrd_NW,95,556},{wrd_Up,95,556},
	    {wrd_Down,95,556},{wrd_East,0,106},{wrd_West,0,626},{0,0,0}},
    },
    { // 109
	"You're in Mirror Canyon.",
	"You are in a north/south canyon about 25 feet across. The floor is\n"
	"covered by white mist seeping in from the north. The walls extend\n"
	"upward for well over 100 feet. Suspended from some unseen point far\n"
	"above you, an enormous two-sided mirror is hanging parallel to and\n"
	"midway between the canyon walls. (The mirror is obviously provided\n"
	"for the use of the dwarves, who as you know, are extremely vain)\n"
	"A small window can be seen in either wall, some fifty feet up.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,69},{wrd_North,0,113},{wrd_Reservoir,0,113},{0,0,0}},
    },
    { // 110
	"You're at window on pit.",
	"You're at a low window overlooking a huge pit, which extends up out of\n"
	"sight. A floor is indistinctly visible over 50 feet below. Traces of\n"
	"white mist cover the floor of the pit, becoming thicker to the left.\n"
	"Marks in the dust around the window would seem to indicate that\n"
	"someone has been here recently. Directly across the pit from you and\n"
	"25 feet away there is a similar window looking into a lighted room. A\n"
	"shadowy figure can be seen there peering back at you.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,71},{wrd_Jump,0,20},{0,0,0}},
    },
    { // 111
	"You're at top of stalactite.",
	"A large stalactite extends from the roof and almost reaches the floor\n"
	"below. You could climb down it, and jump from it to the floor, but\n"
	"having done so you would be unable to reach it to climb back up.",
	(const struct MapDestEntry[]) {
	    {wrd_North,0,70},{wrd_Down,40,50},{wrd_Jump,40,50},{wrd_Climb,40,50},
	    {wrd_Down,50,53},{wrd_Down,0,45},{0,0,0}},
    },
    { // 112
	"",
	"You are in a little maze of twisting passages, all different.",
	(const struct MapDestEntry[]) {
	    {wrd_SW,0,131},{wrd_North,0,132},{wrd_East,0,133},{wrd_NW,0,134},
	    {wrd_SE,0,135},{wrd_NE,0,136},{wrd_West,0,137},{wrd_Down,0,138},
	    {wrd_Up,0,139},{wrd_South,0,140},{0,0,0}},
    },
    { // 113
	"You're at Reservoir.",
	"You are at the edge of a large underground reservoir. An opaque cloud\n"
	"of white mist fills the room and rises rapidly upward. The lake is\n"
	"fed by a stream, which tumbles out of a hole in the wall about 10 feet\n"
	"overhead and splashes noisily into the water somewhere within the\n"
	"mist. The only passage goes back toward the south.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,109},{wrd_Out,0,109},{109,0,109},{0,0,0}},
    },
    { // 114
	"",
	"Dead end",
	(const struct MapDestEntry[]) {
	    {wrd_SE,0,84},{0,0,0}},
    },
    { // 115
	"You're at NE end.",
	"You are at the northeast end of an immense room, even larger than the\n"
	"Giant Room. It appears to be a repository for the \"Adventure\"\n"
	"program. Massive torches far overhead bathe the room with smoky\n"
	"yellow light. Scattered about you can be seen a pile of bottles (all\n"
	"of them empty), a nursery of young beanstalks murmuring quietly, a bed\n"
	"of oysters, a bundle of black rods with rusty stars on their ends, and\n"
	"a collection of brass lanterns. Off to one side a great many dwarves\n"
	"are sleeping on the floor, snoring loudly. A sign nearby reads: \"Do\n"
	"not disturb the dwarves!\" An immense mirror is hanging against one\n"
	"wall, and stretches to the other end of the room, where various other\n"
	"sundry objects can be glimpsed dimly in the distance.",
	(const struct MapDestEntry[]) {
	    {wrd_SW,0,116},{0,0,0}},
    },
    { // 116
	"You're at SW end.",
	"You are at the southwest end of the Repository. To one side is a pit\n"
	"full of fierce green snakes. On the other side is a row of small\n"
	"wicker cages, each of which contains a little sulking bird. In one\n"
	"corner is a bundle of black rods with rusty marks on their ends. A\n"
	"large number of velvet pillows are scattered about on the floor. A\n"
	"vast mirror stretches off to the northeast. At your feet is a large\n"
	"steel grate, next to which is a sign which reads, \"Treasure Vault.\n"
	"Keys in Main Office.\"",
	(const struct MapDestEntry[]) {
	    {wrd_NE,0,115},{wrd_Down,0,593},{0,0,0}},
    },
    { // 117
	"You're on SW side of chasm.",
	"You are on one side of a large, deep chasm. A heavy white mist rising\n"
	"up from below obscures all view of the far side. A SW path leads away\n"
	"from the chasm into a winding corridor.",
	(const struct MapDestEntry[]) {
	    {wrd_SW,0,118},{wrd_Over,233,660},{wrd_Across,233,660},{wrd_Cross,233,660},
	    {wrd_NE,233,660},{wrd_Over,332,661},{wrd_Over,0,303},{wrd_Jump,332,21},
	    {wrd_Jump,0,596},{0,0,0}},
    },
    { // 118
	"You're in sloping corridor.",
	"You are in a long winding corridor sloping out of sight in both\n"
	"directions.",
	(const struct MapDestEntry[]) {
	    {wrd_Down,0,72},{wrd_Up,0,117},{0,0,0}},
    },
    { // 119
	"",
	"You are in a secret canyon which exits to the north and east.",
	(const struct MapDestEntry[]) {
	    {wrd_North,0,69},{wrd_Out,0,69},{wrd_East,0,653},{wrd_Forward,0,653},{0,0,0}},
    },
    { // 120
	"",
	"You are in a secret canyon which exits to the north and east.",
	(const struct MapDestEntry[]) {
	    {wrd_North,0,69},{wrd_East,0,74},{0,0,0}},
    },
    { // 121
	"",
	"You are in a secret canyon which exits to the north and east.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,74},{wrd_Out,0,74},{wrd_North,0,653},{wrd_Forward,0,653},{0,0,0}},
    },
    { // 122
	"You're on NE side of chasm.",
	"You are on the far side of the chasm. A NE path leads away from the\n"
	"chasm on this side.",
	(const struct MapDestEntry[]) {
	    {wrd_NE,0,123},{wrd_Over,233,660},{wrd_Across,233,660},{wrd_Cross,233,660},
	    {wrd_SW,233,660},{wrd_Over,0,303},{wrd_Jump,0,596},{wrd_Fork,0,124},
	    {wrd_View,0,126},{wrd_Barren,0,129},{0,0,0}},
    },
    { // 123
	"You're in corridor.",
	"You're in a long east/west corridor. A faint rumbling noise can be\n"
	"heard in the distance.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,122},{wrd_East,0,124},{wrd_Fork,0,124},{wrd_View,0,126},
	    {wrd_Barren,0,129},{0,0,0}},
    },
    { // 124
	"You're at fork in path.",
	"The path forks here. The left fork leads northeast. A dull rumbling\n"
	"seems to get louder in that direction. The right fork leads southeast\n"
	"down a gentle slope. The main corridor enters from the west.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,123},{wrd_NE,0,125},{wrd_Left,0,125},{wrd_SE,0,128},
	    {wrd_Right,0,128},{wrd_Down,0,128},{wrd_View,0,126},{wrd_Barren,0,129},{0,0,0}},
    },
    { // 125
	"You're at junction with warm walls.",
	"The walls are quite warm here. From the north can be heard a steady\n"
	"roar, so loud that the entire cave seems to be trembling. Another\n"
	"passage leads south, and a low crawl goes east.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,124},{wrd_Fork,0,124},{wrd_North,0,126},{wrd_View,0,126},
	    {wrd_East,0,127},{wrd_Crawl,0,127},{0,0,0}},
    },
    { // 126
	"You're at Breath-taking View.",
	"You are on the edge of a breath-taking view. Far below you is an\n"
	"active volcano, from which great gouts of molten lava come surging\n"
	"out, cascading back down into the depths. The glowing rock fills the\n"
	"farthest reaches of the cavern with a blood-red glare, giving every-\n"
	"thing an eerie, macabre appearance. The air is filled with flickering\n"
	"sparks of ash and a heavy smell of brimstone. The walls are hot to\n"
	"the touch, and the thundering of the volcano drowns out all other\n"
	"sounds. Embedded in the jagged roof far overhead are myriad twisted\n"
	"formations composed of pure white alabaster, which scatter the murky\n"
	"light into sinister apparitions upon the walls. To one side is a deep\n"
	"gorge, filled with a bizarre chaos of tortured rock which seems to\n"
	"have been crafted by the devil himself. An immense river of fire\n"
	"crashes out from the depths of the volcano, burns its way through the\n"
	"gorge, and plummets into a bottomless pit far off to your left. To\n"
	"the right, an immense geyser of blistering steam erupts continuously\n"
	"from a barren island in the center of a sulfurous lake, which bubbles\n"
	"ominously. The far right wall is aflame with an incandescence of its\n"
	"own, which lends an additional infernal splendor to the already\n"
	"hellish scene. A dark, foreboding passage exits to the south.",
	(const struct MapDestEntry[]) {
	    {wrd_South,0,125},{wrd_Passage,0,125},{wrd_Out,0,125},{wrd_Fork,0,124},
	    {wrd_Down,0,610},{wrd_Jump,0,610},{0,0,0}},
    },
    { // 127
	"You're in Chamber of Boulders.",
	"You are in a small chamber filled with large boulders. The walls are\n"
	"very warm, causing the air in the room to be almost stifling from the\n"
	"heat. The only exit is a crawl heading west, through which is coming\n"
	"a low rumbling.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,125},{wrd_Out,0,125},{wrd_Crawl,0,125},{wrd_Fork,0,124},
	    {wrd_View,0,126},{0,0,0}},
    },
    { // 128
	"You're in limestone passage.",
	"You are walking along a gently sloping north/south passage lined with\n"
	"oddly shaped limestone formations.",
	(const struct MapDestEntry[]) {
	    {wrd_North,0,124},{wrd_Up,0,124},{wrd_Fork,0,124},{wrd_South,0,129},
	    {wrd_Down,0,129},{wrd_Barren,0,129},{wrd_View,0,126},{0,0,0}},
    },
    { // 129
	"You're in front of barren room.",
	"You are standing at the entrance to a large, barren room. A sign\n"
	"posted above the entrance reads: \"Caution: Bear in room!\"",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,128},{wrd_Up,0,128},{wrd_Fork,0,124},{wrd_East,0,130},
	    {wrd_Inward,0,130},{wrd_Barren,0,130},{wrd_Enter,0,130},{wrd_View,0,126},{0,0,0}},
    },
    { // 130
	"You're in Barren Room.",
	"You are inside a barren room. The center of the room is completely\n"
	"empty except for some dust. Marks in the dust lead away toward the\n"
	"far end of the room. The only exit is the way you came in.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,129},{wrd_Out,0,129},{wrd_Fork,0,124},{wrd_View,0,126},{0,0,0}},
    },
    { // 131
	"",
	"You are in a maze of twisting little passages, all different.",
	(const struct MapDestEntry[]) {
	    {wrd_West,0,107},{wrd_SE,0,132},{wrd_NW,0,133},{wrd_SW,0,134},
	    {wrd_NE,0,135},{wrd_Up,0,136},{wrd_Down,0,137},{wrd_North,0,138},
	    {wrd_South,0,139},{wrd_East,0,112},{0,0,0}},
    },
    { // 132
	"",
	"You are in a little maze of twisty passages, all different.",
	(const struct MapDestEntry[]) {
	    {wrd_NW,0,107},{wrd_Up,0,131},{wrd_North,0,133},{wrd_South,0,134},
	    {wrd_West,0,135},{wrd_SW,0,136},{wrd_NE,0,137},{wrd_East,0,138},
	    {wrd_Down,0,139},{wrd_SE,0,112},{0,0,0}},
    },
    { // 133
	"",
	"You are in a twisting maze of little passages, all different.",
	(const struct MapDestEntry[]) {
	    {wrd_Up,0,107},{wrd_Down,0,131},{wrd_West,0,132},{wrd_NE,0,134},
	    {wrd_SW,0,135},{wrd_East,0,136},{wrd_North,0,137},{wrd_NW,0,138},
	    {wrd_SE,0,139},{wrd_South,0,112},{0,0,0}},
    },
    { // 134
	"",
	"You are in a twisting little maze of passages, all different.",
	(const struct MapDestEntry[]) {
	    {wrd_NE,0,107},{wrd_North,0,131},{wrd_NW,0,132},{wrd_SE,0,133},
	    {wrd_East,0,135},{wrd_Down,0,136},{wrd_South,0,137},{wrd_Up,0,138},
	    {wrd_West,0,139},{wrd_SW,0,112},{0,0,0}},
    },
    { // 135
	"",
	"You are in a twisty little maze of passages, all different.",
	(const struct MapDestEntry[]) {
	    {wrd_North,0,107},{wrd_SE,0,131},{wrd_Down,0,132},{wrd_South,0,133},
	    {wrd_East,0,134},{wrd_West,0,136},{wrd_SW,0,137},{wrd_NE,0,138},
	    {wrd_NW,0,139},{wrd_Up,0,112},{0,0,0}},
    },
    { // 136
	"",
	"You are in a twisty maze of little passages, all different.",
	(const struct MapDestEntry[]) {
	    {wrd_East,0,107},{wrd_West,0,131},{wrd_Up,0,132},{wrd_SW,0,133},
	    {wrd_Down,0,134},{wrd_South,0,135},{wrd_NW,0,137},{wrd_SE,0,138},
	    {wrd_NE,0,139},{wrd_North,0,112},{0,0,0}},
    },
    { // 137
	"",
	"You are in a little twisty maze of passages, all different.",
	(const struct MapDestEntry[]) {
	    {wrd_SE,0,107},{wrd_NE,0,131},{wrd_South,0,132},{wrd_Down,0,133},
	    {wrd_Up,0,134},{wrd_NW,0,135},{wrd_North,0,136},{wrd_SW,0,138},
	    {wrd_East,0,139},{wrd_West,0,112},{0,0,0}},
    },
    { // 138
	"",
	"You are in a maze of little twisting passages, all different.",
	(const struct MapDestEntry[]) {
	    {wrd_Down,0,107},{wrd_East,0,131},{wrd_NE,0,132},{wrd_Up,0,133},
	    {wrd_West,0,134},{wrd_North,0,135},{wrd_South,0,136},{wrd_SE,0,137},
	    {wrd_SW,0,139},{wrd_NW,0,112},{0,0,0}},
    },
    { // 139
	"",
	"You are in a maze of little twisty passages, all different.",
	(const struct MapDestEntry[]) {
	    {wrd_SW,0,107},{wrd_NW,0,131},{wrd_East,0,132},{wrd_West,0,133},
	    {wrd_North,0,134},{wrd_Down,0,135},{wrd_SE,0,136},{wrd_Up,0,137},
	    {wrd_South,0,138},{wrd_NE,0,112},{0,0,0}},
    },
    { // 140
	"",
	"Dead end",
	(const struct MapDestEntry[]) {
	    {wrd_North,0,112},{wrd_Out,0,112},{0,0,0}},
    },
};
