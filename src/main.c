#include <stdio.h>

int main(int argc, char** argv) {
  if (argc < 3 || argc > 4) {
    puts("Usage: ./scheduler <task list> <FCFS | SRTF | RR> [time quantum*]");
    puts("*Only used for RR mode");
    return 1;
  }
}
