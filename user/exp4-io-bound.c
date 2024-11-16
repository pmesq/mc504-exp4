#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "user/random.h"

#define line_size 100
#define num_lines 100
char file[12] = "ioboundpath";

void swap(int * a, int * b){
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

// escreve 100 vezes linhas de 100 caracteres em um arquivo
void write_lines(int fp){
  char buf[line_size + 5];
  buf[line_size] = '\n';
  buf[line_size + 1] = '\0';
  for(int i = 0; i < num_lines; i++){
    for(int j = 0; j < line_size; j++){
      buf[j] = random(32, 125);
    }
    write(fp, buf, line_size + 1);
  }
}

// executa 50 permutações entre linhas aleatórias de um arquivo
void permute_lines(int fp){
  int sz = (line_size + 1) * (num_lines + 1);
  char *buf = malloc(((line_size + 1) * (num_lines + 1)) * sizeof (char));

  // for(int i = 0; i < 50; i++){
    int l1 = random(0, 99);
    int l2 = random(0, 99);

    if(l1 > l2) {
      swap(&l1, &l2);
    }

    int rsz = read(fp, buf, sz);
    printf("%d\n", rsz);
    // write(1, buf, sz);

    for(int i = 0; i < num_lines; i++){
      for(int j = 0; j <= line_size; j++){
        write(1, &buf[i * (line_size + 1) + j], 1);
      }
    }
  // }
    free(buf);
}

int main(){
  int fp = open(file, O_WRONLY | O_CREATE);

  write_lines(fp);

  fp = open(file, O_RDWR);

  permute_lines(fp);

  return 0;
}