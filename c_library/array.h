
#ifndef CPEAK_ARRAY_H
#define CPEAK_ARRAY_H

#include <string.h>

typedef unsigned char u8;

typedef struct array {
  void*  ptr;
  size_t count;
} array;

typedef struct array_reversed {
  void*  ptr;
  size_t count;
} array_reversed;

typedef struct array_slice {
  void*  ptr;
  size_t count;
  size_t step;
} array_slice;

typedef struct array_slice_reversed {
  void*  ptr;
  size_t count;
  size_t step;
} array_reversed_slice;

typedef array                a;
typedef array_reversed       ar;
typedef array_slice          as;
typedef array_slice_reversed asr;

inline
array_reversed reverse(array a, size_t elem_size) {
  array_reversed result;

  u8* byte_ptr = (u8*)a.ptr;
  byte_ptr += (a.count * elem_size);

  result.ptr = (void*)byte_ptr;
  result.count = a.count;

  return result;
}

inline
array reverse(array_reversed a, size_t elem_size) {
  array result;

  u8* byte_ptr = (u8*)a.ptr;
  byte_ptr -= (a.count * elem_size);

  result.ptr = (void*)byte_ptr;
  result.count = a.count;

  return result;
}

inline
array_slice slice(array a, size_t step) {
  array_slice result;

  return result;
}

inline
void zero_inplace(array a, size_t elem_size) {
  memset(a.ptr, 0, elem_size * a.count);
}

inline
void zero_inplace(array_reversed a, size_t elem_size) {
  zero_inplace(reverse(a, elem_size), elem_size);
}

inline
void zero_inplace(array_slice a, size_t elem_size) {
  
}

#endif
