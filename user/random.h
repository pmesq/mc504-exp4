#ifndef _RANDOM_H
#define _RANDOM_H

#include "kernel/types.h"
#include "user/user.h"

/*
  Returns a pseudo-random integer in the range [l, r).
  [l, r] must satisfy: 0 <= l <= r < 21559.
*/
int random(int l, int r);

#endif
