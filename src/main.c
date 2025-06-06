/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 1.13 4/24/25
 *
 * Program entrypoint
 */

#include "config.h"
#include "simulation.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  struct config_t *config = config_parse(argc, argv);
  if (!config) {
    return 1;
  }

  if (config->numTasks == 0) {
    puts("No tasks specified");
    return 1;
  }

  struct policy_t policy;
  switch (config->policy) {
  case POL_FCFS:
    policy = policy_fcfs_create();
    break;
  case POL_SRTF:
    policy = policy_srtf_create();
    break;
  case POL_RR:
    policy = policy_rr_create();
    break;
  default:
    puts("Bug: invalid policy");
    exit(1);
    break;
  }

  simulation_run(config, policy);

  config_destroy(config);
  return 0;
}
