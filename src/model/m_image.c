#include "m_image.h"

struct image {
	char *       name;
	char *       save_path;
	char *       extension;
	SDL_Surface *img;
};

/**
 * Gets the image extension from the path
 *
 * @param path string containing the path to the image
 * @return extension of the image
 */

static char *get_extension_from_path(char *path){
	char *ext=strrchr(path, '.') + 1;
	if(ext == NULL){
		perror("ext");
		exit(1);
	}
	return ext;
}

/**
 * Creates a brand new image
 *
 * @param path string containing the path to the image
 * @return new image
 */

image *new_img(char *path){
	// if no parameter is passed
	if(path == NULL)
		printf("Please add an argument.\n");
	// checking if it's a valid path by trying to open the file
	FILE *file=fopen(path, "r");
	if(file == NULL)
		printf("Please enter a valid path.\n");
	// allocate memory
	image *new=malloc(sizeof(struct image));
	new->name=NULL;
	new->save_path=path;
	new->extension=get_extension_from_path(path);
	new->img=NULL;
	return new;
}

/**
 * Gets the image name
 *
 * @param img the image to work with
 * @return image name
 */

char *get_img_name(image *img){
	if(img->name == NULL){
		perror("image name");
		exit(1);
	}
	return img->name;
}

/**
 * Gets the image path
 *
 * @param img the image to work with
 * @return image path
 */

char *get_img_path(image *img){
	if(img->save_path == NULL){
		perror("image path");
		exit(1);
	}
	return img->save_path;
}

/**
 * Gets the image extension
 *
 * @param img the image to work with
 * @return image extension
 */

char *get_img_ext(image *img){
	if(img->extension == NULL){
		perror("image ext");
		exit(1);
	}
	return img->extension;
}

/**
 * Gets the surface of the image
 *
 * @param img the image to work with
 * @return SDL_Surface element of the structure
 */

SDL_Surface *get_img_surface(image *img){
	if(img->img == NULL){
		perror("image ext");
		exit(1);
	}
	return img->img;
}

/**
 * Sets a new image name
 *
 * @param img the image to work with
 * @param name new name
 * @return <0 if failed, >=0 if succeed
 */

short set_img_name(image *img, char *name){
	img->name=name;
	return img->name != NULL;
}

/**
 * Sets the image path
 *
 * @param img the image to work with
 * @param name new path
 * @return <0 if failed, >=0 if succeed
 */

short set_img_path(image *img, char *path){
	img->save_path=path;
	return img->save_path != NULL;
}

/**
 * Sets a new extension for the image
 *
 * @param img the image to work with
 * @param name new extension
 * @return <0 if failed, >=0 if succeed
 */

short set_img_ext(image *img, char *ext){
	img->extension=ext;
	return img->extension != NULL;
}

/**
 * Sets a new surface for the image
 *
 * @param img the image to work with
 * @param name new SDL_Surface
 * @return <0 if failed, >=0 if succeed
 */

short set_img_surface(image *img, SDL_Surface *surface){
	img->img=surface;
	return img->img != NULL;
}

/**
 * Frees the image structure
 *
 * @param img the image free
 */

void free_image(image *image){
	SDL_FreeSurface(image->img);
	free(image->extension);
	free(image->save_path);
	free(image->name);
	free(image);
}
