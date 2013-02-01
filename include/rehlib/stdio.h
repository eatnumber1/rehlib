#ifndef _STDIO_H_
#define _STDIO_H_

#include <rehlib/common.h>

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <alloca.h>
#include <errno.h>

// int vsprintfa( char **ret, const char *format, va_list ap );
#define vsprintfa(ret, format, ap) \
	({ \
		char **_ret = (ret); \
		int retlen = -1; \
		if( _ret == NULL ) { \
			errno = EINVAL; \
		} else { \
			const char *_format = (format); \
			va_list ap1, ap2; \
			va_copy(ap1, (ap)); \
			va_copy(ap2, ap1); \
			size_t len = vsnprintf(NULL, 0, _format, ap1); \
			*_ret = alloca((len * sizeof(char)) + 1); \
			retlen = vsprintf(*_ret, _format, ap2); \
			va_end(ap1); \
			va_end(ap2); \
		} \
		retlen; \
	})

// int sprintfa( char **ret, const char *format, ... );
#define sprintfa(ret, format, args...) \
	({ \
		char **_ret = (ret); \
		int retlen = -1; \
		if( _ret == NULL ) { \
			errno = EINVAL; \
		} else { \
			const char *_format = (format); \
			size_t len = snprintf(NULL, 0, _format, args); \
			*_ret = alloca((len * sizeof(char)) + 1); \
			retlen = sprintf(*_ret, _format, args); \
		} \
		retlen; \
	})

#endif
