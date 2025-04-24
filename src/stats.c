/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 1.3 4/24/25
 *
 * Implementation of stats.h
 */

#include "stats.h"
#include "pidmap.h"
#include "policy.h"
#include "task.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stats_t *stats_create(struct config_t *config) {
  struct stats_t *s = (struct stats_t *)malloc(sizeof(struct stats_t));
  memset(s, 0, sizeof(struct stats_t));
  s->map = pidmap_create();
  pidmap_setCapacity(s->map, config->numTasks * 2);
  for (size_t i = 0; i < config->numTasks; i++) {
    struct tinfo_t *t = (struct tinfo_t *)malloc(sizeof(struct tinfo_t));
    memset(t, 0, sizeof(struct tinfo_t));
    assert((void *)t == (void *)&t->task);
    memcpy(&t->task, &config->tasks[i], sizeof(struct task_t));
    pidmap_add(s->map, &t->task);
  }
  return s;
}

void stats_destroy(struct stats_t *stats) {
  for (size_t i = 0; i < stats->map->capacity; i++) {
    struct pmkv_t *n = stats->map->pairs[i];
    while (n) {
      struct pmkv_t *temp = n;
      n = n->next;
      free((struct tinfo_t *)temp->task);
    }
  }
  pidmap_destroy(stats->map);
  free(stats);
}

void stats_process(struct stats_t *stats, struct schedevent_t event) {
  if (event.type == SE_WAIT) {
    stats->waits++;
    return;
  }
  if (event.type == SE_DONE) {
    return;
  }

  struct tinfo_t *t =
      ((struct tinfo_t *)pidmap_get(stats->map, event.data.task));

  switch (event.type) {
  case SE_RUN:
    stats->runs++;
    if (!t->hasRun) {
      t->hasRun = 1;
      t->firstRunTime = event.time;
    }
    break;
  case SE_FINISH:
    t->finishTime = event.time;
    break;
  default:
    break;
  }
}

struct summary_t stats_summarize(struct stats_t *stats) {
  long turnAround = 0;
  long wait = 0;
  long response = 0;
  for (size_t i = 0; i < stats->map->capacity; i++) {
    struct pmkv_t *n = stats->map->pairs[i];
    while (n) {
      struct tinfo_t *t = (struct tinfo_t*)n->task;
      long ta = t->finishTime - t->task.startTime;
      turnAround += ta;
      wait += ta - t->task.burstTime;
      response += t->firstRunTime - t->task.startTime;
      n = n->next;
    }
  }

  long count = stats->map->count;
  long totalTime = stats->runs + stats->waits;

  return (struct summary_t){
      .avgTurnaround = count == 0 ? 0 : (float)turnAround / count,
      .avgWait = count == 0 ? 0 : (float)wait / count,
      .avgResponse = count == 0 ? 0 : (float)response / count,
      .utilization = totalTime == 0 ? 0 : (float)stats->runs / totalTime,
  };
}
