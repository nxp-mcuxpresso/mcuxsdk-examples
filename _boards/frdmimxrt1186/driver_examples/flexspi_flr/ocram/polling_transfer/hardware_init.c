/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "app.h"
#include "fsl_flexspi.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
flexspi_device_config_t deviceconfig = {
    .flexspiRootClk       = 0,
    .flashSize            = MEMORY_SIZE,
    .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval           = 2,
    .CSHoldTime           = 3,
    .CSSetupTime          = 3,
    .dataValidTime        = 2,
    .columnspace          = 0,
    .enableWordAddress    = 0,
    .AWRSeqIndex          = FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(EXAMPLE_FLEXSPI_SLV_MODE),
    .AWRSeqNumber         = 1,
    .ARDSeqIndex          = FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(EXAMPLE_FLEXSPI_SLV_MODE),
    .ARDSeqNumber         = 1,
    .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
};

const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
    /* Read status register - SDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_SDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_4PAD, 0x97, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_SDRx4) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_4PAD, 0),

    /* Read memory -SDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_4PAD, 0x1F, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx4) + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x14, kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x04),

    /* Send mailbox - SDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_SDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_4PAD, 0xA6, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_SDRx4) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_4PAD, 0),

    /* Write Memory - SDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_4PAD, 0x5B, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx4) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_4PAD, 0),

    /* Read status register - SDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_SDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x97, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_SDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_8PAD, 0),

    /* Read memory -SDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x1F, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_8PAD, 0),

    /* Send mailbox - SDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_SDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0xA6, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_SDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_8PAD, 0),

    /* Write Memory - SDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x5B, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_SDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_8PAD, 0),

    /* Read status register - DDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_DDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_4PAD, 0x97, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_DDRx4) + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_4PAD, 0x14, kFLEXSPI_Command_READ_DDR, kFLEXSPI_4PAD, 0x04),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_DDRx4) + 2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_STOP, kFLEXSPI_4PAD, 0, 0, 0, 0),

    /* Read memory -DDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_4PAD, 0x1F, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx4) + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_DDR, kFLEXSPI_4PAD, 0x14, kFLEXSPI_Command_READ_DDR, kFLEXSPI_4PAD, 0x04),

    /* Send mailbox - DDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_DDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_4PAD, 0xA6, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_DDRx4) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_4PAD, 0),

    /* Write Memory - DDRx4 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx4)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_4PAD, 0x5B, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_4PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx4) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_4PAD, 0),

    /* Read status register - DDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_DDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x97, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_REG_STATUS(kFLEXSPI_SLV_IOMODE_DDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_8PAD, 0),

    /* Read memory -DDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x1F, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_READ_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_8PAD, 0),

    /* Send mailbox - DDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_DDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0xA6, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_SEND_MAILBOX(kFLEXSPI_SLV_IOMODE_DDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_8PAD, 0),

    /* Write Memory - DDRx8 */
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx8)] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_8PAD, 0x5B, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x20),
    [4 * FOLLOWER_CMD_LUT_SEQ_IDX_WRITE_MEMORY(kFLEXSPI_SLV_IOMODE_DDRx8) + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_8PAD, 0),
};

uint32_t get_rootClock_freq_hz(clock_root_mux_source_t clk_name)
{
    switch (clk_name)
    {
        case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc24M:
        {
            return CLOCK_GetFreq(kCLOCK_OscRc24M) / EXAMPLE_FLEXSPI_ROOT_CLOCK_DIV;
        }
        case kCLOCK_FLEXSPI1_ClockRoot_MuxOscRc400M:
        {
            return CLOCK_GetFreq(kCLOCK_OscRc400M) / EXAMPLE_FLEXSPI_ROOT_CLOCK_DIV;
        }
        case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll3Pfd0:
        {
            return CLOCK_GetFreq(kCLOCK_SysPll3Pfd0) / EXAMPLE_FLEXSPI_ROOT_CLOCK_DIV;
        }
        case kCLOCK_FLEXSPI1_ClockRoot_MuxSysPll2Pfd0:
        {
            return CLOCK_GetFreq(kCLOCK_SysPll2Pfd0) / EXAMPLE_FLEXSPI_ROOT_CLOCK_DIV;
        }
        default:
        {
            return 0;
        }
    }
}

void flexspi_flr_callback(FLEXSPI_SLV_Type *base, flexspi_slv_handle_t *handle)
{
    uint32_t value;

    if (handle->state != kFLEXSPI_SLV_InvalidInterruptFlag)
    {
        switch (handle->state)
        {
            case kFLEXSPI_SLV_Mail0InterruptFlag:
            {
                value = FLEXSPI_SLV_GetMailboxData(base, 0);
                PRINTF("[Follower] Mailbox data(interrupt): %d.\r\n", FLEXSPI_SLV_MAILBOX_CMD(value));
                break;
            }

            default:
            {
                break;
            }
        }

        handle->state = kFLEXSPI_SLV_InvalidInterruptFlag;
    }
}

void FLEXSPI_SLV_ClkRootFrq(flexspi_slv_clock_freq_t clock_freq)
{
    clock_root_config_t rootCfg = {0};

    switch (clock_freq)
    {
        case kFLEXSPI_SLV_RootClock_50M:
            /* Configure FLEXSPI_SLV using OSC_RC_400M */
            rootCfg.mux = kCLOCK_FLEXSPI_SLV_ClockRoot_MuxOscRc400M;
            rootCfg.div = 8;
            break;

        case kFLEXSPI_SLV_RootClock_66M:
            /* Configure FLEXSPI_SLV using SYS_PLL2_CLK */
            rootCfg.mux = kCLOCK_FLEXSPI_SLV_ClockRoot_MuxSysPll2Out;
            rootCfg.div = 8;
            break;

        case kFLEXSPI_SLV_RootClock_80M:
            /* Configure FLEXSPI_SLV using OSC_RC_400M */
            rootCfg.mux = kCLOCK_FLEXSPI_SLV_ClockRoot_MuxOscRc400M;
            rootCfg.div = 5;
            break;

        case kFLEXSPI_SLV_RootClock_100M:
            /* Configure FLEXSPI_SLV using OSC_RC_400M */
            rootCfg.mux = kCLOCK_FLEXSPI_SLV_ClockRoot_MuxOscRc400M;
            rootCfg.div = 4;
            break;

        case kFLEXSPI_SLV_RootClock_166M:
            /* Configure FLEXSPI_SLV using SYS_PLL1_CLK */
            rootCfg.mux = kCLOCK_FLEXSPI_SLV_ClockRoot_MuxSysPll1Out;
            rootCfg.div = 6;
            break;

        case kFLEXSPI_SLV_RootClock_200M:
            /* Configure FLEXSPI_SLV using SYS_PLL1_CLK */
            rootCfg.mux = kCLOCK_FLEXSPI_SLV_ClockRoot_MuxSysPll1Out;
            rootCfg.div = 5;
            break;

        case kFLEXSPI_SLV_RootClock_400M:
            /* Configure FLEXSPI_SLV using OSC_RC_400M */
            rootCfg.mux = kCLOCK_FLEXSPI_SLV_ClockRoot_MuxOscRc400M;
            rootCfg.div = 1;
            break;

        default:
            /* RootClock_133M: Configure FLEXSPI_SLV using SYS_PLL2_CLK */
            rootCfg.mux = kCLOCK_FLEXSPI_SLV_ClockRoot_MuxSysPll2Out;
            rootCfg.div = 4;
            break;
    }
    CLOCK_SetRootClock(kCLOCK_Root_Flexspi_Slv, &rootCfg);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitFLEXSPI_FLRPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    FLEXSPI_SLV_ClkRootFrq(EXAMPLE_FLEXSPI_SLV_ROOT_CLOCK);
}

/*${function:end}*/
