/*
 * Copyright 2024-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MIMXRT2660EVK_XSPI_NOR_CONFIG__
#define __MIMXRT2660EVK_XSPI_NOR_CONFIG__

#include <stdint.h>
#include <stdbool.h>

/*! @name Driver version */
/*@{*/
/*! @brief XIP_BOARD driver version 1.0.0. */
#define FSL_XIP_BOARD_DRIVER_VERSION (0x10000)
/*@}*/

#define XSPI_CFG_BLK_TAG     (0x42464346UL) /* "FCFB" */
#define XSPI_CFG_BLK_VERSION (0x56010400UL) /* V1.4.0 */
#define XSPI_CFG_BLK_SIZE    (552)

#define XSPI_FEATURE_HAS_PARALLEL_MODE 1

#define CMD_INDEX_READ        0
#define CMD_INDEX_READSTATUS  1
#define CMD_INDEX_WRITEENABLE 2
#define CMD_INDEX_WRITE       4

#define CMD_LUT_SEQ_IDX_READ        0
#define CMD_LUT_SEQ_IDX_READSTATUS  1
#define CMD_LUT_SEQ_IDX_WRITEENABLE 3
#define CMD_LUT_SEQ_IDX_WRITE       9

#define STOP         0x00
#define CMD          0x01
#define ADDR         0x02
#define DUMMY        0x03
#define MODE         0x04
#define MODE2        0x05
#define MODE4        0x06
#define READ         0x07
#define WRITE        0x08
#define JMP_ON_CS    0x09
#define ADDR_DDR     0x0A
#define MODE_DDR     0x0B
#define MODE2_DDR    0x0C
#define MODE4_DDR    0x0D
#define READ_DDR     0x0E
#define WRITE_DDR    0x0F
#define DATA_LEARN   0x10
#define CMD_DDR      0x11
#define CADDR        0x12
#define CADDR_DDR    0x13
#define JMP_TO_SEQ   0x14
#define CMD_READ     0x15
#define CMD_READ_DDR 0x16
#define CMD_RESET    0x17

#define XSPI_1PAD 0
#define XSPI_2PAD 1
#define XSPI_4PAD 2
#define XSPI_8PAD 3

#define XSPI_LUT_OPERAND0(op)  (((uint32_t)(op) & 0xFFU) << 0)
#define XSPI_LUT_NUM_PADS0(p)  (((uint32_t)(p)  & 0x3U)  << 8)
#define XSPI_LUT_OPCODE0(c)    (((uint32_t)(c)  & 0x3FU) << 10)
#define XSPI_LUT_OPERAND1(op)  (((uint32_t)(op) & 0xFFU) << 16)
#define XSPI_LUT_NUM_PADS1(p)  (((uint32_t)(p)  & 0x3U)  << 24)
#define XSPI_LUT_OPCODE1(c)    (((uint32_t)(c)  & 0x3FU) << 26)

#define XSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)                                                    \
    (XSPI_LUT_OPERAND0(op0) | XSPI_LUT_NUM_PADS0(pad0) | XSPI_LUT_OPCODE0(cmd0) | XSPI_LUT_OPERAND1(op1) | \
     XSPI_LUT_NUM_PADS1(pad1) | XSPI_LUT_OPCODE1(cmd1))

typedef enum
{
    kxSpiSerialClk_30MHz  = 1,
    kxSpiSerialClk_50MHz  = 2,
    kxSpiSerialClk_60MHz  = 3,
    kxSpiSerialClk_80MHz  = 4,
    kxSpiSerialClk_100MHz = 5,
    kxSpiSerialClk_120MHz = 6,
    kxSpiSerialClk_133MHz = 7,
    kxSpiSerialClk_166MHz = 8,
} xspi_serial_clk_freq_t;

typedef enum
{
    kxSpiReadSampleClk_LoopbackInternally      = 0,
    kxSpiReadSampleClk_LoopbackFromDqsPad      = 1,
    kxSpiReadSampleClk_ExternalInputFromDqsPad = 3,
} xspi_read_sample_clk_t;

enum
{
    kxSpiMiscOffset_DiffClkEnable            = 0,
    kxSpiMiscOffset_Ck2Enable                = 1,
    kxSpiMiscOffset_ParallelEnable           = 2,
    kxSpiMiscOffset_WordAddressableEnable    = 3,
    kxSpiMiscOffset_SafeConfigFreqEnable     = 4,
    kxSpiMiscOffset_PadSettingOverrideEnable = 5,
    kxSpiMiscOffset_DdrModeEnable            = 6,
    kxSpiMiscOffset_SecondPinGroup           = 8,
    kxSpiMiscOffset_SecondDqsPinGroup        = 9,
    kxSpiMiscOffset_WriteMaskEnable          = 10,
    kxSpiMiscOffset_WriteOpt1Clear           = 11,
};

enum
{
    kxSpiDeviceType_SerialNOR  = 1,
    kxSpiDeviceType_SerialNAND = 2,
    kxSpiDeviceType_SerialRAM  = 3,
};

enum
{
    kSerialFlash_1Pad  = 1,
    kSerialFlash_2Pads = 2,
    kSerialFlash_4Pads = 4,
    kSerialFlash_8Pads = 8,
};

typedef struct
{
    uint8_t  seqNum;
    uint8_t  seqId;
    uint16_t reserved;
} xspi_lut_seq_t;

enum
{
    kDeviceConfigCmdType_Generic    = 0,
    kDeviceConfigCmdType_QuadEnable = 1,
    kDeviceConfigCmdType_Spi2Xpi    = 2,
    kDeviceConfigCmdType_Xpi2Spi    = 3,
    kDeviceConfigCmdType_Spi2NoCmd  = 4,
};

typedef struct
{
    uint32_t tag;                    /* [0x000] = XSPI_CFG_BLK_TAG */
    uint32_t version;                /* [0x004] */
    uint32_t reserved0;              /* [0x008] */
    uint8_t  readSampleClkSrc;       /* [0x00c] */
    uint8_t  csHoldTime;             /* [0x00d] */
    uint8_t  csSetupTime;            /* [0x00e] */
    uint8_t  columnAddressWidth;     /* [0x00f] */
    uint8_t  deviceModeCfgEnable;    /* [0x010] */
    uint8_t  deviceModeType;         /* [0x011] */
    uint16_t waitTimeCfgCommands;    /* [0x012] */
    xspi_lut_seq_t deviceModeSeq;    /* [0x014] */
    uint32_t deviceModeArg;          /* [0x018] */
    uint8_t  configCmdEnable;        /* [0x01c] */
    uint8_t  reserved1[3];           /* [0x01d] */
    xspi_lut_seq_t configCmdSeqs[3]; /* [0x020] */
    uint8_t  reserved2[2];           /* [0x02c-0x02d] */
    uint8_t  maxCsLowInterval;       /* [0x02e] */
    uint8_t  ahbAlignment;           /* [0x02f] */
    uint32_t cfgCmdArgs[3];          /* [0x030] */
    uint8_t  ahbSplitEn;             /* [0x03c] */
    uint8_t  reserved3[3];           /* [0x03d] */
    uint32_t controllerMiscOption;   /* [0x040] */
    uint8_t  deviceType;             /* [0x044] */
    uint8_t  sflashPadType;          /* [0x045] */
    uint8_t  serialClkFreq;          /* [0x046] */
    uint8_t  lutCustomSeqEnable;     /* [0x047] */
    uint32_t reserved4[2];           /* [0x048] */
    uint32_t sflashA1Size;           /* [0x050] */
    uint32_t sflashA2Size;           /* [0x054] */
    uint32_t sflashB1Size;           /* [0x058] */
    uint32_t sflashB2Size;           /* [0x05c] */
    uint32_t csPadSettingOverride;   /* [0x060] */
    uint32_t sclkPadSettingOverride; /* [0x064] */
    uint32_t dataPadSettingOverride; /* [0x068] */
    uint32_t dqsPadSettingOverride;  /* [0x06c] */
    uint32_t timeoutInMs;            /* [0x070] */
    uint32_t commandInterval;        /* [0x074] */
    uint16_t dataValidTime[2];       /* [0x078] */
    uint16_t busyOffset;             /* [0x07c] */
    uint16_t busyBitPolarity;        /* [0x07e] */
    /* lookupTable size is a fixed ROM API value and does not reflect the hardware LUT register count. */
    uint32_t lookupTable[90];        /* [0x080-0x1e7] 360 bytes */
    xspi_lut_seq_t lutCustomSeq[12]; /* [0x1e8-0x217] 48 bytes */
    uint32_t dllCrVal;               /* [0x218] DLLCR setting */
    uint32_t smprVal;                /* [0x21c] SMPR setting */
    uint32_t reserved5[2];           /* [0x220-0x227] */
} xspi_mem_config_t;

#define NOR_CMD_INDEX_READ        CMD_INDEX_READ
#define NOR_CMD_INDEX_READSTATUS  CMD_INDEX_READSTATUS
#define NOR_CMD_INDEX_WRITEENABLE CMD_INDEX_WRITEENABLE
#define NOR_CMD_INDEX_ERASESECTOR 3
#define NOR_CMD_INDEX_PAGEPROGRAM CMD_INDEX_WRITE
#define NOR_CMD_INDEX_CHIPERASE   5
#define NOR_CMD_INDEX_DUMMY       6
#define NOR_CMD_INDEX_ERASEBLOCK  7

#define NOR_CMD_LUT_SEQ_IDX_READ            CMD_LUT_SEQ_IDX_READ
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS      CMD_LUT_SEQ_IDX_READSTATUS
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS_XPI  2
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE     CMD_LUT_SEQ_IDX_WRITEENABLE
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_XPI 4
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR     5
#define NOR_CMD_LUT_SEQ_IDX_ERASEBLOCK      8
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM     CMD_LUT_SEQ_IDX_WRITE
#define NOR_CMD_LUT_SEQ_IDX_CHIPERASE       11
#define NOR_CMD_LUT_SEQ_IDX_READ_SFDP       13
#define NOR_CMD_LUT_SEQ_IDX_RESTORE_NOCMD   14
#define NOR_CMD_LUT_SEQ_IDX_EXIT_NOCMD      15

typedef struct
{
    xspi_mem_config_t memConfig;    /* [0x000-0x227] 552 bytes */
    uint32_t pageSize;              /* [0x228] */
    uint32_t sectorSize;            /* [0x22c] */
    uint8_t  ipcmdSerialClkFreq;    /* [0x230] */
    uint8_t  isUniformBlockSize;    /* [0x231] */
    uint8_t  isDataOrderSwapped;    /* [0x232] */
    uint8_t  reserved0;             /* [0x233] */
    uint8_t  serialNorType;         /* [0x234] */
    uint8_t  needExitNoCmdMode;     /* [0x235] */
    uint8_t  halfClkForNonReadCmd;  /* [0x236] */
    uint8_t  needRestoreNoCmdMode;  /* [0x237] */
    uint32_t blockSize;             /* [0x238] */
    uint32_t flashStateCtx;         /* [0x23c] */
    uint32_t reserved1[10];         /* [0x240-0x267] 40 bytes */
} xspi_nor_config_t;

#endif /* __MIMXRT2660EVK_XSPI_NOR_CONFIG__ */
