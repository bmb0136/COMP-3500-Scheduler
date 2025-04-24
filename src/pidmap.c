/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 2.0 4/24/25
 *
 * Implementation of pidmap.h
 *
 * Uses closed addressing + multiplication method
 */

#include "pidmap.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

static void grow(struct pidmap_t *map, int newSize);

static int hash(struct pidmap_t *map, int pid) {
  const float A = 0.61803398874f;
  return (int)floorf(map->capacity * fmod(pid * A, 1.0f));
}

struct pidmap_t *pidmap_create() {
  struct pidmap_t *map =
      (struct pidmap_t *)malloc(sizeof(struct pidmap_t));
  map->count = 0;
  map->capacity = 4;
  map->pairs = (struct pmkv_t**)calloc(map->capacity, sizeof(struct pmkv_t*));
  return map;
}

void pidmap_destroy(struct pidmap_t *map) {
  for (size_t i = 0; i < map->capacity; i++) {
    struct pmkv_t *n = map->pairs[i];
    while (n) {
      struct pmkv_t *temp = n;
      n = n->next;
      free(temp);
    }
  }
  free(map->pairs);
  free(map);
}

struct task_t *pidmap_get(struct pidmap_t *map, int pid) {
  struct pmkv_t *n = map->pairs[hash(map, pid)];
  while (n) {
    if (n->pid == pid) {
      return n->task;
    }
    n = n->next;
  }

  return NULL;
}

void pidmap_add(struct pidmap_t *map, struct task_t *task) {
  if (map->count + 1 >= map->capacity) {
    grow(map, map->capacity * 2);
  }

  int i = hash(map, task->pid);

  struct pmkv_t * kv = (struct pmkv_t *)malloc(sizeof(struct pmkv_t));
  kv->pid = task->pid;
  kv->task = task;
  kv->next = map->pairs[i];
  map->pairs[i] = kv;

  map->count++;
}

void pidmap_remove(struct pidmap_t *map, int pid) {
  map->count--;

  int i = hash(map, pid);

  struct pmkv_t *n = map->pairs[i], *prev = NULL;

  if (n->pid == pid) {
    map->pairs[i] = n->next;
    free(n);
    return;
  }

  while (n) {
    if (n->pid == pid) {
      prev->next = n->next;
      free(n);
      return;
    }
    prev = n;
    n = n->next;
  }
}

void pidmap_update(struct pidmap_t *map, int pid, struct task_t *newValue) {
  struct pmkv_t *n = map->pairs[hash(map, pid)];

  while (n) {
    if (n->pid == pid) {
      n->task = newValue;
      return;
    }
    n = n->next;
  }
}

void pidmap_setCapacity(struct pidmap_t *map, size_t newSize) {
  if (newSize > map->capacity) {
    grow(map, newSize);
  }
}

static void grow(struct pidmap_t *map, int newSize) {
  assert(newSize > map->capacity && newSize > map->count);

  struct pmkv_t **new = (struct pmkv_t **)calloc(newSize, sizeof(struct pmkv_t*));

  struct pidmap_t temp = (struct pidmap_t){
    .count = map->count,
    .capacity = newSize,
    .pairs = new
  };

  struct pmkv_t *n;

  for (size_t i = 0; i < map->capacity; i++) {
    n = map->pairs[i];
    while (n) {
      pidmap_add(&temp, n->task);
      n = n->next;
    }
  }


  while (n) {
    struct pmkv_t *temp = n;
    n = n->next;
    free(temp);
  }
  free(map->pairs);
  map->pairs = new;
  map->capacity = newSize;
}
