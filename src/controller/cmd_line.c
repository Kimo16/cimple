#include "cmd_line.h"
static int current_window=-1;

/* Don't forget to add free rect in m_*.c file */
/*add test for correct coordinate (paste)*/
typedef struct frame{
	image * img ; 
}frame ;

static frame * frame_tab [10];


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

static SDL_Rect * build_rect(int x , int y , int w , int h ){
	SDL_Rect * rect = NULL ;
	rect -> x = x ; 
	rect -> y = y ; 
	rect -> w = w ; 
	rect -> h = h ; 
	return rect ; 
}

short handler_cmd_bnw(cmd *command){
	if(current_window == -1 || frame_tab[current_window] == NULL) return 0;
	int i = 0 ; 
	image *img=frame_tab[i]->img;
	SDL_Rect * rect = NULL;
	if(strcmp(command->args[1], "-a") == 0){
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );
	}
	else
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );    /*select zone in the windows*/
	return black_and_white_filter(img, *rect);
}

short handler_cmd_copy(cmd *command){
	if(current_window == -1 || frame_tab[current_window] == NULL) return 0;
	int i = 0 ; 
	image *img=frame_tab[i]->img;
	SDL_Rect * rect = NULL ;
	if(strcmp(command->args[1], "-a") == 0){
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );
	}
	else
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );   /*select zone in the windows*/
	return copy(img,*rect);
}

short handler_cmd_contrast(cmd *command){
	if(current_window == -1 || frame_tab[current_window] == NULL) return 0;
	int i = 0 ; 
	image *img=frame_tab[i]->img;
	int    percent=string_to_int(command->args[2]);
	SDL_Rect * rect = NULL ;
	if(strcmp(command->args[1], "-a") == 0){
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );
	}
	else
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );    /*select zone in the windows*/

	return contrast(img, *rect, percent);
}

short handler_cmd_cut(cmd *command){
	if(current_window == -1 || frame_tab[current_window] == NULL) return 0;
	int i = 0 ;
	image *img=frame_tab[i]->img;
	SDL_Rect * rect = NULL;
	if(strcmp(command->args[1], "-a") == 0){
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );
	}
	else
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );    /*select zone in the windows*/
	return negative_filter(img, *rect);
}

short handler_cmd_greyscale(cmd *command){
	if(current_window == -1 || frame_tab[current_window] == NULL) return 0;
	int i = 0 ;
	image *img=frame_tab[i]->img;
	SDL_Rect * rect = NULL ;
	if(strcmp(command->args[1], "-a") == 0){
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );
	}
	else
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );    /*select zone in the windows*/
	return grey_filter(img,*rect);
}

short handler_cmd_fill(cmd *command){
	if(current_window == -1 || frame_tab[current_window] == NULL) return 0;
	int i = 0 ;
	image *   img=frame_tab[i]->img;
	int       col_r=string_to_int(command->args[2]);
	int       col_g=string_to_int(command->args[3]);
	int       col_b=string_to_int(command->args[4]);
	int       col_a=string_to_int(command->args[5]);
	SDL_Rect  * rect = NULL ;
	SDL_Color color={col_r, col_g, col_b, col_a};
	if(strcmp(command->args[1], "-a") == 0){
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );
	}
	else
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );    /*select zone in the windows*/
	return color_zone(img, *rect, color);
}

short handler_cmd_ligth(cmd *command){
	if(current_window == -1 || frame_tab[current_window] == NULL) return 0;
	int i = 0; 
	image *img=frame_tab[i]->img;
	int    percent=string_to_int(command->args[2]);
	SDL_Rect * rect = NULL ;
	if(strcmp(command->args[1], "-a") == 0){
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );
	}
	else
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );   /*select zone in the windows*/

	return light_filter(img, *rect, percent);
}

short handler_cmd_list_buff(cmd *command){
	return 0;
}

short handler_cmd_load(cmd *command){
	return 0;
}

short handler_cmd_negative(cmd *command){
	if(current_window == -1 || frame_tab[current_window] == NULL) return 0;
	int i =  0  ;
	image *  img=frame_tab[i]->img;
	SDL_Rect * rect = NULL ;
	if(strcmp(command->args[1], "-a") == 0){
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );
	}
	else
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );   /*select zone in the windows*/
	return negative_filter(img, *rect);
}

short handler_cmd_paste(cmd *command){
	if(current_window == -1 || frame_tab[current_window] == NULL) return 0;
	int i = 0  ;
	image *  img=frame_tab[i]->img;
	if(strcmp(command->args[1], "-a") == 0){
		if(strcmp(command->args[2], "") != 0 && strcmp(command->args[3], "")){
			fprintf(stderr, "Error command[paste] : too much arguments \n");
			return 0;
		}
		return paste(img , 0 , 0 );
	}
	else{
		int x = string_to_int (command -> args[2]);
		int y = string_to_int (command -> args[3]);
		return paste(img, x , y);
	} /*select image zone*/

}

short handler_cmd_quit(cmd *command){
	return 0;
}

short handler_cmd_replace(cmd *command){
	if(current_window == -1 || frame_tab[current_window] == NULL) return 0;
	int i = 0 ;
	image *img=frame_tab[i]->img;
	int    percent=0;
	int    orig_r=string_to_int(command->args[4]);
	int    orig_g=string_to_int(command->args[5]);
	int    orig_b=string_to_int(command->args[6]);
	int    orig_a=string_to_int(command->args[7]);
	int    targ_r=string_to_int(command->args[8]);
	int    targ_g=string_to_int(command->args[9]);
	int    targ_b=string_to_int(command->args[10]);
	int    targ_a=string_to_int(command->args[11]);
	if(strcmp(command->args[1], "-m") == 0)
		percent=string_to_int(command->args[2]);
	SDL_Color origin_color={orig_r, orig_g, orig_b, orig_a};
	SDL_Color target_color={targ_r, targ_g, targ_b, targ_a};
	SDL_Rect * rect = NULL ;
	if(strcmp(command->args[3], "-a") == 0){
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );
	}
	else{
		rect=build_rect(0,0, img -> surface -> w , img -> surface -> h );            /*select zone in windows*/
	}
	return replace_color(img, *rect, origin_color, target_color,percent);
}

short handler_cmd_resize(cmd *command){
	if(current_window == -1 || frame_tab[current_window] == NULL) return 0;
	int i  = 0 ; 
	image *img=frame_tab[i]->img;
	int    width=string_to_int(command->args[2]);
	int    height=string_to_int(command->args[3]);
	if(strcmp(command->args[1], "workspace") == 0) return resize_workspace(img, width, height);
	else return resize_image(img, width, height);
}

short handler_cmd_rotate(cmd *command){
	if(current_window == -1 || frame_tab[current_window] == NULL) return 0;
	int i = 0 ; 
	image *img=frame_tab[i]->img;
	int   angle =string_to_int(command->args[2]);
	if(strcmp(command->args[1], "-r") == 0) return rotate(img, angle); /*add argument here after merge master*/
	return rotate(img, angle);   /*add argument here after merge master */
}

short handler_cmd_save(cmd *command){
	return 0;
}

short handler_cmd_switch_buff(cmd *command){
	return 0;
}

short handler_cmd_symmetry(cmd *command){
	if(current_window == -1 || frame_tab[current_window] == NULL) return 0;
	int i = 0;
	image *img=frame_tab[i]->img;
	int  mode =0;
	if(strcmp(command->args[1], "-v") == 0) mode=1;
	return symmetry(img, mode );
}

short handler_cmd_truncate(cmd *command){
	return 0;
}

static void cmd_function_handler(cmd *command){
	if(strcmp(command->name, "bnw") == 0) handler_cmd_bnw(command);
	if(strcmp(command->name, "copy") == 0) handler_cmd_copy(command);
	if(strcmp(command->name, "cut") == 0) handler_cmd_cut(command);
	if(strcmp(command->name, "contrast") == 0) handler_cmd_contrast(command);
	if(strcmp(command->name, "greyscale") == 0) handler_cmd_greyscale(command);
	if(strcmp(command->name, "fill") == 0) handler_cmd_fill(command);
	if(strcmp(command->name, "light") == 0) handler_cmd_ligth(command);
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
		if(command != NULL)
			cmd_function_handler(command);
	}
	return;
}
