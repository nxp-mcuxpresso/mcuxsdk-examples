/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "flexio_bissc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define TRG_SHIFTER 0U
#define RXD_SHIFTER 1U

#define MA_TIMER      0U
#define RX_TRIG_TIMER 1U
#define RX_TIMER      2U

#define FLEXIO_BISSC_HANDLE_COUNT 1U

/* Validated HM16 timing: 45 MA clocks and a 39-bit RX window. */
#define FLEXIO_BISSC_MA_FRAME_OVERHEAD 14U
#define FLEXIO_BISSC_RX_FRAME_OVERHEAD 11U

/* BiSS-C CRC6 parameters. Standard BiSS-C: poly 0x43, MSB-first, final XOR 0x3F. */
#define CRC6_POLY_BISSC 0x43U /*!< x^6 + x + 1, MSB-first feed. */
#define CRC6_MASK       0x3FU /*!< 6-bit mask. */
#define CRC6_MSB        0x20U /*!< Most significant bit of 6-bit CRC. */
#define CRC6_FINAL_XOR  0x3FU /*!< Standard BiSS-C inverted output. */

/*******************************************************************************
 * Variables
 ******************************************************************************/

static FLEXIO_BISSC_Type *s_flexioBisscHandle[FLEXIO_BISSC_HANDLE_COUNT];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static uint8_t FLEXIO_BISSC_CRC6(uint32_t *data, uint32_t len);
static void    FLEXIO_BISSC_ArrayRightShift(uint32_t *data, uint32_t wordLen, uint32_t shift);
static uint32_t FLEXIO_BISSC_BitMask(uint8_t width);

/*******************************************************************************
 * Codes
 ******************************************************************************/

int FLEXIO_BISSC_Init(FLEXIO_BISSC_Type *base, uint32_t srcClock_Hz)
{
    assert(base != NULL);
    assert(base->flexio != NULL);

    flexio_config_t         flexioConfig;
    flexio_shifter_config_t shifterConfig = {0U};
    flexio_timer_config_t   timerConfig   = {0U};
    uint32_t                rxBitLen;
    uint16_t                baudDiv;

    if ((srcClock_Hz == 0U) || (base->baudRateBps == 0U))
    {
        return -1;
    }

    baudDiv = (uint16_t)((srcClock_Hz / base->baudRateBps) >> 1U) - 1U;
    if (baudDiv > 0xFFU)
    {
        return -1;
    }

    rxBitLen           = (uint32_t)base->mtLen + (uint32_t)base->stLen + 8U;
    base->rxShifterNum = (uint8_t)((rxBitLen + 31U) >> 5U);
    if ((base->rxShifterNum == 0U) ||
        ((uint32_t)base->shifterStartIdx + RXD_SHIFTER + base->rxShifterNum > FLEXIO_SHIFTBUF_COUNT) ||
        ((uint32_t)base->timerStartIdx + RX_TIMER >= FLEXIO_TIMCTL_COUNT))
    {
        return -1;
    }

    FLEXIO_GetDefaultConfig(&flexioConfig);
    flexioConfig.enableInDebug = true;
    FLEXIO_Init(base->flexio, &flexioConfig);

    /* 1. Configure the shifter used only to generate the software trigger. */
    shifterConfig.timerSelect = base->timerStartIdx + MA_TIMER;
    shifterConfig.pinConfig   = kFLEXIO_PinConfigOutputDisabled;
    shifterConfig.shifterMode = kFLEXIO_ShifterModeTransmit;
    FLEXIO_SetShifterConfig(base->flexio, base->shifterStartIdx + TRG_SHIFTER, &shifterConfig);

    /* 2. Configure the receive shifter chain. The last shifter samples the SL pin. */
    shifterConfig.timerSelect   = base->timerStartIdx + RX_TIMER;
    shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnPositive;
    shifterConfig.pinConfig     = kFLEXIO_PinConfigOutputDisabled;
    shifterConfig.pinSelect     = base->slPinIdx;
    shifterConfig.pinPolarity   = kFLEXIO_PinActiveHigh;
    shifterConfig.shifterMode   = kFLEXIO_ShifterModeReceive;
    shifterConfig.inputSource   = kFLEXIO_ShifterInputFromNextShifterOutput;
    shifterConfig.shifterStop   = kFLEXIO_ShifterStopBitDisable;
    shifterConfig.shifterStart  = kFLEXIO_ShifterStartBitDisabledLoadDataOnEnable;
    for (uint32_t i = 0U; i < (uint32_t)base->rxShifterNum - 1U; i++)
    {
        FLEXIO_SetShifterConfig(base->flexio, base->shifterStartIdx + RXD_SHIFTER + i, &shifterConfig);
    }
    shifterConfig.inputSource = kFLEXIO_ShifterInputFromPin;
    FLEXIO_SetShifterConfig(base->flexio, base->shifterStartIdx + RXD_SHIFTER + base->rxShifterNum - 1U,
                            &shifterConfig);

    /* 3. Configure MA clock timer. */
    if (base->hwTrigger)
    {
        timerConfig.triggerSelect   = 0U;
        timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveHigh;
        timerConfig.triggerSource   = kFLEXIO_TimerTriggerSourceExternal;
    }
    else
    {
        timerConfig.triggerSelect   = FLEXIO_TIMER_TRIGGER_SEL_SHIFTnSTAT(base->shifterStartIdx + TRG_SHIFTER);
        timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
        timerConfig.triggerSource   = kFLEXIO_TimerTriggerSourceInternal;
    }
    timerConfig.pinConfig      = kFLEXIO_PinConfigOutput;
    timerConfig.pinSelect      = base->maPinIdx;
    timerConfig.pinPolarity    = kFLEXIO_PinActiveLow;
    timerConfig.timerMode      = kFLEXIO_TimerModeDual8BitBaudBit;
    timerConfig.timerOutput    = kFLEXIO_TimerOutputZeroNotAffectedByReset;
    timerConfig.timerDecrement = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    timerConfig.timerReset     = kFLEXIO_TimerResetNever;
    timerConfig.timerDisable   = kFLEXIO_TimerDisableOnTimerCompare;
    timerConfig.timerEnable    = kFLEXIO_TimerEnableOnTriggerHigh;
    timerConfig.timerStop      = kFLEXIO_TimerStopBitDisabled;
    timerConfig.timerStart     = kFLEXIO_TimerStartBitDisabled;
    timerConfig.timerCompare =
        (((((uint32_t)base->ackLen + (uint32_t)base->mtLen + (uint32_t)base->stLen +
            FLEXIO_BISSC_MA_FRAME_OVERHEAD)
           << 1U) -
          1U)
         << 8U) |
        baudDiv;
    FLEXIO_SetTimerConfig(base->flexio, base->timerStartIdx + MA_TIMER, &timerConfig);

    /* 4. Configure helper timer used as RX trigger. */
    timerConfig.pinConfig      = kFLEXIO_PinConfigOutputDisabled;
    timerConfig.timerMode      = kFLEXIO_TimerModeSingle16Bit;
    timerConfig.timerOutput    = kFLEXIO_TimerOutputOneNotAffectedByReset;
    timerConfig.timerDecrement = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    timerConfig.timerReset     = kFLEXIO_TimerResetNever;
    timerConfig.timerDisable   = kFLEXIO_TimerDisableOnPreTimerDisable;
    timerConfig.timerEnable    = kFLEXIO_TimerEnableOnPrevTimerEnable;
    timerConfig.timerStop      = kFLEXIO_TimerStopBitDisabled;
    timerConfig.timerStart     = kFLEXIO_TimerStartBitDisabled;
    timerConfig.timerCompare   = 0xFFFFU;
    FLEXIO_SetTimerConfig(base->flexio, base->timerStartIdx + RX_TRIG_TIMER, &timerConfig);

    /* 5. Configure SL sample timer. */
    timerConfig.triggerSelect   = FLEXIO_TIMER_TRIGGER_SEL_TIMn(base->timerStartIdx + RX_TRIG_TIMER);
    timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveHigh;
    timerConfig.triggerSource   = kFLEXIO_TimerTriggerSourceInternal;
    timerConfig.pinConfig       = kFLEXIO_PinConfigOutputDisabled;
    timerConfig.pinSelect       = base->slPinIdx;
    timerConfig.pinPolarity     = kFLEXIO_PinActiveHigh;
    timerConfig.timerMode       = kFLEXIO_TimerModeDual8BitBaudBit;
    timerConfig.timerOutput     = kFLEXIO_TimerOutputZeroNotAffectedByReset;
    timerConfig.timerDecrement  = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    timerConfig.timerReset      = kFLEXIO_TimerResetNever;
    timerConfig.timerDisable    = kFLEXIO_TimerDisableOnTimerCompare;
    timerConfig.timerEnable     = base->rxStartOnSlRisingEdge ? kFLEXIO_TimerEnableOnPinRisingEdgeTriggerHigh :
                                                                 kFLEXIO_TimerEnableOnTriggerHigh;
    timerConfig.timerStop       = kFLEXIO_TimerStopBitDisabled;
    timerConfig.timerStart      = kFLEXIO_TimerStartBitDisabled;
    timerConfig.timerCompare =
        (((((uint32_t)base->mtLen + (uint32_t)base->stLen + FLEXIO_BISSC_RX_FRAME_OVERHEAD) << 1U) - 1U)
         << 8U) |
        baudDiv;
    FLEXIO_SetTimerConfig(base->flexio, base->timerStartIdx + RX_TIMER, &timerConfig);

    base->rxdBufferReady = false;
    s_flexioBisscHandle[0] = base;

    return 0;
}

void FLEXIO_BISSC_EnableRxInterrupt(FLEXIO_BISSC_Type *base, bool enable)
{
    assert(base != NULL);

    if (enable)
    {
        base->flexio->SHIFTSIEN |= 1UL << (base->shifterStartIdx + RXD_SHIFTER);
    }
    else
    {
        base->flexio->SHIFTSIEN &= ~(1UL << (base->shifterStartIdx + RXD_SHIFTER));
    }
}

void FLEXIO_BISSC_SwTrigger(FLEXIO_BISSC_Type *base)
{
    assert(base != NULL);

    for (uint32_t i = 0U; i < base->rxShifterNum; i++)
    {
        FLEXIO_ClearShifterStatusFlags(base->flexio, 1UL << (base->shifterStartIdx + RXD_SHIFTER + i));
    }
    FLEXIO_ClearTimerStatusFlags(base->flexio, (1UL << (base->timerStartIdx + MA_TIMER)) |
                                                   (1UL << (base->timerStartIdx + RX_TRIG_TIMER)) |
                                                   (1UL << (base->timerStartIdx + RX_TIMER)));

    base->flexio->SHIFTBUF[base->shifterStartIdx + TRG_SHIFTER] = 0U;
}

static uint8_t FLEXIO_BISSC_CRC6(uint32_t *data, uint32_t len)
{
    /*
     * BiSS-C CRC6 matched to the live HM16 encoder (poly 0x43, MSB-first,
     * final XOR with CRC6_FINAL_XOR = 0x3F). The receive buffer stores the field
     * payload with WARN at bit 0 and MT at the most significant bit. len is the
     * payload width (mtLen+stLen+2), always <= 30 bits, so a single 64-bit word
     * holds the whole payload.
     */
    uint64_t payload;
    uint64_t mask;
    uint8_t  crc = 0U;

    if ((len == 0U) || (len > 64U))
    {
        return 0U;
    }

    payload = (uint64_t)data[0];
    if (len > 32U)
    {
        payload |= (uint64_t)data[1] << 32U;
    }

    mask = 1ULL << (len - 1U);

    for (uint32_t i = 0U; i < len; i++)
    {
        uint8_t bit      = ((payload & mask) != 0U) ? 1U : 0U;
        uint8_t feedback = ((crc & CRC6_MSB) != 0U) ? 1U : 0U;

        mask >>= 1U;
        feedback ^= bit;
        crc = (uint8_t)((crc << 1U) & CRC6_MASK);
        if (feedback != 0U)
        {
            crc ^= CRC6_POLY_BISSC;
        }
    }

    crc ^= CRC6_FINAL_XOR;

    return (uint8_t)(crc & CRC6_MASK);
}

static void FLEXIO_BISSC_ArrayRightShift(uint32_t *data, uint32_t wordLen, uint32_t shift)
{
    if ((wordLen != 0U) && (shift != 0U))
    {
        uint32_t wordShift = shift >> 5U;
        uint32_t bitShift  = shift & 31U;

        for (uint32_t i = 0U; i < wordLen; i++)
        {
            uint32_t newVal = 0U;

            if ((i + wordShift) < wordLen)
            {
                newVal = data[i + wordShift] >> bitShift;

                if ((bitShift != 0U) && ((i + wordShift + 1U) < wordLen))
                {
                    newVal |= data[i + wordShift + 1U] << (32U - bitShift);
                }
            }

            data[i] = newVal;
        }
    }
}

static uint32_t FLEXIO_BISSC_BitMask(uint8_t width)
{
    return (width >= 32U) ? 0xFFFFFFFFUL : ((1UL << width) - 1UL);
}

static void FLEXIO_BISSC_ProcessRxWindow(FLEXIO_BISSC_Type *base)
{
    assert(base != NULL);

    /* The validated 39-bit HM16 window includes one bit after the CRC LSB. */
    FLEXIO_BISSC_ArrayRightShift(base->rxdBuffer, base->rxShifterNum, 1U);
}

void FLEXIO_BISSC_DataParser(FLEXIO_BISSC_Type *base)
{
    assert(base != NULL);

    uint32_t *data = base->rxdBuffer;
    uint8_t   crcRecv;
    uint8_t   crcCal;

    crcRecv = (uint8_t)(*data & 0x3FU);

    FLEXIO_BISSC_ArrayRightShift(data, base->rxShifterNum, 6U);
    crcCal         = FLEXIO_BISSC_CRC6(data, (uint32_t)base->mtLen + (uint32_t)base->stLen + 2U);
    base->crcMatch = (crcCal == crcRecv);

    base->warnBit  = (uint8_t)(*data & 1U);
    base->errorBit = (uint8_t)((*data & 2U) >> 1U);

    FLEXIO_BISSC_ArrayRightShift(data, base->rxShifterNum, 2U);
    base->st = *data & FLEXIO_BISSC_BitMask(base->stLen);

    FLEXIO_BISSC_ArrayRightShift(data, base->rxShifterNum, base->stLen);
    base->mt = *data & FLEXIO_BISSC_BitMask(base->mtLen);
}

void FLEXIO_BISSC_ReadBlocking(FLEXIO_BISSC_Type *base)
{
    assert(base != NULL);

    while ((base->flexio->SHIFTSTAT & (1UL << (base->shifterStartIdx + RXD_SHIFTER))) == 0U)
    {
    }

    for (uint32_t i = 0U; i < base->rxShifterNum; i++)
    {
        base->rxdBuffer[i] =
            base->flexio->SHIFTBUFBIS[base->shifterStartIdx + RXD_SHIFTER + base->rxShifterNum - 1U - i];
    }

    FLEXIO_BISSC_ProcessRxWindow(base);
}

void FLEXIO_BISSC_IRQHandler(void)
{
    FLEXIO_BISSC_Type *base  = s_flexioBisscHandle[0];
    FLEXIO_Type       *flexio;

    if (base == NULL)
    {
        return;
    }

    flexio = base->flexio;
    if ((flexio->SHIFTSTAT & (1UL << (base->shifterStartIdx + RXD_SHIFTER))) != 0U)
    {
        for (uint32_t i = 0U; i < base->rxShifterNum; i++)
        {
            base->rxdBuffer[i] =
                flexio->SHIFTBUFBIS[base->shifterStartIdx + RXD_SHIFTER + base->rxShifterNum - 1U - i];
        }
        FLEXIO_BISSC_ProcessRxWindow(base);
        base->rxdBufferReady = true;
    }
}
