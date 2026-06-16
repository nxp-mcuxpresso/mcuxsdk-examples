/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${function:start}*/
/* Override the weak SystemInit from system_MCXW236.c.
 * The default implementation writes to SYSCON->TRACECLKDIV (0x40000308),
 * which is a Secure-only peripheral (APB Bridge 0 slot 0 = Secure in tzm_config.c).
 * The NS startup (IAR crt0) calls SystemInit before main(), so without this override
 * the NS world immediately faults with BFAR = 0x40000308. */
void SystemInit(void)
{
}

/* Secure world already initialized all clocks, pins, power, and the debug console.
 * NS debug output uses DbgConsole_Printf_NSE (NSC callback into secure world),
 * so no local UART/console init is needed here. Calling POWER_Init(), CLOCK_EnableClock(),
 * BOARD_InitBootClocks(), or BOARD_InitDebugConsole() from NS world would access
 * Secure-only SYSCON registers and trigger another bus fault. */
void BOARD_InitHardware(void)
{
}
/*${function:end}*/
