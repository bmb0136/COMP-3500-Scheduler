#include "taskqueue.h"
#include <stdlib.h>

static struct tqnode_t* newNode(struct task_t task);

int taskqueue_pop(struct taskqueue_t *queue, struct task_t *output) {
  if (queue->count == 0) {
    return 0;
  }

  struct tqnode_t *oldStart = queue->start;
  *output = oldStart->task;
  queue->start = oldStart->next;

  free(oldStart);

  queue->count--;
  if (queue->count == 0) {
    queue->end = NULL;
  }
  return 1;
}

void taskqueue_push(struct taskqueue_t *queue, struct task_t task) {
  struct tqnode_t *t = newNode(task);

  if (queue->count == 0) {
    queue->start = queue->end = t;
    queue->count = 1;
    return;
  }

  queue->end->next = t;
  t->prev = queue->end;
  queue->end = t;
  queue->count++;
}

void taskqueue_destroy(struct taskqueue_t *queue) {
  struct tqnode_t *n = queue->start;

  while (n) {
    struct tqnode_t *next = n->next;
    free(n);
    n = next;
  }

  free(queue);
}

static struct tqnode_t* newNode(struct task_t task) {
  struct tqnode_t *n = (struct tqnode_t*)malloc(sizeof(struct tqnode_t));
  n->next = NULL;
  n->prev = NULL;
  n->task = task;
  return n;
}
