#ifndef POLICY_SRTF_H
#define POLICY_SRTF_H
#include "task.h"

void policy_srtf_init();
int policy_srtf(int timeQuantum, struct task_t* newTask);

#endif
