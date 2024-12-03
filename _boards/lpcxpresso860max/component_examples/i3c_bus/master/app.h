/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_MASTER           I3C0
#define EXAMPLE_I3C_OD_BAUDRATE  1000000U
#define EXAMPLE_I3C_PP_BAUDRATE  2000000U
#define I3C_CLOCK_FREQUENCY      CLOCK_GetI3cClkFreq()
#define I3C_SLOW_CLOCK_FREQUENCY CLOCK_GetLpOscFreq()
#define FSL_FEATURE_I3C_HAS_IBI_PAYLOAD_SIZE_OPTIONAL_BYTE (1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
