#include <stdio.h>
#include <stdlib.h>
#include "parser_test.h"
#include "err_flags.h"
#include "parser.h"
#define LEN_PARSE_TAB 20 


static char ** symmetry_tab = {"symmetry v","symmetry h","symmetry -r 90","symmetry -v","symmetry 98","symmetry hello",NULL};
static char *** sym_argstab = {{"symmetry","v",NULL},{"symmetry","-r","90",NULL},{"symmetry","-v",NULL},{"symmetry","98",NULL},{"symmetry","hello",NULL}};
static short * sym_err_tab = {0,0,EOPT,EOPT,EINV,EINV,0};

						
static char ** rotate_tab = {"rotate -r 90","rotate 90","rotate 100","rotate -r 10","rotate 0","rotate -90","rotate -r -10",NULL};
static char ** rot_argstab = {{"rotate","-r","90",NULL},{"rotate","90",NULL},{"rotate","100",NULL},{"rotate","-r","10",NULL},{"rotate","0",NULL},
				{"rotate","-90",NULL},{"rotate","-r","-10",NULL},NULL};
static short * rot_err_tab = {0,0,ENUMV,ENUMV,0,ENUMV,ENUMV,0};


static char ** truncate_tab = {"truncate","truncate 123 0 123 0","truncate aze 100 zez 10","truncate 100 230",NULL};
static char *** trun_argstab = {{"truncate",NULL},{"truncate","123","0","123","0",NULL},{"truncate","aze","100","zez","10",NULL},
				{"truncate","100","230",NULL},NULL};
static short * trunc_err_tab = {EINV,0,ENUMV,EINV,0};



static char ** resize_tab = {"resize 100 100","resize workspace","resize workspace 100","resize workspace 100 100","resize image",NULL};
static char *** res_argstab = {{"resize","100","100",NULL},{"resize","workspace",NULL},{"resize","workspace","100",NULL},
				{"resize","workspace","100","100",NULL},{"resize","image",NULL},NULL};
static short * res_err_tab = {EINV,EMSG,EMSG,0,EINV,0};

static char ** fill_tab = {"fill 100 190 100 0","fill -a 100 100 100","fill -a 257 200 200 200","fill 200 200 200","fill its a joke",NULL};
static char *** fill_argstab = {{"fill","100","190","100","0",NULL},{"fill","-a","100","100","100",NULL},
				{"fill","-a","257","200","200","200",NULL},{"fill","200","200","200",NULL},{"fill","its","a","joke",NULL},NULL};
static short * fill_err_tab = {0,EMSG,ENUMV,EMSG,ENUMV,0};

static char ** replace_tab = {"replace -a 100 200 123 12 123 239 12 9","replace -a 200 -m 20",
					"replace -a 200 250 30 200 22 11 9 10 -m 199","replace -a -200 250 30 200 22 11 9 10 -m 19",NULL};
static char *** rep_argstab = {{"replace","-a","100","200","123","12","123","239","12","9",NULL},{"replace","-a","200","-m","20",NULL},
				{"replace","-a","200","250","30","200","22","11","9","10","-m","199",NULL},
				{"replace","-a","-200","250","30","200","22","11","9","10","-m","19",NULL},NULL};
static short * rep_err_tab = {0,EMSG,ENUMV,ENUMV,0};
							
static char ** save_tab = {"save ../../","save ./","save -f hjg ./ ","save nodir/ ""save -f png ./",NULL};
static char *** save_argstab = {{"save","../../",NULL},{"save","./",NULL},{"save","-f","hjg","./",NULL},
				{"save","-f","png","./",NULL},NULL};
static short * save_err_tab = {0,0,EFFORM,0,0};

static char ** load_tab = {"load ./noexist.png","load image.png","load 900","load image.zzz",NULL};
static char *** load_argstab = {{"load","./noexist.png",NULL},{"load","image.png",NULL},{"load","900",NULL},{"load","image.zzz",NULL},NULL};
static short * load_err_tab = {0,0,EFFORM,EFFORM,0};

static char ** negative_tab = {"negative -a 232","negative -a","negative","negative   ","negative 223",NULL};
static char *** neg_argstab = {{"negative","-a","232",NULL},{"negative","-a",NULL},{"negative",NULL},{"negative",NULL},NULL};
static short * neg_err_tab={EINV,0,0,0,EINV,0};

static char ** bnw_tab = {"bnw -a 232","bnw -a ","bnw","bnw    ","bnw 223",NULL};
static char *** bnw_argstab = {{"bnw","-a","232",NULL},{"bnw","-a",NULL},{"bnw",NULL},{"bnw",NULL},{"bnw","223",NULL},NULL};
static short * bnw_err_tab={EINV,0,0,0,EINV,0};

static char ** greyscale_tab = {"greyscale -a 232","greyscale -a ","greyscale","greyscale    ","greyscale 223",NULL};
static char *** grey_argstab = {{"greyscale","-a","232",NULL},{"greyscale","-a",NULL},{"greyscale",NULL},{"greyscale",NULL},{"greyscale","223",NULL},NULL};
static short * grey_err_tab ={EINV,0,EMSG,EMSG,EINV,0};

static char ** switch_tab = {"switch_buffer zer","switch_buffer -1  ","switch_buffer","switch_buffer 3",NULL};
static char *** switch_argstab = {{"switch_buffer","zer",NULL},{"switch_buffer","-1",NULL},{"switch_buffer",NULL}{"switch_buffer","3",NULL},NULL};
static short * switch_argstab ={EINV,ENUMV,EMSG,0,0};

static char ** list_buffer = {"list_buffer -a ","list_buffer 10","  list_buffer ",NULL};
static char *** listb_argstab = {{"list_buffer","-a",NULL},{"list_buffer","10",NULL},{"list_buffer",NULL},NULL};
static short * listb_err_tab ={EINV,EINV,0,0};



  


static void test_parse_line(char ** tab1 , char** tab2){

}
static void test_parse_error(char **tab){
	
}

int run_parser_test() {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(input_test)
  };
  return cmocka_run_group_tests(tests,NULL,NULL);
}