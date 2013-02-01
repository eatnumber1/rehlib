#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>

#ifdef USE_GSL
#include <gsl/gsl_errno.h>
#endif

#include <glib.h>
#include <gmodule.h>

#include <rehlib/common.h>
#include "_refmem.h"

const gchar * EXPORT g_module_check_init( GModule *module ) {
	const gchar *ret;
	if( (ret = refmem_module_check_init(module)) != NULL ) return ret;
	return NULL;
}

// TODO: Don't use USE_GSL to prevent compile-time dep on gsl.
#ifndef NDEBUG
#ifdef USE_GSL
static void gsl_error_handler( const char * reason, const char * file, int line, int gsl_errno ) {
	fprintf(stderr, "gsl error %d in %s:%d: %s\n", gsl_errno, file, line, reason);
	abort();
}

static void CONSTRUCTOR kill_on_gsl_error() {
	gsl_set_error_handler(gsl_error_handler);
}
#endif /* USE_GSL */
# ifdef __APPLE__
static void DESTRUCTOR leak_sleep() {
	if( getenv("LEAK_CHECK") != NULL ) {
		unsigned int seconds = 100;
		const char *secs_s = getenv("LEAK_CHECK_SECONDS");
		if( secs_s != NULL ) {
			char *endptr;
			errno = 0;
			long seconds_l = strtol(secs_s, &endptr, 10);
			if( *endptr == '\0' && errno == 0 && seconds_l <= UINT_MAX && seconds_l >= 0 )
				seconds = (unsigned int) seconds_l;
		}
		printf("Sleeping %u seconds for leaks check.\n", seconds);
		sleep(seconds);
	}
}
# endif /* __APPLE__ */
#else /* NDEBUG */
#ifdef USE_GSL
static void CONSTRUCTOR disable_gsl_abort() {
	gsl_set_error_handler_off();
}
#endif /* USE_GSL */
#endif /* NDEBUG */
