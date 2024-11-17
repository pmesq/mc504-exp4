#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void print_float(int num, int den, int precision){
    int pot = 1;
    for(int i = 0; i < precision; i++) pot *= 10;

    int piso = (num / den);
    int n = (num * pot) / den;

    printf("%d", piso);
    printf(",");
    printf("%d", n - piso * pot);
}