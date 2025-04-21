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

void policy_init(struct policy_t policy) {
  wait = taskheap_create(&cmpByStart);
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
      policy.addToReadyQueue(wait->items[0]);
      taskheap_pop(wait, NULL);
    }

    if (policy.getReadyQueueCount() == 0) {
      if (wait->count == 0) {
        emit((struct schedevent_t){.time = time, .type = SE_DONE});
        return;
      }
      emit((struct schedevent_t){.time = time, .type = SE_WAIT});
      time++;
      continue;
    }

    struct task_t current = policy.getCurrentTask();

    if (current.burstTime == 0) {
      emit((struct schedevent_t){
          .time = time, .type = SE_FINISH, .data.task = current.pid});
      policy.onTaskFinished(current);
      if (policy.getReadyQueueCount() == 0) {
        if (wait->count == 0) {
          emit((struct schedevent_t){.time = time, .type = SE_DONE});
          return;
        }
        emit((struct schedevent_t){.time = time, .type = SE_WAIT});
        time++;
        continue;
      }
      current = policy.getCurrentTask();
    }

    emit((struct schedevent_t){
        .time = time, .type = SE_RUN, .data.task = current.pid});
    policy.decerementBurstTime(current);
    time++;
  }
}
