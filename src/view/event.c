#include "event.h"

static frame frame_buffer[MAX_BUFFER];
static int cursor = 0;

typedef enum {ADD, DEL, QUIT} State

void draw_select(SDL_Rect selection) {
  // Draw a rect on frame
}

// Get event of selection
SDL_Rect get_select_array() {
  SDL_Event event = NULL;
  SDL_Rect rect = NULL;
  State state = ADD;
  while(state != QUIT) {

    SDL_WaitEvent(&event);
    
    if(event.type == SDL_KEYDOWN) { 
      if(event.key.keysym.sym == SDLK_c)
        rect = NULL;
      if(event.key.keysym.sym == SDLK_a)
        state = ADD;
      if(event.key.keysym.sym == SDLK_d)
        state = DEL;
      if(event.key.key.sym == SDLK_q) {
        state = QUIT;
        
      }
    }
    
    if (event.windowID == SDL_GetWindowID(frame_buffer[cursor].window)) {

      if(state == ADD && event.type == SDL_MOUSEMOTION &&
         (event.motion.state & SDL_BUTTON_LEFT)) {
        rect = { .x = event.motion.x,
                 .y = event.motion.y,
                 .h = event.motion.xrel,
                 .w = event.motion.yrel
        }; 
      } else {continue;}
      
      if(state == DEL && event.type == MOUSEBUTTONDOWN) {
        rect = NULL; // Replace by a linkedList in future
      }
    }
    //draw_select();
  }
  return rect;
}

// Return the cursor buffer or -1
frame get_cursor_buffer(){
  if(cursor == -1)
    return NULL;
  else
    return frame[cursor];
}

// Get first free buffer
int get_free_buffer() {
  for (int i = 0 ; i < MAX_BUFFER ; i++) {
    if(frame_buffer[i] == NULL)
      return i;
  }
  return -1;
}

// Move to first non_empty buffer
void moveto_first_buffer() {
  for (int i = 0 ; i < MAX_BUFFER ; i++) {
    if(frame_buffer[i] != NULL)
      cursor = i;
  }
}

// Delete a buffer and update cursor
void free_frame_buffer(int i) {
  if (i > 0 && i < MAX_BUFFER && frame_buffer[i] != NULL)
    free_frame(frame_buffer[i]);
  get_cursor_buffer();
}

// Delete frames
void free_frames() {
  for (int i = 0 ; i < MAX_BUFFER ; i++) 
    free_frame_buffer(i);

}
