/*
29/01/2022
made a .csv conversion tool
love smithy


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
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
    clearscreen();
 	
	
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
int save_room(room *rom_ptr, int index){
	char	rfile[256], filebak[256];
	int		fd;


	//if(!Rom[num].rom)
	//	return(0);
	//memset(rfile,0,strlen(rfile));
	//memset(filebak,0,strlen(filebak));

	printf("starting the save function now\n");
	get_room_filename( index, rfile );
	//printf("got the room filename\n");
	sprintf(filebak, "%snew ", rfile);
	//printf("fileback is: %s\n", filebak );
	//printf("file is: %s\n", rfile);
	//link(rfile, filebak);
	//printf("linked the files\n");
	//rename(rfile, filebak);
	//printf("%s\n", rfile );
	fd = open(rfile, O_RDWR | O_CREAT | O_BINARY, ACC);
	printf("opening the file now\n");
	if(fd < 0)
		return(-1);
	//printf("writing the room now\n");
	//elog("writing the room now");
	//printf("here we go!\n");
	if(write_rom(fd, rom_ptr, PERMONLY) < 0) {
		printf("failed to write the room\n");
		close(fd);
		//unlink(rfile);
		//rename(filebak, rfile);
		return(-1);
	}
	close(fd);
	//unlink(filebak);
	//printf("going back to the previous function\n");
	return(0);

}
int save_object(object *obj_ptr, int index){
	object    *po;
        otag      *fo;
        room      *rom;
        creature  *pc;
        char	  file[80];
	
	sprintf(file, "%s/o%02d", get_object_path(), index/OFILESIZE);
	
	/* save some pointers */
	rom = obj_ptr->parent_rom;
	po  = obj_ptr->parent_obj;
	fo  = obj_ptr->first_obj;
	pc  = obj_ptr->parent_crt;

	/* now clear them for the save */
	obj_ptr->parent_rom = 0;
	obj_ptr->parent_obj = 0;
	obj_ptr->first_obj = 0;
	obj_ptr->parent_crt = 0;    


	if ( save_obj_to_file( index, obj_ptr ) != 0 )
	{
		//elog_broad( "Error saving object in dm_save_obj()");
		printf("Error: object was not saved.\n");
		return(-1);
	}
	else
	{
		printf("Object #%d updated.\n", index);
	}


	/* swap this new object if its in the queue */
	replace_obj_in_queue(index, obj_ptr );


	/* and restore the pointers */
	obj_ptr->parent_rom = rom;
	obj_ptr->parent_obj = po;
	obj_ptr->first_obj = fo;
	obj_ptr->parent_crt = pc;    
     
   

   return(0);  
}

int save_creature(creature *crt_ptr, int index){
		
	ttag      *tp,*tempt;
    char	  file[80];
	int	  fd, i=0,x;
	otag	  *op, *otemp;
	ctag	  *fol;
	
	F_CLR(crt_ptr, MPERMT);
	
    tp = crt_ptr->first_tlk;
	while(tp) {
		tempt = tp->next_tag;
		if(tp->key) free(tp->key);
		if(tp->response) free(tp->response);
		if(tp->action) free(tp->action);
		if(tp->target) free(tp->target);
	        free(tp);
		tp = tempt;
	}

    crt_ptr->first_tlk = 0;

	op=crt_ptr->first_obj;
	if(!F_ISSET(crt_ptr, MTRADE)) { 
		while(op) {
			x=find_obj_num(op->obj);
			if(!x) {
			printf("Unique object in inventory not saved.\n");
			op=op->next_tag;
			continue;
			}
			crt_ptr->carry[i]= x;
			printf("Inventory save.\n");
			i++;
			if(i>9){
			printf("Only first 10 objects in inventory saved.\n");
			break;
			}		
		op=op->next_tag;
		}
	 
	}
	/* flush inventory -- dont exactly know why this is necessary... */
	
	op=crt_ptr->first_obj;
	crt_ptr->first_obj=0;
	while(op) {
		otemp=op->next_tag;
		free_obj(op->obj);
		free(op);
		op=otemp;
	} 


	if(crt_ptr->first_enm) {
		printf("clearing enemy list b4 save\n");
		crt_ptr->first_enm = NULL;
	}

	
	/* clean up possesed before save */
	if(F_ISSET (crt_ptr, MDMFOL)) { /* clear relevant follow lists */
	    F_CLR(crt_ptr->following, PALIAS);
		F_CLR(crt_ptr, MDMFOL);
		/*Ply[crt_ptr->following->fd].extr->alias_crt = 0;*/
		
		fol = crt_ptr->following->first_fol;
		if(fol->crt == crt_ptr) {
				crt_ptr->following->first_fol = fol->next_tag;
				free(fol);
		}
	}

	/* Update index */
	sprintf(crt_ptr->password, "%d", index);

	sprintf(file, "%s/m%02d", get_monster_path(), index/MFILESIZE);
	
    
	if ( save_crt_to_file(index, crt_ptr) != 0)
	{
		
		
		printf("Error: creature was not saved\n" );
		return(-1);
	}
	else
	{
		printf("Creature #%d updated.\n", index);
		
	}


	/* swap this new creature if its in the queue */
	replace_crt_in_queue(index, crt_ptr );

	return(0);

}



void convert_from_csv(){
	clearscreen();
	posit(3,20); printf("Converting from csv");
	
	int i, j, m, n, o, p, q, r, result, c=0, d=0, temp, index;
	char array[182][380];
	char file[80] = { };
	char str[2000] = { };
	char test[20] = { };
	char tempstr[80];
	
	FILE *fpt;

	//MONSTERS FIRST
	posit(4,20); printf("converting monsters");
	fpt = fopen("monsters_datatbase.csv", "r");
	
	for (i=0; i<120; i++){
		memset(array[i],0,strlen(str));

	}
	object *opt;

	creature *crt_ptr;
	crt_ptr = (creature *)malloc(sizeof(creature));
	if(!crt_ptr) {
		printf("Cannot allocate memory for creature.\n");
		merror("dm_add_crt", NONFATAL);
		return;
	}
	zero(crt_ptr, sizeof(creature));
	crt_ptr->first_obj = 0;
	crt_ptr->first_fol = 0;
	crt_ptr->first_enm = 0;
	crt_ptr->first_tlk = 0;
	crt_ptr->parent_rom = 0;
	crt_ptr->following = 0;
	for(n=0; n<20; n++)
		crt_ptr->ready[n] = 0;
	do{
		memset(str,0,strlen(str));
		result = fscanf(fpt, "%1999[^;\n]", str);
		//printf("result of fscan is %i\n", result);
		
		
		if (result == 0){
			if (!str){
				printf("found an empty space\n");
				/*memset(str,0,strlen(str));*/
			}
		}
		result = fscanf(fpt, "%*c");
		/*else{*/
			if (d > 0){
			/*c will count the number of columns in the 
			csv file
			currently there are 119 */
			
			/*posit(16,20)*///;printf("step %i: [%s]\n", c, str );
			if (c == 0){
				index = atoi(str);
				/*posit(15,20)*/; printf("converting monster %03d\n", index);
				strcpy(crt_ptr->password, str);

			}
			
			//printf("up to step %i\n", c );
			

			strcpy(array[c], str);
			//printf("array[%i]: [%s]\n\n",c ,array[c]);
			

			/*if we're reading the first column, try to see if there's
			a creature with that index number and open it up*/
			/*if (c == 1){
				index = atoi(str);
				posit(15,20); printf("converting monster %03d\n", index);
				strcpy(crt_ptr->password, str);

			}*/
			/*only populate the other values if we actually
			opened a creature*/
			
			/*once we are at the end of the row, close and 
			save the creature
			then move the counter back to the start for the new row*/
			if (c == 118){
				/*posit(7,20)*/; printf("attempting to save...\n");
				for (i=0; i<119; i++){
					if (!array[i]){
						strcpy(array[i], "0");
					}
					if (array[i] == "(null)"){
						strcpy(array[i], "0");
					}
				}
				strcpy(crt_ptr->name, array[1]);
				
				strcpy(crt_ptr->description, array[2]);
				
				strcpy(crt_ptr->talk, array[3]);
				
				strcpy(crt_ptr->key[0], array[5]);
				
				strcpy(crt_ptr->key[1], array[6]);
				
				strcpy(crt_ptr->key[2], array[7]);
				
				crt_ptr->fd = (long)atoi(array[8]);
				crt_ptr->level = (long)atoi(array[9]);
				
				crt_ptr->type = (long)atoi(array[10]);
				
				crt_ptr->class = (long)atoi(array[11]);
				
				crt_ptr->race = (long)atoi(array[12]);
				
				crt_ptr->numwander = (long)atoi(array[13]);
				
				crt_ptr->alignment = (long)atoi(array[14]);
				
				crt_ptr->strength = (long)atoi(array[15]);
				
				crt_ptr->dexterity = (long)atoi(array[16]);
				
				crt_ptr->constitution = (long)atoi(array[17]);
				
				crt_ptr->intelligence = (long)atoi(array[18]);
				
				crt_ptr->piety = (long)atoi(array[19]);
				
				crt_ptr->hpmax = (long)atoi(array[20]);
				
				crt_ptr->hpcur = (long)atoi(array[21]);
				
				crt_ptr->mpmax = (long)atoi(array[22]);
				
				crt_ptr->mpcur = (long)atoi(array[23]);
				
				crt_ptr->armor = (long)atoi(array[24]);
				
				crt_ptr->thaco = (long)atoi(array[25]);
				
				crt_ptr->experience = (long)atoi(array[26]);
				
				crt_ptr->gold = (long)atoi(array[27]);
				
				crt_ptr->ndice = (long)atoi(array[28]);
				
				crt_ptr->sdice = (long)atoi(array[29]);
				
				crt_ptr->pdice = (long)atoi(array[30]);
				
				crt_ptr->special = (long)atoi(array[31]);
				
				for (i=0; i<6; i++){
					crt_ptr->proficiency[i] = (long)atoi(array[32+i]);
					printf("%i",i);
				}
				
				for (i=0; i<256; i++){
					F_CLR(crt_ptr, i);
					S_CLR(crt_ptr, i);
					Q_CLR(crt_ptr, i);
				}

				//clear the flags because dirty memory
				for (i=0; i<128; i++){
					
					F_CLR(crt_ptr, i);
					
				}

				for (i=0; i<16; i++){
					temp = atoi(array[62+i]);
					if (temp > 0){
						F_SET(crt_ptr, temp-1);
					
						printf("setting flag %i\n", temp-1);
					}
					
				}
				F_CLR(crt_ptr, 0);
				
				//clear the spells because dirty memory
				for (i=0; i<128; i++){
					
					S_CLR(crt_ptr, i);
					
				}
				for (i=0; i<16; i++){
					temp = atoi(array[46+i]);
					if (temp > 0){
						S_SET(crt_ptr, temp-1);
					}
					
				}
				
				
				for (i=0; i<8; i++){
					
					crt_ptr->realm[i] = (int)atoi(array[38+i]);
					
				}
				
				//clear the quests because dirty memory
				for (i=0; i<128; i++){
					
					Q_CLR(crt_ptr, i);
					
				}
				for (i=0; i<16; i++){
					temp = atoi(array[78+i]);
					Q_SET(crt_ptr, temp);
					
				}
				crt_ptr->questnum = (long)atoi(array[94]);
				
				for (i=0; i<10; i++){
					crt_ptr->carry[i] = (long)atoi(array[95+i]);
					
				}
				
				/*crt_ptr->rom_num = array[105];
				crt_ptr->bank_balance = array[106];
				strcpy(crt_ptr->title, array[107]);
				for (i=0; i<5; i++){
					crt_ptr->misc_stats[0] = array[108+i];
					printf(".");
				}
				crt_ptr->clanindex = array[113];printf(".");
				crt_ptr->clanexp = array[114];printf(".");
				crt_ptr->guildtype = array[115];printf("n");
				crt_ptr->guildexp = array[116];printf(".");*/
				crt_ptr->special1 = (long)atoi(array[117]);
				crt_ptr->special2 = (long)atoi(array[118]);
				/*posit(5,20);*/ printf("end of values\n");
				/*save the creature*/
				/*sprintf(crt_ptr->password, "%d", temp);*/
				
				
				sprintf(file, "%s/m%02d", get_monster_path(), index/MFILESIZE);
				
				if (index < 5001){
					clearscreen();
					posit(2,1); printf("Creature name:");
					posit(3,1); printf("%s",crt_ptr->name);
					posit(5,1); printf("Description:");
					posit(6,1); printf("%s",crt_ptr->description);
					posit(8,1); printf("%s", "Talk:");
					posit(9,1); printf("%s",crt_ptr->talk);
					posit(10,1); printf("Experience: %ld", crt_ptr->experience);
					printf("[%s]", array[26]);
					posit(10,30); printf("  Gold: %ld", crt_ptr->gold);
					printf("[%s]", array[27]);
					posit(11,1); printf("    # Dice: %-5d", crt_ptr->ndice);
					printf("[%s]", array[28]);
					posit(12,30); printf(" Sides: %-5d", crt_ptr->sdice);
					printf("[%s]", array[29]);
					posit(12,60); printf("  Plus: %-5d", crt_ptr->pdice);
					printf("[%s], %i", array[30], atoi(array[30]));
					posit(13,1); printf("     Quest: %-3d", crt_ptr->questnum);
					posit(14,29); printf("Special: %-3d", crt_ptr->special);
					posit(15,1); printf("   HP: %-5d/%-5d", crt_ptr->hpcur, crt_ptr->hpmax);
					posit(16,30); printf("   MP: %-5d/%-5d", crt_ptr->mpcur, crt_ptr->mpmax);
					posit(17,1); printf("Armor: %-3d", crt_ptr->armor);
					posit(18,30); printf("Thac0: %-3d", crt_ptr->thaco);
					posit(19,1); printf("  Num: %-3d", crt_ptr->numwander);
					posit(20,1); printf("Level: %-3d", crt_ptr->level);
					posit(20,20); printf("Class: %-3d", crt_ptr->class);
					posit(20,40); printf("Align: %-5d", crt_ptr->alignment);
					posit(21,1); printf("  Str: %-2d", crt_ptr->strength);
					posit(21,20); printf("  Dex: %-2d", crt_ptr->dexterity);
					posit(21,40); printf("  Con: %-2d", crt_ptr->constitution);
					posit(22,1); printf("  Int: %-2d", crt_ptr->intelligence);
					posit(22,20); printf("  Pty: %-2d", crt_ptr->piety);
					
					//check flags
					posit(23,10); printf("1");
					posit(23,20); printf("2");
					posit(23,30); printf("3");
					posit(23,40); printf("4");
					posit(23,50); printf("5");
					posit(23,60); printf("6");
					posit(24,1); printf("1234567890123456789012345678901234567890123456789012345678901234");
					
					int k;

					for(k=0;k<64;k++){
						str[k] = (crt_ptr->flags[k/8] & 1<<(k%8)) ? '*':'.';
						str[64] = 0;
						posit(25,1); printf("%s",str);
					}		
					

					printf("Hit Enter:\n");
					getnum(&i,0,0);
				
				if (save_creature(crt_ptr, index) < 0){
					posit(5,20); printf("error with monster %i", index);
				}	
				}
				c = -1;
				d++;

			}
			c++;
			}
			else {
				c++;
				if (c == 118){
					c = -1;
					d++;
				}
			}
		/*}/*end of the else*/
	}while(result != EOF);
	fclose(fpt);
	posit(24,40); printf("Hit Enter:");
	getnum(&i,0,0);
	
//OBJECTS-------------------------------------
	posit(4,20); printf("converting objects");
	fpt = fopen("objects_datatbase.csv", "r");
	
	for (i=0; i<120; i++){
		memset(array[i],0,strlen(str));

	}

	object *obj_ptr;
	obj_ptr = (object *)malloc(sizeof(object));

	if(!obj_ptr) {
		printf("Cannot allocate memory for object.\n");
		merror("dm_add_obj", NONFATAL);
		return;
	}
	zero(obj_ptr, sizeof(object));
	obj_ptr->first_obj = 0;
    obj_ptr->parent_obj = 0;
    obj_ptr->parent_rom = 0;
    obj_ptr->parent_crt = 0;
	
    c =0;

	do{
		memset(str,0,strlen(str));
		
		result = fscanf(fpt, "%1999[^;\n]", str);
		//printf("result of fscan is %i\n", result);
		if (result == 0){
			if (!str){
				printf("found an empty space\n");
				/*memset(str,0,strlen(str));*/
			}
		}
		result = fscanf(fpt, "%*c");

		/*else{*/
			if (d > 0){
			/*c will count the number of columns in the 
			csv file
			currently there are 60 */
			
			/*posit(16,20)*///;printf("step %i: [%s]\n", c, str );
			if (c == 0){
				index = atoi(str);
				/*posit(15,20)*///; printf("converting object %03d\n", index);
				//strcpy(crt_ptr->password, str);

			}
			
			/*posit(2,20);printf("up to step %i\n", c );*/
			
			strcpy(array[c], str);
			//printf("array[%i]: [%s]\n\n",c ,array[c]);

			/*if we're reading the first column, try to see if there's
			a creature with that index number and open it up*/
			/*if (c == 1){
				index = atoi(str);
				posit(15,20); printf("converting monster %03d\n", index);
				strcpy(crt_ptr->password, str);

			}*/
			/*only populate the other values if we actually
			opened a creature*/
			
			/*once we are at the end of the row, close and 
			save the creature
			then move the counter back to the start for the new row*/
			if (c == 60){
				/*posit(7,20)*/; printf("attempting to save...\n");
				for (i=0; i<61; i++){
					if (!array[i]){
						strcpy(array[i], "0");
					}
					if (array[i] == "(null)"){
						strcpy(array[i], "0");
					}
				}
				strcpy(obj_ptr->name, array[1]);
				
				strcpy(obj_ptr->description, array[2]);
				
				strcpy(obj_ptr->key[0], array[3]);
				
				strcpy(obj_ptr->key[1], array[4]);
				
				strcpy(obj_ptr->key[2], array[5]);

				strcpy(obj_ptr->use_output, array[6]);
				
				obj_ptr->value = (long)atoi(array[7]);
				obj_ptr->weight = (long)atoi(array[8]);
				
				obj_ptr->type = (long)atoi(array[9]);
				
				obj_ptr->adjustment = (long)atoi(array[10]);
				
				obj_ptr->shotsmax = (long)atoi(array[11]);
				
				obj_ptr->shotscur = (long)atoi(array[12]);
				
				obj_ptr->ndice = (long)atoi(array[13]);
				
				obj_ptr->sdice = (long)atoi(array[14]);
				
				obj_ptr->pdice = (long)atoi(array[15]);

				obj_ptr->armor = (long)atoi(array[16]);

				obj_ptr->wearflag = (long)atoi(array[17]);
				
				obj_ptr->magicpower= (long)atoi(array[18]);
				
				obj_ptr->magicrealm = (long)atoi(array[19]);
				
				obj_ptr->special = (long)atoi(array[20]);

				for (i=0; i<256; i++){
					F_CLR(obj_ptr, i);
				}
				for (i=0; i<16; i++){
					temp = atoi(array[21+i]);
					if (temp > 0){
						F_SET(obj_ptr, temp-1);
					}	
				}
				obj_ptr->questnum = (long)atoi(array[37]);

				obj_ptr->strength = (long)atoi(array[38]);
				
				obj_ptr->dexterity = (long)atoi(array[39]);
				
				obj_ptr->constitution = (long)atoi(array[40]);
				
				obj_ptr->intelligence = (long)atoi(array[41]);
				
				obj_ptr->piety = (long)atoi(array[42]);
				
				for (i=0; i<16; i++){
					
					obj_ptr->sets_flag[i] = (int)atoi(array[43+i]);
				}
				
				obj_ptr->special1 = (long)atoi(array[59]);
				obj_ptr->special2 = (long)atoi(array[60]);
				
				/*posit(5,20);*/ printf("end of values\n");
				/*save the creature*/
				/*sprintf(obj_ptr->password, "%d", temp);*/
				
				
				sprintf(file, "%s/o%02d", get_object_path(), index/MFILESIZE);
				
				if (index < 5001){
					clearscreen();
					posit(2,1); printf("Objectname:");
					posit(3,1); printf("%s",obj_ptr->name);
					posit(5,1); printf("Description:");
					posit(6,1); printf("%s",obj_ptr->description);
					
					posit(11,1); printf("    # Dice: %-5d", obj_ptr->ndice);
					printf("[%s]", array[28]);
					posit(12,30); printf(" Sides: %-5d", obj_ptr->sdice);
					printf("[%s]", array[29]);
					posit(12,60); printf("  Plus: %-5d", obj_ptr->pdice);
					printf("[%s], %i", array[30], atoi(array[30]));
					posit(13,1); printf("     Quest: %-3d", obj_ptr->questnum);
					posit(14,29); printf("Special: %-3d", obj_ptr->special);
					posit(17,1); printf("Armor: %-3d", obj_ptr->armor);
					posit(21,1); printf("  Str: %-2d", obj_ptr->strength);
					posit(21,20); printf("  Dex: %-2d", obj_ptr->dexterity);
					posit(21,40); printf("  Con: %-2d", obj_ptr->constitution);
					posit(22,1); printf("  Int: %-2d", obj_ptr->intelligence);
					posit(22,20); printf("  Pty: %-2d", obj_ptr->piety);
					
					//posit(25,40); printf("Hit Enter:");
					//getnum(&i,0,0);
				
				if (save_object(obj_ptr, index) < 0){
					posit(5,20); printf("error with object %i", index);
				}	
				}
				c = -1;
				d++;

			}
			c++;
			}
			else {
				c++;
				if (c == 60){
					c = 0;
					d++;
				}
			}
		/*}/*end of the else*/
	}while(result != EOF);
	fclose(fpt);
	posit(24,40); printf("Hit Enter:");
	getnum(&i,0,0);
	
//ROOMS-------------------------------------
	posit(4,20); printf("converting rooms\n");
	fpt = fopen("rooms_datatbase.csv", "r");
	
	for (i=0; i<129; i++){
		memset(array[i],0,strlen(str));

	}
	
	room *rom_ptr, *new_rom;
	printf("mallon rom_ptr\n");
	rom_ptr = (room *)malloc(sizeof(room));
	lasttime *perm;
	printf("malloc perm\n");
	perm = (struct lasttime *)malloc(sizeof(lasttime));
	exit_ *exit0;
	printf("malloc exit0\n");
	exit0 = (exit_ *)malloc(sizeof(exit_));
	exit_ *exit1;
	printf("malloc exit1\n");
	exit1 = (exit_ *)malloc(sizeof(exit_));
	exit_ *exit2;
	printf("malloc exit2\n");
	exit2 = (exit_ *)malloc(sizeof(exit_));
	exit_ *exit3;
	printf("malloc exit3\n");
	exit3 = (exit_ *)malloc(sizeof(exit_));
	exit_ *exit4;
	printf("malloc exit4\n");
	exit4 = (exit_ *)malloc(sizeof(exit_));
	exit_ *exit5;
	printf("malloc exit5\n");
	exit5 = (exit_ *)malloc(sizeof(exit_));
	exit_ *exit6;
	printf("malloc exit6\n");
	exit6 = (exit_ *)malloc(sizeof(exit_));
	exit_ *exit7;
	printf("malloc exit7\n");
	exit7 = (exit_ *)malloc(sizeof(exit_));

	//exit_ *exit7;
	//exit7 = (exit_ *)malloc(sizeof(exit_));

	exit_ *exit8;
	exit8 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit9;
	exit9 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit10;
	exit10 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit11;
	exit11 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit12;
	exit12 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit13;
	exit13 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit14;
	exit14 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit15;
	exit15 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit16;
	exit16 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit17;
	exit17 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit18;
	exit18 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit19;
	exit19 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit20;
	exit20 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit21;
	exit21 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit22;
	exit22 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit23;
	exit23 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit24;
	exit24 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit25;
	exit25 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit26;
	exit26 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit27;
	exit27 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit28;
	exit28 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit29;
	exit29 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit30;
	exit30 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit31;
	exit31 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit32;
	exit32 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit33;
	exit33 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit34;
	exit34 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit35;
	exit35 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit36;
	exit36 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit37;
	exit37 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit38;
	exit38 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit39;
	exit39 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit40;
	exit40 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit41;
	exit41 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit42;
	exit42 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit43;
	exit43 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit44;
	exit44 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit45;
	exit45 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit46;
	exit46 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit47;
	exit47 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit48;
	exit48 = (exit_ *)malloc(sizeof(exit_));
	
	exit_ *exit49;
	exit49 = (exit_ *)malloc(sizeof(exit_));
	
	struct ext_tag *xtagA;
	printf("malloc xtagA\n");
	xtagA = (struct ext_tag*)malloc(2000);
	struct ext_tag *xtagB;
	printf("malloc xtagB\n");
	xtagB = (struct ext_tag*)malloc(2000);
	struct ext_tag *xtagC;
	printf("malloc xtagC\n");
	xtagC = (struct ext_tag*)malloc(2000);
	struct ext_tag *xtagD;
	printf("malloc xtagD\n");
	xtagD = (struct ext_tag*)malloc(2000);
	struct ext_tag *xtagE;
	printf("malloc xtagE\n");
	xtagE = (struct ext_tag*)malloc(2000);
	struct ext_tag *xtagF;
	printf("malloc xtagF\n");
	xtagF = (struct ext_tag*)malloc(2000);
	struct ext_tag *xtagG;
	printf("malloc xtagG\n");
	xtagG = (struct ext_tag*)malloc(2000);
	struct ext_tag *xtagH;
	printf("malloc xtagH\n");
	xtagH = (struct ext_tag*)malloc(2000);
	
	struct ext_tag *xtagI;
	xtagI = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagJ;
	xtagJ = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagK;
	xtagK = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagL;
	xtagL = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagM;
	xtagM = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagN;
	xtagN = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagO;
	xtagO = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagP;
	xtagP = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagQ;
	xtagQ = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagR;
	xtagR = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagS;
	xtagS = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagT;
	xtagT = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagU;
	xtagU = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagV;
	xtagV = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagW;
	xtagW = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagX;
	xtagX = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagY;
	xtagY = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagZ;
	xtagZ = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAA;
	xtagAA = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAB;
	xtagAB = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAC;
	xtagAC = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAD;
	xtagAD = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAE;
	xtagAE = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAF;
	xtagAF = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAG;
	xtagAG = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAH;
	xtagAH = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAI;
	xtagAI = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAJ;
	xtagAJ = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAK;
	xtagAK = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAL;
	xtagAL = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAM;
	xtagAM = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAN;
	xtagAN = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAO;
	xtagAO = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAP;
	xtagAP = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAQ;
	xtagAQ = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAR;
	xtagAR = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAS;
	xtagAS = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAT;
	xtagAT = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAU;
	xtagAU = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAV;
	xtagAV = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAW;
	xtagAW = (struct ext_tag*)malloc(2000);
		
	struct ext_tag *xtagAX;
	xtagAX = (struct ext_tag*)malloc(2000);
			
	char *buf, *desc;
	printf("malloc str1\n");
	char *str1; str1 = (char *)malloc(2000);
	printf("malloc str2\n");
	char *str2; str2 = (char *)malloc(2000);
	int len, exists;
	printf("mallocs done\n");

	char exitarray[131][80];

	if(!rom_ptr) {
		printf("Cannot allocate memory for room.\n");
		merror("dm_add_rom", NONFATAL);
		return;
	}
	zero(rom_ptr, sizeof(room));
		
    c =0;
    d =0;
    memset(str1,0,strlen(str1));
    memset(str2,0,strlen(str2));

	do{
		memset(str,0,strlen(str));
		
		
		result = fscanf(fpt, "%1999[^;\n]", str);
		//printf("result of fscan is %i\n", result);
		if (result == 0){
			if (!str){
				printf("found an empty space\n");
				/*memset(str,0,strlen(str));*/
			}
		}
		result = fscanf(fpt, "%*c");
		//printf("step %i: [%s]\n", c, str );

		/*else{*/
		if (d > 0){
			/*c will count the number of columns in the 
			csv file
			currently there are 182 (+ index) */
				
			/*posit(16,20)*/;
			if (c == 0){
				index = atoi(str);
				/*posit(15,20)*/; printf("converting room %03d\n", index);
				//strcpy(crt_ptr->password, str);

			}
			//printf("up to step %i\n", c );
			strcpy(array[c], str);
			//printf("array[%i]: [%s]\n\n",c ,array[c]);
			if (c==2){
				strcpy(str1, str);
				printf("[%s]\n",str1);
			}
			if (c==3){
				strcpy(str2, str);
				printf("[%s]\n",str2);
			}
			/*if we're reading the first column, try to see if there's
			a creature with that index number and open it up*/
			/*if (c == 1){
				index = atoi(str);
				posit(15,20); printf("converting monster %03d\n", index);
				strcpy(crt_ptr->password, str);

			}*/
			/*only populate the other values if we actually
			opened a creature*/
			
			/*once we are at the end of the row, close and 
			save the creature
			then move the counter back to the start for the new row*/
			if (c == 128){
			/*posit(7,20)*/; printf("attempting to save...\n");
			for (i=0; i<183; i++){
				if (!array[i]){
					strcpy(array[i], "0");
				}
				if (array[i] == "(null)"){
					strcpy(array[i], "0");
				}
			}

			get_room_filename(index, file);
			printf("getting room filename\n");
			if (!file)
				printf("no file \n");
			printf("%s\n", file );
			if (file)
				printf("file path exists\n");
			if (!file_exists(file)){
				printf("%s\n",array[0]);
				new_rom = (room *)malloc(sizeof(room));
				if(!new_rom)
					merror("dm_add_room", FATAL);

				zero(new_rom, sizeof(room));
				printf("making new room\n");
				new_rom->rom_num = (short) index;
				sprintf(new_rom->name, "Room #%d", index);

				F_SET(new_rom, RNOTEL); /* set room to no tport MJK */
				new_rom->death_rom = 0;

				if ( save_rom_to_file(index, new_rom) != 0 ) {
					printf("Error: Unable to save room to file.\n");
						
				}
	
				free(new_rom);
			}
			//printf("hi\n");
			rom_ptr->rom_num = (long)atoi(array[0]);
			//printf("rom num: %s\n", array[0]); 
			strcpy(rom_ptr->name, array[1]);
			//printf("rom name: %s\n", array[1]); 
			
			len = strlen(str1)+1;
			buf = (char *)malloc(len);
			memcpy(buf, str1, len);
			rom_ptr->short_desc = buf;
			//strcpy(rom_ptr->short_desc, str1);
			//printf("short desc: %s\n", str1);
			//printf("short desc: %s\n", buf);
			//printf("short desc: %s\n", rom_ptr->short_desc);
			int len2;
			len2 = strlen(str2)+1;
			buf = (char *)malloc(len2);
			memcpy(buf, str2, len2);
			rom_ptr->long_desc = buf;
			//strcpy(rom_ptr->long_desc, str2);
			//printf("long desc: %s\n", str2);
			//printf("long desc: %s\n", buf);
			//printf("long desc: %s\n", rom_ptr->long_desc);

			//free(len2);

			rom_ptr->lolevel = (long)atoi(array[4]);
			//printf("lolevel: %s\n", array[4]); 
			rom_ptr->hilevel = (long)atoi(array[5]);
			//printf("hilevel: %s\n", array[5]); 
			rom_ptr->special = (long)atoi(array[6]);
			//printf("special: %s\n", array[6]); 
			rom_ptr->trap = (long)atoi(array[7]);
			//printf("trap: %s\n", array[7]); 
			rom_ptr->trapexit = (long)atoi(array[8]);
			//printf("trapexit: %s\n", array[8]); 
			strcpy(rom_ptr->track, array[9]);
			//printf("track: %s\n", array[9]);
			
			//loop through all the flags first and clear them
			// you don't know where the memory has been its dirty!
			for (i=0; i<128; i++){
				F_CLR(rom_ptr, i);
			}
			for (i=0; i<16; i++){
				if ((long)atoi(array[10+i]) != 0){
					F_SET(rom_ptr, (long)atoi(array[10+i])-1);
					//printf("flag: %s\n", array[10+i]);
				}
			}

			for (i=0; i<10; i++){
				rom_ptr->random[i] = (long)atoi(array[26+i]);
				//printf("random: %s\n", array[25+i]);
			}
 
			int tmep = atoi(array[37]);

			rom_ptr->traffic = tmep;
			printf("traffic: %s\n", array[37]);
			printf("tmep: %i\n", tmep);
			printf("tmep: %c\n", tmep);
			printf("tmep: %-3d\n", tmep);
			printf("traffic: %i\n", rom_ptr->traffic);
			printf("traffic: %c\n", rom_ptr->traffic);
			printf("traffic: %-3d\n", rom_ptr->traffic);
			for (i=0; i<10; i++){
				zero(perm, sizeof(lasttime));

				if (array[37+2*i] >0){
					rom_ptr->perm_mon[i].misc = (long)atoi(array[37+2*i]);
					rom_ptr->perm_mon[i].ltime = (time_t)(long)atoi(array[38+2*i]);
					rom_ptr->perm_mon[i].interval = (time_t)(long)atoi(array[38+2*i]);
					//printf("perm: %s\n", array[37+2*i]);
					//printf("permtime: %s\n", array[38+2*i]);
				}

				if (array[57+2*i] >0){
					rom_ptr->perm_obj[i].misc = (long)atoi(array[57+2*i]);
					rom_ptr->perm_obj[i].ltime = (time_t)(long)atoi(array[58+2*i]);
					rom_ptr->perm_obj[i].interval = (time_t)(long)atoi(array[58+2*i]);
				}
					
					
			}

			rom_ptr->beenhere = (long)atoi(array[77]); 
			//printf("beenhere: %s\n", array[77]);
			//printf("clearing xtagA\n");
			//if (xtagA) zero(xtagA, sizeof(xtagA));
			//printf("clearing xtagB\n");
			//if (xtagB) zero(xtagB, sizeof(xtagB));
			//printf("begining exit stuff\n");
			zero(exit0, sizeof(struct exit_));
			zero(exit1, sizeof(struct exit_));
			zero(exit2, sizeof(struct exit_));
			zero(exit3, sizeof(struct exit_));
			zero(exit4, sizeof(struct exit_));
			zero(exit5, sizeof(struct exit_));
			zero(exit6, sizeof(struct exit_));
			zero(exit7, sizeof(struct exit_));
			zero(exit8, sizeof(struct exit_));
			zero(exit9, sizeof(struct exit_));
			zero(exit10, sizeof(struct exit_));
			zero(exit11, sizeof(struct exit_));
			zero(exit12, sizeof(struct exit_));
			zero(exit13, sizeof(struct exit_));
			zero(exit14, sizeof(struct exit_));
			zero(exit15, sizeof(struct exit_));
			zero(exit16, sizeof(struct exit_));
			zero(exit17, sizeof(struct exit_));
			zero(exit18, sizeof(struct exit_));
			zero(exit19, sizeof(struct exit_));
			zero(exit20, sizeof(struct exit_));
			zero(exit21, sizeof(struct exit_));
			zero(exit22, sizeof(struct exit_));
			zero(exit23, sizeof(struct exit_));
			zero(exit24, sizeof(struct exit_));
			zero(exit25, sizeof(struct exit_));
			zero(exit26, sizeof(struct exit_));
			zero(exit27, sizeof(struct exit_));
			zero(exit28, sizeof(struct exit_));
			zero(exit29, sizeof(struct exit_));
			zero(exit30, sizeof(struct exit_));
			zero(exit31, sizeof(struct exit_));
			zero(exit32, sizeof(struct exit_));
			zero(exit33, sizeof(struct exit_));
			zero(exit34, sizeof(struct exit_));
			zero(exit35, sizeof(struct exit_));
			zero(exit36, sizeof(struct exit_));
			zero(exit37, sizeof(struct exit_));
			zero(exit38, sizeof(struct exit_));
			zero(exit39, sizeof(struct exit_));
			zero(exit40, sizeof(struct exit_));
			zero(exit41, sizeof(struct exit_));
			zero(exit42, sizeof(struct exit_));
			zero(exit43, sizeof(struct exit_));
			zero(exit44, sizeof(struct exit_));
			zero(exit45, sizeof(struct exit_));
			zero(exit46, sizeof(struct exit_));
			zero(exit47, sizeof(struct exit_));
			zero(exit48, sizeof(struct exit_));
			zero(exit49, sizeof(struct exit_));
			for (i=49; i>-1; i--){
				//printf("length of exit name is %i\n", strlen(array[78+13*i]));
				//printf("%s\n", array[78+13*i]);
				//if (exit) zero(exit, sizeof(struct exit_));
				//exit = 0;
				//free(exit);
				//if (!exit) printf("cleared the exit variable\n");
				//printf("%s\n", exit->name);
				exists = 0;
				if (array[78+i] != 0 && strlen(array[78+i]) >= 2) {
					
					exists = 1;
					//printf("%s\n", array[78+13*i]);
					//printf("doing the name now\n");
					
					//exit_ *exit;
					//exit = (exit_ *)malloc(sizeof(exit_));
					//zero(exit, sizeof(struct exit_));
					
					// LOOP through the entry of the big room array
					// that corresponds to the exit you are currently
					// considering and load all of the indivdual values
					// into the "exit array" for futher consideration
					//printf("exit from array: %s\n", array[78+i]);
					char *token;
					char *rest;
					//get the first token
					
					int z = 0;
					//char delim[2] = '0';
					//delim[1] = ',';
					token = strtok_r(array[78+i], ",", &rest);
						//get some more tokens
					while (token != NULL && z<127){
						strcpy(exitarray[z], token);
						//printf("exitarray %i: %s [%s]\n", z, exitarray[z], token);
						z++;
						token = strtok_r(rest, ",", &rest);
					}

					/*int z;
					for (z=0; z<131; z++){
						memset(str,0,strlen(str));
	
						result = scanf(array[78+i], " %[^,\n],", str);
						printf("result of fscan is %i\n", result);
						if (result == 0){
							if (!str){
								printf("found an empty space\n");
								//memset(str,0,strlen(str));
							}
						}
						result = scanf(array[78+i], "%*c");
						strcpy(exitarray[z], str);
						printf("exitarray %i: %s [%s]\n", z, exitarray[z], str);
					}*/
						//CONSIDER THE exit number you are up to by checking the
					//value of i. Then, read your exit array values
					// and load them into the appropriate exit
					if (i == 8) {
						strcpy(exit8->name, exitarray[0]);
						exit8->room = (long)atoi(exitarray[1]);
						exit8->key = (long)atoi(exitarray[2]);
						for (j=0; j<128; j++){
							if (exitarray[j+2] != 0){
								//printf("need to set an exit flag from array[%i]: %s \n", j+81+13*i, array[j+81+13*i]+1);
								F_SET(exit8, (long)atoi(exitarray[j+2])-1);
							}
						}
					}
					if (i == 7) {
						strcpy(exit7->name, exitarray[0]);
						exit7->room = (long)atoi(exitarray[1]);
						exit7->key = (long)atoi(exitarray[2]);
						for (j=0; j<128; j++){
							if (exitarray[j+2] != 0){
								//printf("need to set an exit flag from array[%i]: %s \n", j+81+13*i, array[j+81+13*i]+1);
								F_SET(exit7, (long)atoi(exitarray[j+2])-1);
							}
						}
					}
					if (i == 6) {
						strcpy(exit6->name, exitarray[0]);
						exit6->room = (long)atoi(exitarray[1]);
						exit6->key = (long)atoi(exitarray[2]);
						for (j=0; j<128; j++){
							if (exitarray[j+2] != 0){
								//printf("need to set an exit flag from array[%i]: %s \n", j+81+13*i, array[j+81+13*i]+1);
								F_SET(exit6, (long)atoi(exitarray[j+2])-1);
							}
						}
					}
					if (i == 5) {
						strcpy(exit5->name, exitarray[0]);
						exit5->room = (long)atoi(exitarray[1]);
						exit5->key = (long)atoi(exitarray[2]);
						for (j=0; j<128; j++){
							if (exitarray[j+2] != 0){
								//printf("need to set an exit flag from array[%i]: %s \n", j+81+13*i, array[j+81+13*i]+1);
								F_SET(exit5, (long)atoi(exitarray[j+2])-1);
							}
						}
					}
					if (i == 4) {
						strcpy(exit4->name, exitarray[0]);
						exit4->room = (long)atoi(exitarray[1]);
						exit4->key = (long)atoi(exitarray[2]);
						for (j=0; j<128; j++){
							if (exitarray[j+2] != 0){
								//printf("need to set an exit flag from array[%i]: %s \n", j+81+13*i, array[j+81+13*i]+1);
								F_SET(exit4, (long)atoi(exitarray[j+2])-1);
							}
						}
					}
					if (i == 3) {
						strcpy(exit3->name, exitarray[0]);
						exit3->room = (long)atoi(exitarray[1]);
						exit3->key = (long)atoi(exitarray[2]);
						for (j=0; j<128; j++){
							if (exitarray[j+2] != 0){
								//printf("need to set an exit flag from array[%i]: %s \n", j+81+13*i, array[j+81+13*i]+1);
								F_SET(exit3, (long)atoi(exitarray[j+2])-1);
							}
						}
					}
					if (i == 2) {
						strcpy(exit2->name, exitarray[0]);
						exit2->room = (long)atoi(exitarray[1]);
						exit2->key = (long)atoi(exitarray[2]);
						for (j=0; j<128; j++){
							if (exitarray[j+2] != 0){
								//printf("need to set an exit flag from array[%i]: %s \n", j+81+13*i, array[j+81+13*i]+1);
								F_SET(exit2, (long)atoi(exitarray[j+2])-1);
							}
						}

					}
					if (i == 1) {
						strcpy(exit1->name, exitarray[0]);
						exit1->room = (long)atoi(exitarray[1]);
						exit1->key = (long)atoi(exitarray[2]);
						for (j=0; j<128; j++){
							if (exitarray[j+2] != 0){
								//printf("need to set an exit flag from array[%i]: %s \n", j+81+13*i, array[j+81+13*i]+1);
								F_SET(exit1, (long)atoi(exitarray[j+2])-1);
							}
						}
					}
					if (i == 0) {
						strcpy(exit0->name, exitarray[0]);
						exit0->room = (long)atoi(exitarray[1]);
						exit0->key = (long)atoi(exitarray[2]);
						for (j=0; j<128; j++){
							if (exitarray[j+2] != 0){
								//printf("need to set an exit flag from array[%i]: %s \n", j+81+13*i, array[j+81+13*i]+1);
								F_SET(exit0, (long)atoi(exitarray[j+2])-1);
							}
						}
					}
					//printf("%s\n", exit->name);
					
					//printf("%s\n", array[79+13*i]);
					//printf("doing the key now\n");
					//printf("need to set a key flag from array[%i]: %s \n", 80+13*i, array[80+13*i]);
						

					//strcpy(exit->key, (long)atoi(array[80+13*i]));
						//printf("%s\n", array[80+13*i]);
					
					
					//printf("setting the exit tag now\n");
					//xtagA->ext = exit;
					//xtagA->next_tag = 0;
						
				}
					
				if (i == 7){
					xtagA->next_tag = 0;
					if (exit7 && strlen(exit7->name) > 1) xtagA->ext = exit7;
					else xtagA->ext = 0;
				}
				if (i == 6){
					if (!xtagA->ext) xtagB->next_tag =0;
					else xtagB->next_tag = xtagA;
					if (exit6 && strlen(exit6->name) > 1) xtagB->ext = exit6;
					else xtagB->ext = 0;
				}
				if (i == 5){
					if (!xtagB->ext) xtagC->next_tag =0;
					else xtagC->next_tag = xtagB;
					if (exit5 && strlen(exit5->name) > 1) xtagC->ext = exit5;
					else xtagC->ext = 0;
				}
				if (i == 4){
					if (!xtagC->ext) xtagD->next_tag =0;
					else xtagD->next_tag = xtagC;
					if (exit4 && strlen(exit4->name) > 1) xtagD->ext = exit4;
					else xtagD->ext = 0;
				}
				if (i == 3){
					if (!xtagD->ext) xtagE->next_tag =0;
					else xtagE->next_tag = xtagD;
					if (exit3 && strlen(exit3->name) > 1) xtagE->ext = exit3;
					else xtagE->ext = 0;
				}
				if (i == 2){
					if (!xtagE->ext) xtagF->next_tag =0;
					else xtagF->next_tag = xtagE;
					if (exit2 && strlen(exit2->name) > 1) xtagF->ext = exit2;
					else xtagF->ext = 0;
				}
				if (i == 1){
					if (!xtagF->ext) xtagG->next_tag =0;
					else xtagG->next_tag = xtagF;
					if (exit1 && strlen(exit1->name) > 1) xtagG->ext = exit1;
					else xtagG->ext = 0;
				}
				if (i == 0){
					if (!xtagG->ext) xtagH->next_tag =0;
					else xtagH->next_tag = xtagG;
					if (exit0 && strlen(exit0->name) > 1) xtagH->ext = exit0;
					else xtagH->ext = 0;
				}					
			}
				//printf("setting the first_ext tag\n");
				//xtagB = realloc(xtagB, sizeof(xtagA)+sizeof(rom_ptr->first_ext));
				//xtagB->next_tag = rom_ptr->first_ext;
				//xtagB->ext = xtagA->ext;		
			rom_ptr->first_ext = xtagH;
			//printf("death_rom: %s\n", array[182]);
			rom_ptr->death_rom = (long)atoi(array[128]);


				
			/*posit(5,20);*/ printf("end of values\n");
			/*save the creature*/
			/*sprintf(rom_ptr->password, "%d", temp);*/
			
				
			sprintf(file, "%s/o%02d", get_room_path(), index/MFILESIZE);
				
			if (index < 15001){
				//clearscreen();
				printf("roomname:");
				printf("%s\n",rom_ptr->name);
				printf("Description:");
				printf("%s %s\n",rom_ptr->short_desc, rom_ptr->long_desc);
				printf("traffic: %i\n", rom_ptr->traffic);

					
					//posit(25,40); printf("Hit Enter:");
					//getnum(&i,0,0);
				
				if (save_room(rom_ptr, index) < 0){
					printf("error with room %i\n", index);
				}	
			}
			c = -1;
			d++;
			memset(str1,0,strlen(str1));
			memset(str2,0,strlen(str2));

			}
			c++;
		}
		//end of if d > 0
		else {
			//printf("d is 0\n");
			c++;
			if (c == 129){
				c = 0;
				d++;
				memset(str1,0,strlen(str1));
				memset(str2,0,strlen(str2));
			}
		}
		/*}/*end of the else*/
	}while(result != EOF);

	fclose(fpt);
	posit(24,40); printf("Hit Enter:");
	getnum(&i,0,0);
	clearscreen();
	
	return;
}

void convert_to_csv(){
	clearscreen();
	posit(3,20); printf("Converting to csv");

	int i, j, m, n, o, p, q, r, c;
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
					case 0: flags0 = m+1;
					case 1: flags1 = m+1;
					case 2: flags2 = m+1;
					case 3: flags3 = m+1;
					case 4: flags4 = m+1;
					case 5: flags5 = m+1;
					case 6: flags6 = m+1;
					case 7: flags7 = m+1;
					case 8: flags8 = m+1;
					case 9: flags9 = m+1;
					case 10: flags10 = m+1;
					case 11: flags11 = m+1;
					case 12: flags12 = m+1;
					case 13: flags13 = m+1;
					case 14: flags14 = m+1;
					case 15: flags15 = m+1;
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
					case 0: spells0 = o+1;
					case 1: spells1 = o+1;
					case 2: spells2 = o+1;
					case 3: spells3 = o+1;
					case 4: spells4 = o+1;
					case 5: spells5 = o+1;
					case 6: spells6 = o+1;
					case 7: spells7 = o+1;
					case 8: spells8 = o+1;
					case 9: spells9 = o+1;
					case 10: spells10 = o+1;
					case 11: spells11 = o+1;
					case 12: spells12 = o+1;
					case 13: spells13 = o+1;
					case 14: spells14 = o+1;
					case 15: spells15 = o+1;
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


			fprintf(fpt,"%i;%s;%s;%s;%s;%s;%s;%s;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i\n",
			i, crt_ptr->name, crt_ptr->description, crt_ptr->talk, crt_ptr->password, crt_ptr->key[0], crt_ptr->key[1], crt_ptr->key[2], crt_ptr->fd, crt_ptr->level, crt_ptr->type, crt_ptr->class, crt_ptr->race, crt_ptr->numwander, crt_ptr->alignment, crt_ptr->strength, crt_ptr->dexterity, crt_ptr->constitution, crt_ptr->intelligence, crt_ptr->piety, crt_ptr->hpmax, crt_ptr->hpcur, crt_ptr->mpmax, crt_ptr->mpcur, crt_ptr->armor, crt_ptr->thaco, crt_ptr->experience, crt_ptr->gold, crt_ptr->ndice, crt_ptr->sdice, crt_ptr->pdice, crt_ptr->special, crt_ptr->proficiency[0], crt_ptr->proficiency[1], crt_ptr->proficiency[2], crt_ptr->proficiency[3], crt_ptr->proficiency[4], crt_ptr->proficiency[5], crt_ptr->realm[0], crt_ptr->realm[1], crt_ptr->realm[2], crt_ptr->realm[3], crt_ptr->realm[4], crt_ptr->realm[5], crt_ptr->realm[6], crt_ptr->realm[7], spells0, spells1, spells2, spells3, spells4, spells5, spells6, spells7, spells8, spells9, spells10, spells11, spells12, spells13, spells14, spells15, flags0, flags1, flags2, flags3, flags4, flags5, flags6, flags7, flags8, flags9, flags10, flags11, flags12, flags13, flags14, flags15, quests0, quests1, quests2, quests3, quests4, quests5, quests6, quests7, quests8, quests9, quests10, quests11, quests12, quests13, quests14, quests15, crt_ptr->questnum, crt_ptr->carry[0], crt_ptr->carry[1], crt_ptr->carry[2], crt_ptr->carry[3], crt_ptr->carry[4], crt_ptr->carry[5], crt_ptr->carry[6], crt_ptr->carry[7], crt_ptr->carry[8], crt_ptr->carry[9], crt_ptr->rom_num, crt_ptr->bank_balance, crt_ptr->title, crt_ptr->misc_stats[0], crt_ptr->misc_stats[1], crt_ptr->misc_stats[2], crt_ptr->misc_stats[3], crt_ptr->misc_stats[4], crt_ptr->clanindex, crt_ptr->clanexp, crt_ptr->guildtype, crt_ptr->guildexp, crt_ptr->special1, crt_ptr->special2 );
			}
			else {
				posit(8, 20); printf("error on monster %i \n", i );
				//posit(24,40); printf("Hit Enter:"); getnum(&c,0,0);
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
						case 0: flags0 = m+1;
						case 1: flags1 = m+1;
						case 2: flags2 = m+1;
						case 3: flags3 = m+1;
						case 4: flags4 = m+1;
						case 5: flags5 = m+1;
						case 6: flags6 = m+1;
						case 7: flags7 = m+1;
						case 8: flags8 = m+1;
						case 9: flags9 = m+1;
						case 10: flags10 = m+1;
						case 11: flags11 = m+1;
						case 12: flags12 = m+1;
						case 13: flags13 = m+1;
						case 14: flags14 = m+1;
						case 15: flags15 = m+1;
					}
					n++;

				}
			}
		


		fprintf(fpt,"%i;%s;%s;%s;%s;%s;%s;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i\n",
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
	fprintf(fpt, "rom_num; name; short_desc; long_desc; lolevel; hilevel; special; trap; trapexit; track; flags0; flags1; flags2; flags3; flags4; flags5; flags6; flags7; flags8; flags9; flags10; flags11; flags12; flags13; flags14; flags15; random0; random1; random2; random3; random4; random5; random6; random7; random8; random9; traffic; perm0; ptime0; perm1; ptime1; perm2; ptime2; perm3; ptime3; perm4; ptime4; perm5; ptime5; perm6; ptime6; perm7; ptime7; perm8; ptime8; perm9; ptime9; operm0; optime0; operm1; optime1; operm2; optime2; operm3; optime3; operm4; optime4; operm5; optime5; operm6; optime6; operm7; optime7; operm8; optime8; operm9; optime9; beenhere; x1name; x1destination; x1key; x1flag0; x1flag1; x1flag2; x1flag3; x1flag4; x1flag5; x1flag6; x1flag7; x1flag8; x1flag9; x2name; x2destination; x2key; x2flag0; x2flag1; x2flag2; x2flag3; x2flag4; x2flag5; x2flag6; x2flag7; x2flag8; x2flag9; x3name; x3destination; x3key; x3flag0; x3flag1; x3flag2; x3flag3; x3flag4; x3flag5; x3flag6; x3flag7; x3flag8; x3flag9; x4name; x4destination; x4key; x4flag0; x4flag1; x4flag2; x4flag3; x4flag4; x4flag5; x4flag6; x4flag7; x4flag8; x4flag9; x5name; x5destination; x5key; x5flag0; x5flag1; x5flag2; x5flag3; x5flag4; x5flag5; x5flag6; x5flag7; x5flag8; x5flag9; x6name; x6destination; x6key; x6flag0; x6flag1; x6flag2; x6flag3; x6flag4; x6flag5; x6flag6; x6flag7; x6flag8; x6flag9; x7name; x7destination; x7key; x7flag0; x7flag1; x7flag2; x7flag3; x7flag4; x7flag5; x7flag6; x7flag7; x7flag8; x7flag9; x8name; x8destination; x8key; x8flag0; x8flag1; x8flag2; x8flag3; x8flag4; x8flag5; x8flag6; x8flag7; x8flag8; x8flag9; death_rom\n");
	fclose(fpt);

	char xnames[8][20];
	char str1[2000];
	char str2[2000];
	char tempstring[2000];
	char exitstr[2000];
	int len;
	int l, k;
	int xrooms[8];
	int xkeys[8];
	int xkey;
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

	fpt = fopen("rooms_datatbase.csv", "w+");
	for (i=0, j=0; i<15000; i++){
		printf("converting room %i\n", i);
		//hello
		/*for (o=0; o<10; o++){
									
			x1flags[o] = 0;
			x2flags[o] = 0;
			x3flags[o] = 0;
			x4flags[o] = 0;
			x5flags[o] = 0;
			x6flags[o] = 0;
			x7flags[o] = 0;
			x8flags[o] = 0;
					
		}*/
		
		if (load_rom(i, &rom_ptr) > -1) {
			load_rom_from_file(i, &rom_ptr);

			
			printf("room %i located and opened\n", i);

			xt = rom_ptr->first_ext;

			memset(str1,0,strlen(str1));
			if (rom_ptr->short_desc){
			strcpy(str1, rom_ptr->short_desc);
			len = strlen(str1);
				for (m=0; m<len-1; m++){
					if (str1[m] == '\n' || str1[m] == ';'){
						str1[m] = ' ';
					}
				}
			}
			memset(str2,0,strlen(str2));
			if (rom_ptr->long_desc){
				strcpy(str2, rom_ptr->long_desc);
				len = strlen(str2);
				for (m=0; m<len-1; m++){
					if (str2[m] == '\n' || str2[m] == ';'){
						str2[m] = ' ';
					}
				}
			}
		
		
			/*for (m=0, n=0; m<8; m++){
				
				memset(xnames[m],0,strlen(xnames[m]));
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
									case 0: x1flags[p] = o+1;
									case 1: x2flags[p] = o+1;
									case 2: x3flags[p] = o+1;
									case 3: x4flags[p] = o+1;
									case 4: x5flags[p] = o+1;
									case 5: x6flags[p] = o+1;
									case 6: x7flags[p] = o+1;
									case 7: x8flags[p] = o+1;
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
						case 0: flags0 = m+1;
						case 1: flags1 = m+1;
						case 2: flags2 = m+1;
						case 3: flags3 = m+1;
						case 4: flags4 = m+1;
						case 5: flags5 = m+1;
						case 6: flags6 = m+1;
						case 7: flags7 = m+1;
						case 8: flags8 = m+1;
						case 9: flags9 = m+1;
						case 10: flags10 = m+1;
						case 11: flags11 = m+1;
						case 12: flags12 = m+1;
						case 13: flags13 = m+1;
						case 14: flags14 = m+1;
						case 15: flags15 = m+1;
					}
					n++;

				}
			}

		
		


			
			fprintf(fpt,"%i;",i);
			fprintf(fpt,"%s;", rom_ptr->name);
			fprintf(fpt,"%s;", str1);
			fprintf(fpt,"%s;", str2);
			fprintf(fpt,"%i;", rom_ptr->lolevel);
			fprintf(fpt,"%i;", rom_ptr->hilevel);
			fprintf(fpt,"%i;", rom_ptr->special);
			fprintf(fpt,"%i;", rom_ptr->trap);
			fprintf(fpt,"%i;", rom_ptr->trapexit);
			fprintf(fpt,"%s;", rom_ptr->track);
			fprintf(fpt,"%i;", flags0);
			fprintf(fpt,"%i;", flags1);
			fprintf(fpt,"%i;", flags2);
			fprintf(fpt,"%i;", flags3);
			fprintf(fpt,"%i;", flags4);
			fprintf(fpt,"%i;", flags5);
			fprintf(fpt,"%i;", flags6);
			fprintf(fpt,"%i;", flags7);
			fprintf(fpt,"%i;", flags8);
			fprintf(fpt,"%i;", flags9);
			fprintf(fpt,"%i;", flags10);
			fprintf(fpt,"%i;", flags11);
			fprintf(fpt,"%i;", flags12);
			fprintf(fpt,"%i;", flags13);
			fprintf(fpt,"%i;", flags14);
			fprintf(fpt,"%i;", flags15);
			fprintf(fpt,"%i;", rom_ptr->random[0]);
			fprintf(fpt,"%i;", rom_ptr->random[1]);
			fprintf(fpt,"%i;", rom_ptr->random[2]);
			fprintf(fpt,"%i;", rom_ptr->random[3]);
			fprintf(fpt,"%i;", rom_ptr->random[4]);
			fprintf(fpt,"%i;", rom_ptr->random[5]);
			fprintf(fpt,"%i;", rom_ptr->random[6]);
			fprintf(fpt,"%i;", rom_ptr->random[7]);
			fprintf(fpt,"%i;", rom_ptr->random[8]);
			fprintf(fpt,"%i;", rom_ptr->random[9]);
			//int * temp;
			//char * temp2;
			//temp2 = rom_ptr->traffic;
			//temp = atoi(temp2);
			//printf("Room Traffic is  %i\n", rom_ptr->traffic );
			fprintf(fpt,"%i;", (rom_ptr->traffic - '0' +48));
			fprintf(fpt,"%i;", rom_ptr->perm_mon[0].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[0].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[1].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[1].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[2].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[2].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[3].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[3].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[4].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[4].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[5].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[5].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[6].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[6].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[7].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[7].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[8].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[8].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[9].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_mon[9].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[0].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[0].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[1].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[1].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[2].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[2].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[3].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[3].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[4].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[4].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[5].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[5].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[6].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[6].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[7].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[7].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[8].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[8].interval);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[9].misc);
			fprintf(fpt,"%i;", rom_ptr->perm_obj[9].interval);
			fprintf(fpt,"%i;", rom_ptr->beenhere);
			
			

			//for the exits, compile a string structured as follows:
			//"name, room, key, flag1, 2, 3, 4, 5, 6, 7, 8, 9, 10;"
			//capacity for up to 50 exits
		
			
			xt = rom_ptr->first_ext;
			for (m=0, n=0; m<50; m++){
				strcpy(exitstr, "");
				strcpy(tempstring, "");
				
				if (n==0 && xt){
					if (xt->ext){
						strcpy(exitstr, "");
					
						strcpy(tempstring, "");
						sprintf(tempstring, "%s", xt->ext->name);
						strcat(exitstr, tempstring);
					
						strcat(exitstr, ",");
					
						strcpy(tempstring, "");
						sprintf(tempstring, "%i", xt->ext->room);
						strcat(exitstr, tempstring);
					 
						strcat(exitstr, ",");
					
						strcpy(tempstring, "");
						xkey = xt->ext->key;
						sprintf(tempstring, "%i", xkey);
						strcat(exitstr, tempstring);
					 
						strcat(exitstr, ",");
						//flags now
				
						//if there is an exit
					
						//accept all 128 flags
						for (o=0, p=0; o<128; o++){
						
						
								if (F_ISSET(xt->ext, o)){
									strcpy(tempstring, "");
									sprintf(tempstring, "%i", o+1);
									strcat(exitstr, tempstring);
								
								}
							
						
							strcat(exitstr, ", ");
						}
					
				
						strcat(exitstr, ";");
					}
				}
			 	else if (n > 0 || !xt) strcat(exitstr, "0;");
				//get ready for the next one
			
				fprintf(fpt, "%s", exitstr);
				printf("Exit %i: %s\n", m,  exitstr );
				if (xt){
					if (xt->next_tag){
						xt = xt->next_tag;	
					}
					else{
						n++;
					}
				}
				else {
					n++;
				}

			}
			
			printf("%i\n", rom_ptr->death_rom);
			fprintf(fpt, "%i\n", rom_ptr->death_rom);
				
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

