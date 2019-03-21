#ifndef M_IMAGE_H
#define M_IMAGE_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct image image;
image *new_img(char *path);
char *get_img_name(image *img);
char *get_img_path(image *img);
char *get_img_ext(image *img);
SDL_Surface *get_img_surface(image *img);
short set_img_name(image *img, char *name);
short set_img_path(image *img, char *path);
short set_img_ext(image *img, char *ext);
short set_img_surface(image *img, SDL_Surface *texture);
void free_image(image *image);

#endif
