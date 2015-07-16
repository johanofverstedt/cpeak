
#ifndef CPEAK_ALLOC_H
#define CPEAK_ALLOC_H

#include "types.h"
#include "macro.h"

// allocator function-pointer type aliases

typedef FPTR(alloc_fptr, void*, void*, size_type);
typedef FPTR(free_fptr, void, void*, void*);
typedef FPTR(realloc_fptr, void*, void*, void*, size_type);
typedef FPTR(push_fptr, void, void*);
typedef FPTR(pop_fptr, void, void*);

typedef struct allocator_interface {
  alloc_fptr   alloc_impl;
  free_fptr    free_impl;
  realloc_fptr realloc_impl;
  push_fptr    push_impl;
  pop_fptr     pop_impl;
} allocator_interface;

typedef struct allocator {
  allocator_interface* functions;
  void*                data;
} allocator;

inline
void* cpeak_alloc(allocator a, size_type size) {
  void* result;

  result = a.functions->alloc_impl(a.data, size);

  return result;
}

inline
void cpeak_free(allocator a, void* ptr) {
  a.functions->free_impl(a.data, ptr);
}

inline
void* cpeak_realloc(allocator a, void* ptr, size_type new_size) {
  void* result;

  result = a.functions->realloc_impl(a.data, ptr, new_size);

  return result;
}

inline
void cpeak_push(allocator a) {
  a.functions->push_impl(a.data);
}

inline
void cpeak_pop(allocator a) {
  a.functions->pop_impl(a.data);
}

// the standard malloc/free/realloc allocator

extern
allocator get_std_alloc();

extern
allocator std_alloc;

#endif
