#include "out.h"

static unsigned char * convert_pixels(SDL_Surface *surface) {
  int size = 3*sizeof(unsigned char) * surface->w * surface->h;
  unsigned char * cpy_pixels = malloc(size);
  if (cpy_pixels == NULL) {
    fprintf(stderr, "Can't malloc pixels array for conversion\n");
    return NULL;
  }
  uint32_t *pixels = surface->pixels;
  SDL_Color c = {0};
  int i, j;
  SDL_LockSurface(surface);
  for(i=0 ; i < surface->h ; i++)
    for(j=0 ; j < surface->w ; j++){
      SDL_GetRGBA(pixels[i*surface->w+j], surface->format, &c.r, &c.g, &c.b, &c.a);
      cpy_pixels[(i*surface->w+j)*3] = c.r;
      cpy_pixels[(i*surface->w+j)*3 + 1] = c.g;
      cpy_pixels[(i*surface->w+j)*3 + 2] = c.b;
  }
  return cpy_pixels;
}


/**
 * Save an image in BMP format
 * @param img an img with a surface to save
 */
static short save_bmp(image *img) {
  return SDL_SaveBMP(
                     get_img_surface(img),
                     get_full_image_path(img)
                     );
}

/**
 * Save an image in PNG format
 * @param img the image with a surface to save
 */
static short save_png(image *img) {
  return 0;
}

/**
 * Save an image in JEPG format
 * @param img the image with a surface to save
 */
static short save_jpeg(image *img) {
  FILE * output;
  SDL_Surface *surface = get_img_surface(img);
  char *file = get_full_image_path(img);
  struct jpeg_compress_struct jpg;
  struct jpeg_error_mgr jpgerror;
  
  jpg.err = jpeg_std_error(&jpgerror);
  jpeg_create_compress(&jpg);

  if((output = fopen(file, "wb")) == NULL) {
    fprintf(stderr, "Can't write image...");
    return 0;
  }

  // Setup image jpeg
  jpeg_stdio_dest(&jpg, output);
  jpg.image_width = surface->w;
  jpg.image_height = surface->h;
  jpg.input_components = 3;
  jpg.in_color_space = JCS_RGB;
  jpeg_set_defaults(&jpg);

  // Get format pixels
  unsigned char * pixels = convert_pixels(surface);
  if(pixels == NULL)
    return -1;

   // Start compression
  jpeg_start_compress(&jpg, TRUE); 
  JSAMPROW r_pointer[1]; // Point a line
  // Size of a row in pixels * color by pixel
  int row_length = jpg.image_width * 3;  
  while (jpg.next_scanline < jpg.image_height) {
    r_pointer[0] = & pixels[jpg.next_scanline * row_length];
    jpeg_write_scanlines(&jpg, r_pointer, 1);
  }
  jpeg_finish_compress(&jpg); // End compression
  free(pixels);
  return fclose(output);
}

short save_image_as(image* img, char * path , char* format) {
  return 0;
}

short save_image(image* img) {
  return -1;
}

short save_secure(image* img) {
  return -1;
}
