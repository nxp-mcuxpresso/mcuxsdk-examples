/*
 * Copyright 2025 NXP
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
#define APP_MU MUB
#define APP_MU_CHANNEL (0)
   
#define APP_POWER_MODE_NAME {"Sleep", "Deep Sleep", \
                              "Power Down1", "Power Down2", \
                              "Deep Power Down1", "Deep Power Down2", \
                              "Deep Power Down3", "Shut Down"}
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
