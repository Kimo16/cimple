#include "m_color.h"
#include <math.h>


/**
 * Change all surface pixels with negative filter
 *
 * @param SDL_surface * img , pointer to a surface representing an image
 * @return 0 if changes failed , 1 if all changes done.
 */

short negative_filter(SDL_Surface *img){
	if(img == NULL){
		fprintf(stderr, "Error negative_filter(SDL_Surface * img) : Null argument \n");
		return 0;
	}
	SDL_Surface *neg_img=SDL_CreateRGBSurfaceWithFormat(0, img->h, img->w, 32, img->format->format);
	if(neg_img == NULL){
			fprintf(stderr,"SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
			return 0;
	}

	if(SDL_MUSTLOCK(neg_img) == 1) SDL_LockSurface(neg_img);
	if(SDL_MUSTLOCK(img) == 1) SDL_LockSurface(img);

	Uint32 *pixel_tmp = img->pixels;
	Uint32 *pixel_neg_tmp = neg_img->pixels;

	for(int i=0; i < neg_img ->h; i++)
		for(int j=0; j < neg_img->w; j++){
			SDL_Color c={0};
			SDL_GetRGBA(pixel_tmp[i * img->w + j], neg_img->format, &c.r, &c.g, &c.b, &c.a);
			Uint32 c_neg=SDL_MapRGBA(neg_img->format, 255 - c.r, 255 - c.g, 255 - c.b, c.a);
			pixel_neg_tmp[i * img->w + j]=c_neg;
		}

	SDL_UnlockSurface(neg_img);
	SDL_UnlockSurface(img);
	SDL_Surface *tmp=img;
	img=neg_img;
	SDL_FreeSurface(tmp);
	return 1;
}

/**
 * Change all surface pixels with black and white filter
 *
 * @param SDL_surface * img , pointer to a surface representing an image
 * @return 0 if changes failed , 1 if all changes done.
 */

short black_and_white_filter(SDL_Surface *img){
	if(img == NULL){
		fprintf(stderr, "Error black_and_white_filter() : Null argument \n");
		return 0;
	}

	SDL_Surface *bnw_img=SDL_CreateRGBSurfaceWithFormat(0, img->h, img->w, 32, img->format->format);
	if(bnw_img == NULL){
			fprintf(stderr,"SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
			return 0;
	}

	if(SDL_MUSTLOCK(bnw_img) == 1 )SDL_LockSurface(bnw_img);
	if(SDL_MUSTLOCK(img) == 1 )SDL_LockSurface(img);
	
	Uint32 *pixel_tmp=img->pixels;
	Uint32 *bnw_pixel_tmp=bnw_img->pixels;

	for(int i=0; i < bnw_img->h; i++)
		for(int j=0; j < bnw_img->w; j++){
			SDL_Color c={0};
			SDL_GetRGBA(pixel_tmp[i * img->w + j], bnw_img->format, &c.r, &c.g, &c.b, &c.a);
			int    gray_scale=(int)c.r * 0.2125 + c.g * 0.7154 + c.b * 0.0721;   	 /*CIE 709 recommandation for grayscale*/
			Uint32 c_bnw;
			if(gray_scale < 128)
				c_bnw=SDL_MapRGBA(bnw_img->format, 0, 0, 0, c.a);
			else
				c_bnw=SDL_MapRGBA(bnw_img->format, 255, 255, 255, c.a);
			bnw_pixel_tmp[i * bnw_img->w + j]=c_bnw;
		}

	SDL_UnlockSurface(bnw_img);
	SDL_UnlockSurface(img);
	SDL_Surface *tmp=img;
	img=bnw_img;
	SDL_FreeSurface(tmp);
	return 1;
}

/**
 * Change all surface pixels with grey filter
 *
 * @param SDL_surface * img , pointer to a surface representing an image
 * @return 0 if changes failed , 1 if all changes done.
 */

short grey_filter(SDL_Surface *img){
	if(img == NULL){
		fprintf(stderr, "Error grey_filter() : Null argument \n");
		return 0;
	}

	SDL_Surface *gray_img=SDL_CreateRGBSurfaceWithFormat(0, img->h, img->w, 32, img->format->format);
	if(gray_img == NULL){
			fprintf(stderr,"SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
			return 0;
	}
	
	if(SDL_MUSTLOCK(gray_img) == 1) SDL_LockSurface(gray_img);
	if(SDL_MUSTLOCK(img) == 1) SDL_LockSurface(img);
	
	Uint32 *pixel_tmp=img->pixels;
	Uint32 *gray_pixel_tmp=img->pixels;

	for(int i=0; i < gray_img->h; i++)
		for(int j=0; j < gray_img->w; j++){
			SDL_Color c={0};
			SDL_GetRGBA(pixel_tmp[i * img->w + j], gray_img->format, &c.r, &c.g, &c.b, &c.a);
			int    gray_scale=(int)((c.r * 0.2125) + (c.g * 0.7154) + (c.b * 0.0721));      /*CIE 709 recommandation for grayscale*/
			Uint32 c_gray=SDL_MapRGBA(gray_img->format, gray_scale, gray_scale, gray_scale, c.a);
			gray_pixel_tmp[i * gray_img->w + j]=c_gray;
		}

	SDL_UnlockSurface(gray_img);
	SDL_UnlockSurface(img);
	SDL_Surface *tmp=img;
	img=gray_img;
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
	if(margin == 0 ) return 1;
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
 * @param SDL_surface * img , pointer to a surface representing an image
 * @param SDL_Color origin , represent the color to replace 
 * @param SDL_Color target_color , represent the color to apply
 * @param int margin , represent the percentage of color proximity level
 * @return 0 if changes failed , 1 if all changes done.
 */

short replace_color(SDL_Surface *img, SDL_Color origin_color, SDL_Color target_color, int margin){
	if(img == NULL){
		fprintf(stderr, "Error replace_color() : Null argument \n");
		return 0;
	}

	SDL_Surface *repl_img=SDL_CreateRGBSurfaceWithFormat(0, img->h, img->w, 32, img->format->format);
	if(repl_img == NULL){
			fprintf(stderr,"SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
			return 0;
	}

	if(SDL_MUSTLOCK(repl_img) == SDL_TRUE) SDL_LockSurface(repl_img);
	if(SDL_MUSTLOCK(img) == SDL_TRUE) SDL_LockSurface(img);
	
	Uint32 *pixel_tmp=img->pixels;
	Uint32 *repl_pixel_tmp=img->pixels;

	for(int i=0; i < repl_img->h; i++)
		for(int j=0; j < repl_img->w; j++){
			SDL_Color current_color={0};
			SDL_GetRGBA(pixel_tmp[i * img->w + j], repl_img->format, &current_color.r, &current_color.g, &current_color.b, &current_color.a);
			Uint32 repl_c;
			if(margin_colors(current_color, origin_color, margin) == 1)
				repl_c=SDL_MapRGBA(repl_img->format, target_color.r, target_color.g, target_color.b, target_color.a);
			else
				repl_c=pixel_tmp[i * img->w + j];
			repl_pixel_tmp[i * repl_img->w + j]=repl_c;
		}

	SDL_UnlockSurface(repl_img);
	SDL_UnlockSurface(img);
	SDL_Surface *tmp=img;
	img=repl_img;
	SDL_FreeSurface(tmp);
	return 1;
}


static int max(int x, int y){
	return x > y ? x : y;
}

static int min(int x, int y){
	return x < y ? x : y;
}

/**
 * Change all surface pixels with negative filter
 * 
 * @param SDL_surface * img , pointer to a surface representing an image
 * @param SDL_color , represents the color to apply in the chosen area 
 * @param int x1 , x2 , y1 , y2 , represent the area coordinates on the target surface
 * @return 0 if changes failed , 1 if all changes done.
 */
short color_zone(SDL_Surface *img, SDL_Color color, int x1, int x2, int y1, int y2){
	if(img == NULL ){
		fprintf(stderr, "Error color_zone() : Null argument\n");
		return 0;
	}

	SDL_Surface *zone_img=SDL_CreateRGBSurfaceWithFormat(0, img->h, img->w, 32, img->format->format);
	if(zone_img == NULL){
			fprintf(stderr,"SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
			return 0;
	}

	if(SDL_MUSTLOCK(zone_img) == 1)SDL_LockSurface(zone_img);
	if(SDL_MUSTLOCK(img) == 1 ) SDL_LockSurface(img);
	
	Uint32 *pixel_tmp=img->pixels;
	Uint32 *zone_pixel_tmp=img->pixels;

	for(int i=0; i < zone_img->h; i++)
		for(int j=0; j < zone_img->w; j++){
			SDL_Color color_c={0};
			Uint32    c_zone;
			if(i >= min(y1, y2) && i <= max(y1, y2) && j >= min(x1, x2) && j <= max(x1, x2))
				c_zone=SDL_MapRGBA(zone_img->format, color_c.r, color_c.g, color_c.b, color_c.a);
			else
				c_zone=pixel_tmp[i * img->w + j];
			zone_pixel_tmp[i * zone_img->w + j]=c_zone;
		}

	SDL_UnlockSurface(zone_img);
	SDL_UnlockSurface(img);
	SDL_Surface *tmp=img;
	img=zone_img;
	SDL_FreeSurface(tmp);
	return 1;
}
