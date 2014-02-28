/**
 * Abstract type for a Scheme element.
 */
#ifndef __SCHEME_ELEMENT_H__
#define __SCHEME_ELEMENT_H__

// Scheme element.
typedef struct scheme_element scheme_element;

/**** Virtual functions ****/

/**
 * Get Scheme element's type.
 *
 * @param  element  A Scheme element.
 *
 * @return String identifying element's type.
 */
char *scheme_element_get_type(scheme_element *element);

/**
 * Free Scheme element.
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
 * Copy must be freed with scheme_element_free().
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
 * @return 1 if Scheme element's type matches given identifier.
 *         0 otherwise.
 */
int scheme_element_is_type(scheme_element *element, char *type);

#endif
