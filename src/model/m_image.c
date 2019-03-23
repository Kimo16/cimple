#include "m_image.h"

struct image {
	char *       name;
	char *       save_path;
	char *       extension;
	SDL_Surface *img;
};

/**
 * Fills the struct image elements according to path
 *
 * @param init_path string containing the full path to the image (i.e. /toto/tata/image.png)
 * @param path pointer to write the path to (i.e. /toto/tata)
 * @param name pointer to write the image name
 * @param ext pointer to write the image extension
 * @return extension of the image
 */

static short break_full_path(char *init_path, char **path, char **name, char **ext){
	char *parsed_name=memrchr(init_path, '/', strlen(init_path) - 1) + 1;
	if(parsed_name == NULL){
		fprintf(stderr, "Name not set\n");
		return -1;
	}
	*name=parsed_name;
	char *parsed_ext=memrchr(parsed_name, '.', strlen(parsed_name));
	if(parsed_ext == NULL){
		fprintf(stderr, "Extension not set\n");
		return -1;
	}
	*parsed_ext='\0';
	*ext=parsed_ext + 1;
	char *parsed_path=malloc(parsed_name - init_path + 1);
	memcpy(parsed_path, init_path, parsed_name - init_path + 1);
	if(parsed_path == NULL){
		fprintf(stderr, "Path not set\n");
		return -1;
	}
	parsed_path[parsed_name - init_path]='\0';
	*path=parsed_path;
	return 0;
}

/**
 * Creates a brand new image
 *
 * @param path string containing the path to the image
 * @return new image
 */

image *new_img(char *path){
	// checking if it's a valid path
	if(path == NULL || !access(path, R_OK | W_OK)){
		fprintf(stderr, "Path not valid\n");
		return NULL;
	}
	// allocate memory
	image *new=malloc(sizeof(struct image));
	if(!break_full_path(path, &new->save_path, &new->name, &new->extension)){
		fprintf(stderr, "Image not initialized\n");
		free(new);
		return NULL;
	}
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
 * Return full path of the image
 *
 * @param img the image to work with
 * @return string with the full path (i.e. /toto/tata/imagr.png)
 */

char *get_full_image_path(image *image){
	int   size_fullpath=strlen(image->extension) + strlen(image->save_path) + strlen(image->name) + 2;
	char *fullpath=malloc(size_fullpath);
	snprintf(fullpath, size_fullpath, "%s%s.%s", image->save_path, image->name, image->extension);
	return fullpath;
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
	if(image != NULL){
		if(image->img != NULL) SDL_FreeSurface(image->img);
		if(image->extension != NULL) free(image->extension);
		if(image->save_path != NULL) free(image->save_path);
		if(image->name != NULL) free(image->name);
		free(image);
	}
}
