/*
 * COMP 3500 Project 5: CPU Scheduler
 * Brandon Buckley
 *
 * Version 2.4 4/23/25
 *
 * Code for running a policy simulation
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "config.h"
#include "policy.h"

/*
 * simulation_run(): Run a simulation
 *
 * Arguments:
 * - config: The simulation configuration
 * - policy: The policy to use
 */
void simulation_run(struct config_t *config, struct policy_t policy);

#endif
