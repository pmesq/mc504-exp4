#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "user/random.h"

#define line_size 100
#define num_lines 100
char file[12] = "ioboundpath";

void swap(char * a, char * b){
  char tmp = *a;
  *a = *b;
  *b = tmp;
}

// escreve 100 vezes linhas de 100 caracteres em um arquivo
void write_lines(){
  int fp = open(file, O_WRONLY | O_CREATE);

  char buf[line_size + 5];
  buf[line_size] = '\n';
  buf[line_size + 1] = '\0';
  for(int i = 0; i < num_lines; i++){
    for(int j = 0; j < line_size; j++){
      buf[j] = random(32, 125);
    }
    write(fp, buf, line_size + 1);
  }

  close(fp);
}

// executa 50 permutações entre linhas aleatórias de um arquivo
void permute_lines(){
  int sz = (line_size + 1) * (num_lines + 1);
  char *buf = malloc(((line_size + 1) * (num_lines + 1)) * sizeof (char));

  for(int i = 0; i < 50; i++){
    int fp = open(file, O_RDONLY);
    int rsz = read(fp, buf, sz);
    close(fp);

    int l1 = random(0, 99);
    int l2 = random(0, 99);

    for(int i = 0; i <= line_size; i++){
      swap(&buf[l1 * (line_size + 1) + i], &buf[l2 * (line_size + 1) + i]);
    }

    fp = open(file, O_WRONLY | O_TRUNC);
    write(fp, buf, rsz);
    close(fp);
  }

  free(buf);
}

int main(){
  write_lines();
  permute_lines();
  unlink(file);
  return 0;
}