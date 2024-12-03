/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_trgmux.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Set the source for the ADC0 module */
    CLOCK_SetIpSrc(kCLOCK_Adc0, kCLOCK_IpSrcFircAsync);
    /* Set the source for the LPIT module */
    CLOCK_SetIpSrc(kCLOCK_Lpit0, kCLOCK_IpSrcFircAsync);
    /* LPIT Channel 0 trigger input source: FTM0 */
    TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_Lpit, kTRGMUX_TriggerInput0, kTRGMUX_SourceFtm0);
    /* ADC0 Channel A trigger input source: LPIT0CH0  */
    TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_Adc0, kTRGMUX_TriggerInput0, kTRGMUX_SourceLpit0Ch0);
    /* Select TRGMUX output as ADC0 trigger source and pre-trigger source  */
    SIM->ADCOPT |= SIM_ADCOPT_ADC0PRETRGSEL(1);
    SIM->ADCOPT |= SIM_ADCOPT_ADC0TRGSEL(1);

    /* Due to FTM output trigger period width is 1 ~ 3 system clock cycles and LPIT trigger detected logic use its
     * function clock, we must make LPIT function large enough, otherwise LPIT may be miss FTM trigger output */
    assert(SystemCoreClock <= LPIT_SOURCECLOCK);
}
/*${function:end}*/
