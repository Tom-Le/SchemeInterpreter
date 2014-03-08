/**
 * Utilities for strings.
 */

#ifndef __STRUTIL_H__
#define __STRUTIL_H__

#include <stdlib.h>

/**
 * Allocate new char buffer with more space.
 * If the supplied new size is smaller than old string's size,
 * a new buffer with size double the old one's is initialized.
 *
 * @param  s        Old string.
 * @param  size     Old string's size.
 * @param  newSize  New string's size.
 *
 * @return New string.
 */
char *reallocate_char(char *s, int size, int newSize);

#endif
