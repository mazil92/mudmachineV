/*
new combat routines by smithy

09/01/2022: 
*/

#include "../include/mordb.h"
#include "mextern.h"

#include <ctype.h>
#include <string.h>

/**/
/*compute_resistance_player

computes the resistance of a player to a particular damage type*/
/*when you make spells that add resistances, make sure you 
put them in here!*/
long compute_resistance(creature *crt_ptr, int damage_type){
	if (crt_ptr->type ==PLAYER){
		return(compute_resistance_player(crt_ptr, damage_type));
	}
	else{
		return(compute_resistance_creature(crt_ptr, damage_type));
	}
	return(0);
}

long compute_resistance_player(creature *ply_ptr, int damage_type){
	int i, j;
	long temp =0, temp2 =0;

	/*check each wear location
	look if they're wearing something there
	if they are, look if that item grants resistance to the
	incoming damage type
	if it does, add it to the tally*/
	for (i = 0, j = 0; i < MAXWEAR; i++ ){
		if (ply_ptr->ready[i]){
			if (!F_ISSET(ply_ptr->ready[i], OADDSA)){
			temp2 = temp2 + ply_ptr->ready[i]->sets_flag[damage_type-1];
			}
			
			j = ply_ptr->ready[i]->magicrealm;
			
			/* when using magicrealm, just code it as the damage type
			don't bother converting it to a realm. it's too hard
			additionally, use the special value minus 8
			yes, I know that's not very intuitive
			keep that in mind when setting those values for objects*/
			if (damage_type == j && F_ISSET(ply_ptr->ready[i], ORESIST1)) {
				temp =  temp + ply_ptr->ready[i]->special -8;

			}
			j = ply_ptr->ready[i]->special1;
			if (damage_type == j && F_ISSET(ply_ptr->ready[i], ORESIST2)) {
				temp =  temp + ply_ptr->ready[i]->special2;
			}
		}
	}
	/*add other resistance sources here*/
	
	
return (temp+temp2);
}

/*compute_resistance_creature

computes the resistance of a monster to a particular damage type*/
long compute_resistance_creature (creature *crt_ptr, int damage_type) {
	int i, j;
	long temp;

	switch(damage_type){
		
		/*use proficiencies 0,1,2,3,4,5*/
		case DSHARP:{
		temp = crt_ptr->proficiency[0];
		break;}
		case DTHRUST:{
		temp = crt_ptr->proficiency[1];
		break;}
		case DBLUNT:{
		temp = crt_ptr->proficiency[2];
		break;}
		case DPOLE:{
		temp = crt_ptr->proficiency[3];
		break;}
		case DMISSILE:{
		temp = crt_ptr->proficiency[4];
		break;}
		case DHAND:{
		temp = crt_ptr->proficiency[5];
		break;}

		/*use Realms 0, 1, 2, 3*/
		case DEARTH:{
		temp = crt_ptr->realm[0];
		break;}
		case DWIND:{
		temp = crt_ptr->realm[1];
		break;}
		case DFIRE:{
		temp = crt_ptr->realm[2];
		break;}
		case DWATER:{
		temp = crt_ptr->realm[3];
		break;}
		
		/*use realms 4, 5, 6. Yes I know that's clunky*/
		case DPOISON:{
		temp = crt_ptr->realm[4];
		break;}
		case DMAGIC:{
		temp = crt_ptr->realm[5];
		break;}
		case DDIVINE:{
		temp = crt_ptr->realm[6];
		break;}
	}

	
	/*add other resistance sources here*/
	
	
return (temp);
}

/*Players Dealing Damage
This function deals damage of a specified type from an attacker to a target */
int player_dealing_damage (creature *ply_ptr ){

}

short compute_DR (creature *crt_ptr){
	if (crt_ptr->type ==PLAYER){
		return(compute_DR_player(crt_ptr));
	}
	else{
		return(compute_DR_creature(crt_ptr));
	}
	return(0);
}

short compute_DR_creature (creature *crt_ptr){
	int DR;

	/*yeah it's clunky but we're using the creature's realm 7*/
	DR = 0;
	DR = crt_ptr->realm[7];
	/*add other calculations here*/
	return(DR);
}

/*compute DR player
This function calculates a player's damage reduction
it looks at all the items they are wearing and asks
if that item contributes it's armor value towards AC or DR
and then tallies up all the values that contribute to DR*/
short compute_DR_player (creature *ply_ptr){
	int 	DR, i;

	
	DR = 0;
	for(i=0; i<MAXWEAR; i++)
		if(ply_ptr->ready[i] && ((F_ISSET(ply_ptr->ready[i], OGIVESDR) )))
			DR = DR + ply_ptr->ready[i]->armor;

	/*add other calculations here*/
	return(DR);
}

/*output damage in colour
this function takes a given damage number, type and the 
player who requested that ouput. Then, it displays that number
in the colour that corresponds to that damage type*/
void damage_output (creature *ply_ptr, int damage, int damage_type){
	int 	fd;
	
	fd = ply_ptr->fd;
	sprintf(g_buffer, "%i", damage);
			switch(damage_type){
				case DSHARP: {
					ANSI(fd, AM_LOWINTESITY);
					ANSI(fd, AFC_WHITE);
					output(fd, g_buffer); 
					break;}
				case DTHRUST:{ 
					ANSI(fd, AM_ITALIC);
					ANSI(fd, AFC_WHITE);
					output(fd, g_buffer); 
					break;}
				case DPOLE: {
					ANSI(fd, AM_ITALIC);
					ANSI(fd, AFC_YELLOW);
					output(fd, g_buffer); 
					break;}
				case DBLUNT: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_BLACK);
					output(fd, g_buffer);  
					break;}
				case DMISSILE: {
					ANSI(fd, AM_UNDERLINE);
					ANSI(fd, AFC_WHITE);
					output(fd, g_buffer); 
					break;}
				case DHAND: {
					ANSI(fd, AM_NORMAL);
					ANSI(fd, AFC_WHITE);
					output(fd, g_buffer);  
					break;}
				case DEARTH: {
					ANSI(fd, AM_LOWINTESITY);
					ANSI(fd, AFC_GREEN);
					output(fd, g_buffer);  
					break;}
				case DWIND: {
					ANSI(fd, AM_LOWINTESITY);
					ANSI(fd, AFC_CYAN);
					output(fd, g_buffer);  
					break;}
				case DFIRE: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_RED);
					output(fd, g_buffer); 
					break;}
				case DWATER: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_BLUE);
					output(fd, g_buffer); 
					break;}
				case DPOISON: {
					ANSI(fd, AM_REVERSE);
					ANSI(fd, AFC_GREEN);
					output(fd, g_buffer); 
					break;}
				case DMAGIC: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_MAGENTA);
					output(fd, g_buffer); 
					break;}
				case DDIVINE: {
					ANSI(fd, AM_BOLD);
					ANSI(fd, AFC_YELLOW);
					output(fd, g_buffer);  
					break;}
				}
}


/*armor_confidence
this function provides a random number between 0 and 100 to be used
as a percentage in the calc damage formula. It represents how much
of a defender's DR is used for a given hit
Players can improve their armor confidence just by wearing more armor!*/
int armor_confidence (creature *crt_ptr){
	int value =0, confidence = 0;
	if (crt_ptr->type == MONSTER){
		value = 40 + (int)crt_ptr->dexterity + (int)crt_ptr->constitution;
		confidence = MIN(value, 90);
		
	}
	else{
		if (crt_ptr->ready[BODY-1]){
			value = value + 18;
		}
		if (crt_ptr->ready[ARMS-1]){
			value = value + 9;
		}
		if (crt_ptr->ready[LEGS-1]){
			value = value + 9;
		}
		if (crt_ptr->ready[NECK1-1]){
			value = value + 3;
		}
		if (crt_ptr->ready[NECK2-1]){
			value = value + 3;
		}
		if (crt_ptr->ready[HANDS-1]){
			value = value + 4;
		}
		if (crt_ptr->ready[HEAD-1]){
			value = value + 11;
		}
		if (crt_ptr->ready[FEET-1]){
			value = value + 5;
		}
		if (crt_ptr->ready[FINGER1-1]){
			value = value + 1;
		}
		if (crt_ptr->ready[FINGER2-1]){
			value = value + 1;
		}
		if (crt_ptr->ready[FINGER3-1]){
			value = value + 1;
		}
		if (crt_ptr->ready[FINGER4-1]){
			value = value + 1;
		}
		if (crt_ptr->ready[FINGER5-1]){
			value = value + 1;
		}
		if (crt_ptr->ready[FINGER6-1]){
			value = value + 1;
		}
		if (crt_ptr->ready[FINGER7-1]){
			value = value + 1;
		}
		if (crt_ptr->ready[FINGER8-1]){
			value = value + 1;
		}
		if (crt_ptr->ready[HELD-1]){
			value = value + 2;
		}
		if (crt_ptr->ready[SHIELD-1]){
			value = value + 16;
		}
		if (crt_ptr->ready[FACE-1]){
			value = value + 7;
		}
		
		confidence = MIN(92, value);
	}
	return(confidence);
}

/*calc damage
this function calculates damage based on the amount, type, DR, and resistance*/
double calc_damage (float amt, float DR, float resistance, float confidence ){
	float dmg = 0.0;
	int temp;

	/* DR represents the maximum value an incoming
	amount of damage can be reduced by

	A defender's resistance to that damage type
	determines how much of the DR is utilised for any
	source of damage

	Further, there is a confidence interval that determines 
	how much DR can be ultilised based on the amount
	of armor being worn
	for monsters, this value depends on their DEX and CON%*/
	/*resistance = MIN(100, resistance);*/
	DR = DR/10;
	resistance = resistance/100;
	temp = mrand((int)confidence, 100);
	confidence = (float)temp/100;
	dmg = (amt - (DR * resistance * confidence));
	return (dmg);

}


/*these next few functions are just components of the original combat routine
I put them here because I felt the original routine was a 
clunky mess that needed some de-cluttering*/

void fade_invis (int fd, creature *ply_ptr){
	if(F_ISSET(ply_ptr, PINVIS) && ply_ptr->type==PLAYER) {
        	F_CLR(ply_ptr, PINVIS);
        	output(fd, "Your invisibility fades.\n");
        	broadcast_rom(fd, ply_ptr->rom_num, "%M fades into view.",
			      m1arg(ply_ptr));
    	}
}

void attack_cooldown (time_t t, creature *ply_ptr){
	ply_ptr->lasttime[LT_ATTCK].ltime = t;
    
    if (ply_ptr->ready[WIELD-1]){
    	switch (ply_ptr->ready[WIELD-1]->type){
    		case SHARP:{
    			ply_ptr->lasttime[LT_ATTCK].interval = 2;

    		}
    		case THRUST:{
    			ply_ptr->lasttime[LT_ATTCK].interval = 3;

    		}
    		case BLUNT:{
    			ply_ptr->lasttime[LT_ATTCK].interval = 5;

    		}
    		case POLE:{
    			ply_ptr->lasttime[LT_ATTCK].interval = 4;

    		}
    		case MISSILE:{
    			ply_ptr->lasttime[LT_ATTCK].interval = 5;

    		}
    	}
    }
    else
        ply_ptr->lasttime[LT_ATTCK].interval = 3;

    if(F_ISSET(ply_ptr, PHASTE) && ply_ptr->type==PLAYER)
        ply_ptr->lasttime[LT_ATTCK].interval -= 1;
    
    if(F_ISSET(ply_ptr, PBLIND) && ply_ptr->type==PLAYER)
		ply_ptr->lasttime[LT_ATTCK].interval = 7;
}

void add_attackers (int fd, creature *ply_ptr, creature *crt_ptr){
	if(F_ISSET(ply_ptr, PALIAS) && ply_ptr->type==PLAYER) {
		mprint(fd, "You attack %m.\n", m1arg(crt_ptr));
		broadcast_rom(fd, ply_ptr->rom_num, "%M attacks %m.",
				m2args(ply_ptr, crt_ptr));
       		}
	
	else if(add_enm_crt(ply_ptr->name, crt_ptr) < 0 ) {
		/* if(is_charm_crt(crt_ptr->name, ply_ptr))
			del_charm_crt(crt_ptr, ply_ptr); */
          
	   		mprint(fd, "You attack %m.\n", m1arg(crt_ptr));
           		broadcast_rom(fd, ply_ptr->rom_num, "%M attacks %m.",
				m2args(ply_ptr, crt_ptr));
     		}
		
	else if(ply_ptr->type==MONSTER) {
		add_enm_crt(ply_ptr->name, crt_ptr);
	}
}

void do_attacking(int amount, creature *ply_ptr, creature *crt_ptr){
	
	
	crt_ptr->hpcur = crt_ptr->hpcur - amount;
	
	if (crt_ptr->type == MONSTER && ply_ptr->type == PLAYER){
		
		add_enm_dmg(ply_ptr->name, crt_ptr, amount);
		
	}
	
}

int choose_creature_damage_type(creature *crt_ptr){
	int i, j, match = 0;
	int choice = 0;
	int choices[13];

/*make a list of damage types you can choose from
then choose a random one from that list
always have DHAND enabled*/

	for (i=0, j=0; i < 13; i++){
		choices[i] = 0;
		if (F_ISSET(crt_ptr, MDEALSSHARP + i)){
			choices[i] = 1;
			j ++;
		}
	}

	if (j == 0){
		return(DHAND);
	}


	do{
		choice = mrand(1,13);
		if (choices[choice] == 1){
			match = choice;
		}
	} while (match == 0);
	
	if (match == 0){
		return(DHAND);
	}
	else{
		return(match);
	}

}

void combat_output (creature *ply_ptr, creature *crt_ptr, int n){
	/*DAMAGE IS BEING DONE HERE. n above is the damage value that was rolled.
SO NOW WE CARE ABOUT DAMAGE TYPES
AND RESISTANCES*/
	float temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8, tempdam;
	int j =0, k, total =0, fd, x; 
	int damage[13];
	char dmgstr[80], tempstr[80]; 
        
	fd = ply_ptr->fd;
        
        for (k=0, j=0; k < 13; k++){
        	damage[k] = 0;}
        	
        if (ply_ptr->ready[WIELD-1]){
        for (k=0, j=0; k < 13; k++){
        	
        	/*check if the attacker's weapon has damage in that type
        	assign it the corresponding amount of damage based on n
        	then check the defender's resistance to that type
        	then run the do_attacking function on that amount of damage
        	and then add that amount of damage to that part of the damage
        	array

        	after the for loop, read the array again to output it
        	However, if no damage types were recorded as being used
        	you can then check the weapons type and just do n damage
        	as that type (or barehand as appropriate)*/
        		
        		temp8 = ply_ptr->ready[WIELD-1]->sets_flag[k];

        		if (F_ISSET(ply_ptr, PENVENOM)){
        			if (k == DPOISON-1){
        				temp8 += 100;
        				F_CLR(ply_ptr, PENVENOM);
        				
        				if (j == 0 && !ply_ptr->ready[WIELD-1]->sets_flag[DMAGIC -1] && !ply_ptr->ready[WIELD-1]->sets_flag[DDIVINE-1]){
        					j ++;
        					
        					tempdam = calc_damage((float)n,(float)compute_DR(crt_ptr),(float)compute_resistance(crt_ptr, ply_ptr->ready[WIELD-1]->type+1),(float)armor_confidence(crt_ptr));
        					damage[ply_ptr->ready[WIELD-1]->type] = (int)tempdam;
        					damage[ply_ptr->ready[WIELD-1]->type] = MAX(0, damage[ply_ptr->ready[WIELD-1]->type]);
        					total = total + damage[ply_ptr->ready[WIELD-1]->type];
        					do_attacking(damage[ply_ptr->ready[WIELD-1]->type], ply_ptr, crt_ptr);
        				}
        			}
        			/*This is a bit clunky but I don't care
        			if the weapon hasn't done any other damage yet, 
        			add it's normal damage type now*/
        			
        		}
        		


        		if (temp8){
        			
        			

        			j++;
        			

        			temp1 = (float)n;
        			temp2 = (float)(temp8);

        			tempdam = temp1*(temp2/100);
        			/*sprintf(g_buffer, "|%f, ", tempdam);
        			output(fd, g_buffer);*/
        			/*damage[k] = (int)tempdam;*/
        			/*sprintf(g_buffer, "%f, %f, %f| ", (float)compute_DR_creature(crt_ptr),(float)compute_resistance_creature(crt_ptr, k+1),(float)armor_confidence(crt_ptr));
        			output(fd, g_buffer);*/
        			if (crt_ptr->type == MONSTER){
        				
        				tempdam = calc_damage(tempdam,(float)compute_DR_creature(crt_ptr),(float)compute_resistance_creature(crt_ptr, k+1),(float)armor_confidence(crt_ptr));
        				/*sprintf(g_buffer, "|%f| ", tempdam);
        				output(fd, g_buffer);*/
        				damage[k] = (int)tempdam;
        			
        			}
        			if (crt_ptr->type == PLAYER){
        				
        				tempdam = calc_damage((float)damage[k],(float)compute_DR_player(crt_ptr),(float)compute_resistance_player(crt_ptr, k+1),(float)armor_confidence(crt_ptr));
        				damage[k] = (int)tempdam;
        				
        			}
        			
        			damage[k] = MAX(0, damage[k]);
        			
        			total = total + damage[k];
        			
        			do_attacking(damage[k], ply_ptr, crt_ptr);
        			
        		}
        		
        	}
        }
        
        if (j == 0){
        	
        	if (crt_ptr->type == MONSTER){
				
				if (ply_ptr->ready[WIELD-1]){
					total = (int)calc_damage((float)n, (float)compute_DR_creature(crt_ptr), (float)compute_resistance_creature(crt_ptr, ply_ptr->ready[WIELD-1]->type + 1), (float)armor_confidence(crt_ptr));
				}
				else{
					temp3 = (float)n;
					temp4 = (float)compute_DR_creature(crt_ptr);
					temp5 = (float)compute_resistance_creature(crt_ptr, DHAND);
					temp6 = (float)armor_confidence(crt_ptr);
					temp7 = calc_damage(temp3, temp4, temp5, temp6);
					/*sprintf(g_buffer, "\n|| %f, %f, %f, %f || %f || %f ||", temp3, temp4, temp5, temp6, temp7, (temp3 - (temp4/10 * temp5/100 * temp6/100)));
					output(fd, g_buffer);*/
					total = (int)temp7;					
	  			}
			}
			else {
		   		if (ply_ptr->ready[WIELD-1]){
					
					total = (int)calc_damage((float)n, (float)compute_DR_player(crt_ptr), (float)compute_resistance_player(crt_ptr, ply_ptr->ready[WIELD-1]->type +1), (float)armor_confidence(crt_ptr));
	   			}
	   			else{		   		
	   				total = (int)calc_damage((float)n, (float)compute_DR_player(crt_ptr), (float)compute_resistance_player(crt_ptr, DHAND), (float)armor_confidence(crt_ptr));
				}
			}
       		total = MAX(0, total);
       		
        	do_attacking(total, ply_ptr, crt_ptr);
        	
        }

        
        sprintf(g_buffer, "You hit for %d damage.\n", total);
		
		output(fd, g_buffer);
		/*if PFLAG Damage info is on...*/
		
		output(fd, "(");
		
		sprintf(dmgstr, "");
        
        sprintf(tempstr, "");
		for (k=0, j=0; k < 13; k++){
			
			if (damage[k] != 0){

				if (j > 0)
        			{
        				output(fd, ", ");
        			}
				j++;
				x = k;
				damage_output(ply_ptr, damage[k], k+1);
				switch (x){
					case 0: sprintf(dmgstr, " sharp"); output(fd, dmgstr); break;
					case 1: sprintf(dmgstr, " thrust"); output(fd, dmgstr); break;
					case 2: sprintf(dmgstr, " blunt"); output(fd, dmgstr); break;
					case 3: sprintf(dmgstr, " pole"); output(fd, dmgstr); break;
					case 4: sprintf(dmgstr, " missile"); output(fd, dmgstr); break;
					case 5: sprintf(dmgstr, " hand"); output(fd, dmgstr); break;
					case 6: sprintf(dmgstr, " earth"); output(fd, dmgstr); break;
					case 7: sprintf(dmgstr, " wind"); output(fd, dmgstr); break;
					case 8: sprintf(dmgstr, " fire"); output(fd, dmgstr); break;
					case 9: sprintf(dmgstr, " water"); output(fd, dmgstr); break;
					case 10: sprintf(dmgstr, " poison"); output(fd, dmgstr); break;
					case 11: sprintf(dmgstr, " magic"); output(fd, dmgstr); break;
					case 12: sprintf(dmgstr, " divine"); output(fd, dmgstr); break;
				}
			
				
				ANSI(fd, AM_NORMAL);
				ANSI(fd, AFC_WHITE);
			}
		}
		
		if (j == 0){
			
			if (ply_ptr->ready[WIELD-1]){
		   		
		   		damage_output(ply_ptr, total, ply_ptr->ready[WIELD-1]->type + 1);
		   		switch (ply_ptr->ready[WIELD-1]->type){
				case 0: {sprintf(dmgstr, " sharp"); break;}
				case 1: {sprintf(dmgstr, " thrust"); break;}
				case 2: {sprintf(dmgstr, " blunt"); break;}
				case 3: {sprintf(dmgstr, " pole"); break;}
				case 4: {sprintf(dmgstr, " missile"); break;}
		   		}
		   	}
		   	else{
		   		
		   		damage_output(ply_ptr, total, DHAND);
		   		sprintf(dmgstr, " hand");
		   		
		   	}
		   	output(fd, dmgstr);
			ANSI(fd, AM_NORMAL);
			ANSI(fd, AFC_WHITE);
		}
		output(fd, ")\n"); 

        /* if (ply_ptr->ready[WIELD-1]){
        	damage_output(ply_ptr, n, ply_ptr->ready[WIELD-1]->type + 1);
        	output(fd, " damage.");
        	ANSI(fd, AM_NORMAL);
			ANSI(fd, AFC_WHITE);
        	
        	/* I MEAN NO SORRY I DELETED IT
        	/*NEW BONUS DAMAGE HERE

        
        	if (bonusone){
        		output(fd, " Plus a bonus ");
        
        		damage_output(ply_ptr, bonusone, ply_ptr->ready[WIELD-1]->magicrealm);
        	
        
        		output(fd, " damage.");
        		ANSI(fd, AM_NORMAL);
				ANSI(fd, AFC_WHITE);
        	}
        	if (bonustwo){
        		output(fd, " Plus a bonus ");
        
        		damage_output(ply_ptr, bonustwo, ply_ptr->ready[WIELD-1]->special1);
        	
        
        		output(fd, " damage.");
        		ANSI(fd, AM_NORMAL);
				ANSI(fd, AFC_WHITE);
        	} 

        }
        else {damage_output(ply_ptr, n, DHAND);
        	output(fd, " damage.");
        	ANSI(fd, AM_NORMAL);
			ANSI(fd, AFC_WHITE);

        } OH WELL*/
               
		sprintf(g_buffer, "%%M hit you for %d damage.\n", n);
		mprint(crt_ptr->fd, g_buffer, m1arg(ply_ptr) );
	
		if(F_ISSET(crt_ptr, MNOPRE) || crt_ptr->type == PLAYER)
			sprintf(g_buffer, "%s %s %s!", ply_ptr->name,
				hit_description(n), crt_ptr->name);
		else
			sprintf(g_buffer, "%s %s the %s!", ply_ptr->name,
				hit_description(n), crt_ptr->name);

		broadcast_dam(ply_ptr->fd, crt_ptr->fd, ply_ptr->rom_num,
				g_buffer, NULL);

		if (ply_ptr->type == MONSTER && crt_ptr->type == MONSTER)
			broadcast_rom2(crt_ptr->fd,fd,crt_ptr->rom_num,
					"%M hits %m!", 
				m2args(ply_ptr,crt_ptr));

}