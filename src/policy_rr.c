/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 3.3 4/22/25
 *
 * Implementation of the Round Robin policy
 * Uses taskring_t as a ready "queue"
 */

#include "policy.h"
#include "taskring.h"

static struct taskring_t *ring;

static void init() {
  ring = taskring_create();
}

static void destroy() {
  taskring_destroy(ring);
}

static struct task_t get(struct pinput_t input) {
  return ring->current->task;
}

static void add(struct pinput_t input, struct task_t task) {
  task.startTime = input.timeQuantum;
  taskring_push(ring, task);
}

static void finish(struct pinput_t input, struct task_t task) {
  taskring_pop(ring, NULL);
}

static void dec(struct pinput_t input, struct task_t task) {
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

static size_t count(struct pinput_t input) {
  return ring->count;
}

struct policy_t policy_rr_create() {
  return (struct policy_t){.init = &init,
                           .destroy = &destroy,
                           .getCurrentTask = &get,
                           .onTaskFinished = &finish,
                           .addToReadyQueue = &add,
                           .getReadyQueueCount = &count,
                           .decerementBurstTime = &dec};
}
