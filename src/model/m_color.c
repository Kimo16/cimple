#include "m_color.h"


short negative_filter(SDL_Surface* img){
	if( img == NULL){
		fprintf(stderr, "Error negative_filter(SDL_Surface * img) : Null argument \n");
		return 0;
	}
	SDL_Surface * neg_img = SDL_CreateRGBSurfaceWithFormat(0, img -> h , img -> w , 32 , img -> format -> format );
	SDL_LockSurface(neg_img);
	SDL_LockSurface(img);
	Uint32 *pixel_tmp = img -> pixels; 

	for (int i = 0 ; i < img -> h ; i++ ){
		for(int j = 0 ; j < img -> w ; j++){
			SDL_Color c = {0};
			SDL_GetRGBA(pixel_tmp[ i * img -> w + j ] , neg_img -> format , &c.r , &c.g , &c.b , &c.a );
			SDL_Color c_neg = SDL_MapRGBA( neg_img -> format , 255-c.r, 255-c.g , 255-c.b, c.a );
			neg_img -> pixels[ i * img -> w + j ] = c_neg ; 
		}
	}
	SDL_UnlockSurface(neg_img);
	SDL_UnlockSurface(img);
	SDL_Surface * tmp = img;
	img = neg_img;
	SDL_FreeSurface(tmp);

	return 1 ; 
}


short black_and_white_filter(SDL_Surface* img){
	if( img == NULL){
		fprintf(stderr, "Error black_and_white_filter(SDL_Surface * img) : Null argument \n");
		return 0;
	}
	int median = 128 ;
	SDL_Surface * bnw_img = SDL_CreateRGBSurfaceWithFormat(0, img -> h , img -> w , 32 , img -> format -> format );
	SDL_LockSurface(bnw_img);
	SDL_LockSurface(img);
	Uint32 *pixel_tmp = img -> pixels; 

	for (int i = 0 ; i < img -> h ; i++ ){
		for(int j = 0 ; j < img -> w ; j++){
			SDL_Color c = {0};
			SDL_GetRGBA(pixel_tmp[ i * img -> w + j ] , bnw_img -> format , &c.r , &c.g , &c.b , &c.a );
			int gray_scale = (int) c.r * 0.2125 + c.g * 0.7154 + c.b * 0.0721;    /*CIE 709 recommandation for grayscale*/
			SDL_Color c_bnw ; 
			if(gray_scale < median )
				c_bnw= SDL_MapRGBA( bnw_img -> format , 0 , 0 , 0 , c.a );
			else 
				c_bnw = SDL_MapRGBA( bnw_img -> format , 255 , 255 , 255 , c.a);
			bnw_img -> pixels[ i * img -> w + j ] = c_bnw ; 
		}
	}
	SDL_UnlockSurface(bnw_img);
	SDL_UnlockSurface(img);
	SDL_Surface * tmp = img;
	img = bnw_img;
	SDL_FreeSurface(tmp);

	return 1 ; 
}


short grey_filter(SDL_Surface* img,int level){
	if( img == NULL){
		fprintf(stderr, "Error black_and_white_filter(SDL_Surface * img) : Null argument \n");
		return 0;
	}
	int median = 128 ;
	SDL_Surface * gray_img = SDL_CreateRGBSurfaceWithFormat(0, img -> h , img -> w , 32 , img -> format -> format );
	SDL_LockSurface(gray_img);
	SDL_LockSurface(img);
	Uint32 *pixel_tmp = img -> pixels; 

	for (int i = 0 ; i < img -> h ; i++ ){
		for(int j = 0 ; j < img -> w ; j++){
			SDL_Color c = {0};
			SDL_GetRGBA(pixel_tmp[ i * img -> w + j ] , gray_img -> format , &c.r , &c.g , &c.b , &c.a );
			int gray_scale = (int) ( (c.r * 0.2125) + (c.g * 0.7154) + (c.b * 0.0721) );    /*CIE 709 recommandation for grayscale*/
			SDL_Color c_gray ; 
			c_gray= SDL_MapRGBA( gray_img -> format , gray_scale , gray_scale , gray_scale , c.a );
			gray_img -> pixels[ i * img -> w + j ] = c_gray ; 
		}
	}
	SDL_UnlockSurface(gray_img);
	SDL_UnlockSurface(img);
	SDL_Surface * tmp = img;
	img = gray_img;
	SDL_FreeSurface(tmp);

	return 1 ;


}

/*euclidian distance between two colors */

short margin_colors(SDL_Color origin_color,SDL_Color target_color , int margin ){
	return 0;
}

short replace_color(SDL_Surface* img,SDL_Color origin_color,SDL_Color target_color,int margin){
	return 1

}



short color_zone(SDL_Surface* img ,SDL_Color color,int x1,int x2,int y1,int y2){
	return 1 ; 

}