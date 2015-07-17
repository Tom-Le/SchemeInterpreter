/**
 * Scheme namespace.
 *
 * Provide mapping from an identifier to a Scheme element.
 *
 * This type is compatible with scheme_element.
 * You may cast any scheme_namespace pointer to scheme_element and pass it to
 * any function that accepts a scheme_element pointer.
 */

#ifndef __SCHEME_NAMESPACE_H__
#define __SCHEME_NAMESPACE_H__

#include "scheme-element.h"

// Scheme namespace.
typedef struct scheme_namespace scheme_namespace;

/**
 * Create new, empty Scheme namespace.
 *
 * Returned pointer must be freed with scheme_element_free(). Any element
 * stored in the freed namespace will also be freed. If you need to retain
 * any of them, consider making a copy with scheme_element_copy().
 *
 * A copy of a Scheme namespace created with scheme_element_copy() will
 * include a copy of every element it refers to.
 *
 * @param  superset  If not NULL, newly created namespace will keep a weak
 *                   reference to the given namespace and will be able to
 *                   refer to any identifier stored there.
 *
 *                   A copy of a Scheme namespace created with
 *                   scheme_element_copy() will also only store a weak
 *                   reference to this namespace.
 *
 * @return Newly created Scheme namespace.
 */
scheme_namespace *scheme_namespace_new(scheme_namespace *superset);

/**
 * Create new Scheme namespace with some initial values.
 *
 * This namespace includes built-in Scheme procedures.
 *
 * Returned pointer must be freed with scheme_element_free(). Any element
 * stored in the freed namespace will also be freed. If you need to retain
 * any of them, consider making a copy with scheme_element_copy().
 *
 * A copy of a Scheme namespace created with scheme_element_copy() will
 * include a copy of every element it refers to.
 *
 * @param  superset  If not NULL, newly created namespace will keep a weak
 *                   reference to the given namespace and will be able to
 *                   refer to any identifier stored there.
 *
 *                   A copy of a Scheme namespace created with
 *                   scheme_element_copy() will also only store a weak
 *                   reference to this namespace.
 *
 * @return Newly created Scheme namespace.
 */
scheme_namespace *scheme_namespace_base_new(scheme_namespace *superset);

/**
 * Get element associated with an identifier in the namespace.
 *
 * If there is no element in this namespace that is associated with the
 * given identifier, will also attempt to search namespace's superset.
 * Elements in a namespace take priority over elements in its superset.
 *
 * @param  namespace   A Scheme namespace.
 * @param  identifier  An identifier.
 *
 * @return Associated element, or NULL if there is no element associated
 *         with given identifier.
 */
scheme_element *scheme_namespace_get(scheme_namespace *namespace, char *identifier);

/**
 * Store a copy of a Scheme element, associated with an identifier, in
 * the given namespace.
 *
 * @param  namespace   A Scheme namespace.
 * @param  identifier  An identifier.
 * @param  element     A Scheme element.
 */
void scheme_namespace_set(scheme_namespace *namespace, char *identifier, scheme_element *element);

/**
 * Get namespace's type.
 *
 * @return Namespace's type.
 */
scheme_element_type *scheme_namespace_get_type();

#endif
