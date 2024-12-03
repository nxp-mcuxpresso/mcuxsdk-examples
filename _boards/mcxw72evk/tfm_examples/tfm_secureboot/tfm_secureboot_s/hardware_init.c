/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"

#include "board.h"
#include "cmsis.h"
void BOARD_InitHardware(void)
{
    BOARD_InitPins();

    BOARD_BootClockRUN();

    /* Setup clock for LPUART1 */
    CLOCK_SetIpSrc(kCLOCK_Lpuart1, kCLOCK_IpSrcFro6M);
}

void SystemInit(void)
{
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));    /* set CP10, CP11 Full Access in Secure mode */
    SCB_NS->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10, CP11 Full Access in Non-secure mode */
#endif                                                    /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

    SCB->CPACR |= ((3UL << 0 * 2) | (3UL << 1 * 2));    /* set CP0, CP1 Full Access in Secure mode (enable PowerQuad) */
    SCB_NS->CPACR |= ((3UL << 0 * 2) | (3UL << 1 * 2)); /* set CP0, CP1 Full Access in Normal mode (enable PowerQuad) */

    SCB->NSACR |= ((3UL << 0) | (3UL << 10));           /* enable CP0, CP1, CP10, CP11 Non-secure Access */

#if (DISABLE_WDOG)
    while ((WDOG0->CS & WDOG_CS_RCS_MASK) != WDOG_CS_RCS_MASK)
    {
    }

    if ((WDOG0->CS & WDOG_CS_CMD32EN_MASK) != 0U)
    {
        WDOG0->CNT = 0xD928C520U;
    }
    else
    {
        WDOG0->CNT = 0xC520U;
        WDOG0->CNT = 0xD928U;
    }

    while ((WDOG0->CS & WDOG_CS_ULK_MASK) != WDOG_CS_ULK_MASK)
    {
    }

    WDOG0->TOVAL = 0xFFFF;
    WDOG0->CS    = (uint32_t)((WDOG0->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK;

    while ((WDOG0->CS & WDOG_CS_RCS_MASK) != WDOG_CS_RCS_MASK)
    {
    }
#endif /* (DISABLE_WDOG) */
    extern void *__VECTOR_TABLE[];
    SCB->VTOR = (uint32_t)&__VECTOR_TABLE[0];
}
