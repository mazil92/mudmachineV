/*
 * ACCESS.C:
 *
 *      This file contains the routines necessary to access arrays
 *
 *      Copyright (C) 1991, 1992, 1993 Brooke Paul
 *      Copyright (C) 1998 John P. Freeman
 * $Id: access.c,v 6.14 2001/04/12 05:00:57 develop Exp $
 *
 * $Log: access.c,v $
 * Revision 6.14  2001/04/12 05:00:57  develop
 * readded room_damage spells to spellist
 *
 * Revision 6.13  2001/03/08 16:09:09  develop
 * *** empty log message ***
 *
 *
 */

#include "../include/mordb.h"

#include "mextern.h"


int		Spy[PMAX];


static struct {
char *splstr;
int splno;
SPELLFN splfn;
int level;
int realm;
int mana;
int proftype;
int spelltype;

} spllist[] = {
/* SPELL FLAGS ARE DEFINED IN MTYPE.H*/
/* SPLSTR ,   		SPLNO,  SPLFN, 				LVL, 	REALM, MANA, PRFTYPE, 	SPLTYPE*/

{ "rumble",   		SRUMBL, offensive_spell,  	1, 		EARTH, 3, 	DAMAGE, 	DSINGLE  },
{ "crush",   		SCRUSH, offensive_spell,  	2, 		EARTH, 7, 	DAMAGE, 	DSINGLE  },
/* EARTH UTILITY SPELL, 						2,      EARTH, ?? 	?? 			??*/
{ "shatterstone",   SENGUL, offensive_spell,  	3, 		EARTH, 10, 	DAMAGE, 	DSINGLE  },
{ "resist-earth",   SSSHLD, earth_shield,  	  	3, 		EARTH, 10, 	DURATION, 	PBUFF  },
{ "engulf",   		SSHATT, offensive_spell,  	4, 		EARTH, 15, 	DAMAGE, 	DSINGLE  },
/* RESIST EARTH, 								4,      EARTH, 15 	INTENSITY 	PBUFF*/
{ "earthquake",   	SEQUAK, offensive_spell,  	5, 		EARTH, 25, 	DAMAGE, 	DSINGLE  },
/* EARTH UTILITY SPELL 							5,      EARTH, ?? 	?? 			??*/
{ "tremor",   		STRMOR, room_damage,  		6, 		EARTH, 25, 	DAMAGE, 	DROOM  },
/* EARTH UTILITY SPELL 							6,      EARTH, ?? 	?? 			??*/
/* EARTH ATTUNEMENT (doubles earth damage)		7,      EARTH, 30 	INTENSITY 	PBUFF*/

{ "hurt",   		SHURTS, offensive_spell,  	1, 		WIND,  3, 	DAMAGE, 	DSINGLE },
{ "dustgust",   	SDUSTG, offensive_spell, 	2, 		WIND,  7, 	DAMAGE, 	DSINGLE },
/* WIND UTILITY SPELL 							2,      WIND,   ?? ?? ??*/
{ "shockbolt",   	SSHOCK, offensive_spell, 	3, 		WIND, 10, 	DAMAGE, 	DSINGLE },
{ "resist-cold",   	SRCOLD, resist_cold, 		3, 		WIND, 10, 	DURATION, 	PBUFF },
{ "lightning",   	SLGHTN, offensive_spell, 	4, 		WIND, 15, 	DAMAGE, 	DSINGLE },
/* RESIST WIND 									4,      WIND,   15 	INTENSITY 	PBUFF*/
{ "thunderbolt",   	STHUND, offensive_spell, 	5, 		WIND, 25, 	DAMAGE, 	DSINGLE },
/* WIND UTILITY SPELL 							5,      WIND, ?? ?? ??*/
{ "tornado",   		STORNA, room_damage, 		6, 		WIND, 25, 	DAMAGE, 	DROOM },
/* WIND UTILITY SPELL 							6,      WIND, ?? ?? ??*/
/* WIND ATTUNEMENT (doubles wind damage)		7,      WIND, 30 	INTENSITY 	PBUFF*/

{ "burn",   		SBURNS, offensive_spell, 	1,		FIRE, 3,	DAMAGE,		DSINGLE },
{ "fireball",   	SFIREB, offensive_spell,	2,		FIRE, 7,	DAMAGE,		DSINGLE},
/* FIRE UTILITY SPELL 							2,      FIRE,  ?? 	?? 			??*/
{ "burstflame",   	SBURST, offensive_spell,	3,		FIRE, 10,	DAMAGE,		DSINGLE },
{ "resist-fire",  	SRFIRE, resist_fire,		3,		FIRE, 10,	DURATION,	PBUFF },
{ "immolate",   	SIMMOL, offensive_spell,	4,		FIRE, 15,	DAMAGE,		DSINGLE },
/* FIRE DAMAGE RESISTANCE						4,      FIRE,  ?? 	?? 			??*/
{ "flamefill",   	SFLFIL, offensive_spell,	5,		FIRE, 25,	DAMAGE,		DSINGLE },
/* FIRE UTILITY SPELL 							5,      FIRE,  ?? 	?? 			??*/
{ "incinerate",   	SINCIN, room_damage,		6,		FIRE, 25,	DAMAGE,		DROOM },
/* FIRE UTILITY SPELL 							6,      FIRE,  ?? 	?? 			??*/
/* FIRE ATTUNEMENT (doubles fire damage)		7,      FIRE,  30 	INTENSITY 	PBUFF*/

{ "blister",   		SBLIST, offensive_spell,	1,		WATER, 3,	DAMAGE,		DSINGLE },
{ "waterbolt",   	SWBOLT, offensive_spell,	2,		WATER, 7,	DAMAGE,		DSINGLE },
/* cleanse, (a water healing spell)				2,      WATER, 8,	Healing,	hsingle	*/
{ "steamblast",   	SSTEAM, offensive_spell,	3,		WATER, 10,	DAMAGE,		DSINGLE },
{ "breathe-water",  SBRWAT, breathe_water,		3,		WATER, 10,	DURATION,	PBUFF  },
{ "bloodboil",   	SBLOOD, offensive_spell,	4,		WATER, 15,	DAMAGE,		DSINGLE },
/* WATER damage resistance						4,      WATER,  ?? 	?? 			??*/
{ "iceblade",  		SICEBL, offensive_spell,	5,		WATER, 25,	DAMAGE,		DSINGLE },
/* WATER UTILITY SPELL 							5,      WATER,  ?? 	?? 			??*/
{ "flood",   		SFLOOD, room_damage,		6,		WATER, 25,	DAMAGE,		DROOM  },
/* WATER UTILITY SPELL 							6,      WATER,  ?? 	?? 			??*/
/* WATER ATTUNEMENT (doubles water damage)		7,      WATER,  30 	INTENSITY 	PBUFF*/

{ "vigor",   		SVIGOR, vigor, 				1, 		LIFE, 	2, 	HEALING, 	HSINGLE },
{ "mend-wounds",   	SMENDW, mend,				2,		LIFE, 	4, 	HEALING, 	HSINGLE },
{ "curepoison",   	SCUREP, curepoison,			2, 		LIFE,	6,	FIXED,		PBUFF },
{ "room-vigor",   	SRVIGO, room_vigor,			3,		LIFE, 	12,	HEALING, 	HROOM }, 
{ "remove-disease", SRMDIS, rm_disease,			3, 		LIFE,	12,	FIXED,		PBUFF },
{ "cure-blindness", SRMBLD, rm_blind,			4, 		LIFE,	12,	FIXED,		PBUFF },
/*{soothe, (its a better mendwounds)			4,		LIFE, 	16, HEALING, 	HSINGLE	}*/
/*{banishing light, (deals dmg to undead)		5,		LIFE,	20, DAMAGE,		DSINGLE }*/
/*{lifelink, (damage you deal restores hp)		5,		LIFE,	25	INTENSITY,	PBUFF }*/
{ "heal",   		SFHEAL, heal,				6,		LIFE, 	25, HEALING, 	HSINGLE },
/*{revival, restore some hp when dying			7,		LIFE,	30, INTENSITY,	PBUFF }*/


{ "know-aura",   	SKNOWA, know_alignment,		1, ENCHANTMENT,	6,	DURATION,	PBUFF },
{ "bless",   		SBLESS, bless,				2, ENCHANTMENT,	10,	INTENSITY,	PBUFF },
{ "protection",   	SPROTE, protection,			2, ENCHANTMENT,	10,	INTENSITY,	PBUFF },
{ "detect-invisible", SDINVI, detectinvis,		3, ENCHANTMENT,	10,	DURATION,	PBUFF },
{ "detect-magic",   SDMAGI, detectmagic,		3, ENCHANTMENT,	 6,	DURATION,	PBUFF },
/*{detect-relics(relics to be implemented soon) 4, Enchantment, 15, DURATION, 	PBUFF}*/
{ "remove-curse",   SREMOV, remove_curse,		4, ENCHANTMENT,	12, FIXED,		OBUFF },
/* HONED EDGE (deals extra damage every attack) 5, Enchantment, 20, DURATION, 	PBUFF*/
{ "clairvoyance",   SLOCAT, locate_player,		5, ENCHANTMENT, 15, FIXED,		SSPECIAL },
{ "enchant",   		SENCHA, enchant,			6, ENCHANTMENT,	30, DURATION,	OBUFF },
{ "conjure",   		SCONJU, conjure,			6, ENCHANTMENT, 30, FIXED,		SSPECIAL },
/*{DIVINE-WEAPON (makes your weapon amazing)	7, ENCHANTMENT,	30, INTENSITY,	PBUFF }*/

{ "light",   		SLIGHT, light, 				1, 		ARCANA,	5,	DURATION,	PBUFF },
{ "levitate",   	SLEVIT, levitate,			2, 		ARCANA,	10, DURATION,	PBUFF },
/*{MAGIC-MISSILE (generic damage spell)			2, 		ARCANA,	12, DAMAGE,		DSINGLE }*/
{ "fly",   			SFLYSP, fly,				3, 		ARCANA,	10, DURATION,	PBUFF },
/*{ARCANE-MISSILE (generic damage spell)		3, 		ARCANA,	15, DAMAGE,		DSINGLE }*/
{ "invisibility",   SINVIS, invisibility,		4, 		ARCANA,	15, DURATION,	PBUFF },
{ "teleport",   	STELEP, teleport,			4, 		ARCANA,	30, FIXED,		TPORT },
/*{ARCANE-STORM (ROOM DAMAGE)					5, 		ARCANA,	30, DAMAGE,		DROOM }*/
{ "summon",   		SSUMMO, summon,				5, 		ARCANA,	30, FIXED,		TPORT },
{ "track",   		STRACK, magictrack,			6, 		ARCANA,	15, FIXED,		TPORT },
{ "word-of-recall", SRECAL, recall,				6, 		ARCANA,	30, FIXED,		TPORT },
/*{MASS-PORTAL (word-of-recall your whole party)7, 		ARCANA,	35, fixed,		tport }*/


/*{FUMBLE (chance to fumble)					1, 		SORCERY, 5,  INTENSITY,	MBUFF }*/
{ "stun",   		SBEFUD, befuddle,			2,		SORCERY, 10, DURATION,	MBUFF },
{ "fear",   		SFEARS, fear,				2, 		SORCERY, 10, FIXED,		MBUFF},
{ "drain",   		SDREXP, drain_exp,			3, 		SORCERY, 12, INTENSITY, SSPECIAL},
/*{Cripple (some damage and lowers attack speed)3, 		SORCERY, 15, DAMAGE,	MBUFF }*/
{ "blind",   		SBLIND, blind,				4, 		SORCERY, 12, FIXED,		MBUFF},
{ "curse",   		SCURSE, curse,				4, 		SORCERY, 10, FIXED,		MBUFF},
{ "silence",   		SSILNC, silence,			5, 		SORCERY, 10, FIXED, 	MBUFF},
/*{LEECH (drains mana on hit)					5, 		SORCERY, 25, DAMAGE,	DROOM }*/
{ "resist-magic",   SRMAGI, resist_magic,		6,		SORCERY, 15, DURATION,	PBUFF },
/*{Mass-Siphon (spell drain on all targets)		6, 		SORCERY, 30, INTENSITY,	SSPECIAL }*/
/*{Mass-Cripple (cripple on all targets)		7, 		SORCERY, 30, DAMAGE,	SSPECIAL }*/

{ "restore",   		SRESTO, restore,			0,		NOREALM, 0, NOPROF, NOTYPE },

{ "transport",   	STRANO, object_send,		0,		NOREALM, 0, NOPROF, NOTYPE },

{ "fortune",   		SFORTU, fortune,			0,		NOREALM, 12, NOPROF, NOTYPE },

{ "dispel",   		SDISPL, dispel,				0,		NOREALM, 12, NOPROF, NOTYPE },

{ "@",   			-1,		0, 0, NOREALM, 0, NOPROF, NOTYPE }
};

static int spllist_size = sizeof(spllist)/sizeof(*spllist);




static char number[][10] = {
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine",
	"ten",
	"eleven",
	"twelve",
	"thirteen",
	"fourteen",
	"fifteen",
	"sixteen",
	"seventeen",
	"eighteen",
	"nineteen",
	"twenty"
};



static long quest_exp[] = {
	120, 500, 1000, 1000, 1000, 1000, 125, 125,
	1000, 1000, 125, 125, 125, 1000, 2500, 2500,
	500, 5, 5, 5, 125, 125, 125, 125,
	125, 125, 125, 125, 125, 125, 125, 125,
	125, 125, 125, 125, 125, 125, 125, 125,
	125, 125, 125, 125, 125, 125, 125, 125,
	125, 125, 125, 125, 125, 125, 125, 125,
	125, 125, 125, 125, 125, 125, 125, 125,
	125, 125, 125, 125, 125, 125, 125, 125,
	125, 125, 125, 125, 125, 125, 125, 125,
	125, 125, 125, 125, 125, 125, 125, 125,
	125, 125, 125, 125, 125, 125, 125, 125,
	125, 125, 125, 125, 125, 125, 125, 125,
	125, 125, 125, 125, 125, 125, 125, 125,
	125, 125, 125, 125, 125, 125, 125, 125,
	125, 125, 125, 125, 125, 125, 125, 125
};











/**********************************************************************/
/* get_spell_name()												  */
/**********************************************************************/
char *get_spell_name( int nIndex )
{
	/* do bounds checking */
	ASSERTLOG( nIndex >= 0 );
	ASSERTLOG(nIndex < spllist_size );

	nIndex =   MAX( 0, MIN(nIndex, spllist_size ) );

	return( spllist[nIndex].splstr );
}


/**********************************************************************/
/* get_spell_num()												  */
/**********************************************************************/
int get_spell_num( int nIndex )
{
	/* do bounds checking */
	ASSERTLOG( nIndex >= 0 );
	ASSERTLOG(nIndex < spllist_size );

	nIndex =   MAX( 0, MIN(nIndex, spllist_size ) );

	return( spllist[nIndex].splno );
}



/**********************************************************************/
/* get_spell_function()												  */
/**********************************************************************/
SPELLFN get_spell_function( int nIndex )
{
	/* do bounds checking */
	ASSERTLOG( nIndex >= 0 );
	ASSERTLOG(nIndex < spllist_size );

	nIndex =   MAX( 0, MIN(nIndex, spllist_size ) );

	return( spllist[nIndex].splfn );
}


/**********************************************************************/
/* get_spell_list_size()					      */
/**********************************************************************/
int get_spell_list_size()
{
	return( spllist_size );
}



/**********************************************************************/
/* int_to_test()	  												  */
/**********************************************************************/
char *int_to_text( int nNumber )
{
	static	char strNum[10];
	char *strReturn;

	/* check for array bounds */
	if ( nNumber < 21 && nNumber >= 0)
	{
		strReturn = number[nNumber];
	}
	else
	{
		sprintf( strNum, "%d", nNumber );
		strReturn = strNum;
	}

	return(strReturn);
}



/**********************************************************************/
/* get_quest_exp()	  												  */
/**********************************************************************/
long get_quest_exp( int nQuest )
{
	int nMax = sizeof(quest_exp)/sizeof(long);

	/* quests are 1 based and this array is zero based */
	/* so subtract one first */
	nQuest--;

	nQuest = MAX(0, MIN(nQuest, nMax - 1 ) );

	return( quest_exp[nQuest ]);
}



/**********************************************************************/
/* get_spy()	  												  */
/**********************************************************************/
int get_spy( int id )
{
	ASSERTLOG(id >= 0);
	ASSERTLOG(id < PMAX );

	id = MAX(0, MIN(id, PMAX - 1));

	return( Spy[id] );
}


/**********************************************************************/
/* set_spy()	  												  */
/**********************************************************************/
void set_spy( int id, int fd )
{
	ASSERTLOG(id >= 0);
	ASSERTLOG(id < PMAX );

	id = MAX(0, MIN(id, PMAX - 1));

	Spy[id ] = fd;

	return;
}

/**********************************************************************/
/* clear_spy()	  												  */
/**********************************************************************/
void clear_spy()
{
	int	i;

	for( i = 0; i < PMAX; i++) 
		Spy[i] = -1;

	return;
}
