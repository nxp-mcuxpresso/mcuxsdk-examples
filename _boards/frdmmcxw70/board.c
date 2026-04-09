/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include "fsl_common.h"
#include "fsl_glikey.h"
#include "fsl_debug_console.h"
#include "board.h"
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Initialize debug console.
 */
void BOARD_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq = 0U;

    CLOCK_SetIpSrc(kCLOCK_Lpuart1, kCLOCK_IpSrcFro192M);
    CLOCK_SetIpSrcDiv(kCLOCK_Lpuart1, 1U);
    CLOCK_EnableClock(kCLOCK_Fro_hf_div);

    uartClkSrcFreq = CLOCK_GetIpFreq(kCLOCK_Lpuart1);

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

/*!
 * @brief Enable write for the Glikey protected registers.
 *
 * @param base GLIKEY peripheral base pointer
 * @param idx target index.
 */
void GlikeyWriteEnable(GLIKEY_Type *base, uint8_t idx)
{
    (void)GLIKEY_SyncReset(base);

    (void)GLIKEY_StartEnable(base, idx);
    (void)GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP1);
    (void)GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP2);
    (void)GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP3);
    (void)GLIKEY_ContinueEnable(base, GLIKEY_CODEWORD_STEP_EN);
}

/*!
 * @brief Reset the Glikey to init status.
 *
 * @param base GLIKEY peripheral base pointer
 */
void GlikeyClearConfig(GLIKEY_Type *base)
{
    (void)GLIKEY_SyncReset(base);
}