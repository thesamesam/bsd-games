// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "hdr.h"

// Location long descriptions
const char* const ltext [LOCSIZ] = {
	// 0
	"",
	// 1
	"You are standing at the end of a road before a small brick building.\n"
	"Around you is a forest.  A small stream flows out of the building and\n"
	"down a gully.",
	// 2
	"You have walked up a hill, still in the forest.  The road slopes back\n"
	"down the other side of the hill.  There is a building in the distance.",
	// 3
	"You are inside a building, a well house for a large spring.",
	// 4
	"You are in a valley in the forest beside a stream tumbling along a\n"
	"rocky bed.",
	// 5
	"You are in open forest, with a deep valley to one side.",
	// 6
	"You are in open forest near both a valley and a road.",
	// 7
	"At your feet all the water of the stream splashes into a 2-inch slit\n"
	"in the rock.  Downstream the streambed is bare rock.",
	// 8
	"You are in a 20-foot depression floored with bare dirt.  Set into the\n"
	"dirt is a strong steel grate mounted in concrete.  A dry streambed\n"
	"leads into the depression.",
	// 9
	"You are in a small chamber beneath a 3x3 steel grate to the surface.\n"
	"A low crawl over cobbles leads inward to the west.",
	// 10
	"You are crawling over cobbles in a low passage.  There is a dim light\n"
	"at the east end of the passage.",
	// 11
	"You are in a debris room filled with stuff washed in from the surface.\n"
	"A low wide passage with cobbles becomes plugged with mud and debris\n"
	"here, but an awkward canyon leads upward and west.  A note on the wall\n"
	"says \"Magic word XYZZY\".",
	// 12
	"You are in an awkward sloping east/west canyon.",
	// 13
	"You are in a splendid chamber thirty feet high.  The walls are frozen\n"
	"rivers of orange stone.  An awkward canyon and a good passage exit\n"
	"from east and west sides of the chamber.",
	// 14
	"At your feet is a small pit breathing traces of white mist.  An east\n"
	"passage ends here except for a small crack leading on.",
	// 15
	"You are at one end of a vast hall stretching forward out of sight to\n"
	"the west.  There are openings to either side.  Nearby, a wide stone\n"
	"staircase leads downward.  The hall is filled with wisps of white mist\n"
	"swaying to and fro almost as if alive.  A cold wind blows up the\n"
	"staircase.  There is a passage at the top of a dome behind you.",
	// 16
	"The crack is far too small for you to follow.",
	// 17
	"You are on the east bank of a fissure slicing clear across the hall.\n"
	"The mist is quite thick here, and the fissure is too wide to jump.",
	// 18
	"This is a low room with a crude note on the wall.  The note says,\n"
	"\"You won't get it up the steps\".",
	// 19
	"You are in the Hall of the Mountain King, with passages off in all\n"
	"directions.",
	// 20
	"You are at the bottom of the pit with a broken neck.",
	// 21
	"You didn't make it.",
	// 22
	"The dome is unclimbable.",
	// 23
	"You are at the west end of the Twopit Room.  There is a large hole in\n"
	"the wall above the pit at this end of the room.",
	// 24
	"You are at the bottom of the eastern pit in the Twopit Room.  There is\n"
	"a small pool of oil in one corner of the pit.",
	// 25
	"You are at the bottom of the western pit in the Twopit Room.  There is\n"
	"a large hole in the wall about 25 feet above you.",
	// 26
	"You clamber up the plant and scurry through the hole at the top.",
	// 27
	"You are on the west side of the fissure in the Hall of Mists.",
	// 28
	"You are in a low N/S passage at a hole in the floor.  The hole goes\n"
	"down to an E/W passage.",
	// 29
	"You are in the south side chamber.",
	// 30
	"You are in the west side chamber of the Hall of the Mountain King.\n"
	"A passage continues west and up here.",
	// 31
	"",
	// 32
	"You can't get by the snake.",
	// 33
	"You are in a large room, with a passage to the south, a passage to the\n"
	"west, and a wall of broken rock to the east.  There is a large \"Y2\" on\n"
	"a rock in the room's center.",
	// 34
	"You are in a jumble of rock, with cracks everywhere.",
	// 35
	"You're at a low window overlooking a huge pit, which extends up out of\n"
	"sight.  A floor is indistinctly visible over 50 feet below.  Traces of\n"
	"white mist cover the floor of the pit, becoming thicker to the right.\n"
	"Marks in the dust around the window would seem to indicate that\n"
	"someone has been here recently.  Directly across the pit from you and\n"
	"25 feet away there is a similar window looking into a lighted room.  A\n"
	"shadowy figure can be seen there peering back at you.",
	// 36
	"You are in a dirty broken passage.  To the east is a crawl.  To the\n"
	"west is a large passage.  Above you is a hole to another passage.",
	// 37
	"You are on the brink of a small clean climbable pit.  A crawl leads\n"
	"west.",
	// 38
	"You are in the bottom of a small pit with a little stream, which\n"
	"enters and exits through tiny slits.",
	// 39
	"You are in a large room full of dusty rocks.  There is a big hole in\n"
	"the floor.  There are cracks everywhere, and a passage leading east.",
	// 40
	"You have crawled through a very low wide passage parallel to and north\n"
	"of the Hall of Mists.",
	// 41
	"You are at the west end of Hall of Mists.  A low wide crawl continues\n"
	"west and another goes north.  To the south is a little passage 6 feet\n"
	"off the floor.",
	// 42
	"You are in a maze of twisty little passages, all alike.",
	// 43
	"You are in a maze of twisty little passages, all alike.",
	// 44
	"You are in a maze of twisty little passages, all alike.",
	// 45
	"You are in a maze of twisty little passages, all alike.",
	// 46
	"Dead end",
	// 47
	"Dead end",
	// 48
	"Dead end",
	// 49
	"You are in a maze of twisty little passages, all alike.",
	// 50
	"You are in a maze of twisty little passages, all alike.",
	// 51
	"You are in a maze of twisty little passages, all alike.",
	// 52
	"You are in a maze of twisty little passages, all alike.",
	// 53
	"You are in a maze of twisty little passages, all alike.",
	// 54
	"Dead end",
	// 55
	"You are in a maze of twisty little passages, all alike.",
	// 56
	"Dead end",
	// 57
	"You are on the brink of a thirty foot pit with a massive orange column\n"
	"down one wall.  You could climb down here but you could not get back\n"
	"up.  The maze continues at this level.",
	// 58
	"Dead end",
	// 59
	"You have crawled through a very low wide passage parallel to and north\n"
	"of the Hall of Mists.",
	// 60
	"You are at the east end of a very long hall apparently without side\n"
	"chambers.  To the east a low wide crawl slants up.  To the north a\n"
	"round two foot hole slants down.",
	// 61
	"You are at the west end of a very long featureless hall.  The hall\n"
	"joins up with a narrow north/south passage.",
	// 62
	"You are at a crossover of a high N/S passage and a low E/W one.",
	// 63
	"Dead end",
	// 64
	"You are at a complex junction.  A low hands and knees passage from the\n"
	"north joins a higher crawl from the east to make a walking passage\n"
	"going west.  There is also a large room above.  The air is damp here.",
	// 65
	"You are in Bedquilt, a long east/west passage with holes everywhere.\n"
	"To explore at random select north, south, up, or down.",
	// 66
	"You are in a room whose walls resemble Swiss cheese.  Obvious passages\n"
	"go west, east, NE, and NW.  Part of the room is occupied by a large\n"
	"bedrock block.",
	// 67
	"You are at the east end of the Twopit Room.  The floor here is\n"
	"littered with thin rock slabs, which make it easy to descend the pits.\n"
	"There is a path here bypassing the pits to connect passages from east\n"
	"and west.  There are holes all over, but the only big one is on the\n"
	"wall directly over the west pit where you can't get to it.",
	// 68
	"You are in a large low circular chamber whose floor is an immense slab\n"
	"fallen from the ceiling (Slab Room).  East and west there once were\n"
	"large passages, but they are now filled with boulders.  Low small\n"
	"passages go north and south, and the south one quickly bends west\n"
	"around the boulders.",
	// 69
	"You are in a secret N/S canyon above a large room.",
	// 70
	"You are in a secret N/S canyon above a sizable passage.",
	// 71
	"You are in a secret canyon at a junction of three canyons, bearing\n"
	"north, south, and SE.  The north one is as tall as the other two\n"
	"combined.",
	// 72
	"You are in a large low room.  Crawls lead north, SE, and SW.",
	// 73
	"Dead end crawl.",
	// 74
	"You are in a secret canyon which here runs E/W.  It crosses over a\n"
	"very tight canyon 15 feet below.  If you go down you may not be able\n"
	"to get back up.",
	// 75
	"You are at a wide place in a very tight N/S canyon.",
	// 76
	"The canyon here becomes too tight to go further south.",
	// 77
	"You are in a tall E/W canyon.  A low tight crawl goes 3 feet north and\n"
	"seems to open up.",
	// 78
	"The canyon runs into a mass of boulders -- dead end.",
	// 79
	"The stream flows out through a pair of 1 foot diameter sewer pipes.\n"
	"It would be advisable to use the exit.",
	// 80
	"You are in a maze of twisty little passages, all alike.",
	// 81
	"Dead end",
	// 82
	"Dead end",
	// 83
	"You are in a maze of twisty little passages, all alike.",
	// 84
	"You are in a maze of twisty little passages, all alike.",
	// 85
	"Dead end",
	// 86
	"Dead end",
	// 87
	"You are in a maze of twisty little passages, all alike.",
	// 88
	"You are in a long, narrow corridor stretching out of sight to the\n"
	"west.  At the eastern end is a hole through which you can see a\n"
	"profusion of leaves.",
	// 89
	"There is nothing here to climb.  Use \"up\" or \"out\" to leave the pit.",
	// 90
	"You have climbed up the plant and out of the pit.",
	// 91
	"You are at the top of a steep incline above a large room.  You could\n"
	"climb down here, but you would not be able to climb up.  There is a\n"
	"passage leading back to the north.",
	// 92
	"You are in the Giant Room.  The ceiling here is too high up for your\n"
	"lamp to show it.  Cavernous passages lead east, north, and south.  On\n"
	"the west wall is scrawled the inscription, \"Fee fie foe foo\" [sic].",
	// 93
	"The passage here is blocked by a recent cave-in.",
	// 94
	"You are at one end of an immense north/south passage.",
	// 95
	"You are in a magnificent cavern with a rushing stream, which cascades\n"
	"over a sparkling waterfall into a roaring whirlpool which disappears\n"
	"through a hole in the floor.  Passages exit to the south and west.",
	// 96
	"You are in the Soft Room.  The walls are covered with heavy curtains,\n"
	"the floor with a thick pile carpet.  Moss covers the ceiling.",
	// 97
	"This is the Oriental Room.  Ancient oriental cave drawings cover the\n"
	"walls.  A gently sloping passage leads upward to the north, another\n"
	"passage leads SE, and a hands and knees crawl leads west.",
	// 98
	"You are following a wide path around the outer edge of a large cavern.\n"
	"Far below, through a heavy white mist, strange splashing noises can be\n"
	"heard.  The mist rises up through a fissure in the ceiling.  The path\n"
	"exits to the south and west.",
	// 99
	"You are in an alcove.  A small NW path seems to widen after a short\n"
	"distance.  An extremely tight tunnel leads east.  It looks like a very\n"
	"tight squeeze.  An eerie light can be seen at the other end.",
	// 100
	"You're in a small chamber lit by an eerie green light.  An extremely\n"
	"narrow tunnel exits to the west.  A dark corridor leads NE.",
	// 101
	"You're in the Dark-Room.  A corridor leading south is the only exit.",
	// 102
	"You are in an arched hall.  A coral passage once continued up and east\n"
	"from here, but is now blocked by debris.  The air smells of sea water.",
	// 103
	"You're in a large room carved out of sedimentary rock.  The floor and\n"
	"walls are littered with bits of shells embedded in the stone.  A\n"
	"shallow passage proceeds downward, and a somewhat steeper one leads\n"
	"up.  A low hands and knees passage enters from the south.",
	// 104
	"You are in a long sloping corridor with ragged sharp walls.",
	// 105
	"You are in a cul-de-sac about eight feet across.",
	// 106
	"You are in an anteroom leading to a large passage to the east.  Small\n"
	"passages go west and up.  The remnants of recent digging are evident.\n"
	"A sign in midair here says \"Cave under construction beyond this point.\n"
	"Proceed at own risk.  [Witt construction company]\"",
	// 107
	"You are in a maze of twisty little passages, all different.",
	// 108
	"You are at Witt's End.  Passages lead off in *all* directions.",
	// 109
	"You are in a north/south canyon about 25 feet across.  The floor is\n"
	"covered by white mist seeping in from the north.  The walls extend\n"
	"upward for well over 100 feet.  Suspended from some unseen point far\n"
	"above you, an enormous two-sided mirror is hanging parallel to and\n"
	"midway between the canyon walls.  (The mirror is obviously provided\n"
	"for the use of the dwarves, who as you know, are extremely vain.)  A\n"
	"small window can be seen in either wall, some fifty feet up.",
	// 110
	"You're at a low window overlooking a huge pit, which extends up out of\n"
	"sight.  A floor is indistinctly visible over 50 feet below.  Traces of\n"
	"white mist cover the floor of the pit, becoming thicker to the left.\n"
	"Marks in the dust around the window would seem to indicate that\n"
	"someone has been here recently.  Directly across the pit from you and\n"
	"25 feet away there is a similar window looking into a lighted room.  A\n"
	"shadowy figure can be seen there peering back at you.",
	// 111
	"A large stalactite extends from the roof and almost reaches the floor\n"
	"below.  You could climb down it, and jump from it to the floor, but\n"
	"having done so you would be unable to reach it to climb back up.",
	// 112
	"You are in a little maze of twisting passages, all different.",
	// 113
	"You are at the edge of a large underground reservoir.  An opaque cloud\n"
	"of white mist fills the room and rises rapidly upward.  The lake is\n"
	"fed by a stream, which tumbles out of a hole in the wall about 10 feet\n"
	"overhead and splashes noisily into the water somewhere within the\n"
	"mist.  The only passage goes back toward the south.",
	// 114
	"Dead end",
	// 115
	"You are at the northeast end of an immense room, even larger than the\n"
	"Giant Room.  It appears to be a repository for the \"Adventure\"\n"
	"program.  Massive torches far overhead bathe the room with smoky\n"
	"yellow light.  Scattered about you can be seen a pile of bottles (all\n"
	"of them empty), a nursery of young beanstalks murmuring quietly, a bed\n"
	"of oysters, a bundle of black rods with rusty stars on their ends, and\n"
	"a collection of brass lanterns.  Off to one side a great many dwarves\n"
	"are sleeping on the floor, snoring loudly.  A sign nearby reads: \"Do\n"
	"not disturb the dwarves!\"  An immense mirror is hanging against one\n"
	"wall, and stretches to the other end of the room, where various other\n"
	"sundry objects can be glimpsed dimly in the distance.",
	// 116
	"You are at the southwest end of the Repository.  To one side is a pit\n"
	"full of fierce green snakes.  On the other side is a row of small\n"
	"wicker cages, each of which contains a little sulking bird.  In one\n"
	"corner is a bundle of black rods with rusty marks on their ends.  A\n"
	"large number of velvet pillows are scattered about on the floor.  A\n"
	"vast mirror stretches off to the northeast.  At your feet is a large\n"
	"steel grate, next to which is a sign which reads, \"Treasure Vault.\n"
	"Keys in Main Office.\"",
	// 117
	"You are on one side of a large, deep chasm.  A heavy white mist rising\n"
	"up from below obscures all view of the far side.  A SW path leads away\n"
	"from the chasm into a winding corridor.",
	// 118
	"You are in a long winding corridor sloping out of sight in both\n"
	"directions.",
	// 119
	"You are in a secret canyon which exits to the north and east.",
	// 120
	"You are in a secret canyon which exits to the north and east.",
	// 121
	"You are in a secret canyon which exits to the north and east.",
	// 122
	"You are on the far side of the chasm.  A NE path leads away from the\n"
	"chasm on this side.",
	// 123
	"You're in a long east/west corridor.  A faint rumbling noise can be\n"
	"heard in the distance.",
	// 124
	"The path forks here.  The left fork leads northeast.  A dull rumbling\n"
	"seems to get louder in that direction.  The right fork leads southeast\n"
	"down a gentle slope.  The main corridor enters from the west.",
	// 125
	"The walls are quite warm here.  From the north can be heard a steady\n"
	"roar, so loud that the entire cave seems to be trembling.  Another\n"
	"passage leads south, and a low crawl goes east.",
	// 126
	"You are on the edge of a breath-taking view.  Far below you is an\n"
	"active volcano, from which great gouts of molten lava come surging\n"
	"out, cascading back down into the depths.  The glowing rock fills the\n"
	"farthest reaches of the cavern with a blood-red glare, giving every-\n"
	"thing an eerie, macabre appearance.  The air is filled with flickering\n"
	"sparks of ash and a heavy smell of brimstone.  The walls are hot to\n"
	"the touch, and the thundering of the volcano drowns out all other\n"
	"sounds.  Embedded in the jagged roof far overhead are myriad twisted\n"
	"formations composed of pure white alabaster, which scatter the murky\n"
	"light into sinister apparitions upon the walls.  To one side is a deep\n"
	"gorge, filled with a bizarre chaos of tortured rock which seems to\n"
	"have been crafted by the devil himself.  An immense river of fire\n"
	"crashes out from the depths of the volcano, burns its way through the\n"
	"gorge, and plummets into a bottomless pit far off to your left.  To\n"
	"the right, an immense geyser of blistering steam erupts continuously\n"
	"from a barren island in the center of a sulfurous lake, which bubbles\n"
	"ominously.  The far right wall is aflame with an incandescence of its\n"
	"own, which lends an additional infernal splendor to the already\n"
	"hellish scene.  A dark, foreboding passage exits to the south.",
	// 127
	"You are in a small chamber filled with large boulders.  The walls are\n"
	"very warm, causing the air in the room to be almost stifling from the\n"
	"heat.  The only exit is a crawl heading west, through which is coming\n"
	"a low rumbling.",
	// 128
	"You are walking along a gently sloping north/south passage lined with\n"
	"oddly shaped limestone formations.",
	// 129
	"You are standing at the entrance to a large, barren room.  A sign\n"
	"posted above the entrance reads:  \"Caution!  Bear in room!\"",
	// 130
	"You are inside a barren room.  The center of the room is completely\n"
	"empty except for some dust.  Marks in the dust lead away toward the\n"
	"far end of the room.  The only exit is the way you came in.",
	// 131
	"You are in a maze of twisting little passages, all different.",
	// 132
	"You are in a little maze of twisty passages, all different.",
	// 133
	"You are in a twisting maze of little passages, all different.",
	// 134
	"You are in a twisting little maze of passages, all different.",
	// 135
	"You are in a twisty little maze of passages, all different.",
	// 136
	"You are in a twisty maze of little passages, all different.",
	// 137
	"You are in a little twisty maze of passages, all different.",
	// 138
	"You are in a maze of little twisting passages, all different.",
	// 139
	"You are in a maze of little twisty passages, all different.",
	// 140
	"Dead end\n"
};

// Location short descriptions
const char* const stext [LOCSIZ] = {
	// 0
	"",
	// 1
	"You're at end of road again.",
	// 2
	"You're at hill in road.",
	// 3
	"You're inside building.",
	// 4
	"You're in valley.",
	// 5
	"You're in forest.",
	// 6
	"You're in forest.",
	// 7
	"You're at slit in streambed.",
	// 8
	"You're outside grate.",
	// 9
	"You're below the grate.",
	// 10
	"You're in Cobble Crawl.",
	// 11
	"You're in Debris Room.",
	// 12
	"",
	// 13
	"You're in Bird Chamber.",
	// 14
	"You're at top of small pit.",
	// 15
	"You're in Hall of Mists.",
	// 16
	"",
	// 17
	"You're on east bank of fissure.",
	// 18
	"You're in Nugget of Gold Room.",
	// 19
	"You're in Hall of Mt King.",
	// 20
	"",
	// 21
	"",
	// 22
	"",
	// 23
	"You're at west end of Twopit Room.",
	// 24
	"You're in east pit.",
	// 25
	"You're in west pit.",
	// 26
	"",
	// 27
	"",
	// 28
	"",
	// 29
	"",
	// 30
	"",
	// 31
	"",
	// 32
	"",
	// 33
	"You're at \"Y2\".",
	// 34
	"",
	// 35
	"You're at window on pit.",
	// 36
	"You're in dirty passage.",
	// 37
	"",
	// 38
	"",
	// 39
	"You're in Dusty Rock room.",
	// 40
	"",
	// 41
	"You're at west end of Hall of Mists.",
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
	"",
	// 51
	"",
	// 52
	"",
	// 53
	"",
	// 54
	"",
	// 55
	"",
	// 56
	"",
	// 57
	"You're at brink of pit.",
	// 58
	"",
	// 59
	"",
	// 60
	"You're at east end of Long Hall.",
	// 61
	"You're at west end of Long Hall.",
	// 62
	"",
	// 63
	"",
	// 64
	"You're at Complex Junction.",
	// 65
	"",
	// 66
	"You're in Swiss Cheese room.",
	// 67
	"You're at east end of Twopit Room.",
	// 68
	"You're in Slab Room.",
	// 69
	"",
	// 70
	"",
	// 71
	"You're at junction of three secret canyons.",
	// 72
	"",
	// 73
	"",
	// 74
	"You're in secret E/W canyon above tight canyon.",
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
	"You're in narrow corridor.",
	// 89
	"",
	// 90
	"",
	// 91
	"You're at steep incline above large room.",
	// 92
	"You're in Giant Room.",
	// 93
	"",
	// 94
	"",
	// 95
	"You're in cavern with waterfall.",
	// 96
	"You're in Soft Room.",
	// 97
	"You're in Oriental Room.",
	// 98
	"You're in Misty Cavern.",
	// 99
	"You're in Alcove.",
	// 100
	"You're in Plover Room.",
	// 101
	"You're in Dark-Room.",
	// 102
	"You're in Arched Hall.",
	// 103
	"You're in Shell Room.",
	// 104
	"",
	// 105
	"",
	// 106
	"You're in Anteroom.",
	// 107
	"",
	// 108
	"You're at Witt's End.",
	// 109
	"You're in Mirror Canyon.",
	// 110
	"You're at window on pit.",
	// 111
	"You're at top of stalactite.",
	// 112
	"",
	// 113
	"You're at Reservoir.",
	// 114
	"",
	// 115
	"You're at NE end.",
	// 116
	"You're at SW end.",
	// 117
	"You're on SW side of chasm.",
	// 118
	"You're in sloping corridor.",
	// 119
	"",
	// 120
	"",
	// 121
	"",
	// 122
	"You're on NE side of chasm.",
	// 123
	"You're in corridor.",
	// 124
	"You're at fork in path.",
	// 125
	"You're at junction with warm walls.",
	// 126
	"You're at Breath-taking View.",
	// 127
	"You're in Chamber of Boulders.",
	// 128
	"You're in limestone passage.",
	// 129
	"You're in front of barren room.",
	// 130
	"You're in Barren Room.",
	// 131
	"",
	// 132
	"",
	// 133
	"",
	// 134
	"",
	// 135
	"",
	// 136
	"",
	// 137
	"",
	// 138
	"",
	// 139
	"",
	// 140
	""
};

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

// verb-destination mapping for each location
const struct travlist* const travel[LOCSIZ] = {
	// 0
	(const struct travlist[]) {{0,0,0}},
	// 1
	(const struct travlist[]) {{0,2,2},{0,2,44},{0,2,29},{0,3,3},{0,3,12},{0,3,19},{0,3,43},{0,4,5},{0,4,13},{0,4,14},{0,4,46},{0,4,30},{0,5,6},{0,5,45},{0,5,43},{0,8,63},{0,0,0}},
	// 2
	(const struct travlist[]) {{0,1,2},{0,1,12},{0,1,7},{0,1,43},{0,1,45},{0,1,30},{0,5,6},{0,5,45},{0,5,46},{0,0,0}},
	// 3
	(const struct travlist[]) {{0,1,3},{0,1,11},{0,1,32},{0,1,44},{0,11,62},{0,33,65},{0,79,5},{0,79,14},{0,0,0}},
	// 4
	(const struct travlist[]) {{0,1,4},{0,1,12},{0,1,45},{0,5,6},{0,5,43},{0,5,44},{0,5,29},{0,7,5},{0,7,46},{0,7,30},{0,8,63},{0,0,0}},
	// 5
	(const struct travlist[]) {{0,4,9},{0,4,43},{0,4,30},{50,5,6},{50,5,7},{50,5,45},{0,6,6},{0,5,44},{0,5,46},{0,0,0}},
	// 6
	(const struct travlist[]) {{0,1,2},{0,1,45},{0,4,9},{0,4,43},{0,4,44},{0,4,30},{0,5,6},{0,5,46},{0,0,0}},
	// 7
	(const struct travlist[]) {{0,1,12},{0,4,4},{0,4,45},{0,5,6},{0,5,43},{0,5,44},{0,8,5},{0,8,15},{0,8,16},{0,8,46},{0,595,60},{0,595,14},{0,595,30},{0,0,0}},
	// 8
	(const struct travlist[]) {{0,5,6},{0,5,43},{0,5,44},{0,5,46},{0,1,12},{0,7,4},{0,7,13},{0,7,45},{303,9,3},{303,9,19},{303,9,30},{0,593,3},{0,0,0}},
	// 9
	(const struct travlist[]) {{303,8,11},{303,8,29},{0,593,11},{0,10,17},{0,10,18},{0,10,19},{0,10,44},{0,14,31},{0,11,51},{0,0,0}},
	// 10
	(const struct travlist[]) {{0,9,11},{0,9,20},{0,9,21},{0,9,43},{0,11,19},{0,11,22},{0,11,44},{0,11,51},{0,14,31},{0,0,0}},
	// 11
	(const struct travlist[]) {{303,8,63},{0,9,64},{0,10,17},{0,10,18},{0,10,23},{0,10,24},{0,10,43},{0,12,25},{0,12,19},{0,12,29},{0,12,44},{0,3,62},{0,14,31},{0,0,0}},
	// 12
	(const struct travlist[]) {{303,8,63},{0,9,64},{0,11,30},{0,11,43},{0,11,51},{0,13,19},{0,13,29},{0,13,44},{0,14,31},{0,0,0}},
	// 13
	(const struct travlist[]) {{303,8,63},{0,9,64},{0,11,51},{0,12,25},{0,12,43},{0,14,23},{0,14,31},{0,14,44},{0,0,0}},
	// 14
	(const struct travlist[]) {{303,8,63},{0,9,64},{0,11,51},{0,13,23},{0,13,43},{150,20,30},{150,20,31},{150,20,34},{0,15,30},{0,16,33},{0,16,44},{0,0,0}},
	// 15
	(const struct travlist[]) {{0,18,36},{0,18,46},{0,17,7},{0,17,38},{0,17,44},{0,19,10},{0,19,30},{0,19,45},{150,22,29},{150,22,31},{150,22,34},{150,22,35},{150,22,23},{150,22,43},{0,14,29},{0,34,55},{0,0,0}},
	// 16
	(const struct travlist[]) {{0,14,1},{0,0,0}},
	// 17
	(const struct travlist[]) {{0,15,38},{0,15,43},{312,596,39},{412,21,7},{412,597,41},{412,597,42},{412,597,44},{412,597,69},{0,27,41},{0,0,0}},
	// 18
	(const struct travlist[]) {{0,15,38},{0,15,11},{0,15,45},{0,0,0}},
	// 19
	(const struct travlist[]) {{0,15,10},{0,15,29},{0,15,43},{311,28,45},{311,28,36},{311,29,46},{311,29,37},{311,30,44},{311,30,7},{0,32,45},{35,74,49},{211,32,49},{0,74,66},{0,0,0}},
	// 20
	(const struct travlist[]) {{0,0,1},{0,0,0}},
	// 21
	(const struct travlist[]) {{0,0,1},{0,0,0}},
	// 22
	(const struct travlist[]) {{0,15,1},{0,0,0}},
	// 23
	(const struct travlist[]) {{0,67,43},{0,67,42},{0,68,44},{0,68,61},{0,25,30},{0,25,31},{0,648,52},{0,0,0}},
	// 24
	(const struct travlist[]) {{0,67,29},{0,67,11},{0,0,0}},
	// 25
	(const struct travlist[]) {{0,23,29},{0,23,11},{724,31,56},{0,26,56},{0,0,0}},
	// 26
	(const struct travlist[]) {{0,88,1},{0,0,0}},
	// 27
	(const struct travlist[]) {{312,596,39},{412,21,7},{412,597,41},{412,597,42},{412,597,43},{412,597,69},{0,17,41},{0,40,45},{0,41,44},{0,0,0}},
	// 28
	(const struct travlist[]) {{0,19,38},{0,19,11},{0,19,46},{0,33,45},{0,33,55},{0,36,30},{0,36,52},{0,0,0}},
	// 29
	(const struct travlist[]) {{0,19,38},{0,19,11},{0,19,45},{0,0,0}},
	// 30
	(const struct travlist[]) {{0,19,38},{0,19,11},{0,19,43},{0,62,44},{0,62,29},{0,0,0}},
	// 31
	(const struct travlist[]) {{524,89,1},{0,90,1},{0,0,0}},
	// 32
	(const struct travlist[]) {{0,19,1},{0,0,0}},
	// 33
	(const struct travlist[]) {{0,3,65},{0,28,46},{0,34,43},{0,34,53},{0,34,54},{0,35,44},{159,302,71},{0,100,71},{0,0,0}},
	// 34
	(const struct travlist[]) {{0,33,30},{0,33,55},{0,15,29},{0,0,0}},
	// 35
	(const struct travlist[]) {{0,33,43},{0,33,55},{0,20,39},{0,0,0}},
	// 36
	(const struct travlist[]) {{0,37,43},{0,37,17},{0,28,29},{0,28,52},{0,39,44},{0,65,70},{0,0,0}},
	// 37
	(const struct travlist[]) {{0,36,44},{0,36,17},{0,38,30},{0,38,31},{0,38,56},{0,0,0}},
	// 38
	(const struct travlist[]) {{0,37,56},{0,37,29},{0,37,11},{0,595,60},{0,595,14},{0,595,30},{0,595,4},{0,595,5},{0,0,0}},
	// 39
	(const struct travlist[]) {{0,36,43},{0,36,23},{0,64,30},{0,64,52},{0,64,58},{0,65,70},{0,0,0}},
	// 40
	(const struct travlist[]) {{0,41,1},{0,0,0}},
	// 41
	(const struct travlist[]) {{0,42,46},{0,42,29},{0,42,23},{0,42,56},{0,27,43},{0,59,45},{0,60,44},{0,60,17},{0,0,0}},
	// 42
	(const struct travlist[]) {{0,41,29},{0,42,45},{0,43,43},{0,45,46},{0,80,44},{0,0,0}},
	// 43
	(const struct travlist[]) {{0,42,44},{0,44,46},{0,45,43},{0,0,0}},
	// 44
	(const struct travlist[]) {{0,43,43},{0,48,30},{0,50,46},{0,82,45},{0,0,0}},
	// 45
	(const struct travlist[]) {{0,42,44},{0,43,45},{0,46,43},{0,47,46},{0,87,29},{0,87,30},{0,0,0}},
	// 46
	(const struct travlist[]) {{0,45,44},{0,45,11},{0,0,0}},
	// 47
	(const struct travlist[]) {{0,45,43},{0,45,11},{0,0,0}},
	// 48
	(const struct travlist[]) {{0,44,29},{0,44,11},{0,0,0}},
	// 49
	(const struct travlist[]) {{0,50,43},{0,51,44},{0,0,0}},
	// 50
	(const struct travlist[]) {{0,44,43},{0,49,44},{0,51,30},{0,52,46},{0,0,0}},
	// 51
	(const struct travlist[]) {{0,49,44},{0,50,29},{0,52,43},{0,53,46},{0,0,0}},
	// 52
	(const struct travlist[]) {{0,50,44},{0,51,43},{0,52,46},{0,53,29},{0,55,45},{0,86,30},{0,0,0}},
	// 53
	(const struct travlist[]) {{0,51,44},{0,52,45},{0,54,46},{0,0,0}},
	// 54
	(const struct travlist[]) {{0,53,44},{0,53,11},{0,0,0}},
	// 55
	(const struct travlist[]) {{0,52,44},{0,55,45},{0,56,30},{0,57,43},{0,0,0}},
	// 56
	(const struct travlist[]) {{0,55,29},{0,55,11},{0,0,0}},
	// 57
	(const struct travlist[]) {{0,13,30},{0,13,56},{0,55,44},{0,58,46},{0,83,45},{0,84,43},{0,0,0}},
	// 58
	(const struct travlist[]) {{0,57,43},{0,57,11},{0,0,0}},
	// 59
	(const struct travlist[]) {{0,27,1},{0,0,0}},
	// 60
	(const struct travlist[]) {{0,41,43},{0,41,29},{0,41,17},{0,61,44},{0,62,45},{0,62,30},{0,62,52},{0,0,0}},
	// 61
	(const struct travlist[]) {{0,60,43},{0,62,45},{100,107,46},{0,0,0}},
	// 62
	(const struct travlist[]) {{0,60,44},{0,63,45},{0,30,43},{0,61,46},{0,0,0}},
	// 63
	(const struct travlist[]) {{0,62,46},{0,62,11},{0,0,0}},
	// 64
	(const struct travlist[]) {{0,39,29},{0,39,56},{0,39,59},{0,65,44},{0,65,70},{0,103,45},{0,103,74},{0,106,43},{0,0,0}},
	// 65
	(const struct travlist[]) {{0,64,43},{0,66,44},{80,556,46},{0,68,61},{80,556,29},{50,70,29},{0,39,29},{60,556,45},{75,72,45},{0,71,45},{80,556,30},{0,106,30},{0,0,0}},
	// 66
	(const struct travlist[]) {{0,65,47},{0,67,44},{80,556,46},{0,77,25},{0,96,43},{50,556,50},{0,97,72},{0,0,0}},
	// 67
	(const struct travlist[]) {{0,66,43},{0,23,44},{0,23,42},{0,24,30},{0,24,31},{0,0,0}},
	// 68
	(const struct travlist[]) {{0,23,46},{0,69,29},{0,69,56},{0,65,45},{0,0,0}},
	// 69
	(const struct travlist[]) {{0,68,30},{0,68,61},{331,120,46},{0,119,46},{0,109,45},{0,113,75},{0,0,0}},
	// 70
	(const struct travlist[]) {{0,71,45},{0,65,30},{0,65,23},{0,111,46},{0,0,0}},
	// 71
	(const struct travlist[]) {{0,65,48},{0,70,46},{0,110,45},{0,0,0}},
	// 72
	(const struct travlist[]) {{0,65,70},{0,118,49},{0,73,45},{0,97,48},{0,97,72},{0,0,0}},
	// 73
	(const struct travlist[]) {{0,72,46},{0,72,17},{0,72,11},{0,0,0}},
	// 74
	(const struct travlist[]) {{0,19,43},{331,120,44},{0,121,44},{0,75,30},{0,0,0}},
	// 75
	(const struct travlist[]) {{0,76,46},{0,77,45},{0,0,0}},
	// 76
	(const struct travlist[]) {{0,75,45},{0,0,0}},
	// 77
	(const struct travlist[]) {{0,75,43},{0,78,44},{0,66,45},{0,66,17},{0,0,0}},
	// 78
	(const struct travlist[]) {{0,77,46},{0,0,0}},
	// 79
	(const struct travlist[]) {{0,3,1},{0,0,0}},
	// 80
	(const struct travlist[]) {{0,42,45},{0,80,44},{0,80,46},{0,81,43},{0,0,0}},
	// 81
	(const struct travlist[]) {{0,80,44},{0,80,11},{0,0,0}},
	// 82
	(const struct travlist[]) {{0,44,46},{0,44,11},{0,0,0}},
	// 83
	(const struct travlist[]) {{0,57,46},{0,84,43},{0,85,44},{0,0,0}},
	// 84
	(const struct travlist[]) {{0,57,45},{0,83,44},{0,114,50},{0,0,0}},
	// 85
	(const struct travlist[]) {{0,83,43},{0,83,11},{0,0,0}},
	// 86
	(const struct travlist[]) {{0,52,29},{0,52,11},{0,0,0}},
	// 87
	(const struct travlist[]) {{0,45,29},{0,45,30},{0,0,0}},
	// 88
	(const struct travlist[]) {{0,25,30},{0,25,56},{0,25,43},{0,20,39},{0,92,44},{0,92,27},{0,0,0}},
	// 89
	(const struct travlist[]) {{0,25,1},{0,0,0}},
	// 90
	(const struct travlist[]) {{0,23,1},{0,0,0}},
	// 91
	(const struct travlist[]) {{0,95,45},{0,95,73},{0,95,23},{0,72,30},{0,72,56},{0,0,0}},
	// 92
	(const struct travlist[]) {{0,88,46},{0,93,43},{0,94,45},{0,0,0}},
	// 93
	(const struct travlist[]) {{0,92,46},{0,92,27},{0,92,11},{0,0,0}},
	// 94
	(const struct travlist[]) {{0,92,46},{0,92,27},{0,92,23},{309,95,45},{309,95,3},{309,95,73},{0,611,45},{0,0,0}},
	// 95
	(const struct travlist[]) {{0,94,46},{0,94,11},{0,92,27},{0,91,44},{0,0,0}},
	// 96
	(const struct travlist[]) {{0,66,44},{0,66,11},{0,0,0}},
	// 97
	(const struct travlist[]) {{0,66,48},{0,72,44},{0,72,17},{0,98,29},{0,98,45},{0,98,73},{0,0,0}},
	// 98
	(const struct travlist[]) {{0,97,46},{0,97,72},{0,99,44},{0,0,0}},
	// 99
	(const struct travlist[]) {{0,98,50},{0,98,73},{0,301,43},{0,301,23},{0,100,43},{0,0,0}},
	// 100
	(const struct travlist[]) {{0,301,44},{0,301,23},{0,301,11},{0,99,44},{159,302,71},{0,33,71},{0,101,47},{0,101,22},{0,0,0}},
	// 101
	(const struct travlist[]) {{0,100,46},{0,100,71},{0,100,11},{0,0,0}},
	// 102
	(const struct travlist[]) {{0,103,30},{0,103,74},{0,103,11},{0,0,0}},
	// 103
	(const struct travlist[]) {{0,102,29},{0,102,38},{0,104,30},{114,618,46},{115,619,46},{0,64,46},{0,0,0}},
	// 104
	(const struct travlist[]) {{0,103,29},{0,103,74},{0,105,30},{0,0,0}},
	// 105
	(const struct travlist[]) {{0,104,29},{0,104,11},{0,103,74},{0,0,0}},
	// 106
	(const struct travlist[]) {{0,64,29},{0,65,44},{0,108,43},{0,0,0}},
	// 107
	(const struct travlist[]) {{0,131,46},{0,132,49},{0,133,47},{0,134,48},{0,135,29},{0,136,50},{0,137,43},{0,138,44},{0,139,45},{0,61,30},{0,0,0}},
	// 108
	(const struct travlist[]) {{95,556,43},{95,556,45},{95,556,46},{95,556,47},{95,556,48},{95,556,49},{95,556,50},{95,556,29},{95,556,30},{0,106,43},{0,626,44},{0,0,0}},
	// 109
	(const struct travlist[]) {{0,69,46},{0,113,45},{0,113,75},{0,0,0}},
	// 110
	(const struct travlist[]) {{0,71,44},{0,20,39},{0,0,0}},
	// 111
	(const struct travlist[]) {{0,70,45},{40,50,30},{40,50,39},{40,50,56},{50,53,30},{0,45,30},{0,0,0}},
	// 112
	(const struct travlist[]) {{0,131,49},{0,132,45},{0,133,43},{0,134,50},{0,135,48},{0,136,47},{0,137,44},{0,138,30},{0,139,29},{0,140,46},{0,0,0}},
	// 113
	(const struct travlist[]) {{0,109,46},{0,109,11},{0,109,109},{0,0,0}},
	// 114
	(const struct travlist[]) {{0,84,48},{0,0,0}},
	// 115
	(const struct travlist[]) {{0,116,49},{0,0,0}},
	// 116
	(const struct travlist[]) {{0,115,47},{0,593,30},{0,0,0}},
	// 117
	(const struct travlist[]) {{0,118,49},{233,660,41},{233,660,42},{233,660,69},{233,660,47},{332,661,41},{0,303,41},{332,21,39},{0,596,39},{0,0,0}},
	// 118
	(const struct travlist[]) {{0,72,30},{0,117,29},{0,0,0}},
	// 119
	(const struct travlist[]) {{0,69,45},{0,69,11},{0,653,43},{0,653,7},{0,0,0}},
	// 120
	(const struct travlist[]) {{0,69,45},{0,74,43},{0,0,0}},
	// 121
	(const struct travlist[]) {{0,74,43},{0,74,11},{0,653,45},{0,653,7},{0,0,0}},
	// 122
	(const struct travlist[]) {{0,123,47},{233,660,41},{233,660,42},{233,660,69},{233,660,49},{0,303,41},{0,596,39},{0,124,77},{0,126,28},{0,129,40},{0,0,0}},
	// 123
	(const struct travlist[]) {{0,122,44},{0,124,43},{0,124,77},{0,126,28},{0,129,40},{0,0,0}},
	// 124
	(const struct travlist[]) {{0,123,44},{0,125,47},{0,125,36},{0,128,48},{0,128,37},{0,128,30},{0,126,28},{0,129,40},{0,0,0}},
	// 125
	(const struct travlist[]) {{0,124,46},{0,124,77},{0,126,45},{0,126,28},{0,127,43},{0,127,17},{0,0,0}},
	// 126
	(const struct travlist[]) {{0,125,46},{0,125,23},{0,125,11},{0,124,77},{0,610,30},{0,610,39},{0,0,0}},
	// 127
	(const struct travlist[]) {{0,125,44},{0,125,11},{0,125,17},{0,124,77},{0,126,28},{0,0,0}},
	// 128
	(const struct travlist[]) {{0,124,45},{0,124,29},{0,124,77},{0,129,46},{0,129,30},{0,129,40},{0,126,28},{0,0,0}},
	// 129
	(const struct travlist[]) {{0,128,44},{0,128,29},{0,124,77},{0,130,43},{0,130,19},{0,130,40},{0,130,3},{0,126,28},{0,0,0}},
	// 130
	(const struct travlist[]) {{0,129,44},{0,129,11},{0,124,77},{0,126,28},{0,0,0}},
	// 131
	(const struct travlist[]) {{0,107,44},{0,132,48},{0,133,50},{0,134,49},{0,135,47},{0,136,29},{0,137,30},{0,138,45},{0,139,46},{0,112,43},{0,0,0}},
	// 132
	(const struct travlist[]) {{0,107,50},{0,131,29},{0,133,45},{0,134,46},{0,135,44},{0,136,49},{0,137,47},{0,138,43},{0,139,30},{0,112,48},{0,0,0}},
	// 133
	(const struct travlist[]) {{0,107,29},{0,131,30},{0,132,44},{0,134,47},{0,135,49},{0,136,43},{0,137,45},{0,138,50},{0,139,48},{0,112,46},{0,0,0}},
	// 134
	(const struct travlist[]) {{0,107,47},{0,131,45},{0,132,50},{0,133,48},{0,135,43},{0,136,30},{0,137,46},{0,138,29},{0,139,44},{0,112,49},{0,0,0}},
	// 135
	(const struct travlist[]) {{0,107,45},{0,131,48},{0,132,30},{0,133,46},{0,134,43},{0,136,44},{0,137,49},{0,138,47},{0,139,50},{0,112,29},{0,0,0}},
	// 136
	(const struct travlist[]) {{0,107,43},{0,131,44},{0,132,29},{0,133,49},{0,134,30},{0,135,46},{0,137,50},{0,138,48},{0,139,47},{0,112,45},{0,0,0}},
	// 137
	(const struct travlist[]) {{0,107,48},{0,131,47},{0,132,46},{0,133,30},{0,134,29},{0,135,50},{0,136,45},{0,138,49},{0,139,43},{0,112,44},{0,0,0}},
	// 138
	(const struct travlist[]) {{0,107,30},{0,131,43},{0,132,47},{0,133,29},{0,134,44},{0,135,45},{0,136,46},{0,137,48},{0,139,49},{0,112,50},{0,0,0}},
	// 139
	(const struct travlist[]) {{0,107,49},{0,131,50},{0,132,43},{0,133,44},{0,134,45},{0,135,30},{0,136,48},{0,137,29},{0,138,46},{0,112,47},{0,0,0}},
	// 140
	(const struct travlist[]) {{0,112,45},{0,112,11},{0,0,0}}
};
