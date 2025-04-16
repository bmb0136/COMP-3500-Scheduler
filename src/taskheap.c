#include "taskheap.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static size_t parent(size_t i) {
  return (i - 1) / 2;
}

static size_t left(size_t i) {
  return (2 * i) + 1;
}

static size_t right(size_t i) {
  return (2 * i) + 2;
}

static void swap(struct taskheap_t *heap, size_t a, size_t b) {
  struct task_t temp = heap->items[a];
  heap->items[a] = heap->items[b];
  heap->items[b] = temp;
  pidmap_update(heap->pidmap, heap->items[a].pid, &heap->items[b]);
  pidmap_update(heap->pidmap, heap->items[b].pid, &heap->items[a]);
}

struct taskheap_t *taskheap_create() {
  struct taskheap_t *heap =
      (struct taskheap_t *)malloc(sizeof(struct taskheap_t));
  heap->count = 0;
  heap->capacity = 4;
  heap->items = (struct task_t *)malloc(heap->capacity * sizeof(struct task_t));
  heap->pidmap = pidmap_create();
  return heap;
}

void taskheap_destroy(struct taskheap_t *heap) {
  pidmap_destroy(heap->pidmap);
  free(heap->items);
  free(heap);
}

void taskheap_push(struct taskheap_t *heap, struct task_t task) {
  if (heap->count >= heap->capacity) {
    heap->capacity *= 2;
    struct task_t *old = heap->items;
    heap->items = (struct task_t*)malloc(heap->capacity * sizeof(struct task_t));
    memcpy(heap->items, old, heap->count * sizeof(struct task_t));
    free(old);
  }

  size_t i = heap->count++;
  heap->items[i] = task;

  while (i >= 0 && heap->items[i].burstTime < heap->items[parent(i)].burstTime) {
    swap(heap, i, parent(i));
    i = parent(i);
  }

  pidmap_add(heap->pidmap, &heap->items[i]);
}

char taskheap_pop(struct taskheap_t *heap, struct task_t *output) {
  if (heap->count < 1) {
    return 0;
  }

  if (output) {
    *output = heap->items[0];
  }

  pidmap_remove(heap->pidmap, heap->items[0].pid);
  heap->items[0] = heap->items[--heap->count];

  size_t i = 0;
  while (i < heap->count) {
    int L = left(i);
    int R = right(i);
    if (heap->items[i].burstTime > heap->items[L].burstTime) {
      swap(heap, i, L);
      i = L;
    } else if (heap->items[i].burstTime > heap->items[R].burstTime) {
      swap(heap, i, R);
      i = R;
    } else {
      break;
    }
  }

  return 1;
}

void taskheap_updatekey(struct taskheap_t *heap, int pid, int key) {
  struct task_t *oldPos = pidmap_get(heap->pidmap, pid);
  
  assert(oldPos >= &heap->items[0] && oldPos <= &heap->items[heap->count - 1]);

  swap(heap, 0, oldPos - &heap->items[0]);
  heap->items[0].burstTime = key;

  size_t i = 0;
  while (i < heap->count) {
    int L = left(i);
    int R = right(i);
    if (heap->items[i].burstTime > heap->items[L].burstTime) {
      swap(heap, i, L);
      i = L;
    } else if (heap->items[i].burstTime > heap->items[R].burstTime) {
      swap(heap, i, R);
      i = R;
    } else {
      break;
    }
  }
}
