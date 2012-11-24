#include <rehlib/GBTError.h>

#include <glib.h>

#include <stdarg.h>
#include <execinfo.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

static const int DEF_CALLSTACK_LEN = 128;

GBTError *g_bt_error_new( GQuark domain, gint code, const gchar *format, ... ) {
	va_list ap;
	va_start(ap, format);
	GBTError *ret = g_bt_error_new_valist(domain, code, format, ap);
	va_end(ap);
	return ret;
}

GBTError *g_bt_error_new_valist( GQuark domain, gint code, const gchar *format, va_list ap ) {
	GBTError *ret = g_slice_new(GBTError);
	ret->gerror = g_error_new_valist(domain, code, format, ap);

	for( int callstack_len = DEF_CALLSTACK_LEN; callstack_len < INT_MAX; callstack_len++ ) {
		void *callstack[callstack_len];
		int frames = backtrace(callstack, callstack_len);
		if( frames != callstack_len ) {
			ret->callstack = g_memdup(callstack, sizeof(void *) * frames);
			ret->callstack_len = frames;
			break;
		}
	}
	return ret;
}

void g_bt_error_free( GBTError **error ) {
	if( error == NULL ) return;
	GBTError *err = *error;
	if( err == NULL ) return;
	g_error_free(err->gerror);
	g_free(err->callstack);
	g_slice_free(GBTError, err);
	*error = NULL;
}

void g_bt_set_error( GBTError **err, GQuark domain, gint code, const gchar *format, ... ) {
	if( err == NULL ) return;
	g_assert(*err == NULL);
	va_list ap;
	va_start(ap, format);
	*err = g_bt_error_new_valist(domain, code, format, ap);
	va_end(ap);
}

gchar **g_bt_backtrace( GBTError *error ) {
	static GMutex mutex;

	g_mutex_lock(&mutex);
	char **traces = backtrace_symbols(error->callstack, error->callstack_len);
	gchar **ret = g_new(gchar *, error->callstack_len);
	for( int i = 0; i < error->callstack_len; i++ ) {
		ret[i] = g_strdup(traces[i]);
	}
	free(traces);
	g_mutex_unlock(&mutex);
	return ret;
}
