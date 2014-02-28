/**
 * Private portion of Scheme element definition.
 */

#include "scheme-element.h"

#ifndef __SCHEME_ELEMENT_PRIVATE_H__
#define __SCHEME_ELEMENT_PRIVATE_H__

struct scheme_element;
struct scheme_element_vtable;

// Scheme element struct.
struct scheme_element {
    struct scheme_element_vtable *vtable;
};

// Scheme element's virtual function table.
// For documentation on these, check matching functions in scheme-element.h.
struct scheme_element_vtable {
    char *(*get_type)(scheme_element *);
    void (*free)(scheme_element *);
    void (*print)(scheme_element *);
    scheme_element *(*copy)(scheme_element *);
};

#endif
