// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "offload_snitchCluster.h"

// Include Runtime Headers
#include "util.h"
#include "shared.h"
#include "log.h"

// Import HAL Headers
#include "device_api.h"

/*
 * Snitch cluster offload driver implementation.
 *
 * _setup_trampoline() writes the function pointer, argument pointer, and stack pointer
 * into shared_data.trampoline_* arrays indexed by (hartID - CLUSTER_HART_BASE).  The
 * host truncates each pointer to 32 bits because cluster cores use RV32 — the upper
 * 32 bits of the host's RV64 pointer are discarded, which is safe because all shared
 * memory lies within the 32-bit physical address space.
 *
 * offload_snitchCluster() iterates over all cores in the cluster, calls
 * _setup_trampoline() for each, then releases the cluster from reset.
 * offload_snitchCluster_core() targets a single core.
 *
 * wait_snitchCluster_busy() polls the SOC_CTRL busy register.  The register is set
 * by the device-side trampoline and cleared when the cluster calls snrt_exit().
 */

/* Writes the function pointer, argument pointer, and stack pointer into the shared trampoline
 * arrays for the given hart. */
static void _setup_trampoline(uint32_t hartID, void (*function)(void *), void *args, void *stack) {
    uint32_t trampoline_idx = hartID - CLUSTER_HART_BASE;

    // Assign trampoline with captured arguments to the persistent function pointer
    shared_data.trampoline_function[trampoline_idx] =
        (uint32_t)((uint64_t)function & 0xFFFFFFFF); // Ensure LSB is 0 for RV32 pointer
    shared_data.trampoline_args[trampoline_idx] =
        (uint32_t)((uint64_t)args & 0xFFFFFFFF); // Ensure LSB is 0 for RV32 pointer
    shared_data.trampoline_stack[trampoline_idx] =
        (uint32_t)((uint64_t)stack & 0xFFFFFFFF); // Ensure LSB is 0 for RV32 pointer
}

/* Returns the global hart ID for the given cluster and core index. */
static uint32_t _get_hart_id(uint32_t clusterId, uint32_t core_id) {
    return _chimera_hartBase[clusterId] + core_id;
}

/* Sets the interrupt handler address for all Snitch cores via the SOC_CTRL register. */
void setup_snitchCluster_interruptHandler(void *handler) {
    volatile void **snitchTrapHandlerAddr =
        (volatile void **)(SOC_CTRL_BASE + CHIMERA_SNITCH_INTR_HANDLER_ADDR_REG_OFFSET);

    *snitchTrapHandlerAddr = handler;
}

/*
 * Generates 16-byte-aligned stack pointers for all cores in a cluster using per-core stack sizes.
 * Stacks grow downward from sp; returns the pointer to the end of allocated stack memory.
 */
void *generate_snitchCluster_SPs(uint8_t clusterId, void *sp, uint32_t *stack_size,
                                 void **stack_ptr) {
    for (uint32_t core_id = 0; core_id < _chimera_numCores[clusterId]; core_id++) {
        // Align to 16 Byte boundaries
        sp = (void *)((uintptr_t)sp & ~(uintptr_t)0xFUL);
        // set stack pointer
        stack_ptr[core_id] = sp;
        // subtract the required amount of stack
        sp -= stack_size[core_id];
    }
    return sp;
}

/*
 * Generates 16-byte-aligned stack pointers for all cores in a cluster using a uniform stack size.
 * Stacks grow downward from sp; returns the pointer to the end of allocated stack memory.
 */
void *generate_snitchCluster_SPs_uniform(uint8_t clusterId, void *sp, uint32_t stack_size,
                                         void **stack_ptr) {
    for (uint32_t core_id = 0; core_id < _chimera_numCores[clusterId]; core_id++) {
        // Align to 16 Byte boundaries
        sp = (void *)((uintptr_t)sp & ~(uintptr_t)0xFUL);
#ifdef TRACE
        printf("[TRACE] Cluster %d Core %d stack @ %p with size: %d \n", clusterId, core_id, sp,
               stack_size);
#endif
        // set stack pointer
        stack_ptr[core_id] = sp;
        // subtract the required amount of stack
        sp -= stack_size;
    }
    return sp;
}

/* Sets up the trampoline for a single core, programs the boot address, and sends a software
 * interrupt. */
void offload_snitchCluster_core(void *function, void *trampoline, void *args, void *stack_ptr,
                                uint8_t clusterId, uint32_t core_id) {
    volatile void **snitchBootAddr =
        (volatile void **)(SOC_CTRL_BASE + CHIMERA_SNITCH_BOOT_ADDR_REG_OFFSET);

    uint32_t hartId = _get_hart_id(clusterId, core_id);

    printf("Offloading to core %d in cluster %d with hartid %d\n", core_id, clusterId, hartId);

    _setup_trampoline(hartId, function, args, stack_ptr);

    *snitchBootAddr = trampoline;

    // Check if the cluster is busy
    wait_snitchCluster_busy(clusterId);
    volatile uint32_t *interruptTarget = ((uint32_t *)CLINT_CTRL_BASE) + hartId;
    *interruptTarget = 1;
}

/* Sets up trampolines for all cores in a cluster and sends a software interrupt to each. */
void offload_snitchCluster(void *function, void *trampoline, void *args, void **stack_ptr,
                           uint8_t clusterId) {
    volatile uint32_t *snitchBootAddr =
        (uint32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_BOOT_ADDR_REG_OFFSET);

    uint32_t hartId = _get_hart_id(clusterId, 0);

#ifdef TRACE
    printf_log("[TRACE] Offloading to all cores in cluster %d starting at hartid %d\n", clusterId,
               hartId);
#endif

    // Check if the cluster is busy
    wait_snitchCluster_busy(clusterId);

    for (uint32_t i = 0; i < _chimera_numCores[clusterId]; i++) {
        _setup_trampoline(hartId + i, function, args, stack_ptr[i]);
        *snitchBootAddr = (uint32_t)((uint64_t)trampoline & 0xFFFFFFFF);
        fence();
        // Send interrupt to the core
        volatile uint32_t *interruptTarget = ((uint32_t *)CLINT_CTRL_BASE) + hartId + i;
        *interruptTarget = 1;
    }

#ifdef TRACE
    printf("[TRACE] Trampoline Function: %p\n", trampoline);
    for (uint32_t i = 0; i < _chimera_numCores[clusterId]; i++) {
        uint32_t trampoline_idx = hartId - CLUSTER_HART_BASE + i;
        printf("[TRACE] Function [%02d:%02d] : %p @ %p\n", clusterId, i, function,
               &shared_data.trampoline_function[trampoline_idx]);
        printf("[TRACE] Args     [%02d:%02d] : %p @ %p\n", clusterId, i, args,
               &shared_data.trampoline_args[trampoline_idx]);
        printf("[TRACE] Stack    [%02d:%02d] : %p @ %p\n", clusterId, i, stack_ptr[i],
               &shared_data.trampoline_stack[trampoline_idx]);
    }
#endif

    // Prevent race contidtion by waiting for all cores to set the busy flag
    for (volatile int i = 0; i < 10; i++);
}

/* Returns 1 if the cluster is busy, 0 if idle, -1 if clusterId is out of range. */
int snitchCluster_busy(uint8_t clusterId) {
    volatile int32_t *busy_ptr;

    switch (clusterId) {
    case 0:
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_BUSY_REG_OFFSET);
        break;
    case 1:
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_BUSY_REG_OFFSET);
        break;
    case 2:
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_BUSY_REG_OFFSET);
        break;
    case 3:
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_BUSY_REG_OFFSET);
        break;
    case 4:
        busy_ptr = (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_BUSY_REG_OFFSET);
        break;
    default:
        return -1;
    }

    return *busy_ptr;
}

/*
 * Busy-waits until the cluster is idle, then inserts a short NOP delay to avoid a race condition
 * where cores have not yet set the busy flag after being released.
 */
void wait_snitchCluster_busy(uint8_t clusterId) {
    while (snitchCluster_busy(clusterId) == 1);
    // TODO: temporary race condition fix
    for (int i = 0; i < 5000; i++) {
        // NOP
        asm volatile("addi x0, x0, 0\n" :::);
    }

    return;
}

/*
 * Busy-waits until the cluster writes a non-zero return value to the SOC_CTRL return register,
 * then clears it and returns the value. The return value must be non-zero or this will loop
 * forever.
 */
uint32_t wait_snitchCluster_return(uint8_t clusterId) {
    volatile int32_t *snitchReturnAddr;
    switch (clusterId) {
    case 0:
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_0_RETURN_REG_OFFSET);
        break;
    case 1:
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_1_RETURN_REG_OFFSET);
        break;
    case 2:
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_2_RETURN_REG_OFFSET);
        break;
    case 3:
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_3_RETURN_REG_OFFSET);
        break;
    case 4:
        snitchReturnAddr =
            (volatile int32_t *)(SOC_CTRL_BASE + CHIMERA_SNITCH_CLUSTER_4_RETURN_REG_OFFSET);
        break;
    }

    while (*snitchReturnAddr == 0) {
    }

    uint32_t retVal = *snitchReturnAddr;
    *snitchReturnAddr = 0;
    return retVal;
}

/* Enables or disables clock gating for the specified cluster. */
void set_snitchCluster_clockGating(uint8_t clusterId, bool enable) {

    switch (clusterId) {
    case 0:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_CLK_GATE_EN_REG_OFFSET) = enable;
        break;
    case 1:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_CLK_GATE_EN_REG_OFFSET) = enable;
        break;
    case 2:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_CLK_GATE_EN_REG_OFFSET) = enable;
        break;
    case 3:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_CLK_GATE_EN_REG_OFFSET) = enable;
        break;
    case 4:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_CLK_GATE_EN_REG_OFFSET) = enable;
        break;
    default:
        break;
    }
}

/* Enables or disables clock gating for all clusters. */
void setAll_snitchCluster_clockGating(bool enable) {
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_0_CLK_GATE_EN_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_1_CLK_GATE_EN_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_2_CLK_GATE_EN_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_3_CLK_GATE_EN_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_CLUSTER_4_CLK_GATE_EN_REG_OFFSET) = enable;
}

/* Asserts or de-asserts soft reset for the specified cluster. */
void set_snitchCluster_reset(uint8_t clusterId, bool enable) {
    switch (clusterId) {
    case 0:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_0_REG_OFFSET) = enable;
        break;
    case 1:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_1_REG_OFFSET) = enable;
        break;
    case 2:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_2_REG_OFFSET) = enable;
        break;
    case 3:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_3_REG_OFFSET) = enable;
        break;
    case 4:
        *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_4_REG_OFFSET) = enable;
        break;
    default:
        break;
    }
}

/* Asserts or de-asserts soft reset for all clusters. */
void setAll_snitchCluster_reset(bool enable) {
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_0_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_1_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_2_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_3_REG_OFFSET) = enable;
    *(volatile uint8_t *)(SOC_CTRL_BASE + CHIMERA_RESET_CLUSTER_4_REG_OFFSET) = enable;
}

static int snitchcluster_open(chi_device_t *dev) {
    (void)dev;
    return 0;
}
static int snitchcluster_close(chi_device_t *dev) {
    (void)dev;
    return 0;
}
static ssize_t snitchcluster_read(chi_device_t *dev, void *buf, uint32_t len,
                                  chi_device_callback_t cb) {
    (void)dev;
    (void)buf;
    (void)len;
    (void)cb;
    return -1;
}

// const chi_device_api_t default_snitchcluster_api = {
//     .open = snitchcluster_open,
//     .close = snitchcluster_close,
//     .read = snitchcluster_read
//     .write = (ssize_t (*)(chi_device_t *, const void *, uint32_t,
//                           chi_device_callback_t))offload_snitchCluster
// };
