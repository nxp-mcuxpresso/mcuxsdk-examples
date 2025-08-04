/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __APP_H__
#define __APP_H__
#include "usb_host_config.h"
#include "usb_host.h"
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_USB_HOST_TASK_PRIORITY (tskIDLE_PRIORITY + 5)
#define APP_USB_DEVICE_TASK_PRIORITY (APP_USB_HOST_TASK_PRIORITY - 1)
#define APP_MAIN_TASK_PRIORITY (APP_USB_HOST_TASK_PRIORITY - 1)

#endif /* __APP_H__ */
