#ifndef _STRTON_H_
#define _STRTON_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	STRTON_CHAR,
	STRTON_INT,
	STRTON_SHORT,
	STRTON_LONG,
	STRTON_LLONG,
	STRTON_IMAX,
} strton_type_t;

typedef enum {
	STRTON_FLAG_NONE = 0,
	STRTON_FLAG_UNSIGNED,
	STRTON_FLAG_SIGNED
} strton_flags_t;

bool strton(const char *str, int base, void *out, strton_type_t type, uint_fast8_t flags);

#endif
