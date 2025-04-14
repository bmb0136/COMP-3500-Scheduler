#include "taskheap.h"
#include <stdlib.h>
#include <string.h>

static size_t parent(size_t i) {
  return (i - 1) / 2;
}

static size_t left(size_t i) {
  return (2 * i) + 1;
}

static size_t right(size_t i) {
  return (2 * i) + 2;
}

struct taskheap_t *taskheap_create() {
  struct taskheap_t *heap =
      (struct taskheap_t *)malloc(sizeof(struct taskheap_t));
  heap->count = 0;
  heap->capacity = 4;
  heap->items = (struct task_t *)malloc(heap->capacity * sizeof(struct task_t));
  return heap;
}

void taskheap_destroy(struct taskheap_t *heap) {
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

  struct task_t temp;
  while (i >= 0 && heap->items[i].burstTime < heap->items[parent(i)].burstTime) {
    temp = heap->items[i];
    heap->items[i] = heap->items[parent(i)];
    heap->items[parent(i)] = temp;
    i = parent(i);
  }
}

char taskheap_pop(struct taskheap_t *heap, struct task_t *output) {
  if (heap->count < 1) {
    return 0;
  }

  if (output) {
    *output = heap->items[0];
  }

  heap->items[0] = heap->items[--heap->count];

  size_t i = 0;
  struct task_t temp;
  while (i < heap->count) {
    int L = left(i);
    int R = right(i);
    if (heap->items[i].burstTime > heap->items[L].burstTime) {
      temp = heap->items[i];
      heap->items[i] = heap->items[L];
      heap->items[L] = temp;
      i = L;
    } else if (heap->items[i].burstTime > heap->items[R].burstTime) {
      temp = heap->items[i];
      heap->items[i] = heap->items[R];
      heap->items[R] = temp;
      i = R;
    } else {
      break;
    }
  }

  return 1;
}
