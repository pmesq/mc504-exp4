#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "user/random.h"
#include "user/exp4-io-bound.h"

#define line_size 100
#define num_lines 100
char file[12] = "ioboundpath";

void swap(char * a, char * b){
  char tmp = *a;
  *a = *b;
  *b = tmp;
}

// escreve 100 vezes linhas de 100 caracteres em um arquivo
void write_lines(int * file_time){
  int fp = open(file, O_WRONLY | O_CREATE);

  char buf[line_size + 5];
  buf[line_size] = '\n';
  buf[line_size + 1] = '\0';
  for(int i = 0; i < num_lines; i++){
    for(int j = 0; j < line_size; j++){
      buf[j] = random(32, 125);
    }

    int ini = uptime();
    write(fp, buf, line_size + 1);
    int fim = uptime();
    * file_time += fim - ini;
  }

  close(fp);
}

// executa 50 permutações entre linhas aleatórias de um arquivo
void permute_lines(int * mem_time, int * file_time){
  int sz = (line_size + 1) * (num_lines + 1);

  int ini = uptime();
  char *buf = malloc(((line_size + 1) * (num_lines + 1)) * sizeof (char));
  int fim = uptime();
  * mem_time += fim - ini;

  for(int i = 0; i < 50; i++){
    int fp = open(file, O_RDONLY);

    ini = uptime();
    int rsz = read(fp, buf, sz);
    fim = uptime();
    * mem_time += fim - ini;
    
    close(fp);

    int l1 = random(0, 99);
    int l2 = random(0, 99);

    for(int i = 0; i < line_size; i++){
      swap(&buf[l1 * (line_size + 1) + i], &buf[l2 * (line_size + 1) + i]);
    }

    fp = open(file, O_WRONLY | O_TRUNC);

    ini = uptime();
    write(fp, buf, rsz);
    fim = uptime();
    * file_time += fim - ini;

    close(fp);
  }

  ini = uptime();
  free(buf);
  fim = uptime();
  * mem_time += fim - ini;
}

void io_bound(int * mem_time, int * file_time){
  write_lines(file_time);
  permute_lines(mem_time, file_time);

  int ini = uptime();
  unlink(file);
  int fim = uptime();
  * file_time += fim - ini;
}