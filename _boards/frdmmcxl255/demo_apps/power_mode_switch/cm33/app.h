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
#define DEMO_POWER_MODE_NAME {"Active", "Sleep", "Deep Sleep", \
                              "Power Down1", "Power Down2", \
                              "Deep Power Down1", "Deep Power Down2", \
                              "Deep Power Down3", "Shut Down"}
#define DEMO_POWER_MODE_DESC { \
      "Active: All power domains active", \
      "Sleep:" ,\
      "Deep Sleep: CM33 is off but the power and state of all registers are kept alive. Power consumption: 44uA ", \
      "Power Down 1:", \
      "Power Down 2:", \
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

#define APP_MU MUA
#define APP_MU_CHANNEL (0U)

//#define DEMO_CORE_FREQ_HZ (96000000U)
//
//
//#define DEMO_WAKEUP_PIN_NAME ("SW2")
//#define DEMO_WAKEUP_PIN_WS_ID (kPower_WS_P1_16FallEdgeInt)
//
//#define DEMO_EXT_INT_PIN_NAME ("SW4")
//#define DEMO_EXT_INT_PIN_WS_ID (kPower_WS_ExternalINTFallEdge)
//        
//#define DEMO_DPD1_RETAINED_RAM_BLOCKS   (0x02U)
//#define DEMO_DPD2_RETAINED_RAM_BLOCKS   (0x02U)

#define CORE1_BOOT_ADDRESS      (0xA1000000UL)

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
extern unsigned char core1_image_start[];
#define CORE1_IMAGE_START core1_image_start
#elif defined(__GNUC__)
extern const char core1_image_start[];
extern const char *core1_image_end;
extern int core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_BootCore1(void);
uint32_t get_core1_image_size(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
