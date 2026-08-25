
// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include Target Specific Headers
#include "soc.h"
#include "test.h"

// Include Driver Headers
#include "driver.h"
#include "sw/device/lib/dif/dif_gpio.h"

// Include Runtime Headers
#include "log.h"
#include "fll.h"
#include "util.h"
#include "clint.h"

// Import HAL Headers

extern uintptr_t volatile tohost, fromhost;

int test_cluster(test_cluster_cfg_t *test_cfg) {
    dif_result_t result;
    uint32_t retVal = 0;

    // Read the RTC frequency from a hardware register
    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);

#if defined(HARDWARE_BACKEND_ASIC)
    restore_default_freq(rtc_freq);

    // Calculate the initial core frequency from the RTC frequency
    // Relative Error = 1 / 128 = 0.78%
    // Absolute Error = 78.125 kHz
    uint32_t ref_time_inv = rtc_freq / 128; // 32768 // 128 = 256
    uint32_t core_freq = clint_get_core_freq(rtc_freq, ref_time_inv);
#else
    uint32_t core_freq = 500000000; // 500 MHz for RTL
    printf("Assuming core frequency: %u.%03u MHz\n", (core_freq / 1000000), (core_freq % 1000000));
#endif

    volatile uint32_t *scratch =
        (volatile uint32_t *)(&__base_regs + CHESHIRE_SCRATCH_0_REG_OFFSET);

    if (test_cfg->mode != TEST_MODE_DUTCTL) {
        printf("\n\n");
        printf_log("========================================\n");
        printf_log("Chimera %s Test \n", test_cfg->name);
        printf_log("========================================\n");
        printf_log("Initial frequency: %u.%03u MHz\n", (core_freq / 1000000),
                   (core_freq % 1000000));
    }

    // Buffer to hold run again
    char input_buffer[32];
    do {
        if (test_cfg->mode == TEST_MODE_DUTCTL) {
            // In DUTCTL mode, run once with default parameters
            for (int id = 0; id < test_cfg->clusters; id++) {
                uint8_t clusterId = test_cfg->clusterIds[id];
                test_cluster_args_t *arg = test_cfg->args[id];
                arg->repetitions = scratch[1];
                printf("@dutctl:dutmeas:cfg_repetitions_%d:%d\r\n", clusterId,
                       test_cfg->args[id]->repetitions);
            }
        } else if (test_cfg->mode == TEST_MODE_INTERACTIVE) {
            printf_log("Enter number of repetitions (default 1): ");
            fflush(stdout);

            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                printf("\n");
                printf_log("Error reading input\n");
                return -1;
            }
            int repetitions = 1;
            if (strlen(input_buffer) > 1) {
                repetitions = atoi(input_buffer);
            }
            printf("%d\n", repetitions);
            for (int id = 0; id < test_cfg->clusters; id++) {
                test_cluster_args_t *arg = test_cfg->args[id];
                arg->repetitions = repetitions;
            }
        } else {
            // In other modes, use default of 1
            for (int id = 0; id < test_cfg->clusters; id++) {
                test_cluster_args_t *arg = test_cfg->args[id];
                arg->repetitions = test_cfg->default_repetitions;
            }
        }

        if (test_cfg->mode == TEST_MODE_DUTCTL) {
            uint32_t core_mV = scratch[2]; // e.g. 900
            printf("@dutctl:dutmeas:cfg_core_mV:%d\r\n", core_mV);
            printf("@dutctl:psuctl:%d.%d:0:E36312A_1:1\r\n", core_mV / 1000, core_mV % 1000);
        }

#if defined(HARDWARE_BACKEND_ASIC)
        int target_freq_mhz = 0;
        if (test_cfg->mode == TEST_MODE_DUTCTL) {
            // In DUTCTL mode, read target frequency from scratch register
            target_freq_mhz = scratch[3]; // e.g. 170
            printf("@dutctl:dutmeas:cfg_fll_MHz:%d\r\n", target_freq_mhz);
        } else if (test_cfg->mode == TEST_MODE_INTERACTIVE) {
            // Ask user for target frequency
            printf_log(
                "Enter target frequency in MHz (10-1000, or -1 to skip FLL configuration): ");
            fflush(stdout);

            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                printf("\n");
                printf_log("Error reading input\n");
                return -1;
            }

            target_freq_mhz = atoi(input_buffer);
            printf("%d\n", target_freq_mhz);
        } else {
            // In other modes, default to 200 MHz
            printf_log("Using target frequency = %d MHz\n", test_cfg->default_frequency_mhz);
            target_freq_mhz = test_cfg->default_frequency_mhz;
        }

        uint32_t actual_freq = core_freq;

        if (target_freq_mhz == -1) {
            printf_log("Skipping FLL configuration, using external clock\n");
        } else if (target_freq_mhz < 10 || target_freq_mhz > 1000) {
            printf_log("Error: Frequency out of range (10-1000 MHz)\n");
            continue;
        } else {
            if (test_cfg->mode != TEST_MODE_DUTCTL) {
                printf_log("Configuring system to %d MHz...\n", target_freq_mhz);
            }

            uint32_t target_freq = target_freq_mhz * 1000000;
            actual_freq = configure_fll(target_freq, rtc_freq);
            if (actual_freq == 0) {
                printf_log("Error: Failed to configure FLL\n");
                return -1;
            }
            if (test_cfg->mode != TEST_MODE_DUTCTL) {
                printf_log("FLL configured successfully!\n");
                printf_log("Actual frequency: %u.%03u MHz\n", (actual_freq / 1000000),
                           (actual_freq % 1000000));
            }
            if (test_cfg->mode == TEST_MODE_DUTCTL) {
                printf("@dutctl:dutmeas:meas_fll_MHz:%d.%03d\r\n", (actual_freq / 1000000),
                       (actual_freq % 1000000));
            }
        }

#else
        uint32_t actual_freq = core_freq;
#endif
        // Setup cluster
        setup_snitchCluster_interruptHandler(test_cfg->function_interrupt);
        void *stack_cluster_ptr[test_cfg->clusters][NUM_CLUSTER_CORES];
        for (int id = 0; id < test_cfg->clusters; id++) {
            uint8_t clusterId = test_cfg->clusterIds[id];

            generate_snitchCluster_SPs(clusterId, test_cfg->stack_start[id],
                                       test_cfg->stack_sizes[id], stack_cluster_ptr[id]);

            set_snitchCluster_clockGating(clusterId, 0);

            set_snitchCluster_reset(clusterId, 1);
            for (volatile int i = 0; i < 10; i++);
            set_snitchCluster_reset(clusterId, 0);
            for (volatile int i = 0; i < 1000; i++);
        }

        // Record start time
        clint_mtime_t start_time = clint_get_mtime();
        uint32_t timeout_ticks = rtc_freq * test_cfg->timeout / 1000;
        clint_mtime_t deadline = start_time;
        deadline.low += timeout_ticks;
        if (deadline.low < start_time.low) {
            deadline.high += 1; // Handle overflow
        }

        if (test_cfg->mode == TEST_MODE_DUTCTL) {
            // For 500 iterations, computation should last at least 1s even at 400MHz
            printf("@dutctl:psumeas:meas_core_V:500:E36312A_1:1\r\n");
        }

        // Disable interrupts
        set_mie(0);

        for (int id = 0; id < test_cfg->clusters; id++) {
            test_cluster_args_t *arg = test_cfg->args[id];
            offload_snitchCluster(test_cfg->function_test, (void *)arg, stack_cluster_ptr[id],
                                  test_cfg->clusterIds[id]);
        }
        // Enable interrupts
        set_mie(1);

        // Handle tohost/fromhost communication with timeout
        int timed_out = 0;
        int done = 0;
        while (done != test_cfg->clusters && !timed_out) {
            done = 0;
            for (int id = 0; id < test_cfg->clusters; id++) {
                done += snitchCluster_busy(test_cfg->clusterIds[id]) ? 0 : 1;
            }
            // Check for timeout
            clint_mtime_t current_time = clint_get_mtime();
            if (clint_mtime_less_than(deadline, current_time)) {
                printf_log("Error: Cluster execution timed out after %u ms\n", test_cfg->timeout);
                if (test_cfg->mode == TEST_MODE_DUTCTL) {
                    printf("@dutctl:dutmeas:meas_errors:-1\n");
                }
                timed_out = 1;
                break;
            }

            // Wait for tohost to be set by the device
            if (tohost != 0) {
                volatile uint32_t syscall_addr = tohost;

                // Acknowledge tohost
                tohost = 0;

                // Cluster does tohost = (uintptr_t)buf->hdr.syscall_mem;
                uint32_t *syscall_mem = (uint32_t *)syscall_addr;

                if (syscall_mem[0] == 64) { // sys_write
                    fwrite((const void *)syscall_mem[2], 1, syscall_mem[3], (FILE *)syscall_mem[1]);
                    fflush((FILE *)syscall_mem[1]);
                } else {
                    printf_log("Unknown syscall: %u\n", syscall_mem[0]);
                }

                // Notify cluster that syscall is done
                fromhost = syscall_addr;
            }
            // Enter low-power mode until next interrupt
            clint_sleep_ticks(0, 10);
        }

        for (int id = 0; id < test_cfg->clusters; id++) {
            uint8_t clusterId = test_cfg->clusterIds[id];
            test_cluster_args_t *arg = test_cfg->args[id];
            retVal = 0;
            if (!timed_out) {
                retVal = wait_snitchCluster_return(test_cfg->clusterIds[id]);
                retVal = retVal >> 1;
            }

            set_snitchCluster_clockGating(test_cfg->clusterIds[id], 1);

            // Calculate and display metrics
            if (test_cfg->mode != TEST_MODE_DUTCTL) {
                printf_log("----------------------------------------\n");
                printf_log("Execution Results from cluster %d\n", clusterId);
                printf_log("----------------------------------------\n");
                printf_log("Return value: 0x%08x (%d errors)\n", retVal, retVal);
            }

            if (!timed_out) {
                // Calculate operations per second
                // Ops in Op/cycle * 1e6
                // Frequency in Hz
                // Ops per cycle in  Op / cycle * 1e6 * 1e-3 * Hz * 1e-3 = Op/s
                uint32_t kops_per_sec =
                    ((arg->result)->ops_per_cycle / 10000) * (actual_freq / 100000);

                if (test_cfg->mode == TEST_MODE_DUTCTL) {
                    printf("@dutctl:dutmeas:meas_ops_per_cycle_%d:%u.%06u\n", clusterId,
                           (arg->result)->ops_per_cycle / 1000000,
                           (arg->result)->ops_per_cycle % 1000000);
                    // MOp/s
                    printf("@dutctl:dutmeas:meas_ops_per_second_%d:%u.%03u\n", clusterId,
                           kops_per_sec / 1000, kops_per_sec % 1000);
                    printf("@dutctl:dutmeas:meas_runtime_cycles_%d:%u\n", clusterId,
                           (arg->result)->runtime_cycles);
                    printf("@dutctl:dutmeas:meas_errors_%d:%u\n", clusterId, (arg->result)->errors);
                } else {
                    printf_log("Op/Cycle: %u.%06u\n", (arg->result)->ops_per_cycle / 1000000,
                               (arg->result)->ops_per_cycle % 1000000);
                    printf_log("Op/s: %u.%03u M\n", kops_per_sec / 1000, kops_per_sec % 1000);
                    printf_log("Runtime Cycles: %u\n", (arg->result)->runtime_cycles);
                }
            }
        }

#if defined(HARDWARE_BACKEND_ASIC)
        // Restore default frequency if FLL was used
        if (target_freq_mhz != -1 && target_freq_mhz >= 10 && target_freq_mhz <= 1000) {
            uint32_t restored_freq = restore_default_freq(rtc_freq);
            if (test_cfg->mode != TEST_MODE_DUTCTL) {
                printf_log("Restored to %u.%03u MHz\n", (restored_freq / 1000000),
                           (restored_freq % 1000000));
            }
        }
#endif

        if (test_cfg->mode == TEST_MODE_INTERACTIVE) {
            printf_log("========================================\n");
            printf_log("Run again? (y/n): ");
            fflush(stdout);
            if (fgets(input_buffer, 2, stdin) == NULL ||
                (input_buffer[0] != 'y' && input_buffer[0] != 'Y')) {
                break;
            }
            printf("%s\n", input_buffer);
        } else {
            break;
        }

        if (timed_out) {
            return -1;
        }
    } while (1);

    return retVal;
}
