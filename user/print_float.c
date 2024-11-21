#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void print_float(long num, long den, int precision){
    printf("%ld.", num / den);

    long resto = num % den;
    for(int i = 0; i < precision; i++){
        resto *= 10;
        printf("%ld", resto / den);
        resto %= den;
    }
}