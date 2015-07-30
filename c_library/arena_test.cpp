
#include "arena.h"
#include <stdio.h>

usize arena_offset(arena a, void* ptr) {
  usize result = (u8*)ptr - (u8*)a;

  return result;
}

void print_offsets(arena a, void** allocations, u32 allocation_count) {
  for(u32 i = 0U; i < allocation_count; ++i) {
    printf("a[%u] offset: %u\n", i, arena_offset(a, allocations[i]));
  }  
}

int main(int argc, char** argv) {
  arena a = make_system_arena(1024);

  printf("System native width: %u\n\n", sizeof(void*));
  printf("System usize width: %u\n\n", sizeof(usize));

  const u32 alloc_count = 6U;

  void* aa[alloc_count];

  aa[0] = arena_alloc(a, 24U);

  u8 values[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256};

  memcpy(aa[0], values, sizeof(u8) * 9);

  arena_push(a);

  aa[1] = arena_alloc(a, 16U);
  aa[2] = arena_alloc(a, 32U);
  aa[3] = arena_alloc(a, 8U);
  aa[4] = arena_alloc(a, 0U);
  aa[5] = arena_alloc(a, 32U);
  
  print_offsets(a, aa, alloc_count);

  arena_pop(a);

  aa[1] = arena_alloc(a, 16U);
  aa[2] = arena_alloc(a, 32U);

  arena_push(a);

  aa[3] = arena_alloc(a, 8U);
  aa[4] = arena_alloc(a, 0U);
  aa[5] = arena_alloc(a, 32U);

  print_offsets(a, aa, alloc_count);

  memcpy(aa[5], values, sizeof(u8) * 9);

  fflush(stdout);
  arena_memdump(stdout, a);

  printf("\n\n\n");

  arena_pop_erase(a);

  fflush(stdout);
  arena_memdump(stdout, a);

  return 0;
}