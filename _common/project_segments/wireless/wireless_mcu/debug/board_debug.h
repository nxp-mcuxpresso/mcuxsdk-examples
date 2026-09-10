/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_DEBUG_H_
#define _BOARD_DEBUG_H_

/*!
 * @addtogroup FWK_Board_module
 * The FWK_Board module
 *
 * FWK_Board module provides APIs to config platform functionalities and peripherals.
 * @{
 */
/*!
 * @addtogroup FWK_Board
 * The FWK_Board main module
 *
 * FWK_Board main module provides APIs to initialise hardware and manage serial consoles.
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * \brief Initializes SWO pins.
 *
 */
void BOARD_InitSWO(void);

/*!
 * \brief Deinitializes SWO pins.
 *
 * \details Prevents leakage in lowpower caused by SWO pins in pull down mode.
 *
 */
void BOARD_DeInitSWO(void);

#if defined(BOARD_DBG_FAULT_SINK_ENABLE) && (BOARD_DBG_FAULT_SINK_ENABLE != 0)
/*!
 * \brief Route the framework fault-handler diagnostic text to the debug log engine.
 *
 * \details Registers an output sink with the framework fault handler so its
 * human-readable register/fault dump is written through the shared debug log
 * engine, over the same port the engine uses. This is independent of the
 * coredump feature: it works whether or not the coredump backend is enabled.
 * Call once at boot, after BOARD_DbgLoggerInit(). A fault that fires before
 * this call produces no fault-report text.
 *
 */
void BOARD_DbgFaultSinkInit(void);
#endif

/*!
 * @}  end of FWK_Board addtogroup
 */
/*!
 * @}  end of FWK_Board_module addtogroup
 */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
