#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "parse.h"
#include "parse_token.h"
#include "err_flags.h"

#define LEN_NAME        13
#define LEN_BNW         3
#define LEN_FILL        7
#define LEN_LOAD        5
#define LEN_LIST_BUFFER 2
#define LEN_RESIZE      5
#define LEN_ROTATE      4
#define LEN_TRUNCATE    6
#define LEN_SAVE        5
#define LEN_SWITCH      3
#define LEN_GREYS       3
#define LEN_NEG         3
#define LEN_REPLACE     13
#define LEN_SYM         3

struct cmd_info {
	char *name;
	int   len;
	short args_type [LEN_REPLACE];
	char *option [LEN_REPLACE];
};

static struct cmd_info info_tab[LEN_NAME]={
	{.name="bnw", .len=LEN_BNW, .args_type={0, 1}, .option={"", "-a"}},                                                   //ok //ok
	{.name="greyscale", .len=LEN_GREYS, .args_type={0, 1}, .option={"", "-a"}},                                           //ok //ok
	{.name="fill", .len=LEN_FILL, .args_type={0, 1, PIXEL, PIXEL, PIXEL, PIXEL}, .option={"", "-a", "", "", "", ""}}, //ok //ok
	{.name="list_buffer", .len=LEN_LIST_BUFFER, .args_type={0}, .option={""}},                                            //ok //ok
	{.name="load", .len=LEN_LOAD, .args_type={0, 2, NUMBER, STRING}, .option={"", "-w", "", ""}},                              //ok however check no (load)
	{.name="negative", .len=LEN_NEG, .args_type={0, 1}, .option={"", "-a"}},                                              //ok //ok 
	{.name="replace", .len=LEN_REPLACE, .args_type={0, 1, 2, POURC, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL}, .option={"", "-a", "-m", "", "", "", "", "", "", "", "", ""}},
	{.name="resize", .len=LEN_RESIZE, .args_type={0, VIEW, NUMBER, NUMBER}, .option={"", "", "", ""}},                    //ok //ok
	{.name="rotate", .len=LEN_ROTATE, .args_type={0, 1, ANGLE}, .option={"", "-r", ""}},                                 // ok //ok
	{.name="save", .len=LEN_SAVE, .args_type={0, 2, EXT, STRING}, .option={"", "-f", "", ""}},                                 //ok //ok
	{.name="switch_buffer",.len=LEN_SWITCH,.args_type={0,NUMBER},.option={"",""}},
	{.name="symmetry", .len=LEN_SYM, .args_type={0, SYMTYPE}, .option={"", ""}},                                          //ok // ok 
	{.name="truncate", .len=LEN_TRUNCATE, .args_type={0, NUMBER, NUMBER, NUMBER, NUMBER}, .option={"", "", "", "", ""}} //ok //ok
};

short msg_error(short type, int flags, char *cmd_name, char *str){
	if(flags == EINVA) fprintf(stderr, "Error command [%s]: invalid arguments '%s'\n", cmd_name, str);
	if(flags == EMSG) fprintf(stderr, "Error command [%s]: missing arguments\n", cmd_name);
	if(flags == ENUMV){
		if(type == PIXEL) fprintf(stderr, "Error command [%s]: invalid arguments '%s', please enter numerics values between 0 and 255\n", cmd_name, str);
		if(type == NUMBER) fprintf(stderr, "Error command [%s]: invalid arguments '%s', please enter numerics values \n", cmd_name, str);
		if(type == POURC) fprintf(stderr, "Error command [%s]: invalid arguments '%s', please enter a numeric values between 0 and 100\n", cmd_name, str);
	}
	if(flags == EFFORM) fprintf(stderr, "Error command [%s]: invalids arguments '%s', please enter a valid image extension\n", cmd_name, str);
	if(flags == EOPT) fprintf(stderr, "Error command [%s]: invalid arguments '%s', please enter a valid command option\n", cmd_name, str);
	return flags;
}

char *string_cpy(char *s){
	char *str=malloc(sizeof(char) * (strlen(s) + 1));
	str=memcpy(str, s, strlen(s) + 1);
	str[strlen(s)]='\0';
	return str;
}

short find_index (char *str){
	if(str == NULL) return -1;
	for (int i = 0 ; i < LEN_NAME ; i++){

		if(strcmp(str,info_tab[i].name) == 0)return i;
	}
	return -1;
}

cmd *alloc_cmd(){
	cmd *command=malloc(sizeof(command) + sizeof(char *) + sizeof(char *));
	if(command == NULL) return NULL;

	command->name=NULL;
	command->args=NULL;
	command->size=0;

	return command;
}

void free_cmd(cmd *command){
	free(command->name);
	for(int i=0; i < (command->size) - 1; i++)
		if(command->args[i] != NULL && strlen(command->args[i]) != 0) free(command->args[i]);
	free(command->args);
	free(command);
}

short realloc_cmd_args(cmd *command){
	command->size+=1;
	char **tab=realloc(command->args, (command->size) * sizeof(char *));
	if(tab == NULL) return 1;
	command->args=tab;
	command->args[command->size - 1]=NULL;
	return 0;
}

void set_cmd_args(cmd * command ){
	for(int i = 0 ; i < command -> size -1 ; i++){
		command -> args[i] ="";
	}
	command -> args [command -> size -1 ] = NULL;
}

short init_cmd(cmd * command , char * str){
	short index ;
	if ((index = find_index(str)) == -1 ) return index;
	command -> name = string_cpy( str);
	command -> size = info_tab[index].len;
	command -> args = malloc(command -> size *(sizeof(char *)));
	set_cmd_args(command);
	command -> args[0] = string_cpy(str) ; 
	return index; 
}


short is_natural(char *str){
	int i;
	int n=sscanf(str, "%u", &i);
	return n == 1 ? 0 : ENUMV;
}

short is_option(char *str){
	if(str == NULL || strlen(str) != 2 || str[0] != '-') return EOPT;
	return isalpha(str[1]) ?  0 : EOPT;
}

short is_option_flags(short index){
	return index > 0 && index < 11 ? 0 : 1;
}

short is_angle(char *str){
	int i;
	int n=sscanf(str, "%u", &i);
	return (n == 1 && i %90 == 0) ? 0 : ENUMV;  
}

short is_pixel(char *str){
	int i;
	int n=sscanf(str, "%u", &i);
	return n == 1 && i <= 255 && i >= 0 ? 0 : ENUMV;  // replace with flags error
}

short is_view(char *str){
	return strcmp(str, "worspace") == 0 || strcmp(str, "image") == 0 ? 0 : EINVA; // replace with flags error
}

short is_symtype(char *str){
	return strcmp(str, "v") == 0 || strcmp(str, "h") == 0 ? 0 : EINVA;   // replace with flags error
}

short is_extension(char *str){
	return strcmp(str, "png") == 0 ||
		   strcmp(str, "jpeg") == 0 ||
		   strcmp(str, "gif") == 0 ||
		   strcmp(str, "bmp") == 0 ? 0 : EFFORM;  // replace with flags error
}

short is_pourcent(char *str){
	int i;
	int n=sscanf("a", "%u", &i);
	return n == 1 && i <= 100 && i >= 0 ? 0 : ENUMV;
}


short check_token(short flags, char *cmd_name, char *arg){
	if(strlen(arg) == 0) return msg_error(0, EMSG, cmd_name, NULL);
	if(flags == STRING && strlen(arg) == 0) return msg_error(0, EMSG, cmd_name, NULL);
	if(flags == NUMBER && is_natural(arg)) return msg_error(NUMBER, ENUMV, cmd_name, arg);
	if(flags == PIXEL && is_pixel(arg)) return msg_error(PIXEL, ENUMV, cmd_name, arg);
	if(flags == POURC && is_pourcent(arg)) return msg_error(POURC, ENUMV, cmd_name, arg);
	if(flags == EXT && is_extension(arg)) return msg_error(EXT, EFFORM, cmd_name, arg);
	if(flags == VIEW && is_view(arg) ) return msg_error(VIEW,EINVA, cmd_name, arg);
	if(flags == SYMTYPE && is_symtype(arg)) return msg_error(SYMTYPE,EINVA, cmd_name, arg);
	if(flags == ANGLE && is_angle(arg)) return msg_error(ANGLE, ENUMV, cmd_name, arg);
	return 0;
}


short check_arguments(cmd * command ){
	if(command == NULL) return 0;
	
	short n , index, flags ;
	index = find_index(command -> name ) ; 

	for(int i = 1 ; i < info_tab[index].len -1 ; i++ ){
		
		flags = info_tab[index].args_type[i];
		
		if(is_option_flags(flags) && strlen(command->args[i]) == 0 ){
			i+= flags - 1;
			continue;
		}
		if(is_option_flags( flags ) && strcmp( command -> args[i] , info_tab[index].option[i] ) != 0){
			return msg_error(0, EOPT, command->name , command -> args[i]);
		}
		if((n = check_token( info_tab[index].args_type[i],command -> name , command -> args[i])) != 0) return n;
	
	}
	if ( command -> size > info_tab[index].len ){
		fprintf(stderr,"Error command [%s] : too much arguments \n", command -> name );
		return EINVA;    
	}
	return 0;
}

short build_args(cmd * command,char *s , short index ){
	if (s ==NULL) return 0;
	int i = 1 ;
	char * str = string_cpy(s);
	char * space = " ";
	char * token ="";
	while(token != NULL){
		
		if(i != 1 ) token = strtok(NULL,space);
		else token = strtok(str,space);

		if(token != NULL){
			if( is_option_flags(info_tab[index].args_type[i]) ==1 && is_option(token) )
				i += info_tab[index].args_type[i];
			if(i >= (command -> size) -1 ) 
				realloc_cmd_args( command );
			command -> args[i++] = token;
		}
	}
	return 0;
}

cmd * parse_line( char * line){  
	cmd * command = NULL;
	short index;
	command = alloc_cmd(); 
	char * s = string_cpy(line);        
	char * space = " ";               
	char * token = strtok(s,space);     
	char * s1 = strtok(NULL,"");        
	if((index  = init_cmd(command,token) ) == -1  ){
		msg_error(0,EUNKN,token,NULL);  
		return NULL;
	}
	if(index > -1 ) build_args( command , s1 , index );
	

	//free(s);
	//free(token);            
	return command ;
}


