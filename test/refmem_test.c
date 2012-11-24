#include <glib.h>

#include <stdbool.h>
#include <setjmp.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <rehlib/common.h>

#include "refmem.c"

typedef struct {
	GSList *memory;
} RefmemTestFixture;

static RefmemTestFixture *fixture;

static void *test_malloc( size_t size ) {
	g_assert(fixture != NULL);
	void *mem = malloc(size);
	if( mem == NULL ) return NULL;
	fixture->memory = g_slist_prepend(fixture->memory, mem);
	return mem;
}

static void test_free( void *mem ) {
	g_assert(fixture != NULL);
	g_assert(g_slist_find(fixture->memory, mem) != NULL);
	fixture->memory = g_slist_remove(fixture->memory, mem);
	free(mem);
}

static void *test_realloc( void *mem, size_t size ) {
	g_assert(fixture != NULL);
	g_assert(g_slist_find(fixture->memory, mem) != NULL);
	void *mem2 = realloc(mem, size);
	if( mem2 == NULL ) return NULL;
	if( mem2 != mem ) {
		fixture->memory = g_slist_remove(fixture->memory, mem);
		fixture->memory = g_slist_prepend(fixture->memory, mem2);
	}
	return mem2;
}

static void test_setup( RefmemTestFixture *f, const void *data ) {
	(void) data;
	fixture = f;
	GMemVTable vt;
	memset(&vt, 0, sizeof(GMemVTable));
	vt.malloc = test_malloc;
	vt.free = test_free;
	vt.realloc = test_realloc;
	refmem_set_vtable(&vt);
}

static void test_teardown( RefmemTestFixture *f, const void *data ) {
	(void) data;
	g_slist_free_full(f->memory, (GDestroyNotify) free);
	fixture = NULL;
	refmem_default_vtable();
}

static void test_alloc( RefmemTestFixture *fixture, const void *data ) {
	(void) data;
	RefMemory *refmem = as_refmem(refmem_alloc(sizeof(char)));
	if( refmem->refcnt != 1 || g_slist_length(fixture->memory) != 1 || fixture->memory->data != refmem ) {
		g_test_fail();
		return;
	}
}

static void test_unref( RefmemTestFixture *fixture, const void *data ) {
	(void) data;
	void *mem = refmem_alloc(sizeof(char));
	refmem_unref(mem);
	if( g_slist_length(fixture->memory) != 0 ) {
		g_test_fail();
		return;
	}
}

static void test_ref( RefmemTestFixture *fixture, const void *data ) {
	(void) data;
	void *mem = refmem_alloc(sizeof(char));
	RefMemory *refmem = as_refmem(mem);
	refmem_ref(mem);
	if( refmem->refcnt != 2 ) {
		g_test_fail();
		return;
	}
	if( g_slist_length(fixture->memory) != 1 ) {
		g_test_fail();
		return;
	}
}

static void test_multi_ref_unref( RefmemTestFixture *fixture, const void *data ) {
	(void) data;
	void *mem = refmem_alloc(sizeof(char));
	RefMemory *refmem = as_refmem(mem);
	refmem_ref(mem);
	if( refmem->refcnt != 2 ) {
		g_test_fail();
		return;
	}
	refmem_unref(mem);
	if( refmem->refcnt != 1 ) {
		g_test_fail();
		return;
	}
	refmem_unref(mem);
	if( g_slist_length(fixture->memory) != 0 ) {
		g_test_fail();
		return;
	}
}

static void *test_oom_allocator() {
	errno = ENOMEM;
	return NULL;
}

static void test_oom_free() {
	g_assert_not_reached();
}

typedef struct {
	bool handler_called;
	jmp_buf handler_escape;
} RefmemOOMTestFixture;

static void test_oom_handler( RefmemOOMTestFixture *fixture ) {
	fixture->handler_called = true;
	longjmp(fixture->handler_escape, true);
	g_assert_not_reached();
}

static void test_oom_setup( RefmemOOMTestFixture *fixture, const void *data ) {
	(void) data;
	GMemVTable vt;
	memset(&vt, 0, sizeof(GMemVTable));
	vt.malloc = (void *(*)(size_t)) test_oom_allocator;
	vt.free = (void (*)(void *)) test_oom_free;
	vt.realloc = (void *(*)(void *, size_t)) test_oom_allocator;
	refmem_set_vtable(&vt);

	fixture->handler_called = false;
	refmem_set_oom_handler((OOMHandler) test_oom_handler, fixture);
}

static void test_oom_teardown( RefmemOOMTestFixture *fixture, const void *data ) {
	(void) data, (void) fixture;
	refmem_default_vtable();
	refmem_default_oom_handler();
}

static void test_oom_try_alloc( RefmemOOMTestFixture *fixture, const void *data ) {
	(void) data;
	if( !setjmp(fixture->handler_escape) ) {
		GBTError *err = NULL;
		void *mem = refmem_try_alloc(sizeof(char), &err);
		if( errno != ENOMEM || mem != NULL || fixture->handler_called || err == NULL || err->gerror->code != REFMEM_ERROR_ERRNO ) {
			g_test_fail();
			g_bt_error_free(&err);
			return;
		}
		g_bt_error_free(&err);
	} else {
		g_test_fail();
		return;
	}
}

static void test_oom_alloc( RefmemOOMTestFixture *fixture, const void *data ) {
	(void) data;
	if( !setjmp(fixture->handler_escape) ) {
		refmem_alloc(sizeof(char));
		g_test_fail();
		return;
	} else {
		if( errno != ENOMEM || !fixture->handler_called ) {
			g_test_fail();
			return;
		}
	}
}

void refmem_register_tests() {
	g_test_add("/refmem/oom/try_alloc", RefmemOOMTestFixture, NULL, test_oom_setup, test_oom_try_alloc, test_oom_teardown);
	g_test_add("/refmem/oom/alloc", RefmemOOMTestFixture, NULL, test_oom_setup, test_oom_alloc, test_oom_teardown);
	g_test_add("/refmem/alloc", RefmemTestFixture, NULL, test_setup, test_alloc, test_teardown);
	g_test_add("/refmem/unref", RefmemTestFixture, NULL, test_setup, test_unref, test_teardown);
	g_test_add("/refmem/ref", RefmemTestFixture, NULL, test_setup, test_ref, test_teardown);
	g_test_add("/refmem/multi_ref_unref", RefmemTestFixture, NULL, test_setup, test_multi_ref_unref, test_teardown);
}
