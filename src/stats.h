#ifndef STATS_H
#define STATS_H

#include "config.h"
#include "pidmap.h"
#include "policy.h"

struct tinfo_t {
  struct task_t task;
  char hasRun;
  int firstRunTime, finishTime;
};

struct stats_t {
  struct pidmap_t *map;
  int waits, runs;
};

struct summary_t {
  float utilization, avgWait, avgResponse, avgTurnaround;
};

struct stats_t *stats_create(struct config_t *config);
void stats_destroy(struct stats_t *stats);

void stats_process(struct stats_t *stats, struct schedevent_t ev);
struct summary_t stats_summarize(struct stats_t *stats);

#endif
