/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "mc_periph_init.h"
#include "freemaster.h"
#include "freemaster_serial_lpuart.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_rgpio.h"
#include "fsl_lpuart.h"
#include "m1_sm_servo.h"
#include "m2_sm_servo.h"
#include "board.h"
#include "mu_ipc_shm_client.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

/* Example's feature set in form of bits inside ui16featureSet.
   This feature set is expected to be growing over time.
   ... | FEATURE_S_RAMP | FEATURE_FIELD_WEAKENING | FEATURE_ENC
*/
#define FEATURE_ENC (1)               /* Encoder feature flag */
#define FEATURE_FIELD_WEAKENING (0)   /* Field weakening feature flag */
#define FEATURE_S_RAMP (0)            /* S-ramp feature flag */

#define FEATURE_SET (FEATURE_ENC << (0) | \
                     FEATURE_FIELD_WEAKENING << (1) | \
                     FEATURE_S_RAMP << (2))

/*! @brief The UART to use for FreeMASTER communication */
#define _FMSTR_LPUART_BASE_ADDR(x)   LPUART##x
#define _FMSTR_LPUART_IRQn(x)        LPUART##x##_IRQn
#define _FMSTR_LPUART_IRQ_HANDLER(x) LPUART##x##_IRQHandler

#define FMSTR_LPUART_BASE_ADDR(x)    _FMSTR_LPUART_BASE_ADDR(x)
#define FMSTR_LPUART_IRQn(x)         _FMSTR_LPUART_IRQn(x)
#define FMSTR_LPUART_IRQ_HANDLER(x)  _FMSTR_LPUART_IRQ_HANDLER(x)

#define BOARD_USER_BUTTON_PRIORITY 4

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1)   \
    uint32_t val  = SysTick->VAL;  \
    uint32_t load = SysTick->LOAD; \
    par1          = load - val

/* Three instruction added after interrupt flag clearing as required */
#define M1_END_OF_ISR \
    {                 \
        __DSB();      \
        __ISB();      \
    }


/*******************************************************************************
 * Variables
 ******************************************************************************/
static int64_t i64EthercatPosTargetM1 = 0x0;
static int64_t i64EthercatPosTargetM2 = 0x0;
static int32_t i32EthercatM1PosCurrMT = 0x0;
static int32_t i32EthercatM2PosCurrMT = 0x0;
static int32_t i32EthercatM1PosOldST = -1;
static int32_t i32EthercatM2PosOldST = -1;
static int32_t i32InitializedM1 = 0;
static int32_t i32InitializedM2 = 0;
extern struct param_t *g_param;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../../../examples/_boards/imx943evk/ecat_examples/dual_cores_servo_motor/remote/cm7_core0",        /* User Path 1- the highest priority */
    "",       /* User Path 2 */
    "imx943evk", /* board id */
    "pmsm_servo_dual",     /* example id */
    MCRSP_VER,      /* sw version */
    FEATURE_SET,    /* example's feature-set */
};


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void init_freemaster_lpuart(void);

static void BOARD_InitSysTick(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

int32_t GetM1PositionActualValue(void)
{
    return (M1_MCDRV_ENC_GET_POSITION(&g_sM1Enc) | (i32EthercatM1PosCurrMT << 25))& 0xFFFFFFFF;
}

int32_t GetM2PositionActualValue(void)
{
   return (M2_MCDRV_ENC_GET_POSITION(&g_sM2Enc)  | (i32EthercatM2PosCurrMT << 25))& 0xFFFFFFFF;
}

acc32_t GetM1PositionCmdValue(int32_t targetPos)
{
    int32_t i32MultTurn = 0;
    uint16_t u16SingleTurn = 0;
    int32_t delta;
    delta = ((int32_t)targetPos - (int32_t)(i64EthercatPosTargetM1 & 0xFFFFFFFF));
    i64EthercatPosTargetM1 += delta;
    i32MultTurn = (i64EthercatPosTargetM1 >> 25) & 0x1FFFFFFFF ;
    u16SingleTurn = (i64EthercatPosTargetM1 >> 10) & 0x7FFF;
    return (acc32_t)((i32MultTurn << 15 ) + u16SingleTurn);
}

acc32_t GetM2PositionCmdValue(int32_t targetPos)
{
    int32_t i32MultTurn = 0;
    uint16_t u16SingleTurn = 0;
    int32_t delta;
    delta = ((int32_t)targetPos - (int32_t)(i64EthercatPosTargetM2 & 0xFFFFFFFF));
    i64EthercatPosTargetM2 += delta;
    i32MultTurn = (i64EthercatPosTargetM2 >> 25) & 0x1FFFFFFFF ;
    u16SingleTurn = (i64EthercatPosTargetM2 >> 10) & 0x7FFF;
    return (acc32_t)((i32MultTurn << 15 ) + u16SingleTurn);
}

int Cia402_status_machine_trans(uint8_t axis, uint8_t trans_id, struct param_t *g_param)
{
   int ret = 0;
   if (axis == 0) {
       switch (trans_id) {
           case 0: break;
           case 1: break;
           case 2: break;
           case 3:
               M1_OpenPWM();
               break;
           case 4:
               M1_SetAppSwitch(1);
               i64EthercatPosTargetM1 = M1_MCDRV_ENC_GET_POSITION(&g_sM1Enc);
               if (g_sM1Ctrl.eState != kSM_AppRun || g_eM1StateRun != kRunState_Spin)
               {
                   ret = -1;
               }
               break;
           case 5:
           case 6:
               M1_SetAppSwitch(0);
               M1_ClosePWM();
               break;
           case 7:
           case 8:
           case 9:
           case 10:
           case 11:
           case 12:
           case 13:
           case 14:
           case 15:
               M1_ClosePWM();
               break;
       }
   } else if (axis == 1){
       switch (trans_id) {
           case 0: break;
           case 1: break;
           case 2: break;
           case 3: M2_OpenPWM(); break;
           case 4:
               M2_SetAppSwitch(1);
               i64EthercatPosTargetM2 = M2_MCDRV_ENC_GET_POSITION(&g_sM2Enc);
               if (g_sM2Ctrl.eState != kSM_AppRun || g_eM2StateRun != kRunState_Spin) {
                   ret = -1;
               }
               break;
           case 5:
           case 6:
               M2_SetAppSwitch(0);
               M2_ClosePWM();
               break;
           case 7:
           case 8:
           case 9:
           case 10:
           case 11:
           case 12:
           case 13:
           case 14:
           case 15:
               M2_ClosePWM();
               break;
       }
   }
   return ret;
}

int obj_write_callback(uint16_t Index, uint8_t Subindex, uint8_t size, void *pData)
{
    uint8_t *p = pData;
    if (Index == 0x6060 && Subindex == 0x0) {
        if (p[0] == 0x08) {
            g_sM1Drive.eControl = kControlMode_PositionFOC;
        }
    } else if (Index == 0x6860 && Subindex == 0x0) {
        if (p[0] == 0x08) {
            g_sM2Drive.eControl = kControlMode_PositionFOC;
        }
    } 
    return 0;
}

int obj_read_callback(uint16_t Index, uint8_t Subindex, uint8_t size, void *pData)
{
    uint32_t *p = pData;
    if (Index == 0x6064 && Subindex == 0x0) {
        *p = GetM1PositionActualValue();
        return sizeof(*p);
    } else if (Index == 0x606c && Subindex == 0x0) {
        *p = (uint32_t)g_sM1Drive.sSpeed.fltSpeedFilt;
        return sizeof(*p);
    }  else  if (Index == 0x6864 && Subindex == 0x0) {
        *p = GetM2PositionActualValue();
        return sizeof(*p);
    } else if (Index == 0x686c && Subindex == 0x0) {
        *p = (uint32_t)g_sM2Drive.sSpeed.fltSpeedFilt;
        return sizeof(*p);
    }
    return 0;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /*Accessing ID structure to prevent optimization*/
    g_sAppIdFM.ui16FeatureSet = FEATURE_SET;

    uint32_t ui32PrimaskReg;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

	/* Waiting until MU_ipc_shm_master_init() is completed on CM33 core */
    MU_ipc_shm_client_init();
    /* BootPins must be initialzed after MU_ipc_shm_client_init
     * due to BOARD_PCA6416_I2C6 is in conflicat with CM33 core.
    */
    BOARD_InitBootPins();
    /* FreeMASTER communication layer initialization */
    init_freemaster_lpuart();
    FMSTR_Init();
    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();
    /* Init peripheral motor control driver for motors M1 and M2 */
    MCDRV_Init();
    /* Turn off application */
    M1_SetAppSwitch(FALSE);
    M2_SetAppSwitch(FALSE);

    /* Enable interrupts */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Enable PWM clock */
    g_sM1Pwm3ph.pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_RUN(0xF);    /* Enable PWM for Motor 1 */
    g_sM2Pwm3ph.pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_RUN(0xF);    /* Enable PWM for Motor 2 */

    PRINTF("CM7_Core0 SSC dual-cores servo motor example ");
    PRINTF("(Compiled on: %s at %s)\r\n", __DATE__, __TIME__);

    /* Infinite loop */
    while (1)
    {
        if (!i32InitializedM1) {
            if (g_param->axis[0].axis_is_active) {
                M1_MCDRV_SINC_INIT();
                M1_MCDRV_ENCODER_PERIPH_INIT();
                i32InitializedM1 = 1;
            }
        }

        if (!i32InitializedM2) {
            if (g_param->axis[1].axis_is_active) {
                M2_MCDRV_SINC_INIT();
                M2_MCDRV_ENCODER_PERIPH_INIT();
                i32InitializedM2 = 1;
            }
        }
       /* FreeMASTER Polling function */
       FMSTR_Poll();
    }
}

RAM_FUNC_LIB
void M1_ENCODER_IRQHandler(void)
{
    int32_t detal = 0;
    int32_t position;

#if (M1_ENCODER == ENCODER_BISS)
    /* clear BiSS EOT interrupt routed via XBAR */
    XBAR_ClearOutputStatusFlag(kXBAR1_OutputEdma4IpdReq76);
#endif

    M1_MCDRV_SINC_GET(&g_sM1Curr3phDcBus);
    M1_MCDRV_ENCODER_GET(&g_sM1Enc);
    position = M1_MCDRV_ENC_GET_POSITION(&g_sM1Enc);
    if (!(i32EthercatM1PosOldST < 0)) {
        detal = position - i32EthercatM1PosOldST;
        if (detal < -16777216) {
            i32EthercatM1PosCurrMT++;
        }
        else if (detal > 16777216) {
            i32EthercatM1PosCurrMT--;
        }
    }
    i32EthercatM1PosOldST = position;

    /* M1 State machine */
    SM_StateMachineFast(&g_sM1Ctrl);
#if (M1_ENCODER == ENCODER_ENDAT3)
    /* Clear EnDat3 FG_IRQ0 flag */
    ENDAT3_IRQ_Clear(ENDAT3, CLEAR_FG_IRQ0);
#endif  /* EnDat3 encoder is used. */
    SDK_ISR_EXIT_BARRIER;
}

RAM_FUNC_LIB
void M2_ENCODER_IRQHandler(void)
{
    int32_t detal = 0;
    int32_t position;

#if (M2_ENCODER == ENCODER_BISS)
    /* clear BiSS EOT interrupt routed via XBAR */
    XBAR_ClearOutputStatusFlag(kXBAR1_OutputEdma4IpdReq76);
#endif

    M2_MCDRV_SINC_GET(&g_sM2Curr3phDcBus);
    M2_MCDRV_ENCODER_GET(&g_sM2Enc);
    position = M2_MCDRV_ENC_GET_POSITION(&g_sM2Enc);
    if (!(i32EthercatM2PosOldST < 0)) {
        detal = position - i32EthercatM2PosOldST;
        if (detal < -16777216) {
            i32EthercatM2PosCurrMT++;
        }
        else if (detal > 16777216) {
            i32EthercatM2PosCurrMT--;
        }
    }
    i32EthercatM2PosOldST = position;

    /* M2 State machine */
    SM_StateMachineFast(&g_sM2Ctrl);
    FMSTR_Recorder(0);
#if (M2_ENCODER == ENCODER_ENDAT3)
    /* Clear EnDat3 FG_IRQ0 flag */
    ENDAT3_IRQ_Clear(ENDAT3, CLEAR_FG_IRQ0);
#endif  /* EnDat3 encoder is used. */
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief   motor_slow_task called with EtherCAT DC period and processes following functions:
 *           - motor slow application machine function
 */
RAM_FUNC_LIB
int motor_slow_task(int axis, struct pdo_to_motor_t *pdo_m, struct pdo_to_esc_t *pdo_e, struct param_t *param)
{
    if (axis == 0) {
        M1_SetPosition(GetM1PositionCmdValue(pdo_m->Target_pos));
        SM_StateMachineSlow(&g_sM1Ctrl);
        pdo_e->Actual_pos = GetM1PositionActualValue();
        pdo_e->Actual_vel = (uint32_t)(g_sM1Drive.sSpeed.fltSpeedFilt);

    } else if (axis == 1) {
        M2_SetPosition(GetM2PositionCmdValue(pdo_m->Target_pos));
        SM_StateMachineSlow(&g_sM2Ctrl);
        pdo_e->Actual_pos = GetM2PositionActualValue();
        pdo_e->Actual_vel = (uint32_t)(g_sM2Drive.sSpeed.fltSpeedFilt);
    }
    return 0;
}

/*!
 * @brief LPUART Module initialization (LPUART is a the standard block included e.g. in K66F)
 */
static void init_freemaster_lpuart(void)
{
    lpuart_config_t config;

    /* clang-format off */
    clk_t clk = {
        .clkId = kCLOCK_IpInvalid,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };
    /* clang-format on */

    clk.clkId = BOARD_GetUartClkId(BOARD_DEBUG_CONSOLE_PORT);

    CLOCK_SetRate(&clk);
    CLOCK_EnableClock(clk.clkId);

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200U;
    config.enableTx     = false;
    config.enableRx     = false;

    LPUART_Init(FMSTR_LPUART_BASE_ADDR(BOARD_DEBUG_UART_INSTANCE), &config, CLOCK_GetRate(clk.clkId));

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress(FMSTR_LPUART_BASE_ADDR(BOARD_DEBUG_UART_INSTANCE));

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(FMSTR_LPUART_IRQn(BOARD_DEBUG_UART_INSTANCE));
    EnableGlobalIRQ(0);
#endif
}

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
/*
 *   Application interrupt handler of communication peripheral used in interrupt modes
 *   of FreeMASTER communication.
 *
 *   NXP MCUXpresso SDK framework defines interrupt vector table as a part of "startup_XXXXXX.x"
 *   assembler/C file. The table points to weakly defined symbols, which may be overwritten by the
 *   application specific implementation. FreeMASTER overrides the original weak definition and
 *   redirects the call to its own handler.
 *
 */

void FMSTR_LPUART_IRQ_HANDLER(BOARD_DEBUG_UART_INSTANCE)(void)
{
    /* Call FreeMASTER Interrupt routine handler */
    FMSTR_SerialIsr();
}
#endif

/*!
 *@brief      SysTick initialization for CPU cycle measurement
 *
 *@param      none
 *
 *@return     none
 */
static void BOARD_InitSysTick(void)
{
    /* Initialize SysTick core timer to run free */
    /* Set period to maximum value 2^24*/
    SysTick->LOAD = 0xFFFFFF;

    /*Clock source - System Clock*/
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    /*Start Sys Timer*/
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}
