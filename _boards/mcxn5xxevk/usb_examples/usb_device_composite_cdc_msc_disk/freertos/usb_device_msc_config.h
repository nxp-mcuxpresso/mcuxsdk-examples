/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _USB_DEVICE_MSC_CONFIG_H_
#define _USB_DEVICE_MSC_CONFIG_H_
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*${macro:start}*/
/* USB MSC config*/
/*buffer size for sd card example. the larger the buffer size ,the faster the data transfer speed is ,*/
/*the block size should be multiple of 512, the least value is 512*/
#if ((defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)))

#define USB_DEVICE_MSC_WRITE_BUFF_SIZE (512 * 64U)
#define USB_DEVICE_MSC_READ_BUFF_SIZE  (512 * 64U)

#else

#define USB_DEVICE_MSC_WRITE_BUFF_SIZE (512 * 8U)
#define USB_DEVICE_MSC_READ_BUFF_SIZE  (512 * 8U)

#endif
/*${macro:end}*/

#endif /* _USB_DEVICE_MSC_CONFIG_H_ */
