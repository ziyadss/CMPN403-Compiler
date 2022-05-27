#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int asprintf(char **ret, const char *format, ...)
{
    *ret = NULL;

    va_list ap;
    va_start(ap, format);
    int count = vsnprintf(NULL, 0, format, ap);
    va_end(ap);

    if (count >= 0)
    {
        char *buffer = malloc(count + 1);

        va_start(ap, format);
        count = vsnprintf(buffer, count + 1, format, ap);
        va_end(ap);

        *ret = buffer;
    }

    return count;
}