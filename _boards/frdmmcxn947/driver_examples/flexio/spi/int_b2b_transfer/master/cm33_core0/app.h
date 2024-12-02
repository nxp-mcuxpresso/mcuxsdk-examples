/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_FLEXIO_BASE      (FLEXIO0)
#define FLEXIO_SPI_MOSI_PIN    28U
#define FLEXIO_SPI_MISO_PIN    29U
#define FLEXIO_SPI_SCK_PIN     30U
#define FLEXIO_SPI_CSn_PIN     31U
#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetFlexioClkFreq()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
