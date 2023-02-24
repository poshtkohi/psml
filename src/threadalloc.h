
/*
 * Copyright (C) 2003 - 2006 by Konstantin Knizhnik
 * Copyright (C) 2006  McObject LLC
 *
 * Permission to use, copy, modify, and distribute this software and its documentation for any purpose
 * and without fee is hereby granted, provided that the above copyright notices appear in all copies and
 * that the copyright notices and this permission notice appear in supporting documentation, and that the
 * name of the copyright holders not be used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 * 
 * The copyright holders make no representations about the suitability of this software for any purpose
 * It is provided "as is" without express or implied warranty.
 */

#ifndef __THREAD_ALLOC_H__
#define __THREAD_ALLOC_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* thread_alloc(size_t size);
void* thread_realloc(void* addr, size_t size);
void  thread_free(void* addr);

#ifdef __cplusplus
}

#include <new>

#ifdef REDEFINE_DEFAULT_NEW_OPERATOR

void* operator new(size_t size) throw(std::bad_alloc) { return thread_alloc(size); }
void operator delete(void* addr) throw() { thread_free(addr); }

void* operator new[](size_t size) throw(std::bad_alloc) { return thread_alloc(size); }
void operator delete[](void* addr) throw() { thread_free(addr); }

#endif

#endif

#endif

