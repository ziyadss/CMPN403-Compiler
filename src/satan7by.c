#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define MAP_KV(T, Name)       \
    typedef struct Map_##Name \
    {                         \
        char *key;            \
        T value;              \
    } Map_##Name##_El, *Map_##Name

MAP_KV(int, Str);
// first is type, second is name
// bas keda
// MAP_KV(char *, Str)

int main()
{
    Map_Str bebe = NULL;

    Map_Str_El element = {.key = "me", .value = 3};

    shput(bebe, element.key, element.value);
    Map_Str_El *beb = shgetp_null(bebe, "me");

    if (beb == NULL)
        printf("bebe\n");
    else
        printf("%d\n", beb->value);

    shfree(bebe);
    return 0;
}