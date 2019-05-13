#ifndef EVENT_H
#define EVENT_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "window.h"
#include "m_image.h"

#define MAX_BUFFER 10

SDL_Rect get_select_array(void);
SDL_Point get_point(void);

frame *get_cursor_buffer(void);
int get_free_buffer(void);

void moveto_first_buffer(void);
int moveto_buffer(int pos);

int new_frame(char*);
void print_frame(void);
void check_current_frame(void);


void free_frame_buffer(int);
void free_frames(void);

#endif
