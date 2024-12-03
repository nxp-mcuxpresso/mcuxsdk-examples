/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#ifndef _LPM_H_
#define _LPM_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*
 * NCP handshake process definition
 * 0 -> not start
 * 1 -> in process
 * 2 -> finish
 * 3 -> time wait
 *
 * NCP LPM handshake state machine:
 *      ------> NCP_LMP_HANDSHAKE_NOT_START <---------------------(6)
 *     |                     |                                     |
 *     |                    (1)                                    |
 *     |                     |                                     |
 *     |                    \|/                                    |
 *    (3)       NCP_LMP_HANDSHAKE_IN_PROCESS --(3)--> NCP_LMP_HANDSHAKE_TIME_WAIT <---
 *     |                     |                                /|\     |               |
 *     |                    (2)                                |      |               |
 *     |                     |                                 |       -------(5)-----
 *     |                    \|/                                |
 *      ------- NCP_LMP_HANDSHAKE_FINISH ---------------------(4)
 *
 *
 * (1)-> IDLE task notifies PM enter >= PM2, IDLE task notifies SLEEP_ENTER event to NCP host.
 * (2)-> NCP device receives NCP_EVENT_MCU_SLEEP_CFM before LPM timer expires.
 * (3)-> NCP device resumes from WFI,IDLE task notifies SLEEP_EXIT event to NCP host.
 * (4)-> LPM timer expires after NCP device/host handshake finish, still keep original PM constrain.
 * (5)-> NCP device receives SLEEP_CFM, still keep original PM constrain.
 * (6)-> NCP device resumes from WFI, IDLE task notifies SLEEP_EXIT event to NCP host, releases the original PM constrain.
 */
#define NCP_LMP_HANDSHAKE_NOT_START  0
#define NCP_LMP_HANDSHAKE_IN_PROCESS 1
#define NCP_LMP_HANDSHAKE_FINISH     2
#define NCP_LMP_HANDSHAKE_TIME_WAIT  3

#if defined(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY)
#ifndef LPM_RTC_PIN1_PRIORITY
#define LPM_RTC_PIN1_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#endif
#else
#ifndef LPM_RTC_PIN1_PRIORITY
#define LPM_RTC_PIN1_PRIORITY (3U)
#endif
#endif

/*******************************************************************************
 * API
 ******************************************************************************/
void lpm_pm3_exit_hw_reinit();
int LPM_Init(void);

uint8_t lpm_getHandshakeState(void);
void lpm_setHandshakeState(uint8_t state);
void LPM_ConfigureNextLowPowerMode(uint8_t nextMode, uint32_t timeS);
#endif /* _LPM_H_ */
