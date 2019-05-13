#include "cmd_line.h"



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

/**
 * Call black_and_white model function in m_color.c to modify and apply modifications by calling view function
 * Call select function in event.c when -a option isn't present
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

short handler_cmd_bnw(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();

	if(black_and_white_filter(img, rect) != 1) return 0;
	if(update_frame(f, NULL) != 1) return 0;
	return 1;
}

/**
 * Call greyscale model function in m_color.c to modify pixels and apply modifications by calling view function
 * Call select function in event.c when -a option isn't present
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

short handler_cmd_greyscale(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();

	if(grey_filter(img, rect) != 1) return 0;
	if(update_frame(f, NULL) != 1) return 0;
	return 1;
}

/**
 * Call negative model function in m_color.c to modify pixels and apply modifications by calling view function
 * Call select function in event.c when -a option isn't present
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

short handler_cmd_negative(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();

	if(negative_filter(img, rect) != 1) return 0;
	if(update_frame(f, NULL) != 1) return 0;
	return 1;
}

/**
 * Call contrast model function in m_color.c to modify pixels and apply modifications by calling view function
 * Call select function in event.c when -a option isn't present
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

short handler_cmd_contrast(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;
	if(strcmp(command->args[2], "") == 0){
		fprintf(stderr, "Error command [%s] : please enter a percent rate\n", command->args[0]);
		return 0;
	}
	int percent=string_to_int(command->args[2]);

	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();

	if(contrast(img, rect, percent) != 1) return 0;
	if(update_frame(f, NULL) != 1) return 0;
	return 1;
}

/**
 * Call light model function in m_color.c to modify pixels and apply modifications by calling view function
 * Call select function in event.c when -a option isn't present
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

short handler_cmd_light(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;
	if(strcmp(command->args[2], "") == 0){
		fprintf(stderr, "Error command [%s] : please enter a percent rate\n", command->args[0]);
		return 0;
	}
	int      percent=string_to_int(command->args[2]);
	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();
	if(light_filter(img, rect, percent) != 1) return 0;
	if(update_frame(f, NULL) != 1) return 0;
	return 1;
}

/**
 * Call replace model function in m_color.c to modify pixels and apply modifications by calling view function
 * Call select function in event.c when -a option isn't present
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

short handler_cmd_replace(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	int percent=1;
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
		rect=get_select_array();

	if(replace_color(img, rect, origin_color, target_color, percent) != 1) return 0;
	if(update_frame(f, NULL) != 1) return 0;
	return 1;
}

/**
 * Call colorzone function in m_color.c and apply the modification by calling view function
 * Call select function in event.c when -a option isn't present
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

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
	if(update_frame(f, NULL) != 1) return 0;
	return 1;
}

/**
 * Call copy function in m_transform.c and apply the modification by calling view function
 * Call select function in event.c when -a option isn't present
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

short handler_cmd_copy(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();

	if(copy(img, rect) != 1) return 0;
	if(update_frame(f, NULL) != 1) return 0;
	return 1;
}

/**
 * Call cut function in m_transform.c and apply the modification by calling view function
 * Call select function in event.c when -a option isn't present
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

short handler_cmd_cut(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();

	if(cut(img, rect) != 1) return 0;
	if(update_frame(f, NULL) != 1) return 0;
	return 1;
}

/**
 * Call paste function in m_transform.c and apply the modification by calling view function
 * Call select point function in event.c when -a option isn't present
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

short handler_cmd_paste(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	int x=0, y=0;
	if(strcmp(command->args[1], "") == 0){
		SDL_Point p=get_point();
		x=p.x;
		y=p.y;
	}
	if(paste(img, x, y) != 1) return 0;
	if(update_frame(f, NULL) != 1) return 0;
	return 1;
}

/**
 * Call symmetry function in m_transform.c and apply the modification by calling view function
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

short handler_cmd_symmetry(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	int mode=0;

	if(strcmp(command->args[1], "v") == 0) mode=1;
	if(symmetry(img, mode) != 1) return 0;
	if(update_frame(f, NULL) != 1) return 0;
	return 1;
}

/**
 * Call resize function in m_frame.c and apply the modification by calling view function
 * Resize the workspace or the current image
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

short handler_cmd_resize(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;
	short  n;
	int    width=string_to_int(command->args[2]);
	int    height=string_to_int(command->args[3]);

	if(strcmp(command->args[1], "workspace") == 0) n=resize_workspace(img, width, height);
	else n=resize_image(img, width, height);
	if(n != 1) return 0;
	if(update_frame(f, NULL) != 1) return 0;
	return 1;
}

/**
 * Call rotate function in m_frame.c and apply the modification by calling view function
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

short handler_cmd_rotate(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;
	int    n;
	int    angle=string_to_int(command->args[2]);
	if(strcmp(command->args[1], "-r") == 0) n=rotate(img, angle, 1);
	n=rotate(img, angle, 0);
	update_frame(f, NULL);
	return n;
}

/**
 * Call truncation function in m_frame.c and apply the modification by calling view function
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */


short handler_cmd_truncate(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL) return 0;
	image *img=f->image;

	SDL_Rect rect={0, 0, get_img_surface(img)->w, get_img_surface(img)->h};

	if(strcmp(command->args[1], "") == 0)
		rect=get_select_array();
	else {
		int x1=string_to_int(command->args[2]);
		int y1=string_to_int(command->args[3]);
		int x2=string_to_int(command->args[4]);
		int y2=string_to_int(command->args[5]);
		rect.x=x1;
		rect.y=y1;
		rect.w=x2 - x1;
		rect.h=y2 - y1;
	}

	if(truncate_image(img, rect) != 1) return 0;
	if(update_frame(f, NULL) != 1) return 0;
	return 1;
}

/**
 * Print frame list by calling function in event.c
 *
 * @param cmd * command , pointer to a command structure
 * @return 1
 */

short handler_cmd_list_buff(cmd *command){
	print_frame();
	return 1;
}

/**
 * Quit Cimple program , or close a specific frame by window id
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if changes failed , 1 if changes done.
 */

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
	free_frame_buffer(index);
	return 1;
}

/**
 * Change the current frame by calling view function in event.c
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if change failed , 1 if change done .
 */

short handler_cmd_switch_buff(cmd *command){
	short s=moveto_buffer(string_to_int(command->args[1]));
	if(s != 1){
		if(s == -1) fprintf(stderr, "Error command[%s] : Invalid window id , index out of born ( [0;10] ) \n", command->name);
		if(s == 0) fprintf(stderr, "Error command[%s] : Invalid window id , window isn't initialised \n", command->name);
		return 0;
	}
	return s;
}

/**
 * Call load function in in.c and open an image in a frame by calling view function
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if load failed , 1 if load done.
 */

short handler_cmd_load(cmd *command){
	if(strcmp(command->args[1], "-w") != 0){
		if(new_frame(command->args[3]) != 0) return 0;
	}
	else{
		int index=string_to_int(command->args[2]);
		if(moveto_buffer(index) != 1){
			fprintf(stderr, "Error command[load] : invalid window id \n");
			return 0;
		}
		if(update_frame(get_cursor_buffer(), command->args[3]) != 1) return 0;
	}
	return 1;
}

/**
 * Call save function in out.c and apply the modification by calling view function if image format change
 *
 * @param cmd * command , pointer to a command structure
 * @return 0 if save failed , 1 if save done.
 */

short handler_cmd_save(cmd *command){
	frame *f=get_cursor_buffer();
	if(f == NULL){
		fprintf(stderr, "Error command [%s] : no window founded , please load an image\n", command->name);
		return 0;
	}
	image *img=f->image;

	if(strcmp(command->args[1], "-p") == 0){
		image *new_img=save_image_as(img, command->args[2]);
		if(new_img == NULL){
			fprintf(stderr, "Error command [%s] : error while saving the image\n", command->args[0]);
			return 0;
		}
		f->image=new_img;
		return update_frame(f, NULL);
	}
	if(save_image(img) != 0) return 0;
	return 1;
}

/**
 * Redirection to a specific handler function by the help of command name
 *
 * @param cmd pointer contains all command informations
 * @return 0 if the command will execute with sucess , 1 if an error has occured
 */


static short cmd_function_handler(cmd *command){
	if(strcmp(command->name, "bnw") == 0) return handler_cmd_bnw(command);
	if(strcmp(command->name, "copy") == 0) return handler_cmd_copy(command);
	if(strcmp(command->name, "cut") == 0) return handler_cmd_cut(command);
	if(strcmp(command->name, "contrast") == 0) return handler_cmd_contrast(command);
	if(strcmp(command->name, "greyscale") == 0) return handler_cmd_greyscale(command);
	if(strcmp(command->name, "fill") == 0) return handler_cmd_fill(command);
	if(strcmp(command->name, "light") == 0) return handler_cmd_light(command);
	if(strcmp(command->name, "list_buffer") == 0) return handler_cmd_list_buff(command);
	if(strcmp(command->name, "load") == 0) return handler_cmd_load(command);
	if(strcmp(command->name, "negative") == 0) return handler_cmd_negative(command);
	if(strcmp(command->name, "paste") == 0) return handler_cmd_paste(command);
	if(strcmp(command->name, "quit") == 0) return handler_cmd_quit(command);
	if(strcmp(command->name, "replace") == 0) return handler_cmd_replace(command);
	if(strcmp(command->name, "resize") == 0) return handler_cmd_resize(command);
	if(strcmp(command->name, "rotate") == 0) return handler_cmd_rotate(command);
	if(strcmp(command->name, "save") == 0) return handler_cmd_save(command);
	if(strcmp(command->name, "switch_buffer") == 0) return handler_cmd_switch_buff(command);
	if(strcmp(command->name, "symmetry") == 0) return handler_cmd_symmetry(command);
	if(strcmp(command->name, "truncate") == 0) return handler_cmd_truncate(command);
	fprintf(stderr, "Critical Program error : existing command unrecognized \n");
	return 0;
}

/**
 * Loop on user command input and call parse function to build command structure and give it
 * to the command handler function
 *
 * @return n
 */


short cimple_handler(){
	int n=0;
	SDL_Event event;
	while(1){
		SDL_PollEvent(&event);
		char *cmd_line=getcmdline();
		if(cmd_line == NULL) continue;
		cmd *command=parse_line(string_cpy(cmd_line));
		if(command != NULL){
			n=cmd_function_handler(command);
			free(cmd_line);
			free_cmd(command);
		}
	}
	return n;
}
