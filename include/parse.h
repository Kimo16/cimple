#ifndef PARSE_H
#define PARSE_H

typedef struct cmd{
	char * name;
	char ** args;
	int size;
}cmd;


cmd* parse_line(char * cmd_line);

#endif