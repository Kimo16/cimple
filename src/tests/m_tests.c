#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "m_color.h"
#include "in.h"
#include "m_image.h"
#include "m_transform.h"
#include "m_frame.h"


//    -lcmocka -I/usr/include/SDL2 -D_REENTRANT -L/usr/lib -pthread -lSDL2 -lSDL2_image

static void compare_textures(SDL_Texture *test_texture, SDL_Texture *ref_texture, int diff){
	int ref_w, ref_h, test_w, test_h;
	SDL_PixelFormat *pixel_format=SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if(pixel_format == NULL){
		perror("PixelFormat");
		exit(1);
	}
	SDL_QueryTexture(test_texture, &pixel_format->format, NULL, &test_w, &test_h);
	SDL_QueryTexture(ref_texture, &pixel_format->format, NULL, &ref_w, &ref_h);
	assert_int_equal(ref_w, test_w);
	assert_int_equal(ref_h, test_h);
	Uint32 *pixels_ref, *pixels_test;
	SDL_LockTexture(test_texture, NULL, (void **)&pixels_test, NULL);
	SDL_LockTexture(ref_texture, NULL, (void **)&pixels_ref, NULL);
	for(int i=0; i < ref_h; i++)
		for(int j=0; j < ref_w; j++){
			SDL_Color c_ref={0};
			SDL_Color c_test={0};
			SDL_GetRGBA(pixels_ref[i * ref_h + j], pixel_format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
			SDL_GetRGBA(pixels_test[i * ref_h + j], pixel_format, &c_test.r, &c_test.g, &c_test.b, &c_test.a);
			assert_in_range(c_test.r, c_ref.r - diff, c_ref.r + diff);
			assert_in_range(c_test.g, c_ref.g - diff, c_ref.g + diff);
			assert_in_range(c_test.b, c_ref.b - diff, c_ref.b + diff);
			assert_in_range(c_test.a, c_ref.a - diff, c_ref.a + diff);
		}
	SDL_UnlockTexture(ref_texture);
	SDL_UnlockTexture(test_texture);
}

static void negative_filter_test(void **state){
	//Loading of the reference image and the image to test
	image *test_image=load_image("./../../tests/img/test_image.png");
	image *ref_image=load_image("./../../tests/img/test_image_negative.png");
	//Loading corresponding textures
	SDL_Texture *test_texture=get_img_texture(test_image);
	SDL_Texture *ref_texture=get_img_texture(ref_image);
	//Applying the negative effect
	negative_filter(test_texture);
	//Launching test
	compare_textures(test_texture, ref_texture, 15);
	//Closing images
	free_image(test_image);
	free_image(ref_image);
}

static void black_and_white_filter_test(void **state){
	//Loading of the reference image and the image to test
	image *test_image=load_image("./../../tests/img/test_image.png");
	image *ref_image=load_image("./../../tests/img/test_image_bnw.png");
	//Loading corresponding textures
	SDL_Texture *test_texture=get_img_texture(test_image);
	SDL_Texture *ref_texture=get_img_texture(ref_image);
	//Applying the black and white filter
	negative_filter(test_texture);
	//Launching test
	compare_textures(test_texture, ref_texture, 0);
	//Closing images
	free_image(test_image);
	free_image(ref_image);
}

static void grey_filter_test(void **state){
	//Loading of the reference image and the image to test
	image *test_image=load_image("./../../tests/img/test_image.png");
	image *ref_image=load_image("./../../tests/img/test_image_grayscale.png");
	//Loading corresponding textures
	SDL_Texture *test_texture=get_img_texture(test_image);
	SDL_Texture *ref_texture=get_img_texture(ref_image);
	//Applying the grayscale effect
	negative_filter(test_texture);
	//Launching test
	compare_textures(test_texture, ref_texture, 10);
	//Closing images
	free_image(test_image);
	free_image(ref_image);
}

static void color_zone_test(void **state){
	//Loading the image to test
	image *test_image=load_image("./../../tests/img/test_image.png");
	//Loading corresponding textures
	SDL_Texture *test_texture=get_img_texture(test_image);
	//Draw an orange rectangle
	SDL_Color color = {.r = 255, .g = 165, .b = 0, .a=255};
	color_zone(test_texture, color, 10, 10, 50, 50);
	//Test the image
	SDL_PixelFormat *pixel_format=SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if(pixel_format == NULL){
		perror("PixelFormat");
		exit(1);
	}
	SDL_QueryTexture(test_texture, &pixel_format->format, NULL, NULL, NULL);
	Uint32 *pixels_test;
	SDL_LockTexture(test_texture, NULL, (void **)&pixels_test, NULL);
	for(int i=10; i < 50; i++)
		for(int j=10; j < 50; j++){
			SDL_Color c_test={0};
			SDL_GetRGBA(((Uint32 *)pixels_test)[i * 50 + j], pixel_format, &c_test.r, &c_test.g, &c_test.b, &c_test.a);
			assert_int_equal(c_test.r, color.r);
			assert_int_equal(c_test.g, color.g);
			assert_int_equal(c_test.b, color.b);
			assert_int_equal(c_test.a, color.a);
		}
	SDL_UnlockTexture(test_texture);
	free_image(test_image);
}

static void symmetry_test(void **state){
	//Loading the images for tests
	image *test_image=load_image("./../../tests/img/test_image.png");
	image *ref_image=load_image("./../../tests/img/test_image.png");
	//Loading corresponding textures
	SDL_Texture *test_texture=get_img_texture(test_image);
	SDL_Texture *ref_texture=get_img_texture(ref_image);
	SDL_PixelFormat *pixel_format=SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if(pixel_format == NULL){
		perror("PixelFormat");
		exit(1);
	}
	int w, h;
	SDL_QueryTexture(test_texture, &pixel_format->format, NULL, &w, &h);
	Uint32 *pixels_test, *pixels_ref;
	SDL_LockTexture(test_texture, NULL, (void **)&pixels_test, NULL);
	SDL_LockTexture(ref_texture, NULL, (void **)&pixels_ref, NULL);
	//Testing vertical symmetry
	symmetry(test_texture, 1);
	for(int i=0; i < h; i++)
		for(int j=0; j < w; j++){
			SDL_Color c_test={0};
			SDL_Color c_ref={0};
			SDL_GetRGBA(pixels_ref[i * h + j], pixel_format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
			SDL_GetRGBA(pixels_test[i * h + (w - j)], pixel_format, &c_test.r, &c_test.g, &c_test.b, &c_test.a);
			assert_int_equal(c_test.r, c_ref.r);
			assert_int_equal(c_test.g, c_ref.g);
			assert_int_equal(c_test.b, c_ref.b);
			assert_int_equal(c_test.a, c_ref.a);
		}
	//Testing horizontal symmetry
	test_texture=get_img_texture(test_image);
	symmetry(test_texture, 0);
	for(int i=0; i < h; i++)
		for(int j=0; j < w; j++){
			SDL_Color c_test={0};
			SDL_Color c_ref={0};
			SDL_GetRGBA(pixels_ref[i * h + j], pixel_format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
			SDL_GetRGBA(pixels_test[(h - i) + j], pixel_format, &c_test.r, &c_test.g, &c_test.b, &c_test.a);
			assert_int_equal(c_test.r, c_ref.r);
			assert_int_equal(c_test.g, c_ref.g);
			assert_int_equal(c_test.b, c_ref.b);
			assert_int_equal(c_test.a, c_ref.a);
		}
	//Closing everything
	SDL_UnlockTexture(test_texture);
	SDL_UnlockTexture(ref_texture);
	free_image(test_image);
}

 static void rotate_test(void **state){
	 //Loading the images for tests
 	image *test_image=load_image("./../../tests/img/test_image.png");
 	image *ref_image=load_image("./../../tests/img/test_image.png");
 	//Loading corresponding textures
 	SDL_Texture *test_texture=get_img_texture(test_image);
 	SDL_Texture *ref_texture=get_img_texture(ref_image);
 	SDL_PixelFormat *pixel_format=SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
 	if(pixel_format == NULL){
 		perror("PixelFormat");
 		exit(1);
 	}
 	int w_test, h_test, w_ref, h_ref;
 	SDL_QueryTexture(ref_texture, &pixel_format->format, NULL, &w_ref, &h_ref);
 	Uint32 *pixels_test, *pixels_ref;
 	SDL_LockTexture(test_texture, NULL, (void **)&pixels_test, NULL);
 	SDL_LockTexture(ref_texture, NULL, (void **)&pixels_ref, NULL);
 	//Testing rotate
 	rotate(test_texture, 90);
	SDL_QueryTexture(test_texture, &pixel_format->format, NULL, &w_test, &h_test);
	//Checking dimensions
	assert_int_equal(w_test, h_ref);
	assert_int_equal(h_test, w_ref);
	//Checking if pixels moved correctly
 	for(int i=0; i < h_ref; i++)
 		for(int j=0; j < w_ref; j++){
 			SDL_Color c_test={0};
 			SDL_Color c_ref={0};
 			SDL_GetRGBA(pixels_ref[i*h_ref + j], pixel_format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
 			SDL_GetRGBA(pixels_test[j* h_ref +w_ref-i], pixel_format, &c_test.r, &c_test.g, &c_test.b, &c_test.a);
 			assert_int_equal(c_test.r, c_ref.r);
 			assert_int_equal(c_test.g, c_ref.g);
 			assert_int_equal(c_test.b, c_ref.b);
 			assert_int_equal(c_test.a, c_ref.a);
 		}
		SDL_UnlockTexture(test_texture);
		SDL_UnlockTexture(ref_texture);
		free_image(test_image);
 }

 static void resize_workspace_test(void **state){
	 //Loading the image to test
 	image *test_image=load_image("./../../tests/img/test_image.png");
 	//Loading corresponding texture
 	SDL_Texture *test_texture=get_img_texture(test_image);
	int w_before, h_before, w_after, h_after;
	SDL_QueryTexture(test_texture, NULL, NULL, &w_before, &h_before);
 	//Resizing
	resize_workspace(test_texture, 100, 100);
 	//Launching test
	int w, h;
	SDL_QueryTexture(test_texture, NULL, NULL, &w_after, &h_after);
	assert_int_equal(w_after, w_before+100);
	assert_int_equal(h_after, h_before+100);
 	//Closing image
 	free_image(test_image);
 }

 static void new_img_test(void **state){
 	image * img = new_img("./../../tests/img/test_image.png");
	assert_non_null(img);
 }

// m_*_test functions test the getters and setters for a struct image fields

 static void m_img_texture_test(void **state){
	 image * img = new_img("./../../tests/img/test_image.png");
 	 SDL_Texture * texture;
	 set_img_texture(img, texture);
	 assert_ptr_equal(texture, get_img_texture(img));
	 free_image(img);
 }

 static void m_img_name_test(void **state){
	 image * img = new_img("./../../tests/img/test_image.png");
 	 set_img_name(img, "Image-sama");
	 assert_string_equal("Image-sama", get_img_name(img));
	 free_image(img);
 }
 static void m_img_path_test(void **state){
	 image * img = new_img("./../../tests/img/test_image.png");
	 set_img_path(img, "./../../tests/img/");
	 assert_string_equal("./../../tests/img/", get_img_path(img));
	 free_image(img);
 }
 static void m_img_ext_test(void **state){
	 image * img = new_img("./../../tests/img/test_image.png");
	 set_img_ext(img, "png");
	 assert_string_equal("png", get_img_ext(img));
	 free_image(img);
 }

int m_tests(SDL_Texture *texture){
	const struct CMUnitTest tests[]={
		unit_test(negative_filter_test),
		unit_test(black_and_white_filter_test),
		unit_test(grey_filter_test),
		unit_test(color_zone_test),
		unit_test(rotate_test),
		unit_test(symmetry_test),
		unit_test(resize_workspace_test),
		unit_test(new_img_test),
	  unit_test(m_img_texture_test),
	  unit_test(m_img_name_test),
	  unit_test(m_img_path_test),
	  unit_test(m_img_ext_test),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
