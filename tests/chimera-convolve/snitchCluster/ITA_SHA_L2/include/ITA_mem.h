// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

#ifndef _ITA_MEM_INCLUDE_GUARD_
#define _ITA_MEM_INCLUDE_GUARD_

#include <stdint.h>

#include "ITA_types.h"
#include "test_cluster.h"

// clang-format off
extern const int8_t input_q[SEQUENCE_LENGTH*EMBEDDING_SPACE];
extern const int8_t input_k[SEQUENCE_LENGTH*EMBEDDING_SPACE];
extern const int8_t input_Wq[1][EMBEDDING_SPACE*PROJECTION_SPACE];
extern const int8_t input_Wk[1][EMBEDDING_SPACE*PROJECTION_SPACE];
extern const int8_t input_Wv[1][EMBEDDING_SPACE*PROJECTION_SPACE];
extern const int8_t input_Wo[1][EMBEDDING_SPACE*PROJECTION_SPACE];
extern const ita_int24_t input_Bq[1][PROJECTION_SPACE];
extern const ita_int24_t input_Bk[1][PROJECTION_SPACE];
extern const ita_int24_t input_Bv[1][PROJECTION_SPACE];
extern const ita_int24_t input_Bo[1][EMBEDDING_SPACE];
extern const uint32_t requant_eps_mult[1][2];
extern const uint32_t requant_right_shift[1][2];
extern const int32_t requant_add[1][2];
extern const int8_t golden_interm_Pq[1][SEQUENCE_LENGTH*PROJECTION_SPACE];
extern const int8_t golden_interm_Pk[1][SEQUENCE_LENGTH*PROJECTION_SPACE];
extern const int8_t golden_interm_Pv[1][SEQUENCE_LENGTH*PROJECTION_SPACE];
extern const int8_t golden_interm_attention[1][SEQUENCE_LENGTH*SEQUENCE_LENGTH];
extern const int8_t golden_interm_head_output[1][SEQUENCE_LENGTH*PROJECTION_SPACE];
extern const int8_t golden_output[1][SEQUENCE_LENGTH*EMBEDDING_SPACE];

// clang-format on

#endif