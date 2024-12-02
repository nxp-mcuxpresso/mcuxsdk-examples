/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "clock_config.h"
#include "fsl_semc.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_nand_flash.h"
#include "fsl_semc_nand_flash.h"
/*${header:end}*/

/*${function:start}*/
// semc_nand_timing_config_t timingConfig;

void delayUs(uint32_t delay_us)
{
    uint32_t s_tickPerMicrosecond = CLOCK_GetFreq(kCLOCK_CpuClk) / 1000000;

    // Make sure this value is greater than 0
    if (!s_tickPerMicrosecond)
    {
        s_tickPerMicrosecond = 1;
    }
    delay_us = delay_us * s_tickPerMicrosecond;
    while (delay_us)
    {
        __NOP();
        delay_us--;
    }
}

semc_nand_timing_config_t timing = {
    .tCeSetup_Ns        = 1,
    .tCeHold_Ns         = 1,
    .tCeInterval_Ns     = 1,
    .tWeLow_Ns          = 8,
    .tWeHigh_Ns         = 8,
    .tReLow_Ns          = 21,
    .tReHigh_Ns         = 1,
    .tTurnAround_Ns     = 1,
    .tWehigh2Relow_Ns   = 105,
    .tRehigh2Welow_Ns   = 105,
    .tAle2WriteStart_Ns = 75,
    .tReady2Relow_Ns    = 25,
    .tWehigh2Busy_Ns    = 105,
};

semc_nand_config_t semcNandConfig = {
    .cePinMux          = kSEMC_MUXCSX2,                       /*!< The CE# pin mux setting. */
    .axiAddress        = EXAMPLE_SEMC_NAND_AXI_START_ADDRESS, /*!< The base address. */
    .axiMemsize_kbytes = 16 * 1024,                           /*!< The memory size is 8*1024*2*1024*1024 = 16Gb. */
    .ipgAddress        = EXAMPLE_SEMC_NAND_IPG_START_ADDRESS, /*!< The base address. */
    .ipgMemsize_kbytes = 2 * 1024 * 1024,                     /*!< The memory size is 8*1024*2*1024*1024 = 16Gb. */
    .rdyactivePolarity = kSEMC_RdyActiveLow,                  /*!< Wait ready polarity. */
    .arrayAddrOption   = kSEMC_NandAddrOption_5byte_CA2RA3,
    .edoModeEnabled    = false,                               /*!< Address mode. */
    .columnAddrBitNum  = kSEMC_NandColum_13bit,
    .burstLen          = kSEMC_Nand_BurstLen64,               /*!< Burst length. */
    .portSize          = kSEMC_PortSize8Bit,                  /*!< Port size. */
    .timingConfig      = &timing,
};

semc_mem_nand_config_t semcMemConfig = {
    .semcNandConfig   = &semcNandConfig,
    .delayUS          = delayUs,
    .onfiVersion      = kNandOnfiVersion_1p0,
    .readyCheckOption = kNandReadyCheckOption_RB,
    .eccCheckType     = kNandEccCheckType_SoftwareECC,
};

nand_config_t nandConfig = {
    .memControlConfig = (void *)&semcMemConfig,
    .driverBaseAddr   = (void *)EXAMPLE_SEMC,
};

void BOARD_InitMem(void)
{
    semc_config_t config;

    /* Initializes the MAC configure structure to zero. */
    memset(&config, 0, sizeof(semc_config_t));

    /* Initialize SEMC. */
    SEMC_GetDefaultConfig(&config);
    SEMC_Init(SEMC, &config);
    /* Set SEMC clk source for NAND flash memory controller usage. */
    semcMemConfig.clkSrc_Hz = EXAMPLE_SEMC_CLK_FREQ;
}

void BOARD_ReConfigMPU(void)
{
    /* Disable I cache and D cache */
    SCB_DisableICache();
    SCB_DisableDCache();

    /* Disable MPU */
    ARM_MPU_Disable();

    /* Region 0 setting */
    MPU->RBAR = ARM_MPU_RBAR(0, 0xC0000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 1 setting */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

/* Region 2 setting */
#if defined(XIP_EXTERNAL_FLASH)
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512MB);
#else
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);
#endif

    /* Region 3 setting */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 4 setting */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

    /* Region 5 setting */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

    /* Region 6 setting */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

#if defined(SDRAM_MPU_INIT)
    /* Region 7 setting */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);

    /* Region 8 setting */
    MPU->RBAR = ARM_MPU_RBAR(8, 0x81E00000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2MB);
#endif

    /* Region 9 setting */
    MPU->RBAR = ARM_MPU_RBAR(9, EXAMPLE_SEMC_NAND_AXI_START_ADDRESS);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2GB);

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */
    SCB_EnableDCache();
    SCB_EnableICache();
}

void BOARD_InitHardware(void)
{
    BOARD_ReConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    /* Set semc clock to 166Mhz */
    CLOCK_InitSysPfd(kCLOCK_Pfd2, 29);
    CLOCK_SetMux(kCLOCK_SemcMux, 1);
    CLOCK_SetDiv(kCLOCK_SemcDiv, 1);
    BOARD_InitMem();
    BOARD_InitDebugConsole();

    /*ENABLE THE SEMC AXI ADDRESS ACCESS*/
    CCM->CCGR1 |= 3U << 18U;
}
/*${function:end}*/
