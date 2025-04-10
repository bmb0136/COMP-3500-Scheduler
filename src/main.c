#include <stdio.h>

int main(int argc, char** argv) {
  if (argc < 2 || argc > 3) {
    puts("Usage: ./scheduler <task list> <FCFS | SRTF | RR> [time quantum*]");
    puts("*Only used for RR mode");
    return 1;
  }
}
