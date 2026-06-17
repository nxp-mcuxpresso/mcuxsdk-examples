/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_espi.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#ifndef EXAMPLE_ESPI_DEBUG_LOG
#define EXAMPLE_ESPI_DEBUG_LOG 0U
#endif

#ifndef EXAMPLE_ESPI_SAF_RX_SPLIT_MAX_COUNT
#define EXAMPLE_ESPI_SAF_RX_SPLIT_MAX_COUNT 4U
#endif

typedef struct
{
    const char *name;
    espi_vw_wr_flags_t flag;
} vw_flag_name_t;

static struct
{
    struct
    {
        uint32_t addr;
        uint32_t len;
        uint32_t rxType;
    } item[EXAMPLE_ESPI_SAF_RX_SPLIT_MAX_COUNT];
    uint32_t tag;
    uint32_t curIdx;
    uint32_t totIdx;
} g_readQueue;

static espi_handle_t g_espiHandle;
static espi_config_t g_espi_config;
static uint8_t g_virtualFlash[256];
static uint8_t g_msgBuffer[256];
static char g_cmdLine[30];

static const espi_port_config_t g_portCfg[] = {
    {
        .type       = kESPI_PortType_MailboxOOBSplit,
        .direction  = 0U,
        .ramOffset  = 0x0000U,
        .ramSize    = kESPI_RamSize256B,
        .addrOffset = 0x0000U, /* Useless. */
        .addrBase   = kESPI_AddrBaseDirect,
    },
    {
        .type       = kESPI_PortType_ACPIEndpoint,
        .direction  = 0U,
        .ramOffset  = 0x0100U,         /* Useless. */
        .ramSize    = kESPI_RamSize4B, /* Useless. */
        .addrOffset = 0x0100U,
        .addrBase   = 0U,
    },
    {
        .type       = kESPI_PortType_ACPIIndexData,
        .direction  = 1U,
        .ramOffset  = 0x0200U,         /* Useless. */
        .ramSize    = kESPI_RamSize4B, /* Useless. */
        .addrOffset = 0x0200U,
        .addrBase   = kESPI_AddrBaseDirect,
        .idxOffset  = 1,
    },
    {
        .type       = kESPI_PortType_MailboxSingle,
        .direction  = 0U,
        .ramOffset  = 0x0300U,
        .ramSize    = kESPI_RamSize256B,
        .addrOffset = 0x0300U,
        .addrBase   = kESPI_AddrBaseDirect,
    },
    {
        .type       = kESPI_PortType_BusMasterFlashSingle,
        .direction  = 0U,
        .ramOffset  = 0x0500U,
        .ramSize    = kESPI_RamSize64B, /* Useless. */
        .addrOffset = 0x500U,
        .addrBase   = kESPI_AddrBase0,
    },
};

static void ExampleFlashOps(ESPI_Type *base, espi_handle_t *handle, espi_flash_request_t *req, void *userData)
{
    uint8_t *memory = g_virtualFlash;
    uint32_t addr   = req->addr;
    uint32_t length = req->length;
    uint8_t *data   = req->data;

    if (req->type == kESPI_FlashTransErase)
    {
        ESPI_SetFlashCompletion(base, handle->safPort, req->tag, (uint32_t)kESPI_SSTCL_SAFReqAccepted,
                                (uint32_t)kESPI_SAFReadMiddle);
        (void)PRINTF("[SAF] Erase addr=0x%08X, len=%u\r\n", (unsigned)addr, (unsigned)length);
        (void)memset(&memory[addr], 0xFF, length);
        ESPI_SetFlashOpLen(base, handle->safPort, (uint32_t)kESPI_OMFLEN_SAFCompletionNoData, length);
        ESPI_SetFlashCompletion(base, handle->safPort, req->tag, (uint32_t)kESPI_SSTCL_SAFCompletion,
                                (uint32_t)kESPI_SAFReadMiddle);
    }
    else if (req->type == kESPI_FlashTransWrite)
    {
        ESPI_SetFlashCompletion(base, handle->safPort, req->tag, (uint32_t)kESPI_SSTCL_SAFReqAccepted,
                                (uint32_t)kESPI_SAFReadMiddle);
        (void)memcpy(&memory[addr], data, length);
        ESPI_SetFlashOpLen(base, handle->safPort, (uint32_t)kESPI_OMFLEN_SAFCompletionNoData, length);
        ESPI_SetFlashCompletion(base, handle->safPort, req->tag, (uint32_t)kESPI_SSTCL_SAFCompletion,
                                (uint32_t)kESPI_SAFReadMiddle);

        (void)PRINTF("[SAF] Write addr=0x%08X, len=%u, first bytes(Hex)=", (unsigned)addr, (unsigned)length);
        for (uint32_t i = 0U; (i < length) && (i < 8U); i++)
        {
            assert((UINT32_MAX - i) >= addr);
            (void)PRINTF("%02X ", memory[addr + i]);
        }
        (void)PRINTF("\r\n");
    }
    else if (req->type == kESPI_FlashTransRead)
    {
        /* Promira WAIT_STATE workaround: host sends one extra clock cycle,
           triggering a spurious callback with length=0. Skip it. */
        if (length == 0U)
        {
            return;
        }
        uint32_t maxPayload = ESPI_GetFlashMaxPayload(base);

        if (req->readStart)
        {
            ESPI_SetFlashCompletion(base, handle->safPort, req->tag, (uint32_t)kESPI_SSTCL_SAFReqAccepted,
                                    (uint32_t)kESPI_SAFReadMiddle);

            g_readQueue.tag    = req->tag;
            g_readQueue.curIdx = 0;
            g_readQueue.totIdx = 0;

            uint32_t remaining = length;
            uint32_t offset    = 0U;
            while (remaining > 0U)
            {
                uint32_t transLen = (remaining > maxPayload) ? maxPayload : remaining;
                espi_saf_rx_completion_type_t rxType;

                if (length <= maxPayload)
                {
                    rxType = kESPI_SAFReadOnly;
                }
                else if (remaining == length)
                {
                    rxType = kESPI_SAFReadFirst;
                }
                else if (remaining == transLen)
                {
                    rxType = kESPI_SAFReadLast;
                }
                else
                {
                    rxType = kESPI_SAFReadMiddle;
                }

                assert(g_readQueue.totIdx < EXAMPLE_ESPI_SAF_RX_SPLIT_MAX_COUNT);
                g_readQueue.item[g_readQueue.totIdx].addr   = addr + offset;
                g_readQueue.item[g_readQueue.totIdx].len    = transLen;
                g_readQueue.item[g_readQueue.totIdx].rxType = rxType;
                g_readQueue.totIdx++;

                assert((UINT32_MAX - offset) >= transLen);
                offset += transLen;
                remaining -= transLen;
            }
        }

        if (g_readQueue.curIdx < g_readQueue.totIdx)
        {
            uint32_t idx  = g_readQueue.curIdx;
            uint32_t addr = g_readQueue.item[idx].addr;
            uint32_t len  = g_readQueue.item[idx].len;

            (void)memcpy(data, &memory[addr], len);

            ESPI_SetFlashOpLen(base, handle->safPort, (uint32_t)kESPI_OMFLEN_SAFCompletionWithData, len);
            ESPI_SetFlashCompletion(base, handle->safPort, req->tag, (uint32_t)kESPI_SSTCL_SAFCompletion,
                                    (uint32_t)g_readQueue.item[idx].rxType);
            g_readQueue.curIdx++;
        }

        (void)PRINTF("[SAF] Read addr=0x%08X, len=%u, first bytes(Hex)=", (unsigned)addr, (unsigned)length);
        for (uint32_t i = 0U; (i < length) && (i < 8U); i++)
        {
            (void)PRINTF("%02X ", data[i]);
        }
        (void)PRINTF("\r\n");
    }
}

/* Read one line from console into buffer. Returns length. */
int read_line(char *buf, uint32_t maxlen)
{
    assert(maxlen > 0U);
    uint32_t idx = 0;

    while (idx < (maxlen - 1U))
    {
        int c = GETCHAR(); /* blocks until char */

        /* Handle Enter (CR or LF) to confirm the line */
        if (c == '\r' || c == '\n')
        {
            /* Echo newline to console */
            (void)PRINTF("\r\n");
            break;
        }

        /* Handle backspace/delete */
        if (c == '\b' || c == 0x7F)
        {
            if (idx > 0U)
            {
                idx--;
                /* Erase last char on console */
                (void)PRINTF("\b \b");
            }
            continue;
        }

        /* Only accept printable ASCII */
        if (c >= 0x20 && c <= 0x7E)
        {
            buf[idx++] = (char)c;
            /* Echo character */
            PUTCHAR((char)c);
        }
        /* Ignore other control characters */
    }
    buf[idx] = '\0';
    return idx;
}

/* Convert one hex digit to 0..15, or -1 if invalid. */
static int hex_nibble(char c)
{
    unsigned int uc = (unsigned int)(unsigned char)c;
    if ((uc >= (unsigned int)'0') && (uc <= (unsigned int)'9'))
    {
        return (int)(uc - (unsigned int)'0');
    }
    uc |= 0x20U; /* fold 'A'-'F' to 'a'-'f' */
    if ((uc >= (unsigned int)'a') && (uc <= (unsigned int)'f'))
    {
        return (int)((uc - (unsigned int)'a') + 10U);
    }
    return -1;
}

/* Helper: parse hex string (spaces allowed) into bytes. Returns byte count. */
int parse_hex_bytes(const char *s, uint8_t *out, int maxout)
{
    int len       = 0;
    const char *p = s;
    while (*p != '\0' && len < maxout)
    {
        while (*p && isspace((unsigned char)*p))
            p++;
        if (!*p)
            break;
        /* accept 0x prefix */
        if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
            p += 2;
        int hi = hex_nibble(p[0]);
        if (hi >= 0)
        {
            p++;
            int lo = hex_nibble(p[0]);
            if (lo < 0)
            {
                return -1;
            }
            p++;
            out[len++] = (uint8_t)((((unsigned int)hi << 4U) | ((unsigned int)lo & 0xFU)) & 0xFFU);
        }
        else
        {
            /* invalid, skip */
            p++;
        }
    }
    return len;
}

static const vw_flag_name_t vw_flag_names[] = {
    {"dswpwrokrst", kESPI_VWireWr_DswPwrokRst},
    {"booterrn", kESPI_VWireWr_BootErrn},
    {"bootdone", kESPI_VWireWr_BootDone},
    {"e2p", kESPI_VWireWr_E2P},
    {"susackn", kESPI_VWireWr_SusAckN},
    {"hostrstack", kESPI_VWireWr_HostRstAck},
    {"rcinn", kESPI_VWireWr_Rcinn},
    {"smin", kESPI_VWireWr_Smin},
    {"scin", kESPI_VWireWr_Scin},
    {"pmen", kESPI_VWireWr_Pmen},
    {"wakenscin", kESPI_VWireWr_WakenScin},
    {"oobrstack", kESPI_VWireWr_OobRstAck},
};

static void ESPI_PrintVWireFlagList(void)
{
    (void)PRINTF("Available VWire flags (send_vw_flag <name> <val>):\r\n");
    for (uint32_t i = 0U; i < (uint32_t)(sizeof(vw_flag_names) / sizeof(vw_flag_names[0])); i++)
    {
        uint32_t mask = (uint32_t)vw_flag_names[i].flag;
        /* bit width = number of set bits in mask */
        uint32_t width = 0U;
        for (uint32_t m = mask; m > 0U; m >>= 1U)
        {
            assert((UINT32_MAX - width) >= (m & 1U));
            width += (m & 1U);
        }
        (void)PRINTF("  %-12s (bits=%u)%s\r\n", vw_flag_names[i].name, width, (width > 1U) ? " [multi-bit]" : "");
    }
}

static bool ESPI_ParseVWireFlagName(const char *s, espi_vw_wr_flags_t *out)
{
    if (s == NULL || out == NULL)
        return false;

    const char *p      = s;
    const char *prefix = "kESPI_VWireWr_";
    size_t plen        = strlen(prefix);
    if (strncmp(p, prefix, plen) == 0)
        p += plen; /* allow full constant names */

    /* normalize to lowercase for comparison */
    char namebuf[32];
    size_t n = strlen(p);
    if (n >= sizeof(namebuf))
        n = sizeof(namebuf) - 1U;
    for (size_t i = 0U; i < n; i++)
    {
        namebuf[i] = (char)(tolower((unsigned char)p[i]) & 0x7F);
    }
    namebuf[n] = '\0';

    for (uint32_t i = 0U; i < (uint32_t)(sizeof(vw_flag_names) / sizeof(vw_flag_names[0])); i++)
    {
        if (strcmp(namebuf, vw_flag_names[i].name) == 0)
        {
            *out = vw_flag_names[i].flag;
            return true;
        }
    }
    return false;
}

static void print_espi_config(ESPI_Type *base)
{
    uint32_t cfg = base->ESPICFG;
    (void)PRINTF("\n--- Current eSPI Configuration (0x%08X) ---\r\n", cfg);

    /* IO Mode (SPIMOD bits [4:3]). */
    uint32_t io_mode = (cfg >> 3) & 0x3;
    (void)PRINTF("  IO Mode: ");
    switch (io_mode)
    {
        case 0:
            (void)PRINTF("Single SPI\r\n");
            break;
        case 1:
            (void)PRINTF("Dual SPI\r\n");
            break;
        case 2:
            (void)PRINTF("Quad SPI\r\n");
            break;
        default:
            (void)PRINTF("Reserved\r\n");
            break;
    }

    /* SPI Speed (SPISPD bits [9:7]). */
    uint32_t speed = (cfg >> 7) & 0x7;
    (void)PRINTF("  SPI Speed: ");
    switch (speed)
    {
        case 0:
            (void)PRINTF("<=20 MHz\r\n");
            break;
        case 1:
            (void)PRINTF("<=25 MHz\r\n");
            break;
        case 2:
            (void)PRINTF("<=33 MHz\r\n");
            break;
        case 3:
            (void)PRINTF("<=50 MHz\r\n");
            break;
        case 4:
            (void)PRINTF("<=66 MHz\r\n");
            break;
        default:
            (void)PRINTF("Reserved (%u)\r\n", speed);
            break;
    }

    /* CRC Checking (bit 10). */
    (void)PRINTF("  CRC Checking: %s\r\n", (cfg & (1U << 10)) ? "Enabled" : "Disabled");

    /* Alert configuration (bits 5-6). */
    (void)PRINTF("  Alert Pin: %s\r\n", (cfg & (1U << 5)) ? "Dedicated Pin" : "MISO");
    if (cfg & (1U << 5))
    {
        (void)PRINTF("  Alert Type: %s\r\n", (cfg & (1U << 6)) ? "Open Drain" : "Push-Pull");
    }

    /* Channel status. */
    (void)PRINTF("  Channels:\r\n");
    (void)PRINTF("    Ch0 (Memory): %s\r\n", (cfg & (1U << 12)) ? "Enabled" : "Disabled");
    (void)PRINTF("    Ch1 (VWire):  %s\r\n", (cfg & (1U << 13)) ? "Enabled" : "Disabled");
    (void)PRINTF("    Ch2 (OOB):    %s\r\n", (cfg & (1U << 14)) ? "Enabled" : "Disabled");
    (void)PRINTF("    Ch3 (Flash):  %s\r\n", (cfg & (1U << 18)) ? "Enabled" : "Disabled");

    /* Payload sizes. */
    uint32_t mem_payload   = (cfg >> 19) & 0x3;
    uint32_t oob_payload   = (cfg >> 21) & 0x3;
    uint32_t flash_payload = (cfg >> 1) & 0x3;

    (void)PRINTF("  Max Payload Sizes:\r\n");
    if (cfg & (1U << 12)) /* Memory channel enabled. */
    {
        (void)PRINTF("    Memory: %u bytes\r\n", 64U << mem_payload);
    }
    if (cfg & (1U << 14)) /* OOB channel enabled. */
    {
        (void)PRINTF("    OOB:    %u bytes\r\n", 64U << oob_payload);
    }
    if (cfg & (1U << 18)) /* Flash channel enabled. */
    {
        (void)PRINTF("    Flash:  %u bytes\r\n", 64U << flash_payload);
    }

    /* Flash erase size (bits [17:15]). */
    if (cfg & (1U << 18))
    {
        uint32_t erase = (cfg >> 15) & 0x7;
        (void)PRINTF("  Flash Erase: ");
        switch (erase)
        {
            case 0:
                (void)PRINTF("Disabled\r\n");
                break;
            case 1:
                (void)PRINTF("4 KB\r\n");
                break;
            case 2:
                (void)PRINTF("64 KB\r\n");
                break;
            case 3:
                (void)PRINTF("4 KB & 64 KB\r\n");
                break;
            case 4:
                (void)PRINTF("128 KB\r\n");
                break;
            case 5:
                (void)PRINTF("256 KB\r\n");
                break;
            default:
                (void)PRINTF("Reserved (%u)\r\n", erase);
                break;
        }
    }

    /* SAF (Slave Attached Flash) support (bit 0). */
    (void)PRINTF("  SAF Support: %s\r\n", (cfg & 0x1U) ? "Yes" : "No");

    /* Bus Master support (bit 11). */
    (void)PRINTF("  Bus Master: %s\r\n", (cfg & (1U << 11)) ? "Enabled" : "Disabled");
}

static void ESPI_CommonCallback(ESPI_Type *base, uint32_t status, void *userData)
{
    status_t result;

    (void)userData;

#if EXAMPLE_ESPI_DEBUG_LOG
    (void)PRINTF("\r\nGlobalCallback status = 0x%08X\r\n", status);
#endif

    if (status & kESPI_BusResetFlag)
    {
        (void)PRINTF("eSPI bus reset.\r\n");
        print_espi_config(base);
    }

    if (status & kESPI_CrcErrorFlag)
    {
        (void)PRINTF("eSPI bus CRC Error!\r\n");
    }

    if (status & kESPI_GpioFlag)
    {
#if EXAMPLE_ESPI_DEBUG_LOG
        (void)PRINTF("eSPI bus GPIO interrupt!\r\n");
#endif
    }

    if (status & kESPI_WireChangeFlag)
    {
        uint32_t vw = ESPI_GetVWire(base);
        (void)PRINTF("Virtual wire change: =0x%08X\r\n", vw);

        if (vw & kESPI_VWireRd_SlpS3N)
            (void)PRINTF(" - SLP_S3N\r\n");
        if (vw & kESPI_VWireRd_SlpS4N)
            (void)PRINTF(" - SLP_S4N\r\n");
        if (vw & kESPI_VWireRd_SlpS5N)
            (void)PRINTF(" - SLP_S5N\r\n");
        if (vw & kESPI_VWireRd_SusStat)
            (void)PRINTF(" - SUS_STAT\r\n");
        if (vw & kESPI_VWireRd_PltRst)
            (void)PRINTF(" - PLTRST\r\n");
        if (vw & kESPI_VWireRd_OobRstWarn)
            (void)PRINTF(" - OOB_RST_WARN\r\n");
        if (vw & kESPI_VWireRd_HostRstWarn)
            (void)PRINTF(" - HOST_RST_WARN\r\n");
        if (vw & kESPI_VWireRd_SusWarn)
            (void)PRINTF(" - SUS_WARN\r\n");
        if (vw & kESPI_VWireRd_SusPwrdnAck)
            (void)PRINTF(" - SUS_PWRDN_ACK\r\n");
        if (vw & kESPI_VWireRd_SlpAN)
            (void)PRINTF(" - SLP_AN\r\n");
        if (vw & kESPI_VWireRd_SlpLAN)
            (void)PRINTF(" - SLP_LAN\r\n");
        if (vw & kESPI_VWireRd_SlpWLAN)
            (void)PRINTF(" - SLP_WLAN\r\n");
        if (vw & kESPI_VWireRd_P2E)
            (void)PRINTF(" - P2E group set\r\n");
        if (vw & kESPI_VWireRd_HostC10N)
            (void)PRINTF(" - HOST_C10N\r\n");
    }

    if (status & kESPI_IrqUpdateFlag)
    {
        (void)PRINTF("IRQ update completed\r\n");
    }

    if (status & kESPI_Port80InterruptFlag)
    {
        espi_p80_status_t p80Stat;
        result = ESPI_GetPort80Status(base, &p80Stat);
        if (result == kStatus_Fail)
        {
            (void)PRINTF("P80 is not enabled!\r\n");
        }
        else
        {
            (void)PRINTF("P80 Code 0x%02X (prev: 0x%02X, count: %u)\r\n", p80Stat.currentCode, p80Stat.previousCode,
                         p80Stat.counter);
        }
    }
}

static void ESPI_PortCallback(ESPI_Type *base, espi_handle_t *handle, uint32_t port, uint32_t status, void *userData)
{
    (void)userData;

#if EXAMPLE_ESPI_DEBUG_LOG
    (void)PRINTF("\r\nPort %u interrupt status 0x%08X.\r\n", port, status);
#endif

    if ((status & kESPI_PortErrorInterrupt) != 0U)
    {
        espi_port_error_t error;
        ESPI_GetPortErrorStatus(base, port, status, &error);
        (void)PRINTF("Port %u error code %u\r\n", port, error);

        switch (error)
        {
            case (uint32_t)kESPI_PortError_EndpointWriteOverrun:
                (void)PRINTF(" - Endpoint Write Overrun: Host wrote when WRDY=1.\r\n");
                break;
            case (uint32_t)kESPI_PortError_EndpointInvalidSize:
                (void)PRINTF(" - Endpoint Invalid Size: Transfer size > 1 byte.\r\n");
                break;
            case (uint32_t)kESPI_PortError_MailboxInvalidAccess:
                (void)PRINTF(" - Mailbox Invalid Access: Invalid host read/write access.\r\n");
                break;
            case (uint32_t)kESPI_PortError_MailboxOverrunUnderrun:
                (void)PRINTF(" - Mailbox Overrun/Underrun: Write overrun or read underrun.\r\n");
                break;
            case (uint32_t)kESPI_PortError_MailboxSizeOverflow:
                (void)PRINTF(" - Mailbox Size Overflow: Request size exceeds mailbox boundary.\r\n");
                break;
            case (uint32_t)kESPI_PortError_MailboxRAMBusError:
                (void)PRINTF(" - Mailbox RAM/Bus Error: AHB/RAM access error.\r\n");
                break;
            case (uint32_t)kESPI_PortError_MasterEraseFailed:
                (void)PRINTF(" - Flash erase failed.\r\n");
                break;
            default:
                break;
        }
    }

    if (port == handle->oobPort)
    {
        if (status & kESPI_PortReadInterrupt)
        {
            (void)PRINTF("OOB sent over.\r\n");
        }
        if (status & kESPI_PortWriteInterrupt)
        {
            uint32_t len = ESPI_GetPortMailboxSize(base, port);
            ESPI_ReadOOB(base, &g_espiHandle, g_msgBuffer, &len);
            (void)PRINTF("OOB received (%u bytes): ", len);
            for (uint32_t i = 0; (i < len) && (i < sizeof(g_msgBuffer)); i++)
                (void)PRINTF("%02X ", g_msgBuffer[i]);
            (void)PRINTF("\r\n");
        }
    }
    else if (port == g_espiHandle.safPort)
    {
#if EXAMPLE_ESPI_DEBUG_LOG
        (void)PRINTF("SAF event.\r\n");
#endif
    }
    else
    {
        espi_port_type_t type = ESPI_GetPortType(base, port);
        uint32_t idx, data;

        if (type == kESPI_PortType_ACPIEndpoint)
        {
            if ((status & kESPI_PortWriteInterrupt) != 0U)
            {
                ESPI_GetEndpointData(base, port, &idx, &data);
                (void)PRINTF("Received endpoint message: idx = 0x%X, datain = 0x%X\r\n", idx, data);
                /* Send data back. */
                if (idx == 0U)
                {
                    ESPI_WritePortData(base, port, data);
                    (void)PRINTF("Endpoint data ready: 0x%X\r\n", data);
                }
            }
            if ((status & kESPI_PortReadInterrupt) != 0U)
            {
                ESPI_GetEndpointData(base, port, &idx, &data);
                (void)PRINTF("Endpoint data read: idx = 0x%X\r\n", idx);
            }
        }
        else if (type == kESPI_PortType_ACPIIndexData)
        {
            if ((status & kESPI_PortSpec0Interrupt) != 0U)
            {
                ESPI_GetEndpointData(base, port, &idx, &data);
                (void)PRINTF("Received Index-data message: idx = 0x%X, datain = 0x%X\r\n", idx, data);
            }
            if ((status & kESPI_PortReadInterrupt) != 0U)
            {
                ESPI_GetEndpointData(base, port, &idx, &data);
                ESPI_WritePortData(base, port, 0xBB);
                (void)PRINTF("Index-data sent back: idx = 0x%X, dataout =0x%X\r\n", idx, 0xBB);
            }
        }
        /* Mailbox ports. */
        else
        {
            if ((status & (kESPI_PortWriteInterrupt | kESPI_PortSpec0Interrupt)) != 0U)
            {
                uint8_t *ramBuf = ESPI_GetPortRamBuffer(base, port);
                uint32_t msgLen = ESPI_GetPortMsgLen(base, port);

                if (msgLen > sizeof(g_msgBuffer))
                    msgLen = sizeof(g_msgBuffer);
                (void)memcpy(g_msgBuffer, ramBuf, msgLen);
                (void)PRINTF("Mailbox received (%u bytes): ", msgLen);
                for (uint32_t i = 0; i < msgLen; i++)
                    (void)PRINTF("%02X ", g_msgBuffer[i]);
                (void)PRINTF("\r\n");

                base->PORT[port].IRULESTAT = (base->PORT[port].IRULESTAT & ~ESPI_IRULESTAT_SSTCL_MASK) |
                                             ESPI_IRULESTAT_SSTCL((uint32_t)kESPI_SSTCL_MailboxWrEmpty);
            }
            if (status & kESPI_PortReadInterrupt)
            {
                if ((status & kESPI_PortSpec1Interrupt) != 0U)
                {
                    (void)PRINTF("Mailbox read started.\r\n");
                }
                if ((status & kESPI_PortSpec3Interrupt) != 0U)
                {
                    (void)PRINTF("Mailbox read done.\r\n");
                }
            }
        }
    }
}

void print_help(void)
{
    (void)PRINTF("\r\nInteractive commands:\r\n");
    (void)PRINTF(" show_config               -- Show eSPI configuration\r\n");
    (void)PRINTF(" status                    -- Show eSPI status flags\r\n");
    (void)PRINTF(" send_vw_mask <hexmask>    -- Apply VW by mask (32-bit hex)\r\n");
    (void)PRINTF(" send_vw_flag <name> <val> -- Set VW flag by name (val may be multi-bit)\r\n");
    (void)PRINTF(" vw_flags                  -- List available VW flag names\r\n");
    (void)PRINTF(" send_oob <hexbytes>       -- Send OOB payload (hex, e.g. AA55 or 0xAA 0x55)\r\n");
    (void)PRINTF(" push_irq <num>            -- Push IRQ (0-255) to host\r\n");
    (void)PRINTF(" reset_p80                 -- Reset Port 80 counter\r\n");
    (void)PRINTF(" help                      -- Help\r\n\r\n");
}

int main(void)
{
    espi_callback_config_t espi_callback = {.commonCallback = ESPI_CommonCallback, .portCallback = ESPI_PortCallback};

    BOARD_InitHardware();

    (void)PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    (void)PRINTF("\r\nInteractive eSPI device example.\r\n");
    (void)PRINTF("\r\nCLOCK_GetEspiClkFreq = %u.\r\n", CLOCK_GetEspiClkFreq());

    ESPI_GetDefaultConfig(&g_espi_config);
    g_espi_config.ramBaseAddr    = EXAMPLE_RAM_BASE;
    g_espi_config.base0Addr      = 0x1000U;
    g_espi_config.base1Addr      = 0x2000U;
    g_espi_config.portConfig     = g_portCfg;
    g_espi_config.portCount      = ARRAY_SIZE(g_portCfg);
    g_espi_config.enableAlertPin = true;
    g_espi_config.enableOOB      = true;
    g_espi_config.enableSAF      = true;
    g_espi_config.enableP80      = true;

    ESPI_Init(EXAMPLE_ESPI_BASE, &g_espi_config);
    ESPI_CreateHandle(EXAMPLE_ESPI_BASE, &g_espiHandle, &g_espi_config, &espi_callback, NULL);
    ESPI_FlashCreateHandle(EXAMPLE_ESPI_BASE, &g_espiHandle, ExampleFlashOps, sizeof(g_virtualFlash));
    ESPI_EnableInterrupts(EXAMPLE_ESPI_BASE, kESPI_AllInterruptEnable);
    for (uint32_t i = 0; i < ESPI_PORT_COUNT; i++)
    {
        ESPI_EnablePortInterrupts(EXAMPLE_ESPI_BASE, i, kESPI_PortAllInterrupts);
    }

    print_help();

    while (1)
    {
        (void)PRINTF("> ");
        int n = read_line(g_cmdLine, sizeof(g_cmdLine));
        if (n <= 0)
            continue;
        char *cmd = strtok(g_cmdLine, " \t\r\n");
        if (!cmd)
            continue;

        char *args = strtok(NULL, "");
        if (args)
        {
            while (*args && isspace((unsigned char)*args))
                args++;
        }

        if ((strcmp(cmd, "help") == 0) || (strcmp(cmd, "h") == 0))
        {
            print_help();
            continue;
        }
        else if (strcmp(cmd, "show_config") == 0)
        {
            print_espi_config(EXAMPLE_ESPI_BASE);
            continue;
        }
        else if (strcmp(cmd, "status") == 0)
        {
            (void)PRINTF("eSPI status: 0x%08X\r\n", ESPI_GetStatusFlags(EXAMPLE_ESPI_BASE));
            continue;
        }
        else if (strcmp(cmd, "send_vw_mask") == 0)
        {
            if (!args || !*args)
            {
                (void)PRINTF("Missing mask\r\n");
                continue;
            }
            uint32_t mask             = (uint32_t)strtoul(args, NULL, 0);
            EXAMPLE_ESPI_BASE->WIREWO = mask;
            (void)PRINTF("VW sends as mask 0x%08X\r\n", mask);
            continue;
        }
        else if (strcmp(cmd, "send_vw_flag") == 0)
        {
            char flag_name[32];
            unsigned int val = 0U;
            if (!args || (sscanf(args, "%31s %u", flag_name, &val) != 2))
            {
                (void)PRINTF("Usage: send_vw_flag <name> <val>\r\n");
                ESPI_PrintVWireFlagList();
                continue;
            }

            espi_vw_wr_flags_t flag;
            if (!ESPI_ParseVWireFlagName(flag_name, &flag))
            {
                (void)PRINTF("Unknown VW flag name: %s\r\n", flag_name);
                ESPI_PrintVWireFlagList();
                continue;
            }

            status_t res = ESPI_SendVWire(EXAMPLE_ESPI_BASE, flag, val);
            (void)PRINTF("\r\nESPI_SendVWire(%s, val=%u) -> %d\r\n", flag_name, val, res);
            continue;
        }
        else if (strcmp(cmd, "vw_flags") == 0)
        {
            ESPI_PrintVWireFlagList();
            continue;
        }
        else if (strcmp(cmd, "send_oob") == 0)
        {
            if (!args || !*args)
            {
                (void)PRINTF("No OOB data provided\r\n");
                continue;
            }
            uint32_t enc_oob =
                (EXAMPLE_ESPI_BASE->PORT[g_espiHandle.oobPort].RAMUSE & ESPI_RAMUSE_LEN_MASK) >> ESPI_RAMUSE_LEN_SHIFT;
            uint32_t maxOob = (uint32_t)(4U << enc_oob);
            if (maxOob > sizeof(g_msgBuffer))
                maxOob = sizeof(g_msgBuffer);
            int len = parse_hex_bytes(args, g_msgBuffer, (int)maxOob);
            if (len < 0)
            {
                (void)PRINTF("Invalid hex input (use two chars per byte, e.g. AA BB)\r\n");
                continue;
            }
            if (len == 0)
            {
                (void)PRINTF("No OOB data parsed\r\n");
                continue;
            }
            if ((uint32_t)len > maxOob)
            {
                (void)PRINTF("OOB data too long (max %u bytes)\r\n", maxOob);
                continue;
            }
            status_t result = ESPI_SendOOB(EXAMPLE_ESPI_BASE, &g_espiHandle, g_msgBuffer, (uint32_t)len, true);
            (void)PRINTF("ESPI_SendOOB -> %d (len=%d)\r\n", result, len);
            continue;
        }
        else if (strcmp(cmd, "push_irq") == 0)
        {
            if (!args || !*args)
            {
                (void)PRINTF("Missing IRQ number (0-255)\r\n");
                (void)PRINTF("Usage: push_irq <num>\r\n");
                continue;
            }

            int irq_num = atoi(args);
            if (irq_num < 0 || irq_num > 255)
            {
                (void)PRINTF("Invalid IRQ number: %d (must be 0-255)\r\n", irq_num);
                continue;
            }

            ESPI_PushIrq(EXAMPLE_ESPI_BASE, irq_num);

            continue;
        }
        else if (strcmp(cmd, "reset_p80") == 0)
        {
            ESPI_ResetPort80Counter(EXAMPLE_ESPI_BASE);
            (void)PRINTF("Port 80 counter reset.\r\n");
            continue;
        }
        else
        {
            (void)PRINTF("Unknown command. Type 'help' for help.\r\n");
        }
    }
}
