/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 1.2 4/23/25
 *
 * Representation of tasks to run
 */

#ifndef TASK_H
#define TASK_H

/*
 * struct task_t: A task (process) to run
 *
 * Fields:
 * - pid: The Process ID
 * - startTime: The (ideal) time that this process should start executing
 * - burstTime: The number of timesteps that this process should execute for
 */
struct task_t {
  int pid, startTime, burstTime;
};

#endif
