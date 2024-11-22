#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void print_float(long num, long den, int precision){
  printf("%ld.", num / den);
  long remainder = num % den;
  for(int i = 0; i < precision; i++){
    remainder *= 10;
    printf("%ld", remainder / den);
    remainder %= den;
  }
}