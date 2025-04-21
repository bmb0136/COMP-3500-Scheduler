#include "policy.h"
#include "taskheap.h"

static void init();
static void run(struct pinput_t input, pemit_fp emit);
static void destroy();

struct policy_t policy_srtf_create() {
  return (struct policy_t){.init = &init, .run = &run, .destroy = &destroy};
}

static struct taskheap_t *heap, *wait;

static int cmpByBurst(struct task_t *a, struct task_t *b) {
  int x = a->burstTime - b->burstTime;
  if (x == 0) {
    x = a->startTime - b->startTime;
  }
  return x;
}

static int cmpByStart(struct task_t *a, struct task_t *b) {
  int x = a->startTime - b->startTime;
  if (x == 0) {
    x = a->pid - b->pid;
  }
  return x;
}

static void init() {
  heap = taskheap_create(&cmpByBurst);
  wait = taskheap_create(&cmpByStart);
}

static void destroy() {
  taskheap_destroy(heap);
  taskheap_destroy(wait);
}

static void run(struct pinput_t input, pemit_fp emit) {
  unsigned int time = 0;

  for (size_t i = 0; i < input.numTasks; i++) {
    taskheap_push(wait, input.futureTasks[i]);
  }

  while (1) {
    // Add tasks to ready queue
    while (wait->count > 0 && time >= wait->items[0].startTime) {
      taskheap_push(heap, wait->items[0]);
      taskheap_pop(wait, NULL);
    }

    if (heap->count == 0) {
      if (wait->count == 0) {
        emit((struct schedevent_t){.time = time, .type = SE_DONE});
        return;
      }
      emit((struct schedevent_t){.time = time, .type = SE_WAIT});
      time++;
      continue;
    }

    struct task_t current = heap->items[0];

    if (current.burstTime == 0) {
      emit((struct schedevent_t){
          .time = time, .type = SE_FINISH, .data.task = current.pid});
      taskheap_pop(heap, NULL);
      current = heap->items[0];
      if (heap->count == 0) {
        if (wait->count == 0) {
          emit((struct schedevent_t){.time = time, .type = SE_DONE});
          return;
        }
        emit((struct schedevent_t){.time = time, .type = SE_WAIT});
        time++;
        continue;
      }
    }

    emit((struct schedevent_t){
        .time = time, .type = SE_RUN, .data.task = current.pid});
    taskheap_updatekey(heap, current.pid, --current.burstTime);
    time++;
  }
}
