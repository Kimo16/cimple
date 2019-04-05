#include "m_transform.h"

/**
 *
 * Colors the (x,y) pixel with the specific color
 *
 * @param surface target surface
 * @param color color
 * @param x coord
 * @param y coord
 * @return 1 if success, 0 if failed
 */

static short setPixel(SDL_Surface *surface, SDL_Color color, size_t x, size_t y){
	if(surface == NULL){
		fprintf(stderr, "setpixel error");
		return 0;
	}
	Uint32 *pixels=surface->pixels;
	if(pixels == NULL){
		fprintf(stderr, "setpixel error");
		return 0;
	}
	Uint32 new_color=SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
	pixels[y * surface->w + x]=new_color;
	return 1;
}

/**
 * Applies vertical or horizontal symmetry on a surface
 *
 * @param img target surface, gets replaced by a new one
 * @param vertical 1 for vertical symmetry, 0 for horizontal symmetry
 * @return 1 if success, 0 if failed
 */

short symmetry(SDL_Surface *img, short vertical){
	if(img == NULL){
		fprintf(stderr, "Null argument in symmetry.");
		return 0;
	}
	if(vertical != 0 && vertical != 1){
		fprintf(stderr, "Wrong [vertical] argument in symmetry.");
		return 0;
	}
	SDL_Surface *new_surface;
	new_surface=SDL_CreateRGBSurfaceWithFormat(0, img->w, img->h, 32, img->format->format);
	if(new_surface == NULL){
		SDL_Log("SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
		exit(1);
	}
	SDL_LockSurface(new_surface);
	SDL_LockSurface(img);
	Uint32 *pixels_ref=img->pixels;
	int     height=new_surface->h;
	int     width=new_surface->w;
	// Vertical symmetry
	if(vertical == 1)
		for(int i=0; i < height; i++)
			for(int j=0; j < width; j++){
				SDL_Color c_ref={0};
				SDL_GetRGBA(pixels_ref[i * width + j], new_surface->format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
				int rc=setPixel(new_surface, c_ref, i, width - j);
				if(rc == 0){
					SDL_UnlockSurface(new_surface);
					SDL_UnlockSurface(img);
					SDL_FreeSurface(new_surface);
					fprintf(stderr, "pixel not set");
					return 0;
				}
			}
	// Horizontal symmetry
	if(vertical == 0)
		for(int i=0; i < height; i++)
			for(int j=0; j < width; j++){
				SDL_Color c_ref={0};
				SDL_GetRGBA(pixels_ref[i * width + j], new_surface->format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
				int rc=setPixel(new_surface, c_ref, height - i, j);
				if(rc == 0){
					SDL_UnlockSurface(new_surface);
					SDL_UnlockSurface(img);
					SDL_FreeSurface(new_surface);
					fprintf(stderr, "pixel not set");
					return 0;
				}
			}
	SDL_UnlockSurface(new_surface);
	SDL_UnlockSurface(img);
	SDL_Surface *tmp=img;
	img=new_surface;
	SDL_FreeSurface(tmp);
	return 1;
}

/**
 * Rotates the image clockwise
 *
 * @param img target surface, gets replaced by a new one
 * @param angle a positive miltiple of 90,the angle to rotate the surface
 * @return 1 if success, 0 if failed, -1 if surface wasn't created
 */

short rotate(SDL_Surface *img, int angle){
	if(img == NULL){
		fprintf(stderr, "Null image in rotate");
		return 0;
	}
	SDL_Surface *new_surface;
	// if image is not changed (i.e. 360 degrees)
	if((angle / 90) % 4 == 0)
		return 1;
	// create a surface to be filled
	new_surface=SDL_CreateRGBSurfaceWithFormat(0, img->h, img->w, 32, img->format->format);
	if(new_surface == NULL){
		SDL_Log("SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
		return -1;
	}
	SDL_LockSurface(new_surface);
	SDL_LockSurface(img);
	Uint32 *pixels_ref=img->pixels;
	// when image is turned once clockwise
	if((angle / 90) % 4 == 1){
		int     width=new_surface->w;
		for(int i=0; i < img->h; i++)
			for(int j=0; j < img->w; j++){
				SDL_Color c_ref={0};
				SDL_GetRGBA(pixels_ref[i * img->w + j], new_surface->format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
				int rc=setPixel(new_surface, c_ref, j, width - i);
				if(rc == 0){
					SDL_UnlockSurface(new_surface);
					SDL_UnlockSurface(img);
					SDL_FreeSurface(new_surface);
					fprintf(stderr, "pixel not set");
					return 0;
				}
			}
	}
	// when image is turned twice clockwise
	if((angle / 90) % 4 == 2){
		int     height=new_surface->h;
		int     width=new_surface->w;
		for(int i=0; i < img->h; i++)
			for(int j=0; j < img->w; j++){
				SDL_Color c_ref={0};
				SDL_GetRGBA(pixels_ref[i * img->w + j], new_surface->format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
				int rc=setPixel(new_surface, c_ref, height - i, width - j);
				if(rc == 0){
					SDL_UnlockSurface(new_surface);
					SDL_UnlockSurface(img);
					SDL_FreeSurface(new_surface);
					fprintf(stderr, "pixel not set");
					return 0;
				}
			}
	}
	// when image is turned three times clockwise
	if((angle / 90) % 4 == 3){
		int     height=new_surface->h;
		for(int i=0; i < img->h; i++)
			for(int j=0; j < img->w; j++){
				SDL_Color c_ref={0};
				SDL_GetRGBA(pixels_ref[i * img->w + j], new_surface->format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
				int rc=setPixel(new_surface, c_ref, height - j, i);
				if(rc == 0){
					SDL_UnlockSurface(new_surface);
					SDL_UnlockSurface(img);
					SDL_FreeSurface(new_surface);
					fprintf(stderr, "pixel not set");
					return 0;
				}
			}
	}
	SDL_UnlockSurface(new_surface);
	SDL_UnlockSurface(img);
	SDL_Surface *tmp=img;
	img=new_surface;
	SDL_FreeSurface(tmp);
	return 1;
}
