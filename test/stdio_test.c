#include <rehlib/stdio.h>

#include <glib.h>

#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

static void test_vsprintfa_hello_internal( int dummy, ... ) {
	char *str;

	va_list ap;
	va_start(ap, dummy);
	int ret = vsprintfa(&str, "Hello %s!", ap);
	va_end(ap);
	size_t len = strlen("Hello World!");
	g_assert(ret >= 0);
	size_t uret = (size_t) ret;
	g_assert(uret == len);
}

static void test_vsprintfa_hello() {
	test_vsprintfa_hello_internal(0, "World");
}

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
	g_test_add_func("/stdio/vsprintfa/hello", test_vsprintfa_hello);
}
