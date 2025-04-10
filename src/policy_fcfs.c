#include "policy_fcfs.h"
#include "taskqueue.h"

static struct taskqueue_t* queue;
static int taskTimeLeft;

void policy_fcfs_init() {
  queue = taskqueue_create();
  taskTimeLeft = 0;
}

int policy_fcfs(int timeQuantum, struct task_t* newTask) {
  if (newTask) {
    taskqueue_push(queue, *newTask);
  }
  if (taskTimeLeft > 0) {
    taskTimeLeft--;
  } else {
    taskqueue_pop(queue, 0);

    if (queue->count == 0) {
      return -1;
    }

    taskTimeLeft = queue->start->task.burstTime;
  }
  return queue->start->task.pid;
}
