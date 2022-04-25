/* GLOBAL.C:
 *
 *	Global variables.
 *
 *	Copyright (C) 1991, 1992, 1993 Brooke Paul
 *
 * $Id: global.c,v 6.31 2021/09/25 03:00:00 develop tnl, hp mp $
 *
 * $Log: global.c,v $
 *
10/04/2022
skillup functionality working
statup command working

28/03/2022
objectives command working now and is informed by the 
objectives master list within ../bin
skilltree command working and is informed by the 
skilltree master list within ../bin

20/01/2022: Objectives command
 09/01/2022: Skills command
 *
 * 04/12/2021: Added midas command
 *
 * Revision 6.31  2021/09/25 03:00:00  develop
 * added tnl, hp and mp commands to cmnd struct
 *
 * Revision 6.30  2001/07/29 22:11:02  develop
 * added shop_cost
 *
 * Revision 6.29  2001/07/25 02:55:04  develop
 * fixes for thieves dropping stolen items
 * fixes for gold dropping by pkills
 *
 * Revision 6.29  2001/07/23 04:04:09  develop
 * added alias to *st for dm_status
 *
 * Revision 6.28  2001/07/22 20:42:54  develop
 * added dm_stolen
 *
 * Revision 6.27  2001/07/18 01:43:24  develop
 * changed cost of stores
 *
 * Revision 6.26  2001/07/17 19:39:49  develop
 * re-arranged some help file numbers
 *
 * Revision 6.25  2001/07/17 19:28:44  develop
 * *** empty log message ***
 *
 * Revision 6.25  2001/07/15 05:42:50  develop
 * set_rom_owner setup_shop created
 *
 * Revision 6.24  2001/07/03 22:03:38  develop
 * testing intelligent monsters
 *
 * Revision 6.23  2001/07/03 22:00:22  develop
 * *** empty log message ***
 *
 * Revision 6.22  2001/07/03 21:52:28  develop
 * *** empty log message ***
 *
 * Revision 6.21  2001/06/30 01:58:34  develop
 * added ENMLIST
 *
 * Revision 6.20  2001/06/30 01:50:30  develop
 * removed experience loss when conjured creature kills a player
 *
 * Revision 6.19  2001/06/29 03:22:12  develop
 * added struct for default enemy lists
 *
 * Revision 6.18  2001/06/10 13:25:38  develop
 * set bonus table back to original
 *
 * Revision 6.17  2001/04/30 19:55:02  develop
 * renumbered a couple of help files
 *
 * Revision 6.16  2001/04/23 03:52:29  develop
 * added NOCREATE flag to toggle character creation
 *
 * Revision 6.15  2001/04/12 05:00:57  develop
 * STRMOR <-> SEQUAK (named wrong)
 *
 * Revision 6.14  2001/03/08 16:09:09  develop
 * *** empty log message ***
 *
 */
#include <stdio.h>

#include "../include/mordb.h"
#define MIGNORE
#include "mextern.h"

/*****************************************************************
*
* Configurable settings.  These are the defaults for setting that
* can be overridden from the mordor.cf file in the BINPATH
*
******************************************************************/

/* DM's name */
	char dmname[][20] = {
		"Morgoth", "Sorahl", "China", "Sandman", "Ugluk", "Darwin", "Tesseract", "Erech", 
	};
	char title[80]="Mordor MUD Server";
	char auth_questions_email[80]="";
	char questions_to_email[80]="";
	char register_questions_email[80]="";
	char account_exists[80]="You already have an account here.";
	char dm_pass[20]="";  /* No DM creation by default */


	char tx_mesg1[80]="The Ithil Express has docked in Parth.";
	char tx_mesg2[80]="The Ithil Express has docked in Celduin.";

	char sunrise[80]="The sun rises.";
	char sunset[80]="The sun disappears over the horizon.";
	char earth_trembles[80]="The earth trembles under your feet.";
	char heavy_fog[80]="A heavy fog blankets the earth.";
	char beautiful_day[80]="It's a beautiful day today.";
	char bright_sun[80]="The sun shines brightly across the land.";
	char glaring_sun[80]="The glaring sun beats down upon the inhabitants of the world.";
	char heat[80]="The heat today is unbearable.";
	char still[80]="The air is still and quiet.";
	char light_breeze[80]="A light breeze blows from the south.";
	char strong_wind[80]="A strong wind blows across the land.";
	char wind_gusts[80]="The wind gusts, blowing debris through the streets.";
	char gale_force[80]="Gale force winds blow in from the sea.";
	char clear_skies[80]="Clear, blue skies cover the land.";
	char light_clouds[80]="Light clouds appear over the mountains.";
	char thunderheads[80]="Thunderheads roll in from the east.";
	char light_rain[80]="A light rain falls quietly.";
	char heavy_rain[80]="A heavy rain begins to fall.";
	char sheets_rain[80]="Sheets of rain pour down from the skies.";
	char torrent_rain[80]="A torrent soaks the ground.";
	char no_moon[80]="The sky is dark as pitch.";
	char sliver_moon[80]="A sliver of silver can be seen in the night sky.";
	char half_moon[80]="Half a moon lights the evening skies.";
	char waxing_moon[80]="The night sky is lit by the waxing moon.";
	char full_moon[80]="The full moon shines across the land.";




	int		ANSILINE=0;
	int		AUTOSHUTDOWN=0;
	int		CHECKDOUBLE=0;
	int		EATNDRINK=0;
	int		GETHOSTBYNAME=0;
	int		HEAVEN=0;
	int		ISENGARD=0;
	int		LASTCOMMAND=0;
	int		PARANOID=0;
	int		RECORD_ALL=0;
	int		RFC1413=0;
	int		SECURE=0;
	int		SCHED=0;
	int		SUICIDE=0;
	int		NOCREATE=0;

	unsigned short	PORTNUM=4040;
	int		CRASHTRAP=0;
	int		GUILDEXP=0;
	int		SAVEONDROP=0;
	int		NOBULLYS=0;
	int		MSP=0;


    int    PROMPTCOLOR = AFC_MAGENTA;
    int    CREATURECOLOR = AFC_WHITE;
    int    BROADCASTCOLOR = AFC_YELLOW;
    int    MAINTEXTCOLOR = AFC_WHITE;
    int    PLAYERCOLOR = AFC_CYAN;
    int    ITEMCOLOR = AFC_YELLOW;
    int    EXITCOLOR = AFC_GREEN;
	int	   ROOMNAMECOLOR = AFC_CYAN;
    int    ECHOCOLOR = AFC_CYAN;
    int    WEATHERCOLOR = AFC_WHITE;


    int    ATTACKCOLOR = AFC_RED;
    int    MISSEDCOLOR = AFC_CYAN;
    int    TRAPCOLOR = AFC_MAGENTA;
    int    POISONCOLOR = AFC_RED;
    int    DISEASECOLOR = AFC_RED;
    int    BLINDCOLOR = AFC_RED;
	int	   SILENCECOLOR = AFC_YELLOW;	
	int	   ERRORCOLOR = AFC_RED;

	/* not used yet */
    int    LOGONOFFCOLOR = AFC_WHITE;
    int    TITLECOLOR = AFC_WHITE;
    int    TALKCOLOR = AFC_WHITE;
    int    FROSTCOLOR = AFC_YELLOW;

	int	start_room_num = 1;
	int	death_room_num = 50;
	int	jail_room_num = 6972;
	int 	pkill_in_battle = 1;
	int 	init_eq[9] = { 1505, 1502, 1503, 1504, 1, 3, 2, 5, 6 };
	int 	trophy_num = 1500;
	long	shop_cost = 1000000;

	time_t	dm_timeout = 0;
	time_t	ct_timeout = 1200;
	time_t	bld_timeout = 1200;
	time_t	ply_timeout = 300;

#ifdef TODO

	make these configurable flags in mordor.cf
	that then control whether that command is logged or not.

	*teleport
	*rm
	*reload
	*save
	*create
	*perm
	*invis
	*send
	*purge
	*ac
	*users
	*echo
	*flushrooms
	*shutdown
	*force
	*flushcrtobj
	*monster
	*status
	*add
	*set
	*log
	*spy
	*lock
	*finger
	*list
	*info
	*parameter
	*silence
	*broad
 	*replace
	*name
   	*append
   	*prepend
   	*gcast
	*group
	*notepad
 	*delete
	*oname
	*cname
	*active
	*dust
	*dmhelp
	*attack
	*enemy
	*charm
	*auth
	*possess
	*tell
	*memory
	*find
	*clear
	*talk
	*gamestat
	*advance
	*dmallocstats
	*war
#endif

/* end configurable settings */

char	g_buffer[4096];
int		Tablesize;
int		Cmdnum;
long	Time;
time_t	StartTime;
struct	lasttime	Shutdown;
struct	lasttime	Guildwar;
struct	lasttime	Weather[5];
int		Numlockedout;
lockout	*Lockout;

int	bHavePort;
short	Random_update_interval = 13;

plystruct Ply[PMAX];


//SMITHY RESTRUCTUED
//YOU NEED TO JUST REFERENCE THE VALUES YOU NEED
//AS YOU NEED THEM
//SINCE PLAYERS WILL HAVE VARIOUS TIERS
//FOR EACH ATTRIBUTE
//EG S tier for HP but D Tier for MP
//keep the same data for all classes' barehand dice, though
class_stats_struct class_stats[CLASS_COUNT] = {
	{  0,  0,  0,  0,  0,  0,  0},
	{ 24,  5,  8,  6,  2,  3,  1},	/* S tier */
	{ 22,  5,  7,  5,  2,  3,  1},	/* A tier */
	{ 18,  4,  6,  4,  2,  3,  1},	/* B tier */
	{ 19,  3,  5,  3,  2,  3,  1},	/* C tier */
	{ 14,  3,  5,  2,  2,  3,  1},	/* D tier */
	{ 14,  3,  4,  1,  2,  3,  1},	/* E tier */
	{ 18,  3,  6,  3,  2,  3,  0},	/* ranger */
	{ 18,  3,  5,  2,  2,  3,  1},	/* thief */
	{ 15,  3,  5,  4,  2,  3,  1},  /* bard */
	{ 17,  3,  6,  2,  2,  3,  1},  /* monk */
	{ 15,  4,  5,  3,  2,  3,  1},  /* druid */
	{ 15,  4,  4,  4,  2,  3,  1},	/* alchemist */
	{ 30, 30, 10, 10,  5,  5,  5},	/* builder */
	{ 30, 30, 10, 10,  5,  5,  5},	/* caretaker */
	{ 30, 30, 10, 10,  5,  5,  5}	/* DM */
};

int bonus[35] = { -4, -4, -4, -3, -3, -2, -2, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		  2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };

/* int bonus[35] = { -4, -4, -3,-3,-3,-2, -2, -1, 0, 0, 0, 0, 0, 1, 1, 1, 1,
                  2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 }; */


/* INT is a data type on some platforms so I had to change it */
/* but I did not want to mess up this nicely arranged array */
#define INT INTELLIGENCE

/*

The level_cycle has been severely tweaked, with this idea in mind:
P = Primary Requisite
S = Secondary Requisite
M = Important but not a requisite
N = Not a requisite
Z = Unneeded/unwanted stat

The core structure is this for all classes:
Level:  2   3   4   5   6   7   8   9   10  11
Stat:   S   M   N   Z   P   S   P   M   N   P

*/

short level_cycle[][10] = {
   /* 2    3    4    5    6    7    8    9    10   11 */
    { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0   },
    { STR, CON, INT, PTY, DEX, STR, DEX, CON, INT, DEX },  /* assassin */
    { CON, DEX, PTY, INT, STR, CON, STR, DEX, PTY, STR },  /* barbarian */
    { INT, DEX, CON, STR, PTY, INT, PTY, DEX, CON, PTY },  /* cleric */
    { DEX, CON, PTY, INT, STR, DEX, STR, CON, PTY, STR },  /* fighter */
    { PTY, DEX, CON, STR, INT, PTY, INT, DEX, CON, INT },  /* mage */
    { STR, CON, DEX, INT, PTY, STR, PTY, CON, DEX, PTY },  /* paladin */
    { STR, CON, PTY, INT, DEX, STR, DEX, CON, PTY, DEX },  /* ranger */
    { INT, STR, CON, PTY, DEX, INT, DEX, STR, CON, DEX },  /* thief */
    { DEX, STR, PTY, CON, INT, DEX, INT, STR, PTY, INT },  /* bard */
    { DEX, PTY, INT, STR, CON, DEX, CON, PTY, INT, CON },  /* monk */
    { PTY, CON, INT, STR, DEX, PTY, DEX, CON, INT, DEX },  /* druid */
    { INT, PTY, DEX, STR, CON, INT, CON, PTY, DEX, CON },  /* alchemist */
    { STR, DEX, INT, CON, PTY, STR, DEX, INT, CON, PTY },  /* builder */
    { STR, DEX, INT, CON, PTY, STR, DEX, INT, CON, PTY },  /* caretaker */
    { STR, DEX, INT, CON, PTY, STR, DEX, INT, CON, PTY }   /* DM */
};

#undef INT


//THIS HAS ALSO BEEN EDITED TO JUST BE TIER BASED
//YOU Need to be quite particular when you are referencing THAC0
//so that it doesn't just revert to what the game
//normally thinks the thaco is (based on the class number)
short thaco_list[][20] = { 
		{ 20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20 }, 
/*S TIER*/	{ 18,17,17,16,15,14,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },
/*A Tier*/	{ 19,18,17,16,15,14,14,13,13,12,12,11,10, 9, 8, 7, 6, 5, 4, 3 },
/*B Tier*/	{ 19,20,19,18,18,17,16,16,15,14,14,13,12,11,10, 9, 8, 7, 6, 5 },
/*C Tier*/	{ 20,19,18,17,16,16,15,15,14,13,13,12,12,11,10,10, 9, 9, 8, 7 },
/*D Tier*/	{ 20,19,19,19,19,18,18,17,17,16,15,14,13,13,12,12,11,11,10, 9 },
/*E Tier*/	{ 20,20,19,19,19,18,18,17,17,16,16,15,15,14,14,13,13,12,12,11 },
/*r*/	{ 19,19,18,17,16,16,15,15,14,14,13,12,12,11,11,10, 9, 9, 8, 7 },
/*t*/	{ 20,20,19,19,18,18,17,17,16,16,15,15,14,14,13,13,12,12,11,11 },
/*bd*/  { 18,18,18,17,17,16,16,15,15,14,14,13,12,11,11,10, 9, 9, 8, 7 },
/*mn*/  { 18,18,17,17,16,16,15,15,14,14,13,12,11,11,10,10, 9, 8, 7, 6 },	
/*dr*/  { 20,19,19,18,18,17,17,16,15,14,14,13,13,12,12,11,10,10, 9, 8 },
/*al*/  { 20,20,19,19,18,18,17,17,16,16,15,15,15,14,14,13,13,12,11,10 },
/*bu*/	{  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*ct*/	{  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
/*dm*/	{  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};


//also smithy restructured
long needed_exp[] = {
    /*2   3     4     5     6     7      8      9      10 */
    502, 1003, 2004, 4005, 8006, 16007, 32008, 65009, 100010,
    150011, 250012, 350013, 500014, 700015, 1000016, 1400017,
    1900018, 2400019, 3000020, 3600021, 4200022, 4800023, 5500024,
    7000025, 15000026};

guild cur_guilds[10];

char ply_cond_desc[][50] = {
        {" is in excellent condition.\n"},
        {" has a few small scratches.\n"},
        {" is wincing in pain.\n"},
        {" is slightly bruised and battered.\n"},
        {" has some minor wounds.\n"},
        {" is bleeding profusely.\n"},
        {" has a nasty and gaping wound.\n"},
        {" has many grevious wounds.\n"},
        {" is mortally wounded.\n"},
        {" is barely clinging to life.\n"}
        };

char obj_cond_desc[][60] = {
        {"It is in pristine condition.\n"},
        {"It is in excellent condition.\n"},
        {"It is in good condition.\n"},
        {"It has a few scratches.\n"},
        {"It is showing some wear.\n"},
        {"It has many scratches and dents.\n"},
        {"It is worn from heavy use.\n"},
        {"It is in bad condition.\n"},
        {"It is nearly broken.\n"},
        {"It looks like it could fall apart any moment now.\n"},
        {"It is broken and utterly useless.\n"}
	};

char dam_desc[][20] = {
	{"barely nicks"},
	{"scratches"},
	{"bruises"},
	{"hurts"},
	{"wounds"},
	{"smites"},
	{"maims"},
	{"pulverizes"},
	{"devestates"},
	{"mangles"},
	{"obliterates"},
	{"annihilates"},
	{"ferociously rends"}
	};



typedef int (* PFN_COMMAND ) (creature *, cmd * );

struct {
	char	*cmdstr;
	int	cmdno;
	PFN_COMMAND cmdfn;
	int 	lt; /*any LT number associated with this command*/
	time_t 	cooldown; /* base cooldown for this skill*/
	int 	level; /*level requirement to use ability*/
	int 	skillno; // the skill number associated with the ability 

} cmdlist[] = {
	/*cmdstr,		cmdno,	cmdfn,	lt,			cooldown,	level, skillno*/
	{ "n", 			1, 		move,	 0,			0L,			0,		0},
	{ "north", 		1, 		move,	 0,			0L,			0,		0 },
	{ "s", 			1, 		move,	 0,			0L,			0,		0 },
	{ "south", 		1, 		move,	 0,			0L,			0,		0 },
	{ "e", 			1, 		move,	 0,			0L,			0,		0 },
	{ "east", 		1, 		move,	 0,			0L,			0,		0 },
	{ "w", 			1, 		move,	 0,			0L,			0,		0 },
	{ "west", 		1, 		move,	 0,			0L,			0,		0 },
	{ "northeast",	1, 		move,	 0,			0L,			0,		0},
	{ "ne", 		1, 		move,	 0,			0L,			0,		0},
	{ "northwest", 	1, 		move,	 0,			0L,			0,		0 },
	{ "nw", 		1, 		move,	 0,			0L,			0,		0 },
	{ "southeast", 	1, 		move,	 0,			0L,			0,		0 },
	{ "se", 		1, 		move,	 0,			0L,			0,		0 },
	{ "southwest", 	1, 		move,	 0,			0L,			0,		0 },
	{ "sw", 		1, 		move,	 0,			0L,			0,		0 },
	{ "u", 			1, 		move,	 0,			0L,			0,		0 },
	{ "up", 		1, 		move,	 0,			0L,			0,		0 },
	{ "d", 			1, 		move,	 0,			0L,			0,		0 },
	{ "down", 		1, 		move,	 0,			0L,			0,		0 },
	{ "out", 		1, 		move,	 0,			0L,			0,		0 },
	{ "leave", 		1, 		move,	 0,			0L,			0,		0 },
	{ "l" , 		2, 		look,	 0,			0L,			0,		0},
	{ "look", 		2, 		look,	 0,			0L,			0,		0 },
	{ "consider", 	2, 		look,	 0,			0L,			0,		0 },
	{ "examine", 	2, 		look,	 0,			0L,			0,		0 },
	{ "quit", 		3, 		quit,	 0,			0L,			0,		0 },
	{ "quit", 		3, 		quit,	 0,			0L,			0,		0 },
	{ "goodbye", 	3, 		quit,	 0,			0L,			0,		0 },
	{ "goodbye", 	3, 		quit,	 0,			0L,			0,		0 },
	{ "logout", 	3, 		quit,	 0,			0L,			0,		0 },
	{ "say", 			4, 		say,	 0,			0L,			0,		0},
/*	{ "\"", 		4, 		say }, */
	{ "'", 			4, 		say,	 0,			0L,			0,		0 },
	{ "get", 		5, 		get,	 0,			0L,			0,		0 },
	{ "take", 		5, 		get,	 0,			0L,			0,		0 },
	{ "inventory", 	6, 	inventory,	 0,			0L,			0,		0 },
	{ "i", 			6, 	inventory,	 0,			0L,			0,		0 },
	{ "drop", 		7, 		drop,	 0,			0L,			0,		0 },
	{ "put", 		7, 		drop,	 0,			0L,			0,		0 },
	{ "who", 		8, 		who,	 0,			0L,			0,		0 },
	{ "wear", 		9, 		wear,	 0,			0L,			0,		0 },
	{ "remove", 	10, remove_obj,	 0,			0L,			0,		0 },
	{ "rm", 		10, remove_obj,	 0,			0L,			0,		0 },
	{ "equipment", 	11, equipment,	 0,			0L,			0,		0 },
	{ "hold", 		12, 	hold,	 0,			0L,			0,		0 },
	{ "wield", 		13, 	ready,	 0,			0L,			0,		0 },
	{ "ready", 		13, 	ready,	 0,			0L,			0,		0 },
	{ "help", 		14, 	help,	 0,			0L,			0,		0 },
	{ "?", 			14, 	help,	 0,			0L,			0,		0 },
	{ "health", 	15, 	health,	 0,			0L,			0,		0 },
	{ "score", 		15, 	health,	 0,			0L,			0,		0 },
	{ "sc", 		15, 	health,	 0,			0L,			0,		0 },
	{ "information",16, 	info,	 0,			0L,			0,		0 },
	{ "send", 		17, 	psend,	 0,			0L,			0,		0 },
	{ "sen", 		17, 	psend,	 0,			0L,			0,		0 },
	{ "tell", 		17, 	psend,	 0,			0L,			0,		0 },
	{ "follow", 	18, 	follow,	 0,			0L,			0,		0 },
	{ "lose", 		19, 	lose,	 0,			0L,			0,		0 },
	{ "group", 		20, 	group,	 0,			0L,			0,		0 },
	{ "party", 		20, 	group,	 0,			0L,			0,		0 },
	{ "track", 		21, 	track,	 LT_TRACK,	10L,		1,		127 },
	{ "peek", 		22, 	peek,	 LT_PEEKS,	10L,		1,		0 },
	{ "attack", 	23, 	attack,	 LT_ATTCK,	3L,			1,		0 },
	{ "kill", 		23, 	attack,	 LT_ATTCK,	3L,			0,		0 },
	{ "k", 			23, 	attack,	 LT_ATTCK,	3L,			0,		0 },
	{ "search", 	24, 	search,	 LT_SERCH,	7L,			1,		0 },
	{ "emote",		25, 	emote,	 0,		0L,			0,		0},    
	{ ":",			25, 	emote,	 0,		0L,			0,		0},    
	{ "hide", 		26, 	hide,	 LT_HIDES,		15L,	1,		0 },
	{ "set", 		27, 	set,	 0,		0L,			0,		0 },
	{ "clear", 		28, 	clear,	 0,		0L,			0,		0 },
	{ "yell", 		29, 	yell,	 0,		0L,			0,		0 },
	{ "go", 		30, 	go,	 0,		0L,			0,		0 },
	{ "enter", 		30, 	go,	 0,		0L,			0,		0 },
	{ "open", 		31, openexit,	 0,		0L,			0,		0 },
	{ "close", 		32, closeexit,	 0,		0L,			0,		0 },
	{ "shut", 		32, closeexit,	 0,		0L,			0,		0 },
	{ "unlock", 	33, 	unlock,	 0,		0L,			0,		0 },
	{ "lock", 		34, 	lock,	 0,		0L,			0,		0 },
	{ "pick", 		35, 	picklock, LT_PICKL,		10L,	7,		122 },
	{ "steal", 		36, 	steal,	 0,		0L,			1,		143 },
	{ "flee", 		37, 	flee,	 0,		0L,			0,		0 },
	{ "fl", 		37, 	flee,	 0,		0L,			0,		0 },
	{ "run", 		37, 	flee,	 0,		0L,			0,		0 },
	{ "c", 			38, 	cast,	 LT_SPELL,		5L,			0,		0 },
	{ "cast", 		38, 	cast,	 LT_SPELL,		5L,			1,		0 },
	{ "study", 		39, 	study,	 0,		0L,			0,		0 },
	{ "learn", 		39, study,	 0,		0L,			0,		0 },
	{ "read", 		40, readscroll,	 0,		0L,			0,		0 },
	{ "recite", 	40, readscroll,	 0,		0L,			0,		0 },
	{ "list", 		41, list,	 0,		0L,			0,		0 },
	{ "buy", 		42, buy,	 0,		0L,			0,		0 },
	{ "sell", 		43, sell,	 0,		0L,			0,		0 },
	{ "appraise", 	44, appraise,	 0,		0L,			0,		0 },
	{ "cost", 		44, appraise,	 0,		0L,			0,		0 },
	{ "value", 		44, appraise,	 0,		0L,			0,		0 },
	{ "bs", 		45, backstab,	 0,		0L,			4,		0 },
	{ "backstab", 	45, backstab,	 0,		0L,			4,		0 },
	{ "train", 		46, train,	 0,		0L,			0,		0 },
	{ "give", 		47, give,	 0,		0L,			0,		0},
	{ "repair", 	48, repair,	 0,		0L,			0,		0 },
	{ "fix", 		48, repair,	 0,		0L,			0,		0 },
	{ "time", 		49, prt_time,	 0,		0L,			0,		0 },
	{ "circle", 	50, circle,	 LT_ATTCK,		3L,			4,		0 },
	{ "bash", 		51, bash,	 0,		0L,			7,		0 },
	{ "save", 		52, save_ply_cmd,	 0,		0L,			0,		0 },
	{ "sendmail", 	53, postsend,	 0,		0L,			0,		0 },
	{ "readmail", 	54, postread,	 0,		0L,			0,		0 },
	{ "deletemail", 55, postdelete,	 0,		0L,			0,		0 },
	{ "talk", 56, talk,	 0,		0L,			0,		0 },
	{ "ask", 56, talk,	 0,		0L,			0,		0 },
	{ "parley", 56, talk,	 0,		0L,			0,		0 },
	{ "gtalk",57, gtalk,	 0,		0L,			0,		0 },
	{ "drink", 58, drink,	 0,		0L,			0,		0 },
	{ "quaff", 58, drink,	 0,		0L,			0,		0 },
	{ "broadcast", 59, broadsend,	 0,		0L,			2,		0 },
	{ "zap", 60, zap,	 0,		0L,			0,		0 },
	{ "welcome", 61, welcome,	 0,		0L,			0,		0 },
	{ "turn", 62, turn,	 0,		0L,			13,		0 },
	{ "bribe", 63, bribe,	 0,		0L,			0,		0 },
	{ "haste", 64, haste,	 LT_HASTE,		600L,			10,		156 },
	{ "pray", 65, pray,	 LT_PRAYD,		600L,			10,		0 },
	{ "prepare", 66, prepare,	 LT_PREPA,		15L,			1,		0 },
	{ "use", 67, use,	 0,		0L,			0,		0 },
	{ "ignore", 68, ignore,	 0,		0L,			0,		0 },
	{ "whois", 69, whois,	 0,		0L,			0,		0 },
	{ "sneak", 70, sneak,	 0,		0L,			1,		115 },
	{ "teach", 71, teach,	 0,		0L,			1,		0 },
	{ "pledge",72, pledge,	 0,		0L,			10,		0 },
	{ "rescind",73, rescind,	 0,		0L,			10,		0 },
	{ "purchase",74, purchase,	 0,		0L,			0,		0},    
	{ "selection",75, selection,	 0,		0L,			0,		0 }, 
	{ "trade",76, trade,	 0,		0L,			0,		0},    
	{ "suicide", 77, ply_suicide,	 0,		0L,			0,		0 },
	{ "passwd", 78, passwd,	 0,		0L,			0,		0},
	{ "password", 78, passwd,	 0,		0L,			0,		0 },
	{ "vote",79,vote,	 0,		0L,			0,		0},
	{ "finger", 80, pfinger,	 0,		0L,			0,		0},
	{ "charm", 81, bard_song2,	 LT_CHARM,		600L,			10,		0},
	{ "meditate", 82, meditate,	 LT_MEDIT,		120L,			4,		0},
	{ "touch", 83, touch_of_death,	 LT_TOUCH,		600L,		10,		0},
	{ "transform",84,transform,	 0,		0L,			4,		0 },
	{ "apply",85,apply_herb,	 0,		0L,			0,		0 },
	{ "ingest",86,ingest_herb,	 0,		0L,			0,		0 },
	{ "eat",87,eat,	 0,		0L,			0,		0 },
	{ "paste",88,paste_herb,	 0,		0L,			0,		0 },
	{ "recharge", 89, recharge_wand,	 0,		0L,			7,		0 },
	{ "description", 90, describe_me,	 0,		0L,			0,		0 },
	{ "enchant", 91, tmp_enchant,	 0,		0L,			10,		0 },
	{ "version", 92, about,	 0,		0L,			0,		0 },
	{ "scout", 93, scout,	 LT_SCOUT,		10L,			4,		115 },
	{ "transmute", 94, transmute,	 0,		0L,			7,		0 },
	{ "berserk", 95, berserk,	 LT_BSRKN,		0L,			7,		0 },
	{ "uptime", 96, uptime,	 0,		0L,			0,		0},
	{ "barkskin", 97, barkskin,	 LT_BSRKN,		0L,			7,		0 },
/*
	{ "channelemote", 97, channelemote},
	{ "chemote", 97, channelemote},
	{ "broe", 97, channelemote},
*/
	{ "classemote", 98, classemote,	 0,		0L,			0,		0},
	{ "clemote", 98, classemote,	 0,		0L,			0,		0},
	{ "classsend", 99, class_send,	 0,		0L,			0,		0},
	{ "clsend", 99, class_send,	 0,		0L,			0,		0},
	
	/* help 101-399 reserved for DM commands */
	{ "guildtalk", 401, guild_send,	 0,		0L,			0,		0},
	{ "gls", 401, guild_send,	 0,		0L,			0,		0},
	{ "guilds", 402, guilds_list,	 0,		0L,			0,		0},
	{ "offer", 403, offer,	 0,		0L,			0,		0},	
	{ "shop", 404, setup_shop,	 0,		0L,			0,		0 },
	{ "balance", 405, bank_balance,	 0,		0L,			0,		0 },
	{ "withdraw", 405, bank_withdrawl,	 0,		0L,			0,		0 },
	{ "deposit", 405, bank_deposit,	 0,		0L,			0,		0 },
	{ "transfer", 405, transfer_balance,	 0,		0L,			0,		0 },
	{ "tnl", 406, to_next_level,	 0,		0L,			0,		0 },
	{ "hp", 407, say_hp,	 0,		0L,			0,		0 },
	{ "mp", 408, say_mp,	 0,		0L,			0,		0 },
	{ "nod", 100, action,	 0,		0L,			0,		0 },
	{ "sleep", 100, action,	 0,		0L,			0,		0 },
	{ "grab", 100, action,	 0,		0L,			0,		0 },
	{ "shove", 100, action,	 0,		0L,			0,		0 },
	{ "stand", 100, action,	 0,		0L,			0,		0 },
	{ "sit", 100, action,	 0,		0L,			0,		0 },
	{ "nervous", 100, action,	 0,		0L,			0,		0 },
	{ "bird", 100, action,	 0,		0L,			0,		0 },
	{ "ogle", 100, action,	 0,		0L,			0,		0 },
	{ "nod", 100, action,	 0,		0L,			0,		0 },
	{ "relax", 100, action,	 0,		0L,			0,		0 },
	{ "puke", 100, action,	 0,		0L,			0,		0 },
	{ "think", 100, action,	 0,		0L,			0,		0 },	
	{ "cheer", 100, action,	 0,		0L,			0,		0 },
	{ "ponder", 100, action,	 0,		0L,			0,		0 },
	{ "ack", 100, action,	 0,		0L,			0,		0 },
	{ "laugh", 100, action,	 0,		0L,			0,		0 },
	{ "burp", 100, action,	 0,		0L,			0,		0 },
	{ "frustrate", 100, action,	 0,		0L,			0,		0 },
	{ "kick", 100, action,	 0,		0L,			0,		0 },
	{ "tackle", 100, action,	 0,		0L,			0,		0 },
	{ "knee", 100, action,	 0,		0L,			0,		0 },
	{ "pounce", 100, action,	 0,		0L,			0,		0 },
	{ "tickle", 100, action,	 0,		0L,			0,		0 },
	{ "snicker", 100, action,	 0,		0L,			0,		0 },
	{ "tap", 100, action,	 0,		0L,			0,		0 },
	{ "smile", 100, action,	 0,		0L,			0,		0 },
	{ "beam", 100, action,	 0,		0L,			0,		0},
	{ "masturbate", 100, action,	 0,		0L,			0,		0},
	{ "smoke", 100, action,	 0,		0L,			0,		0 },
	{ "shake", 100, action,	 0,		0L,			0,		0 },
	{ "cackle", 100, action,	 0,		0L,			0,		0 },
	{ "chuckle", 100, action,	 0,		0L,			0,		0 },
	{ "wave", 100, action,	 0,		0L,			0,		0 },
	{ "poke", 100, action,	 0,		0L,			0,		0 },
	{ "yawn", 100, action,	 0,		0L,			0,		0 },
	{ "sigh", 100, action,	 0,		0L,			0,		0 },
	{ "bounce", 100, action,	 0,		0L,			0,		0 },
	{ "shrug", 100, action,	 0,		0L,			0,		0 },
	{ "twiddle", 100, action,	 0,		0L,			0,		0 },
	{ "grin", 100, action,	 0,		0L,			0,		0 },
	{ "frown", 100, action,	 0,		0L,			0,		0 },
	{ "giggle", 100, action,	 0,		0L,			0,		0 },
	{ "sing", 100, action,	 0,		0L,			0,		0 },
	{ "hum", 100, action,	 0,		0L,			0,		0 },
	{ "snap", 100, action,	 0,		0L,			0,		0 },
	{ "jump", 100, action,	 0,		0L,			0,		0},
	{ "skip", 100, action,	 0,		0L,			0,		0 },
	{ "dance", 100, action,	 0,		0L,			0,		0 },
	{ "cry", 100, action,	 0,		0L,			0,		0 },
	{ "bleed", 100, action,	 0,		0L,			0,		0 },
	{ "sniff", 100, action,	 0,		0L,			0,		0 },
	{ "whimper", 100, action,	 0,		0L,			0,		0 },
	{ "cringe", 100, action,	 0,		0L,			0,		0 },
	{ "whistle", 100, action,	 0,		0L,			0,		0 },
	{ "smirk", 100, action,	 0,		0L,			0,		0 },
	{ "gasp", 100, action,	 0,		0L,			0,		0 },
	{ "grunt", 100, action,	 0,		0L,			0,		0 },
	{ "stomp", 100, action,	 0,		0L,			0,		0 },
	{ "flex", 100, action,	 0,		0L,			0,		0 },
	{ "curtsy", 100, action,	 0,		0L,			0,		0 },
	{ "blush", 100, action,	 0,		0L,			0,		0 },
	{ "faint", 100, action,	 0,		0L,			0,		0 },
	{ "hug", 100, action,	 0,		0L,			0,		0 },
	{ "expose", 100, action,	 0,		0L,			0,		0 },
	{ "wink", 100, action,	 0,		0L,			0,		0 },
	{ "clap", 100, action,	 0,		0L,			0,		0 },
	{ "drool", 100, action,	 0,		0L,			0,		0},
	{ "copulate", 100, action,	 0,		0L,			0,		0 },
	{ "goose", 100, action,	 0,		0L,			0,		0 },
	{ "fume", 100, action,	 0,		0L,			0,		0},
	{ "rage", 100, action,	 0,		0L,			0,		0 },
	{ "pout", 100, action,	 0,		0L,			0,		0 },
	{ "spit", 100, action,	 0,		0L,			0,		0 },
	{ "fart", 100, action,	 0,		0L,			0,		0 },
	{ "comfort", 100, action,	 0,		0L,			0,		0 },
	{ "pat", 100, action,	 0,		0L,			0,		0 },
	{ "pet", 100, action,	 0,		0L,			0,		0 },
	{ "kiss", 100, action,	 0,		0L,			0,		0 },
	{ "glare", 100, action,	 0,		0L,			0,		0 },
	{ "slap", 100, action,	 0,		0L,			0,		0 },
	{ "suck", 100, action,	 0,		0L,			0,		0 },
	{ "defecate", 100, action,	 0,		0L,			0,		0 },
	{ "bow", 100, action,	 0,		0L,			0,		0 },
	{ "cough", 100, action,	 0,		0L,			0,		0 },
	{ "confused", 100, action,	 0,		0L,			0,		0 },
	{ "grumble", 100, action,	 0,		0L,			0,		0 },
	{ "hiccup", 100, action,	 0,		0L,			0,		0 },
	{ "mutter", 100, action,	 0,		0L,			0,		0 },
	{ "scratch", 100, action,	 0,		0L,			0,		0 },
	{ "strut", 100, action,	 0,		0L,			0,		0 },
	{ "sulk", 100, action,	 0,		0L,			0,		0 },
	{ "wince", 100, action,	 0,		0L,			0,		0 },
	{ "roll", 100, action,	 0,		0L,			0,		0 },
	{ "raise", 100, action,	 0,		0L,			0,		0 },
	{ "whine", 100, action,	 0,		0L,			0,		0 },
	{ "growl", 100, action,	 0,		0L,			0,		0 },
	{ "grr", 100, action,	 0,		0L,			0,		0 },
	{ "high5", 100, action,	 0,		0L,			0,		0 },
	{ "moon", 100, action,	 0,		0L,			0,		0 },
	{ "purr", 100, action,	 0,		0L,			0,		0 },
	{ "taunt", 100, action,	 0,		0L,			0,		0 },
	{ "eye", 100, action,	 0,		0L,			0,		0 },
	{ "msmile", 100, action,	 0,		0L,			0,		0 },
	{ "worship", 100, action,	 0,		0L,			0,		0 },
	{ "flip", 100, action,	 0,		0L,			0,		0 },
	{ "groan", 100, action,	 0,		0L,			0,		0 },
	{ "salute", 100, action,	 0,		0L,			0,		0 },
	{ "sneer", 100, action,	 0,		0L,			0,		0 },
	{ "grovel", 100, action,	 0,		0L,			0,		0 },
	{ "nose", 100, action,	 0,		0L,			0,		0 },
	{ "cuddle", 100, action,	 0,		0L,			0,		0 },
	{ "wsmile", 100, action,	 0,		0L,			0,		0 },
	{ "ssmile", 100, action,	 0,		0L,			0,		0 },
	{ "twirl", 100, action,	 0,		0L,			0,		0 },
	{ "flaugh", 100, action,	 0,		0L,			0,		0 },
	{ "rlaugh", 100, action,	 0,		0L,			0,		0 },
	{ "hlaugh", 100, action,	 0,		0L,			0,		0 },
	{ "snivel", 100, action,	 0,		0L,			0,		0 },
	{ "smell", 100, action,	 0,		0L,			0,		0 },
	{ "pace", 100, action,	 0,		0L,			0,		0 },
	{ "sneeze", 100, action,	 0,		0L,			0,		0 },
	{ "lick", 100, action,	 0,		0L,			0,		0 },
	{ "snort", 100, action,	 0,		0L,			0,		0 },
	{ "grind", 100, action,	 0,		0L,			0,		0 },
	{ "kisshand", 100, action,	 0,		0L,			0,		0 },
	{ "sneer", 100, action,	 0,		0L,			0,		0 },
	{ "point", 100, action,	 0,		0L,			0,		0 },
	{ "imitate", 100, action,	 0,		0L,			0,		0 },
	{ "thumb", 100, action,	 0,		0L,			0,		0 },
	{ "punch", 100, action,	 0,		0L,			0,		0 },
	{ "plead", 100, action,	 0,		0L,			0,		0 },
	{ "grovel", 100, action,	 0,		0L,			0,		0 },
	{ "cower", 100, action,	 0,		0L,			0,		0 },
	{ "cuddle", 100, action,	 0,		0L,			0,		0 },
	{ "shush", 100, action,	 0,		0L,			0,		0 },
	{ "beckon", 100, action,	 0,		0L,			0,		0 },
	{ "greet", 100, action,	 0,		0L,			0,		0 },
	{ "squirm", 100, action,	 0,		0L,			0,		0 },
	{ "idea", 100, action,	 0,		0L,			0,		0 },
	{ "collapse", 100, action,	 0,		0L,			0,		0},
	{ "sweat", 100, action,	 0,		0L,			0,		0 },
	{ "attention", 100, action,	 0,		0L,			0,		0 },
	{ "squirm", 100, action,	 0,		0L,			0,		0 },
	{ "nosepick", 100, action,	 0,		0L,			0,		0 },
	{ "blah", 100, action,	 0,		0L,			0 },
	/* help 101-399 reserved for DM commands */
	{ "*teleport", 101, dm_teleport,	 0,		0L,			0,		0 },
	{ "*t", 101, dm_teleport,	 0,		0L,			0,		0 },
	{ "*rm", 102, dm_rmstat,	 0,		0L,			0,		0 },
	{ "*reload", 103, dm_reload_rom,	 0,		0L,			0,		0 },
	{ "*save", 104, dm_save,	 0,		0L,			0,		0 },
	{ "*create", 105, dm_create_obj,	 0,		0L,			0,		0 },
	{ "*c", 105, dm_create_obj,	 0,		0L,			0,		0 },
	{ "*perm", 106, dm_perm,	 0,		0L,			0,		0 },
	{ "*invis", 107, dm_invis,	 0,		0L,			0,		0 },
	{ "*i", 107, dm_invis,	 0,		0L,			0,		0 },
	{ "*s", 108, dm_send,	 0,		0L,			0,		0 },
	{ "*send", 108, dm_send,	 0,		0L,			0,		0 },
	{ "*purge", 109, dm_purge,	 0,		0L,			0,		0 },
	{ "*ac", 110, dm_ac,	 0,		0L,			0,		0 },
	{ "*users", 111, dm_users,	 0,		0L,			0,		0 },
	{ "*echo", 112, dm_echo,	 0,		0L,			0,		0 },
	{ "*flushrooms", 113, dm_flushsave,	 0,		0L,			0,		0 },
	{ "*sh", 114, shutdown_catch,	 0,		0L,			0,		0},
	{ "*shu", 114, shutdown_catch,	 0,		0L,			0,		0},
	{ "*shut", 114, dm_shutdown,	 0,		0L,			0,		0},
	{ "*shutd", 114, shutdown_catch,	 0,		0L,			0,		0},
	{ "*shutdo", 114, shutdown_catch,	 0,		0L,			0,		0},
	{ "*shutdow", 114, shutdown_catch,	 0,		0L,			0,		0},
	{ "*shutdown", 114, dm_shutdown,	 0,		0L,			0,		0 },
	{ "*f", 115, dm_force,	 0,		0L,			0,		0 },
	{ "*force", 115, dm_force,	 0,		0L,			0,		0 },
	{ "*flushcrtobj", 116, dm_flush_crtobj,	 0,		0L,			0,		0 },
	{ "*monster", 117, dm_create_crt,	 0,		0L,			0,		0 },
	{ "*status", 118, dm_stat,	 0,		0L,			0,		0 },
	{ "*st", 118, dm_stat,	 0,		0L,			0,		0 },
	{ "*add", 119, dm_add_rom,	 0,		0L,			0,		0 },
	{ "*set", 120, dm_set,	 0,		0L,			0,		0 },
	{ "*log", 121, dm_log,	 0,		0L,			0,		0 },
	{ "*spy", 122, dm_spy,	 0,		0L,			0,		0 },
	{ "*lock", 123, dm_loadlockout,	 0,		0L,			0,		0 },
	{ "*finger", 124, dm_finger,	 0,		0L,			0,		0  },
	{ "*list", 125, dm_list,	 0,		0L,			0,		0 },
	{ "*info", 126, dm_info,	 0,		0L,			0,		0 },
	{ "*parameter", 127, dm_param,	 0,		0L,			0,		0 },
	{ "*silence", 128, dm_silence,	 0,		0L,			0,		0},
	{ "*broad", 129, dm_broadecho,	 0,		0L,			0,		0},
 	{ "*replace",130, dm_replace,	 0,		0L,			0,		0},    
	{ "*name",131, dm_nameroom,	 0,		0L,			0,		0},    
   	{ "*append",132, dm_append,	 0,		0L,			0,		0},    
   	{ "*prepend",133, dm_prepend,	 0,		0L,			0,		0},    
   	{ "*gcast",134, dm_cast,	 0,		0L,			0,		0},    
	{ "*group",135, dm_group,	 0,		0L,			0,		0},    
	{ "*notepad",136, notepad,	 0,		0L,			0,		0},
 	{ "*delete",137, dm_delete,	 0,		0L,			0,		0},    
	{ "*oname", 138, dm_obj_name,	 0,		0L,			0,		0 },
	{ "*cname", 139, dm_crt_name,	 0,		0L,			0,		0 },
	{ "*active", 140, list_act,	 0,		0L,			0,		0 },
	{ "*dust", 141, dm_dust,	 0,		0L,			0,		0 },
	{ "*dmhelp", 142, dm_help,	 0,		0L,			0,		0 },
	{ "*attack", 143, dm_attack,	 0,		0L,			0,		0 },
	{ "*enemy", 144, list_enm,	 0,		0L,			0,		0 },
	{ "*charm", 145, list_charm,	 0,		0L,			0,		0 },
	{ "*auth", 146, dm_auth,	 0,		0L,			0,		0 },
	{ "*possess", 147, dm_alias,	 0,		0L,			0,		0 }, 
	{ "*tell", 148, dm_flash,	 0,		0L,			0,		0}, 
	{ "*memory", 149, dm_memory,	 0,		0L,			0,		0 },
	{ "*find", 150, dm_find_db,	 0,		0L,			0,		0 },
	{ "*clear", 151, dm_delete_db,	 0,		0L,			0,		0 },
	{ "*talk", 152, dm_talk,	 0,		0L,			0,		0 },
	{ "*gamestat", 153, dm_game_status,	 0,		0L,			0,		0 },
	{ "*advance", 154, dm_advance,	 0,		0L,			0,		0 },
	{ "*mallocstats", 155, dm_dmalloc_stats,	 0,		0L,			0,		0 },
	{ "*builderhelp", 160, builder_help,	 0,		0L,			0,		0 },
	{ "*own", 161, dm_own_ply,	 0,		0L,			0,		0 },
	{ "*dumpio", 162, dm_dump_io,	 0,		0L,			0,		0 },
	{ "*jail", 163, dm_jail,	 0,		0L,			0,		0 },
	{ "*afk", 164, dm_afk,	 0,		0L,			0 ,		0},
	{ "*mstat", 165, dm_mstat,	 0,		0L,			0,		0 },
	{ "*ostat", 166, dm_ostat,	 0,		0L,			0,		0 },
	{ "*tdump", 167, dm_dump_db,	 0,		0L,			0,		0 },
	{ "*war", 168, guild_war,	 0,		0L,			0,		0 },
	{ "*welcome", 169, welcome_edit,	 0,		0L,			0,		0 },
	{ "*shop", 170, dm_reset_shop,	 0,		0L,			0,		0 },
	{ "*stolen", 171, dm_stolen,	 0,		0L,			0,		0 },
	{ "push", -2, 0,	 0,		0L,			0,		0 },
	{ "press", -2, 0,	 0,		0L,			0,		0 },

/*new skills from here down!!! */	
	{ "midas", 		172, midas,	 		LT_MIDAS,		200L,			13,		0},
	{ "skills", 	173, skills,  		0,				0L,				0,		0},
	{ "objectives",	174, objectives,	0,				0L,				0,		0},

	{ "attune",		174, attune,		LT_ATTUNE,		600L,			1,		0},
	{ "distill",	175, distill,		LT_DISTILL,		60L,			1,		0},
	{ "envenom",	176, envenom,		LT_ENVENOM, 	60L,			1,		141},
	
	{ "skilltree",	177, init_skilltree,		0, 				0L,				0,		0},
	{ "statup",		178, init_statup, 			0, 				0L, 			0,		0},
	{ "@", 0, 0,	 0,		0L,			0,		0 }
}
;



char article[][10] = {
	"the",
	"from",
	"to",
	"with",
	"an",
	"in",
	"for",
	"@"
};




struct osp_t ospell[] = {
	/*
	int	splno;
	char	realm;
	int	mp;
	int	ndice;
	int	sdice;
	int	pdice;
	char	bonus_type;
	int	intcast;
	*/
	{ SHURTS,  WIND,  3, 1, 8,  0, 1, 1},	/* hurt */
	{ SRUMBL, EARTH,  3, 1, 8,  0, 1, 1},	/* rumble */
	{ SBURNS,  FIRE,  3, 1, 7,  1, 1, 1},	/* burn */
	{ SBLIST, WATER,  3, 1, 8,  0, 1, 1},	/* blister */

	{ SDUSTG,  WIND,  7, 2, 5,  7, 2, 4},	/* dustgust */
	{ SCRUSH, EARTH,  7, 2, 5,  7, 2, 4},	/* stonecrush */
	{ SFIREB,  FIRE,  7, 2, 5,  8, 2, 4},	/* fireball */
	{ SWBOLT, WATER,  7, 2, 5,  8, 2, 4},	/* waterbolt */

	{ SSHOCK,  WIND, 10, 2, 5, 13, 2, 7},	/* shockbolt */
	{ SENGUL, EARTH, 10, 2, 5, 13, 2, 7},	/* engulf */
	{ SBURST,  FIRE, 10, 2, 5, 13, 2, 7},	/* burstflame */
	{ SSTEAM, WATER, 10, 2, 5, 13, 2, 7},	/* steamblast */

	{ SLGHTN,  WIND, 15, 3, 4, 18, 3, 10},	/* lightning */
	{ SSHATT, EARTH, 15, 3, 4, 19, 3, 10},	/* shatterstone */
	{ SIMMOL,  FIRE, 15, 3, 4, 18, 3, 10},	/* immolate */
	{ SBLOOD, WATER, 15, 3, 4, 18, 3, 10},	/* bloodboil */

	{ STHUND,  WIND, 25, 4, 5, 30, 3, 13},	/* thuderbolt */
	{ SEQUAK, EARTH, 25, 4, 5, 30, 3, 13}, 	/* tremor */
	{ SFLFIL,  FIRE, 25, 4, 5, 30, 3, 13},	/* flamefill */
	{ SICEBL, WATER, 25, 4, 5, 30, 3, 13},	/* iceblade */

	{ STORNA,  WIND, 25, 4, 5, 30, 3, 16},	/* tornado */
	{ STRMOR, EARTH, 25, 4, 5, 30, 3, 16}, 	/* tremor */
	{ SINCIN,  FIRE, 25, 4, 5, 30, 3, 16},	/* incinerate */
	{ SFLOOD, WATER, 25, 4, 5, 30, 3, 16},	/* flood */

	{ -1, 0, 0, 0, 0, 0, 0, 0}
};

/*
	*** IMPORTANT NOTE REGARDING MONK HITTING ***
	These monk dice arrays are obsoleted by the new hitting formula
	in command5.c.
*/
#ifdef OLD_MONK_DICE

/* monk leveling code */
DICE monk_dice[26] = 
{
	{ 1, 3, 0 },
	{ 1, 3, 0 },
	{ 1, 5, 0 },
	{ 1, 5, 1 },
	{ 1, 6, 0 },
	{ 1, 6, 1 },
	{ 1, 6, 2 },
	{ 2, 3, 1 },
	{ 2, 4, 0 },
	{ 2, 4, 1 },
	{ 2, 5, 0 },
	{ 2, 5, 2 },
	{ 2, 6, 1 },
	{ 2, 6, 2 },
	{ 3, 6, 1 },
	{ 3, 7, 1 },
	{ 4, 7, 1 },
	{ 5, 7, 0 },
	{ 5, 8, 1 },
	{ 6, 7, 0 },
	{ 6, 7, 2 },
	{ 6, 8, 0 },
	{ 6, 8, 2 },
	{ 6, 9, 0 },
	{ 6, 9, 2 },
	{ 6, 10, 0 }
};

/* the original code had a 6, 8, 2 for level 24 */
/* it appears that it should be 6, 9, 2 so I changed it */

#else

DICE monk_dice[26] = 
{
	{ 1, 3, 0 },
	{ 1, 3, 0 },
	{ 1, 5, 0 },
	{ 2, 3, 0 },
	{ 1, 6, 1 },
	{ 2, 3, 1 },
	{ 1, 6, 2 },
	{ 2, 4, 1 },
	{ 1, 8, 2 },
	{ 2, 5, 1 },
	{ 4, 3, 0 },
	{ 3, 4, 1 },
	{ 2, 6, 2 },
	{ 1, 12, 3 },
	{ 3, 5, 1 },
	{ 3, 5, 2 },
	{ 4, 5, 1 },
	{ 4, 5, 2 },
	{ 4, 5, 3 },
	{ 5, 5, 1 },
	{ 5, 5, 2 },
	{ 5, 5, 3 },
	{ 6, 5, 1 },
	{ 6, 5, 2 },
	{ 6, 5, 3 },
	{ 6, 6, 2 }
};


#endif

/* This is for creating monster accoding to the monster guide */
struct {
	int 	stats;
	short 	hp;
	char	armor;
	char	thaco;
	long	experience;
	short	ndice;
	short	sdice;
	short	pdice;
} monster_guide[]= {
{55,10,90,19,10,1,4,1},
{58,20,80,18,15,1,4,2},
{61,30,70,17,35,2,3,1},
{65,45,60,16,50,2,4,0},
{68,60,50,15,65,2,4,2},
{71,75,40,14,100,2,6,0},
{75,90,30,13,140,2,6,2},
{78,105,20,12,200,2,7,2},
{81,120,10,11,250,2,8,2},
{85,135,0,10,350,3,6,2},
{88,150,-5,9,450,2,10,2},
{91,165,-10,8,550,3,8,0},
{95,180,-15,7,700,3,8,2},
{98,195,-20,6,850,3,9,1},
{101,210,-25,5,1000,3,9,3},
{105,225,-30,4,1150,3,10,3},
{108,240,-35,3,1300,6,6,0},
{111,255,-40,2,1450,6,6,4},
{115,270,-45,1,1600,8,5,5},
{118,285,-50,0,1800,9,5,5},
{121,300,-60,-2,2200,10,5,4},
{122,350,-70,-4,2600,11,5,3},
{123,400,-80,-6,3200,12,5,3},
{124,450,-90,-8,4000,13,5,3},
{125,500,-100,-10,5000,14,6,3}
};


/* This is to set default enemylists for monters */

int ENMLIST=7; /* set this to length of monster_enmlist array */

struct {
	char *attacker;
	char *target;
} monster_enmlist[] = {
{ "orc", "militiaman" },
{ "thief", "merchant" },
{ "thief", "nobleman" },
{ "goblin", "militiaman" },
{ "large goblin", "militiaman" },
{ "goblin warrior", "militiaman" },
{ "drunk", "hobbit" }
};
