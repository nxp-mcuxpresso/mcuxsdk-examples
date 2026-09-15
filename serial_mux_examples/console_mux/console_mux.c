/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * console_mux.c - Single-UART, all-virtual console mux demo
 * (imx943evk, MCUXpresso SDK), refactored onto component.serial_mux.transport.
 *
 * All THREE cores expose their console through the ONE physical UART owned by
 * the hub (LPUART8 on cm33_core1). Each core's console maps to one logical
 * COBS channel:
 *
 *   channel MUX_SLOTS_CORES (=2) -> cm33_core1 (hub's own debug output)
 *   channel 0 -> cm7_core0  (client, virtual backend)
 *   channel 1 -> cm7_core1  (client, virtual backend)
 *
 * The reusable hub UART rings, RX ISR body, self-console emit and client
 * virtual-console attach now live in component.serial_mux.transport. This demo
 * only supplies board wiring (LPUART instance, IRQ, clock, baud, region) and a
 * one-line ISR forwarder, proving other demos can reuse the same glue.
 *
 * The build define MUX_CORE_INDEX selects the role:
 *   MUX_CORE_INDEX == -1 -> cm33_core1, the Mux Hub (owns LPUART1).
 *   MUX_CORE_INDEX ==  0 -> cm7_core0, virtual console client, channel 0.
 *   MUX_CORE_INDEX ==  1 -> cm7_core1, virtual console client, channel 1.
 *
 * All comments are ASCII-only English per workspace convention.
 */

#include "fsl_lpuart.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "mux_slots.h"
#include "mux_uart_transport.h"

/*
 * MUX_CORE_INDEX selects this core's role and must be injected per core as a
 * compiler define (never defaulted here): an undefined macro is a hard build
 * error so a core cannot silently inherit the wrong role/channel.
 *
 * Define it in the example's per-core reconfig.cmake, e.g.:
 *   mcux_add_configuration(CC "-DMUX_CORE_INDEX=-1")  # cm33_core1: mux hub
 *   mcux_add_configuration(CC "-DMUX_CORE_INDEX=0")   # cm7_core0 : client, channel 0
 *   mcux_add_configuration(CC "-DMUX_CORE_INDEX=1")   # cm7_core1 : client, channel 1
 * Value -1 is the hub; 0..(MUX_SLOTS_CORES-1) are virtual-console clients.
 */
#ifndef MUX_CORE_INDEX
#error "MUX_CORE_INDEX is not defined. Inject it per core via the example reconfig.cmake, e.g. mcux_add_configuration(CC \"-DMUX_CORE_INDEX=-1\") for the cm33_core1 hub, =0 for cm7_core0 (channel 0), =1 for cm7_core1 (channel 1)."
#endif

/*
 * Board-specific UART-MUX wiring. These are supplied by the board layer
 * (examples/_boards/<board>/.../<core>/app.h), never defaulted here: an
 * undefined macro is a hard build error so a new board cannot silently
 * inherit the wrong LPUART instance or line rate.
 *
 * MUX_BAUDRATE is required on every core (hub and clients agree on it);
 * the LPUART instance/IRQ/clock macros are required on the hub only and
 * are checked inside the hub build below.
 */
#ifndef MUX_BAUDRATE
#error "MUX_BAUDRATE must be defined by the board app.h"
#endif

/*
 * Logical channel that carries the hub's (cm33_core1) own console. Clients
 * occupy channels 0..MUX_SLOTS_CORES-1, so the hub takes the next free
 * channel MUX_SLOTS_CORES, which no client core owns. This avoids a
 * collision with cm7_core0 (channel 0).
 */
#ifndef MUX_HUB_CONSOLE_CHANNEL
#define MUX_HUB_CONSOLE_CHANNEL ((uint8_t)MUX_SLOTS_CORES)
#endif

/*
 * Shared slot region at the fixed shared-RAM base configured via
 * CONFIG_MUX_REGION_ADDR (see the serial_mux component). The component
 * header mux_slots.h enforces that MUX_REGION_ADDR is defined; a missing
 * address is a build error there, not here.
 */
static mux_slots_region_t *const g_region =
    (mux_slots_region_t *)(MUX_REGION_ADDR);

#if (MUX_CORE_INDEX < 0)
/* ================= HUB (cm33_core1, owns LPUART1) ================= */

/* Hub build: the physical UART wiring must come from the board app.h. */
#ifndef MUX_LPUART
#error "MUX_LPUART must be defined by the board hub app.h"
#endif
#ifndef MUX_LPUART_IRQn
#error "MUX_LPUART_IRQn must be defined by the board hub app.h"
#endif
#ifndef MUX_LPUART_IRQHandler
#error "MUX_LPUART_IRQHandler must be defined by the board hub app.h"
#endif
#ifndef MUX_LPUART_CLK_FREQ
#error "MUX_LPUART_CLK_FREQ must be defined by the board hub app.h"
#endif

static mux_uart_hub_t g_hub;

/* Board-fixed vector name: forward the ISR to the reusable transport body. */
void MUX_LPUART_IRQHandler(void)
{
    MUX_UART_HubIrq(&g_hub);
}

#if !defined(CONFIG_MUX_HUB_DEDICATED_CONSOLE)
/* Decoded downlink frame on the hub's own console channel (no owning core).
 * Echo the PC's bytes straight back on the hub self channel so the operator
 * sees what they typed, mirroring how the clients echo their own channels.
 * Shared-mux mode only: in the dedicated-console build the hub speaks on its
 * own physical UART, so this self-channel echo handler is not compiled. */
static void hub_self_echo(uint8_t channel, const uint8_t *payload,
                          uint32_t len, void *user)
{
    mux_uart_hub_t *t = (mux_uart_hub_t *)user;

    (void)channel;
    if (t == NULL || payload == NULL || len == 0u) {
        return;
    }
    MUX_UART_HubEmit(t, "%.*s", (int)len, (const char *)payload);
}
#endif


int main(void)
{
    /* BOARD_InitHardware sets up the LPUART1 pins/clock (mux bridge). The hub
     * has no local debug console: its own output goes out on the dedicated
     * hub channel MUX_SLOTS_CORES, distinct from every client channel. */
    BOARD_InitHardware();

    /* Publish the shared region for the client cores, then bring up the hub
     * UART transport in one call. */
    MUX_SLOTS_HubInit(g_region);

    const mux_uart_hub_cfg_t cfg = {
        .uart         = MUX_LPUART,
        .irqn         = MUX_LPUART_IRQn,
        .clk_freq     = MUX_LPUART_CLK_FREQ,
        .baud         = MUX_BAUDRATE,
        .region       = g_region,
        .self_channel = (uint8_t)MUX_HUB_CONSOLE_CHANNEL,
    };
    MUX_UART_HubInit(&g_hub, &cfg);

#if defined(CONFIG_MUX_HUB_DEDICATED_CONSOLE)
#if !defined(MUX_HUB_CONSOLE_LPUART_INSTANCE_IDX)
#error "CONFIG_MUX_HUB_DEDICATED_CONSOLE is set but the board hub app.h did not define MUX_HUB_CONSOLE_LPUART_INSTANCE_IDX"
#endif
#if !defined(MUX_HUB_CONSOLE_BAUDRATE)
#error "CONFIG_MUX_HUB_DEDICATED_CONSOLE is set but the board hub app.h did not define MUX_HUB_CONSOLE_BAUDRATE"
#endif
#if !defined(MUX_HUB_CONSOLE_CLK_FREQ)
#error "CONFIG_MUX_HUB_DEDICATED_CONSOLE is set but the board hub app.h did not define MUX_HUB_CONSOLE_CLK_FREQ"
#endif
    /* Dedicated-console mode: the hub owns a SECOND physical UART for its own
     * console (board app.h wiring) and speaks on it with standard PRINTF. The
     * mux UART carries only the client virtual channels, so no self-channel
     * handler is registered and nothing is emitted on the mux self channel. */
    (void)DbgConsole_Init(MUX_HUB_CONSOLE_LPUART_INSTANCE_IDX,
                          MUX_HUB_CONSOLE_BAUDRATE, kSerialPort_Uart,
                          MUX_HUB_CONSOLE_CLK_FREQ);
    PRINTF("mux hub up: core_id=%d (hub, dedicated console UART)\r\n",
           (int)MUX_CORE_INDEX);
#else
    /* Shared-mux mode: the hub's own console is one more COBS channel on the
     * same physical mux UART. Route PC input on that channel to the echo
     * handler; otherwise the hub would silently drop PC input on it. */
    MUX_HUB_SetSelfHandler(&g_hub.hub, hub_self_echo, &g_hub);

    /* Announce the hub on its own dedicated console channel. */
    MUX_UART_HubEmit(&g_hub,
                     "mux hub up: core_id=%d (hub, channel %u)\r\n",
                     (int)MUX_CORE_INDEX, (unsigned)MUX_HUB_CONSOLE_CHANNEL);
#endif

    /* Main loop: one service call drains uplink rings + the TX ring. */
    for (;;) {
        if (MUX_UART_HubService(&g_hub) == 0u) {
            __NOP();
        }
    }
}

#else
/* ============ VIRTUAL CONSOLE CLIENT (cm7_core0 / cm7_core1) ============ */

static mux_client_t g_client;

int main(void)
{
    /* cm7 clients do minimal init: no local UART, only system/clock bring-up. */
    BOARD_InitHardware();

    /* The muxuart HAL UART adapter provides the console, so this is the
     * ordinary debug_console_lite bring-up. The HAL UART "instance" is this
     * core's 0-based index, i.e. its mux channel; the adapter attaches the
     * underlying serial_mux client itself, and the source clock is irrelevant
     * because the hub owns the physical wire. */
    (void)g_client;
    (void)g_region;
    DbgConsole_Init((uint8_t)MUX_CORE_INDEX, MUX_BAUDRATE, kSerialPort_Uart, 0U);

    PRINTF("client up: core_id=%d (channel %d)\r\n",
           (int)MUX_CORE_INDEX, (int)MUX_CORE_INDEX);

    for (;;) {
        /* Echo anything the PC sent on this core's channel. */
        int c = DbgConsole_Getchar();
        if (c >= 0) {
            PRINTF("%c", (char)c);
        }
    }
}

#endif /* MUX_CORE_INDEX */
