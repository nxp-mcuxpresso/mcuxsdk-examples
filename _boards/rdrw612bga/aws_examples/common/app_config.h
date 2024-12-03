/*
 *  Copyright 2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

/* @TEST_ANCHOR */
#define WIFI_BOARD_RW610
/* @END_TEST_ANCHOR */

#if defined(WIFI_BOARD_RW610)
#define RW610
#else
#error "Please define macro for RW610 board"
#endif

#include "wifi_config_internal.h"
#include "wifi_config.h"
