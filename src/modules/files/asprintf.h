#pragma once
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#if !defined(HAVE_VASPRINTF)
int vasprintf(char **strp, const char *fmt, va_list ap);
#endif

#if !defined(HAVE_ASPRINTF)
int asprintf(char **strp, const char *fmt, ...);
#endif

int test_asprintf();

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
