/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USB_DEVICE_DFU_CONFIG_H_
#define _USB_DEVICE_DFU_CONFIG_H_
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*${macro:start}*/
/* USB DFU config*/
/*! @brief DFU application address and size*/
#define USB_DFU_APP_ADDRESS                 (0x20000UL)
#define USB_DFU_APP_SIZE                    (0x8000UL)
#define USB_DFU_APP_ADDRESS_DATA_MEMORY_MAP (0x30000UL)
/*${macro:end}*/

#endif /* _USB_DEVICE_DFU_CONFIG_H_ */
