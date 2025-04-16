#ifndef TASKHEAP_H
#define TASKHEAP_H

#include "task.h"
#include "pidmap.h"

struct taskheap_t {
  struct task_t *items;
  int capacity;
  int count;
  struct pidmap_t *pidmap;
};

struct taskheap_t *taskheap_create();
void taskheap_destroy(struct taskheap_t *heap);

void taskheap_push(struct taskheap_t *heap, struct task_t task);
char taskheap_pop(struct taskheap_t *heap, struct task_t *output);
void taskheap_updatekey(struct taskheap_t *heap, int pid, int key);

#endif
