#ifndef PARSE_H
#define PARSE_H

typedef struct cmd{
	char * name;
	char ** args;
	int size;
}cmd;


cmd* parse_line(char * cmd_line);
short check_arguments(char ** args_tab);
void free_cmd(cmd * cmd_struct);
#endif