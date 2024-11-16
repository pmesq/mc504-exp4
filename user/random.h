#include "kernel/types.h"
#include "user/user.h"

const int _RANDOM_P = 2557, _RANDOM_Q = 9883, _RANDOM_M = 21559;
int _random_x = -1;

/*
  Returns a pseudo-random integer in the range [l, r).
  [l, r] must satisfy: 0 <= l <= r < 21559.
*/
int random(int l, int r) {
  if (_random_x == -1) _random_x = uptime();
  _random_x = (_random_x * _RANDOM_P + _RANDOM_Q) % _RANDOM_M;
  return _random_x % (r - l + 1) + l;
}
