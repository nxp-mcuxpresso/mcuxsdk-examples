/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_rdc.h"
#include "fsl_gpio.h"
#include "app.h"
#include "app_srtm.h"
/*${header:end}*/

/*${function:start}*/
void Peripheral_RdcSetting(void)
{
    rdc_domain_assignment_t assignment = {0};
    rdc_periph_access_config_t periphConfig;

    assignment.domainId = BOARD_DOMAIN_ID;

    /* Only configure the RDC if the RDC peripheral write access is allowed. */
    if ((0x1U & RDC_GetPeriphAccessPolicy(RDC, kRDC_Periph_RDC, assignment.domainId)) != 0U)
    {
        RDC_SetMasterDomainAssignment(RDC, kRDC_Master_SDMA3_PERIPH, &assignment);
        RDC_SetMasterDomainAssignment(RDC, kRDC_Master_SDMA3_BURST, &assignment);
        RDC_SetMasterDomainAssignment(RDC, kRDC_Master_SDMA3_SPBA2, &assignment);

        RDC_GetDefaultPeriphAccessConfig(&periphConfig);
        /* Do not allow the A53 domain(domain0) to access the following peripherals. */
        periphConfig.policy = RDC_DISABLE_A53_ACCESS;
        periphConfig.periph = kRDC_Periph_SAI1;
        RDC_SetPeriphAccessConfig(RDC, &periphConfig);
        periphConfig.periph = kRDC_Periph_UART4;
        RDC_SetPeriphAccessConfig(RDC, &periphConfig);
        periphConfig.periph = kRDC_Periph_I2C3;
        RDC_SetPeriphAccessConfig(RDC, &periphConfig);
        periphConfig.periph = kRDC_Periph_GPT1;
        RDC_SetPeriphAccessConfig(RDC, &periphConfig);
#if APP_SRTM_PDM_USED
        periphConfig.periph = kRDC_Periph_MICFIL;
        RDC_SetPeriphAccessConfig(RDC, &periphConfig);
#endif
        periphConfig.periph = kRDC_Periph_SDMA3;
        RDC_SetPeriphAccessConfig(RDC, &periphConfig);
        /* For SAI3, both kRDC_Periph_SAI3_ACCESS and kRDC_Periph_SAI3_LPM registers need set.*/
        periphConfig.periph = kRDC_Periph_SAI3_ACCESS;
        RDC_SetPeriphAccessConfig(RDC, &periphConfig);
        periphConfig.periph = kRDC_Periph_SAI3_LPM;
        RDC_SetPeriphAccessConfig(RDC, &periphConfig);
        /* Remove the SAI2 power control in RDC from M4 to avoid A53 hang when it touches SAI2 under M core enters STOP
         * mode.*/
        periphConfig.policy = RDC_DISABLE_M4_ACCESS;
        periphConfig.periph = kRDC_Periph_SAI2_LPM;
        RDC_SetPeriphAccessConfig(RDC, &periphConfig);
    }
}
void BOARD_InitHardware(void)
{
    uint32_t i = 0;

    BOARD_RdcInit();
    Peripheral_RdcSetting();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();
    /*
     * In order to wakeup M4 from LPM, all PLLCTRLs need to be set to "NeededRun"
     */
    for (i = 0; i != 39; i++)
    {
        CCM->PLL_CTRL[i].PLL_CTRL = kCLOCK_ClockNeededRun;
    }
    CLOCK_SetRootMux(kCLOCK_RootI2c3, kCLOCK_I2cRootmuxSysPll1Div5); /* Set I2C source to SysPLL1 Div5 160MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootI2c3, 1U, 10U);                  /* Set root clock to 160MHZ / 10 = 16MHZ */
    CLOCK_SetRootMux(kCLOCK_RootGpt1, kCLOCK_GptRootmuxOsc24M);      /* Set GPT source to Osc24 MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootGpt1, 1U, 1U);
}
/*${function:end}*/
