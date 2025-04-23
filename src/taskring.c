/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 1.1 4/23/25
 *
 * Implementation of taskring.h
 */

#include "taskring.h"

struct taskring_t *taskring_create() {
  struct taskring_t *ring = (struct taskring_t*)malloc(sizeof(struct taskring_t));
  ring->count = 0;
  ring->current = NULL;
  return ring;
}

void taskring_destroy(struct taskring_t *ring) {
  struct trnode_t *n = ring->current;
  while (n) {
    struct trnode_t *n2 = n->next;
    free(n);
    n = n2;
  }
  free(ring);
}

void taskring_next(struct taskring_t *ring) {
  ring->current = ring->current->next;
}

void taskring_prev(struct taskring_t *ring) {
  ring->current = ring->current->prev;
}

void taskring_push(struct taskring_t *ring, struct task_t task) {
  struct trnode_t *n = (struct trnode_t*)malloc(sizeof(struct trnode_t));
  n->task = task;

  if (ring->count++ == 0) {
    ring->current = n;
    n->next = n;
    n->prev = n;
    return;
  }

  n->next = ring->current;
  n->prev = ring->current->prev;
  ring->current->prev->next = n;
  ring->current->prev = n;
}

char taskring_pop(struct taskring_t *ring, struct task_t *output) {
  if (ring->count < 1) {
    return 0;
  }

  if (output) {
    *output = ring->current->task;
  }

  struct trnode_t *n = ring->current;

  n->prev->next = n->next;
  n->next->prev = n->prev;
  ring->current = n->next;

  free(n);

  if (--ring->count == 0) {
    ring->current = NULL;
  }
  return 1;
}
