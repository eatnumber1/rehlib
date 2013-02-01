#include <rehlib/stdio.h>

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <alloca.h>
#include <errno.h>

int EXPORT ALWAYS_INLINE vsprintfa( char **ret, const char *format, va_list ap ) {
	if( ret == NULL ) {
		errno = EINVAL;
		return -1;
	}

	va_list ap2;
	int saved_errno;
	va_copy(ap2, ap);
	int len = vsnprintf(NULL, 0, format, ap2);
	saved_errno = errno;
	va_end(ap2);
	if( len < 0 ) {
		errno = saved_errno;
		return len;
	}
	*ret = alloca((len * sizeof(char)) + 1); // TODO: Check for overflow
	return vsprintf(*ret, format, ap);
}

int EXPORT ALWAYS_INLINE sprintfa( char **ret, const char *format, ... ) {
	va_list ap;
	va_start(ap, format);
	int retcode = vsprintfa(ret, format, ap);
	va_end(ap);
	return retcode;
}
