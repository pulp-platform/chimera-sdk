// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdint.h>
#include <stddef.h>

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers
#include "alloc.h"

extern void *__l2_heap_start;
extern void *__l2_heap_end;

static uint8_t *memory_island_heap_end = (uint8_t *)&__l2_heap_end;
static uint8_t *memory_island_heap_start = (uint8_t *)&__l2_heap_start;

static MemoryBlock *memory_island_freelist = NULL;

static void *region_malloc(uint8_t **ptr, uint8_t *end, MemoryBlock **freelist, size_t size) {
    size_t total_size = ALLOC_ALIGN(size + sizeof(MemoryBlock));

    MemoryBlock **prev = freelist;
    MemoryBlock *curr = *freelist;

    while (curr) {
        if (curr->size >= size) {
            *prev = curr->next;
            return (void *)(curr + 1);
        }
        prev = &curr->next;
        curr = curr->next;
    }

    if (*ptr + total_size > end) return NULL;

    MemoryBlock *b = (MemoryBlock *)(*ptr);
    b->size = size;
    *ptr += total_size;

    return (void *)(b + 1);
}

static void region_free(MemoryBlock **freelist, void *ptr) {
    if (!ptr) return;
    MemoryBlock *b = (MemoryBlock *)((uint8_t *)ptr - sizeof(MemoryBlock));
    b->next = *freelist;
    *freelist = b;
}

void *memory_island_malloc(size_t size) {
    return region_malloc(&memory_island_heap_start, memory_island_heap_end, &memory_island_freelist,
                         size);
}

void memory_island_free(void *ptr) {
    region_free(&memory_island_freelist, ptr);
}