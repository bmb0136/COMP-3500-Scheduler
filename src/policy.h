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
};

typedef void(pemit_fp)(struct schedevent_t);

struct policy_t {
  void (*init)();
  void (*run)(struct pinput_t input, pemit_fp emit);
  void (*destroy)();
};

struct policy_t policy_fcfs_create();
struct policy_t policy_srtf_create();
struct policy_t policy_rr_create();

#endif
