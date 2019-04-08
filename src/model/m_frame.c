#include "m_frame.h"

static int get_int_max(int a, int b){
	if(a>b) return a;
	return b;
}

static int get_int_min(int a, int b){
	if(a<b) return a;
	return b;
}

short truncate_image(image * target, int x1, int y1, int x2, int y2){
	if(target==NULL){
		fprintf(stderr, "Null image in truncate");
		return 0;
	}
	SDL_Surface * surface = get_img_surface(target);
	// check if image is null
	if(surface == NULL){
		fprintf(stderr, "Null surface in truncate");
		return 0;
	}
	int x_min=get_int_min(x1,x2);
	int x_max=get_int_max(x1,x2);
	int y_min=get_int_min(y1,y2);
	int y_max=get_int_max(y1,y2);
	// checking if coords are in the box
	if(x_min<0 || y_min<0 || x_max>surface->w || y_max>surface->h){
		fprintf(stderr, "Wrong coordinated in truncate");
		return 0;
	}
	int width = x_max - x_min;
	int height = y_max - y_min;
	SDL_Surface * new_surface;
	new_surface=SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, surface->format->format);
	if(SDL_MUSTLOCK(new_surface) == SDL_TRUE) SDL_LockSurface(new_surface);
	if(SDL_MUSTLOCK(img) == SDL_TRUE) SDL_LockSurface(surface);
	Uint32 *pixels_ref=surface->pixels;
	Uint32 * pixels_new = new_surface->pixels;
	for(int i=0; i < height; i++)
		for(int j=0; j < width; j++){
			SDL_Color c_ref={0};
			SDL_GetRGBA(pixels_ref[(i+y_min) * width + (j+x_min)], new_surface->format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
			Uint32 new_color=SDL_MapRGBA(surface->format, c_ref.r, c_ref.g, c_ref.b, c_ref.a);
			pixels_new[i * width + j]=new_color;
		}
	SDL_UnlockSurface(new_surface);
	SDL_UnlockSurface(surface);
	if(set_img_surface(target, new_surface) == 0){
		fprintf(stderr, "Surface not set");
		return 0;
	}
	return 1;
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
