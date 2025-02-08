/**
 * Copyright 2018-2025 NXP.
 * NXP Confidential and Proprietary.
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef _HCI_UART_CONFIG_H_
#define _HCI_UART_CONFIG_H_
#include "fsl_usart.h"

#define HCI_UART_BAUDRATE (230400u)

/*! @brief Enum for UART prefilter states. */
typedef enum hci_uart_prefilter_s
{
    kPREFILTER_ALLOW_SENDING,
    kPREFILTER_BLOCK_SENDING
} hci_uart_prefilter_t;

/*! @brief Callback function pointer type for UART events. */
typedef void (*uartCbFunc_t)(void);

/*! @brief Function pointer type for write prefilter function. */
typedef hci_uart_prefilter_t (*ll_uart_write_prefilter_t)(uint8_t *buffer, uint16_t size);

/*! @brief Structure to hold UART callback functions. */
typedef struct
{
    uartCbFunc_t txCb;
    uartCbFunc_t rxCb;
} hci_uart_cb_t;

/**
 * @brief Initializes the UART interface.
 * @param uartCb Structure holding pointers to callback functions.
 */
void ll_uart_init(hci_uart_cb_t uartCb);

/**
 * @brief Deinitializes the UART interface.
 */
void ll_uart_deinit(void);

/**
 * @brief Reads data from the UART interface.
 *
 * @param buffer Pointer to the buffer where data will be stored.
 * @param size   Size of the buffer.
 */
void ll_uart_read(uint8_t *buffer, uint16_t size);

/**
 * @brief Writes data to the UART interface.
 *
 * @param buffer Pointer to the data buffer to be sent.
 * @param size   Size of the data buffer.
 */
void ll_uart_write(uint8_t *buffer, uint16_t size);

/**
 * @brief Registers a write prefilter function.
 *
 * @param write_preprocessor Function pointer to the write prefilter function.
 */
void ll_uart_register_write_prefilter(ll_uart_write_prefilter_t write_preprocessor);

/**
 * @brief Unregisters the write prefilter function.
 */
void ll_uart_unregister_write_prefilter(void);

#endif /* _HCI_UART_CONFIG_H_ */
