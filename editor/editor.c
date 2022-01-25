/*
 * EDITOR.C:
 *
 *      Object, creature, room and player editor routines.
 *
 *      Copyright (C) 1991 Brett J. Vickers
 *	Copyright 1993, 1999 Brooke Paul
 * $Id: editor.c,v 1.1 2001/08/14 23:13:08 develop Exp develop $
 *
 * $Log: editor.c,v $
 * Revision 1.1  2001/08/14 23:13:08  develop
 * Initial revision
 *
 *
 */
#include <stdlib.h>
#include <stdio.h>


#include "../include/port.h"
#include "../include/morutil.h"
#include "../include/mordb.h"


#include "editor.h"


int main()
{
        int i;

	umask(0);
	while(1) {
		clearscreen();
		posit(8,30);  printf("Mordor Editor");
		posit(10,30); printf("1. Edit Object");
		posit(11,30); printf("2. Edit Creature");
		posit(12,30); printf("3. Edit Room");
		posit(13,30); printf("4. Edit Player");
		posit(14,30); printf("5. Quit");
		posit(16,29); printf("6. Convert FROM csv database");
		posit(17,29); printf("7. Convert TO csv database");
		posit(19,30); printf("Choice: ");

		getnum(&i, 1, 7);

		switch(i) {
		case 1:
			edit_object();
			break;
		case 2:
			edit_monster();
			break;
		case 3:
			edit_room();
			break;
		case 4:
			edit_player();
			break;
		case 5:
			exit(0);
			break;
		case 6:
			convert_from_csv();
			break;
		case 7:
			convert_to_csv();
			break;
		}
	}

	return(0);
}

void convert_from_csv(){
	clearscreen();
	posit(3,20); printf("Converting from csv");
	return;
}

void convert_to_csv(){
	clearscreen();
	posit(3,20); printf("Converting to csv");

	int i, j, m, n, o, p, q, r;
	int flags0 = 0, flags1 = 0, flags2 = 0, flags3 = 0, flags4 = 0, flags5 = 0, flags6 = 0, flags7 = 0, flags8 = 0, flags9 = 0, flags10 = 0, flags11 = 0, flags12 = 0, flags13 = 0, flags14 = 0, flags15 = 0;
	int spells0 = 0, spells1 = 0, spells2 = 0, spells3 = 0, spells4 = 0, spells5 = 0, spells6 = 0, spells7 = 0, spells8 = 0, spells9 = 0, spells10 = 0, spells11 = 0, spells12 = 0, spells13 = 0, spells14 = 0, spells15 = 0;
	int quests0 = 0, quests1 = 0, quests2 = 0, quests3 = 0, quests4 = 0, quests5 = 0, quests6 = 0, quests7 = 0, quests8 = 0, quests9 = 0, quests10 = 0, quests11 = 0, quests12 = 0, quests13 = 0, quests14 = 0, quests15 = 0;
	
	posit(4,20); printf("converting monsters");
	FILE *fpt;
	fpt = fopen("monsters_datatbase.csv", "w+");
	fprintf(fpt, "ID; name; description; talk; password; key0; key1; key2; fd; level; type; class; race; numwander; alignment; strength; dexterity; constitution; intelligence; piety; hpmax; hpcur; mpmax; mpcur; armor; thaco; experience; gold; ndice; sdice; pdice; special; proficiency0; proficiency1; proficiency2; proficiency3; proficiency4; proficiency5; realm0; realm1; realm2; realm3; realm4; realm5; realm6; realm7; spells0; spells1; spells2; spells3; spells4; spells5; spells6; spells7; spells8; spells9; spells10; spells11; spells12; spells13; spells14; spells15; flags0; flags1; flags2; flags3; flags4; flags5; flags6; flags7; flags8; flags9; flags10; flags11; flags12; flags13; flags14; flags15; quests0; quests1; quests2; quests3; quests4; quests5; quests6; quests7; quests8; quests9; quests10; quests11; quests12; quests13; quests14; quests15; questnum; carry0; carry1; carry2; carry3; carry4; carry5; carry6; carry7; carry8; carry9; rom_num; bank_balance; title; misc_stats0; misc_stats1; misc_stats2; misc_stats3; misc_stats4; clanindex; clanexp; guildtype; guildexp; special1; special2\n");


	creature *crt_ptr;
	for (i=0, j=0; i<5000; i++){
		
		if (load_crt(i, &crt_ptr) > -1) {
		load_crt_from_file(i, &crt_ptr);

		posit(5,20); printf("converting monster %i\n", i);

		/*flag check, be sure to incement 'm' when you add more flags!*/
		for (m=0, n=0; m<128; m++){
			switch (n){
					case 0: flags0 = 0;
					case 1: flags1 = 0;
					case 2: flags2 = 0;
					case 3: flags3 = 0;
					case 4: flags4 = 0;
					case 5: flags5 = 0;
					case 6: flags6 = 0;
					case 7: flags7 = 0;
					case 8: flags8 = 0;
					case 9: flags9 = 0;
					case 10: flags10 = 0;
					case 11: flags11 = 0;
					case 12: flags12 = 0;
					case 13: flags13 = 0;
					case 14: flags14 = 0;
					case 15: flags15 = 0;
				}
			if (F_ISSET(crt_ptr, m)){
				switch (n){
					case 0: flags0 = m;
					case 1: flags1 = m;
					case 2: flags2 = m;
					case 3: flags3 = m;
					case 4: flags4 = m;
					case 5: flags5 = m;
					case 6: flags6 = m;
					case 7: flags7 = m;
					case 8: flags8 = m;
					case 9: flags9 = m;
					case 10: flags10 = m;
					case 11: flags11 = m;
					case 12: flags12 = m;
					case 13: flags13 = m;
					case 14: flags14 = m;
					case 15: flags15 = m;
				}
				n++;

			}
		}
		/*spell check, be sure to incement 'm' when you add more flags!*/
		for (o=0, p=0; o<128; o++){
			switch (p){
					case 0: spells0 = 0;
					case 1: spells1 = 0;
					case 2: spells2 = 0;
					case 3: spells3 = 0;
					case 4: spells4 = 0;
					case 5: spells5 = 0;
					case 6: spells6 = 0;
					case 7: spells7 = 0;
					case 8: spells8 = 0;
					case 9: spells9 = 0;
					case 10: spells10 = 0;
					case 11: spells11 = 0;
					case 12: spells12 = 0;
					case 13: spells13 = 0;
					case 14: spells14 = 0;
					case 15: spells15 = 0;
				}
			if (S_ISSET(crt_ptr, o)){
				switch (p){
					case 0: spells0 = o;
					case 1: spells1 = o;
					case 2: spells2 = o;
					case 3: spells3 = o;
					case 4: spells4 = o;
					case 5: spells5 = o;
					case 6: spells6 = o;
					case 7: spells7 = o;
					case 8: spells8 = o;
					case 9: spells9 = o;
					case 10: spells10 = o;
					case 11: spells11 = o;
					case 12: spells12 = o;
					case 13: spells13 = o;
					case 14: spells14 = o;
					case 15: spells15 = o;
				}
				p++;

			}
		}
		/*quests check, be sure to incement 'm' when you add more flags!*/
		for (q=0, r=0; q<128; q++){
			switch (r){
					case 0: quests0 = 0;
					case 1: quests1 = 0;
					case 2: quests2 = 0;
					case 3: quests3 = 0;
					case 4: quests4 = 0;
					case 5: quests5 = 0;
					case 6: quests6 = 0;
					case 7: quests7 = 0;
					case 8: quests8 = 0;
					case 9: quests9 = 0;
					case 10: quests10 = 0;
					case 11: quests11 = 0;
					case 12: quests12 = 0;
					case 13: quests13 = 0;
					case 14: quests14 = 0;
					case 15: quests15 = 0;
				}
			if (Q_ISSET(crt_ptr, q)){
				switch (r){
					case 0: quests0 = q;
					case 1: quests1 = q;
					case 2: quests2 = q;
					case 3: quests3 = q;
					case 4: quests4 = q;
					case 5: quests5 = q;
					case 6: quests6 = q;
					case 7: quests7 = q;
					case 8: quests8 = q;
					case 9: quests9 = q;
					case 10: quests10 = q;
					case 11: quests11 = q;
					case 12: quests12 = q;
					case 13: quests13 = q;
					case 14: quests14 = q;
					case 15: quests15 = q;
				}
				r++;

			}
		}


		fprintf(fpt, "%i; %s; %s; %s; %s; %s; %s; %s; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %s; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i \n",
		i, crt_ptr->name, crt_ptr->description, crt_ptr->talk, crt_ptr->password, crt_ptr->key[0], crt_ptr->key[1], crt_ptr->key[2], crt_ptr->fd, crt_ptr->level, crt_ptr->type, crt_ptr->class, crt_ptr->race, crt_ptr->numwander, crt_ptr->alignment, crt_ptr->strength, crt_ptr->dexterity, crt_ptr->constitution, crt_ptr->intelligence, crt_ptr->piety, crt_ptr->hpmax, crt_ptr->hpcur, crt_ptr->mpmax, crt_ptr->mpcur, crt_ptr->armor, crt_ptr->thaco, crt_ptr->experience, crt_ptr->gold, crt_ptr->ndice, crt_ptr->sdice, crt_ptr->pdice, crt_ptr->special, crt_ptr->proficiency[0], crt_ptr->proficiency[1], crt_ptr->proficiency[2], crt_ptr->proficiency[3], crt_ptr->proficiency[4], crt_ptr->proficiency[5], crt_ptr->realm[0], crt_ptr->realm[1], crt_ptr->realm[2], crt_ptr->realm[3], crt_ptr->realm[4], crt_ptr->realm[5], crt_ptr->realm[6], crt_ptr->realm[7], spells0, spells1, spells2, spells3, spells4, spells5, spells6, spells7, spells8, spells9, spells10, spells11, spells12, spells13, spells14, spells15, flags0, flags1, flags2, flags3, flags4, flags5, flags6, flags7, flags8, flags9, flags10, flags11, flags12, flags13, flags14, flags15, quests0, quests1, quests2, quests3, quests4, quests5, quests6, quests7, quests8, quests9, quests10, quests11, quests12, quests13, quests14, quests15, crt_ptr->questnum, crt_ptr->carry[0], crt_ptr->carry[1], crt_ptr->carry[2], crt_ptr->carry[3], crt_ptr->carry[4], crt_ptr->carry[5], crt_ptr->carry[6], crt_ptr->carry[7], crt_ptr->carry[8], crt_ptr->carry[9], crt_ptr->rom_num, crt_ptr->bank_balance, crt_ptr->title, crt_ptr->misc_stats[0], crt_ptr->misc_stats[1], crt_ptr->misc_stats[2], crt_ptr->misc_stats[3], crt_ptr->misc_stats[4], crt_ptr->clanindex, crt_ptr->clanexp, crt_ptr->guildtype, crt_ptr->guildexp, crt_ptr->special1, crt_ptr->special2 );
	}
	else {
		posit(8, 20); printf("error on monster %i \n", i );
	}
	}

	fclose(fpt);

	clearscreen();
	posit(4,20); printf("converting objects");
	
	fpt = fopen("objects_datatbase.csv", "w+");
	fprintf(fpt, "ID; name; description; key0; key1; key2; use; value; weight; type; adjustment; shotsmax; shotscur; ndice; sdice; pdice; armor; wearflag; magicpower; magicrealm; special; flags0; flags1; flags2; flags3; flags4; flags5; flags6; flags7; flags8; flags9; flags10; flags11; flags12; flags13; flags14; flags15; questnum; strength; dexterity; constitution; intelligence; piety; sets_flag0; sets_flag1; sets_flag2; sets_flag3; sets_flag4; sets_flag5; sets_flag6; sets_flag7; sets_flag8; sets_flag9; sets_flag10; sets_flag11; sets_flag12; sets_flag13; sets_flag14; sets_flag15; special1; special2\n");

	object *obj_ptr;
	for (i=0, j=0; i<5000; i++){
		
		if (load_obj(i, &obj_ptr) > -1) {
		load_obj_from_file(i, &obj_ptr);

		posit(5,20); printf("converting object %i\n", i);

		/*flag check, be sure to incement 'm' when you add more flags!*/
		for (m=0, n=0; m<128; m++){
			switch (n){
					case 0: flags0 = 0;
					case 1: flags1 = 0;
					case 2: flags2 = 0;
					case 3: flags3 = 0;
					case 4: flags4 = 0;
					case 5: flags5 = 0;
					case 6: flags6 = 0;
					case 7: flags7 = 0;
					case 8: flags8 = 0;
					case 9: flags9 = 0;
					case 10: flags10 = 0;
					case 11: flags11 = 0;
					case 12: flags12 = 0;
					case 13: flags13 = 0;
					case 14: flags14 = 0;
					case 15: flags15 = 0;
				}
			if (F_ISSET(obj_ptr, m)){
				switch (n){
					case 0: flags0 = m;
					case 1: flags1 = m;
					case 2: flags2 = m;
					case 3: flags3 = m;
					case 4: flags4 = m;
					case 5: flags5 = m;
					case 6: flags6 = m;
					case 7: flags7 = m;
					case 8: flags8 = m;
					case 9: flags9 = m;
					case 10: flags10 = m;
					case 11: flags11 = m;
					case 12: flags12 = m;
					case 13: flags13 = m;
					case 14: flags14 = m;
					case 15: flags15 = m;
				}
				n++;

			}
		}
		


		fprintf(fpt, "%i; %s; %s; %s; %s; %s; %s; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i\n", 
			i, obj_ptr->name, obj_ptr->description, obj_ptr->key[0], obj_ptr->key[1], obj_ptr->key[2], obj_ptr->use_output, obj_ptr->value, obj_ptr->weight, obj_ptr->type, obj_ptr->adjustment, obj_ptr->shotsmax, obj_ptr->shotscur, obj_ptr->ndice, obj_ptr->sdice, obj_ptr->pdice, obj_ptr->armor, obj_ptr->wearflag, obj_ptr->magicpower, obj_ptr->magicrealm, obj_ptr->special, flags0, flags1, flags2, flags3, flags4, flags5, flags6, flags7, flags8, flags9, flags10, flags11, flags12, flags13, flags14, flags15, obj_ptr->questnum, obj_ptr->strength, obj_ptr->dexterity, obj_ptr->constitution, obj_ptr->intelligence, obj_ptr->piety, obj_ptr->sets_flag[0], obj_ptr->sets_flag[1], obj_ptr->sets_flag[2], obj_ptr->sets_flag[3], obj_ptr->sets_flag[4], obj_ptr->sets_flag[5], obj_ptr->sets_flag[6], obj_ptr->sets_flag[7], obj_ptr->sets_flag[8], obj_ptr->sets_flag[9], obj_ptr->sets_flag[10], obj_ptr->sets_flag[11], obj_ptr->sets_flag[12], obj_ptr->sets_flag[13], obj_ptr->sets_flag[14], obj_ptr->sets_flag[15], obj_ptr->special1, obj_ptr->special2);
	}
	else {
		posit(8, 20); printf("error on object %i \n", i );
	}
	}
	fclose(fpt);

	clearscreen();
	posit(4,20); printf("converting rooms");
	
	fpt = fopen("rooms_datatbase.csv", "w+");
	fprintf(fpt, "rom_num; name; description; lolevel; hilevel; special; trap; trapexit; track; flags0; flags1; flags2; flags3; flags4; flags5; flags6; flags7; flags8; flags9; flags10; flags11; flags12; flags13; flags14; flags15; random0; random1; random2; random3; random4; random5; random6; random7; random8; random9; traffic; perm0; ptime0; perm1; ptime1; perm2; ptime2; perm3; ptime3; perm4; ptime4; perm5; ptime5; perm6; ptime6; perm7; ptime7; perm8; ptime8; perm9; ptime9; operm0; optime0; operm1; optime1; operm2; optime2; operm3; optime3; operm4; optime4; operm5; optime5; operm6; optime6; operm7; optime7; operm8; optime8; operm9; optime9; beenhere; x1name; x1destination; x1key; x1flag0; x1flag1; x1flag2; x1flag3; x1flag4; x1flag5; x1flag6; x1flag7; x1flag8; x1flag9; x2name; x2destination; x2key; x2flag0; x2flag1; x2flag2; x2flag3; x2flag4; x2flag5; x2flag6; x2flag7; x2flag8; x2flag9; x3name; x3destination; x3key; x3flag0; x3flag1; x3flag2; x3flag3; x3flag4; x3flag5; x3flag6; x3flag7; x3flag8; x3flag9; x4name; x4destination; x4key; x4flag0; x4flag1; x4flag2; x4flag3; x4flag4; x4flag5; x4flag6; x4flag7; x4flag8; x4flag9; x5name; x5destination; x5key; x5flag0; x5flag1; x5flag2; x5flag3; x5flag4; x5flag5; x5flag6; x5flag7; x5flag8; x5flag9; x6name; x6destination; x6key; x6flag0; x6flag1; x6flag2; x6flag3; x6flag4; x6flag5; x6flag6; x6flag7; x6flag8; x6flag9; x7name; x7destination; x7key; x7flag0; x7flag1; x7flag2; x7flag3; x7flag4; x7flag5; x7flag6; x7flag7; x7flag8; x7flag9; x8name; x8destination; x8key; x8flag0; x8flag1; x8flag2; x8flag3; x8flag4; x8flag5; x8flag6; x8flag7; x8flag8; x8flag9\n");

	char xnames[8][20];
	char str[2000];
	int len;
	int xrooms[8];
	int xkeys[8];
	int x1flags[10];
	int x2flags[10];
	int x3flags[10];
	int x4flags[10];
	int x5flags[10];
	int x6flags[10];
	int x7flags[10];
	int x8flags[10];

	room *rom_ptr;
	xtag *xt;
	exit_ *exit;

	
	for (i=0, j=0; i<15000; i++){
				for (o=0; o<10; o++){
										
					x1flags[o] = 0;
					x2flags[o] = 0;
					x3flags[o] = 0;
					x4flags[o] = 0;
					x5flags[o] = 0;
					x6flags[o] = 0;
					x7flags[o] = 0;
					x8flags[o] = 0;
					
				}	
		
	if (load_rom(i, &rom_ptr) > -1) {
		load_rom_from_file(i, &rom_ptr);

		posit(5,20); printf("converting room %i\n", i);

		xt = rom_ptr->first_ext;

		memset(str,0,strlen(str));
		if (rom_ptr->long_desc){
		strcpy(str, rom_ptr->long_desc);
		len = strlen(str);
			for (m=0; m<len-1; m++){
				if (str[m] == '\n' || str[m] == ';'){
					str[m] = ' ';
				}
			}
		}
		
		
		for (m=0, n=0; m<8; m++){
			
			strcpy(xnames[m], " ");
			xrooms[m] = 0;
			xkeys[m] = 0;

			if (xt){
				
				strcpy(xnames[m], xt->ext->name);
				
				xrooms[m] = xt->ext->room;
				xkeys[m] = xt->ext->key;
				
				
				for (o=0, p=0; o<128; o++){
					if (F_ISSET(xt->ext, o)){
						if (p < 8){
							switch (m){
								case 0: x1flags[p] = o;
								case 1: x2flags[p] = o;
								case 2: x3flags[p] = o;
								case 3: x4flags[p] = o;
								case 4: x5flags[p] = o;
								case 5: x6flags[p] = o;
								case 6: x7flags[p] = o;
								case 7: x8flags[p] = o;
							}
						}
						p++;
					}
				}
			xt = xt->next_tag;
			}
			/*else{
				
				strcpy(xnames[m], " ");
				
				xrooms[m] = 0;
				xkeys[m] = 0;
				for (o=0; o<8; o++){
					switch (m){
						case 0: x1flags[o] = 0;
						case 1: x2flags[o] = 0;
						case 2: x3flags[o] = 0;
						case 3: x4flags[o] = 0;
						case 4: x5flags[o] = 0;
						case 5: x6flags[o] = 0;
						case 6: x7flags[o] = 0;
						case 7: x8flags[o] = 0;
					}
				}
			}*/

			
			/*if (xt){
			xt = xt->next_tag;}*/
			
		}
		
		/*flag check, be sure to incement 'm' when you add more flags!*/
		for (m=0, n=0; m<128; m++){
			switch (n){
					case 0: flags0 = 0;
					case 1: flags1 = 0;
					case 2: flags2 = 0;
					case 3: flags3 = 0;
					case 4: flags4 = 0;
					case 5: flags5 = 0;
					case 6: flags6 = 0;
					case 7: flags7 = 0;
					case 8: flags8 = 0;
					case 9: flags9 = 0;
					case 10: flags10 = 0;
					case 11: flags11 = 0;
					case 12: flags12 = 0;
					case 13: flags13 = 0;
					case 14: flags14 = 0;
					case 15: flags15 = 0;
				}
			if (F_ISSET(rom_ptr, m)){
				switch (n){
					case 0: flags0 = m;
					case 1: flags1 = m;
					case 2: flags2 = m;
					case 3: flags3 = m;
					case 4: flags4 = m;
					case 5: flags5 = m;
					case 6: flags6 = m;
					case 7: flags7 = m;
					case 8: flags8 = m;
					case 9: flags9 = m;
					case 10: flags10 = m;
					case 11: flags11 = m;
					case 12: flags12 = m;
					case 13: flags13 = m;
					case 14: flags14 = m;
					case 15: flags15 = m;
				}
				n++;

			}
		}
		


		fprintf(fpt, "%i; %s; %s; %i; %i; %i; %i; %i; %s; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %s; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %s; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %s; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %s; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %s; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %s; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %s; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %s; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i; %i\n",
		i, rom_ptr->name, str, rom_ptr->lolevel, rom_ptr->hilevel, rom_ptr->special, rom_ptr->trap, rom_ptr->trapexit, rom_ptr->track, flags0, flags1, flags2, flags3, flags4, flags5, flags6, flags7, flags8, flags9, flags10, flags11, flags12, flags13, flags14, flags15, rom_ptr->random[0], rom_ptr->random[1], rom_ptr->random[2], rom_ptr->random[3], rom_ptr->random[4], rom_ptr->random[5], rom_ptr->random[6], rom_ptr->random[7], rom_ptr->random[8], rom_ptr->random[9], rom_ptr->traffic,
		rom_ptr->perm_mon[0].misc, rom_ptr->perm_mon[0].interval, rom_ptr->perm_mon[1].misc, rom_ptr->perm_mon[1].interval, rom_ptr->perm_mon[2].misc, rom_ptr->perm_mon[2].interval, rom_ptr->perm_mon[3].misc, rom_ptr->perm_mon[3].interval, rom_ptr->perm_mon[4].misc, rom_ptr->perm_mon[4].interval, rom_ptr->perm_mon[5].misc, rom_ptr->perm_mon[5].interval, rom_ptr->perm_mon[6].misc, rom_ptr->perm_mon[6].interval, rom_ptr->perm_mon[7].misc, rom_ptr->perm_mon[7].interval, rom_ptr->perm_mon[8].misc, rom_ptr->perm_mon[8].interval, rom_ptr->perm_mon[9].misc, rom_ptr->perm_mon[9].interval,
		rom_ptr->perm_obj[0].misc, rom_ptr->perm_obj[0].interval, rom_ptr->perm_obj[1].misc, rom_ptr->perm_obj[1].interval, rom_ptr->perm_obj[2].misc, rom_ptr->perm_obj[2].interval, rom_ptr->perm_obj[3].misc, rom_ptr->perm_obj[3].interval, rom_ptr->perm_obj[4].misc, rom_ptr->perm_obj[4].interval, rom_ptr->perm_obj[5].misc, rom_ptr->perm_obj[5].interval, rom_ptr->perm_obj[6].misc, rom_ptr->perm_obj[6].interval, rom_ptr->perm_obj[7].misc, rom_ptr->perm_obj[7].interval, rom_ptr->perm_obj[8].misc, rom_ptr->perm_obj[8].interval, rom_ptr->perm_obj[9].misc, rom_ptr->perm_obj[9].interval, 
		rom_ptr->beenhere, 
		xnames[0], xrooms[0], xkeys[0], x1flags[0], x1flags[1], x1flags[2], x1flags[3], x1flags[4], x1flags[5], x1flags[6], x1flags[7], x1flags[8], x1flags[9],
		xnames[1], xrooms[1], xkeys[1], x2flags[0], x2flags[1], x2flags[2], x2flags[3], x2flags[4], x2flags[5], x2flags[6], x2flags[7], x2flags[8], x2flags[9],
		xnames[2], xrooms[2], xkeys[2], x3flags[0], x3flags[1], x3flags[2], x3flags[3], x3flags[4], x3flags[5], x3flags[6], x3flags[7], x3flags[8], x3flags[9],
		xnames[3], xrooms[3], xkeys[3], x4flags[0], x4flags[1], x4flags[2], x4flags[3], x4flags[4], x4flags[5], x4flags[6], x4flags[7], x4flags[8], x4flags[9],
		xnames[4], xrooms[4], xkeys[4], x5flags[0], x5flags[1], x5flags[2], x5flags[3], x5flags[4], x5flags[5], x5flags[6], x5flags[7], x5flags[8], x5flags[9],
		xnames[5], xrooms[5], xkeys[5], x6flags[0], x6flags[1], x6flags[2], x6flags[3], x6flags[4], x6flags[5], x6flags[6], x6flags[7], x6flags[8], x6flags[9],
		xnames[6], xrooms[6], xkeys[6], x7flags[0], x7flags[1], x7flags[2], x7flags[3], x7flags[4], x7flags[5], x7flags[6], x7flags[7], x7flags[8], x7flags[9],
		xnames[7], xrooms[7], xkeys[7], x8flags[0], x8flags[1], x8flags[2], x8flags[3], x8flags[4], x8flags[5], x8flags[6], x8flags[7], x8flags[8], x8flags[9]);
		
	}
	else {
		posit(8, 20); printf("error on room %i \n", i );
	}
	}

	fclose(fpt);

	return;
}

void edit_object()
{
	int 	i, k,num;
	long	j;
	char	str[256];
	object	*obj_ptr;

	clearscreen();
	posit(3,20); printf("Enter object number to edit: ");
	getnum(&num, 0, 5000);

	load_obj_from_file(num, &obj_ptr);

	while(1) {
		clearscreen();
		posit(2,25);  printf("Object: (%d) %s", num, obj_ptr->name);
		posit(4,30);  printf(" 1. Edit object name/description");
		posit(5,30);  printf(" 2. Edit key names");
		posit(6,30);  printf(" 3. Edit output string");
		posit(7,30);  printf(" 4. Edit value/weight/type/adj/quest");
		posit(8,30);  printf(" 5. Edit shots/dmg/armor/wear/mgc/spcl");
		posit(9,30);  printf(" 6. Edit flags");
		posit(10,30); printf(" 7. Clear this object");
		posit(11,30); printf(" 8. Abort edit");
		posit(12,30); printf(" 9. Back to main menu");

		posit(15,30); printf("Choice: ");

		getnum(&i, 1, 9);

		switch(i) {
		case 1:			/* name */
			clearscreen();
			posit(2,1); printf("Object's name:");
			posit(3,1); printf("%s", obj_ptr->name);
			posit(5,1); printf("Description:");
			posit(6,1); printf("%s", obj_ptr->description);
			posit(3,1); getstr(str, obj_ptr->name, 80);
			strcpy(obj_ptr->name, str);
			posit(6,1); getstr(str, obj_ptr->description, 80);
			strcpy(obj_ptr->description, str);
			break;
		case 2:       		/* keys */
			clearscreen();
			posit(2,1); printf("Keys:");
			posit(3,1); printf("1:%s", obj_ptr->key[0]);
			posit(3,27); printf("2:%s", obj_ptr->key[1]);
			posit(3,54); printf("3:%s", obj_ptr->key[2]);
			posit(3,3); getstr(str, obj_ptr->key[0], 20);
			strcpy(obj_ptr->key[0], str);
			posit(3,29); getstr(str, obj_ptr->key[1], 20);
			strcpy(obj_ptr->key[1], str);
			posit(3,56); getstr(str, obj_ptr->key[2], 20);
			strcpy(obj_ptr->key[2], str);
			break;
		case 3:			/* output string */
			clearscreen();
			posit(2,1); printf("Current output string:");
			posit(3,1); printf("%s", obj_ptr->use_output);
			posit(3,1); getstr(str, obj_ptr->use_output, 80);
			strcpy(obj_ptr->use_output, str);
			break;
		case 4:			/* value/weight/type/adj/quest */
			clearscreen();
			posit(2,1); printf("Value: %-8d", obj_ptr->value);
			posit(2,30); printf("Weight: %-5d",obj_ptr->weight);
			posit(3,1); printf(" Type: %-3d", obj_ptr->type);
			posit(3,30); printf("   Adj: %-3d", obj_ptr->adjustment);
			posit(3,54); printf("Quest: %-3d", obj_ptr->questnum);
			posit(2,8); getnums(&j, (long)obj_ptr->value, 0L, 1000000);
			obj_ptr->value = j;
			posit(2,38); getnums(&j, (long)obj_ptr->weight, 0L, 32000L);
			obj_ptr->weight = j;
			posit(3,8); getnums(&j, (long)obj_ptr->type, -127L, 127L);
			obj_ptr->type = j;
			posit(3,38); getnums(&j, (long)obj_ptr->adjustment, -127L, 127L);
			obj_ptr->adjustment = j;
			posit(3,61); getnums(&j, (long)obj_ptr->questnum, -1L, 127L);
			obj_ptr->questnum = j;
			break;
		case 5:			/* shots/dmg/armor/wear/magic */
			clearscreen();
			posit(2,1); printf(" Shots: %-5d/%-5d", obj_ptr->shotscur, obj_ptr->shotsmax);
			posit(3,1); printf("# Dice: %-5d", obj_ptr->ndice);
			posit(3,30); printf(" Sides: %-5d", obj_ptr->sdice);
			posit(3,60); printf("  Plus: %-5d", obj_ptr->pdice);
			posit(4,1); printf(" Armor: %-3d", obj_ptr->armor);
			posit(4,30); printf("  Wear: %-3d", obj_ptr->wearflag);
			posit(4,60); printf(" Magic: %-3d", obj_ptr->magicpower);
			posit(5,30); printf(" Realm: %-1d", obj_ptr->magicrealm);
			posit(5,59); printf("Special: %-3d", obj_ptr->special);
			posit(2,9); getnums(&j, (long)obj_ptr->shotscur, 0L, 32000L);
			obj_ptr->shotscur = j;
			posit(2,15); getnums(&j, (long)obj_ptr->shotsmax, 0L, 32000L);
			obj_ptr->shotsmax = j;
			posit(3,9); getnums(&j, (long)obj_ptr->ndice, 0L, 32000L);
			obj_ptr->ndice = j;
			posit(3,38); getnums(&j, (long)obj_ptr->sdice, 0L, 32000L);
			obj_ptr->sdice = j;
			posit(3,68); getnums(&j, (long)obj_ptr->pdice, 0L, 32000L);
			obj_ptr->pdice = j;
			posit(4,9); getnums(&j, (long)obj_ptr->armor, -127L, 127L);
			obj_ptr->armor = j;
			posit(4,38); getnums(&j, (long)obj_ptr->wearflag, -127L, 127L);
			obj_ptr->wearflag = j;
			posit(4,68); getnums(&j, (long)obj_ptr->magicpower, -127L, 127L);
			obj_ptr->magicpower = j;
			posit(5,38); getnums(&j, (long)obj_ptr->magicrealm, 0L, 128L);
			obj_ptr->magicrealm = j;
			posit(5,68); getnums(&j, (long)obj_ptr->special, 0L, 32000L);
			obj_ptr->special = j;
			break;
		case 6:			/* flags */
			clearscreen();
			posit(2,10); printf("1");
			posit(2,20); printf("2");
			posit(2,30); printf("3");
			posit(2,40); printf("4");
			posit(2,50); printf("5");
			posit(2,60); printf("6");
			posit(3,1); printf("1234567890123456789012345678901234567890123456789012345678901234");
			posit(7,1); printf("Flag #: ");
			posit(8,1); printf("(0 to exit)");
			while(1) {
				for(k=0;k<64;k++)
					str[k] = (obj_ptr->flags[k/8] & 1<<(k%8)) ? '*':'.';
				str[64] = 0;
				posit(4,1); printf("%s",str);
				posit(7,9); getnum(&k, 0, 64);
				if(!k) break;
				if(obj_ptr->flags[(k-1)/8] & 1<<((k-1)%8))
					obj_ptr->flags[(k-1)/8] &= ~(1<<((k-1)%8));
				else
					obj_ptr->flags[(k-1)/8] |= 1<<((k-1)%8);
			}
			break;
		case 7:
			clearscreen();
			posit(1,1);
			printf("Are you sure you want to clear it? ");
			getstr(str, "n", 2);
			if(str[0]=='y' || str[0]=='Y')
				zero(obj_ptr, sizeof(object));
			break;
		case 8:
			free(obj_ptr);
			return;
		case 9:
			save_obj_to_file(num, obj_ptr);
			free(obj_ptr);
			return;
		}
	}
}

void edit_monster()
{
	int 		i, k,num;
	long 		j;
	char		str[256];
	creature	*crt_ptr;

	clearscreen();
	posit(3,20); printf("Enter creature number to edit: ");
	getnum(&num, 0, 5000);

	load_crt_from_file(num, &crt_ptr);
	crt_ptr->type = 1;			/* monster */

	while(1) {
		clearscreen();
		posit(2,25);  printf("Creature: (%d) %s", num, crt_ptr->name);
		posit(4,30);  printf(" 1. Edit creature name/desc/talk");
		posit(5,30);  printf(" 2. Edit creature keys");
		posit(6,30);  printf(" 3. Edit level/class/align/stats");
		posit(7,30);  printf(" 4. Edit HP/MP/armor/thaco/num");
		posit(8,30);  printf(" 5. Edit exp/gold/dmg/quest/special");
		posit(9,30);  printf(" 6. Edit spells");
		posit(10,30); printf(" 7. Edit flags");
		posit(11,30); printf(" 8. Edit proficiencies");
		posit(12,30); printf(" 9. Edit carryable objects");
		posit(13,30); printf("10. Clear this creature");
		posit(14,30); printf("11. Abort edit");
		posit(15,30); printf("12. Back to main menu");
		posit(18,30); printf("Choice: ");

		getnum(&i, 1, 12);

		switch(i) {
		case 1:
			clearscreen();
			posit(2,1); printf("Creature name:");
			posit(3,1); printf("%s",crt_ptr->name);
			posit(5,1); printf("Description:");
			posit(6,1); printf("%s",crt_ptr->description);
			posit(8,1); printf("%s", "Talk:");
			posit(9,1); printf("%s",crt_ptr->talk);
			posit(3,1); getstr(str, crt_ptr->name, 80);
			strcpy(crt_ptr->name, str);
			posit(6,1); getstr(str, crt_ptr->description, 80);
			strcpy(crt_ptr->description, str);
			posit(9,1); getstr(str, crt_ptr->talk, 80);
			strcpy(crt_ptr->talk, str);
			break;
		case 2:			/* keys */
			clearscreen();
			posit(2,1); printf("Keys:");
			posit(3,1); printf("1:%s", crt_ptr->key[0]);
			posit(3,27); printf("2:%s", crt_ptr->key[1]);
			posit(3,54); printf("3:%s", crt_ptr->key[2]);
			posit(3,3); getstr(str, crt_ptr->key[0], 20);
			strcpy(crt_ptr->key[0], str);
			posit(3,29); getstr(str, crt_ptr->key[1], 20);
			strcpy(crt_ptr->key[1], str);
			posit(3,56); getstr(str, crt_ptr->key[2], 20);
			strcpy(crt_ptr->key[2], str);
			break;
		case 3:			/* level/class/align/stats */
			clearscreen();
			posit(2,1); printf("Level: %-3d", crt_ptr->level);
			posit(2,20); printf("Class: %-3d", crt_ptr->class);
			posit(2,40); printf("Align: %-5d", crt_ptr->alignment);
			posit(3,1); printf("  Str: %-2d", crt_ptr->strength);
			posit(3,20); printf("  Dex: %-2d", crt_ptr->dexterity);
			posit(3,40); printf("  Con: %-2d", crt_ptr->constitution);
			posit(4,1); printf("  Int: %-2d", crt_ptr->intelligence);
			posit(4,20); printf("  Pty: %-2d", crt_ptr->piety);
			posit(2,8); getnums(&j, (long)crt_ptr->level, 0L, 127L);
			crt_ptr->level = j;
			posit(2,27); getnums(&j, (long)crt_ptr->class, -127L, 127L);
			crt_ptr->class = j;
			posit(2,47); getnums(&j, (long)crt_ptr->alignment, -30000L, 30000L);
			crt_ptr->alignment = j;
			posit(3,8); getnums(&j, (long)crt_ptr->strength, 0L, 25L);
			crt_ptr->strength = j;
			posit(3,27); getnums(&j, (long)crt_ptr->dexterity, 0L, 25L);
			crt_ptr->dexterity = j;
			posit(3,47); getnums(&j, (long)crt_ptr->constitution, 0L, 25L);
			crt_ptr->constitution = j;
			posit(4,8); getnums(&j, (long)crt_ptr->intelligence, 0L, 25L);
			crt_ptr->intelligence = j;
			posit(4,27); getnums(&j, (long)crt_ptr->piety, 0L, 25L);
			crt_ptr->piety = j;
			break;
		case 4:			/* HP/MP/Armor/Thaco */
			clearscreen();
			posit(2,1); printf("   HP: %-5d/%-5d", crt_ptr->hpcur, crt_ptr->hpmax);
			posit(2,30); printf("   MP: %-5d/%-5d", crt_ptr->mpcur, crt_ptr->mpmax);
			posit(3,1); printf("Armor: %-3d", crt_ptr->armor);
			posit(3,30); printf("Thac0: %-3d", crt_ptr->thaco);
			posit(4,1); printf("  Num: %-3d", crt_ptr->numwander);
			posit(2,8); getnums(&j, (long)crt_ptr->hpcur, 0L, 32000L);
			crt_ptr->hpcur = j;
			posit(2,14); getnums(&j, (long)crt_ptr->hpmax, 0L, 32000L);
			crt_ptr->hpmax = j;
			posit(2,37); getnums(&j, (long)crt_ptr->mpcur, 0L, 32000L);
			crt_ptr->mpcur = j;
			posit(2,43); getnums(&j, (long)crt_ptr->mpmax, 0L, 32000L);
			crt_ptr->mpmax = j;
			posit(3,8); getnums(&j, (long)crt_ptr->armor, -127L, 127L);
			crt_ptr->armor = j;
			posit(3,37); getnums(&j, (long)crt_ptr->thaco, -127L, 127L);
			crt_ptr->thaco = j;
			posit(4,8); getnums(&j, (long)crt_ptr->numwander, -127L, 127L);
			crt_ptr->numwander = j;
			break;
		case 5: 		/* exp/gold/dmg/quest */
			clearscreen();
			posit(2,1); printf("Experience: %ld", crt_ptr->experience);
			posit(2,30); printf("  Gold: %ld", crt_ptr->gold);
			posit(3,1); printf("    # Dice: %-5d", crt_ptr->ndice);
			posit(3,30); printf(" Sides: %-5d", crt_ptr->sdice);
			posit(3,60); printf("  Plus: %-5d", crt_ptr->pdice);
			posit(4,1); printf("     Quest: %-3d", crt_ptr->questnum);
			posit(4,29); printf("Special: %-3d", crt_ptr->special);
			posit(2,13); getnums(&j, (long)crt_ptr->experience, 0L, 10000000L);
			crt_ptr->experience = j;
			posit(2,38); getnums(&j, (long)crt_ptr->gold, 0L, 20000000L);
			crt_ptr->gold = j;
			posit(3,13); getnums(&j, (long)crt_ptr->ndice, 0L, 32000L);
			crt_ptr->ndice = j;
			posit(3,38); getnums(&j, (long)crt_ptr->sdice, 0L, 32000L);
			crt_ptr->sdice = j;
			posit(3,68); getnums(&j, (long)crt_ptr->pdice, 0L, 32000L);
			crt_ptr->pdice = j;
			posit(4,13); getnums(&j, (long)crt_ptr->questnum, -1L, 127L);
			crt_ptr->questnum = j;
			posit(4,38); getnums(&j, (long)crt_ptr->special, 0L, 32000L);
			crt_ptr->special = j;
			break;
		case 6:		   	/* spells */
			clearscreen();
			posit(2,10); printf("1");
			posit(2,20); printf("2");
			posit(2,30); printf("3");
			posit(2,40); printf("4");
			posit(2,50); printf("5");
			posit(2,60); printf("6");
			posit(2,70); printf("7");
			posit(7,10); printf("8");
			posit(7,20); printf("9");
			posit(7,30); printf("0");
			posit(7,40); printf("1");
			posit(7,50); printf("2");
			posit(6,30); printf("1");
			posit(6,40); printf("1");
			posit(6,50); printf("1");
			posit(3,1); printf("1234567890123456789012345678901234567890123456789012345678901234567890");
			posit(8,1); printf("1234567890123456789012345678901234567890123456789012345678");
			posit(11,1); printf("Spell #: ");
			posit(12,1); printf("(0 to exit)");
			while(1) {
				for(k=0;k<128;k++)
					str[k] = (crt_ptr->spells[k/8] & 1<<(k%8)) ? '*':'.';
				str[128] = 0; k = str[70];
				str[70] = 0;
				posit(4,1); printf("%s",str);
				str[70] = k;
				posit(9,1); printf("%s",&str[70]);
				posit(11,10); getnum(&k, 0, 128);
				if(!k) break;
				if(crt_ptr->spells[(k-1)/8] & 1<<((k-1)%8))
					crt_ptr->spells[(k-1)/8] &= ~(1<<((k-1)%8));
				else
					crt_ptr->spells[(k-1)/8] |= 1<<((k-1)%8);
			}
			break;			
		case 7:			/* flags */
			clearscreen();
			posit(2,10); printf("1");
			posit(2,20); printf("2");
			posit(2,30); printf("3");
			posit(2,40); printf("4");
			posit(2,50); printf("5");
			posit(2,60); printf("6");
			posit(3,1); printf("1234567890123456789012345678901234567890123456789012345678901234");
			posit(7,1); printf("Flag #: ");
			posit(8,1); printf("(0 to exit)");
			while(1) {
				for(k=0;k<64;k++)
					str[k] = (crt_ptr->flags[k/8] & 1<<(k%8)) ? '*':'.';
				str[64] = 0;
				posit(4,1); printf("%s",str);
				posit(7,9); getnum(&k, 0, 64);
				if(!k) break;
				if(crt_ptr->flags[(k-1)/8] & 1<<((k-1)%8))
					crt_ptr->flags[(k-1)/8] &= ~(1<<((k-1)%8));
				else
					crt_ptr->flags[(k-1)/8] |= 1<<((k-1)%8);
			}
			break;
		case 8:
			clearscreen();
			posit(2,1); printf("Proficiencies:");
			posit(3,1); printf("1:%ld", crt_ptr->proficiency[0]);
			posit(3,27); printf("2:%ld", crt_ptr->proficiency[1]);
			posit(3,54); printf("3:%ld", crt_ptr->proficiency[2]);
			posit(4,1); printf("4:%ld", crt_ptr->proficiency[3]);
			posit(4,27); printf("5:%ld", crt_ptr->proficiency[4]);
			posit(3,3); getnums(&j, (long)crt_ptr->proficiency[0], 0L, 1000000L);
			crt_ptr->proficiency[0] = j;
			posit(3,29); getnums(&j, (long)crt_ptr->proficiency[1], 0L, 1000000L);
			crt_ptr->proficiency[1] = j;
			posit(3,56); getnums(&j, (long)crt_ptr->proficiency[2], 0L, 1000000L);
			crt_ptr->proficiency[2] = j;
			posit(4,3); getnums(&j, (long)crt_ptr->proficiency[3], 0L, 1000000L);
			crt_ptr->proficiency[3] = j;
			posit(4,29); getnums(&j, (long)crt_ptr->proficiency[4], 0L, 1000000L);
			crt_ptr->proficiency[4] = j;
			break;
		case 9:			/* carryables */
			clearscreen();
			posit(2,1); printf("Items carried:");
			posit(3,1); printf("1:%-5d",crt_ptr->carry[0]);
			posit(3,17); printf("2:%-5d",crt_ptr->carry[1]);
			posit(3,33); printf("3:%-5d",crt_ptr->carry[2]);
			posit(3,49); printf("4:%-5d",crt_ptr->carry[3]);
			posit(3,65); printf("5:%-5d",crt_ptr->carry[4]);
			posit(4,1); printf("6:%-5d",crt_ptr->carry[5]);
			posit(4,17); printf("7:%-5d",crt_ptr->carry[6]);
			posit(4,33); printf("8:%-5d",crt_ptr->carry[7]);
			posit(4,49); printf("9:%-5d",crt_ptr->carry[8]);
			posit(4,64); printf("10:%-5d",crt_ptr->carry[9]);
			posit(3,3); getnums(&j, (long)crt_ptr->carry[0], 0L, 32000L);
			crt_ptr->carry[0] = j;
			posit(3,19); getnums(&j, (long)crt_ptr->carry[1], 0L, 32000L);
			crt_ptr->carry[1] = j;
			posit(3,35); getnums(&j, (long)crt_ptr->carry[2], 0L, 32000L);
			crt_ptr->carry[2] = j;
			posit(3,51); getnums(&j, (long)crt_ptr->carry[3], 0L, 32000L);
			crt_ptr->carry[3] = j;
			posit(3,67); getnums(&j, (long)crt_ptr->carry[4], 0L, 32000L);
			crt_ptr->carry[4] = j;
			posit(4,3); getnums(&j, (long)crt_ptr->carry[5], 0L, 32000L);
			crt_ptr->carry[5] = j;
			posit(4,19); getnums(&j, (long)crt_ptr->carry[6], 0L, 32000L);
			crt_ptr->carry[6] = j;
			posit(4,35); getnums(&j, (long)crt_ptr->carry[7], 0L, 32000L);
			crt_ptr->carry[7] = j;
			posit(4,51); getnums(&j, (long)crt_ptr->carry[8], 0L, 32000L);
			crt_ptr->carry[8] = j;
			posit(4,67); getnums(&j, (long)crt_ptr->carry[9], 0L, 32000L);
			crt_ptr->carry[9] = j;
			break;
		case 10:
			clearscreen();
			posit(1,1);
			printf("Are you sure you want to clear it? ");
			getstr(str, "n", 2);
			if(str[0]=='y' || str[0]=='Y')
				zero(crt_ptr, sizeof(creature));
			break;
		case 11:
			free(crt_ptr);
			return;
		case 12:
			save_crt_to_file(num, crt_ptr);
			free(crt_ptr);
			return;
		}
	}
}

void edit_room()
{
	int 	i, k,num, x;
	long 	j;
	char	str[256];
	char	*shortd, *longd;
	room	*rom_ptr;

	clearscreen();
	posit(3,20); printf("Enter room number to edit: ");
	getnum(&num, 0, 15000);

	load_rom_from_file(num, &rom_ptr);
	rom_ptr->rom_num = num;
	if(!rom_ptr->established)
		rom_ptr->established = time(0);

	while(1) {
		clearscreen();
		posit(2,25); printf("Room: (%d) %s",num,rom_ptr->name);
		posit(3,30);  printf(" 1. Edit name");
		posit(4,30);  printf(" 2. Edit short description");
		posit(5,30);  printf(" 3. Edit long description");
		posit(6,30);  printf(" 4. Edit level/trap/track/special/etc");
		posit(7,30);  printf(" 5. Edit flags");
		posit(8,30);  printf(" 6. Edit random monsters");
		posit(9,30);  printf(" 7. Edit permanent monsters");
		posit(10,30); printf(" 8. Edit permanent objects");
		posit(11,30); printf(" 9. Edit room statistics");
		posit(12,30); printf("10. Edit exits");
		posit(13,30); printf("11. Clear this room");
		posit(14,30); printf("12. Abort this room");
		posit(15,30); printf("13. Back to main menu");
		posit(18,30); printf("Choice: ");

		getnum(&i, 1, 13);

		switch(i) {
		case 1:
			clearscreen();
			posit(2,1); printf("Room name:");
			posit(3,1); printf("%s",rom_ptr->name);
			posit(3,1); getstr(str, rom_ptr->name, 80);
			strcpy(rom_ptr->name, str);
			break;
		case 2:
			clearscreen();
			posit(1,1); printf("Current short description:");
			posit(2,1); printf("%s",rom_ptr->short_desc);
			posit(14,1); printf("Enter new description:");
			x = 15; 
			do {
				posit(x,1); printf("]");
				posit(x,2); getstr(str, "", 80);
				if(!str[0])
					break;
				if(x==15) {
					shortd = (char *)malloc(2000);
					shortd[0] = 0;
				}
				strcat(shortd, str);
				strcat(shortd, "\n");
				x++;
			} while(x < 24);
			if(x>15) {
				if(rom_ptr->short_desc)
					free(rom_ptr->short_desc);
				shortd[strlen(shortd)-1] = 0;
				rom_ptr->short_desc = shortd;
			}
			break;
		case 3:
			clearscreen();
			posit(1,1); printf("Current long description:");
			posit(2,1); printf("%s",rom_ptr->long_desc);
			posit(14,1); printf("Enter new description:");
			x = 15; 
			do {
				posit(x,1); printf("]");
				posit(x,2); getstr(str, "", 80);
				if(!str[0])
					break;
				if(x==15) {
					longd = (char *)malloc(2000);
					longd[0] = 0;
				}
				strcat(longd, str);
				strcat(longd, "\n");
				x++;
			} while(x < 25);
			if(x>15) {
				if(rom_ptr->long_desc)
					free(rom_ptr->long_desc);
				longd[strlen(longd)-1] = 0;
				rom_ptr->long_desc = longd;
			}
			break;
		case 4:			/* level/trap/track */
			clearscreen();
			posit(2,1); printf("Trap: %-3d", rom_ptr->trap);
			posit(2,30); printf("Trap exit: %-5d", rom_ptr->trapexit);
			posit(2,59); printf("Special: %-3d", rom_ptr->special);
			posit(4,1); printf("Track string:");
			posit(5,1); printf("%s",rom_ptr->track);
			posit(7,1); printf("Low level: %-2d", rom_ptr->lolevel);
			posit(8,1); printf("Hi level: %-2d", rom_ptr->hilevel);
			posit(9,1); printf("Death Room: %-2d", rom_ptr->death_rom);
			posit(10,1); printf("Zone: %-2d", (int)rom_ptr->zone_type);
			posit(10,30); printf("Subzone: %-2d", (int)rom_ptr->zone_subtype);
			posit(11,1); printf("Env: %-2d", (int)rom_ptr->env_type);
			posit(12,1); printf("Special1: %-2d", rom_ptr->special1);
			posit(12,30); printf("Special2: %-2ld", rom_ptr->special2); 
			posit(2,7); getnums(&j, (long)rom_ptr->trap, -127L, 127L);
			rom_ptr->trap = j;
			posit(2,41); getnums(&j, (long)rom_ptr->trapexit, 0L, 32000L);
			rom_ptr->trapexit = j;
			posit(2, 68); getnums(&j, (long)rom_ptr->special, 0L, 32000L);
			rom_ptr->special = j;
			posit(5,1); getstr(str, rom_ptr->track, 80);
			strcpy(rom_ptr->track, str);
			posit(7,12); getnums(&j, (long)rom_ptr->lolevel, 0L, 50L);
			rom_ptr->lolevel = j;
			posit(8,11); getnums(&j, (long)rom_ptr->hilevel, 0L, 50L);
			rom_ptr->hilevel = j;
			posit(9,13); getnums(&j, (long)rom_ptr->death_rom, 0L,32000L);
			rom_ptr->death_rom = j;
			posit(10,7); getnums(&j, (long)rom_ptr->zone_type,0L,50L);
			rom_ptr->zone_type = j;
			posit(10,39); getnums(&j, (long)rom_ptr->zone_subtype,0L,50L);
			rom_ptr->zone_subtype = j;
			posit(11,6); getnums(&j, (long)rom_ptr->env_type,0L,50L);
			rom_ptr->env_type = j;
			posit(12,11); getnums(&j, (long)rom_ptr->special1,0L,32000L);
			rom_ptr->special1 = j;
			posit(12,40); getnums(&j, (long)rom_ptr->special2,0L,32000L);
			rom_ptr->special2 = j;
			break;
		case 5:			/* flags */
			clearscreen();
			posit(2,10); printf("1");
			posit(2,20); printf("2");
			posit(2,30); printf("3");
			posit(2,40); printf("4");
			posit(2,50); printf("5");
			posit(2,60); printf("6");
			posit(3,1); printf("1234567890123456789012345678901234567890123456789012345678901234");
			posit(7,1); printf("Flag #: ");
			posit(8,1); printf("(0 to exit)");
			while(1) {
				for(k=0;k<64;k++)
					str[k] = (rom_ptr->flags[k/8] & 1<<(k%8)) ? '*':'.';
				str[64] = 0;
				posit(4,1); printf("%s",str);
				posit(7,9); getnum(&k, 0, 64);
				if(!k) break;
				if(rom_ptr->flags[(k-1)/8] & 1<<((k-1)%8))
					rom_ptr->flags[(k-1)/8] &= ~(1<<((k-1)%8));
				else
					rom_ptr->flags[(k-1)/8] |= 1<<((k-1)%8);
			}
			break;
		case 6:			/* random monsters */
			clearscreen();
			posit(2,1); printf("Random monsters:");
			for(x=0; x<10; x++) {
				posit(x+3,x==9 ? 2:3); 
				printf("%d:%-5d",x+1,rom_ptr->random[x]);
			}
			posit(14,2); printf("Traffic: %-3d", rom_ptr->traffic);
			for(x=0; x<10; x++) {
				posit(x+3,5);
				getnums(&j, (long)rom_ptr->random[x], 0L, 32000L);
				rom_ptr->random[x] = j;
			}
			posit(14,11); getnums(&j, (long)rom_ptr->traffic, -127L, 127L);
			rom_ptr->traffic = j;
			break;
		case 7:			/* Perm monsters */
			clearscreen();
			posit(2,1); printf("Permanent monsters:");
			posit(4,10); printf("Interval");
			posit(4,20); printf("Lastreset");
			posit(4,30); printf("Monster");
			for(x=0; x<10; x++) {
				posit(x+5, 2); printf("%2d:",x+1);
				posit(x+5, 10); printf("%lu",rom_ptr->perm_mon[x].interval);
				posit(x+5, 20); printf("%lu",rom_ptr->perm_mon[x].ltime);
				posit(x+5, 30); printf("%-5d",rom_ptr->perm_mon[x].misc);
			}
			posit(17,1); printf("Monster #:");
			while(1) {
				posit(17,12); getnum(&x, 0, 10);
				if(!x) break;
				posit(x+4, 10); getnums(&j, (long)rom_ptr->perm_mon[x-1].interval, 0L, 10000000L);
				rom_ptr->perm_mon[x-1].interval = j;
				posit(x+4, 20); getnums(&j, (long)rom_ptr->perm_mon[x-1].ltime, 0L, 10000000L);
				rom_ptr->perm_mon[x-1].ltime = j;
				posit(x+4, 30); getnums(&j, (long)rom_ptr->perm_mon[x-1].misc, 0L, 32000L);
				rom_ptr->perm_mon[x-1].misc = j;
			}
			break;
		case 8:			/* Perm objects */
			clearscreen();
			posit(2,1); printf("Permanent objects:");
			posit(4,10); printf("Interval");
			posit(4,20); printf("Lastreset");
			posit(4,30); printf("Object");
			for(x=0; x<10; x++) {
				posit(x+5, 2); printf("%2d:",x+1);
				posit(x+5, 10); printf("%lu",rom_ptr->perm_obj[x].interval);
				posit(x+5, 20); printf("%lu",rom_ptr->perm_obj[x].ltime);
				posit(x+5, 30); printf("%-5d",rom_ptr->perm_obj[x].misc);
			}
			posit(17,1); printf("Object #:");
			while(1) {
				posit(17,11); getnum(&x, 0, 10);
				if(!x) break;
				posit(x+4, 10); getnums(&j, (long)rom_ptr->perm_obj[x-1].interval, 0L, 10000000L);
				rom_ptr->perm_obj[x-1].interval = j;
				posit(x+4, 20); getnums(&j, (long)rom_ptr->perm_obj[x-1].ltime, 0L, 10000000L);
				rom_ptr->perm_obj[x-1].ltime = j;
				posit(x+4, 30); getnums(&j, (long)rom_ptr->perm_obj[x-1].misc, 0L, 32000L);
				rom_ptr->perm_obj[x-1].misc = j;
			}
			break;
		case 9:			/* Statistics */
			clearscreen();
			posit(2,1); printf("     Visits: %lu", (unsigned long)rom_ptr->beenhere);
			posit(3,1); printf("Established: %lu", (unsigned long)rom_ptr->established);
			posit(2,14); getnums(&j, (long)rom_ptr->beenhere, 0L, 10000000L);
			posit(3,14); getnums(&j, (long)rom_ptr->established, 0L, 10000000L);
			break;
		case 10:		/* Exits */
			clearscreen();
			edit_exits(rom_ptr);
			break;
		case 11:		/* Clear */
			clearscreen();
			posit(1,1);
			printf("Are you sure you want to clear it? ");
			getstr(str, "n", 2);
			if(str[0]=='y' || str[0]=='Y') {
				free_rom(rom_ptr);
				rom_ptr = (room *)malloc(sizeof(room));
				zero(rom_ptr, sizeof(room));
			}
			break;
		case 12:		/* abort */
			free_rom(rom_ptr);
			return;
		case 13:		/* save */
			save_rom_to_file(num, rom_ptr);
			free_rom(rom_ptr);
			return;
		}
	} 
}

void edit_exits(room *rom_ptr)
{
	int 	x, i, k, error;
	long	j;
	xtag 	*xt, *prev;
	char	str[256];

	while(1) {
		clearscreen();
		posit(5,25); printf("Room: (%d) %s",rom_ptr->rom_num, rom_ptr->name);
		posit(6,30); printf("1. Add exit");
		posit(7,30); printf("2. Modify exit");
		posit(8,30); printf("3. Delete exit");
		posit(9,30); printf("4. List exits");
		posit(10,30); printf("5. Back to room menu");
		posit(12,30); printf("Choice: ");

		getnum(&i, 1, 5);

		switch(i) {
		case 1:			/* Add exit */
			xt = rom_ptr->first_ext; x = 1;
			if(xt) {
				while(xt->next_tag) {
					x++;
					xt = xt->next_tag;
				}
				x++;
				xt->next_tag = (xtag *)malloc(sizeof(xtag));
				xt = xt->next_tag;
			}
			else {
				rom_ptr->first_ext = (xtag *)malloc(sizeof(xtag));
				xt = rom_ptr->first_ext;
			}
			xt->ext = (exit_ *)malloc(sizeof(exit_));
			zero(xt->ext, sizeof(exit_));
			xt->next_tag = 0;
			clearscreen();
			posit(2,1); printf("Exit #%d:",x);
			posit(4,10); printf("Name: %s", xt->ext->name);
			posit(5,10); printf("Room: %d", xt->ext->room);
			posit(6,11); printf("Key: %d", xt->ext->key);
			posit(7,1); printf("Time Interval: %lu", xt->ext->ltime.interval);
			posit(8,5); printf("Last Time: %lu", xt->ext->ltime.ltime);
			posit(9,10); printf("Misc: %d", xt->ext->ltime.misc);
			posit(11,10); printf("1");
			posit(11,20); printf("2");
			posit(11,30); printf("3");
			posit(12,1); printf("12345678901234567890123456789012");
			posit(15,1); printf("Flag #: ");
			posit(16,1); printf("(0 to exit)");
			for(j=0;j<32;j++)
				str[j] = (xt->ext->flags[j/8] & 1<<(j%8)) ? '*':'.';
			str[32] = 0;
			posit(13,1); printf("%s",str);
			
			posit(4,16); getstr(str, xt->ext->name, 20);
			strcpy(xt->ext->name, str);
			posit(5,16); getnums(&j, (long)xt->ext->room, 0L, 32000L);
			xt->ext->room = j;
			posit(6,16); getnums(&j, (long)xt->ext->key, -127L, 127L);
			xt->ext->key = j;
			posit(7,16); getnums(&j, (long)xt->ext->ltime.interval, 0L, 10000000L);
			xt->ext->ltime.interval = j;
			posit(8,16); getnums(&j, (long)xt->ext->ltime.ltime, 0L, 10000000L);
			xt->ext->ltime.ltime = j;
			posit(9,16); getnums(&j, (long)xt->ext->ltime.misc, 0L, 32000L);
			xt->ext->ltime.misc = j;
			while(1) {
				posit(15,9); getnum(&k, 0, 32);
				if(!k) break;
				if(xt->ext->flags[(k-1)/8] & 1<<((k-1)%8))
					xt->ext->flags[(k-1)/8] &= ~(1<<((k-1)%8));
				else
					xt->ext->flags[(k-1)/8] |= 1<<((k-1)%8);
				for(j=0;j<32;j++)
					str[j] = (xt->ext->flags[j/8] & 1<<(j%8)) ? '*':'.';
				str[32] = 0;
				posit(13,1); printf("%s",str);
			}
			break;
		case 2:			/* modify exit */
			clearscreen();
			posit(2,1); printf("Exit #"); 
			getnum(&i, 0, 50);
			if(!i) break;
			xt = rom_ptr->first_ext; x = 1; error = 0;
			while(x <= i) {
				if(!xt) {
					error = 1;
					break;
				}
				if(x == i)
					break;
				x++;
				xt = xt->next_tag;
			}
			if(error) break;
			clearscreen();
			posit(2,1); printf("Exit #%d:",x);
			posit(4,10); printf("Name: %s", xt->ext->name);
			posit(5,10); printf("Room: %d", xt->ext->room);
			posit(6,11); printf("Key: %d", xt->ext->key);
			posit(8,1); printf("= Last Time = (old style)");
			posit(9,1); printf("Time Interval: %lu", xt->ext->ltime.interval);
			posit(10,5); printf("Last Time: %lu", xt->ext->ltime.ltime);
			posit(11,10); printf("Misc: %d", xt->ext->ltime.misc);
			posit(13,1); printf("= Random = (new style)");
			posit(14,1); printf("Time Interval: %lu", xt->ext->random.interval);
			posit(15,5); printf("Last Time: %lu", xt->ext->random.ltime);
			posit(16,10); printf("Misc: %d", xt->ext->random.misc);
			posit(17,1); printf("Message1: %s", xt->ext->rand_mesg[0]);
			posit(18,1); printf("Message2: %s", xt->ext->rand_mesg[1]);
			
			posit(20,10); printf("1");
			posit(20,20); printf("2");
			posit(20,30); printf("3");
			posit(21,1); printf("12345678901234567890123456789012");
			posit(24,1); printf("Flag #: ");
			posit(25,1); printf("(0 to exit)");
			for(j=0;j<64;j++)
				str[j] = (xt->ext->flags[j/8] & 1<<(j%8)) ? '*':'.';
			str[64] = 0;
			posit(22,1); printf("%s",str);
			
			posit(4,16); getstr(str, xt->ext->name, 20);
			strcpy(xt->ext->name, str);
			posit(5,16); getnums(&j, (long)xt->ext->room, 0L, 32000L);
			xt->ext->room = j;
			posit(6,16); getnums(&j, (long)xt->ext->key, -127L, 127L);
			xt->ext->key = j;
			posit(9,16); getnums(&j, (long)xt->ext->ltime.interval, 0L, 10000000L);
			xt->ext->ltime.interval = j;
			posit(10,16); getnums(&j, (long)xt->ext->ltime.ltime, 0L, 10000000L);
			xt->ext->ltime.ltime = j;
			posit(11,16); getnums(&j, (long)xt->ext->ltime.misc, 0L, 32000L);
			xt->ext->ltime.misc = j;
			posit(14,16); getnums(&j, (long)xt->ext->random.interval, 0L, 10000000L);
			xt->ext->random.interval = j;
			posit(15,16); getnums(&j, (long)xt->ext->random.ltime, 0L, 10000000L);
			xt->ext->random.ltime = j;
			posit(16,16); getnums(&j, (long)xt->ext->random.misc, 0L, 32000L);
			xt->ext->random.misc = j;
			posit(17,11); getstr(str,xt->ext->rand_mesg[0], 80);
			strcpy(xt->ext->rand_mesg[0], str);
			posit(18,11); getstr(str,xt->ext->rand_mesg[1], 80);
			strcpy(xt->ext->rand_mesg[1], str);
			
			while(1) {
				posit(24,9); getnum(&k, 0, 64);
				if(!k) break;
				if(xt->ext->flags[(k-1)/8] & 1<<((k-1)%8))
					xt->ext->flags[(k-1)/8] &= ~(1<<((k-1)%8));
				else
					xt->ext->flags[(k-1)/8] |= 1<<((k-1)%8);
				for(k=0;k<64;k++)
					str[k] = (xt->ext->flags[k/8] & 1<<(k%8)) ? '*':'.';
				str[64] = 0;
				posit(22,1); printf("%s",str);
			}
			break;
		case 3: 		/* delete */
			clearscreen();
			posit(2,1); printf("Delete exit #");
			getnum(&i, 0, 50);
			if(!i) break;
			xt = rom_ptr->first_ext; x = 1; error = 0;
			while(x <= i) {
				if(!xt) {
					error = 1;
					break;
				}
				if(x == i)
					break;
				x++;
				prev = xt;
				xt = xt->next_tag;
			}
			if(error) break;
			clearscreen();
			if(xt == rom_ptr->first_ext) {
				rom_ptr->first_ext = xt->next_tag;
				free(xt->ext);
				free(xt);
			}
			else {
				prev->next_tag = xt->next_tag;
				free(xt->ext);
				free(xt);
			}
			break;
		case 4:			/* list */
			clearscreen();
			xt = rom_ptr->first_ext; x=0;
			while(xt && x<48) {
			  posit(x%24+1, x/24>0?40:1);
			  printf("%d: (%d) %s", x+1, xt->ext->room, xt->ext->name);
			  xt = xt->next_tag;
			  x++;
			}
			posit(24,40); printf("Hit Enter:");
			getnum(&i,0,0);
			break;
		case 5:
			return;
		}
	}
}

void getnum(int *i, int min, int max)
{
	char str[80];

	savepos();
	do {
		restorepos();
		fgets(str,80,stdin);
		*i = atoi(str);
	} while(*i < min || *i > max);
}

void getnums(long *i, long orig, long min, long max)
{
	char str[80];

	savepos();
	do {
		restorepos();
		fgets(str,80,stdin);
		if(!str[0]) break;
		*i = atol(str);
	} while(*i < min || *i > max);
	if(!str[0])
		*i = orig;
}
	
void getstr(char *str, char *origstr, int maxlen)
{
	char tempstr[256];

	fgets(tempstr,257,stdin);
	if(!tempstr[0])
		strcpy(str, origstr);
	else if(!strcmp(tempstr, "@"))
		str[0] = 0;
	else
		strncpy(str, tempstr, maxlen-1);
	if(str[strlen(str)-1] == '\n')
		str[strlen(str)-1] = '\0';
}

