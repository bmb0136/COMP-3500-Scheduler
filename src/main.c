#include "config.h"
#include "taskqueue.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
  struct config_t *config = config_parse(argc, argv);
  if (!config) {
    return 1;
  }

  struct taskqueue_t* queue = taskqueue_create();
  for (int i = 0; i < config->numTasks; i++) {
    taskqueue_push(queue, config->tasks[i]);
  }

  config_destroy(config);
  taskqueue_destroy(queue);
  return 0;
}
