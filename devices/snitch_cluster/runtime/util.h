// SPDX-FileCopyrightText: 2022 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

// Include Standard Libraries
#include <stdint.h>

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers

/** @name Register Access Helpers
 *  @brief Functions for reading/writing memory-mapped registers.
 *  @{
 */

/**
 * @brief Gets a pointer to an 8-bit register.
 * @param base Base address of the register.
 * @param offset Register offset.
 * @return Pointer to the 8-bit register.
 */
static inline volatile uint8_t *reg8(uintptr_t base, int offset) {
    return (volatile uint8_t *)((uintptr_t)base + (intptr_t)offset);
}

/**
 * @brief Writes a value to an 8-bit register.
 * @param base Base address of the register.
 * @param offset Register offset.
 * @param val Value to write.
 */
static inline void reg8_write(uintptr_t base, int offset, uint8_t val) {
    *reg8(base, offset) = val;
}

/**
 * @brief Reads a value from an 8-bit register.
 * @param base Base address of the register.
 * @param offset Register offset.
 * @return Value read from the register.
 */
static inline uint8_t reg8_read(uintptr_t base, int offset) {
    return *reg8(base, offset);
}

/**
 * @brief Gets a pointer to a 32-bit register.
 * @param base Base address of the register.
 * @param offs Register offset.
 * @return Pointer to the 32-bit register.
 */
static inline volatile uint32_t *reg32(void *base, int offs) {
    return (volatile uint32_t *)((uintptr_t)base + (intptr_t)offs);
}

/** @} */

/** @name Memory and Execution Fencing
 *  @brief Functions to enforce memory ordering and instruction synchronization.
 *  @{
 */

/**
 * @brief Issues a memory fence to enforce ordering of memory accesses.
 */
static inline void fence() {
    asm volatile("fence" ::: "memory");
}

/**
 * @brief Issues an instruction fence to synchronize instruction fetches.
 */
static inline void fencei() {
    asm volatile("fence.i" ::: "memory");
}

/**
 * @brief Issues a wait-for-interrupt (WFI) instruction to enter low-power mode.
 */
static inline void wfi() {
    asm volatile("wfi" ::: "memory");
}

/** @} */

/** @name Interrupt Control
 *  @brief Functions to enable or disable interrupts.
 *  @{
 */

/**
 * @brief Enables or disables M-mode timer interrupts.
 * @param enable Set to 1 to enable, 0 to disable.
 */
static inline void set_mtie(int enable) {
    if (enable)
        asm volatile("csrs mie, %0" ::"r"(128) : "memory");
    else
        asm volatile("csrc mie, %0" ::"r"(128) : "memory");
}

/**
 * @brief Enables or disables M-mode global interrupts.
 * @param enable Set to 1 to enable, 0 to disable.
 */
static inline void set_mie(int enable) {
    if (enable)
        asm volatile("csrsi mstatus, 8" ::: "memory");
    else
        asm volatile("csrci mstatus, 8" ::: "memory");
}

/** @} */

/** @name Cycle Counter
 *  @brief Functions to retrieve system cycle count.
 *  @{
 */

/**
 * @brief Gets the lower 32 bits of the cycle counter since system reset.
 * @return 32-bit cycle count (wraps every ~4 billion cycles).
 * @note Only the lower 32 bits are returned. If callers rely on monotonic
 *       time across wrap boundaries, a 64-bit hi/lo read sequence should
 *       be used instead (read mcycleh, mcycle, mcycleh again to detect carry).
 */
static inline uint32_t get_mcycle() {
    uint32_t mcycle;
    asm volatile("csrr %0, mcycle" : "=r"(mcycle)::"memory");
    return mcycle;
}

/** @} */

/** @name Function Invocation Helpers
 *  @brief Functions for invoking dynamic function pointers.
 *  @{
 */

/**
 * @brief Invokes a function stored in memory.
 *
 * This function may also be used to invoke code that does not return.
 *
 * @param code Pointer to the function to invoke.
 * @return Return value of the invoked function.
 */
static inline uint32_t invoke(void *code) {
    uint32_t (*code_fun_ptr)(void) = code;
    fencei();
    return code_fun_ptr();
}

/**
 * @brief Sets the global pointer register and returns its previous value.
 *
 * @note Use with caution, as modifying the global pointer register (gp)
 *       can affect program execution.
 *
 * @param gp New global pointer value.
 * @return Previous global pointer value.
 */
static inline void *gprw(void *gp) {
    void *ret;
    asm volatile("mv %0, gp" : "=r"(ret)::"memory");
    if (gp) asm volatile("mv gp, %0" ::"r"(gp) : "memory", "gp");
    return ret;
}

/** @} */

/** @name Error Handling Macros
 *  @brief Macros for function return handling.
 *  @{
 */

/**
 * @brief Checks if a function call returns a nonzero value and returns early.
 *
 * This macro evaluates the given function call. If the return value is
 * nonzero, the calling function will return immediately with the same value.
 *
 * @param call Function call to check.
 */
#define CHECK_CALL(call) \
    { \
        int __ccret = (volatile int)(call); \
        if (__ccret) return __ccret; \
    }

/**
 * @brief Asserts a condition and returns an error code if it fails.
 *
 * If the condition evaluates to false, the function returns the provided error code.
 *
 * @param ret Error code to return.
 * @param cond Condition to check.
 */
#define CHECK_ASSERT(ret, cond) \
    if (!(cond)) return (ret);

/** @} */
