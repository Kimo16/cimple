#include "m_frame.h"

static int get_int_max(int a, int b){
	if(a>b) return a;
	return b;
}

static int get_int_min(int a, int b){
	if(a<b) return a;
	return b;
}

short truncate_image(SDL_Surface *img, int x1, int y1, int x2, int y2){
	// check if image is null
	if(img == NULL){
		fprintf(stderr, "Null image in truncate");
		return 0;
	}
	// checking if coords are in the box
	if(get_int_min(x1,x2)<0 || get_int_min(y1,y2)<0 || get_int_max(x1,x2)>surface->w || get_int_max(y1,y2)>surface->h){
		fprintf(stderr, "Wrong coordinated in truncate");
		return 0;
	}
}

short resize_workspace(SDL_Surface *img, int witdh, int height){
	if(img == NULL){
		fprintf(stderr, "Null image in truncate");
		return 0;
	}
	return 1;
}

short resize_image(SDL_Surface *img, int width, int height){
	return 0;
}
