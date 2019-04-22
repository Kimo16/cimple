#ifndef EVENT_H
#define EVENT_H

#include <stdio.h>
#include <stdlib.h>
#include "window.h"

SDL_Rect get_select_array(void);
frame get_cursor_buffer(void);
int get_free_buffer(void);
void moveto_first_buffer(void);
void free_frame_buffer(int);
void free_frames(void);

#endif
