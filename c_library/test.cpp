
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "alloc.h"
#include "array_u32.h"

int main(int argc, char** argv) {
  
  allocator ai = std_alloc;

  array_u32 y1 = iota_u32(ai, 16);

  array_u32 x1 = zero_u32(ai, 16);

  print(x1);
  print(mul(ai, y1, add(ai, x1, y1)));

  return 0;
}