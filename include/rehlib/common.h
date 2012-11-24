#ifndef __COMMON_H_
#define __COMMON_H_
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

#define export __attribute__((visibility("default")))
#define overloadable __attribute__((overloadable))
#define sentinel __attribute__((sentinel))
#define noreturn __attribute__((noreturn))

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

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __COMMON_H_ */
