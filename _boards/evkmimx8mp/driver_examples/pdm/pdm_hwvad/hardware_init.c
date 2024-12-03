/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
/*${header:end}*/

void DEMO_PWM_IRQHandler(void);

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* M7 has its local cache and enabled by default,
     * need to set smart subsystems (0x28000000 ~ 0x3FFFFFFF)
     * non-cacheable before accessing this address region */
    BOARD_InitMemory();

    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootMux(kCLOCK_RootPdm,
                     kCLOCK_PdmRootmuxAudioPll1);  /* Set PDM source from AUDIO PLL1 run at 393216000HZ */
    CLOCK_SetRootDivider(kCLOCK_RootPdm, 1U, 16U); /* Set root clock to 24.576MHZ */

    /* Set the PDM clock source in the audiomix */
    AUDIOMIX_AttachClk(AUDIOMIX, kAUDIOMIX_Attach_PDM_Root_to_CCM_PDM);
}
/*${function:end}*/
