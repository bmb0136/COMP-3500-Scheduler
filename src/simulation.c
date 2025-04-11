#include "simulation.h"

void simulation_run(struct policy_t policy) {
  policy.init();

  // TODO: create emit callback and run

  policy.destroy();
}
