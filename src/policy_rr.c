#include "policy.h"
#include "taskring.h"

static void init();
static void destroy();
static struct task_t get(struct pinput_t input);
static void finish(struct pinput_t input, struct task_t task);
static void add(struct pinput_t input, struct task_t task);
static void dec(struct pinput_t input, struct task_t task);
static size_t count(struct pinput_t input);

struct policy_t policy_rr_create() {
  return (struct policy_t){.init = &init,
                           .destroy = &destroy,
                           .getCurrentTask = &get,
                           .onTaskFinished = &finish,
                           .addToReadyQueue = &add,
                           .getReadyQueueCount = &count,
                           .decerementBurstTime = &dec};
}

static struct taskring_t *ring;

static void init() {
  ring = taskring_create();
}

static void destroy() {
  taskring_destroy(ring);
}

struct task_t get(struct pinput_t input) {
  return ring->current->task;
}

void add(struct pinput_t input, struct task_t task) {
  task.startTime = input.timeQuantum;
  taskring_push(ring, task);
}

void finish(struct pinput_t input, struct task_t task) {
  taskring_pop(ring, NULL);
}

void dec(struct pinput_t input, struct task_t task) {
  struct task_t *current = &ring->current->task;
  current->burstTime--;
  current->startTime--;
  if (current->startTime == 0) {
    current->startTime = input.timeQuantum;
    if (current->burstTime > 0) {
      taskring_next(ring);
    }
  }
}

size_t count(struct pinput_t input) {
  return ring->count;
}
