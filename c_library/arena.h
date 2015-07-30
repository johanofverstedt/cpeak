
/**
 *  arena.h
 *
 *  An arena based memory allocator for both aligned and packed allocations.
 *  Doesn't support the random-access "free" operation - allocations
 *  are freed through push/pop-operations.
 *
 *  Has (2) failure modes when out of memory which can be set by #defines:
 *  ARENA_FAIL_NORMAL - asserts that the out of memory condition is an error.
 *  ARENA_FAIL_NULL   - returns a null-pointer when out of memory. Slower in release builds.
 */

#ifndef ARENA_H
#define ARENA_H

#include "types.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

//
// struct for the arena header
// stored at the beginning of the arena's memory region
//
typedef struct arena_head {
  usize tail;
  usize last;
  usize last_push;
  usize size;
} arena_head;

//
// struct for the auxiliary push/pop linked list
//
typedef struct arena_push_entry {
  usize prev_push_entry;
  usize prev_last;
} arena_push_entry;

typedef arena_head* arena;

#define ALIGN_USIZE_16(X) (((X) + (usize)15U) & (~((usize)15U)))

inline
arena arena_init(void* memory, usize size) {
  arena result = (arena)memory;

  usize ptr_int = (usize)memory;
  usize initial_tail = ALIGN_USIZE_16(ptr_int + sizeof(arena_head)) - ptr_int;

  result->tail = initial_tail;
  result->last = 0U;
  result->last_push = 0U;
  result->size = size;

  return result;
}

inline
void* arena_alloc(arena a, usize size) {
  void* result;
  u8* byte_ptr = (u8*)a;
  usize cur_tail = a->tail;

  result = (void*)(byte_ptr + cur_tail);

  usize new_tail = cur_tail + ALIGN_USIZE_16(size);

  assert(cur_tail + size <= a->size);

  // update arena head
  a->tail = new_tail;
  a->last = cur_tail;

  return result;
}

inline
void* arena_alloc_packed(arena a, usize size) {
  void* result;
  u8* byte_ptr = (u8*)a;
  usize cur_tail = a->tail;

  result = (void*)(byte_ptr + cur_tail);

  usize new_tail = cur_tail + size;

  assert(new_tail <= a->size);

  // update arena head
  a->tail = new_tail;
  a->last = cur_tail;

  return result;  
}

inline
void* arena_try_alloc(arena a, usize size) {
  void* result;
  usize cur_tail = a->tail;
  usize arena_size = a->size;

  usize remaining_size = arena_size - cur_tail;

  if(remaining_size >= size) {
    usize aligned_size = ALIGN_USIZE_16(size);
    usize new_tail = cur_tail + aligned_size;

    a->tail = new_tail;
    a->last = cur_tail;
  } else {
    result = 0; // return a null pointer to signal 'out of memory'
  }

  return result;
}

inline
arena make_arena(arena a, usize size) {
  arena result;
  
  void* memory = arena_alloc(a, size);
  result = arena_init(memory, size);

  return result;
}

inline
arena make_system_arena(usize size) {
  arena result;

  void* memory = malloc(size);
  
  // note: error handling via assert, can't really recover on error anyway
  assert(memory);

  result = arena_init(memory, size);

  return result; 
}

inline
void free_system_arena(arena a) {
  free(a);
}

inline
void* arena_realloc(arena a, void* ptr, usize size) {
  void* result;

  if(ptr) {
    u8* param_byte_ptr = (u8*)ptr;
    u8* arena_byte_ptr = (u8*)a;

    usize cur_last = a->last;

    usize ptr_offset = (usize)(param_byte_ptr - arena_byte_ptr);

    // if the provided pointer is to the last region, just update its size by adjusting the tail offset.
    
    if(ptr_offset == cur_last) {
      usize aligned_size = ALIGN_USIZE_16(size);
      usize new_tail = cur_last + aligned_size;

      a->tail = new_tail;

      result = ptr;
      return result;
    }
  }

  // if we for some reason arrived here, we can't realloc so
  // we fall back on a normal allocation.

  result = arena_alloc(a, size);

  return result;
}

inline
void arena_realign(arena a) {
  u8* arena_byte_ptr = (u8*)a;

  usize cur_tail = a->tail;
  usize ptr_int = (usize)(arena_byte_ptr + cur_tail);

  usize aligned_ptr_int = ALIGN_USIZE_16(ptr_int);
  usize aligned_tail = cur_tail + (aligned_ptr_int - ptr_int);

  a->tail = aligned_tail;
}

inline
void arena_reset(arena a) {
  arena_init((void*)a, a->size);
}

inline
void arena_push(arena a) {
  usize tail = a->tail;
  arena_push_entry* pe = (arena_push_entry*)arena_alloc(a, sizeof(arena_push_entry));

  pe->prev_push_entry = a->last_push; // form a linked list stack of push entries
  pe->prev_last = a->last; // remember that start offset of the last allocated region

  a->last_push = tail; // update the head reference in the push stack
}

inline
void arena_pop(arena a) {
  u8* byte_ptr = (u8*)a;
  usize last_push = a->last_push;

  // precondition: the state-stack contains at least one item
  assert(last_push != 0U);

  arena_push_entry* pe = (arena_push_entry*)(byte_ptr + last_push);

  a->tail = last_push;
  a->last = pe->prev_last;
  a->last_push = pe->prev_push_entry;
}

inline
void arena_pop_erase(arena a) {
  u8* byte_ptr = (u8*)a;
  usize last_push = a->last_push;
  usize cur_tail = a->tail;

  // precondition: the state-stack contains at least one item
  assert(last_push != 0U);

  arena_push_entry* pe = (arena_push_entry*)(byte_ptr + last_push);

  a->tail = last_push;
  a->last = pe->prev_last;
  a->last_push = pe->prev_push_entry;

  memset(byte_ptr + last_push, 0, cur_tail - last_push);
}

// debug functions

#include <stdio.h>

inline
char arena_debug_hex_digit_to_char(u8 hex_digit) {
  assert(hex_digit < 16U);

  if(hex_digit < 10U)
    return '0' + hex_digit;
  else
    return 'a' + (hex_digit - 10U);
}

inline
void arena_debug_u8_to_hex(char* dest, u8 byte_value) {
  u8 first_digit = byte_value >> 4U;
  u8 second_digit = byte_value & 15U;

  dest[0] = arena_debug_hex_digit_to_char(first_digit);
  dest[1] = arena_debug_hex_digit_to_char(second_digit);
  dest[2] = '\0';
}

inline
void arena_memdump(FILE* f, arena a) {
  usize size = a->size;
  usize cur_tail = a->tail;

  usize ptr_int = (usize)a;
  usize initial_tail = ALIGN_USIZE_16(ptr_int + sizeof(arena_head)) - ptr_int;

  u8* byte_ptr = (u8*)a;
  u8* end_ptr = byte_ptr + size;

  // skip header and padding
  byte_ptr += initial_tail;

  while(byte_ptr != end_ptr) {
    char str[3];

    u32 byte_value = (u32)(*byte_ptr);

    arena_debug_u8_to_hex(str, byte_value);    

    fwrite(str, sizeof(char), 2, f);

    ++byte_ptr;
  }
}

#endif
