#include "policy.h"

static void init();
static void destroy();
static struct task_t get();
static void finish(struct task_t task);
static void add(struct task_t task);
static void dec(struct task_t task);
static size_t count();

struct policy_t policy_rr_create() {
  return (struct policy_t){.init = &init,
                           .destroy = &destroy,
                           .getCurrentTask = &get,
                           .onTaskFinished = &finish,
                           .addToReadyQueue = &add,
                           .getReadyQueueCount = &count,
                           .decerementBurstTime = &dec};
}

static void init() {
}

static void destroy() {
}

struct task_t get() {
}

void add(struct task_t task) {
}

void finish(struct task_t task) {
}

void dec(struct task_t task) {
}

size_t count() {
}
