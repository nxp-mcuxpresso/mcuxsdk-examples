/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_XBIC             XBIC_AXBS
#define DEMO_XBIC_MASTER_PORT kXbicAxbsMasterCortexM70Ahbm
#define DEMO_XBIC_SLAVE_PORT  kXbicAxbsSlavePram0
#define DEMO_XBIC_SYNDROMES   kXBIC_SynHwrite
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
