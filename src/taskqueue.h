#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include "task.h"

struct tqnode_t {
  struct task_t task;
  struct tqnode_t *next, *prev;
};

struct taskqueue_t {
  struct tqnode_t *start, *end;
  int count;
};

struct taskqueue_t *taskqueue_create();
int taskqueue_pop(struct taskqueue_t *queue, struct task_t *output);
void taskqueue_push(struct taskqueue_t *queue, struct task_t task);
void taskqueue_destroy(struct taskqueue_t *queue);

#endif
