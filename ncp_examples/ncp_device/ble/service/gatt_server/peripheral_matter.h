/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_MATTER_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_MATTER_H_
/**
 * @brief Matter Service
 * @defgroup  Matter Service
 * @ingroup bluetooth
 * @{
 *
 * [Experimental] Users should note that the APIs can change
 * as a part of ongoing development.
 */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* Definitions
******************************************************************************/

  /**
 *  @brief Device Information Service
 */
#define UUID16_CHIPoBLEService \
	BT_UUID_DECLARE_16(0xFFF6)
#define UUID128_CHIPoBLEChar_RX \
	BT_UUID_DECLARE_128(0x11, 0x9D, 0x9F, 0x42, 0x9C, 0x4F, 0x9F, 0x95, 0x59, 0x45, 0x3D, 0x26, 0xF5, 0x2E, 0xEE, 0x18)
#define UUID128_CHIPoBLEChar_TX \
	BT_UUID_DECLARE_128(0x12, 0x9D, 0x9F, 0x42, 0x9C, 0x4F, 0x9F, 0x95, 0x59, 0x45, 0x3D, 0x26, 0xF5, 0x2E, 0xEE, 0x18)
#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
#define UUID128_CHIPoBLEChar_C3 \
	BT_UUID_DECLARE_128(0x04, 0x8F, 0x21, 0x83, 0x8A, 0x74, 0x7D, 0xB8, 0xF2, 0x45, 0x72, 0x87, 0x38, 0x02, 0x63, 0x64)
#endif
  
/*******************************************************************************
* Prototypes
******************************************************************************/

void peripheral_matter_task(void *args);
void peripheral_matter_connect(struct bt_conn *conn, uint8_t conn_err);
void peripheral_matter_disconnect(struct bt_conn *conn, uint8_t reason);
void peripheral_matter_indication(uint8_t *buff, uint16_t len);
void unregister_matter_service(void);
void init_matter_service(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_SERVICES_MATTER_H_ */
