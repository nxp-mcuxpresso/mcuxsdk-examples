/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"

#include "board.h"
#include "cmsis.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();

    BOARD_BootClockRUN();

    /* Also Setup clock for LPUART1 */
    BOARD_InitDebugConsole();
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
  WDOG0->CS = (uint32_t) ((WDOG0->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK;

  while ((WDOG0->CS & WDOG_CS_RCS_MASK) != WDOG_CS_RCS_MASK)
  {
  }
#endif /* (DISABLE_WDOG) */

    /* Disable aGDET interrupt and reset */
    SPC0->ACTIVE_CFG |= SPC_ACTIVE_CFG_GLITCH_DETECT_DISABLE_MASK;
    SPC0->LP_CFG |= SPC_ACTIVE_CFG_GLITCH_DETECT_DISABLE_MASK;
    SPC0->GLITCH_DETECT_SC &= ~SPC_GLITCH_DETECT_SC_LOCK_MASK;
    SPC0->GLITCH_DETECT_SC = 0x3D;
    SPC0->GLITCH_DETECT_SC |= SPC_GLITCH_DETECT_SC_LOCK_MASK;
  
    extern void *__VECTOR_TABLE[];
    SCB->VTOR = (uint32_t)&__VECTOR_TABLE[0];
}
/*${function:end}*/
