// SPDX-FileCopyrightText: 2022 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

// Include Standard Libraries
#include <stdint.h>

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers
typedef struct __attribute__((packed)) {
    uint32_t errors;
    uint32_t runtime_cycles;
    uint32_t ops_per_cycle;  // Operations per 1M cycles
    uint32_t ops_per_second; // 1M Operations per s
} test_cluster_result_t;

typedef struct __attribute__((packed)) {
    volatile int32_t repetitions;
    volatile test_cluster_result_t *result;
    volatile void *args;
} test_cluster_args_t;

typedef enum {
    TEST_MODE_AUTOMATIC = 0,
    TEST_MODE_DUTCTL = 1,
    TEST_MODE_INTERACTIVE = 2,
} test_mode_t;

typedef struct {
    char *name; // Name of the test
    test_mode_t mode;
    uint32_t default_frequency_mhz; // Default frequency in MHz in automatic mode (-1 to skip FLL)
    uint32_t default_repetitions;   // Default number of repetitions in automatic mode
    uint32_t timeout;               // Timeout in milliseconds
    uint8_t clusterId;              // Cluster ID to run the test on
    void *stack_start;              // Start address of the stack
    uint32_t *stack_sizes;          // Stack sizes for each core in the cluster
    void *function_test;            // Test function to execute on the cluster
    void *function_interrupt;       // Interrupt handler function
    test_cluster_args_t *args;      // Arguments to pass to the test function
} test_cluster_cfg_t;

/**
 * @brief Execute test on specified cluster.
 *
 * Make sure that all GPIOs are initialized properly
 */
int test_cluster(test_cluster_cfg_t *test_cfg);