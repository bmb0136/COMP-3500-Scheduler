#ifndef POLICY_H
#define POLICY_H

#include "task.h"
#include <stddef.h>

struct schedevent_t {
  int time;
  enum schedevent_type_t { SE_WAIT, SE_RUN, SE_FINISH, SE_DONE } type;
  union {
    int task;
  } data;
};

struct pinput_t {
  struct task_t *futureTasks;
  size_t numTasks;
  int timeQuantum;
};

typedef void(pemit_fp)(struct schedevent_t);

struct policy_t {
  void (*init)();
  void (*destroy)();
  void (*addToReadyQueue)(struct pinput_t input, struct task_t task);
  size_t (*getReadyQueueCount)(struct pinput_t input);
  struct task_t (*getCurrentTask)(struct pinput_t input);
  void (*onTaskFinished)(struct pinput_t input, struct task_t task);
  void (*decerementBurstTime)(struct pinput_t input, struct task_t task);
};

struct policy_t policy_fcfs_create();
struct policy_t policy_srtf_create();
struct policy_t policy_rr_create();

void policy_init(struct policy_t policy);
void policy_destroy(struct policy_t policy);
void policy_run(struct policy_t policy, struct pinput_t input, pemit_fp emit);

#endif
