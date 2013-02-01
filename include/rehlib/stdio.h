#ifndef _ASTDIO_H_
#define _ASTDIO_H_

#include <rehlib/common.h>

#include <glib.h>

#include <stdarg.h>
#include <stdio.h>

G_BEGIN_DECLS

int EXPORT ALWAYS_INLINE sprintfa( char **ret, const char *format, ... );
int EXPORT ALWAYS_INLINE vsprintfa( char **ret, const char *format, va_list ap );

G_END_DECLS

#endif
