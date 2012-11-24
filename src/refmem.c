#include "refmem.h"

#include "GBTError.h"

#include <glib.h>
#include <gmodule.h>

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>

#include "_common.h"

typedef struct {
	volatile gint refcnt;
	guchar data[];
} RefMemory;

static GMemVTable vtable = {
	.malloc = malloc,
	.realloc = realloc,
	.free = free,
	.calloc = calloc,
	.try_malloc = malloc,
	.try_realloc = realloc
};

const gchar *refmem_module_check_init( GModule *module ) {
	g_module_make_resident(module);
	return NULL;
}

static GQuark refmem_error_quark() {
	return g_quark_from_static_string("refmem-error-quark");
}

noreturn static void oom() {
	g_error("%s\n", g_strerror(errno));
	g_assert_not_reached();
}

static OOMHandler oom_handler = (OOMHandler) oom;
static void *oom_context;

static RefMemory *as_refmem( void *mem ) {
	return (void *) (((char *) mem) - offsetof(RefMemory, data));
}

static void *as_mem( RefMemory *mem ) {
	return (void *) (((char *) mem) + offsetof(RefMemory, data));
}

export void refmem_set_oom_handler( OOMHandler handler, void *context ) {
	static GMutex mutex;

	g_mutex_lock(&mutex);
	oom_handler = handler;
	oom_context = context;
	g_mutex_unlock(&mutex);
}

export void refmem_default_oom_handler() {
	refmem_set_oom_handler((OOMHandler) oom, NULL);
}

static void *_refmem_alloc( void *(*_malloc)( gsize ), gsize size ) {
	RefMemory *refmem = _malloc(sizeof(RefMemory) + size);
	if( refmem == NULL ) return NULL;
	refmem->refcnt = 1;
	return as_mem(refmem);
}

export void *refmem_alloc( gsize size ) {
	void *ret = _refmem_alloc(vtable.malloc, size);
	if( ret == NULL ) {
		oom_handler(oom_context);
		g_assert_not_reached();
	}
	return ret;
}

export void *refmem_try_alloc( gsize size, GBTError **error ) {
	void *mem = _refmem_alloc(vtable.try_malloc, size);
	if( mem == NULL ) {
		int saved_errno = errno;
		g_bt_set_error(error, refmem_error_quark(), REFMEM_ERROR_ERRNO, "%s", g_strerror(errno));
		errno = saved_errno;
		return NULL;
	}
	return mem;
}

static void *_refmem_realloc( void *(*_realloc)( void *, gsize ), void *mem, gsize size ) {
	RefMemory *refmem = as_refmem(mem);
	if( refmem->refcnt != 1 ) {
		g_warning("Attempt to realloc memory with refcnt > 1");
		return NULL;
	}
	return _realloc(refmem, sizeof(RefMemory) + size);
}

export void *refmem_realloc( void *mem, gsize size ) {
	mem = _refmem_realloc(vtable.realloc, mem, size);
	if( mem == NULL ) {
		oom_handler(oom_context);
		g_assert_not_reached();
	}
	return mem;
}

export void *refmem_try_realloc( void *mem, gsize size, GBTError **error ) {
	mem = _refmem_realloc(vtable.try_realloc, mem, size);
	if( mem == NULL ) {
		int saved_errno = errno;
		g_bt_set_error(error, refmem_error_quark(), REFMEM_ERROR_ERRNO, "%s", g_strerror(errno));
		errno = saved_errno;
		return NULL;
	}
	return mem;
}

export void refmem_ref( void *mem ) {
	RefMemory *refmem = as_refmem(mem);
	g_atomic_int_inc(&refmem->refcnt);
}

export void refmem_unref( void *mem ) {
	RefMemory *refmem = as_refmem(mem);
	if( g_atomic_int_dec_and_test(&refmem->refcnt) ) {
		vtable.free(refmem);
	}
}

static void *_refmem_calloc( gsize count, gsize size ) {
	// TODO: Check for overflows.
	void *ret = vtable.malloc(count * size);
	if( ret == NULL ) return NULL;
	memset(ret, 0, count * size);
	return ret;
}

export void refmem_set_vtable( GMemVTable *vt ) {
	static GMutex mutex;

	g_mutex_lock(&mutex);
	memcpy(&vtable, vt, sizeof(GMemVTable));
	if( vtable.try_malloc == NULL ) vtable.try_malloc = vtable.malloc;
	if( vtable.try_realloc == NULL ) vtable.try_realloc = vtable.realloc;
	if( vtable.calloc == NULL ) vtable.calloc = _refmem_calloc;
	g_mutex_unlock(&mutex);
}

export void refmem_default_vtable() {
	GMemVTable vtable;
	memset(&vtable, 0, sizeof(GMemVTable));
	vtable.malloc = malloc;
	vtable.realloc = realloc;
	vtable.free = free;
	vtable.calloc = calloc;
	vtable.try_malloc = malloc;
	vtable.try_realloc = realloc;
	refmem_set_vtable(&vtable);
}
