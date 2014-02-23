/**
 * Utilities for strings.
 */

#include <stdlib.h>

/**
 * Allocate new char buffer with more space.
 * If the supplied new size is smaller than old string's size,
 * a new buffer with double the old size is initialized.
 *
 * @param  s        Old string.
 * @param  size     Old string's size.
 * @param  newSize  New string's size.
 *
 * @return New string.
 */
char *reallocate_char(char *s, int size, int newSize);
