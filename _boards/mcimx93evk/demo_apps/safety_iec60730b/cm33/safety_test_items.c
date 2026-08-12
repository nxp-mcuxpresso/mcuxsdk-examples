/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_config.h"

#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = ".safety_ram"
#endif

#define MUX_ADDR(arg1)                          MUX_ADDR_(arg1) /* wrapper */
#define MUX_ADDR_(arg1, arg2, arg3, arg4, arg5) arg1            /* get 1st value from macro */
#define PAD_ADDR(arg1)                          PAD_ADDR_(arg1) /* wrapper */
#define PAD_ADDR_(arg1, arg2, arg3, arg4, arg5) arg5            /* get 5th value from macro */

/*******************************************************************************
 *                                                                              *
 *                           STRUCTURE FOR DIO Initialization and TEST          *
 *                                                                              *
 *******************************************************************************/

//EXP_GPIO_IO22    J1001-15
fs_dio_test_rgpio_t dio_safety_test_item_0 = {
    .gpio          = GPIO2_BASE,
    .pinNum        = 22U,
    .pinDir        = PIN_DIRECTION_IN,
    .muxAddr       = MUX_ADDR(IOMUXC_PAD_GPIO_IO22__GPIO2_IO22),
    .padAddr       = PAD_ADDR(IOMUXC_PAD_GPIO_IO22__GPIO2_IO22),
    .pullUpMask    = IOMUXC_PAD_PU_MASK,
    .pullDownMask  = IOMUXC_PAD_PD_MASK,
    .pullResetMask = (IOMUXC_PAD_PU_MASK | IOMUXC_PAD_PD_MASK),
};

//EXP_GPIO_IO05    J1001-29
fs_dio_test_rgpio_t dio_safety_test_item_1 = {
    .gpio          = GPIO2_BASE,
    .pinNum        = 5U,
    .pinDir        = PIN_DIRECTION_IN,
    .muxAddr       = MUX_ADDR(IOMUXC_PAD_GPIO_IO05__GPIO2_IO05),
    .padAddr       = PAD_ADDR(IOMUXC_PAD_GPIO_IO05__GPIO2_IO05),
    .pullUpMask    = IOMUXC_PAD_PU_MASK,
    .pullDownMask  = IOMUXC_PAD_PD_MASK,
    .pullResetMask = (IOMUXC_PAD_PU_MASK | IOMUXC_PAD_PD_MASK),
};

/* NULL terminated array of pointers to dio_test_t items for safety DIO test */
fs_dio_test_rgpio_t *g_dio_safety_test_items[] = {&dio_safety_test_item_0, &dio_safety_test_item_1, NULL};
