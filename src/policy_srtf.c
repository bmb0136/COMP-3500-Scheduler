#include "policy.h"
#include "taskheap.h"

static void init();
static void run(struct pinput_t input, pemit_fp emit);
static void destroy();

struct policy_t policy_srtf_create() {
  return (struct policy_t){.init = &init, .run = &run, .destroy = &destroy};
}

static struct taskheap_t *heap;

static void init() { heap = taskheap_create(); }

static void destroy() { taskheap_destroy(heap); }

static void run(struct pinput_t input, pemit_fp emit) {
  unsigned int time = 0;

  while (1) {
    // Add tasks to ready "queue"
    for (size_t i = 0; i < input.numTasks; i++) {
      struct task_t t = input.futureTasks[i];
      if (time >= t.startTime) {
        taskheap_push(heap, t);
        for (size_t j = i + 1; j < input.numTasks; j++) {
          input.futureTasks[j - 1] = input.futureTasks[j];
        }
        input.numTasks--;
        i--;
      }
    }

    if (heap->count == 0) {
      if (input.numTasks == 0) {
        emit((struct schedevent_t){.time = time, .type = SE_DONE});
        return;
      }
      emit((struct schedevent_t){.time = time, .type = SE_WAIT});
      time++;
      continue;
    }

    struct task_t *current = &heap->items[0];

    if (current->burstTime == 0) {
      taskheap_pop(heap, NULL);
      emit((struct schedevent_t){
          .time = time, .type = SE_FINISH, .data.task = current->pid});
      if (heap->count == 0) {
        if (input.numTasks == 0) {
          emit((struct schedevent_t){.time = time, .type = SE_DONE});
          return;
        }
        emit((struct schedevent_t){.time = time, .type = SE_WAIT});
        time++;
        continue;
      }
    }

    taskheap_updatekey(heap, current->pid, --current->burstTime);

    emit((struct schedevent_t){
        .time = time, .type = SE_RUN, .data.task = current->pid});
  }
}
