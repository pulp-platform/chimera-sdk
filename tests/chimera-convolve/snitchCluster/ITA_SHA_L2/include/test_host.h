// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _TEST_HOST_INCLUDE_GUARD_
#define _TEST_HOST_INCLUDE_GUARD_

#include <stdint.h>

typedef struct {
    const int8_t *interm_Pq;
    const int8_t *interm_Pk;
    const int8_t *interm_Pv;
    const int8_t *interm_qk;
    const int8_t *interm_attention;
    const int8_t *interm_output;
} ita_sha_l2_test_args_t;

#endif //_TEST_HOST_INCLUDE_GUARD_
