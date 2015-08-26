#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <dlfcn.h>

#include "scheme-data-types.h"
#include "loader.h"

// Name of function to get Scheme procedure stored in a handle.
#define SCHEME_PROCEDURE_GETTER_FUNC_NAME "scheme_procedure_get"
// Typedef for this function.
typedef scheme_procedure *(scheme_procedure_getter_func)(void);

// Name of function to get number of aliases stored in a handle.
#define SCHEME_PROCEDURE_ALIASES_COUNT_FUNC_NAME "scheme_procedure_get_alias_count"
// Typedef for this function.
typedef int (scheme_procedure_alias_count_getter_func)(void);

// Name of function to get aliases stored in a handle.
#define SCHEME_PROCEDURE_ALIAS_GETTER_FUNC_NAME "scheme_procedure_get_alias"
// Typedef for this function.
typedef const char *(scheme_procedure_alias_getter_func)(int);

// Linked list for loader.
struct scheme_loader_item {
    // Handle from dlopen.
    void *handle;
    // Next item.
    struct scheme_loader_item *next;
};

// Loader.
struct scheme_loader {
    // List of handles.
    struct scheme_loader_item *handlesList;
};

/**** Private function declarations ****/

/**
 * Free list of handles.
 *
 * @param  list  List of handles.
 */
void _scheme_loader_item_free(struct scheme_loader_item *list);

/**** Private function implementations ****/

void _scheme_loader_item_free(struct scheme_loader_item *list)
{
    if (list == NULL) return;

    _scheme_loader_item_free(list->next);
    dlclose(list->handle);
    free(list);
}

/**** Public function implementations ****/

scheme_loader *scheme_loader_new()
{
    // Set up loader struct.
    scheme_loader *loader = malloc(sizeof(scheme_loader));
    if (loader == NULL) return NULL;
    loader->handlesList = NULL;

    return loader;
}

int scheme_loader_load_file(scheme_loader *loader, const char *path)
{
    // Open a handle.
    void *handle = dlopen(path, RTLD_NOW);
    if (!handle)
    {
        return 0;
    }

    // Verify handle contains a Scheme procedure getter function.
    scheme_procedure_getter_func *getter = dlsym(handle, SCHEME_PROCEDURE_GETTER_FUNC_NAME);
    scheme_procedure *proc = (*getter)();
    if (!scheme_element_is_type((scheme_element *)proc, scheme_procedure_get_type()))
    {
        dlclose(handle);
        return 0;
    }

    // Store handle in loader.
    struct scheme_loader_item *item = malloc(sizeof(struct scheme_loader_item));
    item->handle = handle;
    item->next = NULL;

    if (loader->handlesList == NULL)
    {
        loader->handlesList = item;
    }
    else
    {
        struct scheme_loader_item *lastItem = loader->handlesList;
        while (lastItem->next != NULL)
        {
            lastItem = lastItem->next;
        }

        lastItem->next = item;
    }

    return 1;
}

int scheme_loader_load_folder(scheme_loader *loader, const char *path)
{
    // Open folder.
    DIR *dir_p = opendir(path);
    if (dir_p == NULL)
    {
        return 0;
    }

    // Open each file in folder.
    int successCount = 0;
    struct dirent *dirent_p;
    while ((dirent_p = readdir(dir_p)) != NULL)
    {
        char *filepath = malloc(sizeof(char) * 1024);
        filepath[0] = '\0';
        strcat(filepath, path);
        strcat(filepath, "/");
        strcat(filepath, dirent_p->d_name);

        successCount += scheme_loader_load_file(loader, filepath);

        free(filepath);
    }

    return successCount;
}

void scheme_loader_put_onto_namespace(scheme_loader *loader, scheme_namespace *namespace)
{
    // Go through each handle stored in loader.
    struct scheme_loader_item *item = loader->handlesList;
    while (item != NULL)
    {
        // Get Scheme procedure in handle and store in namespace.
        scheme_procedure_getter_func *getter = dlsym(item->handle, SCHEME_PROCEDURE_GETTER_FUNC_NAME);
        scheme_procedure *proc = (*getter)();
        char *procName = scheme_procedure_get_name(proc);
        scheme_namespace_set(namespace, procName, (scheme_element *)proc);

        // Optionally get aliases for procedure and store procedure in namespace under these aliases.
        scheme_procedure_alias_count_getter_func *aliasCountGetter = dlsym(item->handle, SCHEME_PROCEDURE_ALIASES_COUNT_FUNC_NAME);
        scheme_procedure_alias_getter_func *aliasGetter = dlsym(item->handle, SCHEME_PROCEDURE_ALIAS_GETTER_FUNC_NAME);
        if (aliasGetter != NULL && aliasCountGetter != NULL)
        {
            int aliasCount = (*aliasCountGetter)();
            for (int i = 0; i < aliasCount; ++i)
            {
                const char *alias = (*aliasGetter)(i);
                scheme_namespace_set(namespace, alias, (scheme_element *)proc);
            }
        }

        // Advance to next item.
        item = item->next;
    }
}

void scheme_loader_free(scheme_loader *loader)
{
    // Free list of handles.
    _scheme_loader_item_free(loader->handlesList);
    // Free loader.
    free(loader);
}
