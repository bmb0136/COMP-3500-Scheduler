#ifndef CONFIG_H
#define CONFIG_H

#include "task.h"

enum PolicyType {
  POL_FCFS, POL_SRTF, POL_RR
};

struct config_t {
  enum PolicyType policy;
  int timeQuantum;
  struct task_t *tasks;
  int numTasks;
};

struct config_t *config_parse(int argc, char** argv);

void config_destroy(struct config_t *config);

#endif
