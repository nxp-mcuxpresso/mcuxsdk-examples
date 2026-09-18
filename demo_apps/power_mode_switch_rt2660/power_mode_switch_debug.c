/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "app.h"
#include "power_mode_switch.h"

#if APP_PRINT_DEBUG_INFO

/*******************************************************************************
 * FREQMEAS debug dump - clock root / clock source name tables
 ******************************************************************************/

/*!
 * @brief One clock root / clock source entry (enum value + display name) for the
 * measured-frequency dump.  The display name is the enum identifier minus its
 * "kCLOCK_Root_" / "kCLOCK_SRC_" prefix.
 */
typedef struct
{
    uint32_t    id;   /*!< clock_root_t (roots table) or clock_name_t (sources table). */
    const char *name; /*!< Display name. */
} app_clk_meas_t;

/*! Every clock_root_t across all subsystems (CGU/CMPT/MAIN/WAKE/COMM/AUDIO/MEDIA). */
static const app_clk_meas_t APP_MEAS_ROOTS[] = {
    { kCLOCK_Root_CGU_SXOSC_ROOTCLK, "CGU_SXOSC_ROOTCLK" },
    { kCLOCK_Root_CGU_BASE_CLK, "CGU_BASE_CLK" },
    { kCLOCK_Root_CGU_LOW_CLK, "CGU_LOW_CLK" },
    { kCLOCK_Root_CGU_MAINPLL_DIVX, "CGU_MAINPLL_DIVX" },
    { kCLOCK_Root_CGU_SYSPLL_DIVX, "CGU_SYSPLL_DIVX" },
    { kCLOCK_Root_CGU_PLL_PFDX, "CGU_PLL_PFDX" },
    { kCLOCK_Root_CGU_MEDIA_PFDX, "CGU_MEDIA_PFDX" },
    { kCLOCK_Root_CGU_MAINPFDX_ROOTCLK, "CGU_MAINPFDX_ROOTCLK" },
    { kCLOCK_Root_CGU_COMMPFDX_ROOTCLK, "CGU_COMMPFDX_ROOTCLK" },
    { kCLOCK_Root_CGU_MAINDIVX_ROOTCLK, "CGU_MAINDIVX_ROOTCLK" },
    { kCLOCK_Root_CGU_SAIMCLK_ROOTCLK, "CGU_SAIMCLK_ROOTCLK" },
    { kCLOCK_Root_CGU_SAIMCLK0_ROOTCLK, "CGU_SAIMCLK0_ROOTCLK" },
    { kCLOCK_Root_CGU_SAIMCLK1_ROOTCLK, "CGU_SAIMCLK1_ROOTCLK" },
    { kCLOCK_Root_CGU_SAIMCLK2_ROOTCLK, "CGU_SAIMCLK2_ROOTCLK" },
    { kCLOCK_Root_CGU_LP12M_CORE_ROOTCLK, "CGU_LP12M_CORE_ROOTCLK" },
    { kCLOCK_Root_CGU_LP1M_CORE_ROOTCLK, "CGU_LP1M_CORE_ROOTCLK" },
    { kCLOCK_Root_CGU_ULP32K_ROOTCLK, "CGU_ULP32K_ROOTCLK" },
    { kCLOCK_Root_CGU_FRO192M_ROOTCLK, "CGU_FRO192M_ROOTCLK" },
    { kCLOCK_Root_CGU_FRO96M_ROOTCLK, "CGU_FRO96M_ROOTCLK" },
    { kCLOCK_Root_CGU_FRO48M_ROOTCLK, "CGU_FRO48M_ROOTCLK" },
    { kCLOCK_Root_CGU_FRO24M_ROOTCLK, "CGU_FRO24M_ROOTCLK" },
    { kCLOCK_Root_CGU_SYSPLLDIV4_ROOTCLK, "CGU_SYSPLLDIV4_ROOTCLK" },
    { kCLOCK_Root_CGU_SYSPLLDIV5_ROOTCLK, "CGU_SYSPLLDIV5_ROOTCLK" },
    { kCLOCK_Root_CGU_SYSPLLDIVX_ROOTCLK, "CGU_SYSPLLDIVX_ROOTCLK" },
    { kCLOCK_Root_CGU_MAINPLLDIVX_ROOTCLK, "CGU_MAINPLLDIVX_ROOTCLK" },
    { kCLOCK_Root_CGU_MAINPLLDIV8_ROOTCLK, "CGU_MAINPLLDIV8_ROOTCLK" },
    { kCLOCK_Root_CGU_MAINPLLDIV10_ROOTCLK, "CGU_MAINPLLDIV10_ROOTCLK" },
    { kCLOCK_Root_CGU_MAINPLLDIV20_ROOTCLK, "CGU_MAINPLLDIV20_ROOTCLK" },
    { kCLOCK_Root_CGU_AUDIOPLL_ROOTCLK, "CGU_AUDIOPLL_ROOTCLK" },
    { kCLOCK_Root_CGU_VIDEOPLL_ROOTCLK, "CGU_VIDEOPLL_ROOTCLK" },
    { kCLOCK_Root_CGU_MAIN_ROOTCLK, "CGU_MAIN_ROOTCLK" },
    { kCLOCK_Root_CGU_NPU_ROOTCLK, "CGU_NPU_ROOTCLK" },
    { kCLOCK_Root_CGU_MEDIABUS_ROOTCLK, "CGU_MEDIABUS_ROOTCLK" },
    { kCLOCK_Root_CGU_AUDIOBUS_ROOTCLK, "CGU_AUDIOBUS_ROOTCLK" },
    { kCLOCK_Root_CGU_COMMBUS_ROOTCLK, "CGU_COMMBUS_ROOTCLK" },
    { kCLOCK_Root_CGU_WAKEBUS_ROOTCLK, "CGU_WAKEBUS_ROOTCLK" },
    { kCLOCK_Root_CGU_SYSCON_PDMAIN_CLK, "CGU_SYSCON_PDMAIN_CLK" },
    { kCLOCK_Root_CGU_PERI_ROOTCLK0, "CGU_PERI_ROOTCLK0" },
    { kCLOCK_Root_CGU_PERI_ROOTCLK1, "CGU_PERI_ROOTCLK1" },
    { kCLOCK_Root_CGU_PERI_ROOTCLK2, "CGU_PERI_ROOTCLK2" },
    { kCLOCK_Root_CGU_PERI_ROOTCLK3, "CGU_PERI_ROOTCLK3" },
    { kCLOCK_Root_CGU_PERI_ROOTCLK4, "CGU_PERI_ROOTCLK4" },
    { kCLOCK_Root_CGU_PERI_ROOTCLK5, "CGU_PERI_ROOTCLK5" },
    { kCLOCK_Root_CGU_PERI_ROOTCLK6, "CGU_PERI_ROOTCLK6" },
    { kCLOCK_Root_CGU_PERI_ROOTCLK7, "CGU_PERI_ROOTCLK7" },
    { kCLOCK_Root_CGU_AUDIO_ROOTCLK, "CGU_AUDIO_ROOTCLK" },
    { kCLOCK_Root_CGU_VIDEO_ROOTCLK, "CGU_VIDEO_ROOTCLK" },
    { kCLOCK_Root_CGU_USB1_ROOTCLK, "CGU_USB1_ROOTCLK" },
    { kCLOCK_Root_CGU_ETH_ROOTCLK, "CGU_ETH_ROOTCLK" },
    { kCLOCK_Root_CMPT_cmpt_clk, "CMPT_cmpt_clk" },
    { kCLOCK_Root_CMPT_cpu_clk, "CMPT_cpu_clk" },
    { kCLOCK_Root_CMPT_npu_clk, "CMPT_npu_clk" },
    { kCLOCK_Root_CMPT_systick_clk0, "CMPT_systick_clk0" },
    { kCLOCK_Root_CMPT_systick_clk1, "CMPT_systick_clk1" },
    { kCLOCK_Root_MAIN_main_clk_divided, "MAIN_main_clk_divided" },
    { kCLOCK_Root_MAIN_xspi0_fclk_divided, "MAIN_xspi0_fclk_divided" },
    { kCLOCK_Root_MAIN_xspi1_fclk_divided, "MAIN_xspi1_fclk_divided" },
    { kCLOCK_Root_MAIN_i3c0_fclk, "MAIN_i3c0_fclk" },
    { kCLOCK_Root_MAIN_lpi2c0_fclk, "MAIN_lpi2c0_fclk" },
    { kCLOCK_Root_MAIN_lpi2c1_fclk, "MAIN_lpi2c1_fclk" },
    { kCLOCK_Root_MAIN_lpspi0_fclk, "MAIN_lpspi0_fclk" },
    { kCLOCK_Root_MAIN_lpspi1_fclk, "MAIN_lpspi1_fclk" },
    { kCLOCK_Root_MAIN_lpspi2_fclk, "MAIN_lpspi2_fclk" },
    { kCLOCK_Root_MAIN_lpspi3_fclk, "MAIN_lpspi3_fclk" },
    { kCLOCK_Root_MAIN_lpspi4_fclk, "MAIN_lpspi4_fclk" },
    { kCLOCK_Root_MAIN_lpuart0_fclk, "MAIN_lpuart0_fclk" },
    { kCLOCK_Root_MAIN_lpuart1_fclk, "MAIN_lpuart1_fclk" },
    { kCLOCK_Root_MAIN_lpuart2_fclk, "MAIN_lpuart2_fclk" },
    { kCLOCK_Root_MAIN_lpuart3_fclk, "MAIN_lpuart3_fclk" },
    { kCLOCK_Root_MAIN_lpuart4_fclk, "MAIN_lpuart4_fclk" },
    { kCLOCK_Root_MAIN_lpuart5_fclk, "MAIN_lpuart5_fclk" },
    { kCLOCK_Root_MAIN_flexcan0_fclk, "MAIN_flexcan0_fclk" },
    { kCLOCK_Root_MAIN_flexcan1_fclk, "MAIN_flexcan1_fclk" },
    { kCLOCK_Root_MAIN_flexcan2_fclk, "MAIN_flexcan2_fclk" },
    { kCLOCK_Root_MAIN_flexcan_gfclk, "MAIN_flexcan_gfclk" },
    { kCLOCK_Root_MAIN_qtpm0_fclk, "MAIN_qtpm0_fclk" },
    { kCLOCK_Root_MAIN_lpit0_fclk, "MAIN_lpit0_fclk" },
    { kCLOCK_Root_MAIN_lpit1_fclk, "MAIN_lpit1_fclk" },
    { kCLOCK_Root_MAIN_adc0_fclk, "MAIN_adc0_fclk" },
    { kCLOCK_Root_MAIN_adc1_fclk, "MAIN_adc1_fclk" },
    { kCLOCK_Root_MAIN_sinc0_fclk, "MAIN_sinc0_fclk" },
    { kCLOCK_Root_MAIN_sinc1_fclk, "MAIN_sinc1_fclk" },
    { kCLOCK_Root_MAIN_flexio0_fclk, "MAIN_flexio0_fclk" },
    { kCLOCK_Root_MAIN_flexio1_fclk, "MAIN_flexio1_fclk" },
    { kCLOCK_Root_MAIN_flexio2_fclk, "MAIN_flexio2_fclk" },
    { kCLOCK_Root_MAIN_tpiu_clk, "MAIN_tpiu_clk" },
    { kCLOCK_Root_MAIN_cssi_refclk, "MAIN_cssi_refclk" },
    { kCLOCK_Root_MAIN_otp_clk, "MAIN_otp_clk" },
    { kCLOCK_Root_MAIN_ulp32k, "MAIN_ulp32k" },
    { kCLOCK_Root_WAKE_wake_clk, "WAKE_wake_clk" },
    { kCLOCK_Root_WAKE_wake_sxosc, "WAKE_wake_sxosc" },
    { kCLOCK_Root_WAKE_wake_lp1m, "WAKE_wake_lp1m" },
    { kCLOCK_Root_WAKE_wake_lp12m, "WAKE_wake_lp12m" },
    { kCLOCK_Root_WAKE_wake_ulp32k, "WAKE_wake_ulp32k" },
    { kCLOCK_Root_WAKE_wake_lpclk, "WAKE_wake_lpclk" },
    { kCLOCK_Root_WAKE_i3c0_fclk, "WAKE_i3c0_fclk" },
    { kCLOCK_Root_WAKE_lpi2c0_fclk, "WAKE_lpi2c0_fclk" },
    { kCLOCK_Root_WAKE_lpi2c1_fclk, "WAKE_lpi2c1_fclk" },
    { kCLOCK_Root_WAKE_lpspi0_fclk, "WAKE_lpspi0_fclk" },
    { kCLOCK_Root_WAKE_lpuart0_fclk, "WAKE_lpuart0_fclk" },
    { kCLOCK_Root_WAKE_lpuart1_fclk, "WAKE_lpuart1_fclk" },
    { kCLOCK_Root_WAKE_dmic1_appclk, "WAKE_dmic1_appclk" },
    { kCLOCK_Root_WAKE_qtpm0_fclk, "WAKE_qtpm0_fclk" },
    { kCLOCK_Root_WAKE_lptmr0_fclk, "WAKE_lptmr0_fclk" },
    { kCLOCK_Root_WAKE_lptmr1_fclk, "WAKE_lptmr1_fclk" },
    { kCLOCK_Root_WAKE_swt0_fclk, "WAKE_swt0_fclk" },
    { kCLOCK_Root_WAKE_swt1_fclk, "WAKE_swt1_fclk" },
    { kCLOCK_Root_WAKE_ewm_fclk, "WAKE_ewm_fclk" },
    { kCLOCK_Root_WAKE_acmp0_fclk, "WAKE_acmp0_fclk" },
    { kCLOCK_Root_WAKE_acmp1_fclk, "WAKE_acmp1_fclk" },
    { kCLOCK_Root_WAKE_acmp2_fclk, "WAKE_acmp2_fclk" },
    { kCLOCK_Root_WAKE_acmp3_fclk, "WAKE_acmp3_fclk" },
    { kCLOCK_Root_WAKE_acmp0_rrclk, "WAKE_acmp0_rrclk" },
    { kCLOCK_Root_WAKE_acmp1_rrclk, "WAKE_acmp1_rrclk" },
    { kCLOCK_Root_WAKE_acmp2_rrclk, "WAKE_acmp2_rrclk" },
    { kCLOCK_Root_WAKE_acmp3_rrclk, "WAKE_acmp3_rrclk" },
    { kCLOCK_Root_COMM_comm_clk, "COMM_comm_clk" },
    { kCLOCK_Root_COMM_comm_ulp32k, "COMM_comm_ulp32k" },
    { kCLOCK_Root_COMM_usdhc0_fclk, "COMM_usdhc0_fclk" },
    { kCLOCK_Root_COMM_usdhc1_fclk, "COMM_usdhc1_fclk" },
    { kCLOCK_Root_COMM_xspir_rootclk, "COMM_xspir_rootclk" },
    { kCLOCK_Root_COMM_usb0_phyclk, "COMM_usb0_phyclk" },
    { kCLOCK_Root_COMM_usb0_fro48m, "COMM_usb0_fro48m" },
    { kCLOCK_Root_COMM_usb1_fclk, "COMM_usb1_fclk" },
    { kCLOCK_Root_COMM_usb0_wakeclk, "COMM_usb0_wakeclk" },
    { kCLOCK_Root_COMM_eth0_trxclk, "COMM_eth0_trxclk" },
    { kCLOCK_Root_COMM_eth0_timerclk, "COMM_eth0_timerclk" },
    { kCLOCK_Root_COMM_eth1_trxclk, "COMM_eth1_trxclk" },
    { kCLOCK_Root_COMM_eth1_timerclk, "COMM_eth1_timerclk" },
    { kCLOCK_Root_COMM_eth_refclk, "COMM_eth_refclk" },
    { kCLOCK_Root_COMM_xeno0_liwclk, "COMM_xeno0_liwclk" },
    { kCLOCK_Root_COMM_xeno1_liwclk, "COMM_xeno1_liwclk" },
    { kCLOCK_Root_COMM_dll_refclk, "COMM_dll_refclk" },
    { kCLOCK_Root_AUDIO_audio_clk, "AUDIO_audio_clk" },
    { kCLOCK_Root_AUDIO_dmic0_appclk, "AUDIO_dmic0_appclk" },
    { kCLOCK_Root_AUDIO_sai0_mclk0, "AUDIO_sai0_mclk0" },
    { kCLOCK_Root_AUDIO_sai0_mclk1, "AUDIO_sai0_mclk1" },
    { kCLOCK_Root_AUDIO_sai1_mclk0, "AUDIO_sai1_mclk0" },
    { kCLOCK_Root_AUDIO_sai1_mclk1, "AUDIO_sai1_mclk1" },
    { kCLOCK_Root_AUDIO_sai2_mclk0, "AUDIO_sai2_mclk0" },
    { kCLOCK_Root_AUDIO_sai2_mclk1, "AUDIO_sai2_mclk1" },
    { kCLOCK_Root_AUDIO_spdif_txclk, "AUDIO_spdif_txclk" },
    { kCLOCK_Root_AUDIO_spdif_cdrclk, "AUDIO_spdif_cdrclk" },
    { kCLOCK_Root_AUDIO_asrc_clk, "AUDIO_asrc_clk" },
    { kCLOCK_Root_MEDIA_media_clk, "MEDIA_media_clk" },
    { kCLOCK_Root_MEDIA_mediapll_clk, "MEDIA_mediapll_clk" },
    { kCLOCK_Root_MEDIA_mipicsi_escclk, "MEDIA_mipicsi_escclk" },
    { kCLOCK_Root_MEDIA_mipicsi_clk, "MEDIA_mipicsi_clk" },
    { kCLOCK_Root_MEDIA_mipidsi_escclk_divided, "MEDIA_mipidsi_escclk_divided" },
    { kCLOCK_Root_MEDIA_mipidsi_refclk, "MEDIA_mipidsi_refclk" },
    { kCLOCK_Root_MEDIA_mipidsi_clk, "MEDIA_mipidsi_clk" },
    { kCLOCK_Root_MEDIA_reformat_fclk, "MEDIA_reformat_fclk" },
    { kCLOCK_Root_MEDIA_dcpixel_fclk, "MEDIA_dcpixel_fclk" },
    { kCLOCK_Root_MEDIA_csi_mclkout, "MEDIA_csi_mclkout" },
};

/*! Every clock_name_t clock source below kCLOCK_SRC_BOUNDARY. */
static const app_clk_meas_t APP_MEAS_SRCS[] = {
    { kCLOCK_SRC_BASE, "BASE" },
    { kCLOCK_SRC_LOW, "LOW" },
    { kCLOCK_SRC_MAINPLL_DIVX, "MAINPLL_DIVX" },
    { kCLOCK_SRC_SYSPLL_DIVX, "SYSPLL_DIVX" },
    { kCLOCK_SRC_PLL_PFDX, "PLL_PFDX" },
    { kCLOCK_SRC_MEDIA_PFDX, "MEDIA_PFDX" },
    { kCLOCK_SRC_MAINDIVX, "MAINDIVX" },
    { kCLOCK_SRC_SAIMCLK, "SAIMCLK" },
    { kCLOCK_SRC_SAIMCLK0, "SAIMCLK0" },
    { kCLOCK_SRC_SAIMCLK1, "SAIMCLK1" },
    { kCLOCK_SRC_SAIMCLK2, "SAIMCLK2" },
    { kCLOCK_SRC_ULP32K, "ULP32K" },
    { kCLOCK_SRC_FRO192M, "FRO192M" },
    { kCLOCK_SRC_FRO96M, "FRO96M" },
    { kCLOCK_SRC_FRO48M, "FRO48M" },
    { kCLOCK_SRC_FRO24M, "FRO24M" },
    { kCLOCK_SRC_SYSPLLDIV4, "SYSPLLDIV4" },
    { kCLOCK_SRC_SYSPLLDIV5, "SYSPLLDIV5" },
    { kCLOCK_SRC_MAINPLLDIV8, "MAINPLLDIV8" },
    { kCLOCK_SRC_MAINPLLDIV10, "MAINPLLDIV10" },
    { kCLOCK_SRC_AUDIOPLL, "AUDIOPLL" },
    { kCLOCK_SRC_VIDEOPLL, "VIDEOPLL" },
    { kCLOCK_SRC_CPU, "CPU" },
    { kCLOCK_SRC_NPU, "NPU" },
    { kCLOCK_SRC_MEDIABUS, "MEDIABUS" },
    { kCLOCK_SRC_AUDIOBUS, "AUDIOBUS" },
    { kCLOCK_SRC_COMMBUS, "COMMBUS" },
    { kCLOCK_SRC_WAKEBUS, "WAKEBUS" },
    { kCLOCK_SRC_PERI0, "PERI0" },
    { kCLOCK_SRC_PERI1, "PERI1" },
    { kCLOCK_SRC_PERI2, "PERI2" },
    { kCLOCK_SRC_PERI3, "PERI3" },
    { kCLOCK_SRC_PERI4, "PERI4" },
    { kCLOCK_SRC_PERI5, "PERI5" },
    { kCLOCK_SRC_PERI6, "PERI6" },
    { kCLOCK_SRC_PERI7, "PERI7" },
    { kCLOCK_SRC_AUDIO, "AUDIO" },
    { kCLOCK_SRC_VIDEO, "VIDEO" },
    { kCLOCK_SRC_USB1, "USB1" },
    { kCLOCK_SRC_ETH, "ETH" },
    { kCLOCK_SRC_MAIN, "MAIN" },
    { kCLOCK_SRC_WAKE_LPCLK, "WAKE_LPCLK" },
    { kCLOCK_SRC_MAIN_PERI0_DIV2, "MAIN_PERI0_DIV2" },
    { kCLOCK_SRC_MAIN_PERI1_DIV2, "MAIN_PERI1_DIV2" },
    { kCLOCK_SRC_MAINPFDX_DIV2, "MAINPFDX_DIV2" },
    { kCLOCK_SRC_COMM_PERI1_DIV2, "COMM_PERI1_DIV2" },
    { kCLOCK_SRC_COMM_PERI2_DIV2, "COMM_PERI2_DIV2" },
    { kCLOCK_SRC_COMMPFDX_DIV2, "COMMPFDX_DIV2" },
};

/*!
 * @brief Print the FREQMEAS-measured frequency (MHz) of every clock root and
 * every clock source using CLOCK_MeasureRootClockFreq()/CLOCK_MeasureClockSrcFreq().
 *
 * Each line shows the clock name and its measured frequency as MMMM.fff MHz -
 * the actual measured hardware value, not a computed one.  An entry that measures
 * 0 Hz (no FREQMEAS tap, or the clock is gated off) prints "(gated / no tap)".
 */
void APP_PrintMeasuredClocks(void)
{
    uint32_t i;
    uint32_t freq;

    PRINTF("\r\n---- Measured clock frequencies (FREQMEAS) --------------------\r\n");

    PRINTF("  Clock roots:\r\n");
    for (i = 0U; i < ARRAY_SIZE(APP_MEAS_ROOTS); i++)
    {
        freq = CLOCK_MeasureRootClockFreq((clock_root_t)APP_MEAS_ROOTS[i].id);
        if (freq != 0U)
        {
            PRINTF("    %-30s %4u.%03u MHz\r\n", APP_MEAS_ROOTS[i].name,
                   (uint32_t)(freq / 1000000U), (uint32_t)((freq % 1000000U) / 1000U));
        }
        else
        {
            PRINTF("    %-30s (gated / no tap)\r\n", APP_MEAS_ROOTS[i].name);
        }
    }

    PRINTF("  Clock sources:\r\n");
    for (i = 0U; i < ARRAY_SIZE(APP_MEAS_SRCS); i++)
    {
        freq = CLOCK_MeasureClockSrcFreq((clock_name_t)APP_MEAS_SRCS[i].id);
        if (freq != 0U)
        {
            PRINTF("    %-30s %4u.%03u MHz\r\n", APP_MEAS_SRCS[i].name,
                   (uint32_t)(freq / 1000000U), (uint32_t)((freq % 1000000U) / 1000U));
        }
        else
        {
            PRINTF("    %-30s (gated / no tap)\r\n", APP_MEAS_SRCS[i].name);
        }
    }
    PRINTF("--------------------------------------------------------------\r\n");
}

/*******************************************************************************
 * g_powerModeTable dump - value-kind classifier, display tables, printer
 ******************************************************************************/

/*!
 * @brief How to render a resource's cell value as a short human-readable
 * string in APP_PrintModeTable(), indexed by power_standby_resource_t.
 */
typedef enum
{
    kAPP_ValRaw,      /*!< Print cell->value as 0x%08X. */
    kAPP_ValOnOff,    /*!< bool: 0 -> "OFF", nonzero -> "ON". */
    kAPP_ValDomain,   /*!< pdcon_event_t: kPDCON_EventPowerOff -> "Off", else "Active". */
    kAPP_ValPmuMode,  /*!< PMU_HP/LP/RET/BB -> "HP"/"LP"/"RET"/"BB". */
    kAPP_ValDcdcMode, /*!< kPMU_DcdcModePWM/PFM -> "PWM"/"PFM". */
    kAPP_ValLdoMode,  /*!< kPMU_LdoModeHP/LP -> "HP"/"LP". */
    kAPP_ValMemMode,  /*!< kMEMCON_PowerMode{Active,Retention,PowerDown} -> matching name. */
} app_val_kind_t;

static const app_val_kind_t s_resourceValKind[kPOWER_Resource_COUNT] = {
    [kPOWER_Resource_CpuDomain]      = kAPP_ValDomain,
    [kPOWER_Resource_NpuDomain]      = kAPP_ValDomain,
    [kPOWER_Resource_CommDomain]     = kAPP_ValDomain,
    [kPOWER_Resource_MediaDomain]    = kAPP_ValDomain,
    [kPOWER_Resource_PmuMode]        = kAPP_ValPmuMode,
    [kPOWER_Resource_DcdcMode]       = kAPP_ValDcdcMode,
    [kPOWER_Resource_CoreLvl]        = kAPP_ValRaw,
    [kPOWER_Resource_Ldo0V8Mode]     = kAPP_ValLdoMode,
    [kPOWER_Resource_Ldo1V8Mode]     = kAPP_ValLdoMode,
    [kPOWER_Resource_LdoVdda1V8Mode] = kAPP_ValLdoMode,
    [kPOWER_Resource_Hqref]          = kAPP_ValOnOff,
    [kPOWER_Resource_Sensors]        = kAPP_ValRaw,
    [kPOWER_Resource_ClkLdoa0V8]     = kAPP_ValOnOff,
    [kPOWER_Resource_ClkFro192M]     = kAPP_ValOnOff,
    [kPOWER_Resource_ClkFro12M]      = kAPP_ValOnOff,
    [kPOWER_Resource_ClkMainPll]     = kAPP_ValOnOff,
    [kPOWER_Resource_ClkCorePll]     = kAPP_ValOnOff,
    [kPOWER_Resource_ClkSysPll]      = kAPP_ValOnOff,
    [kPOWER_Resource_ClkLdoq0V8]     = kAPP_ValOnOff,
    [kPOWER_Resource_ClkSxosc]       = kAPP_ValOnOff,
    [kPOWER_Resource_ClkFro12MLp]    = kAPP_ValOnOff,
    [kPOWER_Resource_WakeSram]       = kAPP_ValMemMode,
    [kPOWER_Resource_ComputeSram01]  = kAPP_ValMemMode,
    [kPOWER_Resource_Ocram0M23]      = kAPP_ValMemMode,
    [kPOWER_Resource_Ocram0M4]       = kAPP_ValMemMode,
    [kPOWER_Resource_Ocram0M5]       = kAPP_ValMemMode,
    [kPOWER_Resource_Ocram1M0]       = kAPP_ValMemMode,
    [kPOWER_Resource_Ocram1M1]       = kAPP_ValMemMode,
    [kPOWER_Resource_Ocram2M0]       = kAPP_ValMemMode,
    [kPOWER_Resource_Ocram2M1]       = kAPP_ValMemMode,
    [kPOWER_Resource_LlcCache]       = kAPP_ValMemMode,
    [kPOWER_Resource_Audio8Kb]       = kAPP_ValMemMode,
    [kPOWER_Resource_M85Tcm]         = kAPP_ValMemMode,
    [kPOWER_Resource_NpuTcm]         = kAPP_ValMemMode,
    [kPOWER_Resource_RcgCompute]     = kAPP_ValOnOff,
    [kPOWER_Resource_RcgMain]        = kAPP_ValOnOff,
    [kPOWER_Resource_RcgWake]        = kAPP_ValOnOff,
    [kPOWER_Resource_RcgComm]        = kAPP_ValOnOff,
    [kPOWER_Resource_RcgMedia]       = kAPP_ValOnOff,
    [kPOWER_Resource_RcgAudio]       = kAPP_ValOnOff,
    [kPOWER_Resource_RcgWake1M]      = kAPP_ValOnOff,
    [kPOWER_Resource_RcgWake2M]      = kAPP_ValOnOff,
};

/*! Human-readable resource row names (indexed by power_standby_resource_t). */
static const char *const g_powerResourceNames[kPOWER_Resource_COUNT] = {
    [kPOWER_Resource_CpuDomain]    = "CPU domain event",
    [kPOWER_Resource_NpuDomain]    = "NPU domain event",
    [kPOWER_Resource_CommDomain]   = "COMM domain event",
    [kPOWER_Resource_MediaDomain]  = "MEDIA domain event",
    [kPOWER_Resource_PmuMode]           = "PMU mode",
    [kPOWER_Resource_DcdcMode]          = "DCDC mode",
    [kPOWER_Resource_CoreLvl]           = "Core voltage level",
    [kPOWER_Resource_Ldo0V8Mode]        = "LDO_0V8 mode",
    [kPOWER_Resource_Ldo1V8Mode]        = "LDO_1V8 mode",
    [kPOWER_Resource_LdoVdda1V8Mode]    = "LDO_VDDA_1V8 mode",
    [kPOWER_Resource_Hqref]             = "HQREF enable",
    [kPOWER_Resource_Sensors]           = "Sensor enable mask",
    [kPOWER_Resource_ClkLdoa0V8]        = "LDOA_0V8 keep-on",
    [kPOWER_Resource_ClkFro192M]        = "FRO_192M keep-on",
    [kPOWER_Resource_ClkFro12M]         = "FRO_12M keep-on",
    [kPOWER_Resource_ClkMainPll]        = "MAINPLL keep-on",
    [kPOWER_Resource_ClkCorePll]        = "COREPLL keep-on",
    [kPOWER_Resource_ClkSysPll]         = "SYSPLL keep-on",
    [kPOWER_Resource_ClkLdoq0V8]        = "LDOQ_0V8 keep-on",
    [kPOWER_Resource_ClkSxosc]          = "SXOSC keep-on",
    [kPOWER_Resource_ClkFro12MLp]       = "FRO_12M_LP keep-on",
    [kPOWER_Resource_WakeSram]          = "Wake SRAM mode",
    [kPOWER_Resource_ComputeSram01]     = "OCRAM0 M0/1 mode",
    [kPOWER_Resource_Ocram0M23]         = "OCRAM0 M2/3 mode",
    [kPOWER_Resource_Ocram0M4]          = "OCRAM0 M4 mode",
    [kPOWER_Resource_Ocram0M5]          = "OCRAM0 M5 mode",
    [kPOWER_Resource_Ocram1M0]          = "OCRAM1 M0 mode",
    [kPOWER_Resource_Ocram1M1]          = "OCRAM1 M1 mode",
    [kPOWER_Resource_Ocram2M0]          = "OCRAM2 M0 mode",
    [kPOWER_Resource_Ocram2M1]          = "OCRAM2 M1 mode",
    [kPOWER_Resource_LlcCache]          = "LLC cache mode",
    [kPOWER_Resource_Audio8Kb]          = "Audio 8KB mode",
    [kPOWER_Resource_M85Tcm]            = "M85 TCM mode",
    [kPOWER_Resource_NpuTcm]            = "NPU TCM mode",
    [kPOWER_Resource_RcgCompute]        = "COMPUTE root clk on",
    [kPOWER_Resource_RcgMain]           = "MAIN root clk on",
    [kPOWER_Resource_RcgWake]           = "WAKE root clk on",
    [kPOWER_Resource_RcgComm]           = "COMM root clk on",
    [kPOWER_Resource_RcgMedia]          = "MEDIA root clk on",
    [kPOWER_Resource_RcgAudio]          = "AUDIO root clk on",
    [kPOWER_Resource_RcgWake1M]         = "WAKE_1M root clk on",
    [kPOWER_Resource_RcgWake2M]         = "WAKE_2M root clk on",
};

/*! Standby mode column header names (indexed by app_standby_mode_idx_t). */
static const char *const g_standbyModeNames[kAPP_StandbyModeIdx_COUNT] = {
    [kAPP_StandbyModeIdx_Sleep] = "Sleep",
    [kAPP_StandbyModeIdx_DS1]   = "DS1",
    [kAPP_StandbyModeIdx_DS2]   = "DS2",
    [kAPP_StandbyModeIdx_DS3]   = "DS3",
    [kAPP_StandbyModeIdx_PD]    = "PD",
};

/*! @brief Render a non-raw cell value as its short display string. */
static const char *APP_FormatCellValue(app_val_kind_t kind, uint32_t value)
{
    switch (kind)
    {
        case kAPP_ValOnOff:
            return (value != 0U) ? "ON" : "OFF";
        case kAPP_ValDomain:
            return (value == (uint32_t)kPDCON_EventPowerOff) ? "Off" : "Active";
        case kAPP_ValPmuMode:
            switch (value)
            {
                case PMU_HP:  return "HP";
                case PMU_LP:  return "LP";
                case PMU_RET: return "RET";
                case PMU_BB:  return "BB";
                default:      return "?";
            }
        case kAPP_ValDcdcMode:
            return (value == (uint32_t)kPMU_DcdcModePWM) ? "PWM" : "PFM";
        case kAPP_ValLdoMode:
            return (value == (uint32_t)kPMU_LdoModeHP) ? "HP" : "LP";
        case kAPP_ValMemMode:
            if (value == (uint32_t)kMEMCON_PowerModeRetention) { return "Retention"; }
            if (value == (uint32_t)kMEMCON_PowerModePowerDown) { return "PowerDown"; }
            return "Active";
        case kAPP_ValRaw:
        default:
            return "?";
    }
}

/*!
 * @brief Print every column of g_powerModeTable (the static resource x standby
 * mode reference table), independent of whichever mode/wakeup source is later
 * selected from the menu.
 *
 * Companion to APP_PrintMeasuredClocks(): same debug-gated, informational dump,
 * but of the compile-time g_powerModeTable instead of measured hardware
 * frequencies.  NC cells (not configurable in that mode) print as "NC"; other
 * cells print a short human-readable string per s_resourceValKind[], or raw hex
 * for the two rows (CoreLvl, Sensors) that have no simple state name.
 */
void APP_PrintModeTable(void)
{
    uint32_t r;
    uint32_t c;

    PRINTF("\r\n----------------------- g_powerModeTable (resource x standby mode) -------------------\r\n");
    PRINTF("    %-20s", "resource");
    for (c = 0U; c < (uint32_t)kAPP_StandbyModeIdx_COUNT; c++)
    {
        PRINTF(" | %10s", g_standbyModeNames[c]);
    }
    PRINTF("\r\n");

    for (r = 0U; r < (uint32_t)kPOWER_Resource_COUNT; r++)
    {
        PRINTF("    %-20s", g_powerResourceNames[r]);
        for (c = 0U; c < (uint32_t)kAPP_StandbyModeIdx_COUNT; c++)
        {
            const power_mode_cell_t *cell = &g_powerModeTable[r][c];
            if (!cell->valid)
            {
                PRINTF(" | %10s", "NC");
            }
            else if (s_resourceValKind[r] == kAPP_ValRaw)
            {
                PRINTF(" | 0x%08X", (unsigned int)cell->value);
            }
            else
            {
                PRINTF(" | %10s", APP_FormatCellValue(s_resourceValKind[r], cell->value));
            }
        }
        PRINTF("\r\n");
    }
    PRINTF("---------------------------------------------------------------------------------------\r\n");
}

#endif /* APP_PRINT_DEBUG_INFO */

/*! @} */
