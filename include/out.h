#ifndef OUT_H
#define OUT_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <jpeglib.h>

#include"m_image.h"

short save_image_as(image* img,char * path , char* format);
short save_image(image* img);
short save_secure(image* img);

#endif
