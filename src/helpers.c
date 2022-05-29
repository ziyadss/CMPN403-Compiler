#include "helpers.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

int asprintf(char **ret, const char *format, ...)
{
    *ret = NULL;

    va_list args;
    va_start(args, format);
    int count = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (count >= 0)
    {
        char *buffer = malloc(count + 1);

        va_start(args, format);
        count = vsnprintf(buffer, count + 1, format, args);
        va_end(args);

        *ret = buffer;
    }

    return count;
}