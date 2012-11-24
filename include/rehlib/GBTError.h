#ifndef _G_BT_ERROR_H_
#define _G_BT_ERROR_H_

#include <glib.h>

typedef struct {
	GError *gerror;
	void **callstack;
	int callstack_len;
} GBTError;

GBTError *g_bt_error_new( GQuark domain, gint code, const gchar *format, ... );
GBTError *g_bt_error_new_valist( GQuark domain, gint code, const gchar *format, va_list ap );
gchar **g_bt_backtrace( GBTError *error );
void g_bt_error_free( GBTError **error );
void g_bt_set_error( GBTError **err, GQuark domain, gint code, const gchar *format, ... );

#endif
