/**
 * Abstract type for a Scheme element.
 */
#ifndef __SCHEME_ELEMENT_H__
#define __SCHEME_ELEMENT_H__

// Scheme element.
typedef struct scheme_element scheme_element;

/**** Virtual functions ****/

/**
 * Get Scheme element's type identifier string.
 *
 * A Scheme type's identifier string is usually defined in its
 * corresponding header file.
 *
 * @param  element  A Scheme element.
 *
 * @return String identifying element's type.
 */
char *scheme_element_get_type(scheme_element *element);

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

/**** Convenience functions ****/

/**
 * Check if Scheme element is a given type.
 *
 * @param  element  A Scheme element.
 * @param  type     A type identifier.
 *
 * @return 1 if Scheme element's type matches given identifier,
 *         0 otherwise.
 */
int scheme_element_is_type(scheme_element *element, char *type);

#endif
