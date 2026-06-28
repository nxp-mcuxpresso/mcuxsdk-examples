/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_soc_src.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    clock_root_config_t rootCfg = {0};

    BOARD_ConfigMPU();

    /* Workaround: Disable interrupt which might be enabled by ROM. */
    GPIO_DisableInterrupts(GPIO3, 1U << 24);
    GPIO_ClearPinsInterruptFlags(GPIO3, 1U << 24);

    /* Interrupt flags can't be cleared when wake up from SNVS mode, clear srtc and gpio interrupts here. */
    SNVS->LPSR |= SNVS_LPSR_LPTA_MASK;
    GPIO_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_GPIO_PIN);

    BOARD_InitBootPins();

    rootCfg.mux = kCLOCK_LPUART1_ClockRoot_MuxOscRc16M;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Lpuart1, &rootCfg);

    BOARD_InitDebugConsole();

    // set start address after waking up from SUSPEND mode
    IOMUXC_LPSR_GPR->GPR26 &= ~IOMUXC_LPSR_GPR_GPR26_CM7_INIT_VTOR_MASK;
    IOMUXC_LPSR_GPR->GPR26 |= IOMUXC_LPSR_GPR_GPR26_CM7_INIT_VTOR(IMAGE_ENTRY_ADDRESS >> 7);
}
/*${function:end}*/
