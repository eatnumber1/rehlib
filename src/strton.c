#include <rehlib/strton.h>
#include <rehlib/common.h>

#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>

static bool _strtoimax(const char *str, int base, intmax_t *out) {
	char *endptr = NULL;
	errno = 0;
	intmax_t ret = strtoimax(str, &endptr, base);
	if (errno != 0) return false;
	if (*endptr != '\0') {
		errno = EINVAL;
		return false;
	}
	*out = ret;
	return true;
}

static bool _strtoumax(const char *str, int base, uintmax_t *out) {
	char *endptr = NULL;
	errno = 0;
	uintmax_t ret = strtoumax(str, &endptr, base);
	if (errno != 0) return false;
	if (*endptr != '\0') {
		errno = EINVAL;
		return false;
	}
	*out = ret;
	return true;
}

static bool strton_char(const char *str, int base, void *out, uint_fast8_t flags) {
	if (flags & STRTON_FLAG_UNSIGNED) {
		uintmax_t val;
		if (!_strtoumax(str, base, &val)) return false;
		if (val > UCHAR_MAX) {
			errno = ERANGE;
			return false;
		}
		*((unsigned char *) out) = (unsigned char) val;
	} else if (flags & STRTON_FLAG_SIGNED) {
		intmax_t val;
		if (!_strtoimax(str, base, &val)) return false;
		if (val > SCHAR_MAX || val < SCHAR_MIN) {
			errno = ERANGE;
			return false;
		}
		*((signed char *) out) = (signed char) val;
	} else {
		intmax_t val;
		if (!_strtoimax(str, base, &val)) return false;
		if (val > CHAR_MAX || val < CHAR_MIN) {
			errno = ERANGE;
			return false;
		}
		*((char *) out) = (char) val;
	}
	return true;
}

static bool strton_int(const char *str, int base, void *out, uint_fast8_t flags) {
	if (flags & STRTON_FLAG_UNSIGNED) {
		uintmax_t val;
		if (!_strtoumax(str, base, &val)) return false;
		if (val > UINT_MAX) {
			errno = ERANGE;
			return false;
		}
		*((unsigned int *) out) = (unsigned int) val;
	} else {
		intmax_t val;
		if (!_strtoimax(str, base, &val)) return false;
		if (val > INT_MAX || val < INT_MIN) {
			errno = ERANGE;
			return false;
		}
		*((int *) out) = (int) val;
	}
	return true;
}

static bool strton_short(const char *str, int base, void *out, uint_fast8_t flags) {
	if (flags & STRTON_FLAG_UNSIGNED) {
		uintmax_t val;
		if (!_strtoumax(str, base, &val)) return false;
		if (val > USHRT_MAX) {
			errno = ERANGE;
			return false;
		}
		*((unsigned short *) out) = (unsigned short) val;
	} else {
		intmax_t val;
		if (!_strtoimax(str, base, &val)) return false;
		if (val > SHRT_MAX || val < SHRT_MIN) {
			errno = ERANGE;
			return false;
		}
		*((short *) out) = (short) val;
	}
	return true;
}

static bool strton_long(const char *str, int base, void *out, uint_fast8_t flags) {
	if (flags & STRTON_FLAG_UNSIGNED) {
		uintmax_t val;
		if (!_strtoumax(str, base, &val)) return false;
		if (val > ULONG_MAX) {
			errno = ERANGE;
			return false;
		}
		*((unsigned long *) out) = (unsigned long) val;
	} else {
		intmax_t val;
		if (!_strtoimax(str, base, &val)) return false;
		if (val > LONG_MAX || val < LONG_MIN) {
			errno = ERANGE;
			return false;
		}
		*((long *) out) = (long) val;
	}
	return true;
}

static bool strton_llong(const char *str, int base, void *out, uint_fast8_t flags) {
	if (flags & STRTON_FLAG_UNSIGNED) {
		uintmax_t val;
		if (!_strtoumax(str, base, &val)) return false;
		if (val > ULLONG_MAX) {
			errno = ERANGE;
			return false;
		}
		*((unsigned long long *) out) = (unsigned long long) val;
	} else {
		intmax_t val;
		if (!_strtoimax(str, base, &val)) return false;
		if (val > LLONG_MAX || val < LLONG_MIN) {
			errno = ERANGE;
			return false;
		}
		*((long long *) out) = (long long) val;
	}
	return true;
}

static bool strton_imax(const char *str, int base, void *out, uint_fast8_t flags) {
	if (flags & STRTON_FLAG_UNSIGNED) {
		uintmax_t val;
		if (!_strtoumax(str, base, &val)) return false;
		if (val > UINTMAX_MAX) {
			errno = ERANGE;
			return false;
		}
		*((uintmax_t *) out) = (uintmax_t) val;
	} else {
		intmax_t val;
		if (!_strtoimax(str, base, &val)) return false;
		if (val > INTMAX_MAX || val < INTMAX_MIN) {
			errno = ERANGE;
			return false;
		}
		*((intmax_t *) out) = (intmax_t) val;
	}
	return true;
}

static bool (* const strton_converters[])(const char *, int, void *, uint_fast8_t) = {
	[STRTON_CHAR] = strton_char,
	[STRTON_INT] = strton_int,
	[STRTON_SHORT] = strton_short,
	[STRTON_LONG] = strton_long,
	[STRTON_LLONG] = strton_llong,
	[STRTON_IMAX] = strton_imax
};

bool EXPORT strton(const char *str, int base, void *out, strton_type_t type, uint_fast8_t flags) {
	if (type >= sizeof(strton_converters) / sizeof(strton_converters[0])) {
		errno = EINVAL;
		return false;
	}
	return strton_converters[type](str, base, out, flags);
}
