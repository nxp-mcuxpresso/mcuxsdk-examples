/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_clock.h"
#include "board.h"
#include "fsl_flexspi.h"
#include "fsl_cache.h"
#include "fsl_io_mux.h"
#include "fsl_power.h"
#include "fsl_ocotp.h"
#include "mcuxClEls.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_FLEXSPI_DLL_LOCK_RETRY (10)

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq = 0;

    /* attach FRG0 clock to FLEXCOMM3 (debug console) */
    CLOCK_SetFRGClock(BOARD_DEBUG_UART_FRG_CLK);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

void BOARD_InitSleepPinConfig(void)
{
    int32_t i;

    /* Set all non-AON pins output low level in sleep mode. */
    for (i = 0; i < 22; i++)
    {
        IO_MUX_SetPinOutLevelInSleep(i, IO_MUX_SleepPinLevelLow);
    }
    for (i = 28; i < 64; i++)
    {
        IO_MUX_SetPinOutLevelInSleep(i, IO_MUX_SleepPinLevelLow);
    }

    /* Set RF_CNTL 0-3 output low level in sleep mode. */
    for (i = 0; i < 4; i++)
    {
        IO_MUX_SetRfPinOutLevelInSleep(i, IO_MUX_SleepPinLevelLow);
    }
}

void BOARD_DeinitFlash(FLEXSPI_Type *base)
{
    /* Enable FLEXSPI clock again */
    CLKCTL0->PSCCTL0_SET = CLKCTL0_PSCCTL0_SET_FLEXSPI0_MASK;

    /* Enable FLEXSPI module */
    base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;

    /* Wait until FLEXSPI is not busy */
    while (!((base->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (base->STS0 & FLEXSPI_STS0_SEQIDLE_MASK)))
    {
    }
    /* Disable module during the reset procedure */
    base->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;
}

void BOARD_InitFlash(FLEXSPI_Type *base)
{
    uint32_t status;
    uint32_t lastStatus;
    uint32_t retry;

    /* Loopback from DQS pad can maximize RD board flash speed. */
    if ((base->MCR0 & FLEXSPI_MCR0_RXCLKSRC_MASK) != FLEXSPI_MCR0_RXCLKSRC(1))
    {
        base->MCR0 = (base->MCR0 & ~FLEXSPI_MCR0_RXCLKSRC_MASK) | FLEXSPI_MCR0_RXCLKSRC(1);
    }
    /* If serial root clock is >= 100 MHz, DLLEN set to 1, OVRDEN set to 0, then SLVDLYTARGET setting of 0x0 is
     * recommended. */
    base->DLLCR[0] = 0x1U;

    /* Enable FLEXSPI module */
    base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;

    base->MCR0 |= FLEXSPI_MCR0_SWRESET_MASK;
    while (base->MCR0 & FLEXSPI_MCR0_SWRESET_MASK)
    {
    }

    /* Need to wait DLL locked if DLL enabled */
    if (0U != (base->DLLCR[0] & FLEXSPI_DLLCR_DLLEN_MASK))
    {
        lastStatus = base->STS2;
        retry      = BOARD_FLEXSPI_DLL_LOCK_RETRY;
        /* Wait slave delay line locked and slave reference delay line locked. */
        do
        {
            status = base->STS2;
            if ((status & (FLEXSPI_STS2_AREFLOCK_MASK | FLEXSPI_STS2_ASLVLOCK_MASK)) ==
                (FLEXSPI_STS2_AREFLOCK_MASK | FLEXSPI_STS2_ASLVLOCK_MASK))
            {
                /* Locked */
                retry = 100;
                break;
            }
            else if (status == lastStatus)
            {
                /* Same delay cell number in calibration */
                retry--;
            }
            else
            {
                retry      = BOARD_FLEXSPI_DLL_LOCK_RETRY;
                lastStatus = status;
            }
        } while (retry > 0);
        /* According to ERR011377, need to delay at least 100 NOPs to ensure the DLL is locked. */
        for (; retry > 0U; retry--)
        {
            __NOP();
        }
    }
}

/* BOARD_SetFlexspiClock run in RAM used to configure FlexSPI clock source and divider when XIP. */
void BOARD_SetFlexspiClock(FLEXSPI_Type *base, uint32_t src, uint32_t divider)
{
    if ((CLKCTL0->FLEXSPIFCLKSEL != CLKCTL0_FLEXSPIFCLKSEL_SEL(src)) ||
        ((CLKCTL0->FLEXSPIFCLKDIV & CLKCTL0_FLEXSPIFCLKDIV_DIV_MASK) != (divider - 1)))
    {
        /* Always deinit FLEXSPI and init FLEXSPI for the flash to make sure the flash works correctly after the
         FLEXSPI root clock changed as the default FLEXSPI configuration may does not work for the new root clock
         frequency. */
        BOARD_DeinitFlash(base);

        /* Disable clock before changing clock source */
        CLKCTL0->PSCCTL0_CLR = CLKCTL0_PSCCTL0_CLR_FLEXSPI0_MASK;
        /* Update flexspi clock. */
        CLKCTL0->FLEXSPIFCLKSEL = CLKCTL0_FLEXSPIFCLKSEL_SEL(src);
        CLKCTL0->FLEXSPIFCLKDIV |= CLKCTL0_FLEXSPIFCLKDIV_RESET_MASK; /* Reset the divider counter */
        CLKCTL0->FLEXSPIFCLKDIV = CLKCTL0_FLEXSPIFCLKDIV_DIV(divider - 1);
        while ((CLKCTL0->FLEXSPIFCLKDIV) & CLKCTL0_FLEXSPIFCLKDIV_REQFLAG_MASK)
        {
        }
        /* Enable FLEXSPI clock again */
        CLKCTL0->PSCCTL0_SET = CLKCTL0_PSCCTL0_SET_FLEXSPI0_MASK;

        BOARD_InitFlash(base);
    }
}

static bool LoadGdetCfg(power_gdet_data_t *data)
{
    bool retval = true;

    /* If T3 256M clock is disabled, GDET cannot work. */
    if ((SYSCTL2->SOURCE_CLK_GATE & SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_256M_CG_MASK) != 0U)
    {
        retval = false;
    }
    else
    {
        /* GDET clock set to boot value */
        CLKCTL0->ELS_GDET_CLK_SEL = data->TRIM0;
    }

    if (retval)
    {
        /* LOAD command */
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_GlitchDetector_LoadConfig_Async((uint8_t *)data));
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_GlitchDetector_LoadConfig_Async) != token) ||
            (MCUXCLELS_STATUS_OK_WAIT != result))
        {
            retval = false;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();
    }

    if (retval)
    {
        /* Wait for the mcuxClEls_GlitchDetector_LoadConfig_Async operation to complete. */
        MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
        if ((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
        {
            retval = false;
        }
        MCUX_CSSL_FP_FUNCTION_CALL_END();
    }

    return retval;
}

static void ConfigSvcSensor(void)
{
    uint64_t svc;
    uint32_t pack;
    status_t status;
    power_gdet_data_t gdetData = {0U};
    uint32_t rev = SOCCTRL->CHIP_INFO & SOCCIU_CHIP_INFO_REV_NUM_MASK;

    status = OCOTP_ReadSVC(&svc);
    if (status == kStatus_Success)
    { /* CES */
        status = OCOTP_ReadPackage(&pack);
        if (status == kStatus_Success)
        {
            /*
               A2 CES: Use SVC voltage.
               A1 CES: Keep boot voltage 1.11V.
             */
            POWER_InitVoltage((rev == 2U) ? ((uint32_t)svc >> 16) : 0U, pack);
        }

        /* SVC GDET config */
        status = (status == kStatus_Success) ? OCOTP_OtpFuseRead(149, &gdetData.CFG[0]) : status;
        status = (status == kStatus_Success) ? OCOTP_OtpFuseRead(150, &gdetData.CFG[1]) : status;
        status = (status == kStatus_Success) ? OCOTP_OtpFuseRead(151, &gdetData.CFG[2]) : status;
        /* A2 CES load fuse 155 for trim calculation. A1 CES directly use the default trim value in fuse 152. */
        status = (status == kStatus_Success) ? OCOTP_OtpFuseRead((rev == 2U) ? 155 : 152, &gdetData.CFG[3]) : status;
        status = (status == kStatus_Success) ? OCOTP_OtpFuseRead(153, &gdetData.CFG[4]) : status;
        status = (status == kStatus_Success) ? OCOTP_OtpFuseRead(154, &gdetData.CFG[5]) : status;
        assert(status == kStatus_Success);

        /* Must configure GDET load function for POWER_EnableGDetVSensors(). */
        Power_InitLoadGdetCfg(LoadGdetCfg, &gdetData, pack);
    }
    else
    {
        /* A1/A2 non-CES */
        SystemCoreClockUpdate();

        /* LPBG trim */
        BUCK11->BUCK_CTRL_EIGHTEEN_REG = 0x6U;
        /* Change buck level */
        PMU->PMIP_BUCK_LVL = PMU_PMIP_BUCK_LVL_SLEEP_BUCK18_SEL(0x60U) |  /* 1.8V */
                             PMU_PMIP_BUCK_LVL_SLEEP_BUCK11_SEL(0x22U) |  /* 0.8V */
                             PMU_PMIP_BUCK_LVL_NORMAL_BUCK18_SEL(0x60U) | /* 1.8V */
                             PMU_PMIP_BUCK_LVL_NORMAL_BUCK11_SEL(0x54U);  /* 1.05V */
        /* Delay 600us */
        SDK_DelayAtLeastUs(600, SystemCoreClock);
    }
}

/* This function is used to configure static voltage compansation and sensors, and in XIP case, change FlexSPI clock
   to a stable source before clock tree(Such as PLL and Main clock) update */
void BOARD_ClockPreConfig(void)
{
    OCOTP_OtpInit();
    ConfigSvcSensor();
    OCOTP_OtpDeinit();

    if (BOARD_IS_XIP())
    {
        /* Move FLEXSPI clock source to T3 256m / 4 to avoid instruction/data fetch issue in XIP when
         * updating PLL and main clock.
         */
        BOARD_SetFlexspiClock(FLEXSPI, 6U, 4U);
    }
    else
    {
        RESET_ClearPeripheralReset(kFLEXSPI_RST_SHIFT_RSTn);
        BOARD_DeinitFlash(FLEXSPI);
        CLOCK_AttachClk(kNONE_to_FLEXSPI_CLK);
        CLOCK_DisableClock(kCLOCK_Flexspi);
        RESET_SetPeripheralReset(kFLEXSPI_RST_SHIFT_RSTn);
    }
}

/* Update FlexSPI clock source and set flash to full speed */
void BOARD_ClockPostConfig(void)
{
    if (BOARD_IS_XIP())
    {
        /* Call function BOARD_SetFlexspiClock() to set clock source to aux0_pll_clk. */
        BOARD_SetFlexspiClock(FLEXSPI, 2U, 2U);
    }
    else
    {
    }
}

/* RW61x is found some chips could not pull up some GPIO pins which are
 * MUX with GAC ACOMP if GPIO power domain is 1.8V.
 * The workaround is enable GAC MUX before using GPIOs (GPIO42-50, GPIO55 and GPIO58-61)
 * input function
 */
void BOARD_ApplyGpioPullUpWorkaround(void)
{
    RSTCTL0->PRSTCTL1 &= ~RSTCTL0_PRSTCTL1_GAU_MASK; /* Remove reset GAU. */
    CLKCTL0->PSCCTL1 |= CLKCTL0_PSCCTL1_GAU_MASK;    /* Enable GAU clock. */
    GAU_ACOMP->CTRL0 |= ACOMP_CTRL0_EN_MASK;         /* Enable ACOMP0.    */
    CLKCTL0->PSCCTL1 &= ~CLKCTL0_PSCCTL1_GAU_MASK;   /* Disable GAU clock. */
}
