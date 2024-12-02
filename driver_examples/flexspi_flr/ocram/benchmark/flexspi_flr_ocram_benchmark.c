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
#include "fsl_gpt.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FOLLOWER_STATUS_AXI_ACCESS (FLEXSPI_SLV_MODULE_STATUS_ALLOWAXIWR_MASK | FLEXSPI_SLV_MODULE_STATUS_ALLOWAXIRD_MASK)
#define FOLLOWER_STATUS_AXI_WRITE  (FLEXSPI_SLV_MODULE_STATUS_SEQIDLE_MASK | FLEXSPI_SLV_MODULE_STATUS_WIP_MASK)
#define FOLLOWER_STATUS_AXI_READ   (FLEXSPI_SLV_MODULE_STATUS_SEQIDLE_MASK | FLEXSPI_SLV_MODULE_STATUS_AXIREADIDLE_MASK)
#define FOLLOWER_STATUS_REG_ACCESS (FLEXSPI_SLV_MODULE_STATUS_SEQIDLE_MASK | FLEXSPI_SLV_MODULE_STATUS_REGRWIDLE_MASK)
#define FOLLOWER_STATUS_AXI_WRITE_IDLE FLEXSPI_SLV_MODULE_STATUS_SEQIDLE_MASK

#define TYPE        int
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void flexspi_ocram_init(FLEXSPI_Type *base);
/*******************************************************************************
 * Variables
 ******************************************************************************/
typedef uint32_t iter_t;
typedef struct _state {
	TYPE    *buf;
	TYPE    *lastone;
} state_t;
typedef void (*test_func_t)(iter_t iterations, void *cookie);

static volatile uint64_t use_result_dummy;
static char *io_mode[4] =
{
    "SDRx4", "SDRx8", "DDRx4", "DDRx8"
};
extern uint32_t gptFreq;
/*******************************************************************************
 * Code
 ******************************************************************************/
void use_int(int result)
{
    use_result_dummy += result;
}

void use_pointer(void *result)
{
    use_result_dummy += (int)result;
}

void rd(iter_t iterations, void *cookie)
{
    state_t *state = (state_t *) cookie;
    register TYPE *lastone = state->lastone;
    register int sum = 0;

    while (iterations-- > 0)
    {
        register TYPE *p = state->buf;
        while (p < lastone)
       	{
            sum +=
#define	DOIT(i)	p[i]+
            DOIT(0) DOIT(4) DOIT(8) DOIT(12) DOIT(16) DOIT(20) DOIT(24)
            DOIT(28) DOIT(32) DOIT(36) DOIT(40) DOIT(44) DOIT(48) DOIT(52)
            DOIT(56) DOIT(60) DOIT(64) DOIT(68) DOIT(72) DOIT(76)
            DOIT(80) DOIT(84) DOIT(88) DOIT(92) DOIT(96) DOIT(100)
            DOIT(104) DOIT(108) DOIT(112) DOIT(116) DOIT(120)
            p[124];
            p +=  128;
        }
    }
    use_int(sum);
}
#undef	DOIT

void wr(iter_t iterations, void *cookie)
{
    state_t *state = (state_t *) cookie;
    register TYPE *lastone = state->lastone;

    while (iterations-- > 0)
    {
        register TYPE *p = state->buf;
        while (p < lastone)
       	{
#define	DOIT(i)	p[i] = 1;
            DOIT(0) DOIT(4) DOIT(8) DOIT(12) DOIT(16) DOIT(20) DOIT(24)
            DOIT(28) DOIT(32) DOIT(36) DOIT(40) DOIT(44) DOIT(48) DOIT(52)
            DOIT(56) DOIT(60) DOIT(64) DOIT(68) DOIT(72) DOIT(76)
            DOIT(80) DOIT(84) DOIT(88) DOIT(92) DOIT(96) DOIT(100)
            DOIT(104) DOIT(108) DOIT(112) DOIT(116) DOIT(120) DOIT(124);
            p +=  128;
        }
    }
}
#undef	DOIT

void frd(iter_t iterations, void *cookie)
{
    state_t *state = (state_t *) cookie;
    register int sum = 0;
    register TYPE *lastone = state->lastone;

    while (iterations-- > 0)
    {
        register TYPE *p = state->buf;
        while (p < lastone)
       	{
            sum +=
#define	DOIT(i)	p[i]+
            DOIT(0) DOIT(1) DOIT(2) DOIT(3) DOIT(4) DOIT(5) DOIT(6)
            DOIT(7) DOIT(8) DOIT(9) DOIT(10) DOIT(11) DOIT(12)
            DOIT(13) DOIT(14) DOIT(15) DOIT(16) DOIT(17) DOIT(18)
            DOIT(19) DOIT(20) DOIT(21) DOIT(22) DOIT(23) DOIT(24)
            DOIT(25) DOIT(26) DOIT(27) DOIT(28) DOIT(29) DOIT(30)
            DOIT(31) DOIT(32) DOIT(33) DOIT(34) DOIT(35) DOIT(36)
            DOIT(37) DOIT(38) DOIT(39) DOIT(40) DOIT(41) DOIT(42)
            DOIT(43) DOIT(44) DOIT(45) DOIT(46) DOIT(47) DOIT(48)
            DOIT(49) DOIT(50) DOIT(51) DOIT(52) DOIT(53) DOIT(54)
            DOIT(55) DOIT(56) DOIT(57) DOIT(58) DOIT(59) DOIT(60)
            DOIT(61) DOIT(62) DOIT(63) DOIT(64) DOIT(65) DOIT(66)
            DOIT(67) DOIT(68) DOIT(69) DOIT(70) DOIT(71) DOIT(72)
            DOIT(73) DOIT(74) DOIT(75) DOIT(76) DOIT(77) DOIT(78)
            DOIT(79) DOIT(80) DOIT(81) DOIT(82) DOIT(83) DOIT(84)
            DOIT(85) DOIT(86) DOIT(87) DOIT(88) DOIT(89) DOIT(90)
            DOIT(91) DOIT(92) DOIT(93) DOIT(94) DOIT(95) DOIT(96)
            DOIT(97) DOIT(98) DOIT(99) DOIT(100) DOIT(101) DOIT(102)
            DOIT(103) DOIT(104) DOIT(105) DOIT(106) DOIT(107)
            DOIT(108) DOIT(109) DOIT(110) DOIT(111) DOIT(112)
            DOIT(113) DOIT(114) DOIT(115) DOIT(116) DOIT(117)
            DOIT(118) DOIT(119) DOIT(120) DOIT(121) DOIT(122)
            DOIT(123) DOIT(124) DOIT(125) DOIT(126) p[127];
            p += 128;
        }
    }
    use_int(sum);
}
#undef	DOIT

void fwr(iter_t iterations, void *cookie)
{
    state_t *state = (state_t *) cookie;
    register TYPE *lastone = state->lastone;
    TYPE* p_save = NULL;

    while (iterations-- > 0)
    {
        register TYPE *p = state->buf;
        while (p < lastone)
       	{
#define	DOIT(i)	p[i]=
            DOIT(0) DOIT(1) DOIT(2) DOIT(3) DOIT(4) DOIT(5) DOIT(6)
            DOIT(7) DOIT(8) DOIT(9) DOIT(10) DOIT(11) DOIT(12)
            DOIT(13) DOIT(14) DOIT(15) DOIT(16) DOIT(17) DOIT(18)
            DOIT(19) DOIT(20) DOIT(21) DOIT(22) DOIT(23) DOIT(24)
            DOIT(25) DOIT(26) DOIT(27) DOIT(28) DOIT(29) DOIT(30)
            DOIT(31) DOIT(32) DOIT(33) DOIT(34) DOIT(35) DOIT(36)
            DOIT(37) DOIT(38) DOIT(39) DOIT(40) DOIT(41) DOIT(42)
            DOIT(43) DOIT(44) DOIT(45) DOIT(46) DOIT(47) DOIT(48)
            DOIT(49) DOIT(50) DOIT(51) DOIT(52) DOIT(53) DOIT(54)
            DOIT(55) DOIT(56) DOIT(57) DOIT(58) DOIT(59) DOIT(60)
            DOIT(61) DOIT(62) DOIT(63) DOIT(64) DOIT(65) DOIT(66)
            DOIT(67) DOIT(68) DOIT(69) DOIT(70) DOIT(71) DOIT(72)
            DOIT(73) DOIT(74) DOIT(75) DOIT(76) DOIT(77) DOIT(78)
            DOIT(79) DOIT(80) DOIT(81) DOIT(82) DOIT(83) DOIT(84)
            DOIT(85) DOIT(86) DOIT(87) DOIT(88) DOIT(89) DOIT(90)
            DOIT(91) DOIT(92) DOIT(93) DOIT(94) DOIT(95) DOIT(96)
            DOIT(97) DOIT(98) DOIT(99) DOIT(100) DOIT(101) DOIT(102)
            DOIT(103) DOIT(104) DOIT(105) DOIT(106) DOIT(107)
            DOIT(108) DOIT(109) DOIT(110) DOIT(111) DOIT(112)
            DOIT(113) DOIT(114) DOIT(115) DOIT(116) DOIT(117)
            DOIT(118) DOIT(119) DOIT(120) DOIT(121) DOIT(122)
            DOIT(123) DOIT(124) DOIT(125) DOIT(126) DOIT(127) 1;
            p += 128;
        }
        p_save = p;
    }
    use_pointer(p_save);
}
#undef	DOIT

void run_func(char *func_name, test_func_t func, iter_t iter, void *cookie, uint32_t mb)
{
    TickType_t start_tick = 0, end_tick = 0;
    uint32_t  msecs;
    int i = 3;

    PRINTF("Testing bandwidth of %s\r\n", func_name);

    while (i-- > 0) {
        start_tick = GPT_GetCurrentTimerCount(EXAMPLE_GPT);

        func(iter, cookie);

        end_tick = GPT_GetCurrentTimerCount(EXAMPLE_GPT);

        end_tick = end_tick > start_tick ? end_tick : gptFreq * 50 + end_tick;

        msecs = (end_tick - start_tick) / (gptFreq / 1000);

        PRINTF("read %d MB in %d ms, bandwidth=%d MB per second \r\n", mb, msecs, (mb * 1000) / msecs);
    };
}

void benchmark(void)
{
    state_t state;
    uintptr_t start_addr = OCRAM_BASEADDR;
    TYPE *buf = (TYPE *)start_addr;
    uint32_t buf_len_kb = 450; // KBytes
    uint32_t buf_len = buf_len_kb * 1024;
    iter_t iter = 1024;
    uint32_t  mb;

    mb = iter * buf_len_kb / 1024;
    state.buf = buf;
    state.lastone = (TYPE *)(start_addr + buf_len);

    run_func("OCRAM-RD", rd, iter, (void *)&state, mb);
    run_func("OCRAM-WR", wr, iter, (void *)&state, mb);
    run_func("OCRAM-FRD", frd, iter, (void *)&state, mb);
    run_func("OCRAM-FWR", fwr, iter, (void *)&state, mb);
    PRINTF("**************Testing End*************\r\n");
}

void benchmark_follower(void)
{
    state_t state;
    uintptr_t start_addr = EXAMPLE_FLEXSPI_AMBA_BASE;
    TYPE *buf = (TYPE *)start_addr;
    uint32_t buf_len_kb = 450; // KBytes
    uint32_t buf_len = buf_len_kb * 1024;
    iter_t iter = 1024;
    uint32_t  mb;

    mb = iter * buf_len_kb / 1024;
    state.buf = buf;
    state.lastone = (TYPE *)(start_addr + buf_len);

    run_func("Follower-RD", rd, iter, (void *)&state, mb);
    run_func("Follower-WR", wr, iter, (void *)&state, mb);
    run_func("Follower-FRD", frd, iter, (void *)&state, mb);
    run_func("Follower-FWR", fwr, iter, (void *)&state, mb);
    PRINTF("**************Testing End*************\r\n");
}

int main(void)
{
    flexspi_slv_config_t config;

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

    flexspi_ocram_init(EXAMPLE_FLEXSPI);
    PRINTF("FLEXSPI leader is initialized!\r\n");

    setupTimerInterrupt();
    PRINTF("------GPT clock is %dHz\r\n", gptFreq);
    benchmark();
    benchmark_follower();

    while (1)
    {
    }
}
