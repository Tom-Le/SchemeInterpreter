/**
 * Scheme void symbol.
 *
 * A special symbol representing the absence of a value.
 *
 * This type is compatible with scheme_element.
 * You may cast any scheme_void pointer to scheme_element and pass it to
 * any function that accepts a scheme_element pointer.
 */

#ifndef __SCHEME_VOID_H__
#define __SCHEME_VOID_H__

#include "scheme-element.h"

// Scheme void type identifier.
#define SCHEME_VOID_TYPE "SCHEME_VOID_TYPE"

// Scheme void symbol.
typedef struct scheme_void scheme_void;

/**
 * Get void symbol. You do not need to free this symbol, but it is safe to
 * pass it to scheme_element_free() as nothing will happen.
 *
 * You cannot make a copy of this symbol. If scheme_element_copy() receives
 * a pointer to this symbol, it will return the same pointer.
 *
 * If you pass this symbol to scheme_element_print(), nothing will be
 * printed.
 *
 * @return Void symbol.
 */
scheme_element *scheme_void_get();

#endif
