// Joshua Wienecke NXP

#include "sm_platform.h"
#include "pin_mux.h"
#include "hal_power.h"
#include "hal_clock.h"
#include "hal_pinctrl.h"
#include "fsl_debug_console.h"
#include "fsl_lptmr.h"

#define BOARD_DEBUG_UART_INSTANCE   3
#define BOARD_DEBUG_UART_CLOCK_ROOT hal_clock_lpuart3
#define BOARD_DEBUG_UART_CLK_FREQ   HAL_ClockGetIpFreq(BOARD_DEBUG_UART_CLOCK_ROOT)
#define BOARD_DEBUG_UART_BAUDRATE   115200U
#define BOARD_DEBUG_UART_TYPE       kSerialPort_Uart

static void init_debug_console(void)
{
    hal_clk_t hal_clk = {
        .clk_id = BOARD_DEBUG_UART_CLOCK_ROOT,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    
    HAL_ClockSetRootClk(&hal_clk);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    BOARD_DEBUG_UART_CLK_FREQ);
}

static void init_boot_clocks(void)
{
    g_clockSourceFreq[kCLOCK_Ext]             = 133000000U;
    g_clockSourceFreq[kCLOCK_Osc32K]          = 3000U;
    g_clockSourceFreq[kCLOCK_Osc24M]          = 24000000U;
    g_clockSourceFreq[kCLOCK_Fro]             = 333330000U;
    g_clockSourceFreq[kCLOCK_SysPll1Dfs0]     = 1000000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Dfs0Div2] = 500000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Dfs1]     = 800000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Dfs1Div2] = 400000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Dfs2]     = 666670000U;
    g_clockSourceFreq[kCLOCK_SysPll1Dfs2Div2] = 333330000U;
    g_clockSourceFreq[kCLOCK_AudioPll1]       = 400000000U;
    g_clockSourceFreq[kCLOCK_AudioPll2]       = 400000000U;
    g_clockSourceFreq[kCLOCK_DramPll]         = 1000000000U;
    g_clockSourceFreq[kCLOCK_VideoPll1]       = 700000000U;
    g_clockSourceFreq[kCLOCK_VideoPll2]       = 700000000U;
    g_clockSourceFreq[kCLOCK_VideoPll3]       = 700000000U;
}

static void init_board_display(void) 
{
    hal_clk_t hal_dispapbCLKCfg = {   // display APB
        .clk_id = hal_clock_dispapb,
        .pclk_id = hal_clock_syspll1dfs1div2,
        .div = 3, /* Source clock value 400Mhz, the current freq 133Mhz */
        .enable_clk = true,
        // .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_dispaxiCLKCfg = {  // display AXI
        .clk_id = hal_clock_dispaxi,
        .pclk_id = hal_clock_syspll1dfs2,
        .div = 1, /* AXI clock 800Mhz */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_dispocramCLKCfg = {  // On-chip RAM
        .clk_id = hal_clock_dispocram,
        .pclk_id = hal_clock_syspll1dfs2,
        .div = 2, /* Ocram clocck value 333Mhz*/
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    hal_clk_t hal_lpi2cClkCfg = {      // LPI2C
        .clk_id = hal_clock_lpi2c2,
        .pclk_id = hal_clock_osc24m,
        .div = 1, /* 24Mhz for lpi2c */
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };

    HAL_ClockSetRootClk(&hal_dispapbCLKCfg);
    HAL_ClockSetRootClk(&hal_dispaxiCLKCfg);
    HAL_ClockSetRootClk(&hal_dispocramCLKCfg);
    HAL_ClockSetRootClk(&hal_lpi2cClkCfg);

    // power on the DisplayMix
    hal_pwr_s_t pwrst = {
        .did = HAL_POWER_PLATFORM_MIX_SLICE_IDX_DISPLAY,
        .st = hal_power_state_on,
    };
    if (HAL_PowerSetState(&pwrst) != 0) {
        PRINTF("HAL_PowerSetState failed\n\n");
    }
    if (HAL_PowerGetState(&pwrst) != 0) {
        PRINTF("HAL_PowerGetState failed\r\n");
    }
}

void init_board_dpu(void) 
{
    SM_Platform_Init();

    BOARD_InitBootPins();

    init_boot_clocks();
    init_debug_console();
    init_board_display();
    
}