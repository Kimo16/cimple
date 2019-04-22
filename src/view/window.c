#include "window.h"

frame *init_frame(char *path){
	frame *new_frame=malloc(sizeof(frame));
	if(new_frame == NULL){
		fprintf(stderr, "Frame not initialized");
		return NULL;
	}
	new_frame->image=load_image(path);
	if(new_frame->image == NULL){
		fprintf(stderr, "Image could not be set");
		free_frame(new_frame);
		return NULL;
	}
	SDL_Surface *surface=get_img_surface(new_frame->image);
	if(surface == NULL){
		fprintf(stderr, "Cannot get surface");
		free_frame(new_frame);
		return NULL;
	}
	new_frame->window=SDL_CreateWindow(get_img_name(new_frame->image), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, surface->w, surface->h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(new_frame->window == NULL){
		fprintf(stderr, "Cannot create window");
		free_frame(new_frame);
		return NULL;
	}
	new_frame->renderer=SDL_CreateRenderer(new_frame->window, -1, SDL_RENDERER_ACCELERATED);  // trying to use hardware acceleration for the renderer
	if(new_frame->renderer == NULL){
		new_frame->renderer=SDL_CreateRenderer(new_frame->window, -1, SDL_RENDERER_SOFTWARE); // the renderer is a software fallback
		if(new_frame->renderer == NULL){
			fprintf(stderr, "Renderer not initialized");
			free_frame(new_frame);
			return NULL;
		}
	}
	SDL_Texture  * texture=SDL_CreateTextureFromSurface(new_frame->renderer, surface);
	if(texture == NULL){
		fprintf(stderr, "Error SDL_CreateTextureFromSurface : %s", SDL_GetError());
		free_frame(new_frame);
		return NULL;
	}
	int rc=SDL_RenderCopy(new_frame->renderer, texture, NULL, NULL);
	if(rc < 0){
		fprintf(stderr, "Render error");
		SDL_DestroyTexture(texture);
		free_frame(new_frame);
		return NULL;
	}
  SDL_SetRenderTarget(new_frame->renderer,NULL);
  SDL_DestroyTexture(texture);
	return new_frame;
}

short update_frame(frame *target){
	SDL_Surface * surface = get_img_surface(target->image);
	SDL_Texture *new_texture=SDL_CreateTextureFromSurface(target->renderer, surface);
	if(new_texture == NULL){
		fprintf(stderr, "Error updating texture");
		free_frame(target);
		return 0;
	}
	SDL_SetWindowSize(target->window, surface->w, surface->h);
	int rc=SDL_RenderCopy(target->renderer, new_texture, NULL, NULL);
	if(rc < 0){
		fprintf(stderr, "Render error");
		SDL_DestroyTexture(new_texture);
		free_frame(target);
		return 0;
	}
  SDL_DestroyTexture(new_texture);
	return 1;
}

void free_frame(frame *target){
	if(target->image != NULL) free_image(target->image);
	if(target->window != NULL) SDL_DestroyWindow(target->window);
	if(target->renderer != NULL) SDL_DestroyRenderer(target->renderer);
	free(target);
}
