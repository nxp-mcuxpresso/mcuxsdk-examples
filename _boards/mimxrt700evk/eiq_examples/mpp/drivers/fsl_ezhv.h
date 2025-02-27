/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSL_EZHV_H_
#define FSL_EZHV_H_

#include "fsl_common.h"

/*!
 * @addtogroup ezhv
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief cache driver version. */
#define FSL_EZHV_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/

/*! @brief . */

/*! @brief Callback function prototype for the ezhv driver. */
typedef void (*ezhv_callback_t)(void *param);

/*!
 * @brief Structure for EZHV copy image to destination address
 *
 * Defines source and destination address for copying image with given size.
 */
typedef struct _ezhv_copy_image
{
    uint32_t srcAddr;
    uint32_t destAddr;
    uint32_t size;
} ezhv_copy_image_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * brief Initialize the EZHV.
 *
 * param ezhvCopyImage The address firmware will be copied to
 */
void EZHV_Init(ezhv_copy_image_t *ezhvCopyImage);

 /*!
 * brief Initialize the EZHV.
 *
 * This function is similar with EZHV_Init, the difference is this function
 * does not install the firmware, the firmware could be installed using
 * EZHV_InstallFirmware.
 */
void EZHV_InitWithoutFirmware(void);

/*!
 * brief install EZHV firmware by given image info
 *
 * param ezhvCopyImage The address firmware will be copied to
 */
void EZHV_InstallFirmware(ezhv_copy_image_t *ezhvCopyImage);

/*!
 * brief install EZHV firmware by given image info
 *
 * param bootAddr the boot addr
 * param para, reserved para
 */
void EZHV_Boot(uint32_t bootAddr, void *para);

/*!
 * brief Deinitialize the EZHV.
 */
void EZHV_Deinit(void);

/*!
 * brief Install the complete callback function..
 *
 * param callback The callback called when ezhv program finished.
 * param param parameter for callback function
 */
void EZHV_InstallCallback(ezhv_callback_t callback, void *param);

/*!
 * brief Install the complete callback function..
 *
 * return the base address of arm2ezhv paramter
 */
uint32_t *EZHV_GetParaAddr(void);

/*!
 * brief Install the complete callback function..
 *
 * param para, parameter written into shared mem between arm and ezhv
 * param size, how many bytes of the paramters
 */
void EZHV_InitPara(void*para, uint32_t size);
/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif
