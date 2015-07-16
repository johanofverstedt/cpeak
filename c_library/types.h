
#ifndef CPEAK_TYPES_H
#define CPEAK_TYPES_H

// integral types

typedef unsigned char      u8;
typedef signed char        i8;
typedef unsigned short     u16;
typedef signed short       i16;
typedef unsigned int       u32;
typedef signed int         i32;
typedef unsigned long long u64;
typedef signed long long   i64;

// floating point types

typedef float       f32;
typedef double      f64;
typedef long double f128;

#ifdef CPEAK_64BIT
typedef u64 size_type;
typedef i64 index_type;
#else
typedef u32 size_type;
typedef i32 index_type;
#endif

#endif
