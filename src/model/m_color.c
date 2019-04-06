#include "m_color.h"

#include <math.h>


/**
 * Change all surface pixels with negative filter
 *
 * @param image * img , pointer to an image structure representing an image
 * @return 0 if changes failed , 1 if all changes done.
 */

short negative_filter(image * img){
	if(img == NULL){
		fprintf(stderr, "Error negative_filter(SDL_Surface * img) : Null argument \n");
		return 0;
	}
	SDL_Surface * surface = img -> img;
	SDL_Surface *neg_surface=SDL_CreateRGBSurfaceWithFormat(0, surface ->h, surface->w, 32, surface->format->format);
	if(neg_surface == NULL){
		fprintf(stderr, "SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
		return 0;
	}

	if(SDL_MUSTLOCK(neg_surface) == 1) SDL_LockSurface(neg_surface);
	if(SDL_MUSTLOCK(surface) == 1) SDL_LockSurface(surface);

	Uint32 * src_pixels = surface -> pixels ;
	Uint32 * dest_pixels = neg_surface -> pixels;
	
	for(int i=0; i < neg_surface -> h; i++)
		for(int j=0; j < neg_surface ->w; j++){
			SDL_Color c={0};
			SDL_GetRGBA(src_pixels[i * surface->w + j], neg_surface -> format, &c.r, &c.g, &c.b, &c.a);
			Uint32 c_neg = SDL_MapRGBA(neg_surface->format, 255 - c.r, 255 - c.g, 255 - c.b, c.a);
			dest_pixels[i * img->w + j]=c_neg;
		}

	SDL_UnlockSurface(neg_surface);
	SDL_UnlockSurface(surface);
	SDL_Surface *tmp=surface;
	set_img_surface(img, neg_surface);
	SDL_FreeSurface(tmp);
	return 1;
}

/**
 * Change all surface pixels with black and white filter
 *
 * @param image * img , pointer to an image structure representing an image
 * @return 0 if changes failed , 1 if all changes done.
 */

short black_and_white_filter(image *img){
	if(img == NULL){
		fprintf(stderr, "Error black_and_white_filter() : Null argument \n");
		return 0;
	}
	SDL_Surface * surface = img->img;
	SDL_Surface *bnw_surface=SDL_CreateRGBSurfaceWithFormat(0, surface->h, surface->w, 32, surface->format->format);
	if(bnw_surface == NULL){
		fprintf(stderr, "SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
		return 0;
	}

	if(SDL_MUSTLOCK(bnw_surface) == 1) SDL_LockSurface(bnw_surface);
	if(SDL_MUSTLOCK(surface) == 1) SDL_LockSurface(surface);

	Uint32 * src_pixels = surface -> pixels ;
	Uint32 * dest_pixels = bnw_surface -> pixels;

	for(int i=0; i < bnw_surface->h; i++)
		for(int j=0; j < bnw_surface->w; j++){
			SDL_Color c={0};
			SDL_GetRGBA(src_pixels[i * img->w + j], bnw_surface->format, &c.r, &c.g, &c.b, &c.a);
			int    gray_scale=(int)c.r * 0.2125 + c.g * 0.7154 + c.b * 0.0721;       /*CIE 709 recommandation for grayscale*/
			Uint32 c_bnw;
			if(gray_scale < 128)
				c_bnw=SDL_MapRGBA(bnw_surface->format, 0, 0, 0, c.a);
			else
				c_bnw=SDL_MapRGBA(bnw_surface->format, 255, 255, 255, c.a);
			dest_pixels[i * bnw_surface->w + j]=c_bnw;
		}

	SDL_UnlockSurface(bnw_surface);
	SDL_UnlockSurface(surface);
	SDL_Surface *tmp=surface;
	set_img_surface(img,bnw_surface);
	SDL_FreeSurface(tmp);
	return 1;
}

/**
 * Change all surface pixels with grey filter
 *
 * @param image * img , pointer to image structure representing an image
 * @return 0 if changes failed , 1 if all changes done.
 */

short grey_filter(image *img){
	if(img == NULL){
		fprintf(stderr, "Error grey_filter() : Null argument \n");
		return 0;
	}

	SDL_Surface * surface = img -> img;
	SDL_Surface *gray_surface=SDL_CreateRGBSurfaceWithFormat(0, surface->h, surface->w, 32, surface->format->format);
	if(gray_surface == NULL){
		fprintf(stderr, "SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
		return 0;
	}

	if(SDL_MUSTLOCK(gray_surface) == 1) SDL_LockSurface(gray_surface);
	if(SDL_MUSTLOCK(surface) == 1) SDL_LockSurface(surface);
	
	Uint32 * src_pixels = surface -> pixels ;
	Uint32 * dest_pixels = gray_surface -> pixels;
	
	for(int i=0; i < gray_surface->h; i++)
		for(int j=0; j < gray_surface->w; j++){
			SDL_Color c={0};
			SDL_GetRGBA(src_pixels[i * surface->w + j], gray_surface ->format, &c.r, &c.g, &c.b, &c.a);
			int    gray_scale=(int)((c.r * 0.2125) + (c.g * 0.7154) + (c.b * 0.0721));      /*CIE 709 recommandation for grayscale*/
			Uint32 c_gray=SDL_MapRGBA(gray_surface->format, gray_scale, gray_scale, gray_scale, c.a);
			dest_pixels[i * gray_surface->w + j]=c_gray;
		}

	SDL_UnlockSurface(gray_surface);
	SDL_UnlockSurface(surface);
	SDL_Surface *tmp=surface;
	set_img_surface(img,gray_surface);
	SDL_FreeSurface(tmp);
	return 1;
}

/**
 * calculates the percentage of distance between two colors
 * @param SDL_Color current_color , color to compare
 * @param SDL_Color origin_color , color to compare
 * @param int margin , margin percentage
 * @return short , 0 if the distance between the two colors is bigger than the percentage , 1 if the distance is acceptable
 */

static short margin_colors(SDL_Color current_color, SDL_Color origin_color, int margin){
	if(margin == 0) return 1;
	double delta_r=current_color.r - origin_color.r;
	double delta_g=current_color.g - origin_color.g;
	double delta_b=current_color.b - origin_color.b;
	double r_canal=(current_color.r + origin_color.r) / 2;
	double eucli_dist=sqrt(((2 + (r_canal / 256)) * delta_r * delta_r)
						   + (4 * (delta_g * delta_g))
						   + ((2 + ((255 - r_canal) / 256)) * delta_b * delta_b));
	double margin_pourcent=(eucli_dist / (sqrt((255 * 255) * 3))) * 100;
	if(margin_pourcent <= (double)margin) return 1;
	return 0;
}

/**
 * Change surface pixels color with an another (neighboring) color
 *
 * @param image * img , pointer to image structure representing an image
 * @param SDL_Color origin , represent the color to replace
 * @param SDL_Color target_color , represent the color to apply
 * @param int margin , represent the percentage of color proximity level
 * @return 0 if changes failed , 1 if all changes done.
 */

short replace_color(image *img, SDL_Color origin_color, SDL_Color target_color, int margin){
	if(img == NULL){
		fprintf(stderr, "Error replace_color() : Null argument \n");
		return 0;
	}
	SDL_Surface * surface = img -> img;
	SDL_Surface *repl_surface = SDL_CreateRGBSurfaceWithFormat(0, surface->h, surface->w, 32, surface->format->format);
	if(repl_surface == NULL){
		fprintf(stderr, "SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
		return 0;
	}

	if(SDL_MUSTLOCK(repl_surface) == SDL_TRUE) SDL_LockSurface(repl_surface);
	if(SDL_MUSTLOCK(surface) == SDL_TRUE) SDL_LockSurface(surface);

	Uint32 * src_pixels = surface -> pixels ;
	Uint32 * dest_pixels = repl_surface -> pixels;

	for(int i=0; i < repl_surface->h; i++)
		for(int j=0; j < repl_surface->w; j++){
			SDL_Color c={0};
			SDL_GetRGBA(src_pixels[i * surface->w + j], repl_surface->format, &c.r, &c.g, &c.b, &c.a);
			Uint32 repl_c;
			if(margin_colors(c, origin_color, margin) == 1)
				repl_c=SDL_MapRGBA(repl_surface ->format, target_color.r, target_color.g, target_color.b, target_color.a);
			else
				repl_c=src_pixels[i * surface->w + j];
			dest_pixels[i * repl_surface->w + j]=repl_c;
		}

	SDL_UnlockSurface(repl_surface);
	SDL_UnlockSurface(surface);
	SDL_Surface *tmp=surface;
	set_img_surface(img, repl_surface);
	SDL_FreeSurface(tmp);
	return 1;
}

static int min(int x, int y){
	return x < y ? x : y;
}

/**
 * Fill rectangle on image surface 
 *
 * @param img * img , pointer to image structure representing an image
 * @param SDL_color , represents the color to apply in the chosen area
 * @param int x1 , x2 , y1 , y2 , represent the area coordinates on the target surface
 * @return 0 if changes failed , 1 if all changes done.
 */
short color_zone(image *img, SDL_Color color, int x1, int x2, int y1, int y2){
	if(img == NULL){
		fprintf(stderr, "Error color_zone() : Null argument\n");
		return 0;
	}
	SDL_Surface * surface = img -> img;
	SDL_Surface *zone_surface=SDL_CreateRGBSurfaceWithFormat(0, surface->h, surface ->w, 32, surface->format->format);
	if(zone_surface == NULL){
		fprintf(stderr, "SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
		return 0;
	}
	if(SDL_BlitSurface(surface,NULL,zone_surface,NULL) != 0){
		fprintf(stderr, "SDL_BlitSurface() failed: %s", SDL_GetError());
		return 0;
	}

	if(SDL_MUSTLOCK(zone_surface) == 1) SDL_LockSurface(zone_surface);
	/*if(SDL_MUSTLOCK(img) == 1) SDL_LockSurface(surface);*/

	SDL_Rect rect = {min(x1,x2),min(y1,y2),abs(x2-x1),abs(y2-y1)};
	Uint32 r_color = SDL_MapRGBA (zone_surface -> format , color.r , color.g , color.b , color.a );
	SDL_FillRect (zone_surface , &rect , r_color);

	SDL_UnlockSurface(zone_surface);
	/*SDL_UnlockSurface(surface);*/
	SDL_Surface *tmp=surface;
	set_img_surface(img,zone_surface);
	SDL_FreeSurface(tmp);
	return 1;
}
