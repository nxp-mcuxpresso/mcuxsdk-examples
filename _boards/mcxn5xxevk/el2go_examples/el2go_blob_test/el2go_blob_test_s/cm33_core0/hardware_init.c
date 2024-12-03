/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

/* Same SystemInit() as in system_MCXN547_cm33_core0.c, but with TF-M vector table and TRACECLK disabled */
void SystemInit(void)
{
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access in Secure mode */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    SCB_NS->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access in Non-secure mode */
#endif /* (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */
#endif /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

    SCB->CPACR |= ((3UL << 0*2) | (3UL << 1*2));    /* set CP0, CP1 Full Access in Secure mode (enable PowerQuad) */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    SCB_NS->CPACR |= ((3UL << 0*2) | (3UL << 1*2));    /* set CP0, CP1 Full Access in Normal mode (enable PowerQuad) */
#endif /* (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

    SCB->NSACR |= ((3UL << 0) | (3UL << 10));   /* enable CP0, CP1, CP10, CP11 Non-secure Access */

    SYSCON->ECC_ENABLE_CTRL = 0; /* disable RAM ECC to get max RAM size */

    SYSCON->NVM_CTRL &= ~SYSCON_NVM_CTRL_DIS_MBECC_ERR_DATA_MASK; /* enables bus error on multi-bit ECC error for data */

    extern void *__VECTOR_TABLE[];
    SCB->VTOR = (uint32_t) & (__VECTOR_TABLE[0]);

    SYSCON->TRACECLKDIV = 0;

    /* enable the flash cache LPCAC */
    SYSCON->LPCAC_CTRL &= ~SYSCON_LPCAC_CTRL_DIS_LPCAC_MASK;

    /* Disable aGDET trigger the CHIP_RESET */
    ITRC0->OUT_SEL[4][0] = (ITRC0->OUT_SEL[4][0] & ~ITRC_OUT_SEL_IN9_SELn_MASK) | (ITRC_OUT_SEL_IN9_SELn(0x2));
    ITRC0->OUT_SEL[4][1] = (ITRC0->OUT_SEL[4][1] & ~ITRC_OUT_SEL_IN9_SELn_MASK) | (ITRC_OUT_SEL_IN9_SELn(0x2));
    /* Disable aGDET interrupt and reset */
    SPC0->ACTIVE_CFG |= SPC_ACTIVE_CFG_GLITCH_DETECT_DISABLE_MASK;
    SPC0->GLITCH_DETECT_SC &= ~SPC_GLITCH_DETECT_SC_LOCK_MASK;
    SPC0->GLITCH_DETECT_SC = 0x3C;
    SPC0->GLITCH_DETECT_SC |= SPC_GLITCH_DETECT_SC_LOCK_MASK;

    /* Disable dGDET trigger the CHIP_RESET */
    ITRC0->OUT_SEL[4][0] = (ITRC0->OUT_SEL[4][0] & ~ ITRC_OUT_SEL_IN0_SELn_MASK) | (ITRC_OUT_SEL_IN0_SELn(0x2));
    ITRC0->OUT_SEL[4][1] = (ITRC0->OUT_SEL[4][1] & ~ ITRC_OUT_SEL_IN0_SELn_MASK) | (ITRC_OUT_SEL_IN0_SELn(0x2));
    GDET0->GDET_ENABLE1 = 0;
    GDET1->GDET_ENABLE1 = 0;
}
/*${function:end}*/
