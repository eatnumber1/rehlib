#ifndef _REFMEM_H_
#define _REFMEM_H_

#include <rehlib/GBTError.h>

#include <glib.h>

typedef enum {
	REFMEM_ERROR_INVALID,
	REFMEM_ERROR_ERRNO
} RefmemError;

void *refmem_alloc( gsize size );
void *refmem_try_alloc( gsize size, GBTError **error );
void *refmem_realloc( void *mem, gsize size );
void *refmem_try_realloc( void *mem, gsize size, GBTError **error );

void refmem_ref( void *mem );
void refmem_unref( void *mem );

void refmem_set_vtable( GMemVTable *vtable );
void refmem_default_vtable();

typedef void (*OOMHandler)( void * );
void refmem_set_oom_handler( OOMHandler handler, void *context );
void refmem_default_oom_handler();

#endif
