#include "taskqueue.h"
#include <stdlib.h>

static struct tqnode_t* newNode(struct task_t task);

int taskqueue_pop(struct taskqueue_t *queue, struct task_t *output) {
  exit(1); // TODO
}

void taskqueue_push(struct taskqueue_t *queue, struct task_t task) {
  if (queue->count == 0) {
    queue->start = queue->end = newNode(task);
    queue->count = 1;
    return;
  }
}

void taskqueue_destroy(struct taskqueue_t *queue) {
  exit(1); // TODO
}

static struct tqnode_t* newNode(struct task_t task) {
  struct tqnode_t *n = (struct tqnode_t*)malloc(sizeof(struct tqnode_t));
  n->next = NULL;
  n->task = task;
  return n;
}
