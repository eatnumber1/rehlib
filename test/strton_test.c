#include <rehlib/strton.h>

#include <glib.h>

#include "strton_test.h"

static void test_strton() {
	unsigned char val;
	g_assert(strton("255", 10, &val, STRTON_CHAR, STRTON_FLAG_UNSIGNED));
	g_assert(val == 255);
}

void strton_register_tests() {
	g_test_add_func("/strton", test_strton);
}
