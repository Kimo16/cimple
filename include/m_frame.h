#ifndef M_FRAME_H
#define M_FRAME_H

#include <SDL2/SDL.h>
#include "m_image.h"

	short truncate_image(image * target , SDL_Rect zone;);
	short resize_workspace(image * target,int witdh , int height );
	short resize_image(image * target, int width, int height);


#endif
