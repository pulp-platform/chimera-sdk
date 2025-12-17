// SPDX-FileCopyrightText: 2024 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

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

#ifdef CHIMERA_DRIVER_CLINT32
#include "clint32/clint32.h"
#endif

#ifdef CHIMERA_DRIVER_CLINT64
#include "clint64/clint64.h"
#endif

#ifdef CHIMERA_DRIVER_HYPERBUS
#include "hyperbus/hyperbus.h"
#endif

#endif //_DRIVER_INCLUDE_GUARD_
