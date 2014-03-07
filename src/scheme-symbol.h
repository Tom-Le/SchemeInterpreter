/**
 * Scheme symbol.
 *
 * This type is compatible with scheme_element.
 * You may cast any scheme_symbol pointer to scheme_element and pass it to
 * any function that accepts a scheme_element pointer.
 */

#ifndef __SCHEME_SYMBOL_H__
#define __SCHEME_SYMBOL_H__

#include "scheme-element.h"

// Scheme symbol type.
#define SCHEME_SYMBOL_TYPE "SCHEME_SYMBOL_TYPE"

// Scheme symbol.
typedef struct scheme_symbol scheme_symbol;

/**
 * Create new Scheme symbol.
 * Symbol must be freed afterwards with scheme_element_free().
 *
 * @param  value  Symbol's value.
 *
 * @return Newly created symbol, or NULL if out of memory.
 */
scheme_symbol *scheme_symbol_new(char *value);

/**
 * Get value.
 * Returned string must be freed with free().
 *
 * @param  symbol  A symbol.
 */
char *scheme_symbol_get_value(scheme_symbol *symbol);

#endif
