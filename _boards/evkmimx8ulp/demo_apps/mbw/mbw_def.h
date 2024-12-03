/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MBW_DEF_H_
#define _MBW_DEF_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPIT_BASE       LPIT0
#define DEMO_LPIT_CLOCK_NAME (kCLOCK_Lpit0)
#define DEMO_LPIT_CLOCK_FREQ CLOCK_GetIpFreq(DEMO_LPIT_CLOCK_NAME)
#endif /* _MBW_DEF_H_ */
