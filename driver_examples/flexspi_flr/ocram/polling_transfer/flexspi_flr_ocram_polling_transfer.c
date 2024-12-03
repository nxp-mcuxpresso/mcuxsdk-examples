/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_flexspi.h"
#include "fsl_flexspi_flr.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FOLLOWER_STATUS_AXI_ACCESS (FLEXSPI_SLV_MODULE_STATUS_ALLOWAXIWR_MASK | FLEXSPI_SLV_MODULE_STATUS_ALLOWAXIRD_MASK)
#define FOLLOWER_STATUS_AXI_WRITE  (FLEXSPI_SLV_MODULE_STATUS_SEQIDLE_MASK | FLEXSPI_SLV_MODULE_STATUS_WIP_MASK)
#define FOLLOWER_STATUS_AXI_READ   (FLEXSPI_SLV_MODULE_STATUS_SEQIDLE_MASK | FLEXSPI_SLV_MODULE_STATUS_AXIREADIDLE_MASK)
#define FOLLOWER_STATUS_REG_ACCESS (FLEXSPI_SLV_MODULE_STATUS_SEQIDLE_MASK | FLEXSPI_SLV_MODULE_STATUS_REGRWIDLE_MASK)
#define FOLLOWER_STATUS_AXI_WRITE_IDLE FLEXSPI_SLV_MODULE_STATUS_SEQIDLE_MASK

#define CLOCK_DEBUG 0
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t flexspi_ocram_send_mailbox(FLEXSPI_Type *base, uint8_t index, uint32_t value);
extern status_t flexspi_ocram_status_get(FLEXSPI_Type *base, uint32_t *readValue);
extern status_t flexspi_ocram_read_memory(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t length);
extern status_t flexspi_ocram_write_memory(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t length);
extern void flexspi_ocram_init(FLEXSPI_Type *base);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Program data buffer should be 4-bytes alignment, which can avoid busfault due to this memory region is configured as
   Device Memory by MPU. */
SDK_ALIGN(static char s_follower_write_buffer[256], 4);
SDK_ALIGN(static char s_follower_read_buffer[256], 4);
static char *io_mode[4] =
{
    "SDRx4", "SDRx8", "DDRx4", "DDRx8"
};
/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    uint32_t intEN = 0, slvStatus;
    status_t status;
    uint8_t str_len = 0;
    flexspi_slv_config_t config;
    flexspi_slv_handle_t handle;

    BOARD_InitHardware();

    deviceconfig.flexspiRootClk = get_rootClock_freq_hz(EXAMPLE_FLEXSPI_ROOT_CLOCK_SRC);
    PRINTF("\r\nFLEXSPI Follower(Root Clock: %dMHz, IO Mode: %s, Leader Root Clock: %dHz) example started!\r\n",
           EXAMPLE_FLEXSPI_SLV_ROOT_CLOCK, io_mode[EXAMPLE_FLEXSPI_SLV_MODE], deviceconfig.flexspiRootClk);

    FLEXSPI_SLV_GetDefaultConfig(&config);
    config.io_mode     = EXAMPLE_FLEXSPI_SLV_MODE;
    config.clock_freq  = EXAMPLE_FLEXSPI_SLV_ROOT_CLOCK;
    config.baseAddr1   = OCRAM_BASEADDR;
    config.baseAddr2   = OCRAM_BASEADDR;
    config.addrRange1  = MEMORY_SIZE;
    config.addrRange2  = MEMORY_SIZE;
    config.rxWatermark = 32;
    config.txWatermark = Write_Watermark_256Bytes;

    FLEXSPI_SLV_Init(EXAMPLE_FLEXSPI_SLV, &config);
    PRINTF("FLEXSPI Follower is initialized!\r\n");
#if CLOCK_DEBUG
    PRINTF("\t---detected  Core clock frequency is %4d MHz\r\n",   CLOCK_GetFreq(kCLOCK_ArmPllOut)/1000000);
    PRINTF("\t---detected  OSC_RC_24M frequency is %4d MHz\r\n",   CLOCK_GetFreq(kCLOCK_OscRc24M)/1000000);
    PRINTF("\t---detected  OSC_RC_400M frequency is %4d MHz\r\n",  CLOCK_GetFreq(kCLOCK_OscRc400M)/1000000);
    PRINTF("\t---detected  PLL_528 frequency is %4d MHz\r\n", CLOCK_GetFreq(kCLOCK_SysPll2Out)/1000000);
    PRINTF("\t---detected  PLL_1G frequency is %4d MHz\r\n",  CLOCK_GetFreq(kCLOCK_SysPll1Out)/1000000);
    PRINTF("\tFlexSPI Slave Root Clock SRC is = %2d\r\n", CLOCK_GetRootClockMux(kCLOCK_Root_Flexspi_Slv));
    PRINTF("\tFlexSPI Slave Root Clock DIV is = %2d\r\n", CLOCK_GetRootClockDiv(kCLOCK_Root_Flexspi_Slv));
    PRINTF("\tFlexSPI Slave Root Clock Status is = 0x%8X\r\n", CCM->CLOCK_ROOT[kCLOCK_Root_Flexspi_Slv].STATUS0);
#endif
    /* Sets the read register command */
    FLEXSPI_SLV_Read_Register_CommandSet(EXAMPLE_FLEXSPI_SLV, FLEXSPI_SLV_CMD_DDR(0x97));
    FLEXSPI_SLV_Read_Register_Command_DummyCyclesSet(EXAMPLE_FLEXSPI_SLV, EXAMPLE_FLEXSPI_SLV_DUMMY_CYCLE);
    /* Sets the read memory command */
    FLEXSPI_SLV_Read_CommandSet(EXAMPLE_FLEXSPI_SLV, 0, FLEXSPI_SLV_CMD_DDR(0x1F));
    FLEXSPI_SLV_Read_Command_DummyCyclesSet(EXAMPLE_FLEXSPI_SLV, 0, EXAMPLE_FLEXSPI_SLV_DUMMY_CYCLE);
    /* Sets the write register command */
    FLEXSPI_SLV_Write_Register_CommandSet(EXAMPLE_FLEXSPI_SLV, FLEXSPI_SLV_CMD_DDR(0xA6));
    /* Sets the write memory command */
    FLEXSPI_SLV_Write_CommandSet(EXAMPLE_FLEXSPI_SLV, 0, FLEXSPI_SLV_CMD_DDR(0x5B));

    intEN = FLEXSPI_SLV_MODULE_INTEN_WOFEN_MASK | FLEXSPI_SLV_MODULE_INTEN_RUFEN_MASK |
            FLEXSPI_SLV_MODULE_INTEN_ERRCMDEN_MASK;
    FLEXSPI_SLV_InterruptCreateHandle(EXAMPLE_FLEXSPI_SLV, &handle, Interrupt_callback, &intEN);

    flexspi_ocram_init(EXAMPLE_FLEXSPI);
    PRINTF("FLEXSPI leader is initialized!\r\n");

    /* Get FlexSPI Follower status. */
    status = flexspi_ocram_status_get(EXAMPLE_FLEXSPI, &slvStatus);
    if (status != kStatus_Success)
    {
        PRINTF("[%d] It's failed to get the follower status!\r\n", status);
        PRINTF("[Warn] The follower status is 0x%8X!\r\n",  EXAMPLE_FLEXSPI_SLV->MODULE_STATUS);
        PRINTF("[Warn] The follower control is 0x%8X!\r\n", EXAMPLE_FLEXSPI_SLV->MODULE_CONTROL);
        return status;
    }
    PRINTF("\r\n[Leader] The follower status: 0x%X\r\n", slvStatus);

    if ((slvStatus & FOLLOWER_STATUS_AXI_ACCESS) != FOLLOWER_STATUS_AXI_ACCESS)
    {
        PRINTF("[Leader] The follower AXI read/write access is not allowed!\r\n");
        return -2;
    }
    else
    {
        PRINTF("[Leader] The follower AXI read/write access is allowed!\r\n");
    }

    PRINTF("\r\n[Leader] Waiting that the AXI bus write is idle...\r\n");
    while (slvStatus & FLEXSPI_SLV_MODULE_STATUS_WIP_MASK)
    {
        status = flexspi_ocram_status_get(EXAMPLE_FLEXSPI, &slvStatus);
    }

    sprintf(s_follower_write_buffer, "[%s] This string is used to verify the memory write!", __TIME__);
    str_len = strlen(s_follower_write_buffer) + 1;
    PRINTF("[Leader] Original string: %s\r\n", s_follower_write_buffer);

    PRINTF("[Leader] Writing follower's memory...\r\n");
    status = flexspi_ocram_write_memory(EXAMPLE_FLEXSPI, EXAMPLE_MEMORY_ADDR,
                                        (uint32_t *)s_follower_write_buffer, str_len);
    if (status != kStatus_Success)
    {
        PRINTF("[Leader] Write memory failure: %d!\r\n", status);
        return status;
    }
    PRINTF("[Leader] Write finished!\r\n");

    PRINTF("\r\n[Leader] Waiting that the follower mailbox is idle...\r\n");
    do
    {
        status = flexspi_ocram_status_get(EXAMPLE_FLEXSPI, &slvStatus);
    }
    while (!(slvStatus & FLEXSPI_SLV_MODULE_STATUS_REGRWIDLE_MASK));

    PRINTF("[Leader] Sending a mailbox to the follower...\r\n");
    status = flexspi_ocram_send_mailbox(EXAMPLE_FLEXSPI, EXAMPLE_MAILBOX_INDEX,
                                        FLEXSPI_SLV_MAILBOX_CMD_INT(EXAMPLE_MEMORY_ADDR));
    if (status != kStatus_Success)
    {
        PRINTF("[Leader] Send mailbox failure!\r\n");
        return status;
    }
    PRINTF("[Leader] Send finished -- 0x%X!\r\n", EXAMPLE_FLEXSPI_SLV->SPIMAIL[EXAMPLE_MAILBOX_INDEX]);

    PRINTF("\r\n[Leader] Waiting that the AXI bus read is idle...\r\n");
    do
    {
        status = flexspi_ocram_status_get(EXAMPLE_FLEXSPI, &slvStatus);
    }
    while (!(slvStatus & FLEXSPI_SLV_MODULE_STATUS_AXIREADIDLE_MASK));

    PRINTF("[Leader] Reading the data from the follower memory...\r\n");
    memset(s_follower_read_buffer, 0, 256);
    status = flexspi_ocram_read_memory(EXAMPLE_FLEXSPI, EXAMPLE_MEMORY_ADDR,
                                       (uint32_t *)s_follower_read_buffer, str_len);
    if (status != kStatus_Success)
    {
        PRINTF("[Leader] Read memory failure!\r\n");
        return status;
    }
    PRINTF("[Leader] The read data: %s\r\n", s_follower_read_buffer);

    sprintf(s_follower_write_buffer, "[%s] Verified the memory write via the AHB bus!", __TIME__);
    str_len = strlen(s_follower_write_buffer) + 1;
    PRINTF("\r\n[Leader] Original string(AHB): %s\r\n", s_follower_write_buffer);
    PRINTF("[Leader] Writing follower's memory via AHB bus...\r\n");
    memcpy((void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_MEMORY_ADDR_AHB),
           (void *)s_follower_write_buffer, str_len + 1);
    PRINTF("[Leader] Write finished(AHB)!\r\n");

    PRINTF("\r\n[Leader] Reading the data from the follower memory via AHB bus...\r\n");
    memset(s_follower_read_buffer, 0, 256);
    memcpy((void *)s_follower_read_buffer, (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_MEMORY_ADDR_AHB),
           str_len + 1);
    PRINTF("[Leader] The read data(AHB): %s\r\n", s_follower_read_buffer);

    while (1)
    {
    }
}
