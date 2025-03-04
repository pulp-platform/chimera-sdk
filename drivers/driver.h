// Copyright 2024 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Philip Wiese <wiesep@iis.ee.ethz.ch>
// Sergio Mazzola <smazzola@iis.ee.ethz.ch>

#ifndef _DRIVER_INCLUDE_GUARD_
#define _DRIVER_INCLUDE_GUARD_

// WIESEP: Common header for all drivers
#ifdef CHIMERA_DRIVER_SNITCH_CLUSTER
#include "snitch_cluster/offload_snitchCluster.h"
#endif

#ifdef CHIMERA_DRIVER_PULP_CLUSTER
#include "pulp_cluster/offload_pulpCluster.h"
#include "pulp_cluster/config_pulpCluster.h"
#endif

#endif //_DRIVER_INCLUDE_GUARD_
