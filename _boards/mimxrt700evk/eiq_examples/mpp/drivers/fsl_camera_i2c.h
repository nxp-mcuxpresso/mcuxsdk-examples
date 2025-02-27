/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_CAMERA_I2C_H_
#define _FSL_CAMERA_I2C_H_


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void BOARD_Camera_I2C_Init(LPI2C_Type *base);
status_t BOARD_Camera_I2C_Send(
	uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Camera_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
