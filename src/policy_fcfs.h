#ifndef POLICY_FCFS_H
#define POLICY_FCFS_H
#include "task.h"

void policy_fcfs_init();
int policy_fcfs(int timeQuantum, struct task_t* newTask);

#endif
