/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * DV_USBD_Config.h - USB Device validation configuration for FRDM-MCXA577.
 * Enables all 4 basic tests against Driver_USBD1.
 */
#ifndef DV_USBD_CONFIG_H_
#define DV_USBD_CONFIG_H_

/* USB Device driver instance index - matches Driver_USBD1 exported by fsl_usbd_ehci_cmsis.c */
#define DRV_USBD                        1

/* Enable all basic tests */
#define USBD_GETCAPABILITIES_EN         1
#define USBD_INITIALIZATION_EN          1
#define USBD_POWERCONTROL_EN            1
#define USBD_CHECKINVALIDINIT_EN        1

#endif /* DV_USBD_CONFIG_H_ */
