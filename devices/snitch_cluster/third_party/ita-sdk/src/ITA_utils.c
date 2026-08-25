// SPDX-FileCopyrightText: 2023 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdio.h>

// Include Application Headers
#include "ITA_utils.h"

// Include Target Specific Headers

// Include Driver Headers

// Include Runtime Headers
#include "snrt.h"

int check(int step, const uint8_t *data, const uint8_t *golden, int size, const int8_t *l1_buffer,
          int l1_size) {
    printf("Checking step %d: size=%d bytes\n", step, size);

    int errors = 0;
    int first_error = -1;
    int last_error = -1;

    // Calculate the buffer size
    int buffer_size = l1_size / 4;

    // Pointers to the four buffers
    const int8_t *buffer[4] = {l1_buffer, l1_buffer + buffer_size, l1_buffer + 2 * buffer_size,
                               l1_buffer + 3 * buffer_size};

    int buffer_index = 0;

    int next_buffer_data_index = 0;
    int next_buffer_golden_index = 1;
    int buffer_data_index = 2;
    int buffer_golden_index = 3;

    int remaining = size;
    int processed = 0;

    int copy_size = (remaining < buffer_size) ? remaining : buffer_size;
    int check_size = copy_size;

    // Start the first DMA transfer for the first set of buffers
    snrt_dma_start_1d((void *)buffer[0], (void *)data, copy_size);
    snrt_dma_start_1d((void *)buffer[1], (void *)golden, copy_size);

    while (remaining > 0) {
        next_buffer_data_index = 2 * (!buffer_index);
        next_buffer_golden_index = 2 * (!buffer_index) + 1;
        buffer_data_index = 2 * buffer_index;
        buffer_golden_index = 2 * buffer_index + 1;

        check_size = (remaining < buffer_size) ? remaining : buffer_size;
        remaining -= check_size;
        processed += check_size;
        copy_size = (remaining < buffer_size) ? remaining : buffer_size;

        // Wait for the current DMA transfer to complete
        snrt_dma_wait_all();
        // Start the next DMA transfer for the next set of buffers if within bounds
        if (copy_size > 0) {
            snrt_dma_start_1d((void *)buffer[next_buffer_data_index], (void *)data + processed,
                              copy_size);
            snrt_dma_start_1d((void *)buffer[next_buffer_golden_index], (void *)golden + processed,
                              copy_size);
        }

        // Process the current buffers (buffer 0 and buffer 1)
        for (int j = 0; j < check_size; j++) {
            uint8_t data_val = buffer[buffer_data_index][j];
            uint8_t golden_val = buffer[buffer_golden_index][j];
            if (data_val != golden_val) {
                errors++;
            }
        }

        buffer_index = !buffer_index;
    }
    // Wait for the current DMA transfer to complete
    snrt_dma_wait_all();

    printf("Checking step %d: errors=%d\n", step, errors);
    return errors;
}
