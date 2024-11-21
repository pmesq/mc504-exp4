#include "kernel/types.h"
#include "user/user.h"
#include "user/random.h"
#include "user/print_float.h"
#include "user/exp4-cpu-bound.h"
#include "user/exp4-io-bound.h"

const int ROUNDS = 30;

int max(int a, int b){
  if(a > b) return a;
  return b;
}

int min(int a, int b){
  if(a < b) return a;
  return b;
}

int main() {
  for (int round = 1; round <= ROUNDS; ++round) {
    int tot_time = 0, mem_time = 0, file_time = 0, time;
    
    printf("RODADA %d\n", round);

    // int x = 1, y = 1;
    int x = random(6, 14), y = 20 - x;
    int proc_count = x + y;

    printf("%d processos CPU-bound\n", x);
    printf("%d processos IO-bound\n", y);

    int *proc_times = malloc(proc_count * sizeof (int));

    for (int i = 0; i < proc_count; ++i) {
      int fd_tot[2], fd_mem[2], fd_file[2];
      pipe(fd_tot);
      pipe(fd_mem);
      pipe(fd_file);
      int pid = fork();
      if (pid == -1) {
        fprintf(2, "error forking process %d\n", i);
        exit(1);
      }
      // filho realiza os processos
      else if (pid == 0) {
        close(fd_tot[0]);
        close(fd_mem[0]);
        close(fd_file[0]);
        int proc_mem_time = 0, proc_file_time = 0;
        time = uptime();

        // intercala processos
        if ((i < 2 * x && i % 2 == 0) || i >= 2 * y + 1) {
          cpu_bound(&proc_mem_time);
        } else {
          io_bound(&proc_mem_time, &proc_file_time);
        }

        int proc_tot_time = uptime() - time;
        write(fd_tot[1], &proc_tot_time, sizeof (proc_tot_time));
        write(fd_mem[1], &proc_mem_time, sizeof (proc_mem_time));
        write(fd_file[1], &proc_file_time, sizeof (proc_file_time));
        close(fd_tot[1]);
        close(fd_mem[1]);
        close(fd_file[1]);
        exit(0);
      }
      // pai calcula os tempos
      else {
        close(fd_tot[1]);
        close(fd_mem[1]);
        close(fd_file[1]);
        int proc_tot_time = 0, proc_mem_time = 0, proc_file_time = 0;
        read(fd_tot[0], &proc_tot_time, sizeof (proc_tot_time));
        read(fd_mem[0], &proc_mem_time, sizeof (proc_mem_time));
        read(fd_file[0], &proc_file_time, sizeof (proc_file_time));
        proc_times[i] = proc_tot_time;
        tot_time += proc_tot_time;
        mem_time += proc_mem_time;
        file_time += proc_file_time;
        close(fd_tot[0]);
        close(fd_mem[0]);
        close(fd_file[0]);
        wait(0);
      }
    }

    // transforma em segundos
    tot_time /= 10;
    mem_time /= 10;
    file_time /= 10;
    for (int i = 0; i < proc_count; ++i) {
      proc_times[i] /= 10;
    }

    printf("Processos por segundo: ");
    print_float(proc_count, tot_time, 5);
    printf("\n");


    printf("Vazão: ");
    const int C = 10000;
    int trgput_sum = 0, t_max = 0, t_min = 2000000000;
    
    for(int i = 0; i < proc_count; i++){
      trgput_sum += C / proc_times[i];
      t_max = max(t_max, proc_times[i]);
      t_min = min(t_min, proc_times[i]);
    }

    int trgput_num = t_min * t_max * trgput_sum - proc_count * C * t_min;
    int trgput_den = proc_count * C * (t_max - t_min);
    trgput_num = trgput_den - trgput_num;

    print_float(trgput_num, trgput_den, 5);
    printf("\n");

    int fairness_num = 0, fairness_den = 0;
    for (int i = 0; i < proc_count; ++i) {
      fairness_num += proc_times[i];
      fairness_den += proc_times[i] * proc_times[i];
    }
    fairness_num *= fairness_num;
    fairness_den *= proc_count;

    printf("Justica entre processos: ");
    print_float(fairness_num, fairness_den, 5);
    printf("\n");

    printf("Eficiencia do sistema de arquivos: ");
    print_float(1, file_time, 5);
    printf("\n");

    printf("Overhead de gerenciamento de memoria: ");
    print_float(1, mem_time, 5);
    printf("\n");
    printf("\n");

    printf("Desempenho geral do sistema: ");

    int tot  = trgput_num * (C / 4) / (trgput_den);
        tot += fairness_num * (C / 4) / fairness_den;
        tot += (C / 4) / file_time;
        tot += (C / 4) / mem_time;

    print_float(tot, C, 5);
    printf("\n");
    printf("\n");

    free(proc_times);
  }
  return 0;
}
