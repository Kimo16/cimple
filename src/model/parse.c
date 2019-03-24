#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "parse_token.h"
#include "err_flags.h"

#define LEN_NAME        12
#define LEN_BNW         3
#define LEN_FILL        7
#define LEN_LOAD        5
#define LEN_LIST_BUFFER 2
#define LEN_RESIZE      5
#define LEN_ROTATE      4
#define LEN_TRUNCATE    6
#define LEN_SAVE        5
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

static struct cmd_info name_tab[LEN_NAME]={
	{.name="bnw", .len=LEN_BNW, .args_type={0, 1}, .option={"", "-a"}},                                                   //ok
	{.name="greyscale", .len=LEN_GREYS, .args_type={0, 1}, .option={"", "-a"}},                                           //ok
	{.name="fill", .len=LEN_FILL, .args_type={0, 1, NUMBER, NUMBER, NUMBER, NUMBER}, .option={"", "-a", "", "", "", ""}}, //ok
	{.name="list_buffer", .len=LEN_LIST_BUFFER, .args_type={0}, .option={""}},                                            //ok
	{.name="load", .len=LEN_LOAD, .args_type={0, 2, NUMBER, 0}, .option={"", "-w", "", ""}},                              //ok
	{.name="negative", .len=LEN_NEG, .args_type={0, 1}, .option={"", "-a"}},                                              //ok
	{.name="replace", .len=LEN_REPLACE, .args_type={0, 1, 2, POURC, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL}, .option={"", "-a", "-m", "", "", "", "", "", "", "", "", ""}},
	{.name="resize", .len=LEN_RESIZE, .args_type={0, VIEW, NUMBER, NUMBER}, .option={"", "", "", ""}},                    //ok
	{.name="rotate", .len=LEN_ROTATE, .args_type={0, 1, NUMBER}, .option={"", "-r", ""}},                                 // ok
	{.name="save", .len=LEN_SAVE, .args_type={0, 2, EXT, 0}, .option={"", "-f", "", ""}},                                 //ok however
	{.name="symmetry", .len=LEN_SYM, .args_type={0, SYMTYPE}, .option={"", ""}},                                          //ok
	{.name="truncate", .len=LEN_TRUNCATE, .args_type={0, NUMBER, NUMBER, NUMBER, NUMBER}, .option={"", "", "", "", ""}} //ok
};

cmd * alloc_cmd(){
	cmd * command = malloc(sizeof(command)+sizeof(char *)+sizeof(char *));
	if(command == NULL) return NULL;
	
	command -> name = NULL;
	command -> args = NULL;
	command -> size = 0;
	
	return command;
}

void free_cmd(cmd * command){
	free(command -> name);
	for (int i = 0 ; i<(command -> size) -1 ; i++){
		if(command -> args[i] != NULL && strlen(command -> args[i]) != 0)free(command -> args[i]);
	}
	free(command->args);
	free(command);
}

short realloc_cmd_args(cmd * command){
	command -> size += 1;
	char ** tab = realloc(command -> args , (command -> size) * sizeof(char *));
	if(tab == NULL){
		perror("INVALID REALLOC() -> function : realloc_cmd_tab");
		return 1;
	}
	command -> args = tab;
	command -> args[command -> size -1 ] = NULL;
	return 0;
}

char * string_cpy(char * s){
	char * str = malloc(sizeof(char)*(strlen(s)+1));
	str = memcpy(str,s,strlen(s)+1);
	str[strlen(s)] = '\0';
	return str;
}

short msg_error(short type, int flags, char *cmd_name, char *str){
	if(flags == EINVAL) fprintf(stderr, "Error command [%s]: invalid arguments '%s'\n", cmd_name, str);
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



short is_natural(char * str){
	if(str == NULL || strlen(str)== 0 )return 1;
	return 0;
}
short is_option(char * str){
	if(str == NULL || strlen(str)!=2 || str[0] != '-')return 1;
	return isalpha(str[1]) ?  0 : 1; 
}
short is_pixel(char * str){
	int i ;
	int n = sscanf("a","%u",&i);
	return n == 1 && i <= 255 && i >= 0 ? 0 : 1 ; // replace with flags error
}
short is_view(char * str){
	return strcmp(str,"worspace")== 0 || strcmp(str,"image") == 0 ? 0 : 1 ; // replace with flags error
}
short is_symtype(char * str){
	return strcmp(str,"v")== 0 || strcmp(str,"h") == 0 ? 0 : 1 ;   // replace with flags error
}
short is_extension(char * str){
	return strcmp(str,"png") == 0 
		|| strcmp(str,"jpeg") == 0 
		|| strcmp(str,"gif") == 0
		|| strcmp(str,"bmp") == 0 ? 0 : 1;  // replace with flags error
}
short is_pourcent(char * str){
	int i ;
	int n = sscanf("a","%u",&i);
	return n == 1 && i <= 100 && i >= 0 ? 0 : 1 ;
}


