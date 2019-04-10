#ifndef M_FRAME_H
#define M_FRAME_H

#include <SDL2/SDL.h>
#include "m_image.h"
#include "resize.h"

	short truncate_image(image * target , int x1 , int y1 , int x2 ,int y2);
	short resize_workspace(image * target,int witdh , int height );
	short resize_image(image * target, int width, int height);


#endif
