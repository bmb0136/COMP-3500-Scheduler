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
    struct task_t *t = stats->map->pairs[i].task;
    if (t) {
      free((struct tinfo_t *)t);
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
    // TODO
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
  int turnAround = 0;
  int wait = 0;
  int response = 0;
  for (size_t i = 0; i < stats->map->capacity; i++) {
    if (!stats->map->pairs[i].task) {
      continue;
    }
    struct tinfo_t *t = (struct tinfo_t *)stats->map->pairs[i].task;
    int ta = t->finishTime - t->task.startTime;
    turnAround += ta;
    wait += ta - t->task.burstTime;
    response += t->firstRunTime - t->task.startTime;
  }

  int count = stats->map->count;
  int totalTime = stats->runs + stats->waits;

  return (struct summary_t){
      .avgTurnaround = count == 0 ? 0 : (float)turnAround / count,
      .avgWait = count == 0 ? 0 : (float)wait / count,
      .avgResponse = count == 0 ? 0 : (float)response / count,
      .utilization = totalTime == 0 ? 0 : (float)stats->runs / totalTime,
  };
}
