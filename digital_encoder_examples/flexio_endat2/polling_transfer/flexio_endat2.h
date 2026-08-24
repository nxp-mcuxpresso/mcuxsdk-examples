/*
 * Copyright 2024-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLEXIO_ENDAT2_H_
#define _FLEXIO_ENDAT2_H_

#include "fsl_flexio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FLEXIO_ENDAT2_CMD_LEN           6u
#define FLEXIO_ENDAT2_CMD_CLK_LEN       (2u + FLEXIO_ENDAT2_CMD_LEN + 1)
#define FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB (32u - 1u - FLEXIO_ENDAT2_CMD_LEN)
#define FLEXIO_ENDAT2_CRC_LEN           5u
#define FLEXIO_ENDAT2_CMD_NUM           2u

#define FLEXIO_ENDAT2_CMD_ENCSENDPOSVAL (0x07 << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)
// #define FLEXIO_ENDAT2_CMD_SELECTMEM                 (0x0E << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)
// #define FLEXIO_ENDAT2_CMD_ENCRECVPARA               (0x1C << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)
// #define FLEXIO_ENDAT2_CMD_ENCSENDPARA               (0x23 << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)
#define FLEXIO_ENDAT2_CMD_ENCRECVRST (0x2A << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)
// #define FLEXIO_ENDAT2_CMD_ENCSENDTESTVAL            (0x15 << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)
// #define FLEXIO_ENDAT2_CMD_ENCRECVTESTCMD            (0x31 << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)
// #define FLEXIO_ENDAT2_CMD_ENCSENDPOSVALADDDAT       (0x38 << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)
// #define FLEXIO_ENDAT2_CMD_ENCSENDPOSVALRECVSELMEM   (0x09 << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)
// #define FLEXIO_ENDAT2_CMD_ENCSENDPOSVALRECVPARA     (0x1B << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)
// #define FLEXIO_ENDAT2_CMD_ENCSENDPOSVALSENDPARA     (0x24 << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)
// #define FLEXIO_ENDAT2_CMD_ENCSENDPOSVALRECVERRRST   (0x2D << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)
// #define FLEXIO_ENDAT2_CMD_ENCSENDPOSVALRECVTESTCMD  (0x36 << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)
// #define FLEXIO_ENDAT2_CMD_ENCRECVCOMMCMD            (0x12 << FLEXIO_ENDAT2_CMD_SHIFT_FOR_MSB)

typedef enum
{
    kFlexIO_ENDAT2_cmd_idx_EncSendPosVal = 0u,
    // kFlexIO_ENDAT2_cmd_idx_SelectMem,
    // kFlexIO_ENDAT2_cmd_idx_EncRecvPara,
    // kFlexIO_ENDAT2_cmd_idx_EncSendPara,
    kFlexIO_ENDAT2_cmd_idx_EncRecvRst,
    // kFlexIO_ENDAT2_cmd_idx_EncSendTestVal,
    // kFlexIO_ENDAT2_cmd_idx_EncRecvTestCmd,
    // kFlexIO_ENDAT2_cmd_idx_EncSendPosValAddDat,
    // kFlexIO_ENDAT2_cmd_idx_EncSendPosValRecvSelMem,
    // kFlexIO_ENDAT2_cmd_idx_EncSendPosValRecvPara,
    // kFlexIO_ENDAT2_cmd_idx_EncSendPosValSendPara,
    // kFlexIO_ENDAT2_cmd_idx_EncSendPosValRecvErrRst,
    // kFlexIO_ENDAT2_cmd_idx_EncSendPosValRecvTestCmd,
    // kFlexIO_ENDAT2_cmd_idx_EncRecvCommCmd,
} flexio_endat2_cmd_index_t;

typedef struct _flexio_endat2_cmd_cfg_t
{
    uint32_t cmd_val;
    uint16_t dir_timer_val;
    uint16_t rxd_timer_val;
} flexio_endat2_cmd_cfg_t;

/*! @brief Define FlexIO ENDAT2 access structure typedef. */
typedef struct _flexio_endat2_type
{
    FLEXIO_Type              *flexio;          /*!< FlexIO base pointer. */
    uint8_t                   txdPinIdx;       /*!< FlexIO pin select for data output. */
    uint8_t                   rxdPinIdx;       /*!< FlexIO pin select for data input. */
    uint8_t                   clkPinIdx;       /*!< FlexIO pin select for clock. */
    uint8_t                   dirPinIdx;       /*!< FlexIO pin select for direction. */
    uint8_t                   shifterStartIdx; /*!< FlexIO shifter start index. */
    uint8_t                   timerStartIdx;   /*!< FlexIO timer start index. */
    uint8_t                   rxShifterNum;    /*!< Shifter count used for receiving data from encoder. */
    bool                      hwTrigger;       /*!< FlexIO TX timer trigger source by hardware or software. */
    uint32_t                  baudRateBps;     /*!< BISSC communication baud rate in bits per second. */
    flexio_endat2_cmd_cfg_t   cmd_cfg[FLEXIO_ENDAT2_CMD_NUM];
    flexio_endat2_cmd_index_t current_cmd_idx;
    uint32_t                  rxdBuffer[2];   /*!< Raw data received from SL. */
    volatile bool             rxdBufferReady; /*!< RXD buffer ready or not. */
    uint16_t                  mtLen;          /*!< Bit length of Multi-Turn. */
    uint16_t                  stLen;          /*!< Bit length of Single-Turn. */
    uint32_t                  mt;             /*!< Multi-Turn value. */
    uint32_t                  st;             /*!< Single-Turn value. */
    uint8_t                   error1Bit;      /*!< error1 bit value. */
    uint8_t                   error2Bit;      /*!< error2 bit value. */
    bool                      crcMatch;       /*!< CRC check if matched. */
} FLEXIO_ENDAT2_Type;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C"
{
#endif /*_cplusplus*/

    int  FLEXIO_ENDAT2_Init(FLEXIO_ENDAT2_Type *base, uint32_t srcClock_Hz);
    void FLEXIO_ENDAT2_EnableRxInterrupt(FLEXIO_ENDAT2_Type *base, bool enable);
    void FLEXIO_ENDAT2_CmdPreset(FLEXIO_ENDAT2_Type *base, flexio_endat2_cmd_index_t cmd_idx);
    void FLEXIO_ENDAT2_SwTrigger(FLEXIO_ENDAT2_Type *base);
    int  FLEXIO_ENDAT2_DataParser(FLEXIO_ENDAT2_Type *base);
    int  FLEXIO_ENDAT2_ReadBlocking(FLEXIO_ENDAT2_Type *base);
    void FLEXIO_ENDAT2_IRQHandler(void);

#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif /*_FLEXIO_ENDAT2_H_*/
