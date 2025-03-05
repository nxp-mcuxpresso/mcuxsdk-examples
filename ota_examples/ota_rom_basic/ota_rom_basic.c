/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_shell.h"
#include "board.h"
#include "clock_config.h"
#include "app.h"

#include "mflash_drv.h"
#include "xmodem.h"
#include "platform_bindings.h"
#include "sb3_api.h"

#include <ctype.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static shell_status_t shellCmd_image(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shellCmd_xmodem_sb3(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shellCmd_mem(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shellCmd_reboot(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/


static SHELL_COMMAND_DEFINE(image,
                            "\n\"image [info]\"          : Print image information"
                            "\n",
                            shellCmd_image,
                            SHELL_IGNORE_PARAMETER_COUNT);

static SHELL_COMMAND_DEFINE(mem,
                            "\n\"mem read addr [size]\" : Read memory at given address"
                            "\n\"mem erase addr \"      : Erase sector containing given address"
                            "\n",
                            shellCmd_mem,
                            SHELL_IGNORE_PARAMETER_COUNT);

static SHELL_COMMAND_DEFINE(xmodem_sb3, "\n\"xmodem_sb3\": Start SB3 receiving with XMODEM-CRC\n", shellCmd_xmodem_sb3, SHELL_IGNORE_PARAMETER_COUNT);
static SHELL_COMMAND_DEFINE(reboot, "\n\"reboot\": Triggers software reset\n", shellCmd_reboot, 0);

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

/*
 * Buffer used to handover data from XMODEM to flash programming routine.
 * Uses 4B alignment to be compatible with mflash.
 **/
static uint32_t progbuf[1024/sizeof(uint32_t)];

/*******************************************************************************
 * Code
 ******************************************************************************/

static void hexdump(const void *src, size_t size)
{
    const unsigned char *src8 = src;
    const int CNT             = 16;

    for (size_t i = 0; i < size; i++)
    {
        int n = i % CNT;
        if (n == 0)
            PRINTF("%08x  ", (uint32_t)src+i);
        PRINTF("%02X ", src8[i]);
        if ((i && n == CNT - 1) || (i + 1 == size))
        {
            int rem = CNT - 1 - n;
            for (int j = 0; j < rem; j++)
                PRINTF("   ");
            PRINTF("|");
            for (int j = n; j >= 0; j--)
                PUTCHAR(isprint(src8[i - j]) ? src8[i - j] : '.');
            PRINTF("|\n");
        }
    }
    PUTCHAR('\n');
}

static shell_status_t shellCmd_image(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    mbi_print_info();
    return kStatus_SHELL_Success;
}

static shell_status_t shellCmd_mem(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int ret;
    uint32_t addr;
    uint32_t size = 128;
    char *parse_end;

    if (argc < 3 || argc > 4)
    {
        PRINTF("Wrong argument count\n");
        return kStatus_SHELL_Error;
    }

    addr = strtol(argv[2], &parse_end, 0);
    if (*parse_end != '\0')
    {
        PRINTF("Bad address\n");
        return kStatus_SHELL_Error;
    }

    if (argc == 4)
    {
        size = strtol(argv[3], &parse_end, 0);
        if (*parse_end != '\0')
        {
            PRINTF("Bad size\n");
            return kStatus_SHELL_Error;
        }
    }

    /* mem read addr [size] */

    if (!strcmp(argv[1], "read"))
    {
#ifdef MFLASH_PAGE_INTEGRITY_CHECKS
        if (mflash_drv_is_readable(addr) != kStatus_Success)
        {
            PRINTF("Page not readable\n");
            return kStatus_SHELL_Error;
        }
#endif
        hexdump((void *)addr, size);
    }

    /* mem erase addr */

    else if (!strcmp(argv[1], "erase"))
    {
        ret = mflash_drv_sector_erase(addr & ~(MFLASH_SECTOR_SIZE-1));
        if (ret)
        {
            PRINTF("Failed to erase sector (ret=%d)\n", ret);
            return kStatus_SHELL_Error;
        }
    }

    else
    {
        PRINTF("Wrong arguments. See 'help'\n");
        return kStatus_SHELL_Error;
    }

    return kStatus_SHELL_Success;
}

static int process_received_data_sb3(uint32_t dst_addr, uint32_t offset, uint32_t size)
{
    int ret;
    uint32_t *data = progbuf;
    uint32_t chunk_sz;

    static uint32_t sb_size;
    static uint32_t bytes_processed;

    if(offset == 0)
    {
        /* first chunk */
        if(!sb3_parse_header(data, &sb_size))
        {
            return -1;
        }
        bytes_processed = 0;
    }

    if(sb_size == bytes_processed)
    {
        /* just in case */
        return -1;
    }

    if (sb_size - bytes_processed > size) {
        chunk_sz = size;
    } else {
        /* last chunk */
        chunk_sz = sb_size - bytes_processed;
    }

    /* Processing SB3 image */
    ret = sb3_api_pump((uint8_t *)data, chunk_sz);
    if (ret != kStatus_Success)
    {
        PRINTF("sb3_api_pump failed/n");
        return -1;
    }

    bytes_processed += chunk_sz;
    return 0;
}

static shell_status_t shellCmd_xmodem_sb3(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    long recvsize;

    if (argc > 2)
    {
        PRINTF("Too many arguments.\n");
        return kStatus_SHELL_Error;
    }

    if (sb3_api_init() != kStatus_Success)
    {
        PRINTF("sb3_iap_init failed/n");
        return kStatus_SHELL_Error;
    }

    PRINTF("Started xmodem processing SB3\n");

    struct xmodem_cfg cfg = {
        .putc = xmodem_putc,
        .getc = xmodem_getc,
        .canread = xmodem_canread,
        .canread_retries = xmodem_canread_retries,
        .dst_addr = 0x0,
        .maxsize = 0xFFFFFFFF,
        .buffer = (uint8_t*)progbuf,
        .buffer_size = sizeof(progbuf),
        .buffer_full_callback = process_received_data_sb3
    };

    PRINTF("Initiated XMODEM-CRC transfer. Receiving... (Press 'x' to cancel)\n");

    recvsize = xmodem_receive(&cfg);

    /* With some terminals it takes a while before they recover receiving to the console */
    SDK_DelayAtLeastUs(1000000, SystemCoreClock);

    if (recvsize < 0)
    {
        PRINTF("\nTransfer failed (%d)\n", recvsize);
        return kStatus_SHELL_Error;
    }
    PRINTF("\nReceived %u bytes\n", recvsize);

    PRINTF("SB3 has been processed\n");
    sb3_api_finalize();
    sb3_api_deinit();

    return kStatus_SHELL_Success;
}

static shell_status_t shellCmd_reboot(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    PRINTF("System reset!\n");
    NVIC_SystemReset();

    /* return kStatus_SHELL_Success; */
}

/*!
 * @brief Main function
 */
int main(void)
{
    int ret;
    s_shellHandle = &s_shellHandleBuffer[0];

    /* Init board hardware. */
    BOARD_InitHardware();

    ret = mflash_drv_init();
    if (ret)
    {
        PRINTF("Failed to init flash driver\n");
    }

    PRINTF("\n"
           "*************************************\n"
           "* Basic ROM application example     *\n"
           "*************************************\n\n");

    PRINTF("Built " __DATE__ " " __TIME__ "\n");

    if(sb3_check_provisioning(true) == 0)
    {
        PRINTF("WARNING! Device doesn't seem to be configured properly! Check instructions in readme file.\n");
    }

    ret = SHELL_Init(s_shellHandle, g_serialHandle, "$ ");
    if (ret != kStatus_SHELL_Success)
    {
        PRINTF("Failed to init shell\n");
        goto failed_init;
    }

    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(image));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(xmodem_sb3));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(mem));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(reboot));

    while (1)
    {
        SHELL_Task(s_shellHandle);
    }

failed_init:
    while (1)
    {
    }
}
