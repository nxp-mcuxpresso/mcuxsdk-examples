/*!
 * Copyright 2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file app.h
 * \brief provides applicative API for wireless NBU applications
 *
 */

#ifndef _APP_H_
#define _APP_H_

/*!
 * \brief This function must be implemented by the application.
 * It shall contain every initialization required before starting
 * the ThreadX scheduler. The kernel primitives are not available.
 *
 */
void APP_PreKernelInit(void);

/*!
 * \brief This function must be implemented by the application.
 * It shall contain every initialization required once the ThreadX
 * scheduler has been started. The kernel primitives and context
 * switching are available.
 *
 */
void APP_PostKernelInit(void);

/*!
 * \brief This function must be implemented by the application.
 * This function is called from the idle task implemented in main.c.
 *
 */
void APP_Idle(void);

/*!
 * \brief This function must be implemented by the application.
 * This function is called from the main task implemented in main.c after APP_PostKernelInit().
 * If this function returns, the main task will exit and stop running, so if the application needs
 * to keep it alive, it can implement a while loop as any other thread.
 *
 */
void APP_Main(void);

/*!
 * \brief This function in usually implemented in hardware_init.c and
 * contains hardware initialization like clocks and peripherals.
 *
 */
void BOARD_InitHardware(void);

#endif /* _APP_H_ */
