/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 2.1 4/27/25
 *
 * A HashMap<int, struct task_t*>
 *
 * Sources: Notes from COMP 3270 and COMP 2210
 */

#ifndef PIDMAP_H
#define PIDMAP_H

#include "task.h"
#include <stdlib.h>

/*
 * struct pmkv_t: A key value pair to be stored inside of a pidmap_t
 *
 * Fields:
 * - pid: The key
 * - task: The value
 * - next: Chain
 */
struct pmkv_t {
  int pid;
  struct task_t *task;
  struct pmkv_t *next;
};

/*
 * struct pidmap_t: A hasmap that maps int -> task_t*
 *
 * Fields:
 * - count: The number of tasks in the map
 * - capacity: The size of pairs
 * - pairs: A dynamic array of key value pairs
 */
struct pidmap_t {
  int count;
  int capacity;
  struct pmkv_t **pairs;
};

/*
 * Constructor and Destructor methods
 */
struct pidmap_t *pidmap_create();
void pidmap_destroy(struct pidmap_t *map);

/*
 * pidmap_get(): Look up a PID in the map. May return NULL
 *
 * Arguments:
 * - map: The map to look in
 * - pid: The key to look for
 */
struct task_t* pidmap_get(struct pidmap_t *map, int pid);

/*
 * pidmap_add(): Add a task to the map
 *
 * Arguments:
 * - map: The map to add to
 * - task: A pointer to the task to add
 */
void pidmap_add(struct pidmap_t *map, struct task_t *task);

/*
 * pidmap_remove(): Delete an item from the map
 *
 * Arguments:
 * - map: The map to delete from
 * - pid: The PID of the task to remove
 */
void pidmap_remove(struct pidmap_t *map, int pid);

/*
 * pidmap_update(): Update a task in the map
 *
 * Arguments:
 * - map: The map to update
 * - pid: The PID of the task to update
 * - newValue: A new pointer to the task
 */
void pidmap_update(struct pidmap_t *map, int pid, struct task_t *newValue);

/*
 * pidmap_setCapacity(): Grow the hash table to the specified size
 *
 * Arguments:
 * - map: The map to grow
 * - newSize: The new size, must be > map->count
 */
void pidmap_setCapacity(struct pidmap_t *map, size_t newSize);

#endif
