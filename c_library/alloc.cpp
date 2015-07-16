
#include <stdlib.h>
#include "alloc.h"

//
// cstdlib allocator wrapper functions
//

void* cstdlib_alloc_wrapper(void*, size_type size) {
  void* result = malloc(size);
  
  return result;
}

void cstdlib_free_wrapper(void*, void* ptr) {
  return free(ptr);
}

void* cstdlib_realloc_wrapper(void*, void* ptr, size_type new_size) {
  void* result = realloc(ptr, new_size);

  return result;
}

void cstdlib_push_and_pop_wrapper(void*) {
  ; //no-op
}

// global variable

allocator_interface std_alloc_impl =
  {
    cstdlib_alloc_wrapper,
    cstdlib_free_wrapper,
    cstdlib_realloc_wrapper,
    cstdlib_push_and_pop_wrapper,
    cstdlib_push_and_pop_wrapper
  };

allocator std_alloc = { &std_alloc_impl, 0 };

extern
allocator get_std_alloc() {
  allocator result = std_alloc;

  return result;
}
