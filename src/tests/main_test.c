#include "main_test.h"

int main(){
	int rc=0;
	rc=run_io_test();
	if(rc<0) return rc;
	rc = run_m_tests();
	return rc;
}
