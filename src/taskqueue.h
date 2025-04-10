#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include "task.h"

struct tqnode_t {
  struct task_t task;
  struct tqnode_t *next;
};

struct taskqueue_t {
  struct tqnode_t *start, *end;
  int count;
};

int taskqueue_pop(struct taskqueue_t *queue, struct task_t *output);
void taskqueue_push(struct taskqueue_t *queue, struct task_t task);
void taskqueue_destroy(struct taskqueue_t *queue);

#endif
