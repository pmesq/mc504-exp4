#ifndef _PRINT_FLOAT_H
#define _PRINT_FLOAT_H

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/*
    Prints to stdin the fraction p/q, with precision of d digits.
*/
void print_float(long p, long q, int d);

#endif
