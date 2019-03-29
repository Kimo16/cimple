#include "m_transform.h"

short setPixel(SDL_Surface *surface, Uint8 r, Uint8 g, Uint8 b, Uint8 a, size_t x, size_t y){
    if(surface==NULL){
      fprintf(stderr, "setpixel error");
      return 0;
    }
    Uint32 *pixels = surface->pixels;
    if(pixels==NULL){
      fprintf(stderr, "setpixel error");
      return 0;
    }
    Uint32 couleur = SDL_MapRGBA(surface->format, r, g, b, a);
    pixels[y * surface->w + x] = couleur;
    return 1;
}

short symmetry(SDL_Surface* img,short vertical){
  if(img==NULL){
    fprintf(stderr, "Null argument in symmetry.");
    return 0;
  }
  if(vertical != 0 && vertical!= 1){
    fprintf(stderr, "Wrong [vertical] argument in symmetry.");
    return 0;
  }
  SDL_Surface* new_surface;
  new_surface = SDL_CreateRGBSurfaceWithFormat(0, img->w, img->h, 32, img->format->format);
  if (new_surface == NULL) {
    SDL_Log("SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
    exit(1);
  }
  SDL_LockSurface(new_surface);
  SDL_LockSurface(img);
  Uint32 *pixels_ref = img->pixels;
  int height=new_surface->h;
  int width = new_surface->w;
  // Vertical symmetry
  if(vertical==1){
    for(int i=0; i < height; i++)
  		for(int j=0; j < width; j++){
  			SDL_Color c_ref={0};
  			SDL_GetRGBA(pixels_ref[i * width + j], new_surface->format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
        int rc = setPixel(new_surface, c_ref.r, c_ref.g, c_ref.b, c_ref.a, i, width-j);
        if(rc==0){
          fprintf(stderr, "pixel not set");
          return 0;
        }
      }
  }
  // Horizontal symmetry
  if(vertical==0){
    for(int i=0; i < height; i++)
  		for(int j=0; j < width; j++){
  			SDL_Color c_ref={0};
  			SDL_GetRGBA(pixels_ref[i * width + j], new_surface->format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
        int rc = setPixel(new_surface, c_ref.r, c_ref.g, c_ref.b, c_ref.a, height - i, j);
        if(rc==0){
          fprintf(stderr, "pixel not set");
          return 0;
        }
      }
  }
  SDL_UnlockSurface(new_surface);
  SDL_UnlockSurface(img);
  SDL_Surface * tmp = img;
  img=new_surface;
  SDL_FreeSurface(tmp);
  return 1;
}

short rotate(SDL_Surface* img, int angle){
  return 1;
}
