#include "parse.h"


struct cmd_info {
	char *name;
	int   len;
	short args_type [LEN_REPLACE];
	char *option [LEN_REPLACE];
};

static struct cmd_info info_tab[LEN_NAME]={
	{.name="bnw", .len=LEN_BNW, .args_type={0, 1}, .option={"", "-a"}},
	{.name="greyscale", .len=LEN_GREYS, .args_type={0, 1}, .option={"", "-a"}},
	{.name="fill", .len=LEN_FILL, .args_type={0, 1, PIXEL, PIXEL, PIXEL, PIXEL}, .option={"", "-a", "", "", "", ""}},
	{.name="list_buffer", .len=LEN_LIST_BUFFER, .args_type={0}, .option={""}},
	{.name="load", .len=LEN_LOAD, .args_type={0, 2, NUMBER, FILE}, .option={"", "-w", "", ""}},
	{.name="negative", .len=LEN_NEG, .args_type={0, 1}, .option={"", "-a"}},
	{.name="replace", .len=LEN_REPLACE, .args_type={0, 2, POURC, 1, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL}, .option={"", "-m", "", "-a", "", "", "", "", "", "", "", ""}},
	{.name="resize", .len=LEN_RESIZE, .args_type={0, VIEW, NUMBER, NUMBER}, .option={"", "", "", ""}},
	{.name="rotate", .len=LEN_ROTATE, .args_type={0, 1, ANGLE}, .option={"", "-r", ""}},
	{.name="save", .len=LEN_SAVE, .args_type={0, 2, EXT, STRING}, .option={"", "-f", "", ""}},
	{.name="switch_buffer", .len=LEN_SWITCH, .args_type={0, NUMBER}, .option={"", ""}},
	{.name="symmetry", .len=LEN_SYM, .args_type={0, SYMTYPE}, .option={"", ""}},
	{.name="truncate", .len=LEN_TRUNCATE, .args_type={0, NUMBER, NUMBER, NUMBER, NUMBER}, .option={"", "", "", "", ""}}
};

short msg_error(short type, int flags, char *cmd_name, char *str){
	switch(flags){
	case EINVA:
		if(type == SYMTYPE) fprintf(stderr, "Error command [%s]: invalid argument '%s', please enter 'v' for vertical or 'h for horizontal\n", cmd_name, str);
		else if(type == VIEW) fprintf(stderr, "Error command [%s] , invalid argument '%s' please enter 'workspace' or 'image' for resize one of them\n", cmd_name, str);
		else fprintf(stderr, "Error command [%s]: invalid argument '%s'\n", cmd_name, str);
		break;

	case EMSG:
		fprintf(stderr, "Error command [%s]: missing arguments\n", cmd_name);
		break;

	case ENUMV:
		if(type == PIXEL) fprintf(stderr, "Error command [%s]: invalid arguments '%s', please enter numeric value between 0 and 255\n", cmd_name, str);
		if(type == NUMBER) fprintf(stderr, "Error command [%s]: invalid arguments '%s', please enter positive numeric value \n", cmd_name, str);
		if(type == POURC) fprintf(stderr, "Error command [%s]: invalid arguments '%s', please enter a numeric value between 0 and 100\n", cmd_name, str);
		if(type == ANGLE) fprintf(stderr, "Error command [%s]: invalid argument '%s', please enter a multiple of 90\n", cmd_name, str);
		break;

	case EFFORM:
		if(type == EXT) fprintf(stderr, "Error command [%s]: invalids argument '%s', please enter a valid image extension\n", cmd_name, str);
		if(type == FILE) fprintf(stderr, "Error command [%s]: invalids argument '%s', please enter a valid file format\n", cmd_name, str);
		break;

	case EOPT:
		fprintf(stderr, "Error command [%s]: invalid arguments '%s', please enter a valid command option\n", cmd_name, str);
		break;

	case EUNKN:
		fprintf(stderr, "Error command [%s] : command not found\n", cmd_name);
		break;
	}
	return flags;
}

char *string_cpy(char *s){
	char *str=malloc(sizeof(char) * (strlen(s) + 1));
	if((str=memcpy(str, s, strlen(s) + 1)) == NULL){
		fprintf(stderr, "Error : memory copy failed\n");
		return NULL;
	}
	str[strlen(s)]='\0';
	return str;
}

short find_index(char *str){
	if(str == NULL) return -1;
	int i;
	for(i=0; i < LEN_NAME; i++)
		if(strcmp(str, info_tab[i].name) == 0) return i;
	return -1;
}

cmd *alloc_cmd(){
	cmd *command=malloc(sizeof(cmd) + sizeof(char *) + sizeof(char *));
	if(command == NULL) return NULL;

	command->name=NULL;
	command->args=NULL;
	command->size=0;

	return command;
}

void free_cmd(cmd *command){
	if(command == NULL) return;
	if(command->name!= NULL)free(command->name);
	int i;
	for(i=0; i < (command->size) - 1; i++)
		if(command->args[i] != NULL && strlen(command->args[i]) != 0) free(command->args[i]);
	if(command -> args != NULL) free(command->args);
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

void set_cmd_args(cmd *command){
	int i;
	for(i=0; i < command->size - 1; i++)
		command->args[i]="";
	command->args [command->size - 1]=NULL;
}

short init_cmd(cmd *command, char *str){
	short index;
	if((index=find_index(str)) == -1) return index;
	if((command->name=string_cpy(str))==NULL)return -2;
	command->size=info_tab[index].len;
	command->args=malloc(command->size * (sizeof(char *)));
	if(command -> args == NULL){
		fprintf(stderr, "Error : memory allocation failed\n");
		return -2;
	}
	set_cmd_args(command);
	if((command->args[0]=string_cpy(str))==NULL)return -2;
	return index;
}

short is_natural(char *str){
	int i;
	int n=sscanf(str, "%u", &i);
	return (n == 1 && i >= 0) ? 0 : ENUMV;
}

short is_option(char *str){
	if(str == NULL || strlen(str) != 2 || str[0] != '-') return EOPT;
	return isalpha(str[1]) ?  0 : EOPT;
}

short is_option_flags(short index){
	return (index > 0 && index < 11) ? 1 : 0;
}

short is_angle(char *str){
	int i;
	int n=sscanf(str, "%u", &i);
	return (n == 1 && i % 90 == 0 && i >= 0) ? 0 : ENUMV;
}

short is_pixel(char *str){
	int i;
	int n=sscanf(str, "%u", &i);
	return (n == 1 && i <= 255 && i >= 0) ? 0 : ENUMV;
}

short is_view(char *str){
	return (strcmp(str, "workspace") == 0 || strcmp(str, "image") == 0) ? 0 : EINVA;
}

short is_symtype(char *str){
	return (strcmp(str, "v") == 0 || strcmp(str, "h") == 0) ? 0 : EINVA;
}

short is_extension(char *str){
	return (strcmp(str, "png") == 0 ||
			strcmp(str, "jpeg") == 0 ||
			strcmp(str, "gif") == 0 ||
			strcmp(str, "bmp") == 0) ? 0 : EFFORM;
}

short is_pourcent(char *str){
	int i;
	int n=sscanf(str, "%u", &i);
	return (n == 1 && i <= 100 && i >= 0) ? 0 : ENUMV;
}

short is_file(char *str){
	short i=0, mark=0;
	while(str[i] != '\0'){
		if(str[i] == '.' && str[i + 1] != '\0'){
			if(i != 0 && str[i - 1] != '.' && str[i - 1] != '/' && str[i + 1] != '/' && str[i + 2] != '/') mark+=1;
			if(mark > 1) return EFFORM;
		}
		i+=1;
	}
	return mark == 1 ? 0 : EFFORM;
}

short check_option(short index, short i, char *arg){
	while(i < info_tab[index].len - 1)
		if(strcmp(arg, info_tab[index].option[i++]) == 0) return 0;
	return 1;
}

short check_token(short flags, char *cmd_name, char *arg){
	if(strlen(arg) == 0) return msg_error(0, EMSG, cmd_name, NULL);
	if(flags == STRING && strlen(arg) == 0) return msg_error(0, EMSG, cmd_name, NULL);
	if(flags == FILE && is_file(arg)) return msg_error(FILE, EFFORM, cmd_name, arg);
	if(flags == NUMBER && is_natural(arg)) return msg_error(NUMBER, ENUMV, cmd_name, arg);
	if(flags == PIXEL && is_pixel(arg)) return msg_error(PIXEL, ENUMV, cmd_name, arg);
	if(flags == POURC && is_pourcent(arg)) return msg_error(POURC, ENUMV, cmd_name, arg);
	if(flags == EXT && is_extension(arg)) return msg_error(EXT, EFFORM, cmd_name, arg);
	if(flags == VIEW && is_view(arg)) return msg_error(VIEW, EINVA, cmd_name, arg);
	if(flags == SYMTYPE && is_symtype(arg)) return msg_error(SYMTYPE, EINVA, cmd_name, arg);
	if(flags == ANGLE && is_angle(arg)) return msg_error(ANGLE, ENUMV, cmd_name, arg);
	return 0;
}

short check_arguments(cmd *command){
	if(command == NULL) return 0;
	int   i;
	short n, index, flags;
	index=find_index(command->name);

	for(i=1; i < info_tab[index].len - 1; i++){
		flags=info_tab[index].args_type[i];

		if(is_option_flags(flags) && strlen(command->args[i]) == 0){
			i+=flags - 1;
			continue;
		}
		if(is_option_flags(flags) && strcmp(command->args[i], info_tab[index].option[i]) != 0)
			return msg_error(0, EOPT, command->name, command->args[i]);
		if((n=check_token(info_tab[index].args_type[i], command->name, command->args[i])) != 0) return n;
	}
	if(command->size > info_tab[index].len){
		fprintf(stderr, "Error command [%s] : too much arguments \n", command->name);
		return EINVA;
	}
	return 0;
}

short build_args(cmd *command, char *s, short index){
	if(s == NULL || command == NULL) return 0;
	int   i=1;
	char *str=string_cpy(s);
	if(str == NULL ) return 1;
	char *space=" ";
	char *token="";
	while(token != NULL){
		if(i != 1) token=strtok(NULL, space);
		else token=strtok(str, space);

		if(token != NULL){
			while(is_option_flags(info_tab[index].args_type[i]) == 1){
				if(is_option(token)) i+=info_tab[index].args_type[i];
				else if(is_option(token) == 0 && check_option(index, i + 1, token) == 0) i+=info_tab[index].args_type[i];
				else break;
			}
			if(i >= (command->size) - 1)
				realloc_cmd_args(command);
			if((command->args[i++]=string_cpy(token)) == NULL) return 1;
		}
	}
	if(token != NULL) free(token);
	if(str != NULL) free(str);
	return 0;
}

cmd *parse_line(char *line){
	cmd * command=NULL;
	short index , i = 0 ;
	command=alloc_cmd();
	char *s=NULL;
	if((s=string_cpy(line))==NULL){
		free_cmd(command);
		return NULL;
	}
	char *space=" ";
	char *token=strtok(s, space);
	char *s1=strtok(NULL, "");
	if((index=init_cmd(command, token)) < 0){
		if(index == -1)msg_error(0, EUNKN, token, NULL);
		free_cmd(command);
		if(token != NULL) free(token);
		return NULL;
	}
	if(index > -1) i = build_args(command, s1, index);
	if(i == 1){
		free_cmd(command);
		if(token != NULL) free(token);
		return NULL;
	}
	if(token != NULL) free(token);
	return command;
}
