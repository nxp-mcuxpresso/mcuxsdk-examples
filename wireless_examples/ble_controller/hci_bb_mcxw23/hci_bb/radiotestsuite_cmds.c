/*
 * Copyright 2021-2025 NXP
 * NXP Confidential and Proprietary.
 * This software is owned or controlled by NXP and may only
 * be used strictly in accordance with the applicable license terms. By
 * expressly accepting such terms or by downloading, installing, activating
 * and/or otherwise using the software, you are agreeing that you have read,
 * and that you agree to comply with and are bound by, such license terms. If
 * you do not agree to be bound by the applicable license terms, then you may
 * not retain, install, activate or otherwise use the software.
 */

#include "app.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_system.h"
#include "ble_controller.h"
#include "fsl_iap_ffr.h"
#include "hci.h"
#include "ll_error.h"
#include "hci_transport.h"
#include "hci_custom_cmds.h"
#include "fsl_os_abstraction.h"
#include "fsl_component_led.h"
#include "fsl_component_timer_manager.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/** List of custom HCI commands needed by Radio Test Suite */
typedef enum
{
    HCI_OCF_START_DTM_TX_V2_AND_COUNT = 0x06,
    HCI_OCF_STOP_DTM_TX_V2_AND_COUNT  = 0x07,
    HCI_OCF_SET_APP_MODE              = 0x10,
    HCI_OCF_CHIP_ID_GET               = 0x11,
    HCI_OCF_GET_VERSION               = 0x12,
    HCI_OCF_SET_OUTPUT_POWER          = 0x13,
    HCI_OCF_CW_TRANSMITTER            = 0x14,
    HCI_OCF_SET_PHY_MODE              = 0x15,
    HCI_OCF_PRBS_TRANSMITTER          = 0x16,
    HCI_OCF_CW_RSSI_MEASURE           = 0x17,
    HCI_OCF_CW_RSSI_GET               = 0x18,
    HCI_OCF_START_CW_TEST             = 0x19,
    HCI_OCF_CONFIGURE_XTAL_CAPS       = 0x1A,
    HCI_OCF_SET_CLKOUT                = 0x1C,
} hci_custom_commands_radio_test_tool_t;

/** Radio Test Suite version number */
#define SWMAJOR (2)
#define SWMINOR (1)

/** Size of UUID */
#define UUIDSIZEBYTES (16)

/** The priority of the Radio Test Suite task */
#define RADIO_TEST_SUITE_TASK_PRIO (7)
/** The stack size of the Radio Test Suite task */
#define RADIO_TEST_SUITE_TASK_STACK_SIZE (1248)

/** Events used for signalisation of Radio Test Suite task */
#define EVENT_CW_PROCESS  (0x1)
#define EVENT_EXECUTE_DTM (0x2)

/** Used Ctimer instance for Timer Manager */
#define TM_CTIMER_INSTANCE 0U


/** Application modes of Radio Test Suite */
typedef enum
{
    kAPPMODE_Blackbox = 0,
    APPMODE_CwTest   = 1
} appmode_t;

/** CW teset parameters */
typedef struct
{
    uint16_t delayMs;                    /* Delay between channel hops in ms */
    channel_selection_cw_t channelStart; /* Start channel [0..39] */
    channel_selection_cw_t channelStop;  /* Stop channel [0..39] */
    uint8_t channelIncr;                 /* Channel increment */

    uint8_t active;                         /* Test running or not, set this to >0 to actvate */
    channel_selection_cw_t currentChannel; /* Channel currently being tranmitted on */
    uint16_t currentDelayMs;              /* Hopping timer, initialised to delayMs, hop to next channel when 0 */
    int16_t currentOperateMs;             /* Running time, initialised to timeToOperate*1000, stop when 0 */
} cw_test_params_t;

/** Function called before main is entered */
int RadioTestSuitePreMainInit(void) __attribute__((constructor));

static void StartRadioTestSuiteMode(void);
static void RadioTestSuiteTask(void *argument);
static void HandleStandaloneCwTest(void);
static ble_stat_t SetAppMode(uint8_t nbOfInParams,
                             uint8_t *inParams,
                             uint8_t *evntPcktP,
                             uint8_t *outParamsLength,
                             hci_return_command_type *returnCommandType);
static ble_stat_t GetChipID(uint8_t nbOfInParams,
                            uint8_t *inParams,
                            uint8_t *evntPcktP,
                            uint8_t *outParamsLength,
                            hci_return_command_type *returnCommandType);
static ble_stat_t GetVersion(uint8_t nbOfInParams,
                             uint8_t *inParams,
                             uint8_t *evntPcktP,
                             uint8_t *outParamsLength,
                             hci_return_command_type *returnCommandType);
static ble_stat_t SetOutputPower(uint8_t nbOfInParams,
                                 uint8_t *inParams,
                                 uint8_t *evntPcktP,
                                 uint8_t *outParamsLength,
                                 hci_return_command_type *returnCommandType);
static ble_stat_t CWTransmitter(uint8_t nbOfInParams,
                                uint8_t *inParams,
                                uint8_t *evntPcktP,
                                uint8_t *outParamsLength,
                                hci_return_command_type *returnCommandType);
static ble_stat_t SetPhyMode(uint8_t nbOfInParams,
                             uint8_t *inParams,
                             uint8_t *evntPcktP,
                             uint8_t *outParamsLength,
                             hci_return_command_type *returnCommandType);
static ble_stat_t PrbsTransmitter(uint8_t nbOfInParams,
                                  uint8_t *inParams,
                                  uint8_t *evntPcktP,
                                  uint8_t *outParamsLength,
                                  hci_return_command_type *returnCommandType);
static ble_stat_t RssiMeasure(uint8_t nbOfInParams,
                              uint8_t *inParams,
                              uint8_t *evntPcktP,
                              uint8_t *outParamsLength,
                              hci_return_command_type *returnCommandType);
static ble_stat_t RssiGet(uint8_t nbOfInParams,
                          uint8_t *inParams,
                          uint8_t *evntPcktP,
                          uint8_t *outParamsLength,
                          hci_return_command_type *returnCommandType);
static ble_stat_t StartCwTest(uint8_t nbOfInParams,
                              uint8_t *inParams,
                              uint8_t *evntPcktP,
                              uint8_t *outParamsLength,
                              hci_return_command_type *returnCommandType);
static ble_stat_t ConfigureXtalCaps(uint8_t nbOfInParams,
                                    uint8_t *inParams,
                                    uint8_t *evntPcktP,
                                    uint8_t *outParamsLength,
                                    hci_return_command_type *returnCommandType);
static ble_stat_t StartDTMTxV2AndCount(uint8_t nbOfInParams,
                                       uint8_t *inParams,
                                       uint8_t *evntPcktP,
                                       uint8_t *outParamsLength,
                                       hci_return_command_type *returnCommandType);
static ble_stat_t StopDTMTxV2AndCount(uint8_t nbOfInParams,
                                      uint8_t *inParams,
                                      uint8_t *evntPcktP,
                                      uint8_t *outParamsLength,
                                      hci_return_command_type *returnCommandType);
static ble_stat_t ExecuteDTMTxAndCount(void);
static ble_stat_t SetClkOutFunction(uint8_t nbOfInParams,
                                    uint8_t *inParams,
                                    uint8_t *evntPcktP,
                                    uint8_t *outParamsLength,
                                    hci_return_command_type *returnCommandType);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/** Led handle */
extern led_config_t g_ledMonochrome[LED_TYPE_MONOCHROME_COUNT];
static LED_HANDLE_DEFINE(s_ledMonochromeHandle);

/* LED configurations */
static led_flash_config_t s_ledFlashSlow = {
    .times     = LED_FLASH_CYCLE_FOREVER,
    .period    = 2000,
    .flashType = kLED_FlashOneColor,
    .duty      = 50
};
static led_flash_config_t s_ledFlashFast = {
    .times     = LED_FLASH_CYCLE_FOREVER,
    .period    = 200,
    .flashType = kLED_FlashOneColor,
    .duty      = 50
};

/** Handle of the Radio Test Suite task */
static OSA_TASK_HANDLE_DEFINE(s_RadioTestSuiteTaskHandle);
/** The Radio Test Suite task */
static OSA_TASK_DEFINE(RadioTestSuiteTask, RADIO_TEST_SUITE_TASK_PRIO, 1, RADIO_TEST_SUITE_TASK_STACK_SIZE, 0);
OSA_EVENT_HANDLE_DEFINE(s_EventHandle);

static appmode_t s_appMode                      = kAPPMODE_Blackbox;
static volatile cw_test_params_t s_cwTestParams = {.active = 0, .currentOperateMs = 0, .currentDelayMs = 0};
static volatile uint32_t s_msCounter;
static uint8_t s_txChannel;
static uint8_t s_packetPayload;
static uint8_t s_lengthOfTestData;
static uint8_t s_phy;
static uint32_t s_numberOfPacketsTransmitted = 0;
static uint8_t s_Uuid[UUIDSIZEBYTES];

/* Custom commands related to Radio Test Suite */
static const CustomHciCmd customHCIOperations_RadioTestSuite[] = {
    {.opcode          = GET_OP_CODE(HCI_OCF_START_DTM_TX_V2_AND_COUNT, HCI_OGF_EXT),
     .nbOfInParams = 4,
     .func            = StartDTMTxV2AndCount},
    {.opcode          = GET_OP_CODE(HCI_OCF_STOP_DTM_TX_V2_AND_COUNT, HCI_OGF_EXT),
     .nbOfInParams = 0,
     .func            = StopDTMTxV2AndCount},

    {.opcode = GET_OP_CODE(HCI_OCF_SET_APP_MODE, HCI_OGF_EXT), .nbOfInParams = 1, .func = SetAppMode},
    {.opcode = GET_OP_CODE(HCI_OCF_CHIP_ID_GET, HCI_OGF_EXT), .nbOfInParams = 0, .func = GetChipID},
    {.opcode = GET_OP_CODE(HCI_OCF_GET_VERSION, HCI_OGF_EXT), .nbOfInParams = 0, .func = GetVersion},
    {.opcode = GET_OP_CODE(HCI_OCF_SET_OUTPUT_POWER, HCI_OGF_EXT), .nbOfInParams = 1, .func = SetOutputPower},
    {.opcode = GET_OP_CODE(HCI_OCF_CW_TRANSMITTER, HCI_OGF_EXT), .nbOfInParams = 1, .func = CWTransmitter},
    {.opcode = GET_OP_CODE(HCI_OCF_SET_PHY_MODE, HCI_OGF_EXT), .nbOfInParams = 1, .func = SetPhyMode},
    {.opcode = GET_OP_CODE(HCI_OCF_PRBS_TRANSMITTER, HCI_OGF_EXT), .nbOfInParams = 1, .func = PrbsTransmitter},
    {.opcode = GET_OP_CODE(HCI_OCF_CW_RSSI_MEASURE, HCI_OGF_EXT), .nbOfInParams = 1, .func = RssiMeasure},
    {.opcode = GET_OP_CODE(HCI_OCF_CW_RSSI_GET, HCI_OGF_EXT), .nbOfInParams = 0, .func = RssiGet},
    {.opcode = GET_OP_CODE(HCI_OCF_START_CW_TEST, HCI_OGF_EXT), .nbOfInParams = 8, .func = StartCwTest},
    {.opcode = GET_OP_CODE(HCI_OCF_CONFIGURE_XTAL_CAPS, HCI_OGF_EXT), .nbOfInParams = 2, .func = ConfigureXtalCaps},
    {.opcode = GET_OP_CODE(HCI_OCF_SET_CLKOUT, HCI_OGF_EXT), .nbOfInParams = 1, .func = SetClkOutFunction}};

/*******************************************************************************
 * Static functions
 ******************************************************************************/

/*!
 * @brief Function to start Radio Test Suite Mode once first command is received from Test Tool
 */
static void StartRadioTestSuiteMode(void)
{
    static bool radioTestSuiteStarted = false;

    if(radioTestSuiteStarted == false)
    {
        /* Set systick reload value to generate 1 ms interrupts */
        SysTick_Config(SystemCoreClock / 1000);

        /* Initialize timer manager (needed by LED component)*/
        timer_config_t timerConfig;
        timer_status_t timerStatus;
        led_status_t ledStatus;
        
        CLOCK_AttachClk(kMAIN_CLK_to_CTIMER0);
        timerConfig.instance              = TM_CTIMER_INSTANCE;
        timerConfig.srcClock_Hz           = CLOCK_GetCTimerClkFreq(TM_CTIMER_INSTANCE);
        timerConfig.clockSrcSelect        = 0;
        timerStatus = TM_Init(&timerConfig);
        assert_equal(kStatus_TimerSuccess, timerStatus);
        (void)timerStatus;

        /* Initialize LED */
        ledStatus = LED_Init(s_ledMonochromeHandle, &g_ledMonochrome[0]);
        assert_equal(kStatus_LED_Success, ledStatus);
        (void)ledStatus;

        /* Blink slow to indicate normal operation */
        LED_Flash(s_ledMonochromeHandle, &s_ledFlashSlow);
        radioTestSuiteStarted = true;

        /* Register task for Radio Test Suite*/
        osa_status_t osaStatus =
            OSA_TaskCreate((osa_task_handle_t)s_RadioTestSuiteTaskHandle, OSA_TASK(RadioTestSuiteTask), NULL);
        (void)osaStatus;
        assert_equal(osaStatus, KOSA_StatusSuccess);
        (void)OSA_EventCreate(s_EventHandle, (uint8_t)TRUE);
    }
}

/*!
 * @brief Convert power parameter from Python tool to parameter accepted by the BLE manager.
 * The parameter is not checked for bounds.
 */
static int8_t Power2dBm(uint8_t p)
{
    return p * 2 - 10;
}

/*!
 * @brief Validate channel parameter.
 * The channel number of the Python testsuite matches the number of the BLE manager.
 */
static bool ChannelInRange(uint8_t channel)
{
    return (channel >= kBLEC_CHANNEL_CW_0) && (channel <= kBLEC_CHANNEL_CW_MAX);
}

/*!
 * @brief Validate power parameter (NOT dBm).
 * Valid range for [0..8].
 */
static bool PowerInRange(uint8_t power)
{
    return power <= 8;
}

static void HandleStandaloneCwTest()
{
    /* Are we done yet? */
    if (s_cwTestParams.currentOperateMs == 0) /* Negative = forever, 0 = done, >0 = counting down till done */
    {
        s_cwTestParams.active = 0;
        LED_Flash(s_ledMonochromeHandle, &s_ledFlashSlow);
        BLEController_ContinuousWave_Stop(); /* Continue even with error */

        /* Leave standalone mode */
        s_appMode = kAPPMODE_Blackbox;
        return;
    }

    /* Not done, do we need to hop to other channels? */
    if (s_cwTestParams.currentDelayMs == 0)
    {
        s_cwTestParams.currentDelayMs = s_cwTestParams.delayMs; /* Reload delay timer */

        uint8_t oldChannel = s_cwTestParams.currentChannel;
        s_cwTestParams.currentChannel += s_cwTestParams.channelIncr;    /* Next channel */
        if (s_cwTestParams.currentChannel > s_cwTestParams.channelStop) /* Rewind to start channel? */
        {
            s_cwTestParams.currentChannel = s_cwTestParams.channelStart;
        }
        if (s_cwTestParams.currentChannel != oldChannel)
        {
            BLEController_ContinuousWave_Stop();                                /* Stop current TX */
            BLEController_ContinuousWave_StartTx(s_cwTestParams.currentChannel); /* TX on new channel */
        }
    }
}

static ble_stat_t SetAppMode(uint8_t nbOfInParams,
                             uint8_t *inParams,
                             uint8_t *evntPcktP,
                             uint8_t *outParamsLength,
                             hci_return_command_type *returnCommandType)
{
    ble_stat_t status  = SUCCESS;
    *outParamsLength = 0;

    StartRadioTestSuiteMode();

    /* Extract the mode parameter  from the received pckt_p */
    int mode = inParams[0];

    /* Make sure mode is in the right interval */
    if ((mode >= 0) && (mode <= 1)) /* Constants defined in the Python test suite */
    {
        s_appMode                         = (appmode_t)mode;
        s_cwTestParams.currentOperateMs = 0; /* Unconditionally leave standalone CW test mode */

        *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
        status               = SUCCESS;
    }
    else
    {
        /* mode is out of range */
        status = PARAMETER_OUT_OF_MANDATORY_RANGE;
        /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }

    return status;
}

static ble_stat_t GetChipID(uint8_t nbOfInParams,
                            uint8_t *inParams,
                            uint8_t *evntPcktP,
                            uint8_t *outParamsLength,
                            hci_return_command_type *returnCommandType)
{
    memcpy(evntPcktP, s_Uuid, UUIDSIZEBYTES);
    *outParamsLength = UUIDSIZEBYTES;

    *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;

    return SUCCESS;
}

static ble_stat_t GetVersion(uint8_t nbOfInParams,
                             uint8_t *inParams,
                             uint8_t *evntPcktP,
                             uint8_t *outParamsLength,
                             hci_return_command_type *returnCommandType)
{
    evntPcktP[0]     = SWMAJOR;
    evntPcktP[1]     = SWMINOR;
    evntPcktP[2]     = 0; /* Reserved for future use */
    evntPcktP[3]     = 0; /* Reserved for future use */
    evntPcktP[4]     = (uint8_t)SYSTEM_GetChipVersion();
    *outParamsLength = 5;

    /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
    *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;

    return SUCCESS;
}

static ble_stat_t SetOutputPower(uint8_t nbOfInParams,
                                 uint8_t *inParams,
                                 uint8_t *evntPcktP,
                                 uint8_t *outParamsLength,
                                 hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;

    /* No parameters to return */
    *outParamsLength = 0;

    /* Extract the output power parameter from the received pckt_p */
    uint8_t power = inParams[0];
    /* Make sure power is in the right interval */
    if (PowerInRange(power))
    {
        /* Set power */
        blec_result_t ret = BLEController_SetTxPowerDbm(Power2dBm(power));
        if (ret != kBLEC_Success)
        {
            /* Should not come here */
            status = UNSPECIFIED_ERROR;
            /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
        }
        else
        {
            /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
            status               = SUCCESS;
        }
    }
    else
    {
        /* Power is out of range */
        status = PARAMETER_OUT_OF_MANDATORY_RANGE;
        /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }

    return status;
}

static ble_stat_t CWTransmitter(uint8_t nbOfInParams,
                                uint8_t *inParams,
                                uint8_t *evntPcktP,
                                uint8_t *outParamsLength,
                                hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;

    /* No parameters to return */
    *outParamsLength = 0;

    /* Extract the channel parameter from the received pckt_p */
    bool enable                    = (inParams[0] & 0x80) == 0x80;
    channel_selection_cw_t channel = (channel_selection_cw_t)(inParams[0] & 0x7F);

    if (ChannelInRange(channel))
    {
        if (enable)
        {
            if (BLEController_ContinuousWave_StartTx(channel) == kBLEC_Success)
            {
                /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
                *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
                status               = SUCCESS;
            }
            else
            {
                /* BLE manager problem, should not come here */
                status = UNSPECIFIED_ERROR;
                /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
                *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
            }
        }
        else
        {
            if (BLEController_ContinuousWave_Stop() == kBLEC_Success)
            {
                /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
                *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
                status               = SUCCESS;
            }
            else
            {
                /* BLE manager problem, should not come here */
                status = UNSPECIFIED_ERROR;
                /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
                *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
            }
        }
    }
    else
    {
        /* Channel is out of range */
        status = PARAMETER_OUT_OF_MANDATORY_RANGE;
        /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }

    return status;
}

static tx_rx_phy_t gPhyMode = kBLEC_PHY_1M;

static ble_stat_t SetPhyMode(uint8_t nbOfInParams,
                             uint8_t *inParams,
                             uint8_t *evntPcktP,
                             uint8_t *outParamsLength,
                             hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;

    /* No parameters to return */
    *outParamsLength = 0;

    /* Extract the phymode parameter from the received pckt_p */
    switch (inParams[0])
    {
        case 0:
            gPhyMode = kBLEC_PHY_1M;
            /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
            status               = SUCCESS;
            break;
        case 1:
            gPhyMode = kBLEC_PHY_2M;
            /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
            status               = SUCCESS;
            break;
        default:
            /* Phy mode is out of range */
            status = PARAMETER_OUT_OF_MANDATORY_RANGE;
            /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }

    return status;
}

static ble_stat_t PrbsTransmitter(uint8_t nbOfInParams,
                                  uint8_t *inParams,
                                  uint8_t *evntPcktP,
                                  uint8_t *outParamsLength,
                                  hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;

    /* No parameters to return */
    *outParamsLength = 0;

    /* Extract the channel parameter from the received pckt_p */
    bool enable                    = (inParams[0] & 0x80) == 0x80;
    channel_selection_cw_t channel = (channel_selection_cw_t)(inParams[0] & 0x7F);

    if (ChannelInRange(channel))
    {
        if (enable)
        {
            if (BLEController_ContinuousWave_StartTxModulated(channel, kBLEC_PAYLOAD_TYPE_CW_PRBS9, gPhyMode) ==
                kBLEC_Success)
            {
                /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
                *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
                status               = SUCCESS;
            }
            else
            {
                /* Should not come here */
                status = UNSPECIFIED_ERROR;
                /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
                *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
            }
        }
        else
        {
            if (BLEController_ContinuousWave_Stop() == kBLEC_Success)
            {
                /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
                *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
                status               = SUCCESS;
            }
            else
            {
                /* Should never come here */
                status = UNSPECIFIED_ERROR;
                /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
                *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
            }
        }
    }
    else
    {
        /* Channel is out of range */
        status = PARAMETER_OUT_OF_MANDATORY_RANGE;
        /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }

    return status;
}

/*!
 * @brief Start the RSSI measuring function. This function does NOT return the RSSI value
 */
static ble_stat_t RssiMeasure(uint8_t nbOfInParams,
                              uint8_t *inParams,
                              uint8_t *evntPcktP,
                              uint8_t *outParamsLength,
                              hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;
    blec_result_t result;

    /* No parameters to return */
    *outParamsLength = 0;

    /* Extract the channel parameter from the received pckt_p */
    bool enable                    = (inParams[0] & 0x80) == 0x80;
    channel_selection_cw_t channel = (channel_selection_cw_t)(inParams[0] & 0x7F);

    if (ChannelInRange(channel))
    {
        if (enable)
        {
            /* Start RSSI measurement */
            result = BLEController_ContinuousWave_StartRx(channel); /* start receive to measure RSSI on this channel */
            if (result == kBLEC_Success)
            {
                *outParamsLength = 0;
                /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
                *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
                return SUCCESS;
            }
            else
            {
                /* Something wrong */
                status = UNSPECIFIED_ERROR;
                /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
                *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
            }
        }
        else
        {
            /* Stop reception */
            result = BLEController_ContinuousWave_Stop();
            if (result == kBLEC_Success)
            {
                /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
                *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
                status               = SUCCESS;
            }
            else
            {
                /* Should never come here */
                status = UNSPECIFIED_ERROR;
                /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
                *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
            }
        }
    }
    else
    {
        /* Channel is out of range */
        status = PARAMETER_OUT_OF_MANDATORY_RANGE;
        /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }

    return status;
}

/*!
 * @brief Get the RSSI value measured by the RssiMeasure() function
 */
static ble_stat_t RssiGet(uint8_t nbOfInParams,
                          uint8_t *inParams,
                          uint8_t *evntPcktP,
                          uint8_t *outParamsLength,
                          hci_return_command_type *returnCommandType)
{
    int16_t rssi = (BLEController_ContinuousWave_ReadRSSI() - 2) / 4; /* returned steps are in 0.25 dBm */

    *outParamsLength = 2;
    /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
    *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
    evntPcktP[0]       = rssi & 0xff;
    evntPcktP[1]       = (rssi >> 8) & 0xff;
    return SUCCESS;
}

/*!
 * @brief Start a standalone CW transmitter, hopping through different frequencies
 */
static ble_stat_t StartCwTest(uint8_t nbOfInParams,
                              uint8_t *inParams,
                              uint8_t *evntPcktP,
                              uint8_t *outParamsLength,
                              hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;

    uint16_t timeToOperate = 0;
    uint16_t delayMs        = 0;
    uint8_t power, channelStart, channelStop, channelIncr;

    timeToOperate = inParams[0] + (inParams[1] << 8);
    delayMs        = inParams[2] + (inParams[3] << 8);
    power           = inParams[4];
    channelStart   = inParams[5];
    channelStop    = inParams[6];
    channelIncr    = inParams[7];

    *outParamsLength = 0;

    /* Prepare for the worst: some parameter(s) out of range and return HCI_RETURN_COMMAND_TYPE_STATUS to mention that
     * something went wrong */
    status               = PARAMETER_OUT_OF_MANDATORY_RANGE;
    *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;

    /* Validate arguments and execute */
    if (ChannelInRange(channelStart) && ChannelInRange(channelStop) && ChannelInRange(channelIncr) &&
        PowerInRange(power))
    {
        status = SUCCESS;
        /* Halt SysTick timer and disable interrupt */
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
        __ISB(); /* not sure if we need this */
        __DSB();

        s_cwTestParams.active = 0;

        /* boundary/reload values */
        s_cwTestParams.delayMs      = delayMs;
        s_cwTestParams.channelStart = channelStart;
        s_cwTestParams.channelStop  = channelStop;
        s_cwTestParams.channelIncr  = channelIncr;

        /* "Current" parameters */
        s_cwTestParams.currentOperateMs =
            timeToOperate == 0 ? -1 : timeToOperate * 1000; /* convert operate time to ms_time */
        s_cwTestParams.currentChannel  = s_cwTestParams.channelStart;
        s_cwTestParams.currentDelayMs = s_cwTestParams.delayMs;

        /* Start BLE */
        status = BLEController_SetTxPowerDbm(Power2dBm(power));
        if (status == SUCCESS)
        {
            status = BLEController_ContinuousWave_Stop();
            if (status == SUCCESS)
            {
                status = BLEController_ContinuousWave_StartTx(s_cwTestParams.currentChannel);
                if (status == SUCCESS)
                {
                    /* start standalone test */
                    s_cwTestParams.active = 1;
                    LED_Flash(s_ledMonochromeHandle, &s_ledFlashFast);

                    /* Enable SysTick IRQ and SysTick Timer */
                    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

                    /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
                    *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
                }
            }
        }
    }
    return status;
}

static ble_stat_t ConfigureXtalCaps(uint8_t nbOfInParams,
                                    uint8_t *inParams,
                                    uint8_t *evntPcktP,
                                    uint8_t *outParamsLength,
                                    hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;

    /* No parameters to return */
    *outParamsLength = 0;

    /* Extract the cap parameters from the received pckt_p */
    uint8_t capIn  = inParams[0];
    uint8_t capOut = inParams[1];

    if ((capIn <= 15) && (capOut <= 15))
    {
        POWER_XTAL32K_ConfigureCaps(capIn, capOut);
        /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
    }
    else
    {
        /* Cap(s) out of range */
        status = PARAMETER_OUT_OF_MANDATORY_RANGE;
        /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }

    return status;
}

static ble_stat_t StartDTMTxV2AndCount(uint8_t nbOfInParams,
                                       uint8_t *inParams,
                                       uint8_t *evntPcktP,
                                       uint8_t *outParamsLength,
                                       hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;
    /* Extract the parameters from the received pckt_p */
    s_txChannel          = inParams[0];
    s_lengthOfTestData = inParams[1];
    s_packetPayload      = inParams[2];
    s_phy                 = inParams[3];

    s_numberOfPacketsTransmitted = 0;
    (void)OSA_EventSet((osa_event_handle_t)s_EventHandle, EVENT_EXECUTE_DTM);
    /* Disable s_phy calibration before every DTM command but this command does s_phy calibration once */
    ll_intf_start_phy_clbr(0);

    /* No parameters to return */
    *outParamsLength = 0;
    /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
    *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
    return status;
}

static ble_stat_t StopDTMTxV2AndCount(uint8_t nbOfInParams,
                                      uint8_t *inParams,
                                      uint8_t *evntPcktP,
                                      uint8_t *outParamsLength,
                                      hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;

    /* Parameters to return */
    *outParamsLength = 2;
    /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
    *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
    evntPcktP[0]       = s_numberOfPacketsTransmitted & 0xff;
    evntPcktP[1]       = (s_numberOfPacketsTransmitted >> 8) & 0xff;

    return status;
}

/*
 * @brief workaround for DTM TX not counting packets as defined in BLE core spec.
 * Start a transmitter test and stop it immediately. ll_intf_le_test_end() will wait till the single packet is sent.
 * There is a risk of being interrupted (e.g. systick) between ll_intf_le_enhanced_transmitter_test and
 * ll_intf_le_test_end and if that interruption lasts very long we could have more than one packet transmitted. However
 * a packet lasts in the order of 1 ms, so in practice this is unlikely to happen.
 */
static ble_stat_t ExecuteDTMTxAndCount(void)
{
    ble_stat_t status;
    status = ll_intf_le_enhanced_transmitter_test(s_txChannel, s_lengthOfTestData, s_packetPayload, s_phy);
    if (status == SUCCESS)
    {
        s_numberOfPacketsTransmitted++;
        /* Stop DTM TX.  Function waits till packet is transmitted.  Returned numberOfPackets = 0. */
        uint16_t numberOfPackets;
        status = ll_intf_le_test_end(&numberOfPackets);
    }
    return status;
}

/* @brief Route XTAL oscillator output to CLKOUT on PIO0_9.
 * Connect either the 32.768kHz XTAL oscillator output, the 32MHz XTAL oscillator
 * or none to PIO0_9.
 * For the 32.768kHz XTAL oscillator, the clock out signal is recommended for frequency measurements.
 * For the 32MHz XTAL oscillator, the clock out signal can be used for frequency measurements,
 * but final measurements need to be done on the RF output using CW signals.
 * The signal on the CLKOUT pin is not suited for jitter measurements.
 */
static ble_stat_t SetClkOutFunction(uint8_t nbOfInParams,
                                    uint8_t *inParams,
                                    uint8_t *evntPcktP,
                                    uint8_t *outParamsLength,
                                    hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;

    /* No parameters to return */
    *outParamsLength = 0;

    /* Extract the xtal parameter from the received pckt_p */
    uint8_t xtal = inParams[0];

    switch (xtal)
    {
        case 0:
            CLOCK_AttachClk(kNONE_to_SYS_CLKOUT);
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
            break;
        case 1:
            CLOCK_AttachClk(kEXT_CLK_to_CLKOUT);
            CLOCK_SetClkDiv(kCLOCK_DivClkOut, 1, false);
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
            break;
        case 2:
            CLOCK_AttachClk(kXTAL32K_to_OSC32K);
            CLOCK_AttachClk(kOSC32K_to_CLKOUT);
            CLOCK_SetClkDiv(kCLOCK_DivClkOut, 1, false);
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
            break;
        default:
            /* selected xtal oscillator output out of range */
            status = PARAMETER_OUT_OF_MANDATORY_RANGE;
            /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }
    return status;
}

/*******************************************************************************
 * Public functions
 ******************************************************************************/

void SysTick_Handler(void)
{
    if (s_cwTestParams.active)
    {
        if (s_cwTestParams.currentOperateMs > 0) /* Total running time */
        {
            --s_cwTestParams.currentOperateMs;
        }
        else
        {
            (void)OSA_EventSet((osa_event_handle_t)s_EventHandle, EVENT_CW_PROCESS);
        }

        if (s_cwTestParams.currentDelayMs > 0) /* Delay per channel */
        {
            --s_cwTestParams.currentDelayMs;
        }
        else
        {
            (void)OSA_EventSet((osa_event_handle_t)s_EventHandle, EVENT_CW_PROCESS);
        }
    }
}

static void RadioTestSuiteTask(void *argument)
{
    osa_event_flags_t event = 0U;

    /* Wait for events */
    (void)OSA_EventWait((osa_event_handle_t)s_EventHandle, osaEventFlagsAll_c, (uint8_t)FALSE, osaWaitForever_c,
                        &event);

    if ((event & EVENT_CW_PROCESS) == EVENT_CW_PROCESS)
    {
        HandleStandaloneCwTest();
    }

    if ((event & EVENT_EXECUTE_DTM) == EVENT_EXECUTE_DTM)
    {
        ExecuteDTMTxAndCount();
    }
}

int RadioTestSuitePreMainInit(void)
{
    status_t status;
    flash_config_t flashInstance;
    int res;
    (void)res;
    (void)status;

    /* Retreive UUID */
    status = FLASH_Init(&flashInstance);
    assert_equal(kStatus_Success, status);
    status = FFR_GetUUID(&flashInstance, s_Uuid);
    assert_equal(kStatus_Success, status);

    /* Extend custom command array */
    res = HCICUSTOMCMD_Register(customHCIOperations_RadioTestSuite, sizeof(customHCIOperations_RadioTestSuite) /
                                                                        sizeof(customHCIOperations_RadioTestSuite[0]));
    assert(res == 0);

    return 0;
}
