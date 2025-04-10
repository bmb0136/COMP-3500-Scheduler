#ifndef POLICY_H
#define POLICY_H

#include "task.h"

typedef int(*policy_fp)(int timeQuantum, struct task_t* newTask);

#endif
