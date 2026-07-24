/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Pre-silicon mbedTLS entropy stub for mimxrt2660evk: RT2660 has no in-tree
 * RNG/TRNG driver yet, so mbedtls_hardware_poll is not provided by the mbedtls3x
 * port. Returns a deterministic LCG-derived stream so the linker resolves and
 * the selftest's entropy stage produces a non-zero output. Replace with a real
 * RNG/TRNG source before relying on entropy quality.
 */

#include <stddef.h>
#include <stdint.h>

int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
    static uint32_t s_state = 0xDEADBEEFU;
    size_t i;

    (void)data;

    for (i = 0U; i < len; i++)
    {
        /* Numerical Recipes LCG; deterministic on purpose for pre-silicon. */
        s_state    = (s_state * 1664525U) + 1013904223U;
        output[i] = (unsigned char)(s_state >> 24);
    }

    if (olen != NULL)
    {
        *olen = len;
    }
    return 0;
}
