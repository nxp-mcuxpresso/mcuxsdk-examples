/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLEXIO_BISSC_H_
#define _FLEXIO_BISSC_H_

#include "fsl_flexio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FLEXIO_BISSC_MAX_RX_SHIFTERS (FLEXIO_SHIFTBUF_COUNT - 1U)

typedef struct _flexio_bissc_type
{
    FLEXIO_Type  *flexio;          /*!< FlexIO base pointer. */
    uint8_t       slPinIdx;        /*!< FlexIO pin select for SL data input. */
    uint8_t       maPinIdx;        /*!< FlexIO pin select for MA clock output. */
    uint8_t       shifterStartIdx; /*!< FlexIO shifter start index. */
    uint8_t       timerStartIdx;   /*!< FlexIO timer start index. */
    uint8_t       rxShifterNum;    /*!< Shifter count used for receiving data from encoder. */
    bool          hwTrigger;       /*!< FlexIO MA timer trigger source by hardware or software. */
    bool          rxStartOnSlRisingEdge; /*!< Start RX from SL rising edge, otherwise from MA trigger. */
    uint32_t      baudRateBps;     /*!< BiSS-C communication baud rate in bits per second. */
    uint8_t       mtLen;           /*!< Bit length of multi-turn data. */
    uint8_t       stLen;           /*!< Bit length of single-turn data. */
    uint8_t       ackLen;          /*!< Bit length of ACK field. */
    uint32_t      rxdBuffer[FLEXIO_BISSC_MAX_RX_SHIFTERS];
    volatile bool rxdBufferReady; /*!< RXD buffer ready or not. */
    uint32_t      mt;             /*!< Multi-turn value. */
    uint32_t      st;             /*!< Single-turn value. */
    uint8_t       errorBit;       /*!< Error bit value. */
    uint8_t       warnBit;        /*!< Warning bit value. */
    bool          crcMatch;       /*!< CRC check matched or not. */
} FLEXIO_BISSC_Type;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

int  FLEXIO_BISSC_Init(FLEXIO_BISSC_Type *base, uint32_t srcClock_Hz);
void FLEXIO_BISSC_EnableRxInterrupt(FLEXIO_BISSC_Type *base, bool enable);
void FLEXIO_BISSC_SwTrigger(FLEXIO_BISSC_Type *base);
void FLEXIO_BISSC_DataParser(FLEXIO_BISSC_Type *base);
void FLEXIO_BISSC_ReadBlocking(FLEXIO_BISSC_Type *base);
void FLEXIO_BISSC_IRQHandler(void);

#if defined(__cplusplus)
}
#endif

#endif /* _FLEXIO_BISSC_H_ */
