
#ifndef U32_ARRAY_H
#define U32_ARRAY_H

#include <assert.h>
#include "types.h"
#include "macro.h"
#include "alloc.h"

typedef struct array_u32 {
  u32*      ptr;
  size_type count;
} array_u32;

typedef struct array_reversed_u32 {
  u32*      ptr;
  size_type count;
} array_reversed_u32;

typedef struct array_slice_u32 {
  u32*       ptr;
  size_type  count;
  index_type step;
} array_slice_u32;



inline
array_reversed_u32 reverse(array_u32 arr) {
  array_reversed_u32 result;

  result.ptr   = arr.ptr + arr.count;
  result.count = arr.count;

  return result;
}

inline
array_u32 reverse(array_reversed_u32 arr) {
  array_u32 result;

  result.ptr   = arr.ptr - arr.count;
  result.count = arr.count;

  return result;
}

inline
array_slice_u32 reverse(array_slice_u32 arr) {
  array_slice_u32 result;

  result.ptr = arr.ptr + arr.step * arr.count;
  result.count = arr.count;
  result.step = -arr.step;

  return result;
}

inline
array_u32 take(array_u32 arr, size_type count) {
  array_u32 result;

  assert(arr.count >= count);

  result.ptr   = arr.ptr;
  result.count = count;

  return result;
}

inline
array_u32 take_at_most(array_u32 arr, size_type count) {
  array_u32 result;

  result.ptr = arr.ptr;
  
  if(arr.count <= count) {
    result.count = arr.count;
  } else {
    result.count = count;
  }

  return result;
}

inline
array_u32 drop(array_u32 arr, size_type count) {
  array_u32 result;

  assert(arr.count >= count);

  result.ptr   = arr.ptr + count;
  result.count = arr.count - count;

  return result;
}

inline
array_u32 drop_at_most(array_u32 arr, size_type count) {
  array_u32 result;

  if(arr.count <= count) {
    result.ptr = arr.ptr + arr.count;
    result.count = 0U;
  } else {
    result.ptr = arr.ptr + count;
    result.count = arr.count - count;
  }

  return result;
}

inline
array_u32 add(allocator a, array_u32 x, array_u32 y) {
  array_u32 result;
  
  result.count = MINIMUM(x.count, y.count);
  result.ptr   = (u32*)cpeak_alloc(a, result.count * sizeof(u32));
  
  for(size_type i = 0; i < result.count; ++i) {
    result.ptr[i] = x.ptr[i] + y.ptr[i];
  }

  return result;
}

inline
array_u32 mul(allocator a, array_u32 x, array_u32 y) {
  array_u32 result;

  result.count = MINIMUM(x.count, y.count);
  result.ptr   = (u32*)cpeak_alloc(a, result.count * sizeof(u32));

  for(size_type i = 0; i < result.count; ++i) {
    result.ptr[i] = x.ptr[i] * y.ptr[i];
  }
  
  return result;
}

inline
array_u32 sub(allocator a, array_u32 x, array_u32 y) {
  array_u32 result;

  result.count = MINIMUM(x.count, y.count);
  result.ptr   = (u32*)cpeak_alloc(a, result.count * sizeof(u32));

  for(size_type i = 0; i < result.count; ++i) {
    result.ptr[i] = x.ptr[i] - y.ptr[i];
  }
  
  return result;
}

inline
array_u32 div(allocator a, array_u32 x, array_u32 y) {
  array_u32 result;

  result.count = MINIMUM(x.count, y.count);
  result.ptr   = (u32*)cpeak_alloc(a, result.count * sizeof(u32));

  for(size_type i = 0; i < result.count; ++i) {
    result.ptr[i] = x.ptr[i] / y.ptr[i];
  }
  
  return result;
}

inline
array_u32 mod(allocator a, array_u32 x, array_u32 y) {
  array_u32 result;

  result.count = MINIMUM(x.count, y.count);
  result.ptr   = (u32*)cpeak_alloc(a, result.count * sizeof(u32));

  for(size_type i = 0; i < result.count; ++i) {
    result.ptr[i] = x.ptr[i] % y.ptr[i];
  }
  
  return result;
}

inline
array_u32 right_shift(allocator a, array_u32 x, array_u32 y) {
  array_u32 result;

  result.count = MINIMUM(x.count, y.count);
  result.ptr   = (u32*)cpeak_alloc(a, result.count * sizeof(u32));

  for(size_type i = 0; i < result.count; ++i) {
    result.ptr[i] = (x.ptr[i]) >> (y.ptr[i]);
  }
  
  return result;
}

inline
array_u32 left_shift(allocator a, array_u32 x, array_u32 y) {
  array_u32 result;

  result.count = MINIMUM(x.count, y.count);
  result.ptr   = (u32*)cpeak_alloc(a, result.count * sizeof(u32));

  for(size_type i = 0; i < result.count; ++i) {
    result.ptr[i] = (x.ptr[i]) << (y.ptr[i]);
  }
  
  return result;
}

inline
array_u32 and(allocator a, array_u32 x, array_u32 y) {
  array_u32 result;

  result.count = MINIMUM(x.count, y.count);
  result.ptr   = (u32*)cpeak_alloc(a, result.count * sizeof(u32));

  for(size_type i = 0; i < result.count; ++i) {
    result.ptr[i] = (x.ptr[i]) & (y.ptr[i]);
  }
  
  return result;
}

inline
array_u32 or(allocator a, array_u32 x, array_u32 y) {
  array_u32 result;

  result.count = MINIMUM(x.count, y.count);
  result.ptr   = (u32*)cpeak_alloc(a, result.count * sizeof(u32));

  for(size_type i = 0; i < result.count; ++i) {
    result.ptr[i] = (x.ptr[i]) | (y.ptr[i]);
  }
  
  return result;
}

inline
array_u32 xor(allocator a, array_u32 x, array_u32 y) {
  array_u32 result;

  result.count = MINIMUM(x.count, y.count);
  result.ptr   = (u32*)cpeak_alloc(a, result.count * sizeof(u32));

  for(size_type i = 0; i < result.count; ++i) {
    result.ptr[i] = (x.ptr[i]) ^ (y.ptr[i]);
  }
  
  return result;
}

inline
array_u32 zero_u32(allocator a, size_type count) {
  array_u32 result;

  result.ptr   = (u32*)cpeak_alloc(a, count * sizeof(u32));
  result.count = count;

  memset(result.ptr, 0, count * sizeof(u32));

  return result;
}

inline
array_u32 iota_u32(allocator a, size_type count) {
  array_u32 result;

  result.ptr   = (u32*)cpeak_alloc(a, count * sizeof(u32));
  result.count = count;

  for(size_type i = 0; i < count; ++i) {
    result.ptr[i] = (u32)i;
  }

  return result;
}

inline
void print(array_u32 arr) {
  if(arr.count == 0U) {
    printf("[]");
  } else {
    printf("[%u", arr.ptr[0]);
    for(size_type i = 1; i < arr.count; ++i) {
      printf(", %u", arr.ptr[i]);
    }

    printf("]");
  }
}

#endif
