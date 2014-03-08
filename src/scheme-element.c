#include <string.h>

#include "scheme-element.h"
#include "scheme-element-private.h"

// For scheme_element struct and its virtual function table,
// please check scheme-element-private.h.

/**** Public function implementations ****/

char *scheme_element_get_type(scheme_element *element)
{
    if (element == NULL) return "";
    return element->vtable->get_type();
}

int scheme_element_is_type(scheme_element *element, char *type)
{
    if (element == NULL) return 0;
    return strcmp(scheme_element_get_type(element), type) == 0;
}

void scheme_element_free(scheme_element *element)
{
    if (element == NULL) return;
    element->vtable->free(element);
}

void scheme_element_print(scheme_element *element)
{
    if (element == NULL) return;
    element->vtable->print(element);
}

scheme_element *scheme_element_copy(scheme_element *element)
{
    if (element == NULL) return NULL;
    return element->vtable->copy(element);
}

int scheme_element_compare(scheme_element *element, scheme_element *other)
{
    if (element == NULL) return 0;
    if (other == NULL) return 0;

    return element->vtable->compare(element, other);
}
