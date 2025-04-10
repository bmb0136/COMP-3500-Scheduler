#include "config.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

static int readTaskList(FILE *file, struct task_t **list);

struct config_t *config_parse(int argc, char **argv) {
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
  config->numTasks = readTaskList(taskListFile, &config->tasks);
  printf("Read %d tasks from %s\n", config->numTasks, argv[1]);

  enum PolicyType policy = -1;
  if (strcasecmp(argv[2], "fcfs") == 0) {
    policy = POL_FCFS;
  } else if (strcasecmp(argv[2], "srtf") == 0) {
    policy = POL_SRTF;
  } else if (strcasecmp(argv[2], "rr") == 0) {
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

void config_destroy(struct config_t *config) {
  free(config->tasks);
  free(config);
}

static int readTaskList(FILE *file, struct task_t **list) {
  int capacity = 8;
  *list = (struct task_t*)malloc(capacity * sizeof(struct task_t));

  int count = 0;
  while (!feof(file)) {
    int pid, start, burst;

    int read = fscanf(file, "%d %d %d\n", &pid, &start, &burst);
    if (read != 3) {
      break;
    }
    assert(ferror(file) == 0);

    (*list)[count] = (struct task_t){
      .pid = pid,
      .startTime = start,
      .burstTime = burst 
    };

    count++;
    if (count > capacity) {
      capacity *= 2;
      struct task_t* new = (struct task_t*)malloc(capacity * sizeof(struct task_t));
      memcpy(new, *list, count * sizeof(struct task_t));
      free(*list);
      *list = new;
    }
  }

  return count;
}
