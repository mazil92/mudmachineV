/*New commands made by Smithy
22/01/2022: envenom

18/12/2021: time_output
get_cmd_number
cmdlist_size
skills
canuse

21/12/2021: completed skill command
also added: canuse, time_output, get_skill_number; etc. for skills command to use

13/12/2021: Added checking function CAN_USE

04/12/2021: Added Alchemist Abilities: Midas, Attune, Distill

*/

#include "../include/mordb.h"
#include "mextern.h"

#include <ctype.h>
#include <string.h>

/*ASSASSIN*/

/*ENVENOM
This ability sets a flag on a player to make their next successful hit with a weapon deal 
a strike as though the weapon had +100% poison damage*/
int envenom(creature *ply_ptr, cmd *cmnd){
	time_t		i, t;
	int			chance, m, dmg, fd;
	room		*rom_ptr;

	fd = ply_ptr->fd;
	rom_ptr = ply_ptr->parent_rom;

	
	if(ply_ptr->class != ASSASSIN && ply_ptr->class < BUILDER) {
		output(fd, "Only assassins may use that skill.\n");
		return(0);
	}
	if(ply_ptr->level < 13) {
		output(fd, "Requires level 13!\n");
		return(0);
	}
	
	  
    if(F_ISSET(ply_ptr, PENVENOM)) {
		output(fd, "Your weapon is already dripping with poison!\n");
		    return(0);
	  }


	t = time(0);
	i = LT(ply_ptr, LT_ENVENOM);

	if(t - i < 60L) {
		please_wait(fd, 60L-t+i);
		return(0);
	}
	
	ply_ptr->lasttime[LT_ENVENOM].ltime = t;
	ply_ptr->lasttime[LT_ATTCK].ltime = t;
  
	/*Chance is based on raw dex, player level, and your enchantment proficiency */
    chance = (int)((ply_ptr->level )*20+ply_ptr->dexterity*2+mprofic(ply_ptr, 6)/2);

	if(mrand(1,100) > chance) {
		output(fd, "You failed to apply the poison\n");
		broadcast_rom(fd, ply_ptr->rom_num, "%M failed to poison their weapon.\n",
			      m1arg(ply_ptr));
		return(0);
	}

	output(fd, "Your next strike will be venemous!\n");
		broadcast_rom(fd, ply_ptr->rom_num, "%M applied venom to their weapon.\n",
			      m1arg(ply_ptr));
	F_SET(ply_ptr, PENVENOM);

	return(0);
}

/* ALCHEMIST */
/* MIDAS 
This ability sets a flag on a monster and upon its death it will drop 5 times its normal gold
The ability also deals damage to the monster based on the alchemist's level, con, int, and enchantment realm prof*/
int midas( creature *ply_ptr, cmd *cmnd )
{
	creature	*crt_ptr;
	room		*rom_ptr;
	time_t		i, t;
	int			chance, m, dmg, fd, damage_type;
	float temp1;


	fd = ply_ptr->fd;
	rom_ptr = ply_ptr->parent_rom;

	if(cmnd->num < 2) {
		output(fd, "Midas whom?\n");
		return(0);
	}

	if(ply_ptr->class != ALCHEMIST && ply_ptr->class < BUILDER) {
		output(fd, "Only alchemists may use that skill.\n");
		return(0);
	}
	if(ply_ptr->level < 13) {
		output(fd, "Requires level 13!\n");
		return(0);
	}
	
	crt_ptr = find_crt_in_rom(ply_ptr, rom_ptr, cmnd->str[1], 
		cmnd->val[1], MON_FIRST);

	if(!crt_ptr) {
		output(fd, "That is not here.\n");
		return(0);
	}


	if ( crt_ptr->type == PLAYER )
	{
		output(fd, "You can't bring yourself to do that.\n");
			return(0);
	}

    if(F_ISSET(ply_ptr, PBLIND)) {
		output(fd, "How do you do that?  You're blind.\n");
        return(0);
    }
  
    if(F_ISSET(crt_ptr, MMIDAS)) {
		output(fd, "That creature is already made of gold!\n");
		    return(0);
	  }

	if(F_ISSET(ply_ptr, PINVIS)) {
		F_CLR(ply_ptr, PINVIS);
		output(fd, "Your invisibility fades.\n");
		broadcast_rom(fd, ply_ptr->rom_num, "%M fades into view.",
			      m1arg(ply_ptr));
	}

	t = time(0);
	i = LT(ply_ptr, LT_MIDAS);

	if(t - i < 120L) {
		please_wait(fd, 120L-t+i);
		return(0);
	}

	if(!is_crt_killable(crt_ptr, ply_ptr)) 
	{
		return(0);
	}
	
	if(crt_ptr->type == MONSTER)
		add_enm_crt(ply_ptr->name, crt_ptr);

	ply_ptr->lasttime[LT_MIDAS].ltime = t;
	ply_ptr->lasttime[LT_ATTCK].ltime = t;
	ply_ptr->lasttime[LT_MIDAS].interval = 200L;

  
	/*Chance is based on raw con, player/enemy level, and your enchantment proficiency */
  chance = (int)((ply_ptr->level - crt_ptr->level)*20+ply_ptr->constitution*2+mprofic(ply_ptr, 6)/2);

	if(mrand(1,100) > chance) {
		mprint(fd, "You failed to harm %m.\n", m1arg(crt_ptr));
		broadcast_rom(fd, ply_ptr->rom_num, "%M failed to turn %m into gold.\n",
			      m2args(ply_ptr, crt_ptr));
		return(0);
	}

 /*Damage is based on raw con, raw int, player level, and your enchantment proficiency */
	
		damage_type = DMAGIC;
		dmg = ((int)((ply_ptr->level/2)+mprofic(ply_ptr, 6)/10)*((ply_ptr->constitution+ply_ptr->intelligence)/15));
		temp1 = calc_damage((float)dmg, (float)compute_DR(crt_ptr), (float)compute_resistance(crt_ptr, damage_type), (float)armor_confidence(crt_ptr));
		dmg = (int)temp1;
		m = MIN(crt_ptr->hpcur, dmg);
		crt_ptr->hpcur -= dmg;
		add_enm_dmg(ply_ptr->name, crt_ptr, m);
    	F_SET(crt_ptr, MMIDAS);
  
		sprintf(g_buffer, "Your midas touch hit %%m for %d damage.\n", dmg);
		mprint(fd, g_buffer, m1arg(crt_ptr));
		damage_outputter(ply_ptr, dmg, damage_type);
		if ( crt_ptr == PLAYER )
		{
			mprint(crt_ptr->fd, "%M uses the touch of death on you!", m1arg(ply_ptr) );
			broadcast_rom2(fd, crt_ptr->fd, ply_ptr->rom_num, "%M uses the touch of death on %m.", 
				m2args(ply_ptr, crt_ptr));
		}
		else
		{
			broadcast_rom(fd, ply_ptr->rom_num, "%M uses their midas touch on %m.", 
				m2args(ply_ptr, crt_ptr));
      
      ANSI(fd, AFC_YELLOW);
		  ANSI(fd, AM_BOLD);
      broadcast_rom(fd, ply_ptr->rom_num, "Liquid gold begins to drip from %m.", m1arg(crt_ptr));
      
      mprint(fd, "Liquid gold begins to drip from %m.", m1arg(crt_ptr));
      
      ANSI(fd, AM_NORMAL);
		  ANSI(fd, MAINTEXTCOLOR);
		}

		if(crt_ptr->hpcur < 1) {
			mprint(fd, "You killed %m.\n", m1arg(crt_ptr));
			broadcast_rom(fd, ply_ptr->rom_num, "%M killed %m.",
				      m2args(ply_ptr, crt_ptr));
			die(crt_ptr, ply_ptr);
		}
	

	return(0);

}

/* ATTUNE
This ability sets a flag on alchemists enabling them to have a cast interval of 2 seconds*/
int attune( creature *ply_ptr, cmd *cmnd ){
	time_t		i, t;
	int			chance, m, dmg, fd, damage_type;

	fd = ply_ptr->fd;
	if(ply_ptr->class != ALCHEMIST && ply_ptr->class < BUILDER) {
		output(fd, "Only alchemists may use that skill.\n");
		return(0);
	}
	if(ply_ptr->level < 1) {
		output(fd, "Requires level 1!\n");
		return(0);
	}
	if(ply_ptr->mpcur < 15){
		output(fd, "You need 15 mana to attune!\n");
		return(0);
	}
	if(F_ISSET(ply_ptr, PATTUNE)) {
		output(fd, "You're already attuned!\n");
		return(0);
	}
	t = time(0);
	i = LT(ply_ptr, LT_ATTUNE);

	if(t - i < 600L) {
		please_wait(fd, 600L-t+i);
		return(0);
	}
	/*Chance is based on raw con, player/enemy level, and your enchantment proficiency */
  	chance = (int)((ply_ptr->level)*20+ply_ptr->constitution*2+mprofic(ply_ptr, 6)/2);

  	ply_ptr-> mpcur -= 15;
	if(mrand(1,100) > chance) {
		output(fd, "You failed to attune\n");
		broadcast_rom(fd, ply_ptr->rom_num, "%M failed to attune their magic.\n",
			      m1arg(ply_ptr));
		return(0);
	}
	else {
		output(fd, "You feel magical energy surge through you!\n");
		broadcast_rom(fd, ply_ptr->rom_num, "%M attunes their magic with frenetic energy!\n",
			      m1arg(ply_ptr));
		ply_ptr->intelligence += 5;
		F_SET(ply_ptr, PATTUNE);
		ply_ptr->lasttime[LT_ATTUNE].ltime = t;
		/*ply_ptr->lasttime[LT_ATTUNE].interval = 240L + 60L*(ply_ptr->level/5);*/
		ply_ptr->lasttime[LT_ATTUNE].interval = 30L;
		return(0);
	}



return(0);
}

/* DISTILL 
This ability consumes an Alchemist's gold and converts it into MP*/
int distill( creature *ply_ptr, cmd *cmnd ){
	time_t		i, t;
	int			chance, m, dmg, fd, damage_type;	
	

	if(ply_ptr->class != ALCHEMIST && ply_ptr->class < BUILDER) {
		output(fd, "Only alchemists may use that skill.\n");
		return(0);
	}
	if(ply_ptr->level < 1) {
		output(fd, "Requires level 1!\n");
		return(0);
	}
}

/* CAN_USE
This function checks to see if the player can use the ability fed to the function.
Though, this function will not check an exhaustive list of possibilities
It will check:

-class requirements
-level requirements
-cooldown requirements*/

int can_use ( creature *ply_ptr, int cmdno )
{
	creature	*crt_ptr;
	room		*rom_ptr;
	time_t		i, t;
	int			fd, lt;
	char		cmdname[100]; 

fd = ply_ptr->fd;

/*will probably have to define integers for the skill numbers but oh well


/* CHECK CLASS*/


if (ply_ptr->class < BUILDER){
	
	switch(cmdno){
		/*track*/
		
		case 60: 
		if (ply_ptr->class != RANGER && ply_ptr->class != DRUID ){
			
			return(WRONGCLASS);
		}
		/*peek*/
		case 61:
		if (ply_ptr->class != ASSASSIN && ply_ptr->class != THIEF ){
			return(WRONGCLASS);
		}
		/*pick*/
		case 79:
		if (ply_ptr->class != ASSASSIN && ply_ptr->class != THIEF ){
			return(WRONGCLASS);
		}
		/*steal*/
		case 80:
		if (ply_ptr->class != THIEF){
			return(WRONGCLASS);
		}
		/*backstab*/
		case 97:
		if (ply_ptr->class != ASSASSIN && ply_ptr->class != THIEF ){
			return(WRONGCLASS);
		}
		/*circle*/
		case 103:
		if (ply_ptr->class != BARBARIAN && ply_ptr->class != FIGHTER ){
			return(WRONGCLASS);
		}
		/*bash*/
		case 104:
		if (ply_ptr->class != BARBARIAN && ply_ptr->class != FIGHTER ){
			return(WRONGCLASS);
		}
		/*turn*/
		case 118:
		if (ply_ptr->class != PALADIN && ply_ptr->class != CLERIC ){
			return(WRONGCLASS);
		}
		/*haste*/
		case 120:
		if (ply_ptr->class != RANGER ){
			return(WRONGCLASS);
		}
		/*pray*/
		case 121:
		if (ply_ptr->class != PALADIN && ply_ptr->class != CLERIC ){
			return(WRONGCLASS);
		}
		/*bard_song*/
		case 100:
		if (ply_ptr->class != BARD ){
			return(WRONGCLASS);
		}
		/*bard_song2*/
		case 81:
		if (ply_ptr->class != BARD ){
			return(WRONGCLASS);
		}
		/*meditate*/
		case 139:
		if (ply_ptr->class != MONK ){
			return(WRONGCLASS);
		}
		/*touch_of_death*/
		case 140:
		if (ply_ptr->class != MONK){
			return(WRONGCLASS);
		}
		/*transform*/
		case 141:
		if (ply_ptr->class != DRUID ){
			return(WRONGCLASS);
		}
		/*rechard_wand*/
		case 146:
		if (ply_ptr->class != ALCHEMIST ){
			return(WRONGCLASS);
		}
		/*scout*/
		case 150:
		if (ply_ptr->class != RANGER && ply_ptr->class != DRUID && ply_ptr->class != ASSASSIN && ply_ptr->class != THIEF ){
			return(WRONGCLASS);
		}
		/*transmute*/
		case 151:
		if (ply_ptr->class != DRUID ){
			return(WRONGCLASS);
		}
		/*enchant*/
		case 148:
		if (ply_ptr->class != MAGE && ply_ptr->class != ALCHEMIST ){
			return(WRONGCLASS);
		}
		/*berserk*/
		case 152:
		if (ply_ptr->class != BARBARIAN ){
			return(WRONGCLASS);
		}
		/*barkskin*/
		case 154:
		if (ply_ptr->class != DRUID ){
			return(WRONGCLASS);
		}
		/*midas*/
		case 398:
		if (ply_ptr->class != ALCHEMIST ){
			return(WRONGCLASS);
		}
		/*attune*/
		
		case 402:
		if (ply_ptr->class != ALCHEMIST ){
			return(WRONGCLASS);
		}
		/*distill*/
		
		case 403:
		if (ply_ptr->class != ALCHEMIST ){
			return(WRONGCLASS);
		}
	}


/* CHECK LEVEL*/
if (ply_ptr->level < cmdlist[cmdno].level){
	return(LOWLEVEL);
}

/* CHECK COOLDOWN*/
lt = cmdlist[cmdno].lt;
if (lt > 0){

	t = time(0);
	i = ply_ptr->lasttime[lt].ltime;

	if (t-i < cmdlist[cmdno].cooldown){
		return(ONCOOLDOWN);
	}
}


}

/*if the function gets this far, the player may use it*/
return(CANUSE);

}
/* SKILLS
This function reports to a player the skills they have available to them and their cooldowns*/
int cmdlist_size(){
	int size;

	size = sizeof(*cmdlist)/sizeof(cmdlist[0]);
	return(size);
} 
 

int skills (creature *ply_ptr, cmd *cmnd)
{
	char	alstr[8]; 
	char 	str[2048];
	char	skll[128][30];
	char	cldwn[128][30];
	int 	fd, cnt, i, j, lt, cmdno;
	long	duration, lv;
	time_t  tt, ti, cd;
	cmd 	tempcmd; 

	
	fd = ply_ptr->fd;
	
memset(str,0,strlen(str));
strcpy (str, "Skills available: ");

for(i=0,j=0; i< 400; i++){
	
	
	/*elog("343");*/
	/*sprintf(g_buffer, "%i, %s, %i", i, cmdlist[i].cmdstr, can_use(ply_ptr, i));
	
	output_nl(fd, g_buffer);*/
	
	/*only consider skills that I want to see
	for this, I use a distinction between level 1 and 0 on the cmdlist*/
	if (cmdlist[i].level > 0){
	if((can_use(ply_ptr, i) == CANUSE )|| (can_use(ply_ptr, i) == ONCOOLDOWN )){
		/*elog("346");*/
		/*elog(g_buffer);*/
		strcpy(skll[j], cmdlist[i].cmdstr);
		strcpy(cldwn[j], ", ");
		/*cooldown stuff here*/
		/*elog("353");*/
		lt = cmdlist[i].lt;
		if (lt > 0){
			tt = time(0);
			ti = ply_ptr->lasttime[lt].ltime;
			cd = cmdlist[i].cooldown;
			strcpy(cldwn[j], "[");
			if ((cd-tt+ti) > 0){
				sprintf(g_buffer, "%s", time_output(cd-tt+ti));
			}
			else {
				sprintf(g_buffer, "ready");
			}
			strcat(cldwn[j], g_buffer);		
			strcat(cldwn[j], "]");
			
		}
		else {
			strcpy(cldwn[j], "[ready]");
		}
		strcat(cldwn[j], ", ");
		/*elog("355");*/
		j++;
	}
	}
}
/*elog("366");*/
if(!j){
	strcpy(str, "None.");
}
else {
	/* qsort((void *)skll, j, 20, (PFNCOMPARE)strcmp); */
	for(i=0; i<j; i++) {
		strcat(str, skll[i]);
		strcat(str, cldwn[i]);
		
	}
	/*str[strlen(str)-2] = '.';
	str[strlen(str)-1] = 0;*/
}
/*elog("385");*/
output_nl(fd, str);

/*elog("389");*/
/*F_CLR(Ply[fd], ply_ptr);*/
RETURN(fd, command, 1);
}


/* get_cmd_number*/
int get_cmd_number(cmd *cmnd){

	int	match=0, cmdno=0, c=0;

	do {
		if(!strcmp(cmnd->str[0], cmdlist[c].cmdstr)) {
			match = 1;
			cmdno = c;
			return (cmdno);
			break;
		}
		else if(!strncmp(cmnd->str[0], cmdlist[c].cmdstr, 
			strlen(cmnd->str[0]))) {
			match++;
			cmdno = c;
			return (cmdno);
		}
		c++;
	} while(cmdlist[c].cmdno);

	return (0);
}

/* time output *********************************
 this is the time output itended for use in the skill command output*/
char *time_output(time_t duration ){
if(duration > 60L) {
	sprintf(g_buffer, "%ld:%02ld", (long)duration/60L, (long)duration%60L);
		}
else if(duration < 10L){
	sprintf(g_buffer, "00:0%ld", (long)duration);
}
else if(duration == 0L){
	sprintf(g_buffer, "00:00");
}
else{
			sprintf(g_buffer,"00:%ld", (long)duration);
		}

return(g_buffer );
}