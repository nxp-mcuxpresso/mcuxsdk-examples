/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USB_DEVICE_BOARD_CONFIG_H_
#define _USB_DEVICE_BOARD_CONFIG_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/*!
 * @name Hardware instance define
 * @{
 */

#ifdef USB_ISR_REDIRECT
/*! The below 3 ISRS are renamed as there is alternate implementation in virtual_com.c */
#define USB_OTG1_IRQHandler NOT_USED_HERE_1
#define USB_OTG2_IRQHandler NOT_USED_HERE_2
#define USB0_IRQHandler     NOT_USED_HERE_3
#endif

/*! @brief KHCI instance count */
#define USB_DEVICE_CONFIG_KHCI (0U)

/*! @brief EHCI instance count */
#define USB_DEVICE_CONFIG_EHCI (1U)

/*! @brief LPC USB IP3511 FS instance count */
#define USB_DEVICE_CONFIG_LPCIP3511FS (0U)

/*! @brief LPC USB IP3511 HS instance count */
#define USB_DEVICE_CONFIG_LPCIP3511HS (0U)

/*! @brief Device instance count, the sum of KHCI and EHCI instance counts*/
#define USB_DEVICE_CONFIG_NUM \
    (USB_DEVICE_CONFIG_KHCI + USB_DEVICE_CONFIG_EHCI + USB_DEVICE_CONFIG_LPCIP3511FS + USB_DEVICE_CONFIG_LPCIP3511HS)

/* @} */
/*${macro:end}*/

#endif /* _USB_DEVICE_BOARD_CONFIG_H_ */
