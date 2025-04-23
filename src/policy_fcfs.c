/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 3.3 4/22/25
 *
 * Implementation of the First Come First Serve policy
 * Uses taskqueue_t as a ready queue
 */

#include "policy.h"
#include "taskqueue.h"

static struct taskqueue_t *queue;

static void init() {
  queue = taskqueue_create();
}

static void destroy() {
  taskqueue_destroy(queue);
}

static struct task_t get(struct pinput_t input) {
  return queue->start->task;
}

static void add(struct pinput_t input, struct task_t task) {
  taskqueue_push(queue, task);
}

static void finish(struct pinput_t input, struct task_t task) {
  taskqueue_pop(queue, NULL);
}

static void dec(struct pinput_t input, struct task_t task) {
  queue->start->task.burstTime--;
}

static size_t count(struct pinput_t input) {
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
