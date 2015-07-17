/**
 * Private portion of Scheme element definition.
 *
 * This header file should only be included by code declaring
 * a Scheme data type.
 *
 * Functions in here are implemented in scheme-element.c
 */

#ifndef __SCHEME_ELEMENT_PRIVATE_H__
#define __SCHEME_ELEMENT_PRIVATE_H__

#include "scheme-element.h"

/**** Struct declarations ****/

// Struct for Scheme element.
struct scheme_element;

// Struct for type of a Scheme element.
struct scheme_element_type;

// Struct for a Scheme element's virtual function table.
struct scheme_element_vtable;

/**** Struct definitions ****/

// Scheme element struct definition.
struct scheme_element {
    struct scheme_element_vtable *vtable;
};

// Scheme element type struct.
struct scheme_element_type {
    struct scheme_element_type *super;
    char *name;
};

// Scheme element's virtual function table struct definition.
// For documentation on these, check matching functions in scheme-element.h.
struct scheme_element_vtable {
    scheme_element_type *(*get_type)();
    void (*free)(scheme_element *);
    void (*print)(scheme_element *);
    scheme_element *(*copy)(scheme_element *);
    int (*compare)(scheme_element *, scheme_element *);
};

/**** Public functions ****/

/**
 * Clone an existing virtual function table onto another table.
 *
 * @param  target  Table to be cloned to.
 * @param  source  Table to be cloned from.
 */
void scheme_element_vtable_clone(struct scheme_element_vtable *target, struct scheme_element_vtable *source);

#endif
