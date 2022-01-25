/*
 * ACCESS.C:
 *
 *      This file contains the routines necessary to access arrays
 *
 *      Copyright (C) 1991, 1992, 1993 Brooke Paul
 *      Copyright (C) 1998 John P. Freeman
 * $Id: access.c,v 6.14 2001/04/12 05:00:57 develop Exp $
 *
21/01/2022
Added some more ultility functions that I couldn't put in 
other places due to reasons
and also I'm a bad coder, love Smithy.

09/01/2021:
Added identify spell to spell list

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

{ "vigor",   		SVIGOR, vigor, 				1, 		LIFE, 	2, 	HEALING, 	HSINGLE },

{ "hurt",   		SHURTS, offensive_spell,  	1, 		WIND,  3, 	DAMAGE, 	DSINGLE },

{ "light",   		SLIGHT, light, 				1, 		ARCANA,	5,	DURATION,	PBUFF },

{ "curepoison",   	SCUREP, curepoison,			2, 		LIFE,	6,	FIXED,		PBUFF },

{ "bless",   		SBLESS, bless,				2, ENCHANTMENT,	10,	INTENSITY,	PBUFF },
{ "protection",   	SPROTE, protection,			2, ENCHANTMENT,	10,	INTENSITY,	PBUFF },

{ "fireball",   	SFIREB, offensive_spell,	2,		FIRE, 7,	DAMAGE,		DSINGLE},

{ "invisibility",   SINVIS, invisibility,		4, 		ARCANA,	15, DURATION,	PBUFF },

{ "restore",   		SRESTO, restore,			0,		NOREALM, 0, NOPROF, NOTYPE },

{ "detect-invisible", SDINVI, detectinvis,		3, ENCHANTMENT,	10,	DURATION,	PBUFF },
{ "detect-magic",   SDMAGI, detectmagic,		3, ENCHANTMENT,	 6,	DURATION,	PBUFF },

{ "teleport",   	STELEP, teleport,			4, 		ARCANA,	30, FIXED,		TPORT },

{ "stun",   		SBEFUD, befuddle,			2,		SORCERY, 10, DURATION,	MBUFF },

{ "lightning",   	SLGHTN, offensive_spell, 	4, 		WIND, 15, 	DAMAGE, 	DSINGLE },

{ "iceblade",  		SICEBL, offensive_spell,	5,		WATER, 25,	DAMAGE,		DSINGLE },

{ "enchant",   		SENCHA, enchant,			6, ENCHANTMENT,	30, DURATION,	OBUFF },

{ "word-of-recall", SRECAL, recall,				6, 		ARCANA,	30, FIXED,		TPORT },
{ "summon",   		SSUMMO, summon,				5, 		ARCANA,	30, FIXED,		TPORT },

{ "mend-wounds",   	SMENDW, mend,				2,		LIFE, 	4, 	HEALING, 	HSINGLE },
{ "heal",   		SFHEAL, heal,				6,		LIFE, 	25, HEALING, 	HSINGLE },

{ "track",   		STRACK, magictrack,			6, 		ARCANA,	15, FIXED,		TPORT },
{ "levitate",   	SLEVIT, levitate,			2, 		ARCANA,	10, DURATION,	PBUFF },

{ "resist-fire",  	SRFIRE, resist_fire,		3,		FIRE, 10,	DURATION,	PBUFF },

{ "fly",   			SFLYSP, fly,				3, 		ARCANA,	10, DURATION,	PBUFF },

{ "resist-magic",   SRMAGI, resist_magic,		6,		SORCERY, 15, DURATION,	PBUFF },

{ "shockbolt",   	SSHOCK, offensive_spell, 	3, 		WIND, 10, 	DAMAGE, 	DSINGLE },

{ "rumble",   		SRUMBL, offensive_spell,  	1, 		EARTH, 3, 	DAMAGE, 	DSINGLE  },
{ "burn",   		SBURNS, offensive_spell, 	1,		FIRE, 3,	DAMAGE,		DSINGLE },
{ "blister",   		SBLIST, offensive_spell,	1,		WATER, 3,	DAMAGE,		DSINGLE },
{ "dustgust",   	SDUSTG, offensive_spell, 	2, 		WIND,  7, 	DAMAGE, 	DSINGLE },
{ "waterbolt",   	SWBOLT, offensive_spell,	2,		WATER, 7,	DAMAGE,		DSINGLE },
{ "crush",   		SCRUSH, offensive_spell,  	2, 		EARTH, 7, 	DAMAGE, 	DSINGLE  },
{ "engulf",   		SSHATT, offensive_spell,  	4, 		EARTH, 15, 	DAMAGE, 	DSINGLE  },
{ "burstflame",   	SBURST, offensive_spell,	3,		FIRE, 10,	DAMAGE,		DSINGLE },
{ "steamblast",   	SSTEAM, offensive_spell,	3,		WATER, 10,	DAMAGE,		DSINGLE },
{ "shatterstone",   SENGUL, offensive_spell,  	3, 		EARTH, 10, 	DAMAGE, 	DSINGLE  },
{ "immolate",   	SIMMOL, offensive_spell,	4,		FIRE, 15,	DAMAGE,		DSINGLE },
{ "bloodboil",   	SBLOOD, offensive_spell,	4,		WATER, 15,	DAMAGE,		DSINGLE },
{ "thunderbolt",   	STHUND, offensive_spell, 	5, 		WIND, 25, 	DAMAGE, 	DSINGLE },
{ "earthquake",   	SEQUAK, offensive_spell,  	5, 		EARTH, 25, 	DAMAGE, 	DSINGLE  },
{ "flamefill",   	SFLFIL, offensive_spell,	5,		FIRE, 25,	DAMAGE,		DSINGLE },

{ "know-aura",   	SKNOWA, know_alignment,		1, ENCHANTMENT,	6,	DURATION,	PBUFF },
{ "remove-curse",   SREMOV, remove_curse,		4, ENCHANTMENT,	12, FIXED,		OBUFF },

{ "resist-cold",   	SRCOLD, resist_cold, 		3, 		WIND, 10, 	DURATION, 	PBUFF },
{ "breathe-water",  SBRWAT, breathe_water,		3,		WATER, 10,	DURATION,	PBUFF  },
{ "resist-earth",   SSSHLD, earth_shield,  	  	3, 		EARTH, 10, 	DURATION, 	PBUFF  },

{ "clairvoyance",   SLOCAT, locate_player,		5, ENCHANTMENT, 15, FIXED,		SSPECIAL },

{ "drain",   		SDREXP, drain_exp,			3, 		SORCERY, 12, INTENSITY, SSPECIAL},

{ "remove-disease", SRMDIS, rm_disease,			3, 		LIFE,	12,	FIXED,		PBUFF },
{ "cure-blindness", SRMBLD, rm_blind,			4, 		LIFE,	12,	FIXED,		PBUFF },

{ "fear",   		SFEARS, fear,				2, 		SORCERY, 10, FIXED,		MBUFF},

{ "room-vigor",   	SRVIGO, room_vigor,			3,		LIFE, 	12,	HEALING, 	HROOM }, 

{ "transport",   	STRANO, object_send,		0,		NOREALM, 0, NOPROF, NOTYPE },

{ "blind",   		SBLIND, blind,				4, 		SORCERY, 12, FIXED,		MBUFF},
{ "silence",   		SSILNC, silence,			5, 		SORCERY, 10, FIXED, 	MBUFF},

{ "fortune",   		SFORTU, fortune,			0,		NOREALM, 12, NOPROF, NOTYPE },
{ "conjure",   		SCONJU, conjure,			6, ENCHANTMENT, 30, FIXED,		SSPECIAL },
{ "curse",   		SCURSE, curse,				4, 		SORCERY, 10, FIXED,		MBUFF},
{ "dispel",   		SDISPL, dispel,				0,		NOREALM, 12, NOPROF, NOTYPE },

{ "tornado",   		STORNA, room_damage, 		6, 		WIND, 25, 	DAMAGE, 	DROOM },
{ "incinerate",   	SINCIN, room_damage,		6,		FIRE, 25,	DAMAGE,		DROOM },
{ "tremor",   		STRMOR, room_damage,  		6, 		EARTH, 25, 	DAMAGE, 	DROOM  },
{ "flood",   		SFLOOD, room_damage,		6,		WATER, 25,	DAMAGE,		DROOM  },

{ "identify",		SIDENTIFY, identify,		0,		NOREALM, 1, NOPROF,	NOTYPE },

/* EARTH UTILITY SPELL, 						2,      EARTH, ?? 	?? 			??*/
/* RESIST EARTH, 								4,      EARTH, 15 	INTENSITY 	PBUFF*/
/* EARTH UTILITY SPELL 							5,      EARTH, ?? 	?? 			??*/
/* EARTH UTILITY SPELL 							6,      EARTH, ?? 	?? 			??*/
/* EARTH ATTUNEMENT (doubles earth damage)		7,      EARTH, 30 	INTENSITY 	PBUFF*/

/* WIND UTILITY SPELL 							2,      WIND,   ?? ?? ??*/
/* RESIST WIND 									4,      WIND,   15 	INTENSITY 	PBUFF*/
/* WIND UTILITY SPELL 							5,      WIND, ?? ?? ??*/
/* WIND UTILITY SPELL 							6,      WIND, ?? ?? ??*/
/* WIND ATTUNEMENT (doubles wind damage)		7,      WIND, 30 	INTENSITY 	PBUFF*/

/* FIRE UTILITY SPELL 							2,      FIRE,  ?? 	?? 			??*/
/* FIRE DAMAGE RESISTANCE						4,      FIRE,  ?? 	?? 			??*/
/* FIRE UTILITY SPELL 							5,      FIRE,  ?? 	?? 			??*/
/* FIRE UTILITY SPELL 							6,      FIRE,  ?? 	?? 			??*/
/* FIRE ATTUNEMENT (doubles fire damage)		7,      FIRE,  30 	INTENSITY 	PBUFF*/

/* cleanse, (a water healing spell)				2,      WATER, 8,	Healing,	hsingle	*/
/* WATER damage resistance						4,      WATER,  ?? 	?? 			??*/
/* WATER UTILITY SPELL 							5,      WATER,  ?? 	?? 			??*/
/* WATER UTILITY SPELL 							6,      WATER,  ?? 	?? 			??*/
/* WATER ATTUNEMENT (doubles water damage)		7,      WATER,  30 	INTENSITY 	PBUFF*/

/*{soothe, (its a better mendwounds)			4,		LIFE, 	16, HEALING, 	HSINGLE	}*/
/*{banishing light, (deals dmg to undead)		5,		LIFE,	20, DAMAGE,		DSINGLE }*/
/*{lifelink, (damage you deal restores hp)		5,		LIFE,	25	INTENSITY,	PBUFF }*/
/*{revival, restore some hp when dying			7,		LIFE,	30, INTENSITY,	PBUFF }*/

/*{detect-relics(relics to be implemented soon) 4, Enchantment, 15, DURATION, 	PBUFF}*/
/* HONED EDGE (deals extra damage every attack) 5, Enchantment, 20, DURATION, 	PBUFF*/
/*{DIVINE-WEAPON (makes your weapon amazing)	7, ENCHANTMENT,	30, INTENSITY,	PBUFF }*/

/*{MAGIC-MISSILE (generic damage spell)			2, 		ARCANA,	12, DAMAGE,		DSINGLE }*/
/*{ARCANE-MISSILE (generic damage spell)		3, 		ARCANA,	15, DAMAGE,		DSINGLE }*/
/*{ARCANE-STORM (ROOM DAMAGE)					5, 		ARCANA,	30, DAMAGE,		DROOM }*/
/*{MASS-PORTAL (word-of-recall your whole party)7, 		ARCANA,	35, fixed,		tport }*/

/*{FUMBLE (chance to fumble)					1, 		SORCERY, 5,  INTENSITY,	MBUFF }*/
/*{Cripple (some damage and lowers attack speed)3, 		SORCERY, 15, DAMAGE,	MBUFF }*/
/*{LEECH (drains mana on hit)					5, 		SORCERY, 25, DAMAGE,	DROOM }*/
/*{Mass-Siphon (spell drain on all targets)		6, 		SORCERY, 30, INTENSITY,	SSPECIAL }*/
/*{Mass-Cripple (cripple on all targets)		7, 		SORCERY, 30, DAMAGE,	SSPECIAL }*/

{ "@",   			-1,		0, 0, NOREALM, 0, NOPROF, NOTYPE }
};

static int spllist_size = sizeof(spllist)/sizeof(*spllist);

int get_spell_number(char *spellname){
	int c=0, match=0, splno;

	do {
        if(!strcmp(spellname, get_spell_name(c))) {
            match = 1;
            splno = c;
            return(c);
            break;
        }
        else if(!strncmp(spellname, get_spell_name(c), 
            strlen(spellname))) {
            match++;
            splno = c;
            return(c);
        }
        c++;
    } while(get_spell_num(c) != -1);

}

int spell_damage_type(int spellnumber){
	int temp, type;
	temp = spllist[spellnumber].realm;

	if (temp == EARTH){
		type = DEARTH;
	}
	if (temp == WIND){
		type = DWIND;
	}
	if (temp == FIRE){
		type = DFIRE;
	}
	if (temp == WATER){
		type = DWATER;
	}
	if (temp > WATER){
		type = DMAGIC;
	}

	/*ADD EXTRA CUSTOM CODE HERE*/
	return(type);
}


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
