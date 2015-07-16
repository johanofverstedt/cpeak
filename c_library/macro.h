
#ifndef CPEAK_MACRO_H
#define CPEAK_MACRO_H

#ifndef MINIMUM
#define MINIMUM(X, Y) (((Y) < (X)) ? (Y) : (X))
#endif

#ifndef MAXIMUM
#define MAXIMUM(X, Y) (((X) > (Y)) ? (X) : (Y))
#endif

// function pointer definition macro

#define FPTR(name, return_type, ...) return_type (*name)(##__VA_ARGS__)

#endif
