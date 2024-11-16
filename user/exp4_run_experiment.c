#include "kernel/types.h"
#include "user/user.h"
#include "user/random.h"

const int ROUNDS = 30;

int main() {
  for (int round = 1; round <= ROUNDS; ++round) {
    int x = random(6, 14), y = 20 - x;
    for (int i = 0; i < x; ++i) {
      int pid = fork();
      if (pid == -1) {
        fprintf(2, "fork\n");
        exit(1);
      } else if (pid == 0) {
        char *args[2] = {"exp4-cpu-bound.c", NULL};
        exec("exp4-cpu-bound.c", args);
      } else {
        wait(0);
      }
    }
    for (int i = 0; i < y; ++i) {
      int pid = fork();
      if (pid == -1) {
        fprintf(2, "fork\n");
        exit(1);
      } else if (pid == 0) {
        char *args[2] = {"exp4-io-bound.c", NULL};
        exec("exp4-io-bound.c", args);
      } else {
        wait(0);
      }
    }
  }

  return 0;
}
