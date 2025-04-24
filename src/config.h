/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 1.8 4/24/25
 *
 * Command line argument parser
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "task.h"

/*
 * enum PolicyType: A possible types of policies that are currently implemented
 */
enum policytype_e { POL_FCFS, POL_SRTF, POL_RR };

/*
 * struct config_t: Simulation configuration
 *
 * Fields:
 * - policy: The policy to run
 * - timeQuantum: The number of timesteps to run each task for before cycling to the next when using the POL_RR policy
 * - tasks: The list of tasks to run
 * - numTasks: The length of tasks
 */
struct config_t {
  enum policytype_e policy;
  int timeQuantum;
  struct task_t *tasks;
  int numTasks;
};

/*
 * config_parse(): Contruct a config_t from command line arguments. Returns NULL on failure
 *
 * Arguments: Same as main()
 */
struct config_t *config_parse(int argc, char **argv);

/*
 * Destructor
 */
void config_destroy(struct config_t *config);

#endif
