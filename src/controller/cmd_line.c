#include "cmd_line.h"


/* Don't forget to add free rect in m_*.c file */
/*add test for correct coordinate (paste)*/


static char *string_cpy(char *s){
	char *str=malloc(sizeof(char) * (strlen(s) + 1));
	if((str=memcpy(str, s, strlen(s) + 1)) == NULL){
		fprintf(stderr, "Error : memory copy failed\n");
		return NULL;
	}
	str[strlen(s)]='\0';
	return str;
}

static int string_to_int(char *str){
	int i;
	sscanf(str, "%d", &i);
	return i;
}

short handler_cmd_bnw(cmd *command){                                      /*not finish*/
	frame *f=get_cursor_buffer();
	printf("here\n");
	if(f == NULL) return 0;
	image *  img=f->image;
	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};
	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();
	if(black_and_white_filter(img, rect) != 1) return 0;
	if(update_frame(f) != 1) return 0;
	return 1;
}

short handler_cmd_copy(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();

	if(copy(img, rect) != 1) return 0;
	if(update_frame(f) != 1) return 0;
	return 1;
}

short handler_cmd_contrast(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	int percent=string_to_int(command->args[2]);

	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();

	if(contrast(img, rect, percent) != 1) return 0;
	if(update_frame(f) != 1) return 0;
	return 1;
}

short handler_cmd_cut(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();

	if(negative_filter(img, rect) != 1) return 0;
	if(update_frame(f) != 1) return 0;
	return 1;
}

short handler_cmd_greyscale(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();

	if(grey_filter(img, rect) != 1) return 0;
	if(update_frame(f) != 1) return 0;
	return 1;
}

short handler_cmd_fill(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	int col_r=string_to_int(command->args[2]);
	int col_g=string_to_int(command->args[3]);
	int col_b=string_to_int(command->args[4]);
	int col_a=string_to_int(command->args[5]);

	SDL_Rect  rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};
	SDL_Color color={col_r, col_g, col_b, col_a};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();

	if(color_zone(img, rect, color) != 1) return 0;
	if(update_frame(f) != 1) return 0;
	return 1;
}

short handler_cmd_light(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;
	if( strcmp (command -> args[2] ,"") == 0 ){
		fprintf(stderr, "Error command [%s] : please enter a percent rate\n",command -> args[0] );
		return 0 ;
	}
	int percent=string_to_int(command->args[2]);
	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();
	printf("here8\n");
	if(light_filter(img, rect, percent) != 1) return 0;
	printf("here10\n");
	if(update_frame(f) != 1) return 0;
	return 1;
}

short handler_cmd_list_buff(cmd *command){ /*not finish*/
	//for(int i=0; i < 10; i++){             /*replace 10 by a flag when frame_tab would be define */

	/*if(frame_tab[i] != NULL)
	 *  printf("Window id : %d | image name : %s\n", i, get_img_name(frame_tab[i]->img));
	 * printf("Window id : %d  | No window load \n", i);
	 * }*/
	return 1;
}

short handler_cmd_load(cmd *command){
	if(strcmp(command->args[1], "-w") != 0){
		if(new_frame(command->args[3]) == 0) return 0;
	}
	else{
		int index=string_to_int(command->args[2]);
		if(moveto_buffer(index) != 1){
			fprintf(stderr, "Error command[load] : invalid window id \n");
			return 0;
		}
		if(update_frame(get_cursor_buffer()) == 0) return 1;
	}
	return 1;
}

short handler_cmd_negative(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();

	if(negative_filter(img, rect) != 1) return 0;
	if(update_frame(f) != 1) return 0;
	return 1;
}

short handler_cmd_paste(cmd *command){                                                  /*not finish*/
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	if(strcmp(command->args[1], "-a") == 0){
		if(strcmp(command->args[2], "") != 0 && strcmp(command->args[3], "")){
			fprintf(stderr, "Error command[paste] : too much arguments \n");
			return 0;
		}
		return paste(img, 0, 0);
	}
	else{
		int x=string_to_int(command->args[2]);
		int y=string_to_int(command->args[3]);
		return paste(img, x, y);
	} /*select image zone*/
}

short handler_cmd_quit(cmd *command){
	if(strcmp(command->args[1], "-w") != 0){
		free_frames();
		printf("CIMPLE PHOTO EDITOR ----> SHUT DOWN\n");
		exit(0);
	}
	int index=string_to_int(command->args[2]);
	if(moveto_buffer(index) != 1){
		fprintf(stderr, "Error command [quit] : invalid window id \n");
		return 0;
	}
	free_frame(get_cursor_buffer());
	return 1;
}

short handler_cmd_replace(cmd *command){                                         /*not finish*/
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	int percent=0;
	int orig_r=string_to_int(command->args[4]);
	int orig_g=string_to_int(command->args[5]);
	int orig_b=string_to_int(command->args[6]);
	int orig_a=string_to_int(command->args[7]);
	int targ_r=string_to_int(command->args[8]);
	int targ_g=string_to_int(command->args[9]);
	int targ_b=string_to_int(command->args[10]);
	int targ_a=string_to_int(command->args[11]);

	if(strcmp(command->args[1], "-m") == 0)
		percent=string_to_int(command->args[2]);

	SDL_Color origin_color={orig_r, orig_g, orig_b, orig_a};
	SDL_Color target_color={targ_r, targ_g, targ_b, targ_a};

	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[3], "") == 0)
		rect=get_select_array();                /*select zone in windows*/

	if(replace_color(img, rect, origin_color, target_color, percent) != 1) return 0;
	if(update_frame(f) != 1) return 0;
	return 1;
}

short handler_cmd_resize(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;
	short n;
	int width=string_to_int(command->args[2]);
	int height=string_to_int(command->args[3]);

	if(strcmp(command->args[1], "workspace") == 0) n =  resize_workspace(img, width, height);
	else n = resize_image(img, width, height);
	if( n != 1) return 0 ;
	if(update_frame(f) != 1 ) return 0 ; 
	return 1; 
}

short handler_cmd_rotate(cmd *command){                                      /*not finish*/
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;
	int    n;
	int    angle=string_to_int(command->args[2]);
	if(strcmp(command->args[1], "-r") == 0) n=rotate(img, angle, 1);
	n=rotate(img, angle, 0);
	update_frame(f);
	return n;
}

short handler_cmd_save(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL){
		fprintf(stderr, "Error command [%s] : no window founded , please load an image\n", command->name);
		return 0;
	}
	image *img=f->image;

	if(strcmp(command->args[1], "-f") == 0){
		image *new_img=save_image_as(img, command->args[3], command->args[2]);
		if(new_img == NULL){
			fprintf(stderr, "Error command [%s] : error while saving the image\n", command->args[0]);
			return 0;
		}
		f->image=new_img;
		return update_frame(f);
	}
	if(save_image(img) != 0) return 0;
	return 1;
}

short handler_cmd_switch_buff(cmd *command){
	return 0;
}

short handler_cmd_symmetry(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	int mode=0;

	if(strcmp(command->args[1], "-v") == 0) mode=1;
	return symmetry(img, mode);
}

short handler_cmd_truncate(cmd *command){                                        /*correction in parser needed*/
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	return 0;
}

static void cmd_function_handler(cmd *command){
	if(strcmp(command->name, "bnw") == 0) handler_cmd_bnw(command);
	if(strcmp(command->name, "copy") == 0) handler_cmd_copy(command);
	if(strcmp(command->name, "cut") == 0) handler_cmd_cut(command);
	if(strcmp(command->name, "contrast") == 0) handler_cmd_contrast(command);
	if(strcmp(command->name, "greyscale") == 0) handler_cmd_greyscale(command);
	if(strcmp(command->name, "fill") == 0) handler_cmd_fill(command);
	if(strcmp(command->name, "light") == 0) handler_cmd_light(command);
	if(strcmp(command->name, "list_buffer") == 0) handler_cmd_list_buff(command);
	if(strcmp(command->name, "load") == 0) handler_cmd_load(command);
	if(strcmp(command->name, "negative") == 0) handler_cmd_negative(command);
	if(strcmp(command->name, "paste") == 0) handler_cmd_paste(command);
	if(strcmp(command->name, "quit") == 0) handler_cmd_quit(command);
	if(strcmp(command->name, "replace") == 0) handler_cmd_replace(command);
	if(strcmp(command->name, "resize") == 0) handler_cmd_resize(command);
	if(strcmp(command->name, "rotate") == 0) handler_cmd_rotate(command);
	if(strcmp(command->name, "save") == 0) handler_cmd_save(command);
	if(strcmp(command->name, "switch_buffer") == 0) handler_cmd_switch_buff(command);
	if(strcmp(command->name, "symmetry") == 0) handler_cmd_symmetry(command);
	if(strcmp(command->name, "truncate") == 0) handler_cmd_truncate(command);
}

void cimple_handler(){
	while(1){
		char *cmd_line=getcmdline();
		cmd * command=parse_line(string_cpy(cmd_line));
		printf("here6\n");
		if(command != NULL){
			cmd_function_handler(command);
			free(cmd_line);
			free_cmd(command);
		}
	}
}
