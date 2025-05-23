/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 1.3 4/23/25
 *
 * Implementation of policy.h
 */

#include "policy.h"
#include "taskheap.h"

static struct taskheap_t *wait;

static int cmpByStart(struct task_t *a, struct task_t *b) {
  int x = a->startTime - b->startTime;
  if (x == 0) {
    x = a->pid - b->pid;
  }
  return x;
}

static void updateKey(struct task_t *task, int key) {
  task->startTime = key;
}

void policy_init(struct policy_t policy) {
  wait = taskheap_create(&cmpByStart, &updateKey);
  policy.init();
}

void policy_destroy(struct policy_t policy) {
  taskheap_destroy(wait);
  policy.destroy();
}

void policy_run(struct policy_t policy, struct pinput_t input, pemit_fp emit) {
  unsigned int time = 0;

  for (size_t i = 0; i < input.numTasks; i++) {
    taskheap_push(wait, input.futureTasks[i]);
  }


  while (1) {
    while (wait->count > 0 && time >= wait->items[0].startTime) {
      policy.addToReadyQueue(input, wait->items[0]);
      taskheap_pop(wait, NULL);
    }

    if (policy.getReadyQueueCount(input) == 0) {
      if (wait->count == 0) {
        emit((struct schedevent_t){.time = time, .type = SE_DONE});
        return;
      }
      emit((struct schedevent_t){.time = time, .type = SE_WAIT});
      time++;
      continue;
    }

    struct task_t current = policy.getCurrentTask(input);

    if (current.burstTime == 0) {
      emit((struct schedevent_t){
          .time = time, .type = SE_FINISH, .data.task = current.pid});
      policy.onTaskFinished(input, current);
      if (policy.getReadyQueueCount(input) == 0) {
        if (wait->count == 0) {
          emit((struct schedevent_t){.time = time, .type = SE_DONE});
          return;
        }
        emit((struct schedevent_t){.time = time, .type = SE_WAIT});
        time++;
        continue;
      }
      current = policy.getCurrentTask(input);
    }

    emit((struct schedevent_t){
        .time = time, .type = SE_RUN, .data.task = current.pid});
    policy.decerementBurstTime(input, current);
    time++;
  }
}
