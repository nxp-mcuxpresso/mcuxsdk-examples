/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "haps_mock.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define HAPS_CLOCK_FREQ    12000000U

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * @brief Mock implementation of CLOCK_GetFreq for HAPS environment
 * @param clockName Clock source name
 * @return Clock frequency in Hz
 */
uint32_t CLOCK_GetFreq(uint32_t clockName)
{
    // In HAPS environment, TEST_LPUART_CLKSRC always returns 12MHz
    if (clockName == HAPS_CLOCK)
    {
        return HAPS_CLOCK_FREQ;
    }
    
    // For other clock sources, return default value or handle as needed
    return 0U;
}