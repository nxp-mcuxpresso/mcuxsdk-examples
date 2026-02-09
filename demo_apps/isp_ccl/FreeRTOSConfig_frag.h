/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef FREERTOS_CONFIG_FRAG_H
#define FREERTOS_CONFIG_FRAG_H

#if ( configGENERATE_RUN_TIME_STATS == 1 )
    #include "fsl_device_registers.h"

    #define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS vPortStartStatsTimer
    static inline void vPortStartStatsTimer( void )
    {
        LPTMR_Type *Timer = LPTMR1;
        // reset all to 0;
        Timer->CSR = 0;

        /*! TMS - Timer Mode Select
        *  0b0..Time Counter
        *  0b1..Pulse Counter */
        Timer->CSR |= LPTMR_CSR_TMS(0);
        /*! TFC - Timer Free-Running Counter
        *  0b0..Reset when TCF asserts
        *  0b1..Reset on overflow */
        Timer->CSR |= LPTMR_CSR_TFC(1);
        /*! TPS - Timer Pin Select
        *  0b00..Input 0
        *  0b01..Input 1
        *  0b10..Input 2
        *  0b11..Input 3 */
        Timer->CSR |= LPTMR_CSR_TPS(0);
        /*! TIE - Timer Interrupt Enable
        *  0b0..Disable
        *  0b1..Enable */
        Timer->CSR |= LPTMR_CSR_TIE(0);
        /*! TCF - Timer Compare Flag
        *  0b0..CNR != (CMR + 1) - read only
        *  0b1..CNR = (CMR + 1) - read only
        *  0b0..No effect - write only
        *  0b1..Clear the flag - write only */
        Timer->CSR |= LPTMR_CSR_TCF(0);
        /*! TDRE - Timer DMA Request Enable
        *  0b0..Disable
        *  0b1..Enable */
        Timer->CSR |= LPTMR_CSR_TDRE(0);

        Timer->PSR = 0;
        /*! PCS - Prescaler and Glitch Filter Clock Select
        *  0b00..Clock 0 - ipg_clk_irclk: The frequency depends on configuration of lptmr1_clk_root. My System Manager reports 24MHz.
        *  0b01..Clock 1 - ipg_clk_1kHz: Tied to the fixed 32kHz clock
        *  0b10..Clock 2 - ipg_clk_32kHz: It is fixed at 32 kHz
        *  0b11..Clock 3 - ipg_clk_erclk: Tied to the fixed 32kHz clock*/
        Timer->PSR |= LPTMR_PSR_PCS(0);
        /*! PBYP - Prescaler and Glitch Filter Bypass
        *  0b0..Prescaler and glitch filter enable
        *  0b1..Prescaler and glitch filter bypass */
        Timer->PSR |= LPTMR_PSR_PBYP(0);
        /*! PRESCALE - Prescaler and Glitch Filter Value
        *  0b0000..Prescaler divides the prescaler clock by 2; glitch filter does not support this configuration
        *  0b0001..Prescaler divides the prescaler clock by 4; glitch filter recognizes change on input pin after two rising clock edges
        *  0b0010..Prescaler divides the prescaler clock by 8; glitch filter recognizes change on input pin after four rising clock edges
        *  0b0011..Prescaler divides the prescaler clock by 16; glitch filter recognizes change on input pin after eight rising clock edges
        *  0b0100..Prescaler divides the prescaler clock by 32; glitch filter recognizes change on input pin after 16 rising clock edges
        *  0b0101..Prescaler divides the prescaler clock by 64; glitch filter recognizes change on input pin after 32 rising clock edges
        *  0b0110..Prescaler divides the prescaler clock by 128; glitch filter recognizes change on input pin after 64 rising clock edges
        *  0b0111..Prescaler divides the prescaler clock by 256; glitch filter recognizes change on input pin after 128 rising clock edges
        *  0b1000..Prescaler divides the prescaler clock by 512; glitch filter recognizes change on input pin after 256 rising clock edges
        *  0b1001..Prescaler divides the prescaler clock by 1024; glitch filter recognizes change on input pin after 512 rising clock edges
        *  0b1010..Prescaler divides the prescaler clock by 2048; glitch filter recognizes change on input pin after 1024 rising clock edges
        *  0b1011..Prescaler divides the prescaler clock by 4096; glitch filter recognizes change on input pin after 2048 rising clock edges
        *  0b1100..Prescaler divides the prescaler clock by 8192; glitch filter recognizes change on input pin after 4096 rising clock edges
        *  0b1101..Prescaler divides the prescaler clock by 16,384; glitch filter recognizes change on input pin after 8192 rising clock edges
        *  0b1110..Prescaler divides the prescaler clock by 32,768; glitch filter recognizes change on input pin after 16,384 rising clock edges
        *  0b1111..Prescaler divides the prescaler clock by 65,536; glitch filter recognizes change on input pin after 32,768 rising clock edges */
        Timer->PSR |= LPTMR_PSR_PRESCALE(4);

        Timer->CMR = 0xFFFFFFFF;

        /*! TDRE - Timer DMA Request Enable
        *  0b0..Disable
        *  0b1..Enable */
        Timer->CSR |= LPTMR_CSR_TEN(1);
    }

    static inline void LPTMR1_IRQHandler( void )
    {
        // clear interrupt
        LPTMR1->CSR |= LPTMR_CSR_TCF(1);
    }

    #pragma GCC push_options
    #pragma GCC optimize ("O0")
    static inline uint32_t vPortGetStatsTimerValue( void )
    {
        *((uint32_t *)0x4430000Cu) = 1; // as per datasheet we need to write before read
        return *((uint32_t *)0x4430000Cu);
    }
    #pragma GCC pop_options
    #define portGET_RUN_TIME_COUNTER_VALUE vPortGetStatsTimerValue
#endif

#endif /* FREERTOS_CONFIG_FRAG_H */
