/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MBW_DEF_H_
#define _MBW_DEF_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPIT_BASE       LPIT1
#define DEMO_LPIT_CLOCK_NAME (kCLOCK_Root_Lpit1)
#define DEMO_LPIT_CLOCK_GATE kCLOCK_Lpit1
#define DEMO_LPIT_CLOCK_FREQ CLOCK_GetIpFreq(DEMO_LPIT_CLOCK_NAME)
#endif /* _MBW_DEF_H_ */
