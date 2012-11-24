#include <glib.h>
#include <gmodule.h>

#include <stdio.h>
#include <stdlib.h>

static void module_die( const char *domain ) {
	fprintf(stderr, "%s: %s\n", domain, g_module_error());
	exit(EXIT_FAILURE);
}

int main( int argc, char *argv[] ) {
	g_test_init(&argc, &argv, NULL);
	if( !g_module_supported() ) {
		fprintf(stderr, "Dynamic module loading not supported\n");
		exit(EXIT_FAILURE);
	}
	for( int i = 1; i < argc; i++ ) {
		GModule *module = g_module_open(argv[i], G_MODULE_BIND_LOCAL | G_MODULE_BIND_LAZY);
		if( module == NULL ) module_die("g_module_open");
		void (*register_tests)();
		if( !g_module_symbol(module, "register_tests", (void **) &register_tests) ) module_die("g_module_symbol");
		register_tests();
		if( !g_module_close(module) ) module_die("g_module_close");
	}
	g_test_run();
}
