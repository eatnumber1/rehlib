#include <glib.h>

#include <stddef.h>

#include "common_test.h"

int main( int argc, char *argv[] ) {
	g_test_init(&argc, &argv, NULL);
	register_tests();
	g_test_run();
}
