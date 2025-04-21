#include "simulation.h"
#include "policy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void emit(struct schedevent_t event);

void simulation_run(struct config_t *config, struct policy_t policy) {
  policy_init(policy);

  struct pinput_t input;

  input.futureTasks =
      (struct task_t *)malloc(config->numTasks * sizeof(struct task_t));
  input.numTasks = config->numTasks;
  memcpy(input.futureTasks, config->tasks,
         config->numTasks * sizeof(struct task_t));

  policy_run(policy, input, &emit);

  policy_destroy(policy);
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
}
