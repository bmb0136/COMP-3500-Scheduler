#ifndef TASKRING_H
#define TASKRING_H

#include "task.h"
#include <stdlib.h>

/*
 * struct trnode_t: Double linked list node for taskring_t
 *
 * Fields:
 * - task: The task to store in the node
 * - next: A pointer to the next node
 * - prev: A pointer to the previous node
 *
 * Remarks:
 * - The value of next and prev may never be NULL
 * - A trnode_t may point to itself
 */
struct trnode_t {
  struct task_t task;
  struct trnode_t *next, *prev;
};

/*
 * struct taskring_t: A **cyclic** double linked list of task_t
 *
 * Fields:
 * - count: The number of tasks in the ring
 * - current: The current node in the ring. Effectively the "start" of the ring
 *
 * Remarks:
 * - current == NULL <=> count == 0
 */
struct taskring_t {
  size_t count;
  struct trnode_t *current;
};

/*
 * Constructor and Destructor methods
 */
struct taskring_t *taskring_create();
void taskring_destroy(struct taskring_t *ring);

/*
 * taskring_push(): Add a task to the ring. Adds the task before the current element
 *
 * Arguments:
 * - ring: The ring to add to
 * - task: The task to add
 */
void taskring_push(struct taskring_t *ring, struct task_t task);

/*
 * taskring_pop(): Remove the current task from the ring. Returns 1 on success, and 0 on failure
 *
 * Arguments:
 * - ring: The ring to add to
 * - task: Pointer to save the popped task to (may be NULL)
 */
char taskring_pop(struct taskring_t *ring, struct task_t *output);

/*
 * taskring_next(): Cycle to the next task in the ring
 *
 * Arguments:
 * - ring: The ring to advance
 */
void taskring_next(struct taskring_t *ring);

/*
 * taskring_prev(): Cycle to the previous task in the ring
 *
 * Arguments:
 * - ring: The ring to advance
 */
void taskring_prev(struct taskring_t *ring);

#endif
