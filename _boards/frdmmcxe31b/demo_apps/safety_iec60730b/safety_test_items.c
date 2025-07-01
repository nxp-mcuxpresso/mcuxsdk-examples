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

/*******************************************************************************
 *                                                                              *
 *                           STRUCTURE FOR DIO Initialization and TEST          *
 *                                                                              *
 *******************************************************************************/
#define CALC_GPDxN(n) ((n) + 3 - 2 * ((n) % 4))

//    DIO_PIN_x = port * 32 + pin; 
//
//    DIO_PIN_0 (PTE17) = 4 * 32 + 17 = 145
//
//    kSIUL2_PTA = 0U, /*!< PTA. */
//    kSIUL2_PTB = 1U, /*!< PTB. */
//    kSIUL2_PTC = 2U, /*!< PTC. */
//    kSIUL2_PTD = 3U, /*!< PTD. */
//    kSIUL2_PTE = 4U, /*!< PTE. */
//    kSIUL2_PTF = 5U, /*!< PTF. */
//    kSIUL2_PTG = 6U, /*!< PTG. */

#define DIO_PIN_0 145    // PTE17 (Arduino_D7)
#define DIO_PIN_1 146    // PTE18 (Arduino_D8)

fs_dio_test_siul2_t dio_safety_test_item_0 = {
    .siul2         = SIUL2_BASE,
    .pinNum        = DIO_PIN_0,
    .pinNumGPDxN   = CALC_GPDxN(DIO_PIN_0),
    .pinDir        = PIN_DIRECTION_IN,
};

fs_dio_test_siul2_t dio_safety_test_item_1 = {
    .siul2         = SIUL2_BASE,
    .pinNum        = DIO_PIN_1,
    .pinNumGPDxN   = CALC_GPDxN(DIO_PIN_1),
    .pinDir        = PIN_DIRECTION_IN,
};

/* NULL terminated array of pointers to dio_test_t items for safety DIO test */
fs_dio_test_siul2_t *g_dio_safety_test_items[] = {&dio_safety_test_item_0, &dio_safety_test_item_1, NULL};
