#include "policy.h"
#include "taskheap.h"

static void init();
static void destroy();
static struct task_t get();
static void finish(struct task_t task);
static void add(struct task_t task);
static void dec(struct task_t task);
static size_t count();

struct policy_t policy_srtf_create() {
  return (struct policy_t){.init = &init,
                           .destroy = &destroy,
                           .getCurrentTask = &get,
                           .onTaskFinished = &finish,
                           .addToReadyQueue = &add,
                           .getReadyQueueCount = &count,
                           .decerementBurstTime = &dec};
}

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

struct task_t get() {
  return heap->items[0];
}

void add(struct task_t task) {
  taskheap_push(heap, task);
}

void finish(struct task_t task) {
  taskheap_pop(heap, NULL);
}

void dec(struct task_t task) {
  taskheap_updatekey(heap, task.pid, task.burstTime - 1);
}

size_t count() {
  return heap->count;
}
