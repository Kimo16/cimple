#include "out.h"

/**
 * @brief
 * Write line by line into jpg structure
 * 
 * @param surface pixels to write
 * @param row pointer to one row
 * @param jpg the jpg structure in which writes
 */
static void export_pixels_jpg(SDL_Surface *surface, unsigned char *row, j_compress_ptr jpg) {
  uint32_t *pixels = surface->pixels;
  SDL_Color c = {0};
  int i, j;
  SDL_LockSurface(surface);
  for(i=0 ; i < surface->h ; i++) {
    for(j=0 ; j < surface->w ; j++){
      SDL_GetRGBA(pixels[i*surface->w+j], surface->format, &c.r, &c.g, &c.b, &c.a);
      row[j * 3]     = c.r;
      row[j * 3 + 1] = c.g;
      row[j * 3 + 2] = c.b;
      jpeg_write_scanlines(jpg, &row, 1);
    }
  }
}


/**
 * @brief
 * Write line by line into a png structure
 * 
 * @param surface pixels to write
 * @param row pointer to one row
 * @param png structure in which writes
 */
static void export_pixels_png(SDL_Surface *surface, png_bytep row, png_structp png) {
  uint32_t *pixels = surface->pixels;
  SDL_Color c = {0};
  int i, j;
  SDL_LockSurface(surface);
  for(i=0 ; i < surface->h ; i++) {
    for(j=0 ; j < surface->w ; j++){
      SDL_GetRGBA(pixels[i*surface->w+j], surface->format, &c.r, &c.g, &c.b, &c.a);
      row[j * 4]     = c.r;
      row[j * 4 + 1] = c.g;
      row[j * 4 + 2] = c.b;
      row[j * 4 + 3] = c.a; 
      png_write_row(png, row);
    }
  }
}


/**
 * @brief
 * Save an image in BMP format
 * 
 * @param img an img with a surface to save
 */
static short save_bmp(image *img) {
  return SDL_SaveBMP(
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
static short save_png(image *img) {
  FILE * output;
  SDL_Surface * surface = get_img_surface(img);
  char * file = get_full_image_path(img);
  int err = 1;
  png_structp png = {0};
  png_infop info = {0};
  png_bytep row;

  if ((output = fopen(file, "wb")) == NULL) {
    fprintf(stderr, "Can't write image...\n");
    err = 0;
    return err;
  }

  // Init png 
  png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,NULL);
  if(!png) {
    fclose(output);
    err = 0;
    return err;
  }

  // Link info structure with the png one
  info = png_create_info_struct(png);
  if(!info){
      png_destroy_write_struct(&png, &info);
      fclose(output);
      err = 0;
      return err;
    }
  
  // Store calling environenemnt 
  if(setjmp(png_jmpbuf(png))) {
      if(!info) png_free_data(png, info, PNG_FREE_ALL, -1);
      if(!png) png_destroy_write_struct(&png, (png_infopp)NULL);
      fclose(output);
      err = -1;
      return err;
  }

  // Link output file with png struct
  png_init_io(png, output);

  // Define png header
  png_set_IHDR(
          png,
          info,
          surface->w,
          surface->h ,
          8,
          PNG_COLOR_TYPE_RGB_ALPHA,
          PNG_INTERLACE_NONE,
          PNG_COMPRESSION_TYPE_BASE,
           PNG_FILTER_TYPE_BASE // Load image line per line
  );

  // Write png
  row = (png_bytep) malloc(sizeof(png_byte) * 4 * surface->w);
  if(!row) {
    export_pixels_png(surface, row, png);
    png_write_end(png, NULL);
  }

  // Free
  if(!output) fclose(output);
  if(!info) png_free_data(png, info, PNG_FREE_ALL, -1);
  if(!png) png_destroy_write_struct(&png, (png_infopp)NULL);
  if(!row) free(row);
  return err;
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
  unsigned char* row;
  int err = 1;

  if((output = fopen(file, "wb")) == NULL) {
    fprintf(stderr, "Can't write image...");
    err = 0;
    return err;
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
  row = malloc(sizeof(unsigned char)*3*surface->w);
  if(row == NULL) {
    err = 0;
  } else {
    // Start compression
    jpeg_start_compress(&jpg, TRUE); 
    export_pixels_jpg(surface, row, &jpg);
    jpeg_finish_compress(&jpg);
  }

  // Free
  if(!output) fclose(output);
  jpeg_destroy_compress(&jpg);
  if(!row) free(row);
  return err;
}

/**
 * @brief
 * Save in a new path or new format
 * 
 * @param img image from which takes surface
 * @param path new image path with name
 * @param format the image format
 */
image *save_image_as(image* img, char * path , char* format) {
  if(img == NULL || path == NULL) return NULL;
  image *res = new_img(path);
  if(res == NULL) return NULL; 
  if(!set_img_surface(res, get_img_surface(img))) {
    free_image(res);
    return NULL;
  }
  if(format != NULL && !set_img_ext(res, format)) {
    free_image(res);
    return NULL;
  }
  if(!save_image(res)) {
    return NULL;
  }
  return res;
}

/**
 * @brief
 * Save an image defined in the right format
 *
 * @param img the image with all informations
 */
short save_image(image* img) {
  if(memcmp("jpg", get_img_ext(img), 3) == 0) {
      save_jpeg(img);
      return 1;
  } else if(memcmp("jpeg", get_img_ext(img), 4) == 0) {
      save_jpeg(img);
     return 1;
  } else if(memcmp("JPEG", get_img_ext(img), 4) == 0) {
      save_jpeg(img);
     return 1;
  } else if(memcmp("JPG", get_img_ext(img), 3) == 0) {
      save_jpeg(img);
     return 1;
  } else if(memcmp("png", get_img_ext(img), 3) == 0) {
      save_png(img);
      return 1;
  } else if(memcmp("PNG", get_img_ext(img), 3) == 0) {
      save_png(img);
      return 1;
  } else if(memcmp("bmp", get_img_ext(img),3) == 0) {
      save_bmp(img);
      return 1;
  }
  return 0;
}

/**
 * @brief
 * Make a save in a tmp directory
 * 
 * @param img the image to save
 */
short save_secure(image* img) {
  char *path = "/tmp/cimpletmp/";
  char *save_name = malloc(strlen(path) + strlen(get_img_name(img)) +1);
  sprintf("%s%s", path, get_img_name(img));
  if(save_image_as(img, save_name, "bmp") == NULL)
    return 0;
  return 1;
}
