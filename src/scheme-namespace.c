#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scheme-namespace.h"
#include "scheme-element-private.h"
#include "procedures.h"

#define SCHEME_NAMESPACE_INITIAL_SIZE 32

// Struct for an element stored in namespace.
struct _namespace_item {
    char *identifier;
    scheme_element *element;
};

// Scheme namespace.
struct scheme_namespace {
    struct scheme_element super;
    struct scheme_namespace *superset;
    struct _namespace_item *items;
    int itemCount;
    int itemSize;
};

/**** Private function declarations ****/

/**
 * Free Scheme namespace.
 *
 * @param  element  Should be a namespace.
 */
static void _vtable_free(scheme_element *element);

/**
 * Print Scheme namespace to stdout.
 *
 * @param  element  Should be a namespace.
 */
static void _vtable_print(scheme_element *element);

/**
 * Copy namespace.
 *
 * @param  element  Should be a Scheme namespace.
 *
 * @return A copy, or NULL if out of memory.
 */
static scheme_element *_vtable_copy(scheme_element *element);

/**
 * Compare namespace to another namespace.
 * Will return 0 if either pointer is not a namespace.
 *
 * @param  element  Should be a Scheme namespace.
 * @param  other    A Scheme element.
 */
static int _vtable_compare(scheme_element *element, scheme_element *other);

/**
 * Initialize an already allocated namespace item.
 *
 * @param  item        A namespace item.
 * @param  identifier  An identifier.
 * @param  elemnet     A Scheme element.
 */
static void _namespace_item_init(struct _namespace_item *item, char *identifier, scheme_element *element);

/**
 * Free items in a namespace item.
 *
 * @param  item  A namespace item.
 */
static void _namespace_item_free_content(struct _namespace_item *item);

/**
 * Compare two namespace items.
 *
 * @param  this  A namespace item.
 * @param  that  A namespace item.
 *
 * @return 1 if they have the same identifiers and Scheme elements,
 *         0 otherwise.
 */
static int _namespace_item_compare(struct _namespace_item *this, struct _namespace_item *that);

/**** Private variables ****/

// Global virtual function table.
static struct scheme_element_vtable _scheme_namespace_vtable = {
    .get_type = scheme_namespace_get_type,
    .free = _vtable_free,
    .print = _vtable_print,
    .copy = _vtable_copy,
    .compare = _vtable_compare
};

// Static struct for namespace's type.
static struct scheme_element_type _scheme_namespace_type = {
    .super = NULL,
    .name = "scheme_namespace"
};
static int _scheme_namespace_type_initd = 0;

/**** Private function implementations ****/

static void _vtable_free(scheme_element *element)
{
    scheme_namespace *namespace = (scheme_namespace *)element;

    int itemCount = namespace->itemCount;
    for (int i = 0; i < itemCount; ++i)
    {
        struct _namespace_item item = namespace->items[i];
        _namespace_item_free_content(&item);
    }

    free(namespace->items);
    free(namespace);
}

static void _vtable_print(scheme_element *element)
{
    printf("#<namespace>");
}

static scheme_element *_vtable_copy(scheme_element *element)
{
    scheme_namespace *namespace = (scheme_namespace *)element;

    // Allocate copy.
    scheme_namespace *copy;
    if ((copy = malloc(sizeof(scheme_namespace))) == NULL)
        return NULL;

    // Set up virtual function table.
    copy->super.vtable = &_scheme_namespace_vtable;

    // Copy attributes.
    copy->superset = namespace->superset;
    copy->itemCount = namespace->itemCount;
    copy->itemSize = namespace->itemSize;

    // Allocate identifier lookup table.
    struct _namespace_item *items;
    if ((items = malloc(sizeof(struct _namespace_item) * copy->itemSize)) == NULL)
    {
        free(copy);
        return NULL;
    }

    // Copy items.
    int itemCount = copy->itemCount;
    for (int i = 0; i < itemCount; ++i)
    {
        // TODO: Properly free items array and copy pointer if we run out of memory.
        _namespace_item_init(items + i, namespace->items[i].identifier, namespace->items[i].element);
    }

    copy->items = items;

    return (scheme_element *)copy;
}

static int _vtable_compare(scheme_element *element, scheme_element *other)
{
    if (!scheme_element_is_type(other, &_scheme_namespace_type)) return 0;

    scheme_namespace *this = (scheme_namespace *)element;
    scheme_namespace *that = (scheme_namespace *)other;

    if (this->itemCount != that->itemCount) return 0;

    if (!scheme_element_compare((scheme_element *)this->superset, (scheme_element *)that->superset))
        return 0;

    int count = this->itemCount;
    for (int i = 0; i < count; ++i)
    {
        if (!_namespace_item_compare(this->items + i, that->items + i))
            return 0;
    }

    return 1;
}

static void _namespace_item_init(struct _namespace_item *item, char *identifier, scheme_element *element)
{
    int idLength = strlen(identifier);
    if ((item->identifier = malloc(sizeof(char) * (idLength + 1))) == NULL)
        return;
    strcpy(item->identifier, identifier);

    if ((item->element = scheme_element_copy(element)) == NULL)
    {
        free(item->identifier);
        return;
    }
}

static void _namespace_item_free_content(struct _namespace_item *item)
{
    free(item->identifier);
    scheme_element_free(item->element);
}

static int _namespace_item_compare(struct _namespace_item *this, struct _namespace_item *that)
{
    if (strcmp(this->identifier, that->identifier) != 0) return 0;
    if (!scheme_element_compare(this->element, that->element)) return 0;

    return 1;
}

/**** Public function implementations ****/

scheme_namespace *scheme_namespace_new(scheme_namespace *superset)
{
    // Allocate namespace.
    scheme_namespace *namespace;
    if ((namespace = malloc(sizeof(scheme_namespace))) == NULL)
        return NULL;

    // Set up virtual function table.
    namespace->super.vtable = &_scheme_namespace_vtable;

    // Set up identifier lookup table.
    struct _namespace_item *items;
    if ((items = malloc(sizeof(struct _namespace_item) * SCHEME_NAMESPACE_INITIAL_SIZE)) == NULL)
    {
        free(namespace);
        return NULL;
    }
    namespace->items = items;
    namespace->itemSize = SCHEME_NAMESPACE_INITIAL_SIZE;
    namespace->itemCount = 0;

    // Store superset.
    if (superset != NULL && scheme_element_is_type((scheme_element *)superset, &_scheme_namespace_type))
        namespace->superset = superset;
    else
        namespace->superset = NULL;

    return namespace;
}

/**
 * Macro for adding a built-in Scheme procedure to a namespace.
 * Used in scheme_namespace_base_new().
 *
 * @param  NAMESPACE  A Scheme namespace.
 * @param  PROCNAME   Built-in procedure's name in all lowercase.
 */
#define SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(NAMESPACE, PROCNAME) \
    char *PROCNAME##Name = scheme_procedure_get_name(scheme_procedure_##PROCNAME()); \
    scheme_namespace_set(NAMESPACE , PROCNAME##Name, (scheme_element *)scheme_procedure_##PROCNAME()); \
    free(PROCNAME##Name);

scheme_namespace *scheme_namespace_base_new(scheme_namespace *superset)
{
    scheme_namespace *namespace = scheme_namespace_new(superset);

    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, car);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, quote);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, cdr);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, issymbol);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, cons);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, isequal);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, isnull);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, append);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, assoc);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, cond);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, if);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, define);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, lambda);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, islist);
    SCHEME_NAMESPACE_ADD_BUILTIN_PROCEDURE(namespace, isprocedure);

    return namespace;
}

scheme_element *scheme_namespace_get(scheme_namespace *namespace, char *identifier)
{
    // Search in namespace.
    int count = namespace->itemCount;
    for (int i = 0; i < count; ++i)
    {
        if (strcmp(identifier, namespace->items[i].identifier) == 0)
        {
            return scheme_element_copy(namespace->items[i].element);
        }
    }

    // Search in superset if one exists.
    if (namespace->superset != NULL)
        return scheme_namespace_get(namespace->superset, identifier);

    return NULL;
}

void scheme_namespace_set(scheme_namespace *namespace, char *identifier, scheme_element *element)
{
    // Search in namespace to see if identifier already exists.
    int count = namespace->itemCount;
    for (int i = 0; i < count; ++i)
    {
        if (strcmp(identifier, namespace->items[i].identifier) == 0)
        {
            // Store given element under this identifier.
            scheme_element_free(namespace->items[i].element);
            namespace->items[i].element = scheme_element_copy(element);

            return;
        }
    }

    // Create new namespace item to store element.
    // Ensure we have enough space.
    if (count >= namespace->itemSize)
    {
        int newSize = namespace->itemSize * 2;
        struct _namespace_item *newItems = malloc(sizeof(struct _namespace_item) * newSize);
        if (newItems == NULL) return;

        memcpy(newItems, namespace->items, sizeof(struct _namespace_item) * namespace->itemSize);
        free(namespace->items);

        namespace->items = newItems;
        namespace->itemSize = newSize;
    }

    _namespace_item_init(namespace->items + count, identifier, element);
    namespace->itemCount += 1;
}

scheme_element_type *scheme_namespace_get_type()
{
    if (!_scheme_namespace_type_initd)
    {
        _scheme_namespace_type.super = scheme_element_get_base_type();

        _scheme_namespace_type_initd = 1;
    }

    return &_scheme_namespace_type;
}
