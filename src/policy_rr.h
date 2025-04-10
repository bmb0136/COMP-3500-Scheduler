#ifndef POLICY_RR_H
#define POLICY_RR_H
#include "task.h"

void policy_rr_init();
int policy_rr(int timeQuantum, struct task_t* newTask);

#endif
