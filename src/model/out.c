#include "out.h"


/**
 * @brief
 * Write line by line into jpg structure
 *
 * @param surface pixels to write
 * @param row pointer to one row
 * @param jpg the jpg structure in which writes
 */
static void export_pixels_jpg(SDL_Surface *surface, unsigned char *row, j_compress_ptr jpg){
	uint32_t *pixels = surface->pixels;
	SDL_Color c = {0};
	int       i, j;
	SDL_LockSurface(surface);
	for (i = 0; i < surface->h; i++) {
		for (j = 0; j < surface->w; j++) {
			SDL_GetRGBA(pixels[i * surface->w + j], surface->format, &c.r, &c.g, &c.b, &c.a);
			row[j * 3] = c.r;
			row[j * 3 + 1] = c.g;
			row[j * 3 + 2] = c.b;
		}
		jpeg_write_scanlines(jpg, &row, 1);
	}
}

/**
 * @brief
 * Save an image in BMP format
 *
 * @param img an img with a surface to save
 */
static short save_bmp(image *img){
	return !SDL_SaveBMP(
		get_img_surface(img),
		get_full_image_path(img)
		);
}

/**
 * @brief
 * Save an image in PNG format
 *
 * @param img the image with a surface to save
 */
static short save_png(image *img){
	SDL_Surface *surface = get_img_surface(img);
	char *       file = get_full_image_path(img);
	if (!IMG_SavePNG(surface, file))
		return 1;
	return 0;
}

/**
 * Save an image in JEPG format
 * @param img the image with a surface to save
 */
static short save_jpeg(image *img){
	FILE *       output;
	SDL_Surface *surface = get_img_surface(img);
	char *       file = get_full_image_path(img);
	struct jpeg_compress_struct jpg;
	struct jpeg_error_mgr       jpgerror;
	unsigned char *row;

	if ((output = fopen(file, "wb")) == NULL) {
		fprintf(stderr, "Can't write image...");
		return 1;
	}

	jpg.err = jpeg_std_error(&jpgerror);
	jpeg_create_compress(&jpg);

	// Setup image jpeg
	jpeg_stdio_dest(&jpg, output);
	jpg.image_width = surface->w;
	jpg.image_height = surface->h;
	jpg.input_components = 3;
	jpg.in_color_space = JCS_RGB;
	jpeg_set_defaults(&jpg);


	// Get format pixels
	row = malloc(sizeof(unsigned char) * 3 * surface->w);
	if (row == NULL)
		return 0;
	else {
		// Start compression
		jpeg_start_compress(&jpg, TRUE);
		export_pixels_jpg(surface, row, &jpg);
		jpeg_finish_compress(&jpg);
	}

	// Free
	if (!output) fclose(output);
	jpeg_destroy_compress(&jpg);
	if (!row) free(row);
	return 1;
}

/**
 * @brief
 * Save in a new path or new format
 *
 * @param img image from which takes surface
 * @param path new image path with name
 * @param format the image format
 */
image *save_image_as(image *img, char *path){
	if (img == NULL || path == NULL) return NULL;
	image *res = new_img(path);
	if (res == NULL) return NULL;
	if (!set_img_surface(res, get_img_surface(img))) {
		free_image(res);
		return NULL;
	}
	if (!save_image(res))
		return NULL;
	return res;
}

/**
 * @brief
 * Save an image defined in the right format
 *
 * @param img the image with all informations
 */
short save_image(image *img){
	if (memcmp("jpg", get_img_ext(img), 3) == 0)
		return save_jpeg(img);
	else if (memcmp("jpeg", get_img_ext(img), 4) == 0)
		return save_jpeg(img);
	else if (memcmp("JPEG", get_img_ext(img), 4) == 0)
		return save_jpeg(img);
	else if (memcmp("JPG", get_img_ext(img), 3) == 0)
		return save_jpeg(img);
	else if (memcmp("png", get_img_ext(img), 3) == 0)
		return save_png(img);
	else if (memcmp("PNG", get_img_ext(img), 3) == 0)
		return save_png(img);
	else if (memcmp("bmp", get_img_ext(img), 3) == 0)
		return save_bmp(img);
	return 0;
}

/**
 * @brief
 * Make a save in a tmp directory
 *
 * @param img the image to save
 */
short save_secure(image *img){
	char *path = "/tmp/cimpletmp/";
	char *save_name = malloc(strlen(path) + strlen(get_img_name(img)) + 4);
	sprintf("%s%s.bmp", path, get_img_name(img));
	if (save_image_as(img, save_name) == NULL)
		return 0;
	return 1;
}
