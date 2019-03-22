#include "m_tests.h"

static void compare_surface(SDL_Surface *test_surface, SDL_Surface *ref_surface, int diff){
	SDL_PixelFormat *pixel_format=SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if(pixel_format == NULL){
		perror("PixelFormat");
		exit(1);
	}
	assert_int_equal(ref_surface->w, test_surface->w);
	assert_int_equal(ref_surface->h, test_surface->h);
	Uint32 *pixels_ref=ref_surface->pixels;
	Uint32 *pixels_test=test_surface->pixels;
	SDL_LockSurface(test_surface);
	SDL_LockSurface(ref_surface);
	for(int i=0; i < ref_surface->h; i++)
		for(int j=0; j < ref_surface->w; j++){
			SDL_Color c_ref={0};
			SDL_Color c_test={0};
			SDL_GetRGBA(pixels_ref[i * ref_surface->w + j], pixel_format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
			SDL_GetRGBA(pixels_test[i * ref_surface->w + j], pixel_format, &c_test.r, &c_test.g, &c_test.b, &c_test.a);
			assert_in_range(c_test.r, c_ref.r - diff, c_ref.r + diff);
			assert_in_range(c_test.g, c_ref.g - diff, c_ref.g + diff);
			assert_in_range(c_test.b, c_ref.b - diff, c_ref.b + diff);
			assert_in_range(c_test.a, c_ref.a - diff, c_ref.a + diff);
		}
	SDL_UnlockSurface(test_surface);
	SDL_UnlockSurface(ref_surface);
	SDL_FreeFormat(pixel_format);
}

static void negative_filter_test(void **state){
	//Loading of the reference image and the image to test
	image *test_image=load_image("m_test/test_image.png");
	image *ref_image=load_image("m_test/test_image_negative.png");
	//Loading corresponding surfaces
	SDL_Surface *test_surface=get_img_surface(test_image);
	SDL_Surface *ref_surface=get_img_surface(ref_image);
	//Applying the negative effect
	negative_filter(test_surface);
	//Launching test
	compare_surface(test_surface, ref_surface, 15);
	//Closing images
	free_image(test_image);
	free_image(ref_image);
}

static void black_and_white_filter_test(void **state){
	//Loading of the reference image and the image to test
	image *test_image=load_image("m_test/test_image.png");
	image *ref_image=load_image("m_test/test_image_bnw.png");
	//Loading corresponding surfaces
	SDL_Surface *test_surface=get_img_surface(test_image);
	SDL_Surface *ref_surface=get_img_surface(ref_image);
	//Applying the black and white filter
	black_and_white_filter(test_surface);
	//Launching test
	compare_surface(test_surface, ref_surface, 0);
	//Closing images
	free_image(test_image);
	free_image(ref_image);
}

static void grey_filter_test(void **state){
	//Loading of the reference image and the image to test
	image *test_image=load_image("m_test/test_image.png");
	image *ref_image=load_image("m_test/test_image_grayscale.png");
	//Loading corresponding surfaces
	SDL_Surface *test_surface=get_img_surface(test_image);
	SDL_Surface *ref_surface=get_img_surface(ref_image);
	//Applying the grayscale effect
	grey_filter(test_surface, 10);
	//Launching test
	compare_surface(test_surface, ref_surface, 10);
	//Closing images
	free_image(test_image);
	free_image(ref_image);
}

static void color_zone_test(void **state){
	//Loading the image to test
	image *test_image=load_image("m_test/test_image.png");
	//Loading corresponding surfaces
	SDL_Surface *test_surface=get_img_surface(test_image);
	//Draw an orange rectangle
	SDL_Color color={.r=255, .g=165, .b=0, .a=255};
	color_zone(test_surface, color, 10, 10, 50, 50);
	//Test the image
	SDL_PixelFormat *pixel_format=SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if(pixel_format == NULL){
		perror("PixelFormat");
		exit(1);
	}
	Uint32 *pixels_test=test_surface->pixels;
	SDL_LockSurface(test_surface);
	for(int i=10; i < 50; i++)
		for(int j=10; j < 50; j++){
			SDL_Color c_test={0};
			SDL_GetRGBA(pixels_test[i * 50 + j], pixel_format, &c_test.r, &c_test.g, &c_test.b, &c_test.a);
			assert_int_equal(c_test.r, color.r);
			assert_int_equal(c_test.g, color.g);
			assert_int_equal(c_test.b, color.b);
			assert_int_equal(c_test.a, color.a);
		}
	SDL_UnlockSurface(test_surface);
	SDL_FreeFormat(pixel_format);
	free_image(test_image);
}

static void symmetry_test(void **state){
	//Loading the images for tests
	image *test_image=load_image("m_test/test_image.png");
	image *ref_image=load_image("m_test/test_image.png");
	//Loading corresponding surfaces
	SDL_Surface *    test_surface=get_img_surface(test_image);
	SDL_Surface *    ref_surface=get_img_surface(ref_image);
	SDL_PixelFormat *pixel_format=SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if(pixel_format == NULL){
		perror("PixelFormat");
		exit(1);
	}
	assert_int_equal(ref_surface->w, test_surface->w);
	assert_int_equal(ref_surface->h, test_surface->h);
	Uint32 *pixels_ref=ref_surface->pixels;
	Uint32 *pixels_test=test_surface->pixels;
	SDL_LockSurface(test_surface);
	SDL_LockSurface(ref_surface);
	//Testing vertical symmetry
	symmetry(test_surface, 1);
	for(int i=0; i < test_surface->h; i++)
		for(int j=0; j < test_surface->w; j++){
			SDL_Color c_test={0};
			SDL_Color c_ref={0};
			SDL_GetRGBA(pixels_ref[i * test_surface->w + j], pixel_format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
			SDL_GetRGBA(pixels_test[i * test_surface->w + (test_surface->w - j)], pixel_format, &c_test.r, &c_test.g, &c_test.b, &c_test.a);
			assert_int_equal(c_test.r, c_ref.r);
			assert_int_equal(c_test.g, c_ref.g);
			assert_int_equal(c_test.b, c_ref.b);
			assert_int_equal(c_test.a, c_ref.a);
		}
	//Testing horizontal symmetry
	test_surface=get_img_surface(test_image);
	symmetry(test_surface, 0);
	for(int i=0; i < test_surface->h; i++)
		for(int j=0; j < test_surface->w; j++){
			SDL_Color c_test={0};
			SDL_Color c_ref={0};
			SDL_GetRGBA(pixels_ref[i * test_surface->w + j], pixel_format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
			SDL_GetRGBA(pixels_test[(test_surface->h - i)*test_surface->w + j], pixel_format, &c_test.r, &c_test.g, &c_test.b, &c_test.a);
			assert_int_equal(c_test.r, c_ref.r);
			assert_int_equal(c_test.g, c_ref.g);
			assert_int_equal(c_test.b, c_ref.b);
			assert_int_equal(c_test.a, c_ref.a);
		}
	//Closing everything
	SDL_UnlockSurface(test_surface);
	SDL_UnlockSurface(ref_surface);
	SDL_FreeFormat(pixel_format);
	free_image(test_image);
}

static void rotate_test(void **state){
	//Loading the images for tests
	image *test_image=load_image("m_test/test_image.png");
	image *ref_image=load_image("m_test/test_image.png");
	//Loading corresponding surfaces
	SDL_Surface *    test_surface=get_img_surface(test_image);
	SDL_Surface *    ref_surface=get_img_surface(ref_image);
	SDL_PixelFormat *pixel_format=SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if(pixel_format == NULL){
		perror("PixelFormat");
		exit(1);
	}
	Uint32 *pixels_ref=ref_surface->pixels;
	Uint32 *pixels_test=test_surface->pixels;
	//Testing rotate
	rotate(test_surface, 90);
	//Checking dimensions
	assert_int_equal(test_surface->w, ref_surface->h);
	assert_int_equal(test_surface->h, ref_surface->w);
	//Checking if pixels moved correctly
	for(int i=0; i < ref_surface->h; i++)
		for(int j=0; j < ref_surface->w; j++){
			SDL_Color c_test={0};
			SDL_Color c_ref={0};
			SDL_GetRGBA(pixels_ref[i * ref_surface->w + j], pixel_format, &c_ref.r, &c_ref.g, &c_ref.b, &c_ref.a);
			SDL_GetRGBA(pixels_test[j * ref_surface->w + ref_surface->w - i], pixel_format, &c_test.r, &c_test.g, &c_test.b, &c_test.a);
			assert_int_equal(c_test.r, c_ref.r);
			assert_int_equal(c_test.g, c_ref.g);
			assert_int_equal(c_test.b, c_ref.b);
			assert_int_equal(c_test.a, c_ref.a);
		}
	SDL_UnlockSurface(test_surface);
	SDL_UnlockSurface(ref_surface);
	SDL_FreeFormat(pixel_format);
	free_image(test_image);
}

static void resize_workspace_test(void **state){
	//Loading the image to test
	image *test_image=load_image("m_test/test_image.png");
	//Loading corresponding surface
	SDL_Surface *test_surface=get_img_surface(test_image);
	int          w_before, h_before, w_after, h_after;
	int          width=100, height=100;
	w_before=test_surface->w;
	h_before=test_surface->h;
	//Resizing
	resize_workspace(test_surface, width, height);
	//Launching test
	w_after=test_surface->w;
	h_after=test_surface->h;
	assert_int_equal(w_after, w_before + width);
	assert_int_equal(h_after, h_before + height);
	//Closing image
	free_image(test_image);
}

static void new_img_test(void **state){
	image *img=new_img("m_test/test_image.png");
	assert_non_null(img);
}

// m_*_test functions test the getters and setters for a struct image fields

static void m_img_surface_test(void **state){
	image *      img=new_img("m_test/test_image.png");
	SDL_Surface *surface=SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
	set_img_surface(img, surface);
	assert_ptr_equal(surface, get_img_surface(img));
	free_image(img);
}

static void m_img_name_test(void **state){
	image *img=new_img("m_test/test_image.png ");
	set_img_name(img, "Image - sama ");
	assert_string_equal("Image - sama ", get_img_name(img));
	free_image(img);
}

static void m_img_path_test(void **state){
	image *img=new_img("m_test/test_image.png");
	set_img_path(img, "m_test/");
	assert_string_equal("m_test/", get_img_path(img));
	free_image(img);
}

static void m_img_ext_test(void **state){
	image *img=new_img("m_test/test_image.png");
	set_img_ext(img, "png");
	assert_string_equal("png", get_img_ext(img));
	free_image(img);
}

int run_m_tests(){
	const struct CMUnitTest tests[]={
		unit_test(negative_filter_test),
		unit_test(black_and_white_filter_test),
		unit_test(grey_filter_test),
		unit_test(color_zone_test),
		unit_test(rotate_test),
		unit_test(symmetry_test),
		unit_test(resize_workspace_test),
		unit_test(new_img_test),
		unit_test(m_img_surface_test),
		unit_test(m_img_name_test),
		unit_test(m_img_path_test),
		unit_test(m_img_ext_test),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
