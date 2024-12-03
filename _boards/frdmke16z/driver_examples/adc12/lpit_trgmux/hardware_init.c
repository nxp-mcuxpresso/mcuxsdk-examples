/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_trgmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Set ADC12's clock source to be Slow IRC async clock. */
    CLOCK_SetIpSrc(kCLOCK_Adc0, kCLOCK_IpSrcSircAsync);
    /* Set the source for the LPIT module */
    CLOCK_SetIpSrc(kCLOCK_Lpit0, kCLOCK_IpSrcSircAsync);
}

void BOARD_ConfigTriggerSource(void)
{
    TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_Adc0, kTRGMUX_TriggerInput0, kTRGMUX_SourceLpit0Ch0);

    /* Configure SIM for ADC hardware trigger source selection */
    /* Use TRGMUX output as ADC pre-trigger trigger source */
    SIM->ADCOPT |= SIM_ADCOPT_ADC0TRGSEL(1U);
    SIM->ADCOPT |= SIM_ADCOPT_ADC0PRETRGSEL(1U);
}
/*${function:end}*/
