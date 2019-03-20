#include "io_test.h"
#define WIDTH  300
#define HEIGHT 400

static void test_h_and_w(SDL_Texture *reference, SDL_Texture *subject){
	int w_sub, h_sub;
	int w_ref, h_ref;
	SDL_QueryTexture(reference, NULL, NULL, &w_ref, &h_ref);
	SDL_QueryTexture(subject, NULL, NULL, &w_sub, &h_sub);
	assert_int_equal(h_ref, h_sub);
	assert_int_equal(w_ref, w_sub);
}

static void test_pixel(SDL_Texture *reference, SDL_Texture *subject, int margin){
	SDL_PixelFormat *format; // Format type rgba
	Uint32 *         pixels_ref=NULL, *pixels_sub=NULL;
	int i, j, pitch_ref, pitch_sub;
	format=SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	if(format == NULL){
		fprintf(stderr, "Can't init format!\n");
		exit(1);
	}
	// Lock textures for tests
	SDL_LockTexture(reference, NULL, (void *)&pixels_ref, &pitch_ref);
	SDL_LockTexture(subject, NULL, (void *)&pixels_sub, &pitch_sub);
	for(i=0; i < HEIGHT; i++)
		for(j=0; j < WIDTH; j++){
			SDL_Color c_sub={0}, c_ref={0};
			SDL_GetRGBA(pixels_sub[i * WIDTH + j], format,
						&c_sub.r, &c_sub.g, &c_sub.b, &c_sub.a);
			SDL_GetRGBA(pixels_ref[i * WIDTH + j], format,
						&c_sub.r, &c_sub.g, &c_sub.b, &c_sub.a);
			assert_in_range(c_sub.r, c_ref.r - margin, c_ref.r + margin);
			assert_in_range(c_sub.g, c_ref.g - margin, c_ref.g + margin);
			assert_in_range(c_sub.b, c_ref.b - margin, c_ref.b + margin);
			assert_in_range(c_sub.a, c_ref.a - margin, c_ref.a + margin);
		}
	// Unlock textures after tests
	SDL_UnlockTexture(reference);
	SDL_UnlockTexture(subject);
  SDL_FreeFormat(format);

}

static void input_test(void **state){
	image *      img_ref=load_image("io_test/io_test.bmp");
	image *      img_sub=load_image("io_test/io_test.jpg");
	SDL_Texture *reference=get_img_texture(img_ref);
	SDL_Texture *subject=get_img_texture(img_sub);
	test_h_and_w(reference, subject);
	test_pixel(reference, subject, 10);
	free_image(img_ref);
	free_image(img_sub);
}

int run_io_test(){
	const struct CMUnitTest tests[]={
		cmocka_unit_test(input_test)
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
