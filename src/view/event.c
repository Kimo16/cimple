#include "event.h"

static frame * frame_buffer[MAX_BUFFER];
static int cursor = -1;

// Return 1 if non empty
short non_empty(SDL_Rect rect) {
  return rect.x && rect.y && rect.h && rect.w;
}


// Draw selection
int draw_select(SDL_Rect selection) {
  frame *current = frame_buffer[cursor];

  SDL_Color select = {255, 0, 0, 0};
  SDL_Surface *surface = get_img_surface(current->image);
  SDL_Texture *texture = NULL;

  if(surface == NULL) {
    fprintf(stderr, "Can't get current surface");
    return 0;
  }

  if (SDL_RenderClear(current->renderer)) {
    fprintf(stderr, "Can't clear renderer");
    return 0;
  }

  if((texture = SDL_CreateTextureFromSurface(current->renderer, surface)) == NULL) {
    fprintf(stderr, "Can't convert surface into texture");
    return 0;
  }

  if(non_empty(selection)) {
    SDL_SetRenderTarget(current->renderer, texture);
    SDL_SetRenderDrawColor(current->renderer, select.r, select.g, select.b, select.a);
    SDL_RenderDrawRect(current->renderer, &selection);
    SDL_RenderCopy(current->renderer, texture, NULL, NULL);
    SDL_SetRenderTarget(current->renderer, NULL);
  }
  
  SDL_RenderPresent(current->renderer);
  
  return 1;
}


/**
 * @brief
 * Get point location
 */
SDL_Point get_point() {
  SDL_Event event;
  SDL_Point point;
  memset(&point, 0, sizeof(SDL_Rect));
  short run = 1;

  while(run) {
    SDL_WaitEvent(&event);

    // Key is pressed for leaving
    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) {
      point.x = -1;
      run = 0;
    }   
    else if (event.type == SDL_MOUSEBUTTONDOWN &&
             (event.window.windowID == SDL_GetWindowID(frame_buffer[cursor]->window))) {
      point.x = event.button.x;
      point.y = event.button.y;
      run = 0;
    }
  }
  return point;
}


SDL_Rect get_select_array() {
  SDL_Event event;
  SDL_Rect rect;
  memset(&rect, 0, sizeof(SDL_Rect));
  short run = 1;

  while(run) {
    SDL_WaitEvent(&event);

    // Key is pressed
    if(event.type == SDL_KEYDOWN) { 
      if(event.key.keysym.sym == SDLK_q) {
        memset(&rect, 0, sizeof(SDL_Rect));
        run = 0;
      } else if(non_empty(rect) && event.key.keysym.sym == SDLK_v) {
        run = 0;
      } else if (non_empty(rect) && event.key.keysym.sym == SDLK_c) {
        memset(&rect, 0, sizeof(SDL_Rect));
      }
      
      // Mouse is in motion
    }
    else if (event.type == SDL_MOUSEMOTION &&
             (event.motion.state & SDL_BUTTON_LEFT) &&
             (event.window.windowID == SDL_GetWindowID(frame_buffer[cursor]->window))) {
      rect.x = event.motion.x;
      rect.y = event.motion.y;
      rect.h = event.motion.xrel;
      rect.w = event.motion.yrel;
    }
    
    // Draw the selection
    if(!draw_select(rect)) {
      memset(&rect, 0, sizeof(SDL_Rect));
      run = 0;
    }
  }  
  return rect;
}

/**
 * Get the frame at buffer position
 * @return NULL in case of empty buffer
 */
frame *get_cursor_buffer(){
  if(cursor == -1)
    return NULL;
  return frame_buffer[cursor];
}


/**
 * Get first free buffer
 * @return the position of free buffer
 */
int get_free_buffer() {
  for (int i = 0 ; i < MAX_BUFFER ; i++) {
    if(frame_buffer[i] == NULL)
      return i;
  }
  return -1;
}


/**
 * Move to first non_empty buffer
 */
void moveto_first_buffer() {
  cursor = -1;
  for (int i = 0 ; i < MAX_BUFFER ; i++) {
    if(frame_buffer[i] != NULL)
      cursor = i;
  }
}


/**
 * Init a new frame at free position
 * @param path path to the image
 * @return 0 in case of failure
 */
int new_frame(char *path) {
  int pos = get_free_buffer();
  if(pos == -1) {
    fprintf(stderr, "Can't open more buffers. Max : %d\n", MAX_BUFFER);
    return 0;
  }
  
  frame *current = init_frame(path);
  if(current == NULL) {
    return 0;
  }
  frame_buffer[pos] = current;
  return 1;
}


/**
 * Delete a buffer and move cursor
 * to next non_empty position
 */
void free_frame_buffer(int i) {
  if (i > 0 && i < MAX_BUFFER && frame_buffer[i] != NULL) {
    free_frame(frame_buffer[i]);
    frame_buffer[i] = NULL;
  }
  moveto_first_buffer();
}


/**
 * Delete the content of the entiere buffer
 */
void free_frames() {
  for (int i = 0 ; i < MAX_BUFFER ; i++) 
    free_frame_buffer(i);
}
