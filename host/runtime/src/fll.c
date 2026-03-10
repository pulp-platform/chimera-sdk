// SPDX-FileCopyrightText: 2025 ETH Zurich and University of Bologna
// SPDX-License-Identifier: Apache-2.0

// Include Standard Libraries
#include <stdint.h>
#include <stddef.h>

// Include Target Specific Headers
#include "soc.h"

// Include Driver Headers
#include "driver.h"
#include "sw/device/lib/dif/dif_gpio.h"

// Include Runtime Headers
#include "fll.h"
#include "log.h"

static const dif_gpio_t gpio = {
    .base_addr = (volatile void *)&__base_gpio,
};

int calculate_fll_params(uint32_t target_freq, uint32_t rtc_freq, uint32_t *mult, uint32_t *div) {
    if (!mult || !div || target_freq == 0 || rtc_freq == 0) return -1;

    const uint32_t FMAX = 3300000000u; // 3.3 GHz (fits in 32-bit)
    uint32_t denom = 2u * target_freq; // safe while target_freq <= ~2.147e9
    if (denom == 0u) return -1;        // overflow guard

    // Strict bound: Dout < limit
    const uint32_t limit = FMAX / denom;
    // Smallest Dout is 1 (when div=1). Need 1 < limit to have any valid Dout.
    if (limit <= 1u) {
        printf_log("Error: Target frequency too high for FLL\n");
        return -1;
    }

    const uint32_t below = limit - 1u;
    const uint32_t e = 31u - __builtin_clz(below); // e in [0..30] for our ranges
    const uint32_t dout = 1u << e;

    // Round multiplier for a closer Fout: mult ≈ target * dout / rtc
    const uint32_t num = target_freq * dout;
    uint32_t m = (num + rtc_freq / 2u) / rtc_freq;
    if (m == 0u) m = 1u;

    *div = e + 1u; // because Dout = 2^(div-1)
    *mult = m;

    // // Optional quick check:
    // uint32_t fout = (rtc_freq * (*mult)) / (1u << (*div - 1));
    // // printf_log("target=%u, div=%u, dout=%u, mult=%u, fout=%u\n", target_freq, *div, dout,
    // *mult, fout); printf_log("Calculated FLL params: mult=%u, div=%u => fout=%u Hz (error: %+d
    // ppm)\n",
    //            *mult, *div, fout,
    //            (int32_t)(((int64_t)fout - (int64_t)target_freq) * 1000000 / target_freq));

    return 0;
}

uint32_t configure_fll(uint32_t target_freq, uint32_t rtc_freq) {
#ifndef CHIMERA_DRIVER_FLL
    printf_log("Error: FLL driver not included in build\n");
    return -1;
#else

    uint32_t mult, div;
    dif_result_t result;

    if (calculate_fll_params(target_freq, rtc_freq, &mult, &div) != 0) {
        printf_log("Error: Cannot calculate FLL parameters for %u Hz\n", target_freq);
        return 0;
    }

#if defined(TARGET_PLATFORM_CHIMERA_CONVOLVE)
    result = dif_gpio_write(&gpio, 2, 1);
    if (result != kDifOk) {
        printf_log("Error: Cannot enable FLL bypass\n");
        return 0;
    }
#endif
    // Configure both FLLs (SOC and Cluster)
    volatile uint32_t *fllPtr = getFllPtr(0);
    initFll(fllPtr);
    setFllFreq(fllPtr, mult, div);

    fllPtr = getFllPtr(1);
    initFll(fllPtr);
    setFllFreq(fllPtr, mult, div);

    // Delay for FLL lock
    for (volatile int i = 0; i < 1000; i++);

#if defined(TARGET_PLATFORM_CHIMERA_CONVOLVE)
    // Disable FLL bypass
    result = dif_gpio_write(&gpio, 2, 0);
    if (result != kDifOk) {
        printf_log("Error: Cannot disable FLL bypass\n");
        return 0;
    }
#endif

    // Relative Error = 1 / 128 = 0.78%
    // uint32_t ref_time_inv = rtc_freq / 128; // 32768 // 128 = 256
    // uint32_t core_freq_fll = clint_get_core_freq(rtc_freq, ref_time_inv);

    // // Relative Error = 1 / 32768 = 30.5 ppm
    uint32_t ref_time_inv = 1;
    uint32_t core_freq_fll = clint_get_core_freq(rtc_freq, ref_time_inv);

    // Reconfigure UART with new frequency
    uart_config_t uart_cfg = default_uart_cfg;
    uart_cfg.clk_freq_hz = core_freq_fll;
    default_uart_inst.cfg = &uart_cfg;

    if (iface_open(&default_uart_inst) != 0) {
        return 0;
    }

    return core_freq_fll;
#endif
}

uint32_t restore_default_freq(uint32_t rtc_freq) {
#ifndef CHIMERA_DRIVER_FLL
    printf_log("Error: FLL driver not included in build\n");
    return -1;
#else

    dif_result_t result;

#if defined(TARGET_PLATFORM_CHIMERA_CONVOLVE)
    // Enable FLL bypass
    result = dif_gpio_write(&gpio, 2, 1);
    if (result != kDifOk) {
        printf_log("Error: Cannot enable FLL bypass\n");
        return 0;
    }
#endif

    // Calculate default core frequency
    // Relative Error = 1 / 128 = 0.78%
    // Absolute Error = 78.125 kHz
    uint32_t ref_time_inv = rtc_freq / 128; // 32768 // 128 = 256
    uint32_t core_freq = clint_get_core_freq(rtc_freq, ref_time_inv);

    // Reconfigure UART with default frequency
    uart_config_t uart_cfg = default_uart_cfg;
    uart_cfg.clk_freq_hz = core_freq;
    default_uart_inst.cfg = &uart_cfg;

    if (iface_open(&default_uart_inst) != 0) {
        return 0;
    }

    return core_freq;
#endif
}