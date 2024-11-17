#include "kernel/types.h"
#include "user/user.h"
#include "user/random.h"
#include "user/print_float.h"
#include "user/exp4-cpu-bound.h"
#include "user/exp4-io-bound.h"

const int ROUNDS = 30;

int main() {
  for (int round = 1; round <= ROUNDS; ++round) {
    int tot_time = 0, mem_time = 0, file_time = 0, time;
    
    printf("Rodada %d\n", round);

    int x = 1, y = 0;
    // int x = random(6, 14), y = 20 - x;
    int proc_count = x + y;

    time = uptime();
    int *proc_times = malloc(proc_count * sizeof (int));
    mem_time += uptime() - time;

    for (int i = 0; i < proc_count; ++i) {
      int fd_tot[2], fd_mem[2], fd_file[2];
      pipe(fd_tot);
      pipe(fd_mem);
      pipe(fd_file);
      int pid = fork();
      if (pid == -1) {
        fprintf(2, "error forking process %d\n", i);
        exit(1);
      } else if (pid == 0) {
        close(fd_tot[0]);
        close(fd_mem[0]);
        close(fd_file[0]);
        int proc_mem_time = 0, proc_file_time = 0;
        time = uptime();
        if (i < x) {
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
      } else {
        wait(0);
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
      }
    }
  }

  return 0;
}
