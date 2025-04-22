#ifndef TASKRING_H
#define TASKRING_H

#include "task.h"
#include <stdlib.h>

struct trnode_t {
  struct task_t task;
  struct trnode_t *next, *prev;
};

struct taskring_t {
  size_t count;
  struct trnode_t *current;
  struct task_t *items;
};

struct taskring_t *taskring_create();
void taskring_destroy(struct taskring_t *ring);

void taskring_push(struct taskring_t *ring, struct task_t task);
char taskring_pop(struct taskring_t *ring, struct task_t *output);
void taskring_next(struct taskring_t *ring);
void taskring_prev(struct taskring_t *ring);

#endif
