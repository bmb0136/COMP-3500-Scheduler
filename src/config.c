#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>

static struct task_t *readTaskList(FILE *file);

struct config_t *config_parse(int argc, char** argv) {
  if (argc < 3 || argc > 4) {
    puts("Usage: ./scheduler <task list> <FCFS | SRTF | RR> [time quantum*]");
    puts("*Only used for RR mode");
    return NULL;
  }

  struct config_t *config = malloc(sizeof(struct config_t));
  assert(config != NULL);

  FILE *taskListFile = fopen(argv[1], "r");
  if (!taskListFile) {
    printf("Error: failed to open task list %s", argv[1]);
    return NULL;
  }
  config->tasks = readTaskList(taskListFile);

  enum PolicyType policy = -1;
  if (strcasecmp(argv[2], "fcfs")) {
    policy = POL_FCFS;
  } else if (strcasecmp(argv[2], "srtf")) {
    policy = POL_SRTF;
  } else if (strcasecmp(argv[2], "rr")) {
    policy = POL_RR;
  } else {
    puts("Error: Invalid policy, must be one of: FCFS, SRTF, RR");
    return NULL;
  }
  config->policy = policy;

  int timeQuantum = 0;
  if (argc == 4) {
    timeQuantum = atoi(argv[3]);
    if (timeQuantum < 1) {
      puts("Error: Time quantum must be > 0");
      exit(1);
    }
  } else if (policy == POL_RR) {
    puts("Error: Time quantim must be specified for Round Robin policy");
    return NULL;
  }
  config->timeQuantum = timeQuantum;

  return config;
}

static struct task_t *readTaskList(FILE *file) {
  // TODO
  exit(1);
}
