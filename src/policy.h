/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 3.2 4/23/25
 *
 * Common functionality for all 3 policies
 */

#ifndef POLICY_H
#define POLICY_H

#include "task.h"
#include <stddef.h>

/*
 * struct schedevent_t: Represents an action that a policy can take
 *
 * Fields:
 * - time: The timestep the event occurred on
 * - type: The type of event
 * - data: Data for each event type.
 *   - May possibly contain: a PID
 */
struct schedevent_t {
  int time;
  enum schedevent_type_t { SE_WAIT, SE_RUN, SE_FINISH, SE_DONE } type;
  union {
    int task;
  } data;
};

/*
 * struct pinput_t: Input passed to a policy
 *
 * Fields:
 * - futureTasks: A list of all tasks that need to be run
 * - numTasks: The length of futureTasks
 * - timeQuantum: The number of timesteps to run each process for (used for POL_RR)
 */
struct pinput_t {
  struct task_t *futureTasks;
  size_t numTasks;
  int timeQuantum;
};

/*
 * Delegate void pemit_fp(): Emit a schedevent_t. This function is passed to policies
 */
typedef void(pemit_fp)(struct schedevent_t);

/*
 * struct policy_t: A scheduler policy
 *
 * Fields:
 * - init: Initialization function
 * - destroy: Deinitialization function
 * - addToReadyQueue: Add a task to the ready queue
 * - getReadyQueueCount: Get the size of the ready queue
 * - getCurrentTask: Get the current task to run
 * - onTaskFinished: Callback for when a task finishes
 * - decerementBurstTime: Decrease the burst time of a task
 */
struct policy_t {
  void (*init)();
  void (*destroy)();
  void (*addToReadyQueue)(struct pinput_t input, struct task_t task);
  size_t (*getReadyQueueCount)(struct pinput_t input);
  struct task_t (*getCurrentTask)(struct pinput_t input);
  void (*onTaskFinished)(struct pinput_t input, struct task_t task);
  void (*decerementBurstTime)(struct pinput_t input, struct task_t task);
};

/*
 * Constructor methods
 */
struct policy_t policy_fcfs_create();
struct policy_t policy_srtf_create();
struct policy_t policy_rr_create();

/*
 * policy_init(): Initialize the policy execution
 *
 * Arguments:
 * - policy: The policy to initialize
 */
void policy_init(struct policy_t policy);

/*
 * policy_destroy(): Deinitialize a policy
 *
 * Arguments:
 * - policy: The policy to cleanup
 */
void policy_destroy(struct policy_t policy);

/*
 * policy_run(): Run a policy
 *
 * Arguments:
 * - policy: The policy to apply
 * - input: The input to the policy
 * - emit: The callback for processing events
 */
void policy_run(struct policy_t policy, struct pinput_t input, pemit_fp emit);

#endif
