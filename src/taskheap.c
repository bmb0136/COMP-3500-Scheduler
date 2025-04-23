#include "taskheap.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static int parent(int i) { return (i - 1) / 2; }

static int left(int i) { return (2 * i) + 1; }

static int right(int i) { return (2 * i) + 2; }

static void swap(struct taskheap_t *heap, int a, int b) {
  if (a == b) {
    return;
  }

  assert(a >= 0 && a < heap->count);
  assert(b >= 0 && b < heap->count);

  struct task_t temp = heap->items[a];
  heap->items[a] = heap->items[b];
  heap->items[b] = temp;
  pidmap_update(heap->pidmap, heap->items[a].pid, &heap->items[a]);
  pidmap_update(heap->pidmap, heap->items[b].pid, &heap->items[b]);
}

static int compare(struct taskheap_t *heap, int a, int b) {
  return heap->comparator(&heap->items[a], &heap->items[b]);
}

struct taskheap_t *taskheap_create(thcmp_fp comparator, thupdate_fp keyUpdater) {
  struct taskheap_t *heap =
      (struct taskheap_t *)malloc(sizeof(struct taskheap_t));
  heap->count = 0;
  heap->capacity = 4;
  heap->items = (struct task_t *)malloc(heap->capacity * sizeof(struct task_t));
  heap->pidmap = pidmap_create();
  heap->comparator = comparator;
  heap->keyUpdater = keyUpdater;
  return heap;
}

void taskheap_destroy(struct taskheap_t *heap) {
  pidmap_destroy(heap->pidmap);
  free(heap->items);
  free(heap);
}

void taskheap_push(struct taskheap_t *heap, struct task_t task) {
  if (heap->count + 1 >= heap->capacity) {
    heap->capacity *= 2;
    struct task_t *old = heap->items;
    heap->items =
        (struct task_t *)malloc(heap->capacity * sizeof(struct task_t));
    for (size_t i = 0; i < heap->count; i++) {
      struct task_t *t = &old[i];
      heap->items[i] = *t;
      pidmap_update(heap->pidmap, t->pid, &heap->items[i]);
    }
    free(old);
  }

  int i = heap->count++;
  heap->items[i] = task;

  while (i >= 0 && parent(i) >= 0 && compare(heap, i, parent(i)) < 0) {
    swap(heap, i, parent(i));
    i = parent(i);
  }

  pidmap_add(heap->pidmap, &heap->items[i]);
}

static void heapifyDown(struct taskheap_t *heap, int start) {
  int i = start;
  while (i < heap->count) {
    int L = left(i);
    int R = right(i);
    int cl = L < heap->count ? compare(heap, i, L) > 0 : 0;
    int cr = R < heap->count ? compare(heap, i, R) > 0 : 0;
    char tiebreaker = 1;
    if (cl > 0 && cr > 0) {
      tiebreaker = compare(heap, L, R) < 0;
    }
    if (L < heap->count && cl && tiebreaker) {
      swap(heap, i, L);
      i = L;
    } else if (R < heap->count && cr) {
      swap(heap, i, R);
      i = R;
    } else {
      break;
    }
  }
}

char taskheap_pop(struct taskheap_t *heap, struct task_t *output) {
  if (heap->count < 1) {
    return 0;
  }

  if (output) {
    *output = heap->items[0];
  }

  pidmap_remove(heap->pidmap, heap->items[0].pid);

  if (heap->count > 1) {
    swap(heap, 0, heap->count - 1);
    heap->count--;
    heapifyDown(heap, 0);
  } else {
    heap->count--;
  }

  return 1;
}

void taskheap_updatekey(struct taskheap_t *heap, int pid, int key) {
  struct task_t *pos = pidmap_get(heap->pidmap, pid);
  heap->keyUpdater(pos, key);

  int offset = pos - &heap->items[0];
  assert(offset >= 0 && offset < heap->count);
  assert(heap->items[offset].pid == pid);

  swap(heap, 0, offset);
  heapifyDown(heap, 0);
}
