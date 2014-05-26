#include "common.c"

#include "refmem_test.h"
#include "stdio_test.h"
#include "strton_test.h"

void register_tests() {
	refmem_register_tests();
	stdio_register_tests();
	strton_register_tests();
}
