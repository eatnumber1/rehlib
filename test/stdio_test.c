#include "stdio.c"

#include <glib.h>

#include <string.h>
#include <stddef.h>
#include <stdint.h>

static void test_sprintfa_hello() {
	char *str;

	int ret = sprintfa(&str, "Hello %s!", "World");

	size_t len = strlen("Hello World!");
	g_assert(ret >= 0);
	size_t uret = (size_t) ret;
	g_assert(uret == len);
}

void stdio_register_tests() {
	g_test_add_func("/stdio/sprintfa/hello", test_sprintfa_hello);
}
