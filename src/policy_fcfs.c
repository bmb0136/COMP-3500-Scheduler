#include "policy.h"
#include "taskqueue.h"
#include <stdio.h>

static void init();
static void run(struct pinput_t input, pemit_fp emit);
static void destroy();

struct policy_t policy_fcfs_create() {
  return (struct policy_t) {
    .init = &init,
    .run = &run,
    .destroy = &destroy
  };
}

static struct taskqueue_t *queue;

static void init() {
  queue = taskqueue_create();
}

static void destroy() {
  taskqueue_destroy(queue);
}

static void run(struct pinput_t input, pemit_fp emit) {
  unsigned int time = 0;

  while (1) {
    // Add tasks to ready queue
    for (size_t i = 0; i < input.numTasks; i++) {
      struct task_t t = input.futureTasks[i];
      if (time >= t.startTime) {
        taskqueue_push(queue, t);
        for (size_t j = i + 1; j < input.numTasks; j++) {
          input.futureTasks[j - 1] = input.futureTasks[j];
        }
        input.numTasks--;
        i--;
      }
    }
    // If no tasks to run, wait
    if (queue->count == 0) {

      if (input.numTasks == 0) {
        emit((struct schedevent_t){
          .time = time,
          .type = SE_DONE
        });
        return;
      }

      emit((struct schedevent_t){
        .time = time,
        .type = SE_WAIT
      });
      time++;
      continue;
    }

    struct task_t current = queue->start->task;

    // If the task is done, go to the next task
    if (current.burstTime == 0) {
      emit((struct schedevent_t){
        .time = time,
        .type = SE_FINISH,
        .data.task = current.pid
      });
      taskqueue_pop(queue, NULL);
      if (queue->count == 0) {
        if (input.numTasks == 0) {
          emit((struct schedevent_t){
            .time = time,
            .type = SE_DONE
          });
          return;
        }
        time++;
        continue;
      }
    }

    emit((struct schedevent_t){
      .time = time,
      .type = SE_RUN,
      .data.task = current.pid
   });
    queue->start->task.burstTime--;

    time++;
  }
}
