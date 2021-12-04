/*New commands made by Smithy
04/12/2021: Added Alchemist Abilities: Midas, Attune, Distill

*/

#include "../include/mordb.h"
#include "mextern.h"

#include <ctype.h>
#include <string.h>

/* ALCHEMIST */
/* MIDAS 
This ability sets a flag on a monster and upon its death it will drop 5 times its normal gold
The ability also deals damage to the monster based on the alchemist's level, con, int, and enchantment realm prof*/
int midas( creature *ply_ptr, cmd *cmnd )
{
	creature	*crt_ptr;
	room		*rom_ptr;
	time_t		i, t;
	int			chance, m, dmg, fd;

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

	if(t < i) {
		please_wait(fd, i-t);
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
	ply_ptr->lasttime[LT_MIDAS].interval = 500L - (ply_ptr->level - 13)*20L;

  
	/*Chance is based on raw con, player/enemy level, and your enchantment proficiency */
  chance = (int)((ply_ptr->level - crt_ptr->level)*20+ply_ptr->constitution*2+mprofic(ply_ptr, realm[5])/2);

	if(mrand(1,100) > chance) {
		mprint(fd, "You failed to harm %m.\n", m1arg(crt_ptr));
		broadcast_rom(fd, ply_ptr->rom_num, "%M failed to turn %m into gold.\n",
			      m2args(ply_ptr, crt_ptr));
		return(0);
	}

 /*Damage is based on raw con, raw int, player level, and your enchantment proficiency */
	
		dmg = (int)((ply_ptr->level/2)+mprofic(ply_ptr, realm[5])/10))*((ply_ptr->constitution+ply_ptr->intelligence)/15);
		m = MIN(crt_ptr->hpcur, dmg);
		crt_ptr->hpcur -= dmg;
		add_enm_dmg(ply_ptr->name, crt_ptr, m);
    F_SET(crt_ptr, MMIDAS);
  
		sprintf(g_buffer, "Your midas touch hit %%m for %d damage.\n", dmg);
		mprint(fd, g_buffer, m1arg(crt_ptr));

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
      broadcast_rom(fd, ply_ptr->rom_num, "Liquid gold begins to drip from %m, 
				m1arg(crt_ptr));
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

/* DISTILL 
This ability consumes an Alchemist's gold and converts it into MP*/

/* */
/* */
/* */
