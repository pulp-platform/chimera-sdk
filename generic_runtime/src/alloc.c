// Copyright 2025 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Victor Jung <jungvi@iis.ee.ethz.ch>

#include <stdint.h>
#include <stddef.h>

// JUNGVI: Number of bytes to align to.
#define ALIGNMENT 4
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

// JUNGVI: We should probably only put the allocator logic here and then instanciate allocators in
// the specific platform

// JUNGVI: The source of truth of this macros should be target specific
#define MEM_ISLAND_START 0x48000000
#define MEM_ISLAND_SIZE (64 * 1024) // In bytes

extern void * __heap_start;
extern void * __heap_end;

static uint8_t *memory_island_heap_end = (uint8_t *) &__heap_end;
static uint8_t *memory_island_heap_start = (uint8_t *) &__heap_start;

typedef struct MemoryBlock {
    struct MemoryBlock *next;
    size_t size;
} MemoryBlock;

static MemoryBlock *memory_island_freelist = NULL;

static void *region_malloc(uint8_t **ptr, uint8_t *end, MemoryBlock **freelist, size_t size) {
    size_t total_size = ALIGN(size + sizeof(MemoryBlock));

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
    return region_malloc(&memory_island_heap_start, memory_island_heap_end, &memory_island_freelist, size);
}

void memory_island_free(void *ptr) {
    region_free(&memory_island_freelist, ptr);
}