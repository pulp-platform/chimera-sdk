// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#pragma once

// Include Standard Libraries
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers

/**
 * @defgroup runtime Runtime
 * @brief Host runtime library for Chimera-SDK.
 */

/**
 * \defgroup runtime_alloc Memory Allocator
 * @ingroup runtime
 * @brief Memory allocation subsystem for Chimera-SDK.
 *
 * This module provides dynamic memory allocation functionality for the memory island.
 * It implements a simple linked-list based allocator that manages heap memory regions.
 *
 * The allocator uses a first-fit algorithm with a freelist to track available memory blocks.
 * Memory is aligned to 4-byte boundaries for optimal performance.
 *
 * @{
 */

/**
 * @brief Memory alignment value in bytes.
 *
 * All allocated memory blocks are aligned to this boundary for optimal access performance.
 */
#define ALLOC_ALIGNMENT 4

/**
 * @brief Macro to align a size to the memory alignment boundary.
 *
 * @param size Size in bytes to align.
 * @return Aligned size in bytes.
 */
#define ALLOC_ALIGN(size) (((size) + (ALLOC_ALIGNMENT - 1)) & ~(ALLOC_ALIGNMENT - 1))

/**
 * @brief Memory block structure for the freelist allocator.
 *
 * This structure is used internally to track free memory blocks in a linked list.
 * Each allocated block has this header prepended to store metadata.
 */
typedef struct MemoryBlock {
    struct MemoryBlock *next; /**< Pointer to the next free block in the list. */
    size_t size;              /**< Size of the memory block in bytes. */
} MemoryBlock;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocates memory from the memory island heap.
 *
 * This function allocates a block of memory of the specified size from the memory island heap.
 * The allocated memory is aligned to ALLOC_ALIGNMENT bytes. The memory is not initialized.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to the allocated memory block, or NULL if allocation fails.
 *
 * @note The allocated memory should be freed using memory_island_free() when no longer needed.
 *
 * @see memory_island_free()
 */
void *memory_island_malloc(size_t size);

/**
 * @brief Frees memory previously allocated by memory_island_malloc().
 *
 * This function returns a memory block to the free list, making it available for future
 * allocations. The memory block is added to the head of the freelist for efficient reuse.
 *
 * @param ptr Pointer to the memory block to free. If NULL, the function does nothing.
 *
 * @note The pointer must have been returned by a previous call to memory_island_malloc().
 * @note After calling this function, the memory pointed to by ptr should not be accessed.
 *
 * @see memory_island_malloc()
 */
void memory_island_free(void *ptr);

/**
 * @brief Allocates and zero-initializes memory from the memory island heap.
 *
 * This function allocates memory for an array of num elements, each of size bytes,
 * and initializes all bytes to zero.
 *
 * @param num Number of elements to allocate.
 * @param size Size of each element in bytes.
 * @return Pointer to the allocated and zero-initialized memory block, or NULL if allocation fails.
 *
 * @note The allocated memory should be freed using memory_island_free() when no longer needed.
 *
 * @see memory_island_malloc()
 * @see memory_island_free()
 */
void *memory_island_calloc(size_t num, size_t size);

/**
 * @brief Reallocates memory from the memory island heap.
 *
 * This function changes the size of the memory block pointed to by ptr to size bytes.
 * The contents are unchanged up to the minimum of the old and new sizes.
 * If the new size is larger, the additional memory is uninitialized.
 *
 * @param ptr Pointer to the memory block to reallocate. If NULL, behaves like
 * memory_island_malloc().
 * @param size New size in bytes. If 0 and ptr is not NULL, behaves like memory_island_free().
 * @return Pointer to the reallocated memory block, or NULL if reallocation fails.
 *
 * @note If reallocation fails, the original memory block is unchanged.
 * @note The returned pointer may be different from the input pointer.
 *
 * @see memory_island_malloc()
 * @see memory_island_free()
 */
void *memory_island_realloc(void *ptr, size_t size);

/**
 * @brief Gets the total size of the memory island heap.
 *
 * @return Total heap size in bytes.
 */
size_t memory_island_heap_size(void);

/**
 * @brief Gets the amount of free memory available in the memory island heap.
 *
 * This function calculates the total amount of free memory by traversing the freelist
 * and summing the sizes of all free blocks, plus any remaining unallocated heap space.
 *
 * @return Amount of free memory in bytes.
 */
size_t memory_island_heap_free(void);

/**
 * @brief Gets the amount of allocated memory in the memory island heap.
 *
 * @return Amount of allocated memory in bytes.
 */
size_t memory_island_heap_used(void);

/**
 * @brief Checks if a pointer was allocated by the memory island allocator.
 *
 * This function verifies if the given pointer points to a memory block that was
 * allocated by memory_island_malloc() and is within the heap bounds.
 *
 * @param ptr Pointer to check.
 * @return true if the pointer is valid and within heap bounds, false otherwise.
 */
bool memory_island_ptr_valid(void *ptr);

/**
 * @brief Performs a consistency check on the memory island heap.
 *
 * This function validates the integrity of the heap data structures, including:
 * - Freelist consistency
 * - Memory block header validity
 * - Heap boundary checks
 *
 * @return true if the heap is consistent, false if corruption is detected.
 *
 * @note This function is intended for debugging and should not be used in production code
 *       due to its performance impact.
 */
bool memory_island_heap_check(void);

#ifdef __cplusplus
}
#endif

/** @} */ // end defgroup runtime_alloc
