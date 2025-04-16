#ifndef PIDMAP_H
#define PIDMAP_H

#include "task.h"

struct pmkv_t {
  int pid;
  struct task_t *task;
};

struct pidmap_t {
  int count;
  int capacity;
  struct pmkv_t *pairs;
};

struct pidmap_t *pidmap_create();
void pidmap_destroy(struct pidmap_t *map);

struct task_t* pidmap_get(struct pidmap_t *map, int pid);
void pidmap_add(struct pidmap_t *map, struct task_t *task);
void pidmap_remove(struct pidmap_t *map, int pid);
void pidmap_update(struct pidmap_t *map, int pid, struct task_t *newValue);

#endif
