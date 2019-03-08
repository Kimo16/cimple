#include <cmocka.h>
#include "m_color.h"

static SDL_Texture *current;

static void negative_filter_test(void **state){
}

static void black_and_white_filter_test(void **state){
}

static void grey_filter_test(void **state){
}

static void replace_color_test(void **state){
}

static void color_zone_test(void **state){
}

static void rotate_test(void **state){
}

static void symmetry_test(void **state){
}

static void truncate_test(void **state){
}

static void resize_image_test(void **state){
}

static void resize_workspace_test(void **state){
}

int m_tests(SDL_Texture *texture){
	CMUnitTest tests[]={
		unit_test(negative_filter_test),
		unit_test(black_and_white_filter_test),
		unit_test(grey_filter_test),
		unit_test(replace_color_test),
		unit_test(color_zone_test),
		unit_test(rotate_test),
		unit_test(symmetry_test),
		unit_test(truncate_test),
		unit_test(resize_image_test),
		unit_test(resize_workspace_test)
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
