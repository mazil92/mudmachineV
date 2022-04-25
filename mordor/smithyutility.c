/*
Some utilities by smithy

28/03/2022: Skill add and skill check functions
			skilltree command

27/03/2022: objectives command now reads from 
			objectives file in ../bin

21/01/2022: detect spell realm

20/01/2022: Objectives Command

09/01/2022: identify spell
*/

#include "../include/mordb.h"
#include "mextern.h"


#include <ctype.h>
#include <string.h>

int multi_objective_monster (creature *crt_ptr){
	char str[21];
	char objstr[5];
	sprintf(str, "%s\0", crt_ptr->title);
	//all numbers must be of the form "xxxxyyyyzzzzwwww"

	//the string will be structured as follows:
	//xxxxyyyyzzzzwwww
	//eg 0100019001200124
	//100, 190, 120, 124. Being the lower, upper bounds of the multi-objective
	// and lastly, the individual objective being referenced form that superset
	// the last one is the target value to be above or below

	//elog("multi_objective target requested for an item");
	if ((F_ISSET(crt_ptr, MMULTIOBJSEE)) && crt_ptr->type == MONSTER){
		
		//elog("it has multiobj set");
		sprintf(objstr, "%c%c%c%c\0", str[8],str[9],str[10],str[11]);
		//sprintf(g_buffer, "%i", crt_ptr->sets_flag[3]);
		//elog(g_buffer);
		return(atoi(objstr));
	}
	else {
		return(0);
	}	
}
int multi_objective_monster_upper (creature *crt_ptr){
	char str[21];
	char objstr[5];
	sprintf(str, "%s\0", crt_ptr->title);
	//all numbers must be of the form "111xxxxyyyyzzzzwwww"

	//the string will be structured as follows:
	//xxxxyyyyzzzzwwww
	//eg 0100019001200124
	//100, 190, 120, 124. Being the lower, upper bounds of the multi-objective
	// and lastly, the individual objective being referenced form that superset
	// the last one is the target value to be above or below

	//elog("multi_objective target requested for an item");
	if ((F_ISSET(crt_ptr, MMULTIOBJSEE)) && crt_ptr->type == MONSTER){
		
		//elog("it has multiobj set");
		sprintf(objstr, "%c%c%c%c\0", str[0],str[1],str[2],str[3]);
		//sprintf(g_buffer, "%i", crt_ptr->sets_flag[3]);
		//elog(g_buffer);
		return(atoi(objstr));
	}
	else {
		return(0);
	}	
}
int multi_objective_monster_lower (creature *crt_ptr){
	char str[21];
	char objstr[5];
	sprintf(str, "%s\0", crt_ptr->title);
	//all numbers must be of the form "111xxxxyyyyzzzzwwww"

	//the string will be structured as follows:
	//xxxxyyyyzzzzwwww
	//eg 0100019001200124
	//100, 190, 120, 124. Being the lower, upper bounds of the multi-objective
	// and lastly, the individual objective being referenced form that superset
	// the last one is the target value to be above or below

	//elog("multi_objective target requested for an item");
	if ((F_ISSET(crt_ptr, MMULTIOBJSEE)) && crt_ptr->type == MONSTER){
		
		//elog("it has multiobj set");
		sprintf(objstr, "%c%c%c%c\0", str[4],str[5],str[6],str[7]);
		//sprintf(g_buffer, "%i", crt_ptr->sets_flag[3]);
		//elog(g_buffer);
		return(atoi(objstr));
	}
	else {
		return(0);
	}	
}
int multi_objective_monster_target (creature *crt_ptr){
	char str[21];
	char objstr[5];
	sprintf(str, "%s\0", crt_ptr->title);
	//all numbers must be of the form "xxxxyyyyzzzzwwww"

	//the string will be structured as follows:
	//xxxxyyyyzzzzwwww
	//eg 0100019001200124
	//100, 190, 120, 124. Being the lower, upper bounds of the multi-objective
	// and lastly, the individual objective being referenced form that superset
	// the last one is the target value to be above or below

	//elog("multi_objective target requested for an item");
	if ((F_ISSET(crt_ptr, MMULTIOBJSEE) || (F_ISSET(crt_ptr, MOBJECTIVESEE))) && crt_ptr->type == MONSTER){
		
		//elog("it has multiobj set");
		sprintf(objstr, "%c%c%c%c\0", str[12],str[13],str[14],str[15]);
		//sprintf(g_buffer, "%i", crt_ptr->sets_flag[3]);
		//elog(g_buffer);
		return(atoi(objstr));
	}
	else {
		return(0);
	}	
}

int objective_monster_checker(creature *ply_ptr, creature *crt_ptr){
	//this one just checks if you're allowed through an exit
	//returns 1 on success 0 on failure
	//output(ply_ptr->fd, "checking your multi objective exit");
	//output(ply_ptr->fd, "running an objective monster check for" );
	//output(ply_ptr->fd, ply_ptr->name);
	//output(ply_ptr->fd, "\n");
	if (ply_ptr->class > BUILDER){
		return (1);
	}
	if (F_ISSET(crt_ptr, MOBJECTIVESEE)  && crt_ptr->type == MONSTER){
		//sprintf(g_buffer, "multi objective check %i, exit %i, target %i \n", multi_objective_check(ply_ptr, multi_objective_exit(ext)), multi_objective_exit(ext), multi_objective_exit_target(ext));
		//output(ply_ptr->fd, g_buffer);
		//output(ply_ptr->fd, "it's a Mobjective monster!");
		if (objective_check(ply_ptr, multi_objective_monster_target(crt_ptr))){
			//output(ply_ptr->fd, "it met the requirements!\n");
			if (F_ISSET(crt_ptr, MINVERTEDOBJ)){
				return(0);
			} else return (1);
		}
		
		//its an objective exit but they don't meet the requirements
		if (F_ISSET(crt_ptr, MINVERTEDOBJ)){
			return(1);
		} else return (0);
	}
	else {
		//its not a multi objective exit so they don't need the requirements
		return(1);
	}
	//not sure how you might get to this point but return a fail
	return(0);
}

int multi_objective_monster_checker(creature *ply_ptr, creature *crt_ptr){
	//this one just checks if you're allowed through an exit
	//returns 1 on success 0 on failure
	int ret =0;
	if (ply_ptr->class > BUILDER){
		return (1);
	}
	//output(ply_ptr->fd, "checking your multi objective exit");
	if (F_ISSET(crt_ptr, MMULTIOBJSEE) && crt_ptr->type == MONSTER){
		//sprintf(g_buffer, "multi objective check %i, exit %i, target %i \n", multi_objective_check(ply_ptr, multi_objective_exit(ext)), multi_objective_exit(ext), multi_objective_exit_target(ext));
		//output(ply_ptr->fd, g_buffer);
		if (F_ISSET(crt_ptr, MMULTIOBJABOVE)){
			if (F_ISSET(crt_ptr, MPREMPTIVEOBJ)){
				if ((pre_multi_objective_checker(ply_ptr, multi_objective_monster_lower(crt_ptr), multi_objective_monster_upper(crt_ptr), multi_objective_monster(crt_ptr)) > multi_objective_monster_target(crt_ptr))){
					ret = 1;
				}
			}
			else if ((multi_objective_check(ply_ptr, multi_objective_monster(crt_ptr)) && multi_objective_check(ply_ptr, multi_objective_monster(crt_ptr)) > multi_objective_monster_target(crt_ptr))){
				ret = 1;
			}
		}
		else if (F_ISSET(crt_ptr, MMULTIOBJBELOW)){
			//output(ply_ptr->fd, "exit below value required");
			if (F_ISSET(crt_ptr, MPREMPTIVEOBJ)){
				if ((pre_multi_objective_checker(ply_ptr, multi_objective_monster_lower(crt_ptr), multi_objective_monster_upper(crt_ptr), multi_objective_monster(crt_ptr))) && (pre_multi_objective_checker(ply_ptr, multi_objective_monster_lower(crt_ptr), multi_objective_monster_upper(crt_ptr), multi_objective_monster(crt_ptr)) < multi_objective_monster_target(crt_ptr))){
					ret = 1;
				}
			}
			else if ((multi_objective_check(ply_ptr, multi_objective_monster(crt_ptr)) && multi_objective_check(ply_ptr, multi_objective_monster(crt_ptr)) < multi_objective_monster_target(crt_ptr))){
				ret = 1;
			}

		}
		
	}
	else {
		//its not a multi objective exit so they don't need the requirements
		ret = 1;
	}
	//not sure how you might get to this point but return a fail
	if (F_ISSET(crt_ptr, MINVERTEDOBJ)){
		ret = 1 - ret;
	} 
	return(ret);
}

int multi_objective_item (object *obj_ptr){
	//char str[20];
	//char objstr[5];
	//sprintf(str, "%Lf", obj_ptr->special2);
	//all numbers must be of the form "111xxxxyyyyzzzzwwww"

	//the string will be structured as follows:
	//xxxxyyyyzzzzwwww
	//eg 0100019001200124
	//100, 190, 120, 124. Being the lower, upper bounds of the multi-objective
	// and lastly, the individual objective being referenced form that superset
	// the last one is the target value to be above or below

	
	if (F_ISSET(obj_ptr, OMULTIOBJSEE)){
		//sprintf(objstr, "%c%c%c%c\0", str[15],str[16],str[17],str[18]);
		return(obj_ptr->sets_flag[2]);
	}
	else {
		return(0);
	}
}
int multi_objective_item_upper (object *obj_ptr){
	//char str[20];
	//char objstr[5];
	//sprintf(str, "%Lf", obj_ptr->special2);
	//all numbers must be of the form "111xxxxyyyyzzzzwwww"

	//the string will be structured as follows:
	//xxxxyyyyzzzzwwww
	//eg 0100019001200124
	//100, 190, 120, 124. Being the lower, upper bounds of the multi-objective
	// and lastly, the individual objective being referenced form that superset
	// the last one is the target value to be above or below

	
	if (F_ISSET(obj_ptr, OMULTIOBJSEE)){
		//sprintf(objstr, "%c%c%c%c\0", str[15],str[16],str[17],str[18]);
		return(obj_ptr->sets_flag[1]);
	}
	else {
		return(0);
	}
}
int multi_objective_item_lower (object *obj_ptr){
	//char str[20];
	//char objstr[5];
	//sprintf(str, "%Lf", obj_ptr->special2);
	//all numbers must be of the form "111xxxxyyyyzzzzwwww"

	//the string will be structured as follows:
	//xxxxyyyyzzzzwwww
	//eg 0100019001200124
	//100, 190, 120, 124. Being the lower, upper bounds of the multi-objective
	// and lastly, the individual objective being referenced form that superset
	// the last one is the target value to be above or below

	
	if (F_ISSET(obj_ptr, OMULTIOBJSEE)){
		//sprintf(objstr, "%c%c%c%c\0", str[15],str[16],str[17],str[18]);
		return(obj_ptr->sets_flag[0]);
	}
	else {
		return(0);
	}
}
int multi_objective_item_target (object *obj_ptr){
	//char str[20];
	//char objstr[5];
	//sprintf(str, "%Lf", obj_ptr->special2);
	//all numbers must be of the form "111xxxxyyyyzzzzwwww"

	//the string will be structured as follows:
	//xxxxyyyyzzzzwwww
	//eg 0100019001200124
	//100, 190, 120, 124. Being the lower, upper bounds of the multi-objective
	// and lastly, the individual objective being referenced form that superset
	// the last one is the target value to be above or below

	//elog("multi_objective target requested for an item");
	if (F_ISSET(obj_ptr, OMULTIOBJSEE) || F_ISSET(obj_ptr, OOBJECTIVESEE)){
		
		//elog("it has multiobj set");
		//sprintf(objstr, "%c%c%c%c\0", str[15],str[16],str[17],str[18]);
		sprintf(g_buffer, "%i", obj_ptr->sets_flag[3]);
		//elog(g_buffer);
		return(obj_ptr->sets_flag[3]);
	}
	else {
		return(0);
	}	
}

int objective_item_checker(creature *ply_ptr, object *obj_ptr){
	//this one just checks if you're allowed through an exit
	//returns 1 on success 0 on failure
	//output(ply_ptr->fd, "checking your multi objective exit");
	if (ply_ptr->class > BUILDER){
		return (1);
	}
	if (F_ISSET(obj_ptr, OOBJECTIVESEE )){
		//sprintf(g_buffer, "multi objective check %i, exit %i, target %i \n", multi_objective_check(ply_ptr, multi_objective_exit(ext)), multi_objective_exit(ext), multi_objective_exit_target(ext));
		//output(ply_ptr->fd, g_buffer);
		if (objective_check(ply_ptr, multi_objective_item_target(obj_ptr))){
			if (F_ISSET(obj_ptr, OINVERTEDOBJ)){
				return(0);
			} else return (1);
		}
		
		//its an objective exit but they don't meet the requirements
		if (F_ISSET(obj_ptr, OINVERTEDOBJ)){
			return(1);
		} else return (0);
	}
	else {
		//its not a multi objective exit so they don't need the requirements
		return(1);
	}
	//not sure how you might get to this point but return a fail
	return(0);
}

int multi_objective_item_checker(creature *ply_ptr, object *obj_ptr){
	//this one just checks if you're allowed through an exit
	//returns 1 on success 0 on failure
	int ret =0;
	if (ply_ptr->class > BUILDER){
		return (1);
	}
	//output(ply_ptr->fd, "checking your multi objective exit");
	if (F_ISSET(obj_ptr, OMULTIOBJSEE)){
		//sprintf(g_buffer, "multi objective check %i, exit %i, target %i \n", multi_objective_check(ply_ptr, multi_objective_exit(ext)), multi_objective_exit(ext), multi_objective_exit_target(ext));
		//output(ply_ptr->fd, g_buffer);
		if (F_ISSET(obj_ptr, OMULTIOBJABOVE)){
			if (F_ISSET(obj_ptr, OPREMTPIVEOBJ)){
				if ((pre_multi_objective_checker(ply_ptr, multi_objective_item_lower(obj_ptr), multi_objective_item_upper(obj_ptr), multi_objective_item(obj_ptr)) > multi_objective_item_target(obj_ptr))){
					ret = 1;
				}
			}
			else if ((multi_objective_check(ply_ptr, multi_objective_item(obj_ptr)) && multi_objective_check(ply_ptr, multi_objective_item(obj_ptr)) > multi_objective_item_target(obj_ptr))){
				ret = 1;
			}
		}
		else if (F_ISSET(obj_ptr, OMULTIOBJBELOW)){
			//output(ply_ptr->fd, "exit below value required");
			if (F_ISSET(obj_ptr, OPREMTPIVEOBJ)){
				if ((pre_multi_objective_checker(ply_ptr, multi_objective_item_lower(obj_ptr), multi_objective_item_upper(obj_ptr), multi_objective_item(obj_ptr))) && (pre_multi_objective_checker(ply_ptr, multi_objective_item_lower(obj_ptr), multi_objective_item_upper(obj_ptr), multi_objective_item(obj_ptr)) < multi_objective_item_target(obj_ptr))){
					ret = 1;
				}
			}
			else if ((multi_objective_check(ply_ptr, multi_objective_item(obj_ptr)) && multi_objective_check(ply_ptr, multi_objective_item(obj_ptr)) < multi_objective_item_target(obj_ptr))){
				ret = 1;
			}

		}
		
	}
	else {
		//its not a multi objective exit so they don't need the requirements
		ret = 1;
	}
	//not sure how you might get to this point but return a fail
	if (F_ISSET(obj_ptr, OINVERTEDOBJ)){
		ret = 1 - ret;
	} 
	return(ret);
}

int pre_multi_objective_checker(creature *ply_ptr, int lowbound, int highbound, int number){
//checks what value a mluti-objective WOULD be if a player
//were to complete it next in that particular multi-sequence

	int i, j, count;

	int fd = ply_ptr->fd;
	//set the objective for number. if they already 
	// have it set, then let them know
	if (objective_check(ply_ptr, number)){
		return(0);
	}

	//examine which objectives within the bounds have already been set
	count = 0;
	for (i = lowbound/10; i<highbound/10+1; i++){
		sprintf(g_buffer, "hi");
		output(fd, "");
		if (objective_check(ply_ptr, i*10)){
			sprintf(g_buffer, "hi");
			output(fd, "");
			count++;
		}
	}
	
	return(number + 1 + count);
}

int objective_exit_checker(creature *ply_ptr, exit_ *ext){
	//this one just checks if you're allowed through an exit
	//returns 1 on success 0 on failure
	//output(ply_ptr->fd, "checking your multi objective exit");
	if (F_ISSET(ext, XOBJECTIVESEE ) || F_ISSET(ext, XOBJECTIVEGO )){
		//sprintf(g_buffer, "multi objective check %i, exit %i, target %i \n", multi_objective_check(ply_ptr, multi_objective_exit(ext)), multi_objective_exit(ext), multi_objective_exit_target(ext));
		//output(ply_ptr->fd, g_buffer);
		if (objective_check(ply_ptr, objective_exit(ext))){
			if (F_ISSET(ext, XINVERTEDOBJECTIVE)){
				return(0);
			} else return (1);
		}
		
		//its an objective exit but they don't meet the requirements
		if (F_ISSET(ext, XINVERTEDOBJECTIVE)){
			return(1);
		} else return (0);
	}
	else {
		//its not a multi objective exit so they don't need the requirements
		return(1);
	}
	//not sure how you might get to this point but return a fail
	return(0);
}

int multi_objective_exit_checker(creature *ply_ptr, exit_ *ext){
	//this one just checks if you're allowed through an exit
	//returns 1 on success 0 on failure
	int ret =0;
	//output(ply_ptr->fd, "checking your multi objective exit");
	if (F_ISSET(ext, XMULTIOBJSEE) || F_ISSET(ext, XMULTIOBJGO)){
		//sprintf(g_buffer, "multi objective check %i, exit %i, target %i \n", multi_objective_check(ply_ptr, multi_objective_exit(ext)), multi_objective_exit(ext), multi_objective_exit_target(ext));
		//output(ply_ptr->fd, g_buffer);
		if (F_ISSET(ext, XMULTIOBJABOVE)){
			if (F_ISSET(ext, XPREMPTIVEOBJECTIVE)){
				if ((pre_multi_objective_checker(ply_ptr, multi_objective_exit_lower(ext), multi_objective_exit_upper(ext), multi_objective_exit(ext)) > multi_objective_exit_target(ext))){
					ret = 1;
				}
			}
			else if ((multi_objective_check(ply_ptr, multi_objective_exit(ext)) && multi_objective_check(ply_ptr, multi_objective_exit(ext)) > multi_objective_exit_target(ext))){
				ret = 1;
			}
		}
		else if (F_ISSET(ext, XMULTIOBJBELOW)){
			//output(ply_ptr->fd, "exit below value required");
			if (F_ISSET(ext, XPREMPTIVEOBJECTIVE)){
				if ((pre_multi_objective_checker(ply_ptr, multi_objective_exit_lower(ext), multi_objective_exit_upper(ext), multi_objective_exit(ext))) && (pre_multi_objective_checker(ply_ptr, multi_objective_exit_lower(ext), multi_objective_exit_upper(ext), multi_objective_exit(ext)) < multi_objective_exit_target(ext))){
					ret = 1;
				}
			}
			else if ((multi_objective_check(ply_ptr, multi_objective_exit(ext)) && multi_objective_check(ply_ptr, multi_objective_exit(ext)) < multi_objective_exit_target(ext))){
				ret = 1;
			}

		}
		
	}
	else {
		//its not a multi objective exit so they don't need the requirements
		ret = 1;
	}
	//not sure how you might get to this point but return a fail
	if (F_ISSET(ext, XINVERTEDOBJECTIVE)){
		ret = 1 - ret;
	} 
	return(ret);
}


//multi_objective_check
/*returns 0 if the player has not done that multi-objective yet
otherwise returns the objective indicative of the order the completed
that particular part of the sequence

The input number is the objective series you want to check
eg, in the series between 100 and 190, you might want to check series 150
and it will return a number eg 152 to indicate it was the 2nd completed
in the series
*/
int multi_objective_check(creature *ply_ptr, int number){
	int i;
	
	if (objective_check(ply_ptr, number)){
		for (i=1; i<10; i++){
			
			if (objective_check(ply_ptr, number+i)){
				return(number+i);
			}
		}
		return(number+10);
	}
	else return(0);

}

//objective exp

/*TO assign an objective with experience, 
go to the objectives.master file in ../bin
and then create an entry following the format
xx,xx
value

where xx is the objective number you want to be assigned
with a particular exp of of "value"
the array will only consider entires that have the 
same objective number repeated twice
these value do not show up in the regular
"objectives" command display.*/
int objective_exp(int objective){

	//find the master objectives file in ../bin
	//read the objectives file
	//the structure of the file is pairs of lines
	//first lines give the lower and upper 
	//bounds to display the objective
	//second lines give the data to be displayed
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;

	strcpy(filename, "../bin/objectives.master\0");
	fpt = fopen(filename, "r");
	char array[750][80];
	int c,i,j, result;
	int ret = 0;

	//clean the array
	for (i=0; i<750; i++){
		strcpy(array[i], "");
	}

	//iterate through the file and load the values into 
	//a handy array for future reading
	c = 0;
	result =0;
	while (result != EOF){
		result = fscanf(fpt, "%79[^,\n]", str);
		result = fscanf(fpt, "%*c");
		strcpy(array[c], str);
		
		//sprintf(g_buffer, "array %i: ", c);
		//output(fd, g_buffer);
		//output(fd, array[c]);
		//output(fd, "\n");

		c++;
	}
	fclose(fpt);



	for (i=0; i<c+1; i++){
		if (array[3*i+0]){
			//elog(array[3*i+0]); elog(array[3*i+1]); elog(array[3*i+2]);
			if(!strcmp(array[3*i+0], (array[3*i+1]))){
				if (atoi(array[3*i+0]) == objective){
					ret = atoi(array[3*i+2]);
				}
			}
		}
	}
	
	return (ret);
}

//multi objectives are defined by the special 1 and 2 values
//of the item being picked up
//as well as the normal sets_flag[15] value
int multi_objective_item_pickup(creature *ply_ptr, object *obj_ptr){
	int fd = ply_ptr->fd;


	if (objective_check(ply_ptr, obj_ptr->sets_flag[15])){
   		if (F_ISSET(obj_ptr, ONOTGIVEN)){
   			ANSI(fd, AFC_BLUE);
   			output(fd, "A message reverberates in your mind\n");
   			output(ply_ptr->fd, obj_ptr->description);output(ply_ptr->fd, "\n");
			ANSI(fd, AM_BOLD);
			output(ply_ptr->fd, obj_ptr->use_output);output(ply_ptr->fd, "\n");
			ANSI(fd, AFC_WHITE);
			ANSI(fd, AM_NORMAL);
   		}
   		else {
   			output(ply_ptr->fd, "You may not get that. You have already fulfilled that objective.\n");
		
   		}	
		return(0);
   	}
   	else {
   		
   		multi_objective(ply_ptr, obj_ptr->special1, obj_ptr->special2, obj_ptr->sets_flag[15]);
   		//sprintf(g_buffer, "%i", objective_exp(obj_ptr->sets_flag[15])); output(fd, g_buffer);
   		
   		if (F_ISSET(obj_ptr, ONOTGIVEN)){
   			//the monster doesn't actually give the item, they're just
   			//awarding the objective and that's the end of it.
   			ANSI(fd, AFC_BLUE);
   			output(ply_ptr->fd, obj_ptr->description);output(ply_ptr->fd, "\n");
			ANSI(fd, AM_BOLD);
			output(ply_ptr->fd, obj_ptr->use_output);output(ply_ptr->fd, "\n");
			ANSI(fd, AFC_WHITE);
			ANSI(fd, AM_NORMAL);
   			//The player gets to see a hint, stored in the item's description
   			return(2);
   		}
   		else{
   			return(1);
   		}
   	}
   	return(0);
}

//objective item pickup
int objective_item_pickup(creature *ply_ptr, object *obj_ptr){
	int fd;

	fd = ply_ptr->fd;

	if (objective_check(ply_ptr, obj_ptr->sets_flag[15])){
   		if (F_ISSET(obj_ptr, ONOTGIVEN)){
   			ANSI(fd, AFC_BLUE);
   			output(fd, "A message reverberates in your mind\n");
   			output(ply_ptr->fd, obj_ptr->description);output(ply_ptr->fd, "\n");
			ANSI(fd, AM_BOLD);
			output(ply_ptr->fd, obj_ptr->use_output);output(ply_ptr->fd, "\n");
			ANSI(fd, AFC_WHITE);
			ANSI(fd, AM_NORMAL);
   		}
   		else {
   			output(ply_ptr->fd, "You may not get that. You have already fulfilled that objective.\n");
		
   		}	
		return(0);
   	}
   	else {
   		
   		set_objective(ply_ptr, obj_ptr->sets_flag[15]);
   		//sprintf(g_buffer, "%i", objective_exp(obj_ptr->sets_flag[15])); output(fd, g_buffer);
   		
   		if (F_ISSET(obj_ptr, ONOTGIVEN)){
   			//the monster doesn't actually give the item, they're just
   			//awarding the objective and that's the end of it.
   			ANSI(fd, AFC_BLUE);
   			output(ply_ptr->fd, obj_ptr->description);output(ply_ptr->fd, "\n");
			ANSI(fd, AM_BOLD);
			output(ply_ptr->fd, obj_ptr->use_output);output(ply_ptr->fd, "\n");
			ANSI(fd, AFC_WHITE);
			ANSI(fd, AM_NORMAL);
   			//The player gets to see a hint, stored in the item's description
   			return(2);
   		}
   		else{
   			return(1);
   		}
   	}
   	return(0);
}

//custom rejection message for objective selective exits
char *objective_exit_message(exit_ *xt){
	char str[81] = {};
	
	if (F_ISSET(xt, XOBJECTIVEGO) || F_ISSET(xt, XMULTIOBJGO)){
		char *s_ptr = str;
		strcpy(str, xt->rand_mesg[1]);
		strcat(str, "\0");
		return(s_ptr);
	}
	else {
		return("Unseen forces prevent your movement in that direction.");
	}
}

int multi_objective_exit(exit_ *xt){
	char str[81];
	char objstr[5];
	strcpy(str, xt->rand_mesg[0]);

	//the string will be structured as follows:
	//xxxxyyyyzzzzwwww
	//eg 0100019001200124
	//100, 190, 120, 124. Being the lower, upper bounds of the multi-objective
	// and lastly, the individual objective being referenced form that superset
	// the last one is the target value to be above or below


	if (F_ISSET(xt, XMULTIOBJSEE) || F_ISSET(xt, XMULTIOBJGO)){
		sprintf(objstr, "%c%c%c%c\0", str[8],str[9],str[10],str[11]);
		return(atoi(objstr));
	}
	else {
		return(0);
	}	
}

int multi_objective_exit_lower(exit_ *xt){
	char str[81];
	char objstr[5];
	strcpy(str, xt->rand_mesg[0]);

	//the string will be structured as follows:
	//xxxxyyyyzzzzwwww
	//eg 0100019001200124
	//100, 190, 120, 124. Being the lower, upper bounds of the multi-objective
	// and lastly, the individual objective being referenced form that superset
	// the last one is the target value to be above or below


	if (F_ISSET(xt, XMULTIOBJSEE) || F_ISSET(xt, XMULTIOBJGO)){
		sprintf(objstr, "%c%c%c%c\0", str[0],str[1],str[2],str[3]);
		return(atoi(objstr));
	}
	else {
		return(0);
	}	
}

int multi_objective_exit_upper(exit_ *xt){
	char str[81];
	char objstr[5];
	strcpy(str, xt->rand_mesg[0]);

	//the string will be structured as follows:
	//xxxxyyyyzzzzwwww
	//eg 0100019001200124
	//100, 190, 120, 124. Being the lower, upper bounds of the multi-objective
	// and lastly, the individual objective being referenced form that superset
	// the last one is the target value to be above or below


	if (F_ISSET(xt, XMULTIOBJSEE) || F_ISSET(xt, XMULTIOBJGO)){
		sprintf(objstr, "%c%c%c%c\0", str[4],str[5],str[6],str[7]);
		return(atoi(objstr));
	}
	else {
		return(0);
	}	
}


int multi_objective_exit_target(exit_ *xt){
	char str[81];
	char objstr[5];
	strcpy(str, xt->rand_mesg[0]);

	//the string will be structured as follows:
	//xxxxyyyyzzzzwwww
	//eg 0100019001200124
	//100, 190, 120, 124. Being the lower, upper bounds of the multi-objective
	// and lastly, the individual objective being referenced form that superset
	// the last one is the target value to be above or below


	if (F_ISSET(xt, XMULTIOBJSEE) || F_ISSET(xt, XMULTIOBJGO)){
		sprintf(objstr, "%c%c%c%c\0", str[12],str[13],str[14],str[15]);
		//output(1, objstr);
		return(atoi(objstr));
	}
	else {
		return(0);
	}	
}

int objective_exit(exit_ *xt){
	char str[81];
	strcpy(str, xt->rand_mesg[0]);
	//elog(str);

	//ONLY ONLY EVER EVER DO THIS WHEN THE RANDOM MESSAGE ON THE
	//EXIT IS ACTUALLY A NUMBER
	//AND NOT ACTUALLY TEXT
	//FOR THE LOVE OF GOD
	//AND i'M SORRY FOR BEING A BAD CODER LOVE SMITHY
	if (F_ISSET(xt, XOBJECTIVESEE) || F_ISSET(xt, XOBJECTIVEGO)){
		return(atoi(str));
	}
	else {
		return(0);
	}
}

int multi_objective(creature *ply_ptr, int lowbound, int highbound, int number){
	int i, j, count;

	int fd = ply_ptr->fd;
	//set the objective for number. if they already 
	// have it set, then let them know
	if (objective_check(ply_ptr, number)){
		output(fd, "Sorry, you've already completed this obejctive");
		return(0);
	}

	//examine which objectives within the bounds have already been set
	count = 0;
	for (i = lowbound/10; i<highbound/10+1; i++){
		sprintf(g_buffer, "hi");
		output(fd, " ");
		if (objective_check(ply_ptr, i*10)){
			sprintf(g_buffer, "hi");
			output(fd, " ");
			count++;
		}
	}
	//set the base objective with a notification
	set_objective(ply_ptr, number);

	//now decide which of the objectives within the series to set
	if (count < 10){
		silent_objective(ply_ptr, number + 1 + count);
	}
	return(number + 1 + count);
}

int silent_objective(creature *ply_ptr, int objective){
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;
	
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

		//close the file
		fclose(fpt);
	}
 	
	
 	return(0);
}

int init_statup(creature *ply_ptr, cmd *cmnd){
	int fd;
	fd = ply_ptr->fd;

	ANSI(fd, AM_ITALIC);
	ANSI(fd, AFC_YELLOW);
	output(fd, "\n================================================================================");
	output(fd, "\n    ------------------------------STATUP-------------------------------");
	output(fd, "\n================================================================================");
	ANSI(fd, AM_NORMAL);
	ANSI(fd, AFC_WHITE);
	output(fd, "\n");
	output(fd, "\nMy current stats:");
	sprintf(g_buffer, "\nSTRength: %2d  DEXterity: %2d  CONstitution: %2d  INTelligence: %2d  PieTY: %2d", ply_ptr->strength, ply_ptr->dexterity, ply_ptr->constitution, ply_ptr->intelligence, ply_ptr->piety);
	output(fd, g_buffer);
	output(fd, "\n");
	sprintf(g_buffer, "\nUn-allocated stat points: %2d", check_stat_points(ply_ptr));
	output(fd, g_buffer);

	if (check_stat_points(ply_ptr)){
		
		F_SET(ply_ptr, PREADI);
		statup(fd,5,"");
		
	}
	else{
		output(fd, "\nIf you gain more stat points, you can allocated them using this command.\n");
		return(DOPROMPT);
	}
	return(DOPROMPT);

	
}
int get_cast_speed(creature *ply_ptr){
	int tier, speed;
	tier = check_attribute_tier(ply_ptr, 8);
	switch (tier){
		case 0:
			speed = 3;
		break;
			
		case 1:
			speed = 4;
		break;

		case 2:
			speed = 4;
		break;

		case 3:
			speed = 4;
		break;

		case 4:
			speed = 5;
		break;

		case 5:
			speed = 5;
		break;

		default:
			speed = 5;
		break;
	}
	if (F_ISSET(ply_ptr, PATTUNE)){
		speed = 2;
	}
	return(speed);
}
int get_base_AC(creature *ply_ptr){
	int tier, ac;
	tier = check_attribute_tier(ply_ptr, 4);
	switch (tier){
		case 0:
			ac = 85;
		break;
			
		case 1:
			ac = 90;
		break;

		case 2:
			ac = 100;
		break;

		case 3:
			ac = 110;
		break;

		case 4:
			ac = 120;
		break;

		case 5:
			ac = 130;
		break;

		default:
			ac = 130;
		break;
	}
	ac = ac - 5*bonus[(int)ply_ptr->dexterity];
	ac = MIN(ac, 130);
	
	
	return(ac);
}
int get_MP_tick(creature *ply_ptr){
	int tier, tick;
	tier = check_attribute_tier(ply_ptr, 8);
	switch (tier){
		case 0:
			tick = 6;
		break;
			
		case 1:
			tick = 5;
		break;

		case 2:
			tick = 4;
		break;

		case 3:
			tick = 3;
		break;

		case 4:
			tick = 4;
		break;

		case 5:
			tick = 2;
		break;

		default:
			tick = 2;
		break;
	}
	if ((int)ply_ptr->intelligence > 17){
		tick++;
	}
	
	return(tick);
}

int get_HP_tick(creature *ply_ptr){
	int tier, tick;
	tier = check_attribute_tier(ply_ptr, 4);
	switch (tier){
		case 0:
			tick = 9;
		break;
			
		case 1:
			tick = 7;
		break;

		case 2:
			tick = 5;
		break;

		case 3:
			tick = 4;
		break;

		case 4:
			tick = 3;
		break;

		case 5:
			tick = 2;
		break;

		default:
			tick = 2;
		break;
	}
	tick = tick + (int)ply_ptr->constitution/2;
	tick = MAX(1,tick);
	//if (F_ISSET(ply_ptr->))
	return(tick);
}

char *get_tier_string(int tier){
	switch (tier){
		case 0:
			return("S");
		break;
			
		case 1:
			return("A");
		break;

		case 2:
			return("B");
		break;

		case 3:
			return("C");
		break;

		case 4:
			return("D");
		break;

		case 5:
			return("E");
		break;

		default:
			return("X");
		break;
	}
}

int check_stat_points(creature *ply_ptr){
	int ret =0;
	int c,i,j, result;
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;
	char array[250][10];
	// load the player's file and make one for them if they don't already have it
	strcpy(filename, "../player/");
	strcat(filename, ply_ptr->name);
	strcat(filename, ".skills\0");
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
	result = 0;
	while (result != EOF){
		result = fscanf(fpt, "%11[^,]", str);
		result = fscanf(fpt, "%*c");
		strcpy(array[c], str);
		c++;

	}
	fclose(fpt);
	//iterate through the array and ask if any of the values
	//match the skill we are currently checking
	j = atoi(array[1]);
	
	return(j);
}


int statup(int fd, int param, char *str){
	int num[5];
		int i, j, n, l, k;
		int sum;

	creature *ply_ptr;
	ply_ptr = Ply[fd].ply;
	output_ply_buf(fd);
	switch (param) {
	case 5:
		if (check_stat_points(ply_ptr)){
			ask_for(fd, "Would you like to spend your stat points? [Type y/n to choose]");
			output_ply_buf(fd);

			F_SET(ply_ptr, PREADI);
			RETURN(fd, statup, 0);			
			}
	break;
	case 0:
		if (!strcmp(str, "y")){
			statup(fd, 2, "");


		}
		else{
			statup(fd, 1, "");
		}
		//Ply[fd].io->intrpt &= ~1;
		//Ply[fd].io->fn = skilltree;
		//Ply[fd].io->fnparam = 1;
		
	break;
	case 1:
		output(fd, "Stopping there.\n");
		F_CLR(ply_ptr, PREADI);
		RETURN(fd, command, 1);
	break;
	case 2:
		output(fd, "\nChoose the stat points you would like to allocate:");
		output(fd, "\n");
		output(fd, "\nXX   XX   XX   XX   XX");			
		output(fd, "\nSTR  DEX  CON  INT  PTY");
		output(fd, "\n");
		output(fd, "\n");
		output(fd, "\nYou must enter all 5 numbers, with spaces in between.");
		output(fd, "\nYou will spend a number of stat points equal to the sum of values entered.");
		output(fd, "\nYou may enter '0' for any and all values");
		statup(fd, 3, "");
	break;
	case 3:
		//strcpy(str, "");
		ask_for(fd, "Enter your stat selections [or type 'q' to quit]: ");
		RETURN(fd, statup, 4);
	break;
	case 4:
		n = strlen(str); l = 0; k = 0;
		if (!strcmp(str, "q")){
			statup(fd, 1, "");
		}
		for(i=0; i<=n; i++) {
			if(str[i]==' ' || str[i]==0) {
				str[i] = 0;
				num[k++] = atoi(&str[l]);
				l = i+1;
			}
			if(k>4) break;
		}
		if(k<5) {
			
			output(fd, "Please enter all 5 numbers.\n");
			ask_for(fd, "Enter your stat selections [or type 'q' to quit]: ");
			RETURN(fd, statup, 4);
		}
		sum = 0;
		for(i=0; i<5; i++) {
			if(num[i] < 0 ) {
				output(fd, "Cannot subtract stats\n");
				ask_for(fd, "Enter your stat selections [or type 'q' to quit]: ");
				RETURN(fd, statup, 4);
			}
			sum += num[i];
		}
		if(sum > (check_stat_points(ply_ptr))) {
			output(fd, "Stat total may not exceed available points.\n");
			ask_for(fd, "Enter your stat selections [or type 'q' to quit]: ");
			RETURN(fd, statup, 4);
		}
		//stat input was fine
		Ply[fd].ply->strength += num[0];
		Ply[fd].ply->dexterity += num[1];
		Ply[fd].ply->constitution += num[2];
		Ply[fd].ply->intelligence += num[3];
		Ply[fd].ply->piety += num[4];
		spend_statpoints(ply_ptr, sum);
		output(fd, "Stats added.\n");
		F_CLR(ply_ptr, PREADI);
		RETURN(fd, command, 1);
	break;
	}
	return(0);
}

int spend_statpoints(creature *ply_ptr, int num){
	int ret =0;
	int c,i,j, result;
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;
	char array[250][10];
	// load the player's file and make one for them if they don't already have it
	strcpy(filename, "../player/");
	strcat(filename, ply_ptr->name);
	strcat(filename, ".skills\0");
	fpt = fopen(filename, "a+");
	fclose(fpt);
	fpt = fopen(filename, "r");
	
	result = 0;
	//BE WARY OF DECREASING STAT POINTS BTW
	//BUT WE WILL JUST CLUDGE FOR NOW OKAY
	//clean the array
	for (i=0; i<250; i++){
		strcpy(array[i], "");
	}
	//iterate through the file and load the values into 
	//a handy array for future reading
	c = 0;
	while (result != EOF){
		result = fscanf(fpt, "%11[^,]", str);
		result = fscanf(fpt, "%*c");
		strcpy(array[c], str);
		c++;

	}
	fclose(fpt);

	//decrease stat points by one;
	sprintf(str, "%i", atoi(array[1]) -num );
	strcpy(array[1], str);
	
	//rewrite the file with updated values
	//only write up to the value you are expecting!
	//this should be stored in the integer 'c'
	fpt = fopen(filename, "w+");
	for (i=0, j=0; i<c-1; i++){
		fprintf(fpt, "%s,", array[i] );
	}	
	fclose(fpt);


	return(0);	
}

int add_statpoints(creature *ply_ptr, int num){
	int ret =0;
	int c,i,j, result;
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;
	char array[250][10];
	// load the player's file and make one for them if they don't already have it
	strcpy(filename, "../player/");
	strcat(filename, ply_ptr->name);
	strcat(filename, ".skills\0");
	fpt = fopen(filename, "a+");
	fclose(fpt);
	fpt = fopen(filename, "r");
	
	result = 0;
	//BE WARY OF DECREASING STAT POINTS BTW
	//BUT WE WILL JUST CLUDGE FOR NOW OKAY
	//clean the array
	for (i=0; i<250; i++){
		strcpy(array[i], "");
	}
	//iterate through the file and load the values into 
	//a handy array for future reading
	c = 0;
	while (result != EOF){
		result = fscanf(fpt, "%11[^,]", str);
		result = fscanf(fpt, "%*c");
		strcpy(array[c], str);
		c++;

	}
	fclose(fpt);

	//increase stat points by indicated amount;
	sprintf(str, "%i", atoi(array[1]) +num );
	strcpy(array[1], str);
	
	//rewrite the file with updated values
	//only write up to the value you are expecting!
	//this should be stored in the integer 'c'
	fpt = fopen(filename, "w+");
	for (i=0, j=0; i<c-1; i++){
		fprintf(fpt, "%s,", array[i] );
	}	
	fclose(fpt);


	return(0);	
}

int add_skillpoints(creature *ply_ptr, int num){
	int ret =0;
	int c,i,j, result;
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;
	char array[250][10];
	// load the player's file and make one for them if they don't already have it
	strcpy(filename, "../player/");
	strcat(filename, ply_ptr->name);
	strcat(filename, ".skills\0");
	fpt = fopen(filename, "a+");
	fclose(fpt);
	fpt = fopen(filename, "r");
	
	result = 0;
	//BE WARY OF DECREASING STAT POINTS BTW
	//BUT WE WILL JUST CLUDGE FOR NOW OKAY
	//clean the array
	for (i=0; i<250; i++){
		strcpy(array[i], "");
	}
	//iterate through the file and load the values into 
	//a handy array for future reading
	c = 0;
	while (result != EOF){
		result = fscanf(fpt, "%11[^,]", str);
		result = fscanf(fpt, "%*c");
		strcpy(array[c], str);
		c++;

	}
	fclose(fpt);

	//increase stat points by indicated amount;
	sprintf(str, "%i", atoi(array[0]) +num );
	strcpy(array[0], str);
	
	//rewrite the file with updated values
	//only write up to the value you are expecting!
	//this should be stored in the integer 'c'
	fpt = fopen(filename, "w+");
	for (i=0, j=0; i<c-1; i++){
		fprintf(fpt, "%s,", array[i] );
	}	
	fclose(fpt);


	return(0);	
}

int init_skilltree(creature *ply_ptr, cmd *cmnd){
	//find the master skilltreess file in ../bin
	//read the skilltreess file
	//the structure of the file is pairs of lines
	//first lines give the catalogue info
	//and requirement info of the skill
	//second lines give the name to be displayed
	int fd;
	fd = ply_ptr->fd;

	F_SET(ply_ptr, PREADI);
	skilltree(fd,0,"");
	return(DOPROMPT);
}


int skilltree(int fd, int param, char *str){
	
	int total_selections = 0;
	int selections[25];
	//find the master skilltreess file in ../bin
	//read the skilltreess file
	//the structure of the file is pairs of lines
	//first lines give the catalogue info
	//and requirement info of the skill
	//second lines give the name to be displayed
	creature *ply_ptr;

	ply_ptr = Ply[fd].ply;

	char filename[80] = {};
	char strr[10] = {};
	FILE *fpt;
	char array[3208][30];
	
	int c,i,j,k, result;

	//get all the tree names first, from the first 8 rows
	//sprintf(g_buffer, "K = %i\n", k);
	//output(ply_ptr->fd, g_buffer);

	strcpy(filename, "../bin/skilltree.master\0");
	fpt = fopen(filename, "r");
	
	

	//clean the arrays
	for (i=0; i<3208; i++){
		strcpy(array[i], "");
	}
	
	//iterate through the file and load the values into 
	//a handy array for future reading
	c = 0;
	while (result != EOF){
		result = fscanf(fpt, "%29[^,\n]", strr);
		result = fscanf(fpt, "%*c");
		strcpy(array[c], strr);
		//sprintf(g_buffer, "Array[%i]: %s", c, str);
		//output(fd, g_buffer);
		//output(fd, "\n");
		//sprintf(g_buffer, "array %i: ", c);
		//output(fd, g_buffer);
		//output(fd, array[c]);
		//output(fd, "\n");

		c++;
	}
	fclose(fpt);

	//elog("calling skilltree now");
	//step  = Ply[fd].extr->tempstr[0];
	switch (param) {
	case 0:
		
		output(fd, "\n================================================================================");
		output(fd, "\n    ------------------------------SKILL TREES-------------------------------");
		output(fd, "\n================================================================================");
		output(fd, "\n");
		output(fd, "\nTree 1: "); output(fd, array[0]);
		output(fd, "\nTree 2: "); output(fd, array[1]);
		output(fd, "\nTree 3: "); output(fd, array[2]);
		output(fd, "\nTree 4: "); output(fd, array[3]);
		output(fd, "\nTree 5: "); output(fd, array[4]);
		output(fd, "\n");
		
		ask_for(fd, "Which tree would you like to view? [Type your selection, 1-5 , or 'q' to quit]");
		output_ply_buf(fd);
		Ply[fd].io->intrpt &= ~1;
		Ply[fd].io->fn = skilltree;
		Ply[fd].io->fnparam = 1;
		RETURN(fd, skilltree, 1);
		//return(DOPROMPT);
		//room for 3 more trees in the master list file
	break;
	case 1:
		if (!strcmp(str, "1") || !strcmp(str, "2") || !strcmp(str, "3") ||!strcmp(str, "4") || !strcmp(str, "5")){
			
			k = atoi(str);
			for (i=k-1; i<k; i++){
				output(ply_ptr->fd, "\n================================================================================");
				output(fd, "\n");
				int lines_number;
				lines_number = (75 - strlen(array[i]))/2;
				for (j =0; j < lines_number; j++){

					output(fd, "-");
				}
				sprintf(g_buffer, "%s TREE", array[i]);
				output(ply_ptr->fd, g_buffer);
				for (j =0; j < lines_number; j++){

					output(fd, "-");
				}
						
				output(ply_ptr->fd, "\n================================================================================\n\n");
				//output(ply_ptr->fd, "\n12345678901234567890123456789012345678901234567890123456789012345678901234567890\n");

				//now loop through your array and extract the relevant skills
				int line =0;
				int lineline=0; //to keep track of the "actual line"
				int column =0;
				int length =0;
				int previous_word_end =0;
				int previous_output_column = 0;
				for (line=0; line<21; line++){
					if (line % 4 == 0 || line ==0){
						lineline ++;
					}
					previous_word_end =0;
					previous_output_column = 0;

					int previous_above_column = 0;
					int previous_below_column = 0;
					for (column =0; column < 10; column++){
						if (line % 4 == 0 || line ==0){
							
							//read the array and determine if it belongs
							//on this line and column
							for (j=0; j<50; j++){
								//output(fd, array[8+7*j+0]);
								//output(fd, array[8+7*j+1]);
								//output(fd, array[8+7*j+2]);
								//output(fd, array[8+7*j+3]);
								//output(fd, array[8+7*j+4]);
								//output(fd, array[8+7*j+5]);
								//output(fd, array[8+7*j+6]);
								//sprintf(g_buffer, "TLC: %i%i%i. Array [%i]: %s%s%s %s", i+1, lineline, column, j, array[8+7*j+0],array[8+7*j+1],array[8+7*j+2],array[8+7*j+6]);
								//output(fd, g_buffer);
								if (atoi(array[8+j*32+2]) == i+1){
									//is it on the line we need?
									if (atoi(array[8+j*32+3]) == lineline){
										//is it in the current column?
										if (atoi(array[8+j*32+4]) == column){
											//calc the start point of the word
											int wordstart;
											int len =0;
											int halflen = 0;
											char tempstr[15];

											strcpy(tempstr, array[8+j*32+0]);
											strcat(tempstr, "\0");

											len = strlen(tempstr);
											halflen = len/2 -1;
											wordstart = 5+ 7*atoi(array[8+j*32+4])- halflen -3;
											
											//now let's check if the skill is one we want to output in green
											
											int skillnumber = atoi(array[8+j*32+1]);
											
											//loop from previous end position to start position

											int m, n;
											for (m=previous_word_end; m<wordstart; m++){
												output(fd, " ");

											}
											//now output the word
											output(fd, "[");
											if (skill_check(ply_ptr, skillnumber)){
												ANSI(fd, AM_ITALIC);
												ANSI(fd, AFC_GREEN);
												sprintf(g_buffer, "%i", get_skill_level(ply_ptr, skillnumber));
												output(fd, g_buffer);
												ANSI(fd, AM_NORMAL);
												ANSI(fd, AFC_WHITE);
											}
											else if (!skill_check(ply_ptr, skillnumber) && can_learn_skill(ply_ptr, skillnumber)){
												ANSI(fd, AM_ITALIC);
												ANSI(fd, AFC_CYAN);
												output(fd, "0");
												ANSI(fd, AM_NORMAL);
												ANSI(fd, AFC_WHITE);
											}
											else{
												output(fd, " ");
											}
											
											output(fd, "|");

											//output(fd, array[8+j*7+6]);
											if (skill_check(ply_ptr, skillnumber)){
												ANSI(fd, AM_BOLD);
												ANSI(fd, AFC_GREEN);
												output(fd, array[8+j*32+0]);
												ANSI(fd, AM_NORMAL);
												ANSI(fd, AFC_WHITE);
											}
											else if (!skill_check(ply_ptr, skillnumber) && can_learn_skill(ply_ptr, skillnumber)){
												ANSI(fd, AM_ITALIC);
												ANSI(fd, AFC_CYAN);
												output(fd, array[8+j*32+0]);
												ANSI(fd, AM_NORMAL);
												ANSI(fd, AFC_WHITE);
											}
											else{
												output(fd, array[8+j*32+0]);
											}

											output(fd, "]");
											previous_word_end = wordstart + 4 + len;

										}
									}
								}
							}
						}
						//if line % 4 == 0
						else if (line % 4 == 1 && line < 20){
							//output(fd, "hi");
							//this is one under a row
							for (j=0; j<100; j++){
								//PAY ATTENTION WE ARE LOOKING AT
								//THE REQUIRED SKILLS IE: ARRAY 6,7,8
								//BE SURE TO ONLY OUTPUT THEM ONCE
								//check tree
								if (atoi(array[8+j*32+6]) == i+1){
									//is it on the line we need?
									if (atoi(array[8+j*32+7]) == lineline){
										//is it in the current column?
										if (atoi(array[8+j*32+8]) == column){
											//calc the position of the line
											
											//only output the line if it's not at the previous
											//line location
											if (previous_output_column != 5+7*column){
												int m;
											//loop from previous end position to start position
												for (m=previous_output_column; m<5+7*column; m++){
													output(fd, " ");

												}
												//now output the line
												output(fd, "|");
																	
												previous_output_column = 5+7*column;
											}

										}
									}
								}
							}
						}
						else if (line % 4 == 2 && line < 20){
							//this is two under a row
							//its where connections need to bend
							for (j=0; j<100; j++){
								//PAY ATTENTION WE ARE LOOKING AT
								//THE REQUIRED SKILLS IE: ARRAY 3,4,5
								//BE SURE TO ONLY OUTPUT THEM ONCE
								//check tree
								if (atoi(array[8+j*32+6]) == i+1){
									//is it on the line we need?
									if (atoi(array[8+j*32+7]) == lineline){
										//is it in the current column?
										if (atoi(array[8+j*32+8]) == column || atoi(array[8+j*32+4]) == column ){
											//calc the position of the line
											
											//only output the line if it's not at the previous
											//line location
											if(previous_above_column == column || previous_below_column == column){
												if (previous_output_column != 5+7*column){
													int m;
													int target;
													if (previous_above_column == column){
														target =5+7*column-1;
													}
													else if (previous_below_column == column){
														target = 5+7*column-0;
													}
													for (m=previous_output_column; m<target; m++){
														output(fd, "-");
													}		
												}
											}
											else{
												if (previous_output_column != 5+7*column){
													int m;
													for (m=previous_output_column; m<5+7*column; m++){
														output(fd, " ");
													}
												}
											}

											if (previous_output_column != 5+7*column){
																				
												if((array[8+j*32+4] == column && array[8+j*32+8] == column) /*&& (previous_below_column != column && previous_above_column != column)*/){
													//now output the line
													output(fd, "|");
												}
												else{
													output(fd, "+");
												}

												previous_output_column = 5+7*column;
												
											}
											previous_below_column = atoi(array[8+j*32+4]);
											previous_above_column = atoi(array[8+j*32+8]);

										}
									}
								}
							}
						}
						else if (line % 4 == 3 && line < 20){
							//this is one before a row
							for (j=0; j<100; j++){
								
								//check tree
								if (atoi(array[8+j*32+2]) == i+1){
									//is it on the line we need?
									//PLUS ONE LINE PLEASE
									if (atoi(array[8+j*32+3]) == lineline+1){
										//is it in the current column?
										if (atoi(array[8+j*32+4]) == column){
											//calc the position of the line
											
											//only output the line if it's not at the previous
											//line location
											if (previous_output_column != 5+7*column){
												int m;
											//loop from previous end position to start position
												for (m=previous_output_column; m<5+7*column; m++){
													output(fd, " ");

												}
												//now output the line
												output(fd, "|");
																	
												previous_output_column = 5+7*column;
											}

										}
									}
								}
							}
						}						
						
						
					}
					output(fd, "\n");
				}
			}
			//check if the player has unspent skill points
			result = check_skill_points(ply_ptr);
			if (result){
				sprintf(g_buffer, "You currently have %i skill points.", result);
				output(fd, g_buffer);
				
				ask_for(fd, "Would you like to spend them now? [y/n]\n");
				if (k==1){
					RETURN(fd, skilltree, 2);
				}
				if (k==2){
					RETURN(fd, skilltree, 3);
				}
				if (k==3){
					RETURN(fd, skilltree, 4);
				}
				if (k==4){
					RETURN(fd, skilltree, 5);
				}
				if (k==5){
					RETURN(fd, skilltree, 6);
				}
				
				
			}
			else{
				output(fd, "You currently have 0 skill points. When you gain more points, they will appear here to be spent");
				F_CLR(ply_ptr, PREADI);
				RETURN(fd, command, 1);
			}

				//ask if they want to spend them
			
		}
		else{
			output(fd, "Stopping there");
			F_CLR(ply_ptr, PREADI);
			RETURN(fd, command, 1);
		}
		

	break;

	case 2:
		if (!strcmp(str, "y")){
			k = 1;
			goto skillup;
		}
		else{
			output(fd, "Stopping there");
			F_CLR(ply_ptr, PREADI);
			RETURN(fd, command, 1);
		}
	break;
	case 3:
		if (!strcmp(str, "y")){
			k = 2;
			goto skillup;
		}
		else{
			output(fd, "Stopping there");
			F_CLR(ply_ptr, PREADI);
			RETURN(fd, command, 1);
		}
	break;
	case 4:
		if (!strcmp(str, "y")){
			k = 3;
			goto skillup;
		}
		else{
			output(fd, "Stopping there");
			F_CLR(ply_ptr, PREADI);
			RETURN(fd, command, 1);
		}
	break;
	case 5:
		if (!strcmp(str, "y")){
			k = 4;
			goto skillup;
		}
		else{
			output(fd, "Stopping there");
			F_CLR(ply_ptr, PREADI);
			RETURN(fd, command, 1);
		}
	break;
	case 6:
		if (!strcmp(str, "y")){
			k = 5;
			goto skillup;
		}
		else{
			output(fd, "Stopping there");
			F_CLR(ply_ptr, PREADI);
			RETURN(fd, command, 1);
		}
	break;
	case 7:
		//this is just to guard the label
		skillup:
		for (i=k-1; i<k; i++){
			output(ply_ptr->fd, "\n================================================================================");
			output(fd, "\n");
			int lines_number;
			lines_number = (75 - strlen(array[i]))/2;
			for (j =0; j < lines_number; j++){

				output(fd, "-");
			}
			sprintf(g_buffer, "%s TREE", array[i]);
			output(ply_ptr->fd, g_buffer);
			for (j =0; j < lines_number; j++){

				output(fd, "-");
			}
					
			output(ply_ptr->fd, "\n================================================================================\n\n");
			//output(ply_ptr->fd, "\n12345678901234567890123456789012345678901234567890123456789012345678901234567890\n");

			//now loop through your array and extract the relevant skills
			int line =0;
			int lineline=0; //to keep track of the "actual line"
			int column =0;
			int length =0;
			int previous_word_end =0;
			int previous_output_column = 0;

			int total_selections = 0;
			int selections[25];
			int n;
			for (n=0; n<25; n++){
				selections[n] = 0;
			}

			for (line=0; line<21; line++){
				if (line % 4 == 0 || line ==0){
					lineline ++;
				}
				previous_word_end =0;
				previous_output_column = 0;

				int previous_above_column = 0;
				int previous_below_column = 0;
				for (column =0; column < 10; column++){
					if (line % 4 == 0 || line ==0){
						
						//read the array and determine if it belongs
						//on this line and column
						for (j=0; j<50; j++){
							//output(fd, array[8+7*j+0]);
							//output(fd, array[8+7*j+1]);
							//output(fd, array[8+7*j+2]);
							//output(fd, array[8+7*j+3]);
							//output(fd, array[8+7*j+4]);
							//output(fd, array[8+7*j+5]);
							//output(fd, array[8+7*j+6]);
							//sprintf(g_buffer, "TLC: %i%i%i. Array [%i]: %s%s%s %s", i+1, lineline, column, j, array[8+7*j+0],array[8+7*j+1],array[8+7*j+2],array[8+7*j+6]);
							//output(fd, g_buffer);
							if (atoi(array[8+j*32+2]) == i+1){
								//is it on the line we need?
								if (atoi(array[8+j*32+3]) == lineline){
									//is it in the current column?
									if (atoi(array[8+j*32+4]) == column){
										//calc the start point of the word
										int wordstart;
										int len =0;
										int halflen = 0;
										char tempstr[15];

										strcpy(tempstr, array[8+j*32+0]);
										strcat(tempstr, "\0");

										len = strlen(tempstr);
										halflen = len/2 -1;
										wordstart = 5+ 7*atoi(array[8+j*32+4])- halflen -3;
										
										//now let's check if the skill is one we want to output in green
										
										int skillnumber = atoi(array[8+j*32+1]);
										
										//loop from previous end position to start position

										int m, n;
										for (m=previous_word_end; m<wordstart; m++){
											output(fd, " ");

										}
										//calculate the letter for options

										//now output the word
										output(fd, "[");
										if (skill_check(ply_ptr, skillnumber)){
											//put the selection into the options
											//only if the skill is less than max level (level 9)
											
											ANSI(fd, AM_ITALIC);
											ANSI(fd, AFC_GREEN);

											if (get_skill_level(ply_ptr, skillnumber) < 9){
												selections[total_selections] = skillnumber;
												total_selections ++;

												
												sprintf(g_buffer, "%c", total_selections +96);
												output(fd, g_buffer);
												
											}
											else{

												sprintf(g_buffer, "*");
												output(fd, g_buffer);
												
											}

											ANSI(fd, AM_NORMAL);
											ANSI(fd, AFC_WHITE);
											
										}
										else if (!skill_check(ply_ptr, skillnumber) && can_learn_skill(ply_ptr, skillnumber)){
											//put the selection into the options
											selections[total_selections] = skillnumber;
											total_selections ++;

											ANSI(fd, AM_ITALIC);
											ANSI(fd, AFC_CYAN);
											sprintf(g_buffer, "%c", total_selections +96);
											output(fd, g_buffer);
											ANSI(fd, AM_NORMAL);
											ANSI(fd, AFC_WHITE);
										}
										else{
											output(fd, " ");
										}
										
										output(fd, "|");

										//output(fd, array[8+j*7+6]);
										if (skill_check(ply_ptr, skillnumber)){
											ANSI(fd, AM_BOLD);
											ANSI(fd, AFC_GREEN);
											output(fd, array[8+j*32+0]);
											ANSI(fd, AM_NORMAL);
											ANSI(fd, AFC_WHITE);
										}
										else if (!skill_check(ply_ptr, skillnumber) && can_learn_skill(ply_ptr, skillnumber)){
											ANSI(fd, AM_ITALIC);
											ANSI(fd, AFC_CYAN);
											output(fd, array[8+j*32+0]);
											ANSI(fd, AM_NORMAL);
											ANSI(fd, AFC_WHITE);
										}
										else{
											output(fd, array[8+j*32+0]);
										}

										output(fd, "]");
										previous_word_end = wordstart + 4 + len;

									}
								}
							}
						}
					}
					//if line % 4 == 0
					else if (line % 4 == 1 && line < 20){
						//output(fd, "hi");
						//this is one under a row
						for (j=0; j<100; j++){
							//PAY ATTENTION WE ARE LOOKING AT
							//THE REQUIRED SKILLS IE: ARRAY 6,7,8
							//BE SURE TO ONLY OUTPUT THEM ONCE
							//check tree
							if (atoi(array[8+j*32+6]) == i+1){
								//is it on the line we need?
								if (atoi(array[8+j*32+7]) == lineline){
									//is it in the current column?
									if (atoi(array[8+j*32+8]) == column){
										//calc the position of the line
										
										//only output the line if it's not at the previous
										//line location
										if (previous_output_column != 5+7*column){
											int m;
										//loop from previous end position to start position
											for (m=previous_output_column; m<5+7*column; m++){
												output(fd, " ");

											}
											//now output the line
											output(fd, "|");
																
											previous_output_column = 5+7*column;
										}

									}
								}
							}
						}
					}
					else if (line % 4 == 2 && line < 20){
						//this is two under a row
						//its where connections need to bend
						for (j=0; j<100; j++){
							//PAY ATTENTION WE ARE LOOKING AT
							//THE REQUIRED SKILLS IE: ARRAY 3,4,5
							//BE SURE TO ONLY OUTPUT THEM ONCE
							//check tree
							if (atoi(array[8+j*32+6]) == i+1){
								//is it on the line we need?
								if (atoi(array[8+j*32+7]) == lineline){
									//is it in the current column?
									if (atoi(array[8+j*32+8]) == column || atoi(array[8+j*32+4]) == column ){
										//calc the position of the line
										
										//only output the line if it's not at the previous
										//line location
										if(previous_above_column == column || previous_below_column == column){
											if (previous_output_column != 5+7*column){
												int m;
												int target;
												if (previous_above_column == column){
													target =5+7*column-1;
												}
												else if (previous_below_column == column){
													target = 5+7*column-0;
												}
												for (m=previous_output_column; m<target; m++){
													output(fd, "-");
												}		
											}
										}
										else{
											if (previous_output_column != 5+7*column){
												int m;
												for (m=previous_output_column; m<5+7*column; m++){
													output(fd, " ");
												}
											}
										}

										if (previous_output_column != 5+7*column){
																			
											if((array[8+j*32+4] == column && array[8+j*32+8] == column) /*&& (previous_below_column != column && previous_above_column != column)*/){
												//now output the line
												output(fd, "|");
											}
											else{
												output(fd, "+");
											}

											previous_output_column = 5+7*column;
											
										}
										previous_below_column = atoi(array[8+j*32+4]);
										previous_above_column = atoi(array[8+j*32+8]);

									}
								}
							}
						}
					}
					else if (line % 4 == 3 && line < 20){
						//this is one before a row
						for (j=0; j<100; j++){
							
							//check tree
							if (atoi(array[8+j*32+2]) == i+1){
								//is it on the line we need?
								//PLUS ONE LINE PLEASE
								if (atoi(array[8+j*32+3]) == lineline+1){
									//is it in the current column?
									if (atoi(array[8+j*32+4]) == column){
										//calc the position of the line
										
										//only output the line if it's not at the previous
										//line location
										if (previous_output_column != 5+7*column){
											int m;
										//loop from previous end position to start position
											for (m=previous_output_column; m<5+7*column; m++){
												output(fd, " ");

											}
											//now output the line
											output(fd, "|");
																
											previous_output_column = 5+7*column;
										}

									}
								}
							}
						}
					}						
					
					
				}
				output(fd, "\n");
			}
		}

		output(fd, "\nSkills available for selection are given a letter");
		ask_for(fd, "Type the letter corresponding to your choice (or 'qq' to quit): ");

		str = Ply[fd].extr->tempstr[0];
		//goto skillincrease;
		if (k==1){
			RETURN(fd, skilltree, 8);
		}
		if (k==2){
			RETURN(fd, skilltree, 9);
		}
		if (k==3){
			RETURN(fd, skilltree, 10);
		}
		if (k==4){
			RETURN(fd, skilltree, 11);
		}
		if (k==5){
			RETURN(fd, skilltree, 12);
		}
	break;

	case 8:
		k =1; goto skillincrease;
	break;

	case 9:
		k =2; goto skillincrease;
	break;

	case 10:
		k =3; goto skillincrease;
	break;

	case 11:
		k =4; goto skillincrease;
	break;

	case 12:
		k =5; goto skillincrease;
	break;

	case 13:
		//this is just to guard the label
		skillincrease:
		
		//figure out what choices the player had
		//i'm such a good coder :/ OH WELL
		strcpy(filename, "../bin/skilltree.master\0");
		
		fpt = fopen(filename, "r");
		
		//try to reset to the start of the file
		//clean the arrays
		for (i=0; i<3208; i++){
			strcpy(array[i], "");
		}
		

		
		//iterate through the file and load the values into 
		//a handy array for future reading
		c = 0;
		result = 0;
		while (result != EOF){
			result = fscanf(fpt, "%29[^,\n]", strr);
			result = fscanf(fpt, "%*c");
			strcpy(array[c], strr);
			//sprintf(g_buffer, "Array[%i]: %s", c, strr);
			//output(fd, g_buffer);
			//output(fd, "\n");
			//sprintf(g_buffer, "array %i: ", c);
			//output(fd, g_buffer);
			//output(fd, array[c]);
			//output(fd, "\n");
			//sprintf(g_buffer, "array[%i]: %s", c, array[c]);
			//elog(g_buffer);

			c++;
		}
		fclose(fpt);


		total_selections =0;
		int n;
		for (n=0; n<25; n++){
			selections[n] = 0;
			//output(fd, "hi");
		}
		//int m;
		for (n = 1; n < 7; n++){
			//sprintf(g_buffer, "%i", n);
			//elog(g_buffer);
			for (j = 0; j< 10; j++){
				for (i = 0; i<100; i++){
					//sprintf(g_buffer, "array[%i]: %s", i, array[8+32*i]);
					//elog(g_buffer);
					if (atoi(array[8+i*32+2]) == k){
						//elog("updating skill. Tree check passed");
						if (atoi(array[8+i*32+3]) == n){
							//elog("line check passed");
							if (atoi(array[8+i*32+4]) == j){
								//elog("column check passed");
								int skillnumber = atoi(array[8+i*32+1]);
								if (skill_check(ply_ptr, skillnumber)){
									//elog("skill check passed");
									//put the selection into the options
									//only if the skill is less than max level (level 9)
									
									if (get_skill_level(ply_ptr, skillnumber) < 9){
										//elog("skil level passed");
										selections[total_selections] = skillnumber;
										total_selections ++;
										
									}

								}
								else if (!skill_check(ply_ptr, skillnumber) && can_learn_skill(ply_ptr, skillnumber)){
									//put the selection into the options
									selections[total_selections] = skillnumber;
									total_selections ++;

								}
							}
						}
					}
				}
			}
		}

		char choice;
		int skill_choice;
		
		//did the player make a selection?
		if (str){
			choice = str[0];
		}
		else {
			choice = 0;
		}
		skill_choice = choice -97;
		if (skill_choice < 0){
			skill_choice =0;
		}
		//output(fd, str);
		//sprintf(g_buffer, "%c / %i, skill number = %i\n", choice, skill_choice, selections[skill_choice]);
		//output(fd, g_buffer);
		
		
		if (selections[skill_choice]){
			//if they did, upskill their choice
			up_skill(ply_ptr, selections[skill_choice]);
			output(fd, "Skills updated\n");
			F_CLR(ply_ptr, PREADI);
			RETURN(fd, command, 1);
		}
		else{
			output(fd, "Stopping there\n");
			F_CLR(ply_ptr, PREADI);
			RETURN(fd, command, 1);
		}
	break;
	}
	
		
}

int up_skill(creature *ply_ptr, int skill){
	int ret =0;
	int c,i,j, result;
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;
	char array[250][10];
	// load the player's file and make one for them if they don't already have it
	strcpy(filename, "../player/");
	strcat(filename, ply_ptr->name);
	strcat(filename, ".skills\0");
	fpt = fopen(filename, "a+");
	fclose(fpt);
	fpt = fopen(filename, "r");
	
	//BE WARY OF DECREASING SKILL POINTS BTW
	//BUT WE WILL JUST CLUDGE FOR NOW OKAY
	//clean the array
	for (i=0; i<250; i++){
		strcpy(array[i], "");
	}
	//iterate through the file and load the values into 
	//a handy array for future reading
	c = 0;
	while (result != EOF){
		result = fscanf(fpt, "%11[^,]", str);
		result = fscanf(fpt, "%*c");
		strcpy(array[c], str);
		c++;

	}
	fclose(fpt);

	//decrease skill points by one;
	sprintf(str, "%i", atoi(array[0]) -1 );
	strcpy(array[0], str);
	//iterate through the array and ask if any of the values
	//match the skill we are currently checking
	//check if the player already has the skill
	
	if (skill_check(ply_ptr, skill)){
		for(i=0, j=0; i<250; i++){
			if (array[12+2*i]){
				//sprintf(g_buffer, "array[%i]: %i. %i\n", i, atoi(array[2+2*i]), atoi(array[2+2*i+1]));
				//output(ply_ptr->fd, g_buffer);
				if (atoi(array[12+2*i]) == skill){
					sprintf(str, "%i", atoi(array[12+2*i+1])+1);
					strcpy(array[12+2*i+1], str);
				}
			}
			//sprintf(g_buffer, "array[%i]: %i. %i\n", i, atoi(array[2+2*i]), atoi(array[2+2*i+1]));
			//output(ply_ptr->fd, g_buffer);
		}
	}
	//rewrite the file with updated values
	//only write up to the value you are expecting!
	//this should be stored in the integer 'c'
	fpt = fopen(filename, "w+");
	for (i=0, j=0; i<c-1; i++){
		fprintf(fpt, "%s,", array[i] );
	}	
	fclose(fpt);

	//if not, give it to them
	if (!skill_check(ply_ptr, skill)){
		set_skill(ply_ptr, skill);

	}

	return(0);
		
}

int can_learn_skill(creature *ply_ptr, int skill){
	int fd;
	fd = ply_ptr->fd;
	//FIRST 8 LINES ARE THE TREE NAMES
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;

	strcpy(filename, "../bin/skilltree.master\0");
	fpt = fopen(filename, "r");
	char array[3208][30];
	
	int c,i,j,k, result;

	

	//clean the arrays
	for (i=0; i<3208; i++){
		strcpy(array[i], "");
	}
	
	//iterate through the file and load the values into 
	//a handy array for future reading
	c = 0;
	result = 0;
	while (result != EOF){
		result = fscanf(fpt, "%29[^,\n]", str);
		result = fscanf(fpt, "%*c");
		strcpy(array[c], str);
		//sprintf(g_buffer, "Array[%i]: %s", c, str);
		//output(fd, g_buffer);
		//output(fd, "\n");
		//sprintf(g_buffer, "array %i: ", c);
		//output(fd, g_buffer);
		//output(fd, array[c]);
		//output(fd, "\n");

		c++;
	}
	fclose(fpt);

	int skill_number;

	for (i=0; i< 100; i++){
		skill_number = atoi(array[8+32*i+1]);
		if (skill_number == skill){
			
			int req_skill_number = atoi(array[8+32*i+5]);
			if (skill_check(ply_ptr, req_skill_number) || req_skill_number == 0){
				return(1);
			}
			else{
				return(0);
			}
		}
	}

	return(0);
}

/*SKILL CHECK
this function checks if a player is allowed to use
an indicated skill
by looking up their special player.skills file
function returns 1 if player may use skill*/

int skill_check(creature *ply_ptr, int skill){
	int ret =0;
	int c,i,j, result;
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;
	char array[250][10];
	// load the player's file and make one for them if they don't already have it
	strcpy(filename, "../player/");
	strcat(filename, ply_ptr->name);
	strcat(filename, ".skills\0");
	fpt = fopen(filename, "a+");
	fclose(fpt);
	fpt = fopen(filename, "r");
	
	result =0;
	//clean the array
	for (i=0; i<250; i++){
		strcpy(array[i], "");
	}
	//iterate through the file and load the values into 
	//a handy array for future reading
	c = 0;
	while (result != EOF){
		result = fscanf(fpt, "%11[^,]", str);
		result = fscanf(fpt, "%*c");
		strcpy(array[c], str);
		c++;

	}
	//iterate through the array and ask if any of the values
	//match the skill we are currently checking
	for(i=0, j=0; i<250; i++){
		if (array[12+2*i]){
			//sprintf(g_buffer, "array[%i]: %i. skill: %i\n", i, atoi(array[i]), skill);
			//output(ply_ptr->fd, g_buffer);
			if (atoi(array[12+2*i]) == skill && get_skill_level(ply_ptr, skill) > 0){
				j = 1;
			}
		}
	}
	fclose(fpt);
	return(j);
}
/* GET SKILL LEVEL
reads the player's file and returns how many skill points
they have at the moment
*/
extern int check_skill_points(creature *ply_ptr){
	int ret =0;
	int c,i,j, result;
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;
	char array[250][10];
	// load the player's file and make one for them if they don't already have it
	strcpy(filename, "../player/");
	strcat(filename, ply_ptr->name);
	strcat(filename, ".skills\0");
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
	result = 0;
	while (result != EOF){
		result = fscanf(fpt, "%11[^,]", str);
		result = fscanf(fpt, "%*c");
		strcpy(array[c], str);
		c++;

	}
	fclose(fpt);
	//iterate through the array and ask if any of the values
	//match the skill we are currently checking
	j = atoi(array[0]);
	
	return(j);
}
/* GET SKILL LEVEL
reads the player's file and returns the skill level value
of the skill enquired
*/
int get_skill_level(creature *ply_ptr, int skill){
	int ret =0;
	int c,i,j, result;
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;
	char array[250][10];
	// load the player's file and make one for them if they don't already have it
	strcpy(filename, "../player/");
	strcat(filename, ply_ptr->name);
	strcat(filename, ".skills\0");
	fpt = fopen(filename, "a+");
	fclose(fpt);
	fpt = fopen(filename, "r");
	
	result = 0;
	//clean the array
	for (i=0; i<250; i++){
		strcpy(array[i], "");
	}
	//iterate through the file and load the values into 
	//a handy array for future reading
	c = 0;
	while (result != EOF){
		result = fscanf(fpt, "%11[^,]", str);
		result = fscanf(fpt, "%*c");
		strcpy(array[c], str);
		c++;

	}
	fclose(fpt);
	//iterate through the array and ask if any of the values
	//match the skill we are currently checking
	for(i=0, j=0; i<250; i++){
		if (array[12+2*i]){
			sprintf(g_buffer, "array[%i]: %i. %i\n", i, atoi(array[12+2*i]), atoi(array[12+2*i+1]));
			//output(ply_ptr->fd, g_buffer);
			if (atoi(array[12+2*i]) == skill){
				return(atoi(array[12+2*i+1]));
			}
		}
	}
	
	return(j);
} 

int set_skill(creature *ply_ptr, int skill){
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;
	
	//firstly check if they have done it already
	//if they haven't, proceed
	if (!skill_check(ply_ptr, skill)){
		//find the player's file and open it
	
		strcpy(filename, "../player/");
		strcat(filename, ply_ptr->name);
		strcat(filename, ".skills\0");
		fpt = fopen(filename, "a+");
		//add the skill number to the end of the file
		strcpy(str, "");
		sprintf(str, "%i, 1,", skill);
		//strcat(str, ", 1,");
		fprintf(fpt, str);

		//let the player know they done good
		output(ply_ptr->fd, "\n You have learned a new skill!");
		output(ply_ptr->fd, "\n Type 'skilltree' to read more");
 		output(ply_ptr->fd, "\n");
		//close the file
		fclose(fpt);
	}
 	
	
 	return(0);
}

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
	
	//find the master objectives file in ../bin
	//read the objectives file
	//the structure of the file is pairs of lines
	//first lines give the lower and upper 
	//bounds to display the objective
	//second lines give the data to be displayed
	char filename[80] = {};
	char str[10] = {};
	FILE *fpt;

	strcpy(filename, "../bin/objectives.master\0");
	fpt = fopen(filename, "r");
	char array[750][80];
	int c,i,j, result;

	//clean the array
	for (i=0; i<750; i++){
		strcpy(array[i], "");
	}

	//iterate through the file and load the values into 
	//a handy array for future reading
	c = 0;
	result =0;
	while (result != EOF){
		result = fscanf(fpt, "%79[^,\n]", str);
		result = fscanf(fpt, "%*c");
		strcpy(array[c], str);
		
		//sprintf(g_buffer, "array %i: ", c);
		//output(fd, g_buffer);
		//output(fd, array[c]);
		//output(fd, "\n");

		c++;
	}
	fclose(fpt);

	ANSI(fd, AFC_GREEN);
	output(fd,"\n________________________________________________________________________________\n");	
	output(fd, "                       ====== CURRENT OBJECTIVES ======                      \n");
	output(fd,"________________________________________________________________________________\n");	
	
	ANSI(fd, AFC_WHITE);
	//read through the array
	//every even line in groups of 3
	//eg: (0, 1) will describe the bounds
	// (2) will describe the output

	
	for (i = 0, j =0; i < 50; i++){
		//output(fd, "hey");
		//sprintf(g_buffer, "array[%i]: %i, objective check = %i \n", (i*3), atoi(array[i*3]), objective_check(ply_ptr, atoi(array[i*3])));
		//output(fd, g_buffer);
		if (array[i*3 +0]){
			//sprintf(g_buffer, "array[%i]: %i, objective check = %i \n", i*3, atoi(array[i*3]), objective_check(ply_ptr, atoi(array[i*3])));
			//output(fd, g_buffer);
			if (objective_check(ply_ptr, atoi(array[i*3 + 0])) && !objective_check(ply_ptr, atoi(array[i*3 + 1]))){
		
				output(fd, array[i*3 + 2]);
				output(fd, "\n");
				j++;
			}
		}
	}
	if (j == 0){
		output(fd, "No objectives currently available\n");
	}
	output(fd, "\n");
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

		
	}

	return(0);

}
