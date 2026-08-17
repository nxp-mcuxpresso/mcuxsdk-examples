/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
/*${header:end}*/

/*${function:start}*/

/*
 * See example_board_readme.md for a description of each build target and
 * the expected behavior of this auxiliary image.
 */

#if !defined(USE_PSRAM)

#define SHADOW_FUSE_BOOT_CFG0_INDEX  24UL
#define SHADOW_FUSE_BOOT_CFG0_TARGET 0x82UL /* boot from eMMC */

#define RES_CMD_SUCC          0x000000D6UL
#define CMD_READ_SHADOW_FUSE  0x17F30206UL
#define RES_READ_SHADOW_FUSE  0xE1F30306UL
#define CMD_WRITE_SHADOW_FUSE 0x17F20306UL
#define RES_WRITE_SHADOW_FUSE 0xE1F20206UL

static uint32_t _Read_ShadowFuse(uint32_t fuse_word_index)
{
    S3MU_Type *const mu = MAIN__SENTMU0_SENTMUA;
    uint32_t response[3];

    while ((mu->TSR & S3MU_TSR_TEn(1U << 0)) == 0U) { }
    mu->TR[0] = CMD_READ_SHADOW_FUSE;
    while ((mu->TSR & S3MU_TSR_TEn(1U << 1)) == 0U) { }
    mu->TR[1] = fuse_word_index;

    while ((mu->RSR & S3MU_RSR_RFn(1U << 0)) == 0U) { }
    response[0] = mu->RR[0];
    while ((mu->RSR & S3MU_RSR_RFn(1U << 1)) == 0U) { }
    response[1] = mu->RR[1];
    while ((mu->RSR & S3MU_RSR_RFn(1U << 2)) == 0U) { }
    response[2] = mu->RR[2];

    if ((response[0] == RES_READ_SHADOW_FUSE) && (response[1] == RES_CMD_SUCC))
    {
        return response[2];
    }
    else
    {
        return 0;
    }
}

static bool _Write_ShadowFuse(uint32_t fuse_word_index, uint32_t value)
{
    bool ret            = false;
    S3MU_Type *const mu = MAIN__SENTMU0_SENTMUA;
    uint32_t response[2];

    while ((mu->TSR & S3MU_TSR_TEn(1U << 0)) == 0U) { }
    mu->TR[0] = CMD_WRITE_SHADOW_FUSE;
    while ((mu->TSR & S3MU_TSR_TEn(1U << 1)) == 0U) { }
    mu->TR[1] = fuse_word_index;
    while ((mu->TSR & S3MU_TSR_TEn(1U << 2)) == 0U) { }
    mu->TR[2] = value;

    while ((mu->RSR & S3MU_RSR_RFn(1U << 0)) == 0U) { }
    response[0] = mu->RR[0];
    while ((mu->RSR & S3MU_RSR_RFn(1U << 1)) == 0U) { }
    response[1] = mu->RR[1];

    if ((response[0] == RES_WRITE_SHADOW_FUSE) && (response[1] == RES_CMD_SUCC))
    {
        ret = true;
    }
    return ret;
}

#endif

/* Override the default SystemInit */
void SystemInit(void)
{
#if !defined(USE_PSRAM)
    if (_Read_ShadowFuse(SHADOW_FUSE_BOOT_CFG0_INDEX) != SHADOW_FUSE_BOOT_CFG0_TARGET)
    {
        (void)_Write_ShadowFuse(SHADOW_FUSE_BOOT_CFG0_INDEX, SHADOW_FUSE_BOOT_CFG0_TARGET);
    }
    SYSCON__RESETCON->SWRST = 0x0123FAFA;
#endif
}

void BOARD_InitHardware(void)
{
    /* Intentionally left empty */
}
/*${function:end}*/
