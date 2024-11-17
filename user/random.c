#include "random.h"

const int P = 2557, Q = 9883, M = 21559;
int x = -1;

int random(int l, int r) {
  if (x == -1) x = uptime();
  x = (x * P + Q) % M;
  return x % (r - l + 1) + l;
}
