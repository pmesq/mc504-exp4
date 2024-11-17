#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void print_float(int num, int den, int precision){
    printf("%d.", num / den);

    int resto = num % den;
    for(int i = 0; i < precision; i++){
        resto *= 10;
        printf("%d", resto / den);
        resto %= den;
    }
}