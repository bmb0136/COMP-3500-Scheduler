#ifndef STATS_H
#define STATS_H

#include "config.h"
#include "pidmap.h"
#include "policy.h"

/*
 * struct tinfo_t: Information used to calculate statistics for a task_t
 *
 * Fields:
 * - task: The task to collect information on. DO NOT MOVE FROM FIRST POSITION, this field is used to refer to the whole struct via a pointer to the task only
 * - hasRun: Whether or not the task has been executed (via an SE_RUN event) yet
 * - firstRunTime: The time of the first SE_RUN event for this task
 * - finishTime: The time that the tasked finished (via an SE_FINISH event)
 */
struct tinfo_t {
  struct task_t task;
  char hasRun;
  int firstRunTime, finishTime;
};

/*
 * struct stats_t: Statstics collector
 *
 * Fields:
 * - map: a pidmap_t that is used to store extra information on each task
 * - waits: The number of SE_WAIT events encountered
 * - runs: The number of SE_RUN events encountered
 */
struct stats_t {
  struct pidmap_t *map;
  int waits, runs;
};

/*
 * struct summary_t: Statstics summary returned from stats_summarize()
 *
 * Fields:
 * - utilization: The CPU utilization percentage (0-1 range)
 * - avgWait: The average waiting time
 * - avgResponse: The average response time
 * - avgTurnaround: The average turnaround time
 */
struct summary_t {
  float utilization, avgWait, avgResponse, avgTurnaround;
};

/*
 * Constructor and Destructor methods
 */
struct stats_t *stats_create(struct config_t *config);
void stats_destroy(struct stats_t *stats);

/*
 * stats_process(): Process and collect statistics on a schedevent_t
 *
 * Arguments:
 * - stats: The stats_t object to mutate
 * - ev: The event to process
 */
void stats_process(struct stats_t *stats, struct schedevent_t ev);

/*
 * stats_summarize(): Return statistical information collected via stats_process()
 *
 * Arguments:
 * - stats: The stats_t object to summarize
 */
struct summary_t stats_summarize(struct stats_t *stats);

#endif
