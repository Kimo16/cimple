#ifndef M_COLOR_H
#define M_COLOR_H

#include <SDL2/SDL.h>

short negative_filter(image * img);
short black_and_white_filter(image * img);
short grey_filter(image * img);
short replace_color(image * img,SDL_Color origin_color,SDL_Color target_color,int margin);
short color_zone(image * img ,SDL_Color color,int x1,int x2,int y1,int y2);

#endif
