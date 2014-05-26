#include <rehlib/common.h>
#include <rehlib/stdio.h>

#include <stdio.h>
#include <stddef.h>
#include <alloca.h>

inline ssize_t ALWAYS_INLINE getdelima( char **linep, int delimiter, FILE *stream ) {
	size_t bufsiz = config_get_size("REHLIB_GETDLIMA_BUFSIZ", 1024);
	char buf[bufsiz];
}
