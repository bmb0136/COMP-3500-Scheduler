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

static char tryProbe(struct pidmap_t *map, int key, int *target, int *output) {
  int k = hash(map, key);
  int k2 = 1 + (k % (map->capacity - 1));

  int i = k;
  struct pmkv_t current;
  for (int n = 1; (current = map->pairs[i]).task; n++) {
    if (target && current.pid == *target) {
      *output = i;
      return 1;
    }

    i = (k + (k2 * n)) % map->capacity;
  }
  *output = i;

  return 0;
}

struct pidmap_t *pidmap_create() {
  struct pidmap_t *map =
      (struct pidmap_t *)malloc(sizeof(struct pidmap_t));
  map->count = 0;
  map->capacity = 4;
  map->pairs = (struct pmkv_t *)calloc(map->capacity, sizeof(struct pmkv_t));
  return map;
}

void pidmap_destroy(struct pidmap_t *map) {
  free(map->pairs);
  free(map);
}

struct task_t *pidmap_get(struct pidmap_t *map, int pid) {
  int i;
  if (tryProbe(map, pid, &pid, &i)) {
    return map->pairs[i].task;
  }
  return NULL;
}

void pidmap_add(struct pidmap_t *map, struct task_t *task) {
  if (map->count == map->capacity) {
    grow(map, map->capacity * 2);
  }

  int i;
  tryProbe(map, task->pid, NULL, &i);

  assert(i >= 0 && i < map->capacity);

  map->pairs[i].pid = task->pid;
  map->pairs[i].task = task;
  map->count++;

}

void pidmap_remove(struct pidmap_t *map, int pid) {
  int i;
  if (tryProbe(map, pid, &pid, &i)) {
    memset(&map->pairs[i], 0, sizeof(struct pmkv_t));
  }
}

void pidmap_update(struct pidmap_t *map, int pid, struct task_t *newValue) {
  int i;
  if (tryProbe(map, pid, &pid, &i)) {
    map->pairs[i].task = newValue;
  }
}

static void grow(struct pidmap_t *map, int newSize) {
  assert(newSize > map->capacity && newSize > map->count);

  struct pmkv_t *new = (struct pmkv_t *)calloc(newSize, sizeof(struct pmkv_t));

  struct pidmap_t temp = (struct pidmap_t){
    .count = map->count,
    .capacity = newSize,
    .pairs = new
  };
  for (size_t i = 0; i < map->capacity; i++) {
    if (map->pairs[i].task == NULL) {
      continue;
    }
    int j;
    tryProbe(&temp, map->pairs[i].pid, NULL, &j);
    memcpy(&new[j], &map->pairs[i], sizeof(struct pmkv_t));
  }


  free(map->pairs);
  map->pairs = new;
  map->capacity = newSize;
}
