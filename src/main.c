#include "config.h"

int main(int argc, char** argv) {
  struct config_t *config = config_parse(argc, argv);
  if (!config) {
    return 1;
  }
}
