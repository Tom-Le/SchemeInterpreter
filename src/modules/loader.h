/**
 * Scheme procedure loader.
 *
 * Manages procedures loaded from shared libraries that encapsulate Scheme procedures written in C.
 */
#ifndef __SCHEME_LOADER_H__
#define __SCHEME_LOADER_H__

#include "scheme-data-types.h"

// Procedure manager type.
typedef struct scheme_loader scheme_loader;

/**
 * Create a new loader.
 *
 * @return New loader, or NULL if out of memory.
 */
scheme_loader *scheme_loader_new();

/**
 * Load a file containing a Scheme procedure.
 *
 * @param  loader  Scheme loader to store the loaded procedure.
 * @param  path  Path to file.
 *
 * @return 1 if procedure has been successfully loaded, 0 if an error occurred.
 */
int scheme_loader_load_file(scheme_loader *loader, const char *path);

/**
 * Load files containing Scheme procedures from a folder.
 *
 * @param  loader  Scheme loader to store loaded procedures.
 * @param  path  Path to folder.
 *
 * @return Number of loaded procedures.
 */
int scheme_loader_load_folder(scheme_loader *loader, const char *path);

/**
 * Puts procedures stored in loader onto a namespace.
 *
 * @param  loader  Scheme loader.
 * @param  namespace  Scheme namespace to store procedures.
 */
void scheme_loader_put_onto_namespace(scheme_loader *loader, scheme_namespace *namespace);

/**
 * Unload all procedures in a loader and free it.
 *
 * WARNING: Do not free loader if there is at least one namespace that contains procedures
 * stored inside loader. Those procedures will be freed in the process, potentially crashing
 * the program!
 *
 * @param  loader  Scheme loader.
 */
void scheme_loader_free(scheme_loader *loader);

#endif
