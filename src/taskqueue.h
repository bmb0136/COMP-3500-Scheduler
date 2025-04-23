#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include "task.h"

/*
 * struct tqnode_t: Double linked list node for taskqueue_t
 *
 * Fields:
 * - task: The task to store in the node
 * - next: A pointer to the next node in the queue
 * - prev: A pointer to the previous node in the queue
 */
struct tqnode_t {
  struct task_t task;
  struct tqnode_t *next, *prev;
};

/*
 * struct taskqueue_t: A queue of task_t. Implemented with a double linked list
 *
 * Fields:
 * - start: A pointer the first node in the queue
 * - end: A pointer the last node in the queue
 * - count: The number of tasks in the queue
 */
struct taskqueue_t {
  struct tqnode_t *start, *end;
  int count;
};

/*
 * Constructor and Destructor methods
 */
struct taskqueue_t *taskqueue_create();
void taskqueue_destroy(struct taskqueue_t *queue);

/*
 * taskqueue_push(): Add a task to the queue
 *
 * Arguments:
 * - queue: The queue to add to
 * - task: The task to add
 */
void taskqueue_push(struct taskqueue_t *queue, struct task_t task);

/*
 * taskqueue_pop(): Remove a task from the queue. Returns 1 on success, and 0 on failure
 *
 * Arguments:
 * - queue: The queue to pop from
 * - task: A pointer to save the popped task to (may be NULL)
 */
int taskqueue_pop(struct taskqueue_t *queue, struct task_t *output);

#endif
