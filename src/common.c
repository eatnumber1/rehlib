#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>

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

bool config_try_get_intmax( const char *var_name, intmax_t *ret ) {
	g_return_val_if_fail(ret != NULL, false);
	g_return_val_if_fail(var_name != NULL, false);
	char *str = getenv(var_name);
	if( str == NULL ) return false;

	char *endptr;
	errno = 0;
	intmax_t val = strtoimax(str, &endptr, 0);
	if( val == 0 || errno == ERANGE ) return false;
	if( *str == '\0' || *endptr != '\0' ) return false;

	*ret = val;
	return true;
}

bool config_try_get_uintmax( const char *var_name, uintmax_t *ret ) {
	g_return_val_if_fail(ret != NULL, false);
	g_return_val_if_fail(var_name != NULL, false);
	char *str = getenv(var_name);
	if( str == NULL ) return false;

	char *endptr;
	errno = 0;
	uintmax_t val = strtoumax(str, &endptr, 0);
	if( val == 0 || errno == ERANGE ) return false;
	if( *str == '\0' || *endptr != '\0' ) return false;

	*ret = val;
	return true;
}

bool config_try_get_size( const char *var_name, size_t *ret ) {
	g_return_val_if_fail(ret != NULL, false);
	g_return_val_if_fail(var_name != NULL, false);
	uintmax_t ui;
	if( !config_try_get_uintmax(var_name, &ui) ) return false;
	if( ui > SIZE_MAX ) return false;
	*ret = ui;
	return true;
}

intmax_t config_get_intmax( const char *var_name, intmax_t dflt ) {
	g_return_val_if_fail(var_name != NULL, false);
	intmax_t ret;
	if( !config_try_get_intmax(var_name, &ret) ) return dflt;
	return ret;
}

uintmax_t config_get_uintmax( const char *var_name, uintmax_t dflt ) {
	g_return_val_if_fail(var_name != NULL, false);
	uintmax_t ret;
	if( !config_try_get_uintmax(var_name, &ret) ) return dflt;
	return ret;
}

size_t config_get_size( const char *var_name, size_t dflt ) {
	g_return_val_if_fail(var_name != NULL, false);
	size_t ret;
	if( !config_try_get_size(var_name, &ret) ) return dflt;
	return ret;
}
