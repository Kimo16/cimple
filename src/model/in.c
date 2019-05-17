 #include "in.h"

/**
 * Allow to load an image referenced by
 * [path]
 */
image *load_image(char *path){
	image *img = new_img(path);
	if (img == NULL) {
		fprintf(stderr, "Error : wrong image path.\n");
		return NULL;
	}
	SDL_Surface *tmp = IMG_Load(path);
	if (tmp == NULL) {
		fprintf(stderr, "Error : can't load image.\n");
		free_image(img);
		return NULL;
	}
	SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if (format == NULL) {
		fprintf(stderr, "Error : can't allocate format.\n");
		SDL_FreeSurface(tmp);
		free_image(img);
		return NULL;
	}
	SDL_Surface *copy = SDL_ConvertSurface(tmp, format, 0);
	SDL_FreeSurface(tmp);
	if (copy == NULL) {
		fprintf(stderr, "Error : can't convert image.\n");
		SDL_FreeFormat(format);
		return NULL;
	}
	set_img_surface(img, copy);
	SDL_FreeFormat(format);
	return img;
}
