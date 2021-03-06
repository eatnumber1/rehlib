#ifndef __COMMON_H_
#define __COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __has_builtin
# define __has_builtin(x) 0 // Compatibility with non-clang compilers.
#endif /* __has_builtin */

#ifndef __has_feature
# define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif /* __has_feature */

#ifndef __has_extension
# define __has_extension __has_feature // Compatibility with pre-3.0 clang.
#endif /* __has_extension */

#ifndef __has_attribute
# define __has_attribute(x) 0 // Compatibility with non-clang compilers.
#endif /* __has_attribute */

#define EXPORT __attribute__((visibility("default")))
#define OVERLOADABLE __attribute__((overloadable))
#define SENTINEL __attribute__((sentinel))
#define NORETURN __attribute__((noreturn))
#define ALWAYS_INLINE __attribute__((always_inline))
#define CONSTRUCTOR __attribute__((constructor))
#define DESTRUCTOR __attribute__((destructor))

#if !__has_extension(c_static_assert)
# define _Static_assert(a, b, s)
#endif /* !__has_extension(c_static_assert) */

#ifndef __cplusplus
# if __has_extension(c_generic_selections)
#  define cos(x) _Generic(x, \
	double: cos(x), \
	long double: cosl(x), \
	float: cosf(x) \
)

#  define sin(x) _Generic(x, \
	double: sin(x), \
	long double: sinl(x), \
	float: sinf(x) \
)

#  define sqrt(x) _Generic(x, \
	double: sqrt(x), \
	long double: sqrtl(x), \
	float: sqrtf(x) \
)
# endif /* __has_extension(c_generic_selections) */
#endif /* __cplusplus */

#ifndef MIN
#define MIN(a, b) ({ \
	__typeof__(a) _a = (a); \
	__typeof__(b) _b = (b); \
	_a > _b ? _b : _a; \
})
#endif /* MIN */

#ifndef MAX
#define MAX(a, b) ({ \
	__typeof__(a) _a = (a); \
	__typeof__(b) _b = (b); \
	_a < _b ? _b : _a; \
})
#endif /* MAX */

#ifdef G_LOG_DOMAIN
# undef G_LOG_DOMAIN
#endif /* G_LOG_DOMAIN */
#define G_LOG_DOMAIN "lcs"

bool config_try_get_intmax( const char *var_name, intmax_t *ret );
bool config_try_get_uintmax( const char *var_name, uintmax_t *ret );
bool config_try_get_size( const char *var_name, size_t *ret );

intmax_t config_get_intmax( const char *var_name, intmax_t dflt );
uintmax_t config_get_uintmax( const char *var_name, uintmax_t dflt );
size_t config_get_size( const char *var_name, size_t dflt );

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __COMMON_H_ */
