#include "config.h"
#include "taskqueue.h"
#include "simulation.h"
#include "policy_fcfs.h"
#include "policy_srtf.h"
#include "policy_rr.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
  struct config_t *config = config_parse(argc, argv);
  if (!config) {
    return 1;
  }

  struct taskqueue_t* queue = taskqueue_create();
  for (int i = 0; i < config->numTasks; i++) {
    taskqueue_push(queue, config->tasks[i]);
  }

  policy_fp policy = 0;
  switch (config->policy) {
    case POL_FCFS:
      policy = &policy_fcfs;
      policy_fcfs_init();
      break;
    case POL_SRTF:
      policy = &policy_srtf;
      policy_srtf_init();
      break;
    case POL_RR:
      policy = &policy_rr;
      policy_rr_init();
      break;
    default:
      puts("Error: invalid policy");
      exit(1);
      break;
  }

  simulation_run(config->timeQuantum, queue, policy);

  config_destroy(config);
  taskqueue_destroy(queue);
  return 0;
}
