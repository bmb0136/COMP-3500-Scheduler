#include "policy.h"

static void init();
static void run(struct pinput_t input, pemit_fp emit);
static void destroy();

struct policy_t policy_rr_create() {
  return (struct policy_t) {
    .init = &init,
    .run = &run,
    .destroy = &destroy
  };
}

static void init() {
}

static void destroy() {
}

static void run(struct pinput_t input, pemit_fp emit) {
}
