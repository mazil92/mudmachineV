/*
26/03/2022
NEW ADDITIONS BY SMITHY
ADDED FUNCTIONALITY TO TALKING
NOW WITH CONDITIONAL TALK STRINGS

 * COMMAND8.C:
 *
 *	Additional user routines.
 *
 *	Copyright (C) 1991, 1992, 1993 Brooke Paul
 *
 24/01/2022:
 added the proper weapon damage to bash
 * 

 $Id: command8.c,v 6.18 2001/07/08 20:28:02 develop Exp $
 *
 * $Log: command8.c,v $
 * Revision 6.18  2001/07/08 20:28:02  develop
 * *** empty log message ***
 *
 * Revision 6.17  2001/07/08 20:23:28  develop
 * blocked giving of items to perms
 *
 * Revision 6.16  2001/03/09 05:14:07  develop
 * changes to weight checking
 *
 * Revision 6.15  2001/03/08 16:09:09  develop
 * *** empty log message ***
 *
 */

#include "../include/mordb.h"
#include "mextern.h"


/**********************************************************************/
/*				give				      */
/**********************************************************************/

/* This function allows a player to give an item in his inventory to */
/* another player or monster.					     */

int give( creature *ply_ptr, cmd *cmnd )
{
	object		*obj_ptr;
	creature	*crt_ptr;
	room		*rom_ptr;
	int		fd;

	fd = ply_ptr->fd;
	rom_ptr = ply_ptr->parent_rom;

	if(cmnd->num < 3) {
		output(fd, "Give what to whom?\n");
		return(0);
	}

	if(cmnd->str[1][0] == '$') {
		give_money(ply_ptr, cmnd);
		return(0);
	}

	obj_ptr = find_obj(ply_ptr, ply_ptr->first_obj,
			   cmnd->str[1], cmnd->val[1]);

	if(!obj_ptr) {
		output(fd, "You don't have that.\n");
		return(0);
	}

	F_CLR(ply_ptr, PHIDDN);

	crt_ptr = find_crt_in_rom(ply_ptr, rom_ptr,
			   cmnd->str[2], cmnd->val[2], PLY_FIRST);

	if(!crt_ptr) {
		output(fd, "I don't see that person here.\n");
		return(0);
	}

	if(crt_ptr == ply_ptr) {
		output(fd, "You can't give something to yourself.\n");
		return(0);
	}

	if(obj_ptr->questnum) {
		output(fd, "You can't give away a quest object.\n");
		return(0);
	}

	/* dont' allow giving to perms. Players like to load inventories
		of perms for some reason */
	if(ply_ptr->class < IMMORTAL && crt_ptr->type == MONSTER && F_ISSET(crt_ptr, MPERMT)) {
		output(fd, "You can't give that to them.\n");
                return(0);
        }

	if(crt_ptr->type == PLAYER && 
	    ( weight_ply(crt_ptr) + weight_obj(obj_ptr) > max_weight(crt_ptr) ))
	{
		sprintf(g_buffer, "%s can't hold anymore.\n", crt_ptr->name);
		output(fd, g_buffer );
		return(0);
	}

	del_obj_crt(obj_ptr, ply_ptr);
	add_obj_crt(obj_ptr, crt_ptr);

	if(F_ISSET(obj_ptr, OSTOLE))
	{	
		F_CLR(obj_ptr, OSTOLE);
		sprintf(g_buffer, "The %s is no longer marked as being stolen.\n", obj_ptr->name);
		output(fd, g_buffer);
	}
	
	mprint(fd, "You give %1i to %m.\n", m2args(obj_ptr, crt_ptr));
	mprint(crt_ptr->fd, "%M gave %1i to you.\n", m2args(ply_ptr, obj_ptr));
	broadcast_rom2(fd, crt_ptr->fd, ply_ptr->rom_num, "%M gave %1i to %m.",
		       m3args(ply_ptr, obj_ptr, crt_ptr));

   if(SAVEONDROP) {
               if(ply_ptr->type == PLAYER) {
	 		F_SET(ply_ptr, PSAVES);
			save_ply(ply_ptr);
	 		F_CLR(ply_ptr, PSAVES);
		}
    }
	return(0);

}

/**********************************************************************/
/*				give_money			      */
/**********************************************************************/

/* This function allows a player to give gold to another player.  Gold */
/* is interpreted as gold if it is preceded by a dollar sign.	       */

void give_money( creature *ply_ptr, cmd *cmnd )
{
	creature	*crt_ptr;
	room		*rom_ptr;
	long		amt;
	int		fd;

	rom_ptr = ply_ptr->parent_rom;
	fd = ply_ptr->fd;

	amt = atol(&cmnd->str[1][1]);
	if(amt < 1) {
		output(fd, "Get real.\n");
		return;
	}
	if(amt > ply_ptr->gold) {
		output(fd, "You don't have that much gold.\n");
		return;
	}

	crt_ptr = find_crt_in_rom(ply_ptr, rom_ptr,
			   cmnd->str[2], cmnd->val[2], PLY_FIRST);


	if(!crt_ptr || crt_ptr == ply_ptr) {
		output(fd, "That person is not here.\n");
		return;
	}

	crt_ptr->gold += amt;
	ply_ptr->gold -= amt;

	output(fd, "Ok.\n");
	
	sprintf(g_buffer, "%%M gave you %ld gold pieces.\n", amt);
	mprint(crt_ptr->fd, g_buffer, m1arg(ply_ptr));

	sprintf(g_buffer, "%%M gave %%m %ld gold pieces.", amt);
	broadcast_rom2(fd, crt_ptr->fd, ply_ptr->rom_num, 
		       g_buffer, m2args(ply_ptr, crt_ptr));

   if(SAVEONDROP) {
               if(ply_ptr->type == PLAYER) {
			F_SET(ply_ptr, PSAVES);
			save_ply(ply_ptr);
			F_CLR(ply_ptr, PSAVES);
		}
   }




}

/**********************************************************************/
/*				repair				      */
/**********************************************************************/

/* This function allows a player to repair a weapon or armor if he is */
/* in a repair shoppe.  There is a chance of breakage.		      */

int repair( creature *ply_ptr, cmd *cmnd )
{
	object	*obj_ptr;
	room	*rom_ptr;
	long	cost;
	int	fd, broke;

	fd = ply_ptr->fd;
	rom_ptr = ply_ptr->parent_rom;

	if(cmnd->num < 2) {
		output(fd, "Repair what?\n");
		return(0);
	}

	if(!F_ISSET(rom_ptr, RREPAI)) {
		output(fd, "This is not a repair shop.\n");
		return(0);
	}

	obj_ptr = find_obj(ply_ptr, ply_ptr->first_obj,
			   cmnd->str[1], cmnd->val[1]);

	if(!obj_ptr) {
		output(fd, "You don't have that.\n");
		return(0);
	}

	F_CLR(ply_ptr, PHIDDN);

	if(F_ISSET(obj_ptr, ONOFIX)) {
		output(fd, "The smithy cannot repair that.\n");
		return(0);
	}

	if(obj_ptr->type > MISSILE && obj_ptr->type != ARMOR) {
		output(fd, "You can only repair weapons and armor.\n");
		return(0);
	}

	if(obj_ptr->shotscur > MAX(3, obj_ptr->shotsmax/10)) {
		output(fd, "It's not broken yet.\n");
		return(0);
	}

	cost = obj_ptr->value / 2;

	if(ply_ptr->gold < cost) {
		output(fd, "You don't have enough money.\n");
		return(0);
	}

	sprintf(g_buffer, "The smithy takes %ld gold pieces from you.\n", cost);
	output(fd, g_buffer );
	ply_ptr->gold -= cost;

	broadcast_rom(fd, ply_ptr->rom_num, "%M has the smithy repair %1i.",
		      m2args(ply_ptr, obj_ptr));

	broke = mrand(1,100) + bonus[(int)ply_ptr->piety];
	if((broke <= 15 && obj_ptr->shotscur < 1) ||
	   (broke <= 5 && obj_ptr->shotscur > 0)) {
		sprintf(g_buffer, "\"Darnitall!\" shouts the smithy, \"I broke another. Sorry %s.\"\n", 
			F_ISSET(ply_ptr, PMALES) ? "lad":"lass");
		output(fd, g_buffer );
		broadcast_rom(fd, ply_ptr->rom_num, "Oops!  He broke it.", NULL);
		if(obj_ptr->shotscur > 0) {
			output(fd, "He gives your money back.\n");
			ply_ptr->gold += cost;
		}
		del_obj_crt(obj_ptr, ply_ptr);
		free_obj(obj_ptr);
		return(0);
	}

	if((F_ISSET(obj_ptr, OENCHA) || obj_ptr->adjustment) &&
		mrand(1,50) > ply_ptr->piety) {
		output(fd, "\"It lost that nice glow,\" says the smithy.\n");
		if(obj_ptr->type == ARMOR && obj_ptr->wearflag == BODY)
			obj_ptr->armor =
				MAX(obj_ptr->armor - obj_ptr->adjustment*2, 0);
		else if(obj_ptr->type == ARMOR)
			obj_ptr->armor =
				MAX(obj_ptr->armor - obj_ptr->adjustment, 0);
		else if(obj_ptr->type <= MISSILE) {
			obj_ptr->shotsmax -= obj_ptr->adjustment*10;
			obj_ptr->pdice =
				MAX(obj_ptr->pdice - obj_ptr->adjustment, 0);
		}
		obj_ptr->adjustment = 0;
		F_CLR(obj_ptr, OENCHA);
	}

	obj_ptr->shotscur = (obj_ptr->shotsmax * mrand(5,9)) / 10;

	mprint(fd, "The smithy hands %i back to you, almost good as new.\n", 
		m1arg(obj_ptr));

	return(0);

}

/**********************************************************************/
/*				prt_time			      */
/**********************************************************************/

/* This function outputs the time of day (realtime) to the player.    */

int prt_time( creature *ply_ptr, cmd *cmnd  )
{
	int	fd, daytime;

	fd = ply_ptr->fd;

	daytime = (int)(Time % 24L);
	sprintf(g_buffer, "Game-Time: %d o'clock %s.\n", daytime%12 == 0 ? 12 :
	      daytime%12, daytime > 11 ? "PM":"AM");
	output(fd, g_buffer );

	sprintf(g_buffer, "Real-Time: %s (Server Local Time).\n", 
		get_time_str());
	output(fd, g_buffer );

	return(0);

}

/**********************************************************************/
/*				circle				      */
/**********************************************************************/

/* This function allows fighters and barbarians to run circles about an */
/* enemy, confusing it for several seconds.				*/

int circle( creature *ply_ptr, cmd *cmnd )
{
	creature	*crt_ptr;
	room		*rom_ptr;
	time_t		i, t;
	int		chance, delay, fd;
	/* int		 exp; */

	fd = ply_ptr->fd;
	rom_ptr = ply_ptr->parent_rom;

	if(cmnd->num < 2) {
		output(fd, "Circle whom?\n");
		return(0);
	}

	/*23/04/2022 class restrictions removed by smithy
	skilltrees now account for permission to use skills
	if(ply_ptr->class != FIGHTER && ply_ptr->class != BARBARIAN &&
	   ply_ptr->class < BUILDER) {
		output(fd, "Only barbarians and fighters may circle.\n");
		return(0);
	}*/

	crt_ptr = find_crt_in_rom(ply_ptr, rom_ptr,
			   cmnd->str[1], cmnd->val[1], MON_FIRST);

	if(!crt_ptr || crt_ptr == ply_ptr 
		|| (crt_ptr->type == PLAYER && strlen(cmnd->str[1]) < 3)) {
		output(fd, "That is not here.\n");
		return(0);
	}

	if(crt_ptr->type == PLAYER) 
	{
		if ( !pkill_allowed(ply_ptr, crt_ptr) )
		{
			return(0);
		}

		if(is_charm_crt(ply_ptr->name, crt_ptr) && F_ISSET(crt_ptr, PCHARM)) {
            sprintf(g_buffer, "You are too fond of %s to do that.\n", 
				crt_ptr->name);
			output(fd, g_buffer );
            return(0);
        }
	}

	i = LT(ply_ptr, LT_ATTCK);
	t = time(0);

	if(i > t) {
		please_wait(fd, i-t);
		return(0);
	}

	F_CLR(ply_ptr, PHIDDN);
	if(F_ISSET(ply_ptr, PINVIS)) {
		F_CLR(ply_ptr, PINVIS);
		output(fd, "Your invisibility fades.\n");
		broadcast_rom(fd, ply_ptr->rom_num, "%M fades into view.",
			      m1arg(ply_ptr));
	}

	chance = 50 + (ply_ptr->level-crt_ptr->level)* 5 +
		 (bonus[(int)ply_ptr->dexterity] -bonus[(int)crt_ptr->dexterity])*5;
	if (crt_ptr->type == MONSTER && F_ISSET(crt_ptr, MUNDED))
		chance -= (5 + crt_ptr->level*2);
	chance = MIN(90, chance);
	if(F_ISSET(crt_ptr, MNOCIR) || F_ISSET(ply_ptr, PBLIND))
		chance=1;
		
	if(crt_ptr->type != PLAYER) 
	{
		if(!is_crt_killable(crt_ptr, ply_ptr)) 
		{
			return(0);
		}
		/* if(is_charm_crt(crt_ptr->name, ply_ptr))
			del_charm_crt(crt_ptr, ply_ptr); */

		add_enm_crt(ply_ptr->name, crt_ptr);
	}

	if(mrand(1,100) <= chance) {
		if(ply_ptr->class == BARBARIAN)
			delay = mrand(6,9);
		else
			delay = mrand(6,12);
		crt_ptr->lasttime[LT_ATTCK].ltime = t;
		crt_ptr->lasttime[LT_ATTCK].interval = delay;

		mprint(fd, "You circle %m.\n", m1arg(crt_ptr));

		mprint(crt_ptr->fd, "%M circles you.\n", m1arg(ply_ptr));
		broadcast_rom2(fd, crt_ptr->fd, ply_ptr->rom_num, 
			       "%M circles %m.", m2args(ply_ptr, crt_ptr));
		ply_ptr->lasttime[LT_ATTCK].interval = 2;

                /* if(GUILDEXP)
                {
                        if(crt_ptr->type == MONSTER && check_guild(ply_ptr) == 2)
                        {
                                exp=MAX(1, crt_ptr->level);
                                ply_ptr->experience+= exp;  
                                sprintf(g_buffer, "You guild awards you %d experience for your deed.\n", exp);
                                output(fd, g_buffer);
                        }
                } */

	}
	else {
		output(fd, "You failed to circle it.\n");
		mprint(crt_ptr->fd, "%M tried to circle you.\n", m1arg(ply_ptr));
		broadcast_rom2(fd, crt_ptr->fd, ply_ptr->rom_num,
			       "%M tried to circle %m.", m2args(ply_ptr, crt_ptr));
		ply_ptr->lasttime[LT_ATTCK].interval = 3;
	}

	ply_ptr->lasttime[LT_ATTCK].ltime = t;

	return(0);

}

/**********************************************************************/
/*				bash				      */
/**********************************************************************/

/* This function allows fighters and barbarians to "bash" an opponent, */
/* doing less damage than a normal attack, but knocking the opponent   */
/* over for a few seconds, leaving him unable to attack back.	       */

int bash( creature *ply_ptr, cmd *cmnd )
{
	creature	*crt_ptr;
	room		*rom_ptr;
	time_t		i, t;
	int		m, n, chance, fd, p, addprof;

	fd = ply_ptr->fd;
	rom_ptr = ply_ptr->parent_rom;

	if(cmnd->num < 2) {
		output(fd, "Bash whom?\n");
		return(0);
	}

	/*23/04/2022 class restrictions removed by smithy
	skilltrees now account for permission to use skills
	if(ply_ptr->class != FIGHTER && ply_ptr->class != BARBARIAN &&
	   ply_ptr->class < BUILDER) {
		output(fd, "Only barbarians and fighters may bash.\n");
		return(0);
	}*/

	crt_ptr = find_crt_in_rom(ply_ptr, rom_ptr, cmnd->str[1], cmnd->val[1],
		MON_FIRST);

	if(!crt_ptr || crt_ptr == ply_ptr || ( crt_ptr->type == PLAYER && strlen(cmnd->str[1])<3) ) {
		output(fd, "That isn't here.\n");
		return(0);
	}


	if(crt_ptr->type != PLAYER && is_enm_crt(ply_ptr->name, crt_ptr)) 
	{
		sprintf(g_buffer, "Not while you're already fighting %s.\n",
		      F_ISSET(crt_ptr, MMALES) ? "him":"her");
		output(fd, g_buffer );
		return(0);
	}
	else if(crt_ptr->type == PLAYER) 
	{
		if ( !pkill_allowed(ply_ptr, crt_ptr) )
		{
			return(0);
		}

		if(is_charm_crt(ply_ptr->name, crt_ptr)&& F_ISSET(crt_ptr, PCHARM)) {
		    sprintf(g_buffer, "You like %s too much to do that.\n", crt_ptr->name);
			output(fd, g_buffer );
            return(0);
        }
	}

	i = LT(ply_ptr, LT_ATTCK);
	t = time(0);

	if(i > t) {
		please_wait(fd, i-t);
		return(0);
	}

	ply_ptr->lasttime[LT_ATTCK].ltime = t;
	ply_ptr->lasttime[LT_ATTCK].interval = 3;
	ply_ptr->lasttime[LT_SPELL].ltime = t;
        ply_ptr->lasttime[LT_SPELL].interval = 5;
	
	F_CLR(ply_ptr, PHIDDN);
	if(F_ISSET(ply_ptr, PINVIS)) {
		F_CLR(ply_ptr, PINVIS);
		output(fd, "Your invisibility fades.\n");
		broadcast_rom(fd, ply_ptr->rom_num, "%M fades into view.",
			      m1arg(ply_ptr));
	}

	if(crt_ptr->type != PLAYER) 
	{
		if(!is_crt_killable(crt_ptr, ply_ptr)) 
		{
			return(0);
		}
		if(F_ISSET(crt_ptr, MMGONL)) {
            mprint(fd, "Your weapon has no effect on %m.\n",
                m1arg(crt_ptr));
            return(0);
        }
        if(F_ISSET(crt_ptr, MENONL)) {
            if(!ply_ptr->ready[WIELD-1] || 
                ply_ptr->ready[WIELD-1]->adjustment < 1) {
                mprint(fd, "Your weapon has no effect on %m.\n",
                    m1arg(crt_ptr));
                return(0);
            }
        }  
	/*	if(is_charm_crt(crt_ptr->name, ply_ptr))
			del_charm_crt(crt_ptr, ply_ptr); */
		
		add_enm_crt(ply_ptr->name, crt_ptr);
	}

	chance = 50 + (ply_ptr->level-crt_ptr->level)*10 +
		bonus[(int)ply_ptr->strength]*3 +
		 (bonus[(int)ply_ptr->dexterity] -bonus[(int)crt_ptr->dexterity])*2;
	chance += ply_ptr->class==BARBARIAN ? 5:0;
	chance = MIN(85, chance);
	if(F_ISSET(ply_ptr, PBLIND))
		chance = MIN(20, chance);

	if(mrand(1,100) <= chance) {

		if(ply_ptr->ready[WIELD-1]) {
			if(ply_ptr->ready[WIELD-1]->shotscur < 1) 
			{
				break_weapon( ply_ptr );
				return(0);
			}
		}

		n = ply_ptr->thaco - crt_ptr->armor/10;
		if(mrand(1,20) >= n) {

			crt_ptr->lasttime[LT_ATTCK].ltime = t;
			crt_ptr->lasttime[LT_ATTCK].interval = mrand(5,MIN(7,(ply_ptr->strength/2)));
			crt_ptr->lasttime[LT_SPELL].ltime = t;
                        crt_ptr->lasttime[LT_SPELL].interval = mrand(7,MIN(10,(ply_ptr->strength/2)));
			
			if(ply_ptr->ready[WIELD-1])
				n = mdice(ply_ptr->ready[WIELD-1]) / 2;
			else
				n = mdice(ply_ptr) / 2;

			if(ply_ptr->class == BARBARIAN && mrand(1,100) > 75)
				n = (n*3)/2;

			m = MIN(crt_ptr->hpcur, n);
			if(crt_ptr->type != PLAYER) {
				/* if(is_charm_crt(crt_ptr->name, ply_ptr))
					del_charm_crt(crt_ptr, ply_ptr); */

				add_enm_dmg(ply_ptr->name, crt_ptr, m);
				if(ply_ptr->ready[WIELD-1]) {
					p = MIN(ply_ptr->ready[WIELD-1]->type,
						4);
					addprof = (m * crt_ptr->experience) /
						MAX(crt_ptr->hpmax, 1);
					addprof = MIN(addprof,
						crt_ptr->experience);
					ply_ptr->proficiency[p] += addprof;
				}
				else if(ply_ptr->class == MONK) {
                                  /* give blunt prof for monk barehand */
                                  addprof = (m * crt_ptr->experience) /
                                      MAX(crt_ptr->hpmax, 1);
                                  addprof = MIN(addprof, crt_ptr->experience);
                                  ply_ptr->proficiency[HAND] += addprof;  
	                        }
			}

			if(GUILDEXP)
			{
				if(check_guild(ply_ptr) == 6)
				{
					n += 2;
					if(F_ISSET(ply_ptr, PNOCMP)) {
						output(fd, "Your guild expertise increases your bash.\n");
					}
				}
			}

			
			

			sprintf(g_buffer, "You bash with all your might\n");
			output(fd, g_buffer);

			broadcast_rom2(fd, crt_ptr->fd, crt_ptr->rom_num, 
				       "%M bashes %m.", m2args(ply_ptr, crt_ptr));

			sprintf(g_buffer, "%%M bashed you.\n");
			mprint(crt_ptr->fd, g_buffer, m1arg(ply_ptr) );

			combat_output(ply_ptr, crt_ptr, n);

			/* handle shot reduction */
			attack_with_weapon( ply_ptr );

			ply_ptr->lasttime[LT_ATTCK].interval = 2;

			if(crt_ptr->hpcur < 1) {
				mprint(fd, "You killed %m.\n", m1arg(crt_ptr));
				broadcast_rom2(fd, crt_ptr->fd, ply_ptr->rom_num,
					"%M killed %m.", m2args(ply_ptr, crt_ptr));

				die(crt_ptr, ply_ptr);
			}
			else
				check_for_flee(crt_ptr);
		}
		else {
			output(fd, "Your bash failed.\n");
			mprint(crt_ptr->fd, "%M tried to bash you.\n", m1arg(ply_ptr));
			broadcast_rom2(fd, crt_ptr->fd, crt_ptr->rom_num, 
		       "%M tried to bash %m.", m2args(ply_ptr, crt_ptr));
		}
	}

	else {
		output(fd, "Your bash failed.\n");
		mprint(crt_ptr->fd, "%M tried to bash you.\n", m1arg(ply_ptr));
		broadcast_rom2(fd, crt_ptr->fd, crt_ptr->rom_num, 
			       "%M tried to bash %m.", m2args(ply_ptr, crt_ptr));
	}

	return(0);

}

/**********************************************************************/
/*				save_ply_cmd			      */
/**********************************************************************/
/* This is the function that is called when a players types save      */
/* It calls save_ply (the real save function) and then tells the user */
/* the player was saved.  See save_ply() for more details. */
int save_ply_cmd( creature *ply_ptr, cmd *cmnd )
{
	ASSERTLOG( ply_ptr );

	F_SET(ply_ptr,PSAVES);
	save_ply( ply_ptr );
	F_CLR(ply_ptr,PSAVES);

	output(ply_ptr->fd, "Player saved.\n");

	return(0);
}




/**********************************************************************/
/*				save_ply			      */
/**********************************************************************/
/* This function saves a player when the game decides to.  It does  */
/* not issue a message that the player was saved. This should be called */
/* instead of write_ply becuase it handles saving worn items properly */

/* This function saves a player's char.  Since items need to be un-readied */
/* before a player can be saved to a file, this function makes a duplicate */
/* of the player, unreadies everything on the duplicate, and then saves    */
/* the duplicate to the file.  Afterwards, the duplicate is freed from     */
/* memory.								   */


int save_ply( creature *ply_ptr )
{
	creature	*dum_ptr;
	object		*obj[MAXWEAR];
	int		i, m, n = 0;
	int		nReturn = 0;

	ASSERTLOG( ply_ptr );

	dum_ptr = (creature *)malloc(sizeof(creature));
	if(!dum_ptr)
		merror("save_ply", FATAL);

	*dum_ptr = *ply_ptr;

	for(i=0; i<MAXWEAR; i++) {
		if(dum_ptr->ready[i]) {
			dequip(dum_ptr,dum_ptr->ready[i]);
			obj[n++] = dum_ptr->ready[i];
			add_obj_crt(dum_ptr->ready[i], dum_ptr);
			dum_ptr->ready[i] = 0;
		}
	}

	if(dum_ptr->name[0] == '\0') 
	{
		/* error, no can save */
		nReturn = 1;
	}
	else
	{
		m = write_ply(dum_ptr->name, dum_ptr);
		if ( m < 0 )
			merror("ERROR - write_ply", NONFATAL);

		for(i=0; i<n; i++)
			del_obj_crt(obj[i], dum_ptr);

	}

	free(dum_ptr);

	return(nReturn);
}

/**********************************************************************/
/*				talk				      */
/**********************************************************************/

/* This function allows players to speak with monsters if the monster */
/* has a talk string set.					      */

int talk( creature *ply_ptr, cmd *cmnd )
{
	room		*rom_ptr;
	creature	*crt_ptr;
	ttag		*tp;
	int			fd;

	fd = ply_ptr->fd;
	rom_ptr = ply_ptr->parent_rom;

	if(cmnd->num < 2) {
		output(fd, "Talk to whom?\n");
		return(0);
	}

	crt_ptr = find_crt(ply_ptr, rom_ptr->first_mon,
			   cmnd->str[1], cmnd->val[1]);

	if(!crt_ptr) {

		output(fd, "I don't see that here.\n");
		return(0);
	}

	F_CLR(ply_ptr, PHIDDN);

	if(cmnd->num == 2 || !F_ISSET(crt_ptr, MTALKS)) {
		broadcast_rom(fd, ply_ptr->rom_num, "%M talks with %m.", 
			m2args(ply_ptr, crt_ptr));

		if(crt_ptr->talk[0]) {
			sprintf(g_buffer, "%%M says to %%M, \"%s\".", 
				crt_ptr->talk);
			broadcast_rom(fd, ply_ptr->rom_num,
				g_buffer, m2args(crt_ptr, ply_ptr));

			sprintf(g_buffer, "%%M says to you, \"%s\".\n", crt_ptr->talk);
			mprint(fd, g_buffer, m1arg(crt_ptr));

		}
		else
			broadcast_rom(-1, ply_ptr->rom_num,
				"%M doesn't say anything.", m1arg(crt_ptr));
		if(F_ISSET(crt_ptr, MTLKAG))
			add_enm_crt(ply_ptr->name, crt_ptr);
	}

	else {
		if(!crt_ptr->first_tlk)
			if(!load_crt_tlk(crt_ptr))
				return(PROMPT);
		sprintf(g_buffer, "%%M asks %%m about \"%s\".", cmnd->str[2]);
		broadcast_rom(fd, ply_ptr->rom_num, g_buffer,
			m2args(ply_ptr, crt_ptr));

		tp = crt_ptr->first_tlk;
		
		

		while(tp) {
		//SMITHY ADDITIONS HERE
		//seperate the words out of the key string
			int 	index =0, num =0,i, n;
			char	*word[4];
			char 	keystring[80];
			char 	attribute[14] = "";
			char 	operand[6] = "";
			char 	value[10] = "";	
			
			for (i=0;i<4;i++)
			word[i] = 0;

			strcpy(attribute, "");
			strcpy(operand, "");
			strcpy(value, "");	

			//output(fd, "outputting keystring:");
			//output(fd, tp->key);
			//output(fd, "\n");
			strcpy(keystring, tp->key);
			//output(fd, keystring);
			for (n=0;n<4;n++){

				i=0;
				while(isalpha((int)keystring[i +index]) || isdigit((int)keystring[i +index]) || 
					keystring[i +index] == '-')
					i++;
				word[n] = (char *)malloc(sizeof(char)*i + 1);
				if(!word[n])
					merror("talk_crt_act", FATAL);

				memcpy(word[n],&keystring[index],i);
				//output(fd, word[n]);
				//output(fd, "\n");
				word[n][i] = 0;

				while(isspace((int)keystring[index +i]))
					i++;
	
				index += i;
				num++;
				if(tp->key[index] == 0)
					break;

			}
		// if there are multiple words, the 2nd, 3rd and 4th can be
		// assigned to their appropriate destinations
			if (word[1]){
				
				strcat(word[1], "\0");
				strcpy(attribute, word[1]);
				//elog(word[1]);
				strcat(word[2], "\0");
				strcpy(operand, word[2]);
				//elog(word[2]);
				strcat(word[3], "\0");
				strcpy(value, word[3]);
				//elog(word[3]);
				/*output(fd, "outputting attribute:");
				output(fd, attribute);
				output(fd, "\n");
				output(fd, "outputting operand:");
				output(fd, operand);
				output(fd, "\n");
				output(fd, "outputting value:");
				output(fd, value);
				output(fd, "\n");*/
			}
		//now we can resume the normal code
			//elog("this is before comparing a stat for a talkfile\n");
			if (!strcmp(cmnd->str[2], word[0])){
				if (word[1]){
					//elog("we have to run conditionsmet");
					if (conditionsmet(ply_ptr, attribute, operand, value)){
						sprintf(g_buffer, "%%M says to %%M, \"%s\".", tp->response);
						broadcast_rom(fd, ply_ptr->rom_num,	g_buffer, 
							m2args( crt_ptr, ply_ptr ));

						sprintf(g_buffer, "%%M says to you, \"%s\"\n", tp->response);
						mprint(fd, g_buffer, m1arg(crt_ptr));
						talk_action(ply_ptr, crt_ptr, tp);
						return(0);
					}
				}
				else{
					sprintf(g_buffer, "%%M says to %%M, \"%s\".", tp->response);
					broadcast_rom(fd, ply_ptr->rom_num,	g_buffer, 
						m2args( crt_ptr, ply_ptr ));

					sprintf(g_buffer, "%%M says to you, \"%s\"\n", tp->response);
					mprint(fd, g_buffer, m1arg(crt_ptr));
					talk_action(ply_ptr, crt_ptr, tp);
					return(0);
				}
			}

			/*if(!strcmp(cmnd->str[2], word[0]) && (conditionsmet(ply_ptr, attribute, operand, value) || !word[1])) {
				sprintf(g_buffer, "%%M says to %%M, \"%s\".", tp->response);
				broadcast_rom(fd, ply_ptr->rom_num,	g_buffer, 
					m2args( crt_ptr, ply_ptr ));

				sprintf(g_buffer, "%%M says to you, \"%s\"\n", tp->response);
				mprint(fd, g_buffer, m1arg(crt_ptr));
				talk_action(ply_ptr, crt_ptr, tp);
				return(0);
			}*/
			tp = tp->next_tag;
		}
		broadcast_rom(-1, ply_ptr->rom_num, "%M shrugs.", m1arg(crt_ptr));
		if(F_ISSET(crt_ptr, MTLKAG))
			add_enm_crt(ply_ptr->name, crt_ptr);
	}

	return(0);

}

/******************************************************************/
/*                      talk_action                               */
/******************************************************************/
/* The talk_action function handles a monster's actin when a     *
 * player asks the monster about a key word.  The format for the *
 * is defined in the monster's talk file.  Currently a monster   *
 * can attack, or cast spells on the player who mentions the key *
 * word or preform any of the defined social commands */

void talk_action( creature *ply_ptr, creature *crt_ptr, ttag *tt )
{
cmd		cm;
int 	i, n,splno =0;
object  *obj_ptr;
int		(*fn)();
char tempstr[5];
int j, k;

for (i = 0; i < COMMANDMAX;i++){
	cm.str[i][0] = 0;
	cm.str[i][24] = 0;
	cm.val[i]	= 0;
}
	cm.fullstr[0] = 0;
	cm.num = 0;

switch(tt->type){
	case 1: 	/*attack */
		add_enm_crt(ply_ptr->name, crt_ptr);
		broadcast_rom(ply_ptr->fd, ply_ptr->rom_num,
					"%M attacks %M\n", m2args(crt_ptr,ply_ptr));
		ANSI(ply_ptr->fd, AFC_RED);
		mprint(ply_ptr->fd,"%M attacks you.\n", m1arg(crt_ptr));
		ANSI(ply_ptr->fd, AFC_WHITE);
		break;

	case 2:				/*action command */
		if(action){
			strncpy(cm.str[0],tt->action,25);
			strcat(cm.fullstr,tt->action);
			cm.val[0]  = 1;
			cm.num = 1;

			if(tt->target)
				if(!strcmp(tt->target,"PLAYER")){
					strcpy(cm.str[1],ply_ptr->name);
					strcat(cm.fullstr," ");
					strcat(cm.fullstr,ply_ptr->name);
					cm.val[1]  = 1;
					cm.num = 2;
				}
			action(crt_ptr,&cm);
		}
		break;
	case 3:			/*cast */
		if(tt->action){
			n =  crt_ptr->mpcur; 
			strcpy(cm.str[0],"cast");
			strncpy(cm.str[1],tt->action,25);
			strcpy(cm.str[2],ply_ptr->name);
			cm.val[0]  = cm.val[1]  = cm.val[2]  = 1;
			cm.num = 3;
			sprintf(cm.fullstr,"cast %s %s",tt->action,ply_ptr->name);

		    i = 0;
			do {
		   		if(!strcmp(tt->action, get_spell_name(i))) {
           			splno = i;
           	 		break;
        		}
       	 		i++;
    		} while(get_spell_num(i) != -1);

			if(get_spell_num(i) == -1)
				return;
		 	fn = get_spell_function(splno);
			if(fn == offensive_spell || fn == room_damage) {
				for(i=0; ospell[i].splno != get_spell_num(splno); i++)
					if(ospell[i].splno == -1) return;
				 (*fn)(crt_ptr, &cm, CAST, get_spell_name(splno),
            &ospell[i]);
    		}
			else if(is_enm_crt(ply_ptr->name,crt_ptr)){
				mprint(ply_ptr->fd,"%M refuses to cast any spells on you.\n",
					m1arg(crt_ptr));
				return;
			}	
			else
			  (*fn)(crt_ptr, &cm, CAST);
			
			if (get_spell_num(i)  != SRESTO && crt_ptr->mpcur == n)
			{
				sprintf(g_buffer,
					"%%M appoligies that %s can not currently cast that spell on you.\n",
					(F_ISSET(crt_ptr,MMALES)) ? "he" : "she");
				mprint(ply_ptr->fd,
					g_buffer, m1arg(crt_ptr));
			}
		}
		break;
	case 4: //give
		i = atoi(tt->action);
		if (i > 0){
			n=load_obj(i, &obj_ptr);
            if(n > -1) {
                if(F_ISSET(obj_ptr, ORENCH))
                    rand_enchant(obj_ptr);
 
                if(weight_ply(ply_ptr) + weight_obj(obj_ptr) > 
                   max_weight(ply_ptr)) {
                   output(ply_ptr->fd, "You can't hold anymore.\n");
				   break;
               }    

               if(obj_ptr->sets_flag[15] && (F_ISSET(obj_ptr, OSINGLEOBJ))){
               	 broadcast_rom(ply_ptr->fd, ply_ptr->rom_num, "%M tells %m a secret\n", m2args(crt_ptr,ply_ptr));
               	 if (!objective_item_pickup(ply_ptr, obj_ptr)){
               	 	break;
               	 }	
               }
               else if(obj_ptr->sets_flag[15] && (F_ISSET(obj_ptr, OMULTIOBJ))){
               	 broadcast_rom(ply_ptr->fd, ply_ptr->rom_num, "%M tells %m a secret\n", m2args(crt_ptr,ply_ptr));
               	 if (!multi_objective_item_pickup(ply_ptr, obj_ptr)){
               	 	break;
               	 }	
               }

	            if(obj_ptr->questnum && Q_ISSET(ply_ptr, obj_ptr->questnum)) {
                    output(ply_ptr->fd, "You may not get that. You have already fulfilled that quest.\n");
					break;
                } 
	     		if(obj_ptr->questnum) {
   	                 output(ply_ptr->fd, "Quest fulfilled!  Don't drop it.\n");
   	                 Q_SET(ply_ptr, obj_ptr->questnum-1);
   	                 ply_ptr->experience += get_quest_exp(obj_ptr->questnum);
   	                 sprintf(g_buffer, "%ld experience granted.\n",
                            get_quest_exp(obj_ptr->questnum));
					 output(ply_ptr->fd, g_buffer );
   	                 add_prof(ply_ptr, get_quest_exp(obj_ptr->questnum));
   	             	} 
                
                //new smithy flag. only give the item if you want 
   	            //it to be given
   	            //(useful for quest and objective items)
                if (!F_ISSET(obj_ptr, ONOTGIVEN)){
                	add_obj_crt(obj_ptr, ply_ptr);
					mprint(ply_ptr->fd,"%M gives you %i\n",
					m2args(crt_ptr, obj_ptr));
					broadcast_rom(ply_ptr->fd, ply_ptr->rom_num,
					"%M gives %m %i\n", m3args(crt_ptr,ply_ptr,obj_ptr));
				}
			}
			else
			{
				mprint(ply_ptr->fd,"%M has nothing to give you.\n",
					m1arg(crt_ptr));
			}
        }  
		break;
	case 5: //OBJECTIVES
		//read the objective number off the talk pointer
		i = atoi(tt->action);

		//check if the player has already done that one
		if (!objective_check(ply_ptr, i)){
			//if they haven't, grant it and
			//give them a notification that their
			//objectives have been updated
			set_objective(ply_ptr, i);
		}		
	break;

	case 6: //MULTI-OBJECTIVES
		//read the objective number off the talk pointer
		
		sprintf(tempstr, "%c%c%c%c", tt->action[0], tt->action[1], tt->action[2], tt->action[3]);
		i = atoi(tempstr);
		sprintf(tempstr, "%c%c%c%c", tt->action[4], tt->action[5], tt->action[6], tt->action[7]);
		j = atoi(tempstr);
		sprintf(tempstr, "%c%c%c%c", tt->action[8], tt->action[9], tt->action[10], tt->action[11]);
		k = atoi(tempstr);

		multi_objective(ply_ptr, i, j, k);	
	break;
	case 7: //SILENT OBJECTIVES
		//read the objective number off the talk pointer
		i = atoi(tt->action);

		//check if the player has already done that one
		if (!objective_check(ply_ptr, i)){
			//if they haven't, grant it and
			//give them a notification that their
			//objectives have been updated
			silent_objective(ply_ptr, i);
		}		
	break;
	default:
		break;
	}
return;
}

int objective_check(creature *ply_ptr, int objective){
	int ret =0;
	int c,i,j, result;
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;
	char array[250][10];
	// load the player's file and make one for them if they don't already have it
	strcpy(filename, "../player/");
	strcat(filename, ply_ptr->name);
	strcat(filename, ".objectives\0");
	fpt = fopen(filename, "a+");
	fclose(fpt);
	fpt = fopen(filename, "r");
	

	//clean the array
	for (i=0; i<250; i++){
		strcpy(array[i], "");
	}
	//iterate through the file and load the values into 
	//a handy array for future reading
	c = 0;
	result =0;
	while (result != EOF){
		result = fscanf(fpt, "%11[^,]", str);
		result = fscanf(fpt, "%*c");
		strcpy(array[c], str);
		c++;

	}
	//iterate through the array and ask if any of the values
	//match the objective we are currently checking
	for(i=0, j=0; i<250; i++){
		if (array[i]){
			//sprintf(g_buffer, "array[%i]: %i. objective: %i", i, atoi(array[i]), objective);
			//output(ply_ptr->fd, g_buffer);
			if (atoi(array[i]) == objective){
				j = 1;
			}
		}
	}
	fclose(fpt);
	return(j);
}

int set_objective(creature *ply_ptr, int objective){
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;
	
	int fd = ply_ptr->fd;
	//firstly check if they have done it already
	//if they haven't, proceed
	if (!objective_check(ply_ptr, objective)){
		//find the player's file and open it
	
		strcpy(filename, "../player/");
		strcat(filename, ply_ptr->name);
		strcat(filename, ".objectives\0");
		fpt = fopen(filename, "a+");
		//add the objective number to the end of the file
		strcpy(str, "");
		sprintf(str, "%i", objective);
		strcat(str, ", \0");
		fprintf(fpt, str);

		//let the player know they done good
		ANSI(fd, AFC_CYAN);
		output(ply_ptr->fd, "\n Your objective log has been updated!");
		output(ply_ptr->fd, "\n Type 'objectives' to read more");
 		output(ply_ptr->fd, "\n");
 		if (objective_exp(objective)){
   			ANSI(fd, AM_BOLD);
   			ply_ptr->experience += objective_exp(objective);
   	        sprintf(g_buffer, " %ld experience granted.\n\n", objective_exp(objective));
			output(ply_ptr->fd, g_buffer );
			add_prof(ply_ptr, objective_exp(objective));
   		}
   		ANSI(fd, AM_NORMAL);
   		ANSI(fd, AFC_WHITE);

		//close the file
		fclose(fpt);
	}
 	
	
 	return(0);
}


/*   CONDITIONS MET 
THIS function looks at the inputted attributes of a player
and evaluates if they meet those conditions
returns 1 on succes and 0 on failure*/

int conditionsmet(creature *ply_ptr, char *attribute, char *operand, char *value){
	int ret =0;
	int stat =0;

	//elog("running conditionsmet");
		
	//consider what attribute the talk string is asking for
	// and place that value into the "stat" variable
	if (!strcmp(attribute ,"STRENGTH")){
		stat = ply_ptr->strength;
	}
	if (!strcmp(attribute ,"DEX")){
		stat = ply_ptr->dexterity;
	}
	if (!strcmp(attribute ,"CON")){
		stat = ply_ptr->constitution;
	}
	if (!strcmp(attribute ,"INT")){
		stat = ply_ptr->intelligence;
	}
	if (!strcmp(attribute ,"PIETY")){
		stat = ply_ptr->piety;
	}
	if (!strcmp(attribute ,"LEVEL")){
		stat = ply_ptr->level;
	}
	if (!strcmp(attribute ,"ALIGN")){
		stat = ply_ptr->alignment;
	}
	if (!strcmp(attribute ,"CLASS")){
		stat = ply_ptr->class;
	}
	if (!strcmp(attribute ,"RACE")){
		stat = ply_ptr->race;
	}
	if (!strcmp(attribute ,"QUEST")){
		if (Q_ISSET(ply_ptr, atoi(value))){
			stat =1;
		} 
		else stat = -1;
	}
	if (!strcmp(attribute ,"SPELL")){
		if (S_ISSET(ply_ptr, atoi(value))){
			stat =1;
		} 
		else stat = -1;
	}
	if (!strcmp(attribute ,"FLAG")){
		if (F_ISSET(ply_ptr, atoi(value))){
			stat =1;
		} 
		else stat = -1;
	}
	if (!strcmp(attribute ,"OBJ")){
		if (objective_check(ply_ptr, atoi(value))){
			stat =1;
		} 
		else stat = -1;
	}
	if (!strcmp(attribute, "MULTIOBJABOVE") || !strcmp(attribute, "MULTIOBJBELOW")){
		//find out which one of the objectives in that multi-series they completed
		//elog("multiobjective dependent talkfile activated");
		if (objective_check(ply_ptr, atoi(value))){
			int obj;
			int i, j;
			for (i=1, j=0; i<10; i++){
				if (objective_check(ply_ptr, atoi(value)+i)){
					obj = atoi(value) +i;
					j++;
					//elog("they've done the objective");

				}
			}
			//they did the objective but after the first 9
			if (j==0){
				obj = atoi(value)+10;
			}
			if (!strcmp(attribute, "MULTIOBJABOVE")){
				//elog("trying to compare values above");
				if(atoi(operand) < obj){
					ret = 1;
				}
			}
			if (!strcmp(attribute, "MULTIOBJBELOW")){
				//elog("trying to compare values below");
				if(atoi(operand) > obj){
					ret = 1;
				}
			}
		} 		
		//elog("all done");
	}	
	//else (ret = 0);
	
	
	//consider what OPERAND is being called and then compare
	//the stat and the value to determine success
	if (!strcmp(operand,"ABOVE")){
		//output(ply_ptr->fd, "comparing a stat to be ABOVE threshold\n");
		//sprintf(g_buffer, "stat is %i and value is %i\n", stat, atoi(value));
		//output(ply_ptr->fd, g_buffer);
		if (stat > atoi(value)){
			ret = 1;
		}
	}
	if (!strcmp(operand,"BELOW")){
		if (stat < atoi(value)){
			ret = 1;
		}
	}
	if (!strcmp(operand,"EQUAL")){
		if (stat == atoi(value)){
			ret = 1;
		}
	}
	if (!strcmp(operand,"TRUE")){
		if (stat == 1){
			ret = 1;
		}
	}
	if (!strcmp(operand,"FALSE")){
		if (stat == -1){
			ret = 1;
		}
	}

	return(ret);	
}