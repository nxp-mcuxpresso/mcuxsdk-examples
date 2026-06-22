/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
/*${header:end}*/


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_MU              MU2_A

#define BOOT_CORE1_BY_MU    0

/* Flash base from which the secondary core (M7_1) boots and runs in place. The
 * secondary image is embedded in the primary flash image and linked to this
 * address (see MCXE32B_cm7_core1_flash linker and the primary .core1_code /
 * __core1_image placement). APP_BootCore1() releases M7_1 by writing this base
 * to MC_ME PRTN0_CORE1_ADDR; hardware fetches the initial SP/PC from here. No
 * copy to RAM and no TCM backdoor are involved. */
#define CORE1_BOOT_ADDRESS  0x00600000



#define LED_INIT()          //LED is toggled by the secondary core in this example

#define LED_TOGGLE()        //LED is toggled by the secondary core in this example

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
#pragma section = "__core1_image"
#define CORE1_IMAGE_START __section_begin("__core1_image")
#elif (defined(__GNUC__)) && (!defined(__MCUXPRESSO))
extern const char core1_image_start[];
extern const char *core1_image_end;
extern uint32_t core1_image_size;
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
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif
/*${prototype:end}*/

#endif /* _APP_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
