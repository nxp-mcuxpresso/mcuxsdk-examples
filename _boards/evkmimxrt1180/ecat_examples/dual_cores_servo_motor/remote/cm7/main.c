/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "mc_periph_init.h"
#include "freemaster.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_rgpio.h"
#include "fsl_lpuart.h"
#include "m1_sm_snsless_enc.h"

#include "freemaster_serial_lpuart.h"
#include "board.h"
#include "mid_sm_states.h"
#include "mu_ipc_shm_client.h"
#include "fsl_debug_console.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

#define DAPENG_TEST /* Dapeng test */

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
#define BOARD_FMSTR_UART_PORT LPUART1
#define BOARD_FMSTR_UART_BAUDRATE 115200U

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

/* Init SDK HW */
static void BOARD_Init(void);
/* ADC COCO interrupt */
RAM_FUNC_LIB
void ADC1_IRQHandler(void);

static void BOARD_InitUART(uint32_t u32BaudRate);
static void BOARD_InitSysTick(void);
static void BOARD_InitGPIO(void);
static void Application_Control_BL(void);

lpadc_conv_result_t g_LpadcResultConfigStruct;

/*******************************************************************************
 * Variables
 ******************************************************************************/


run_substate_t g_eM1StateRun_old = 0;
/* CPU load measurement using Systick */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

#ifdef DAPENG_TEST
/* ISR counters */
uint32_t ui32FastIsrCount = 0U;
uint32_t ui32SlowIsrCount = 0U;
#endif

/* Demo mode enabled/disabled */
extern  int cl;

/* Counters used for demo mode */
static uint32_t ui32SpeedStimulatorCnt    = 0U;
static uint32_t ui32PositionStimulatorCnt = 0U;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0U;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../../../examples/evkmimxrt1180/demo_apps/mc_pmsm/pmsm_enc",         /* User Path 1- the highest priority */
    "",       /* User Path 2 */
    "evkmimxrt1180", /* board id */
    "pmsm_enc", /* example id */
    MCRSP_VER,      /* sw version */
    FEATURE_SET,    /* example's feature-set */
};

mid_app_cmd_t g_eMidCmd;                  /* Start/Stop MID command */
ctrl_m1_mid_t g_sSpinMidSwitch;           /* Control Spin/MID switching */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
int32_t GetPositionActualValue(void);
acc32_t GetPositionCmdValue(int32_t targetPos);
/*******************************************************************************
 * Code
 ******************************************************************************/
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
				g_sM1Drive.sMCATctrl.ui16PospeSensor = MCAT_ENC_CTRL;
				M1_SetAppSwitch(1);

				if (g_sM1Ctrl.eState != kSM_AppRun || g_eM1StateRun != kRunState_Spin) {
					ret = -1;
				}
				break;
			case 5:
				M1_SetAppSwitch(0);
				break;
			case 6:
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
    }

    return 0;
}

int obj_read_callback(uint16_t Index, uint8_t Subindex, uint8_t size, void *pData)
{
    uint32_t *p = pData;

    if (Index == 0x6064 && Subindex == 0x0) {
        *p = GetPositionActualValue();
        return sizeof(*p);
    } else if (Index == 0x606c && Subindex == 0x0) {
        *p = (uint32_t)g_sM1Drive.sSpeed.fltSpeedFilt;
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

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();
    PRINTF("cm7_Core running\r\n");

	/* Waiting until MU_ipc_shm_master_init() is completed on CM33 core */
    MU_ipc_shm_client_init();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();

    /* Init peripheral motor control driver for motor M1 */
    MCDRV_Init_M1();

    /* Init UART for FreeMaster communication */
    //BOARD_InitUART(BOARD_FMSTR_UART_BAUDRATE);

    /* FreeMaster init */
    //FMSTR_Init();

    /* Turn off application */
    M1_SetAppSwitch(FALSE);

    /* Init MID state machine - call before the spin state machine */
    g_sSpinMidSwitch.eAppState = kAppStateMID;

    if(g_sSpinMidSwitch.eAppState == kAppStateMID)
    {
      MID_Init_AR();
    }

    /* Spin state machine is default */
    g_sSpinMidSwitch.eAppState = kAppStateSpin;

    /* Enable interrupts */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Enable PWM clock */
    g_sM1Pwm3ph.pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_RUN(0xF);

    PRINTF("CM7 init done...\r\n");

    /* Infinite loop */
    while (1)
    {
        Application_Control_BL();

        /* FreeMASTER Polling function */
        //FMSTR_Poll();
    }
}

int32_t GetPositionActualValue(void)
{
    return g_sM1Enc.pui32QdBase->LPOS + (int16_t)(g_sM1Enc.pui32QdBase->REV) * (M1_POSPE_ENC_PULSES * 4);
}

acc32_t GetPositionCmdValue(int32_t targetPos)
{
    return ACC32(targetPos / (M1_POSPE_ENC_PULSES * 4.0));
}
/*!
 * @brief   ADC conversion complete ISR called with 100us period processes
 *           - motor M1 fast application machine function
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
void ADC1_IRQHandler(void)
{
    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();
    switch(g_sSpinMidSwitch.eAppState)
    {
    case kAppStateSpin:
        /* M1 state machine */
        SM_StateMachineFast(&g_sM1Ctrl);
      break;
    default:
        /* MID state machine */
        MID_ProcessFast_FL();
      break;
    }

    /* Stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles =
        g_ui32NumberOfCycles > g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

    /* Call FreeMASTER recorder */

#ifdef DAPENG_TEST
    /* Increment ISR counter */
    ui32FastIsrCount++;

    if(ui32FastIsrCount > 16000U)
    {
       ui32FastIsrCount = 0;
    }

#endif

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
 * @brief   Called with 1ms period and processes following functions:
 *           - motor M1 slow application machine function
 *
 * @param   void
 *
 * @return  none
 */
RAM_FUNC_LIB
int motor_slow_task(int axis, struct pdo_to_motor_t *pdo_m, struct pdo_to_esc_t *pdo_e, struct param_t *param)
{
    if (axis == 0) {
	    M1_SetPosition(GetPositionCmdValue(pdo_m->Target_pos));
        /* M1 Slow StateMachine call */
        SM_StateMachineSlow(&g_sM1Ctrl);
	    pdo_e->Actual_pos = GetPositionActualValue();
	    pdo_e->Actual_vel = (uint32_t)(g_sM1Drive.sSpeed.fltSpeedFilt);
    }

    return 0;
}

/*!
 * @brief   Application_Control_BL
 *           - Control switching between Spin and MID
 *
 * @param   void
 *
 * @return  none
 */
static void Application_Control_BL(void)
{
	if (g_eM1StateRun_old != g_eM1StateRun) {
		g_eM1StateRun_old = g_eM1StateRun;
	}

    switch(g_sSpinMidSwitch.eAppState)
    {
        case kAppStateSpin:
        /* M1 state machine */
            if(g_sSpinMidSwitch.bCmdRunMid == TRUE)
            {
                if((kSM_AppStop == M1_GetAppState()) && (FALSE == M1_GetAppSwitch()) )
                {
                    MID_Init_AR();
                    g_sSpinMidSwitch.sFaultCtrlM1_Mid &= ~(FAULT_APP_SPIN);
                    g_eMidCmd = kMID_Cmd_Stop;                          /* Reset MID control command */
                    g_sSpinMidSwitch.eAppState = kAppStateMID;          /* MID routines will be processed */
                }
                else
                    g_sSpinMidSwitch.sFaultCtrlM1_Mid |= FAULT_APP_SPIN;

                g_sSpinMidSwitch.bCmdRunMid = FALSE;                  /* Always clear request */
            }

            g_sSpinMidSwitch.bCmdRunM1 = FALSE;
            break;
        default:
            /* MID state machine */
            if(g_sSpinMidSwitch.bCmdRunM1 == TRUE)
            {
                if((g_eMidCmd == kMID_Cmd_Stop) && (kMID_Stop == MID_GetActualState()))
                {
                    g_sSpinMidSwitch.sFaultCtrlM1_Mid &= ~(FAULT_APP_MID);
                    g_sM1Ctrl.eState = kSM_AppInit;                      /* Set Init state for M1 state machine */
                    g_sSpinMidSwitch.eAppState = kAppStateSpin;          /* Switch application state to Spin */
                }
                else
                    g_sSpinMidSwitch.sFaultCtrlM1_Mid |= FAULT_APP_MID;

                /* Always clear request */
                g_sSpinMidSwitch.bCmdRunM1 = FALSE;
                g_sSpinMidSwitch.bCmdRunMid = FALSE;
                break;
            }

            g_sSpinMidSwitch.bCmdRunMid = FALSE;
            MID_Process_BL(&g_eMidCmd);
            break;
    }
}

/*!
 *@brief      Initialization of the UART module
 *
 *@param      u32BaudRate         Baud rate
 *
 *@return     none
 */
static void BOARD_InitUART(uint32_t u32BaudRate)
{
    lpuart_config_t config;

    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_FMSTR_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    LPUART_Init(BOARD_FMSTR_UART_PORT, &config, BOARD_DebugConsoleSrcFreq());

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress(BOARD_FMSTR_UART_PORT);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(BOARD_UART_IRQ);
    EnableGlobalIRQ(0);
#endif
}

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