/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Example Application
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "pin_mux.h"
#include "board.h"

#include "lwip/opt.h"

#include "freemaster.h"
#include "network.h"
#include "freemaster_example.h"
#include "freemaster_net.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/* Stack size of the temporary lwIP initialization thread. */
#define EXAMPLE_THREAD_STACKSIZE        1024

/* Priority of the temporary lwIP initialization thread. */
#define EXAMPLE_FMSTR_THREAD_PRIO       3

/* Priority of the temporary lwIP initialization thread. */
#define EXAMPLE_APP_THREAD_PRIO         2

/* Clock frequency */
#define EXAMPLE_NETWORK_CLOCK_FREQ      CLOCK_GetRootClockFreq(kCLOCK_Root_COMM_comm_clk)

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! Note: All global variables accessed by FreeMASTER are defined in a shared
//! freemaster_example.c file

static FMSTR_BOOL fmstr_initialized = FMSTR_FALSE;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static void fmstr_task(void *arg);
static void example_task(void *arg);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    FMSTR_NET_IF_CAPS caps;
    memset(&caps, 0, sizeof(caps));

    /* Board initialization */
    BOARD_CommonSetting();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitENETPins();
    BOARD_Init6524Pins();

    pcal6524_handle_t* pcal6524Handle = BOARD_GetPCAL6524Handle();
    BOARD_InitPCAL6524(pcal6524Handle);

    /* Refresh SystemCoreClock from the configured clock tree; otherwise it stays at the static
     * DEFAULT_SYSTEM_CLOCK and SysTick-based timing (e.g. iperf throughput) reads at the wrong rate. */
    SystemCoreClockUpdate();
    BOARD_InitDebugConsole();

    /* The shared clock tree leaves ETH1_TRXCLK at 250MHz; the ENET1G RGMII RX delay-line reference
     * must be 125MHz (RM), so re-divide it here rather than in the board-wide clock config. */
    {
        clock_root_config_t trxCfg = {.mux = kCLOCK_ETH1_TRXCLK_ClockRoot_MAINPLLDIV8, .div = 2U, .sndDiv = 1U};
        CLOCK_SetRootClock(kCLOCK_Root_COMM_eth1_trxclk, &trxCfg);
    }

    /* Grant the ENET1 masters TRDC domain 0; without this the ENET DMA bus accesses fault. */
    COMM__TRDC->MDA_DFMT1[kTRDC_COMM_MasterENET1_M0R].MDA_W_DFMT1[0] = TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    COMM__TRDC->MDA_DFMT1[kTRDC_COMM_MasterENET1_M0T].MDA_W_DFMT1[0] = TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;

    /* Reset YT8531 PHY via PCAL6524 P2_2 (BOARD_PCAL6524_ETH1_RST_B) */
    {
        uint32_t rstMask = 1U << BOARD_PCAL6524_ETH1_RST_B;
        (void)PCAL6524_SetDirection(pcal6524Handle, rstMask, kPCAL6524_Output);
        (void)PCAL6524_ClearPins(pcal6524Handle, rstMask);
        SDK_DelayAtLeastUs(10000U, CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk));
        (void)PCAL6524_SetPins(pcal6524Handle, rstMask);
        SDK_DelayAtLeastUs(30000U, CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk));
    }

    /* FreeMaster task */
    if(xTaskCreate(fmstr_task, "fmstr_task", EXAMPLE_THREAD_STACKSIZE, NULL, EXAMPLE_FMSTR_THREAD_PRIO, NULL) == pdFAIL)
        LWIP_ASSERT("fmstr_task: Task creation failed.", 0);

    /* Example application task */
    if(xTaskCreate(example_task, "example_task", EXAMPLE_THREAD_STACKSIZE, NULL, EXAMPLE_APP_THREAD_PRIO, NULL) == pdFAIL)
        LWIP_ASSERT("example_task: Task creation failed.", 0);

    FMSTR_ASSERT_RETURN(FMSTR_NET_DRV.GetCaps != NULL, 0);
    FMSTR_NET_DRV.GetCaps(&caps);
    
    PRINTF("\n\nFreeMaster %s %s Example\n\n", 
           ((caps.flags & FMSTR_NET_IF_CAPS_FLAG_UDP) != 0U ? "UDP" : "TCP"), 
           (FMSTR_NET_BLOCKING_TIMEOUT == 0 ? "Non-Blocking" : "Blocking"));

    vTaskStartScheduler();

    /* Will not get here unless a task calls vTaskEndScheduler ()*/
    return 0;
}

/*
 * FreeMASTER Example application task.
 *
 * This task runs a generic FreeMASTER example code - increments several
 * variables so they can be monitored using the FreeMASTER PC Host tool.
 * Note that the FALSE is passed to generic calls, so that the functions
 * do not call FMSTR_Init() and FMSTR_Poll() API - this is called in a
 * task dedicated to FreeMASTER communication.
 */
static void example_task(void *arg)
{
    while (!fmstr_initialized)
    {
        vTaskDelay(10);
    };

    /* Generic example initialization code */
    FMSTR_Example_Init_Ex(FMSTR_FALSE);

    while (1)
    {
        /* Increment test variables periodically, use the
           FreeMASTER PC Host tool to visualize the variables */
        FMSTR_Example_Poll_Ex(FMSTR_FALSE);

        /* Check the network connection and DHCP status periodically */
        Network_Poll();
    }
}

/*
 * FreeMASTER task.
 *
 * Network communication takes place here. This task sleeps when waiting
 * for a communication and lets the other example tasks to run.
 */
static void fmstr_task(void *arg)
{
    /* Network interface initialization */
    Network_Init(EXAMPLE_NETWORK_CLOCK_FREQ);

    /* FreeMASTER driver initialization */
    FMSTR_Init();

    fmstr_initialized = FMSTR_TRUE;

    while (1)
    {
        /* The FreeMASTER poll handles the communication interface and protocol
           processing. This call will block the task execution when no communication
           takes place (also see FMSTR_NET_BLOCKING_TIMEOUT option) */
        FMSTR_Poll();

        /* When no blocking timeout is specified, the FMSTR_Poll() returns
           immediately without any blocking. We need to sleep to let other
           tasks to run. */
#if FMSTR_NET_BLOCKING_TIMEOUT == 0
        vTaskDelay(1);
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////
