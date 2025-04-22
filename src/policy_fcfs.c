#include "policy.h"
#include "taskqueue.h"

static struct taskqueue_t *queue;

static int compare(struct task_t *a, struct task_t *b) {
  int x = a->startTime - b->startTime;
  if (x == 0) {
    x = a->pid - b->pid;
  }
  return x;
}

static void init() {
  queue = taskqueue_create();
}

static void destroy() {
  taskqueue_destroy(queue);
}

struct task_t get(struct pinput_t input) {
  return queue->start->task;
}

void add(struct pinput_t input, struct task_t task) {
  taskqueue_push(queue, task);
}

void finish(struct pinput_t input, struct task_t task) {
  taskqueue_pop(queue, NULL);
}

void dec(struct pinput_t input, struct task_t task) {
  queue->start->task.burstTime--;
}

size_t count(struct pinput_t input) {
  return queue->count;
}

struct policy_t policy_fcfs_create() {
  return (struct policy_t){.init = &init,
                           .destroy = &destroy,
                           .getCurrentTask = &get,
                           .onTaskFinished = &finish,
                           .addToReadyQueue = &add,
                           .getReadyQueueCount = &count,
                           .decerementBurstTime = &dec};
}
