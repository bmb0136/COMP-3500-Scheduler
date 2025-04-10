#ifndef SIMULATION_H
#define SIMULATION_H

#include "policy.h"
#include "taskqueue.h"

struct simresult_t {
  float avgWaiting, avgResponse, avgTurnaround, cpuUsage;
};

struct simresult_t simulation_run(int timeQuantum, struct taskqueue_t *queue, policy_fp policy);

#endif
