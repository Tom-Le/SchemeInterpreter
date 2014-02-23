#include "strutil.h"

/**** Public function implementations ****/

char *reallocate_char(char *s, int size, int newSize)
{
    newSize = (size < newSize) ? newSize : size * 2;
    char *new = malloc(sizeof(char) * newSize);

    for (int i = 0; i < size; ++i)
        new[i] = s[i];

    return new;
}
