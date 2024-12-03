/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "board.h"
#include "app.h"
#include "mbw.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern int mbw_main(
    uint32_t testno, uint32_t showavg, uint32_t nr_loops, uint64_t block_size, uint32_t mem_start, uint32_t mem_size);

#ifndef CONFIG_TABLE
#error "Please define a config table, such as: #define CONFIG_TABLE ({"DRAM", 0, 1, 0, 1024, 0x80000000, 512 * 1024})"
#endif
static struct config_table cfg_tbl[] = CONFIG_TABLE;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    int32_t i = 0;
    /* Init board hardware. */
    BOARD_InitHardware();
    timer_init();
#ifdef CONFIG_TABLE
    for (i = 0; i < ARRAY_SIZE(cfg_tbl); i++)
    {
        PRINTF("Start----------Memory(%s, range: 0x%x ~ 0x%x) Bandwidth Benchmark(MCore freq: %u)----------\r\n",
               cfg_tbl[i].mem_name, cfg_tbl[i].mem_start, cfg_tbl[i].mem_start + cfg_tbl[i].mem_size, SystemCoreClock);
        mbw_main(cfg_tbl[i].testno, cfg_tbl[i].showavg, cfg_tbl[i].nr_loops, cfg_tbl[i].block_size,
                 cfg_tbl[i].mem_start, cfg_tbl[i].mem_size);
        PRINTF(
            "End------------Memory(%s, range: 0x%x ~ 0x%x) Bandwidth Benchmark(MCore freq: %u)----------\r\n\r\n\r\n",
            cfg_tbl[i].mem_name, cfg_tbl[i].mem_start, cfg_tbl[i].mem_start + cfg_tbl[i].mem_size, SystemCoreClock);
    }
#else
#error "Please define a config table, such as: #define CONFIG_TABLE {{"DRAM", 0, 1, 0, 1024, 0x80000000, 512 * 1024}}"
#endif
    timer_deinit();
    while (1)
    {
    }
}
