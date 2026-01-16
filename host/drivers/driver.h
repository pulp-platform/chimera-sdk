// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Philip Wiese <wiesep@iis.ee.ethz.ch>

#ifndef _DRIVER_INCLUDE_GUARD_
#define _DRIVER_INCLUDE_GUARD_

// WIESEP: Common header for all drivers
#ifdef CHIMERA_DRIVER_CLUSTER
#include "cluster/offload_snitchCluster.h"
#endif

#ifdef CHIMERA_DRIVER_FLL
#include "fll/fll.h"
#endif

#ifdef CHIMERA_DRIVER_UART_APB
#include "uart_apb/uart_apb.h"
#endif

#ifdef CHIMERA_DRIVER_UART_OPENTITAN
#include "uart_opentitan/uart_opentitan.h"
#endif

#ifdef CHIMERA_DRIVER_CLINT32
#include "clint32/clint32.h"
#endif

#ifdef CHIMERA_DRIVER_CLINT64
#include "clint64/clint64.h"
#endif

#endif //_DRIVER_INCLUDE_GUARD_
