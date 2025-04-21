#include "policy.h"
#include "taskheap.h"
#include "taskqueue.h"

static void init();
static void run(struct pinput_t input, pemit_fp emit);
static void destroy();

struct policy_t policy_fcfs_create() {
  return (struct policy_t){.init = &init, .run = &run, .destroy = &destroy};
}

static struct taskqueue_t *queue;
static struct taskheap_t *heap;

static int compare(struct task_t *a, struct task_t *b) {
  int x = a->startTime - b->startTime;
  if (x == 0) {
    x = a->pid - b->pid;
  }
  return x;
}

static void init() {
  queue = taskqueue_create();
  heap = taskheap_create(&compare);
}

static void destroy() {
  taskqueue_destroy(queue);
  taskheap_destroy(heap);
}

static void run(struct pinput_t input, pemit_fp emit) {
  unsigned int time = 0;

  for (size_t i = 0; i < input.numTasks; i++) {
    taskheap_push(heap, input.futureTasks[i]);
  }

  while (1) {
    // Add tasks to ready queue
    while (heap->count > 0 && time >= heap->items[0].startTime) {
      taskqueue_push(queue, heap->items[0]);
      taskheap_pop(heap, NULL);
    }

    // If no tasks to run, wait
    if (queue->count == 0) {

      // If no more tasks, exit
      if (heap->count == 0) {
        emit((struct schedevent_t){.time = time, .type = SE_DONE});
        return;
      }

      emit((struct schedevent_t){.time = time, .type = SE_WAIT});

      time++;
      continue;
    }

    struct task_t current = queue->start->task;

    // If the task is done, go to the next task
    if (current.burstTime == 0) {
      emit((struct schedevent_t){
          .time = time, .type = SE_FINISH, .data.task = current.pid});

      taskqueue_pop(queue, NULL);

      if (queue->count == 0) {

        // If no more tasks, exit
        if (heap->count == 0) {
          emit((struct schedevent_t){.time = time, .type = SE_DONE});
          return;
        }

        time++;
        continue;
      }
    }

    // Run current task
    emit((struct schedevent_t){
        .time = time, .type = SE_RUN, .data.task = current.pid});
    queue->start->task.burstTime--;
    time++;
  }
}
