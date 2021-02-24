#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include "asprintf.h"
/*
//Define the following, and this will work with MSVC or other platforms that lack va_copy, but a simple copying of va_list does the trick
#ifndef va_copy
#define va_copy(dest, src) dest = src
#endif
*/
#define insane_free(ptr) { free(ptr); ptr = 0; }

#if !defined(HAVE_VASPRINTF)
int vasprintf(char **strp, const char *fmt, va_list ap)
{
    int r = -1, size;

    va_list ap2;
    va_copy(ap2, ap);

    size = vsnprintf(0, 0, fmt, ap2);

    if ((size >= 0) && (size < INT_MAX))
    {
        *strp = (char *)malloc(size+1); //+1 for null
        if (*strp)
        {
            r = vsnprintf(*strp, size+1, fmt, ap);  //+1 for null
            if ((r < 0) || (r > size))
            {
                insane_free(*strp);
                r = -1;
            }
        }
    }
    else { *strp = 0; }

    va_end(ap2);

    return(r);
}
#endif

#if !defined(HAVE_ASPRINTF)
int asprintf(char **strp, const char *fmt, ...)
{
    int r;
    va_list ap;
    va_start(ap, fmt);
    r = vasprintf(strp, fmt, ap);
    va_end(ap);
    return(r);
}

#endif

int test_asprintf()
{
    char *s;
    if (asprintf(&s, "Hello, %d in hex padded to 8 digits is: %08x\n", 15, 15) != -1)
    {
        puts(s);
        insane_free(s);
    }
    asprintf(&s, "Numbers!\n0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789\n"
                 "%s",
             "0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789 0123456789\n");
    if (s)
    {
        puts(s);
        puts("\n");
        insane_free(s);
    }
    return(0);
}