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
