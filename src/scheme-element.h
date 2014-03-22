/**
 * Abstract type for a Scheme element.
 */
#ifndef __SCHEME_ELEMENT_H__
#define __SCHEME_ELEMENT_H__

// Typedef for Scheme element.
typedef struct scheme_element scheme_element;

// Typedef for type of a Scheme element.
typedef struct scheme_element_type scheme_element_type;

/**** Virtual functions ****/

/**
 * Get a Scheme element's type.
 *
 * You should not free the returned pointer.
 *
 * @param  element  A Scheme element.
 *
 * @return Element's type.
 */
scheme_element_type *scheme_element_get_type(scheme_element *element);

/**
 * Free previously allocated Scheme element.
 *
 * A Scheme element is usually initialized using a Scheme data type's
 * specific initializer function. Notes on freeing such an element
 * will be found in the function's documentation.
 *
 * @param  element  A Scheme element.
 */
void scheme_element_free(scheme_element *element);

/**
 * Print Scheme element to stdout.
 *
 * @param  element  A Scheme element.
 */
void scheme_element_print(scheme_element *element);

/**
 * Copy Scheme element.
 * Copy need to be freed with scheme_element_free() unless otherwise
 * noted in a Scheme data type's header file.
 *
 * @param  element  A Scheme element.
 *
 * @result A copy or NULL if out of memory.
 */
scheme_element *scheme_element_copy(scheme_element *element);

/**
 * Compare a Scheme element to another element.
 *
 * If either is NULL, return 0.
 *
 * @param  element  A Scheme element.
 * @param  other    Element to compare to.
 *
 * @return 1 if elements are equal, 0 otherwise.
 */
int scheme_element_compare(scheme_element *element, scheme_element *other);

/**** Other functions ****/

/**
 * Get Scheme element base type.
 */
scheme_element_type *scheme_element_get_base_type();

/**
 * Check if a Scheme element's type is compatible with the given type.
 *
 * If a Scheme element's type is compatible with a given type, the element
 * can be passed as a parameter to any function that expects a parameter
 * of the given type.
 *
 * @param  element  A Scheme element.
 * @param  type     A type.
 *
 * @return 1 if Scheme element's type is compatible, 0 otherwise.
 */
int scheme_element_is_type(scheme_element *element, scheme_element_type *type);

#endif
