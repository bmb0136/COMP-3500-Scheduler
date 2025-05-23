/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 3.2 4/23/25
 *
 * Implementation of simulation.h
 */

#include "simulation.h"
#include "policy.h"
#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct stats_t *stats;

static void emit(struct schedevent_t event);

void simulation_run(struct config_t *config, struct policy_t policy) {
  stats = stats_create(config);
  policy_init(policy);

  struct pinput_t input;

  input.futureTasks =
      (struct task_t *)malloc(config->numTasks * sizeof(struct task_t));
  input.numTasks = config->numTasks;
  memcpy(input.futureTasks, config->tasks,
         config->numTasks * sizeof(struct task_t));
  input.timeQuantum = config->timeQuantum;

  policy_run(policy, input, &emit);

  struct summary_t summary = stats_summarize(stats);
  puts("=======================================");
  printf("Average Waiting Time:    %.2f\n", summary.avgWait);
  printf("Average Response Time:   %.2f\n", summary.avgResponse);
  printf("Average Turnaround Time: %.2f\n", summary.avgTurnaround);
  printf("CPU Utilization:         %.2f%%\n", summary.utilization * 100.0f);
  puts("=======================================");

  policy_destroy(policy);
  stats_destroy(stats);
  free(input.futureTasks);
}

static void emit(struct schedevent_t event) {
  switch (event.type) {
  case SE_WAIT:
    printf("<time %d> CPU is idle\n", event.time);
    break;
  case SE_RUN:
    printf("<time %d> Process %d is running\n", event.time, event.data.task);
    break;
  case SE_FINISH:
    printf("<time %d> Process %d finished\n", event.time, event.data.task);
    break;
  case SE_DONE:
    printf("<time %d> All processes finished\n", event.time);
    break;
  default:
    puts("Bug: invalid event type");
    break;
  }
  stats_process(stats, event);
}
