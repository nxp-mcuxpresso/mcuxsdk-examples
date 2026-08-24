/*
 * Copyright 2024-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "flexio_endat2.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TXD_SHIFTER 0
#define DIR_SHIFTER 1
#define RXD_SHIFTER 2
#define RXD_TIMER   0
#define TXD_TIMER   1
#define DIR_TIMER   2

#define FLEXIO_ENC_HANDLE_COUNT 4

/*******************************************************************************
 * Variables
 ******************************************************************************/
static FLEXIO_ENDAT2_Type *s_flexioEncHandle[FLEXIO_ENC_HANDLE_COUNT] = {0u}; /* Static handle array for ISR. */

/*******************************************************************************
 * Codes
 ******************************************************************************/
int FLEXIO_ENDAT2_Init(FLEXIO_ENDAT2_Type *base, uint32_t srcClock_Hz)
{
    assert(base != NULL);

    flexio_shifter_config_t shifterConfig = {0u};
    flexio_timer_config_t   timerConfig   = {0u};
    uint16_t                baudDiv;

    /* Timer low 8-bit are used to configure baudrate. */
    baudDiv = (uint16_t)((srcClock_Hz / base->baudRateBps) >> 1u) - 1u;
    if (baudDiv > 0xFF)
    {
        return -1; /* Target bit rate is too low and FlexIO clock source is too high. */
    }

    base->rxShifterNum = ((base->mtLen + base->stLen + 9u) >> 5u) + 1u; /* start(1-bit), error(2-bit), MT, ST, CRC (6-bit) */
    if ((base->rxShifterNum > (sizeof(base->rxdBuffer) / sizeof(base->rxdBuffer[0]))) ||
        ((base->shifterStartIdx + RXD_SHIFTER + base->rxShifterNum) > FLEXIO_SHIFTBUF_COUNT) ||
        ((base->timerStartIdx + DIR_TIMER) >= FLEXIO_TIMCMP_COUNT))
    {
        return -2; /* Selected frame format does not fit this FlexIO instance or receive buffer. */
    }

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable FlexIO clock. */
    CLOCK_EnableClock(s_flexioClocks[FLEXIO_GetInstance(base->flexio)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Configure FLEXIO */
    base->flexio->CTRL = FLEXIO_CTRL_DBGE_MASK | FLEXIO_CTRL_FLEXEN_MASK;

    /* Do hardware configuration. */
    /* 1. Configure the shifter for tx. */
    shifterConfig.timerSelect   = base->timerStartIdx + TXD_TIMER;
    shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnPositive;
    shifterConfig.pinConfig     = kFLEXIO_PinConfigOutput;
    shifterConfig.pinSelect     = base->txdPinIdx;
    shifterConfig.pinPolarity   = kFLEXIO_PinActiveHigh;
    shifterConfig.shifterMode   = kFLEXIO_ShifterModeTransmit;
    shifterConfig.inputSource   = kFLEXIO_ShifterInputFromPin;
    shifterConfig.shifterStop   = kFLEXIO_ShifterStopBitLow;
    shifterConfig.shifterStart  = kFLEXIO_ShifterStartBitDisabledLoadDataOnShift;
    FLEXIO_SetShifterConfig(base->flexio, base->shifterStartIdx + TXD_SHIFTER, &shifterConfig);

    /* 2. Configure the shifter for direction. */
    shifterConfig.timerSelect   = base->timerStartIdx + DIR_TIMER;
    shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnPositive;
    shifterConfig.pinConfig     = kFLEXIO_PinConfigOutput;
    shifterConfig.pinSelect     = base->dirPinIdx;
    shifterConfig.pinPolarity   = kFLEXIO_PinActiveHigh;
    shifterConfig.shifterMode   = kFLEXIO_ShifterModeTransmit;
    shifterConfig.inputSource   = kFLEXIO_ShifterInputFromPin;
    shifterConfig.shifterStop   = kFLEXIO_ShifterStopBitLow;
    shifterConfig.shifterStart  = kFLEXIO_ShifterStartBitDisabledLoadDataOnEnable;
    FLEXIO_SetShifterConfig(base->flexio, base->shifterStartIdx + DIR_SHIFTER, &shifterConfig);
    base->flexio->SHIFTBUF[base->shifterStartIdx + DIR_SHIFTER] = 0xFFFFFFFF;

    /* 3. Configure the shifters for rx. */
    shifterConfig.timerSelect   = base->timerStartIdx + RXD_TIMER;
    shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnNegitive;
    shifterConfig.pinConfig     = kFLEXIO_PinConfigOutputDisabled;
    shifterConfig.pinSelect     = base->rxdPinIdx;
    shifterConfig.pinPolarity   = kFLEXIO_PinActiveHigh;
    shifterConfig.shifterMode   = kFLEXIO_ShifterModeReceive;
    shifterConfig.inputSource   = kFLEXIO_ShifterInputFromNextShifterOutput;
    shifterConfig.shifterStop   = kFLEXIO_ShifterStopBitDisable;
    shifterConfig.shifterStart  = kFLEXIO_ShifterStartBitHigh;
    uint32_t rxShifterEnd = base->shifterStartIdx + RXD_SHIFTER + base->rxShifterNum;
    for (uint32_t i = base->shifterStartIdx + RXD_SHIFTER; i < (rxShifterEnd - 1u); i++)
    {
        FLEXIO_SetShifterConfig(base->flexio, i, &shifterConfig);
    }
    shifterConfig.inputSource = kFLEXIO_ShifterInputFromPin;
    FLEXIO_SetShifterConfig(base->flexio, rxShifterEnd - 1u, &shifterConfig);

    /* 4. Configure the timer for Clock and Tx control. */
    if (base->hwTrigger)
    {
        timerConfig.triggerSelect   = 0;
        timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveHigh;
        timerConfig.triggerSource   = kFLEXIO_TimerTriggerSourceExternal;
    }
    else
    {
        timerConfig.triggerSelect   = FLEXIO_TIMER_TRIGGER_SEL_SHIFTnSTAT(base->shifterStartIdx + TXD_SHIFTER);
        timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
        timerConfig.triggerSource   = kFLEXIO_TimerTriggerSourceInternal;
    }
    timerConfig.pinConfig      = kFLEXIO_PinConfigOutput;
    timerConfig.pinSelect      = base->clkPinIdx;
    timerConfig.pinPolarity    = kFLEXIO_PinActiveLow;
    timerConfig.timerMode      = kFLEXIO_TimerModeDual8BitBaudBit;
    timerConfig.timerOutput    = kFLEXIO_TimerOutputOneNotAffectedByReset;
    timerConfig.timerDecrement = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    timerConfig.timerReset     = kFLEXIO_TimerResetNever;
    timerConfig.timerDisable   = kFLEXIO_TimerDisableOnPreTimerDisable;
    timerConfig.timerEnable    = kFLEXIO_TimerEnableOnTriggerHigh;
    timerConfig.timerStop      = kFLEXIO_TimerStopBitDisabled;
    timerConfig.timerStart     = kFLEXIO_TimerStartBitDisabled;

    /* High 8-bit are used to configure shift clock edges(transfer width); Low 8-bit are used to configure baudrate. */
    timerConfig.timerCompare = (0xFF << 8u) | baudDiv;
    FLEXIO_SetTimerConfig(base->flexio, base->timerStartIdx + TXD_TIMER, &timerConfig);

    /* 5. Configure the timer for direction. */
    timerConfig.pinConfig      = kFLEXIO_PinConfigOutputDisabled;
    timerConfig.pinSelect      = base->clkPinIdx;
    timerConfig.pinPolarity    = kFLEXIO_PinActiveLow;
    timerConfig.timerMode      = kFLEXIO_TimerModeDual8BitBaudBit;
    timerConfig.timerOutput    = kFLEXIO_TimerOutputZeroNotAffectedByReset;
    timerConfig.timerDecrement = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    timerConfig.timerReset     = kFLEXIO_TimerResetNever;
    timerConfig.timerDisable   = kFLEXIO_TimerDisableOnTimerCompare;
    timerConfig.timerEnable    = kFLEXIO_TimerEnableOnPrevTimerEnable;
    timerConfig.timerStop      = kFLEXIO_TimerStopBitEnableOnTimerCompare;
    timerConfig.timerStart     = kFLEXIO_TimerStartBitDisabled;
    timerConfig.timerCompare   = ((1u * 2u - 1u) << 8u) | baudDiv;
    FLEXIO_SetTimerConfig(base->flexio, base->timerStartIdx + DIR_TIMER, &timerConfig);

    /* 6. Configure the timer for RX. */
    timerConfig.pinConfig      = kFLEXIO_PinConfigOutputDisabled;
    timerConfig.pinSelect      = base->rxdPinIdx;
    timerConfig.pinPolarity    = kFLEXIO_PinActiveHigh;
    timerConfig.timerMode      = kFLEXIO_TimerModeDual8BitBaudBit;
    timerConfig.timerOutput    = kFLEXIO_TimerOutputOneAffectedByReset;
    timerConfig.timerDecrement = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    timerConfig.timerReset     = kFLEXIO_TimerResetOnTimerPinRisingEdge;
    timerConfig.timerDisable   = kFLEXIO_TimerDisableOnTimerCompare;
    timerConfig.timerEnable    = kFLEXIO_TimerEnableOnPinRisingEdge;
    timerConfig.timerStop      = kFLEXIO_TimerStopBitDisabled;
    timerConfig.timerStart     = kFLEXIO_TimerStartBitEnabled;
    timerConfig.timerCompare   = baudDiv;
    FLEXIO_SetTimerConfig(base->flexio, base->timerStartIdx + RXD_TIMER, &timerConfig);

    base->cmd_cfg[kFlexIO_ENDAT2_cmd_idx_EncSendPosVal].cmd_val       = FLEXIO_ENDAT2_CMD_ENCSENDPOSVAL;
    base->cmd_cfg[kFlexIO_ENDAT2_cmd_idx_EncSendPosVal].rxd_timer_val = (((1u + base->mtLen + base->stLen + FLEXIO_ENDAT2_CRC_LEN) * 2u - 1u) << 8u) | baudDiv;
    base->cmd_cfg[kFlexIO_ENDAT2_cmd_idx_EncSendPosVal].dir_timer_val = (((FLEXIO_ENDAT2_CMD_CLK_LEN) * 2u - 1u) << 8u) | baudDiv;

    base->cmd_cfg[kFlexIO_ENDAT2_cmd_idx_EncRecvRst].cmd_val       = FLEXIO_ENDAT2_CMD_ENCRECVRST;
    base->cmd_cfg[kFlexIO_ENDAT2_cmd_idx_EncRecvRst].rxd_timer_val = (((8u + 16u + FLEXIO_ENDAT2_CRC_LEN) * 2u - 1u) << 8u) | baudDiv;
    base->cmd_cfg[kFlexIO_ENDAT2_cmd_idx_EncRecvRst].dir_timer_val = (((FLEXIO_ENDAT2_CMD_CLK_LEN + 8u + 16u) * 2u - 1u) << 8u) | baudDiv;

    base->flexio->TIMCMP[base->timerStartIdx + RXD_TIMER] = base->cmd_cfg[kFlexIO_ENDAT2_cmd_idx_EncSendPosVal].rxd_timer_val;
    base->flexio->TIMCMP[base->timerStartIdx + DIR_TIMER] = base->cmd_cfg[kFlexIO_ENDAT2_cmd_idx_EncSendPosVal].dir_timer_val;
    base->current_cmd_idx                                 = kFlexIO_ENDAT2_cmd_idx_EncSendPosVal;

    for (uint32_t i = 0u; i < FLEXIO_ENC_HANDLE_COUNT; i++)
    {
        if (s_flexioEncHandle[i] == NULL)
        {
            s_flexioEncHandle[i] = base;
            break;
        }
    }

    return 0;
}

void FLEXIO_ENDAT2_EnableRxInterrupt(FLEXIO_ENDAT2_Type *base, bool enable)
{
    if (enable)
    {
        base->flexio->SHIFTSIEN |= 1u << (base->shifterStartIdx + RXD_SHIFTER);
    }
    else
    {
        base->flexio->SHIFTSIEN &= ~(1u << (base->shifterStartIdx + RXD_SHIFTER));
    }
}

void FLEXIO_ENDAT2_CmdPreset(FLEXIO_ENDAT2_Type *base, flexio_endat2_cmd_index_t cmd_idx)
{
    if (cmd_idx != base->current_cmd_idx)
    {
        base->flexio->TIMCMP[base->timerStartIdx + RXD_TIMER] = base->cmd_cfg[cmd_idx].rxd_timer_val;
        base->flexio->TIMCMP[base->timerStartIdx + DIR_TIMER] = base->cmd_cfg[cmd_idx].dir_timer_val;
        base->current_cmd_idx                                 = cmd_idx;
    }

    if (base->hwTrigger)
    {
        base->flexio->SHIFTBUFBIS[base->shifterStartIdx + TXD_SHIFTER] = base->cmd_cfg[base->current_cmd_idx].cmd_val; /* Write the command into the Tx shifter. */
    }
}

void FLEXIO_ENDAT2_SwTrigger(FLEXIO_ENDAT2_Type *base)
{
    base->flexio->SHIFTBUFBIS[base->shifterStartIdx + TXD_SHIFTER] = base->cmd_cfg[base->current_cmd_idx].cmd_val; /* Send the command and trigger the timer. */
}

static uint8_t FLEXIO_ENDAT2_CRC5(uint32_t *data, uint32_t len) /* P(x) = x5 + x3 + x1 + 1; LSB fist in data. */
{
    register uint8_t  crc = 0x1F; /* All filled with 1 in advance. */
    register uint32_t tmp32;
    uint32_t          len_tmp, data_index = 0;
    uint8_t           b0, b1, b3;

    while (len)
    {
        len_tmp = len >= 32u ? 32u : len;
        len -= len_tmp;
        tmp32 = data[data_index++];

        while (len_tmp--)
        {
            b0 = (tmp32 ^ (crc >> 4u)) & 1u;
            b1 = (crc ^ b0) & 1u;
            b3 = ((crc >> 2u) ^ b0) & 1u;
            crc <<= 1u;
            crc &= 0x14u; /* Clear bit0, bit1, and bit3. */
            crc |= (b3 << 3u) | (b1 << 1u) | b0;
            tmp32 >>= 1u;
        }
    }

    return (~crc) & 0x1Fu; /* Reverse the result. */
}

int FLEXIO_ENDAT2_DataParser(FLEXIO_ENDAT2_Type *base)
{
    uint32_t start_pos;
    uint64_t data64 = ((uint64_t)base->rxdBuffer[1] << 32u) | base->rxdBuffer[0];
    uint8_t  crc_recv, crc_cal;

    for (start_pos = 0u; start_pos < 32u; start_pos++)
    {
        if (data64 & (1u << start_pos))
        {
            break;
        }
    }

    data64 >>= (start_pos + 1u);                                                               /* error1 bit, st, and mt. LSB first. */
    crc_cal         = FLEXIO_ENDAT2_CRC5((uint32_t *)&data64, 1u + base->mtLen + base->stLen); /* calculate CRC. */
    base->error1Bit = data64 & 1u;                                                             /* error1 bit */
    base->error2Bit = 0u;
    data64 >>= 1u;
    base->st = data64 & ((1u << base->stLen) - 1u); /* st */
    data64 >>= base->stLen;
    base->mt = data64 & ((1u << base->mtLen) - 1u); /* mt */
    data64 >>= base->mtLen;
    crc_recv       = data64 & ((1u << FLEXIO_ENDAT2_CRC_LEN) - 1u); /* received CRC from encoder. MSB first, result in reversed bit order. */
    uint32_t tmp32 = crc_recv;
    tmp32 = __RBIT(tmp32); /* Reverse bit order. */

    crc_recv       = tmp32 >> (32u - FLEXIO_ENDAT2_CRC_LEN);
    base->crcMatch = (crc_cal == crc_recv);

    if (start_pos >= 32u)
    {
        return -1;
    }
    return 0;
}

int FLEXIO_ENDAT2_ReadBlocking(FLEXIO_ENDAT2_Type *base)
{
    uint32_t timeout = SystemCoreClock / 200u;

    /* Wait until data transfer complete. */
    while (!(base->flexio->SHIFTSTAT & (1u << (base->shifterStartIdx + RXD_SHIFTER))))
    {
        if (timeout-- == 0u)
        {
            return -1;
        }
    }

    for (uint32_t i = 0u; i < base->rxShifterNum; i++)
    {
        base->rxdBuffer[i] = base->flexio->SHIFTBUF[base->shifterStartIdx + RXD_SHIFTER + i];
    }

    return 0;
}

void FLEXIO_ENDAT2_IRQHandler(void)
{
    FLEXIO_ENDAT2_Type *base;
    FLEXIO_Type        *flexio    = NULL;
    int32_t             isr_number = __get_IPSR() - 16u;

#if defined(FLEXIO0)
    if (isr_number == (int32_t)FLEXIO_IRQn)
    {
        flexio = FLEXIO0;
    }
#endif
#if defined(FLEXIO1)
    if (isr_number == (int32_t)FLEXIO1_IRQn)
    {
        flexio = FLEXIO1;
    }
#endif
#if defined(FLEXIO2)
    if (isr_number == (int32_t)FLEXIO2_IRQn)
    {
        flexio = FLEXIO2;
    }
#endif

    if (flexio == NULL)
    {
        return;
    }

    for (uint32_t i = 0; i < FLEXIO_ENC_HANDLE_COUNT; i++)
    {
        base = s_flexioEncHandle[i];

        if (base == NULL)
        {
            break;
        }

        if (base->flexio == flexio)
        {
            if (flexio->SHIFTSTAT & (1u << (base->shifterStartIdx + RXD_SHIFTER)))
            {
                for (uint32_t i = 0u; i < base->rxShifterNum; i++)
                {
                    base->rxdBuffer[i] = flexio->SHIFTBUF[base->shifterStartIdx + RXD_SHIFTER + i];
                }
                base->rxdBufferReady = true;
            }
        }
    }
}
