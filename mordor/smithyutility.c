/*
Some utilities by smithy

21/01/2022: detect spell realm

20/01/2022: Objectives Command

09/01/2022: identify spell
*/

#include "../include/mordb.h"
#include "mextern.h"


#include <ctype.h>
#include <string.h>

void damage_outputter(creature *ply_ptr, int amount, int damage_type ){
	int fd;

	fd = ply_ptr->fd;

	ANSI(fd, AM_NORMAL);
	ANSI(fd, AFC_WHITE);
	output(fd, "(");
	damage_output(ply_ptr, amount, damage_type);
	output(fd, " ");
	switch(damage_type){
		case DSHARP: {
					
					output(fd, "sharp"); 
					break;}
				case DTHRUST:{ 
					
					output(fd, "thrust"); 
					break;}
				case DPOLE: {
					
					output(fd, "blunt"); 
					break;}
				case DBLUNT: {
					
					output(fd, "pole");  
					break;}
				case DMISSILE: {
					
					output(fd, "missile"); 
					break;}
				case DHAND: {
					
					output(fd, "hand");  
					break;}
				case DEARTH: {
					
					output(fd, "earth");  
					break;}
				case DWIND: {
					
					output(fd, "wind");  
					break;}
				case DFIRE: {
					
					output(fd, "fire"); 
					break;}
				case DWATER: {
					
					output(fd, "water"); 
					break;}
				case DPOISON: {
					
					output(fd, "poison"); 
					break;}
				case DMAGIC: {
					
					output(fd, "magic"); 
					break;}
				case DDIVINE: {
					
					output(fd, "divine");  
					break;}
				
				}
				ANSI(fd, AM_NORMAL);
				ANSI(fd, AFC_WHITE);
				output(fd, ")\n");
}

int objectives(creature *ply_ptr, cmd *cmnd){
	int fd;

	fd = ply_ptr->fd;
	output(fd, "\n---------CURRENT OBJECTIVES---------\n");
	

	if (!Q_ISSET(ply_ptr, 12)){
		output(fd, "Quest 1: Help Farmer Joe");
	}


	return (0);
}


int identify(creature *ply_ptr, cmd *cmnd, int how){
	creature	*crt_ptr;
	room		*rom_ptr;
	object		*obj_ptr;
	ctag		*cp;
	time_t		t;
	int		fd;
	int 	match, n;

	fd = ply_ptr->fd;

	if(cmnd->num < 3) {
		output(fd, "Cast identify on what?\n");
		return(0);
	}

	if(ply_ptr->mpcur < 1 && how == CAST) {
		output(fd, "Not enough magic points.\n");
		return(0);
	}
	if(!S_ISSET(ply_ptr, SIDENTIFY) && how == CAST) {
		output(fd, "You don't know that spell.\n");
		return(0);
	}
	if(how == POTION) {
		output(fd, "Nothing happens.\n");
		return(0);
	}
	if(how == CAST || how == SCROLL || how == WAND) {
		if (how == CAST){
			ply_ptr->mpcur -= 1;
		}
		rom_ptr = ply_ptr->parent_rom;

		obj_ptr = find_obj(ply_ptr, ply_ptr->first_obj, cmnd->str[2], cmnd->val[2]);

		if(!obj_ptr || !cmnd->val[1]) {
			for(n=0; n<MAXWEAR; n++) {
				if(!ply_ptr->ready[n])
					continue;
				if(EQUAL(ply_ptr->ready[n], cmnd->str[2]))
					match++;
				else continue;
				if(cmnd->val[1] == match || !cmnd->val[2]) {
					obj_ptr = ply_ptr->ready[n];
				break;
			}
		}
	}

		if(!obj_ptr)
			obj_ptr = find_obj(ply_ptr, rom_ptr->first_obj, cmnd->str[2], cmnd->val[2]);

		if(obj_ptr) {
		
		if(obj_ptr->description[0])
		{
			sprintf(g_buffer, "%s\n", obj_ptr->description);
			output(fd, g_buffer);
		}
		else
			output(fd, "You see nothing special about it.\n");

		if(obj_ptr->type <= ARMOR)
		{
			sprintf(g_buffer, "%s", obj_condition(obj_ptr));
                        output(fd, g_buffer);
		}

		

		if(obj_ptr->type <= MISSILE) {
			mprint(fd, "%I is a ", m1arg(obj_ptr));
			switch(obj_ptr->type) {
			case SHARP: output(fd, "sharp"); break;
			case THRUST: output(fd, "thrusting"); break;
			case POLE: output(fd, "pole"); break;
			case BLUNT: output(fd, "blunt"); break;
			case MISSILE: output(fd, "missile"); break;
			}
			output(fd, " weapon,\n");
			sprintf(g_buffer, "which will deal damage in the range of %i - %i.", (obj_ptr->ndice * 1 + obj_ptr->pdice), (obj_ptr->ndice * obj_ptr->sdice + obj_ptr-> pdice));
			output(fd, g_buffer);

			if F_ISSET(obj_ptr, OBONUS1DAMAGE ){
				sprintf(g_buffer, "\nThis weapon will also deal %hu - %hu additional ", ((obj_ptr->special -8) * 1), ((obj_ptr->special -8) * obj_ptr->sdice));
				output(fd,g_buffer);
				switch(obj_ptr->magicrealm) {
				case DSHARP: {
					ANSI(fd, AM_LOWINTESITY);
					ANSI(fd, AFC_WHITE);
					output(fd, "sharp"); 
					break;}
				case DTHRUST:{ 
					ANSI(fd, AM_ITALIC);
					ANSI(fd, AFC_WHITE);
					output(fd, "thrusting"); 
					break;}
				case DPOLE: {
					ANSI(fd, AM_ITALIC);
					ANSI(fd, AFC_YELLOW);
					output(fd, "pole"); 
					break;}
				case DBLUNT: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_BLACK);
					output(fd, "blunt"); 
					break;}
				case DMISSILE: {
					ANSI(fd, AM_UNDERLINE);
					ANSI(fd, AFC_WHITE);
					output(fd, "missile"); 
					break;}
				case DHAND: {
					ANSI(fd, AM_NORMAL);
					ANSI(fd, AFC_WHITE);
					output(fd, "barehand"); 
					break;}
				case DEARTH: {
					ANSI(fd, AM_LOWINTESITY);
					ANSI(fd, AFC_GREEN);
					output(fd, "earth"); 
					break;}
				case DWIND: {
					ANSI(fd, AM_LOWINTESITY);
					ANSI(fd, AFC_CYAN);
					output(fd, "wind"); 
					break;}
				case DFIRE: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_RED);
					output(fd, "fire"); 
					break;}
				case DWATER: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_BLUE);
					output(fd, "water"); 
					break;}
				case DPOISON: {
					ANSI(fd, AM_REVERSE);
					ANSI(fd, AFC_GREEN);
					output(fd, "poison"); 
					break;}
				case DMAGIC: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_MAGENTA);
					output(fd, "magic"); 
					break;}
				case DDIVINE: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_YELLOW);
					output(fd, "divine"); 
					break;}
					
			}
			ANSI(fd, AM_NORMAL);		
			ANSI	(fd, AFC_WHITE);

			output(fd, " damage.");
			
			}

			if F_ISSET(obj_ptr, OBONUS2DAMAGE ){
				sprintf(g_buffer, "\nThis weapon will also deal %hu - %hu additional ", ((obj_ptr->special2) * 1), ((obj_ptr->special2) * obj_ptr->sdice));
				output(fd,g_buffer);
				switch(obj_ptr->special1){
				case DSHARP: {
					ANSI(fd, AM_LOWINTESITY);
					ANSI(fd, AFC_WHITE);
					output(fd, "sharp"); 
					break;}
				case DTHRUST:{ 
					ANSI(fd, AM_ITALIC);
					ANSI(fd, AFC_WHITE);
					output(fd, "thrusting"); 
					break;}
				case DPOLE: {
					ANSI(fd, AM_ITALIC);
					ANSI(fd, AFC_YELLOW);
					output(fd, "pole"); 
					break;}
				case DBLUNT: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_BLACK);
					output(fd, "blunt"); 
					break;}
				case DMISSILE: {
					ANSI(fd, AM_UNDERLINE);
					ANSI(fd, AFC_WHITE);
					output(fd, "missile"); 
					break;}
				case DHAND: {
					ANSI(fd, AM_NORMAL);
					ANSI(fd, AFC_WHITE);
					output(fd, "barehand"); 
					break;}
				case DEARTH: {
					ANSI(fd, AM_LOWINTESITY);
					ANSI(fd, AFC_GREEN);
					output(fd, "earth"); 
					break;}
				case DWIND: {
					ANSI(fd, AM_LOWINTESITY);
					ANSI(fd, AFC_CYAN);
					output(fd, "wind"); 
					break;}
				case DFIRE: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_RED);
					output(fd, "fire"); 
					break;}
				case DWATER: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_BLUE);
					output(fd, "water"); 
					break;}
				case DPOISON: {
					ANSI(fd, AM_REVERSE);
					ANSI(fd, AFC_GREEN);
					output(fd, "poison"); 
					break;}
				case DMAGIC: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_MAGENTA);
					output(fd, "magic"); 
					break;}
				case DDIVINE: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_YELLOW);
					output(fd, "divine"); 
					break;}
			}
			ANSI(fd, AM_NORMAL);		
			ANSI	(fd, AFC_WHITE);
			output(fd, " damage.");
			
			}

			if F_ISSET(obj_ptr, ONOPHYSICAL){
				output(fd, "\n This weapon doesn't deal any phsyical damage");

			}


			

		}

		if((F_ISSET(obj_ptr, ODRUDT)  && obj_ptr->shotscur < 0) || (F_ISSET(obj_ptr, OALCHT)  && obj_ptr->shotscur < 0)) {
		   output(fd, "It looks to be a workable object.\n");
		   return(0);
		}
		   
		if(obj_ptr->type <= MISSILE || obj_ptr->type == ARMOR ||
		   obj_ptr->type == LIGHTSOURCE || obj_ptr->type == WAND ||
		   obj_ptr->type == KEY) { 
			
		//  This if added with condition descriptions to show when keys/wands
		//  are broken

		    if(obj_ptr->type == KEY || obj_ptr->type == WAND)
		    {
			if(obj_ptr->shotscur < 1)
				output(fd, "It's broken or used up.\n");
			else if(obj_ptr->shotscur <= obj_ptr->shotsmax/10)
				output(fd, "It looks about ready to break.\n");
		    }
			if(F_ISSET(obj_ptr, OTMPEN) &&(ply_ptr->class == ALCHEMIST || (ply_ptr->class ==MAGE && F_ISSET(ply_ptr, PDMAGI)) || ply_ptr->class >= BUILDER)) 
				output(fd, "It is weakly enchanted.\n");
		}

		return(0);
	}

	crt_ptr = find_crt_in_rom(ply_ptr, rom_ptr, cmnd->str[2],
			   cmnd->val[2], MON_FIRST);

	if(crt_ptr)
	{
		if ( crt_ptr->type == MONSTER) {

			mprint(fd, "You see %1m.\n", m1arg(crt_ptr));
			if(crt_ptr->description[0])
			{
				output_nl(fd, crt_ptr->description );
			}
			else
				output(fd, "There is nothing special about it.\n");
			
			sprintf(g_buffer,"%s%s", F_ISSET(crt_ptr, MMALES) ? "He":"She", 
				crt_condition(crt_ptr));
			output(fd, g_buffer);

			if(F_ISSET(ply_ptr, PKNOWA)) {
				sprintf(g_buffer, "%s ",
					F_ISSET(crt_ptr, MMALES) ? "He":"She");
				output(fd, g_buffer);

				if(crt_ptr->alignment < 0)
					output(fd, "has a red aura.\n");
				else if(crt_ptr->alignment == 0)
					output(fd, "has a grey aura.\n");
				else output(fd, "has a blue aura.\n");
			}
	
	/*		if(crt_ptr->hpcur < (crt_ptr->hpmax*3)/10)
			{
				sprintf(g_buffer, "%s has some nasty wounds.\n",
					  F_ISSET(crt_ptr, MMALES) ? "He":"She");
				output(fd, g_buffer);
			}   */
			
			if(is_enm_crt(ply_ptr->name, crt_ptr)) 
			{
				sprintf(g_buffer, "%s looks very angry at you.\n",
					  F_ISSET(crt_ptr, MMALES) ? "He":"She");
				output(fd, g_buffer);
			}
			if(crt_ptr->first_enm) {
				if(!strcmp(crt_ptr->first_enm->enemy, ply_ptr->name))
				{
					sprintf(g_buffer, "%s is attacking you.\n",
						  F_ISSET(crt_ptr, MMALES) ? "He":"She");
					output(fd, g_buffer);
				}
				else
				{
					sprintf(g_buffer, "%s is attacking %s.\n",
						  F_ISSET(crt_ptr, MMALES) ? "He":"She",
						  crt_ptr->first_enm->enemy);
					output(fd, g_buffer);
				}
			}
			consider(ply_ptr, crt_ptr);
			equip_list(fd, crt_ptr);
			return(0);
		}
		else
		{
			sprintf(g_buffer, "You see %s the %s %s.\n", crt_ptr->name,
				  get_race_adjective(crt_ptr->race), title_ply(crt_ptr));
			output(fd, g_buffer);
			if(crt_ptr->description[0])
			{
				output_nl(fd, crt_ptr->description);
			}
			
			if(crt_ptr->class < IMMORTAL) {
			   sprintf(g_buffer,"%s%s", F_ISSET(crt_ptr, MMALES) ? "He":"She",
                                crt_condition(crt_ptr));
                           output(fd, g_buffer);
			}
			
			if(F_ISSET(ply_ptr, PKNOWA) && crt_ptr->class < IMMORTAL) {
				sprintf(g_buffer, "%s ",
					F_ISSET(crt_ptr, MMALES) ? "He":"She");
				output(fd, g_buffer);
				if(crt_ptr->alignment < -100)
					output(fd, "has a red aura.\n");
				else if(crt_ptr->alignment < 101)
					output(fd, "has a grey aura.\n");
				else 
					output(fd, "has a blue aura.\n");
			}
		
		// removed with the addition of condition descriptions AK
		/*	if(crt_ptr->hpcur < (crt_ptr->hpmax*3)/10)
			{
				sprintf(g_buffer, "%s has some nasty wounds.\n",
					  F_ISSET(crt_ptr, PMALES) ? "He":"She");
				output(fd, g_buffer);
			}		*/
			
			equip_list(fd, crt_ptr);
			return(0);
		}
	}
	else
		output(fd, "You don't see that here.\n");

	return(0);
		}



}
