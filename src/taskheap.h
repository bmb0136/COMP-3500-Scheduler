/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 1.5 4/24/25
 *
 * An Indexed Priority Queue of task_t
 */

#ifndef TASKHEAP_H
#define TASKHEAP_H

#include "task.h"
#include "pidmap.h"

/*
 * Delegate int thcmp_fp(): Comparison function for taskheap_t
 *
 * Arguments: two tasks to compare
 *
 * Returns:
 * - 0 if a == b
 * - anything < 0 if a < b
 * - anything > 0 if a > b
 */
typedef int (*thcmp_fp)(struct task_t *a, struct task_t *b);

/*
 * Delegate void thupdate_fp(): Update the priority of a task
 *
 * Arguments:
 * - task: The task to update
 * - key: The new priority
 */
typedef void (*thupdate_fp)(struct task_t *task, int key);

/*
 * struct taskheap_t: Indexed Priority Queue of task_t
 *
 * Fields:
 * - items: A dynamic array of nodes
 * - capacity: The size of the dynamic array
 * - count: The number of tasks in the IPQ
 * - pidmap: Maps each task's PID to its position in the heap
 * - comparator: A function that compares the priority of two tasks, used to enforce the heap property
 * - keyUpdater: A function that updates the key for a task
 *
 * Remarks:
 * - An "Indexed Priority Queue" is a combination of a Heap and Hashmap that allows O(log n) updates to priorities.
 * - items is stored in heap order, e.g., left(i) = 2i+1, right(i) = 2i+2, and root == items[0]
 * - count <= capacity
 */
struct taskheap_t {
  struct task_t *items;
  int capacity;
  int count;
  struct pidmap_t *pidmap;
  thcmp_fp comparator;
  thupdate_fp keyUpdater;
};

/*
 * Constructor and Destructor methods
 */
struct taskheap_t *taskheap_create(thcmp_fp comparator, thupdate_fp keyUpdater);
void taskheap_destroy(struct taskheap_t *heap);

/*
 * taskheap_push(): Add a task to the IPQ
 *
 * Arguments:
 * - heap: The IPQ to add to
 * - task: The task to add
 */
void taskheap_push(struct taskheap_t *heap, struct task_t task);

/*
 * taskheap_pop(): Pop the minimum element off the top off the IPQ. Returns 1 on success, and 0 on failure
 *
 * Arguments:
 * - heap: The IPQ to pop from
 * - output: A pointer to where the popped task is saved (may be NULL)
 */
char taskheap_pop(struct taskheap_t *heap, struct task_t *output);

/*
 * taskheap_updatekey(): Update the priority of a task within the heap
 *
 * Arguments:
 * - heap: The IPQ to apply the change to
 * - pid: The PID of the task to update
 * - key: The new priority of the task
 */
void taskheap_updatekey(struct taskheap_t *heap, int pid, int key);

#endif
