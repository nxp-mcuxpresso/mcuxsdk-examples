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
#define DEMO_POWER_MODE_NAME {"Active", "Deep Sleep", \
                              "Deep Power Down1", "Deep Power Down2", \
                              "Deep Power Down3", "Shut Down"}
#define DEMO_POWER_MODE_DESC { \
      "Active: All power domains active", \
      "Deep Sleep: CM33 is off but the power and state of all registers are kept alive. Power consumption: 44uA ", \
      "Deep Power Down 1: CM33 complete sub clusters is power off, AON domain is active. Power consumption: 13uA ", \
      "Deep Power Down 2: Main domain is powered off, the AON subsystem is power off and the AON ADVC block is active. Power consumption: 7uA", \
      "Deep Power Down 3: Both the AON and Main domain are powered off, only RTC and PMU analog control are alive.", \
      "Shut Down: All blocks including CPU subsystems, AON and RTC are off and only the PAC is active."   \
}
#define DEMO_USER_DATA  (0xAU)

#define DEMO_WAKEUP_REASON      {       \
      "Deep Power Down1", \
      "Deep Power Down2", \
      "Deep Power Down3", \
      "Shut Down Or POR", \
}


#define DEMO_CORE_FREQ_HZ (12000000U)


#define DEMO_WAKEUP_PIN_NAME ("SW2")
#define DEMO_WAKEUP_PIN_WS_ID (kPower_WS_P1_16FallEdgeInt)

#define DEMO_EXT_INT_PIN_NAME ("SW4")
#define DEMO_EXT_INT_PIN_WS_ID (kPower_WS_ExternalINTFallEdge)
        
#define DEMO_DPD1_RETAINED_RAM_BLOCKS   (0x02U)
#define DEMO_DPD2_RETAINED_RAM_BLOCKS   (0x02U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
