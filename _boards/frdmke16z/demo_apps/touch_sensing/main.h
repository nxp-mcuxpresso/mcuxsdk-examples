/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include "nt_setup.h"

/**
 * \brief NT_FRDM_TOUCH_SUPPORT enables the support of the Touch Module for Freedom Board (FRDM-TOUCH board)
 *  for the NXP Touch example.
 *  When it is enabled, the demo will use all electrodes: 2 from MCU board and 10 from FRDM-TOUCH board.
 *  When it is disabled, the demo will use only two electrodes from MCU board board.
 */
#ifndef NT_FRDM_TOUCH_SUPPORT
#define NT_FRDM_TOUCH_SUPPORT 1
#endif

#endif
