#ifndef M_TRANSFORM_H
#define M_TRANSFORM_H

#include <SDL2/SDL.h>
#include "m_image.h"

short symmetry(image * target,short vertical);
short rotate(image * target, int angle);
short copy(image *, SDL_Rect);
short cut(image *, SDL_Rect);
short past(image *, int x, int y);

#endif
