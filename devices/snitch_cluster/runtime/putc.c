// SPDX-FileCopyrightText: 2020 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

/*
 * Snitch cluster character output via HTIF semihosting.
 *
 * Each core owns a per-core putc_buffer_t in .noinit (not zeroed at startup
 * to avoid a dependency on runtime init before the first print).  Characters
 * are buffered until either the buffer is full (PUTC_BUFFER_LEN = 1023 bytes)
 * or a newline is encountered.
 *
 * On flush, the core fills a syscall_mem[8] header (sys_write, fd, buf, len)
 * and writes its address to shared_data.device_to_host.  It then sets the
 * CLINT MSIP bit to trigger a machine-software interrupt on the host (hart 0),
 * which the host trap handler services and signals completion by writing a
 * non-zero value back to shared_data.host_to_device.
 *
 * A TTAS mutex (_snrt_printf_mutex_ptr, allocated in L1 by snrt_printf_init)
 * serialises concurrent flushes across cores so that lines are not interleaved.
 */

// Include Standard Libraries
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#include "snrt.h"
#include "soc.h"
#include "shared.h"

volatile uint32_t *_snrt_printf_mutex_ptr;

void snrt_printf_init() {
    _snrt_printf_mutex_ptr = snrt_l1_alloc(sizeof(uint32_t));
    *_snrt_printf_mutex_ptr = 0;
    return;
}

int snrt_printf_log(const char *fmt, ...) {
    int ret;
    va_list args;

    // Get core and cluster id
    int core_id = snrt_cluster_core_idx();
    int cluster_id = snrt_cluster_idx();

    fprintf(stdout, "[%02d:%02d] ", cluster_id, core_id);

    va_start(args, fmt);
    ret = vfprintf(stdout, fmt, args);
    va_end(args);

    return ret;
}

int snrt_printf(const char *fmt, ...) {
    int ret;
    va_list args;

    va_start(args, fmt);
    ret = vfprintf(stdout, fmt, args);
    va_end(args);

    return ret;
}

// Rudimentary string buffer for putc calls.
#define PUTC_BUFFER_LEN (1024 - sizeof(size_t))

typedef struct {
    size_t size;
    uint32_t syscall_mem[8];
} putc_buffer_header_t;

typedef struct putc_buffer {
    putc_buffer_header_t hdr;
    char data[PUTC_BUFFER_LEN];
} putc_buffer_t;

static volatile putc_buffer_t putc_buffer[SNRT_CLUSTER_NUM * SNRT_CLUSTER_CORE_NUM]
    __attribute__((section(".noinit")));

// Provide an implementation for putchar.
int snrt_putchar(char c, FILE *file) {
    (void)file;
    volatile struct putc_buffer *buf = &putc_buffer[snrt_global_core_idx()];
    buf->data[buf->hdr.size++] = c;
    if (buf->hdr.size == PUTC_BUFFER_LEN || c == '\n') {
        buf->hdr.syscall_mem[0] = 64;                    // sys_write
        buf->hdr.syscall_mem[1] = (uintptr_t)stdout;     // File Descriptor
        buf->hdr.syscall_mem[2] = (uintptr_t)&buf->data; // Buffer
        buf->hdr.syscall_mem[3] = buf->hdr.size;         // Length

        snrt_mutex_ttas_acquire(_snrt_printf_mutex_ptr);
        shared_data.device_to_host = (uintptr_t)buf->hdr.syscall_mem;

        // Trigger MSIP (machine software interrupt) on host (core 0)
        *reg32(&__base_clint, CLINT_MSIP_REG_OFFSET) = 1;
        while (shared_data.host_to_device == 0);
        shared_data.host_to_device = 0;
        buf->hdr.size = 0;
        snrt_mutex_release(_snrt_printf_mutex_ptr);
    }
    return c;
}
