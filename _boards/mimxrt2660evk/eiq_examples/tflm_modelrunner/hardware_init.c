/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "fsl_power.h"
#include "pin_mux.h"
#include "app.h"
#include "timer.h"
#if defined(MODELRUNNER_HTTP) && MODELRUNNER_HTTP
#include "fsl_enet.h"
#include "fsl_trdc_soc.h"
#include "fsl_modcon.h"
#include "fsl_adapter_uart.h"
#include "fsl_pcal6524.h"
#endif
/*${header:end}*/

/*${variable:start}*/
#if defined(MODELRUNNER_HTTP) && MODELRUNNER_HTTP
phy_yt8521_resource_t g_phy_resource;
static pcal6524_handle_t s_pcal6524Handle;
#endif
/*${variable:end}*/

/*${function:start}*/

#if defined(MODELRUNNER_HTTP) && MODELRUNNER_HTTP
/* ENET flexible configuration callback required by the lwIP ENET ethernetif. */
void BOARD_ENETFlexibleConfigure(enet_config_t *config)
{
    config->miiMode = kENET_RgmiiMode;
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET, phyAddr, regAddr, pData);
}
#endif /* MODELRUNNER_HTTP */

/* Return microsecond timestamp; used by modelrunner for per-layer timing. */
int64_t os_clock_now(void)
{
    return TIMER_GetTimeInUS();
}

/*
 * Clean and invalidate a D-cache range by address.
 * Called by the Neutron NPU driver before/after DMA transfers.
 */
void cleanCache_by_Addr(uint32_t addr, uint32_t size)
{
    if (SCB->CCR & SCB_CCR_DC_Msk)
    {
        SCB_CleanDCache_by_Addr((uint32_t *)addr, size);
    }
    /* Memory barriers to ensure the cache operation completes. */
    __DSB();
    __ISB();
}

void BOARD_InitHardware(void)
{
    /* ------------------------------------------------------------------
     * Board common setup: MPU, power domains, clock tree, TRDC, and
     * debug UART console.  All of this is shared with other RT2660 examples.
     * ------------------------------------------------------------------ */
    BOARD_CommonSetting();

    /* ------------------------------------------------------------------
     * NPU: grant TRDC domain 0 to the Neutron master, enable power domain,
     * clocks, and the NPU MPU region used by the driver.
     * ------------------------------------------------------------------ */
    CMPT__TRDC->MDA_DFMT1[kTRDC_CMPT_MasterNPU].MDA_W_DFMT1[0] =
        TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;

    POWER_SetDomainRunMode(kPOWER_DomainNpu, kPDCON_EventNoneOrActive);
    CLOCK_EnableClock(kCLOCK_CMPT_npu_core);
    CLOCK_EnableClock(kCLOCK_CMPT_npu_mem);

    /* MPU region 10: NPU private SRAM window (0x20900000 .. 0x20900FFF). */
    ARM_MPU_SetRegion(10U,
                      ARM_MPU_RBAR(0x20900000, ARM_MPU_SH_NON, 0U, 0U, 0U),
                      ARM_MPU_RLAR(0x20900FFF, 0U));

    /* ------------------------------------------------------------------
     * PSRAM: mapped by the boot configuration (MPU windows are set up in
     * BOARD_CommonSetting) and used as plain memory -- no example-side
     * controller init, same as tflm_cifar10 on this board.
     * ------------------------------------------------------------------ */

    /* ------------------------------------------------------------------
     * SysTick-based microsecond timer used by os_clock_now().
     * ------------------------------------------------------------------ */
    TIMER_Init();

#if defined(MODELRUNNER_HTTP) && MODELRUNNER_HTTP
    /* ------------------------------------------------------------------
     * ENET / PHY initialization (Phase 2 -- HTTP mode only).
     * Mirrors _boards/mimxrt2660evk/lwip_examples/common/enet/hardware_init.c
     * ------------------------------------------------------------------ */

    /* Set UART ISR priority before enabling ENET interrupts. */
    NVIC_SetPriority(HSP_LPUART0_IRQn, HAL_UART_ISR_PRIORITY);

    /* ENET pin mux and PCAL6524 I2C expander (used for PHY reset). */
    BOARD_InitENETPins();
    BOARD_Init6524Pins();
    BOARD_InitPCAL6524(&s_pcal6524Handle);

    /* Refresh SystemCoreClock so SysTick-based timing is correct. */
    SystemCoreClockUpdate();

    /* Re-divide ETH1_TRXCLK to 125 MHz (RGMII RX delay-line reference). */
    {
        clock_root_config_t trxCfg = {.mux = kCLOCK_ETH1_TRXCLK_ClockRoot_MAINPLLDIV8,
                                      .div = 2U, .sndDiv = 1U};
        CLOCK_SetRootClock(kCLOCK_Root_COMM_eth1_trxclk, &trxCfg);
    }

    /* Grant ENET1 DMA masters TRDC domain 0. */
    COMM__TRDC->MDA_DFMT1[kTRDC_COMM_MasterENET1_M0R].MDA_W_DFMT1[0] =
        TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    COMM__TRDC->MDA_DFMT1[kTRDC_COMM_MasterENET1_M0T].MDA_W_DFMT1[0] =
        TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;

    /* Reset YT8531 PHY via PCAL6524 P2_2 (BOARD_PCAL6524_ETH1_RST_B). */
    {
        uint32_t rstMask = 1U << BOARD_PCAL6524_ETH1_RST_B;
        (void)PCAL6524_SetDirection(&s_pcal6524Handle, rstMask, kPCAL6524_Output);
        (void)PCAL6524_ClearPins(&s_pcal6524Handle, rstMask);
        SDK_DelayAtLeastUs(10000U, CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk));
        (void)PCAL6524_SetPins(&s_pcal6524Handle, rstMask);
        SDK_DelayAtLeastUs(30000U, CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk));
    }

    /* Initialise MDIO (SMI) using the ENET1G MAC. */
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET)]);
    ENET_SetSMI(EXAMPLE_ENET, EXAMPLE_CLOCK_FREQ, false);
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    /* Enable RGMII on ENET1G (MODCON CFG bit). */
    MODCON_SetCFG((uint32_t)kModCon_COMM_ETH1, 0U, MODCON_CFG_ENET1G_RGMII_EN(1U));
#endif /* MODELRUNNER_HTTP */
}

/*${function:end}*/
