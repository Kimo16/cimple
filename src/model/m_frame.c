#include "m_frame.h"

/**
 * @brief
 * Returns the maximum of two numbers
 *
 * @param a first number
 * @param b second number
 * @return maximum of two integers
 */

static int get_int_max(int a, int b){
	if(a > b) return a;
	return b;
}

/**
 * @brief
 * Returns the minimum of two numbers
 *
 * @param a first number
 * @param b second number
 * @return minimum of two integers
 */

static int get_int_min(int a, int b){
	if(a < b) return a;
	return b;
}

/**
 * @brief
 * Truncates the image
 *
 * @param target image
 * @param x1 x coordinate of the first summit
 * @param y1 y coordinate of the first summit
 * @param x2 x coordinate of the second summit
 * @param y2 y coordinate of the second summit
 * @return 1 if succeded, 0 if failed
 */

short truncate_image(image *target, int x1, int y1, int x2, int y2){
	if(target == NULL){
		fprintf(stderr, "Null image in truncate");
		return 0;
	}
	SDL_Surface *surface=get_img_surface(target);
	// check if surface is null
	if(surface == NULL){
		fprintf(stderr, "Null surface in truncate");
		return 0;
	}
	int x_min=get_int_min(x1, x2);
	int x_max=get_int_max(x1, x2);
	int y_min=get_int_min(y1, y2);
	int y_max=get_int_max(y1, y2);
	// checking if coords are in the box
	if(x_min < 0 || y_min < 0 || x_max > surface->w || y_max > surface->h){
		fprintf(stderr, "Wrong coordinated in truncate");
		return 0;
	}
	int          width=x_max - x_min;
	int          height=y_max - y_min;
	SDL_Surface *new_surface;
	new_surface=SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, surface->format->format);
	if(SDL_MUSTLOCK(new_surface) == SDL_TRUE) SDL_LockSurface(new_surface);
	if(SDL_MUSTLOCK(surface) == SDL_TRUE) SDL_LockSurface(surface);
	Uint32 *pixels_ref=surface->pixels;
	Uint32 *pixels_new=new_surface->pixels;
	for(int i=0; i < height; i++)
		for(int j=0; j < width; j++){
			SDL_Color c_ref={0};
			SDL_GetRGBA(pixels_ref[(i + y_min) * width + (j + x_min)], new_surface->format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
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

/**
 * @brief
 * Resizes the workspace, width and height may be negative
 *
 * @param target image
 * @param width_p the value added to the width
 * @param height_p the value added to the height
 * @return 1 if succeded, 0 if failed
 */

short resize_workspace(image *target, int width_p, int height_p){
	if(target == NULL){
		fprintf(stderr, "Null image in truncate");
		return 0;
	}
	SDL_Surface *surface=get_img_surface(target);
	if(surface == NULL){
		fprintf(stderr, "Null surface in truncate");
		return 0;
	}
	int          width_new=surface->w + width_p;
	int          height_new=surface->h + height_p;
	SDL_Surface *new_surface;
	new_surface=SDL_CreateRGBSurfaceWithFormat(0, width_new, height_new, 32, surface->format->format);
	if(SDL_MUSTLOCK(new_surface) == SDL_TRUE) SDL_LockSurface(new_surface);
	if(SDL_MUSTLOCK(surface) == SDL_TRUE) SDL_LockSurface(surface);
	Uint32 *pixels_ref=surface->pixels;
	Uint32 *pixels_new=new_surface->pixels;
	for(int i=0; i < height_new; i++)
		for(int j=0; j < width_new; j++){
			Uint32 new_color;
			if(i > surface->h || j > surface->w)
				new_color=SDL_MapRGBA(surface->format, 0, 0, 0, 255);
			else{
				SDL_Color c_ref={0};
				SDL_GetRGBA(pixels_ref[i * surface->w + j], new_surface->format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
				new_color=SDL_MapRGBA(surface->format, c_ref.r, c_ref.g, c_ref.b, c_ref.a);
			}
			pixels_new[i * width_new + j]=new_color;
		}
	SDL_UnlockSurface(new_surface);
	SDL_UnlockSurface(surface);
	if(set_img_surface(target, new_surface) == 0){
		fprintf(stderr, "Surface not set");
		return 0;
	}
	return 1;
}

/**
 * @brief
 * Resizes the image
 *
 * @param target image
 * @param width new width
 * @param height new height
 * @return 1 if succeded, 0 if failed
 */


short resize_image(image *target, int width, int height){
	if(target == NULL){
		fprintf(stderr, "Null image in truncate");
		return 0;
	}
	SDL_Surface *surface=get_img_surface(target);
	if(surface == NULL){
		fprintf(stderr, "Null surface in truncate");
		return 0;
	}
	SDL_Surface *new_surface;
	new_surface=SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, surface->format->format);
	if(new_surface == NULL){
		fprintf(stderr, "New surface not created");
		return 0;
	}
	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
	if(SDL_BlitScaled(surface, NULL, new_surface, NULL) != 0){
		SDL_FreeSurface(new_surface);
		fprintf(stderr, "BlitScale failed");
		return 0;
	}
	if(set_img_surface(target, new_surface) == 0){
		fprintf(stderr, "Surface not set");
		return 0;
	}
	return 1;
}
