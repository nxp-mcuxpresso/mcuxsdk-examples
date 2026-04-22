/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void ENET0_InitPins(void);
extern void ENET0_DeinitPins(void);

#define RTE_ENET                       1
#define RTE_ENET_PHY_ADDRESS           0
#define RTE_ENET_MII                   1
#define RTE_ENET_RMII                  0

#define RTE_ENET_T1S_PLCA_ENABLE           1
#define RTE_ENET_T1S_PLCA_NODE_ID          1U
#define RTE_ENET_T1S_PLCA_NODE_COUNT       8U
#define RTE_ENET_T1S_PLCA_TO_TIMER         0x20U
#define RTE_ENET_T1S_PLCA_BURST_TIMER      0x80U
#define RTE_ENET_T1S_PLCA_MAX_BURST_COUNT  0U

#endif /* _RTE_DEVICE_H */
