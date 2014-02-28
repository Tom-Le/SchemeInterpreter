/**
 * Scheme symbol.
 * Note: Due to implementation, a symbol can only be 1024 characters long at most.
 *
 * This type is compatible with scheme_element.
 * You may pass any scheme_symbol pointer as a scheme_element pointer.
 */

#ifndef __SCHEME_SYMBOL_H__
#define __SCHEME_SYMBOL_H__

#include "scheme-element.h"

// Scheme identifier symbol type.
#define SCHEME_SYMBOL_TYPE "SCHEME_SYMBOL_TYPE"

// Scheme identifier symbol.
typedef struct scheme_symbol scheme_symbol;

/**
 * Create new Scheme identifier symbol.
 * Symbol must be freed afterwards with scheme_element_free().
 *
 * @param  identifier  Symbol's identifier.
 *
 * @return Newly created symbol, or NULL if out of memory.
 */
scheme_symbol *scheme_symbol_new(char *identifier);

/**
 * Get identifier.
 * Returned string must be freed with free().
 *
 * @param  symbol  An identifier symbol.
 */
char *scheme_symbol_get_value(scheme_symbol *symbol);

#endif
