/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_CORE1_H_
#define _APP_CORE1_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define RPMSG_LITE_SHMEM_BASE             (VDEV1_VRING_BASE)
#define ERPC_TRANSPORT_RPMSG_LITE_LINK_ID (1U)
#define RPMSG_LITE_NS_ANNOUNCE_STRING     "rpmsg-virtual-tty-channel"
#define RPMSG_LITE_MASTER_IS_LINUX

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_CORE1_H_ */
