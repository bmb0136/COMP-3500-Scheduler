#include "policy.h"
#include "taskheap.h"

static struct taskheap_t *heap;

static int cmpByBurst(struct task_t *a, struct task_t *b) {
  int x = a->burstTime - b->burstTime;
  if (x == 0) {
    x = a->startTime - b->startTime;
  }
  return x;
}

static void init() {
  heap = taskheap_create(&cmpByBurst);
}

static void destroy() {
  taskheap_destroy(heap);
}

struct task_t get(struct pinput_t input) {
  return heap->items[0];
}

void add(struct pinput_t input, struct task_t task) {
  taskheap_push(heap, task);
}

void finish(struct pinput_t input, struct task_t task) {
  taskheap_pop(heap, NULL);
}

void dec(struct pinput_t input, struct task_t task) {
  taskheap_updatekey(heap, task.pid, task.burstTime - 1);
}

size_t count(struct pinput_t input) {
  return heap->count;
}

struct policy_t policy_srtf_create() {
  return (struct policy_t){.init = &init,
                           .destroy = &destroy,
                           .getCurrentTask = &get,
                           .onTaskFinished = &finish,
                           .addToReadyQueue = &add,
                           .getReadyQueueCount = &count,
                           .decerementBurstTime = &dec};
}
