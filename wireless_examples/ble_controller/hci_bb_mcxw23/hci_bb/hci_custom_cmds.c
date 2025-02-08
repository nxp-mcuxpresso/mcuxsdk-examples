/**
 * Copyright 2024,2025 NXP.
 * NXP Confidential and Proprietary.
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include "hci_custom_cmds.h"
#include "ll_error.h"
#include "fsl_power.h"
#include "ble_controller.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/** Maximum number of custom command lists. */
#define CUSTOM_CMD_LISTS_MAX          (16u)

/** Size of the initialization string. */
#define HCI_BLACKBOX_INIT_STRING_SIZE 32

/** Represents a list of custom HCI commands. */
typedef struct
{
    const CustomHciCmd *cmdList;
    size_t cmdCount;
} CustomHciCmdListItem_t;

/**  Represents the Continuous Wave status. */
typedef enum
{
    CW_IDLE = 0,
    CW_TX,
    CW_RX,
} cw_status_t;

/* Function called before main is entered */
int HciCustomCmdPreMainInit(void) __attribute__((constructor));

static ble_stat_t SetSupplyMode(uint8_t nbOfInParams,
                                uint8_t *inParams,
                                uint8_t *evntPcktP,
                                uint8_t *outParamsLength,
                                hci_return_command_type *returnCommandType);
static ble_stat_t ConfigureBuckOutput(uint8_t nbOfInParams,
                                      uint8_t *inParams,
                                      uint8_t *evntPcktP,
                                      uint8_t *outParamsLength,
                                      hci_return_command_type *returnCommandType);
static ble_stat_t StartCwTx(uint8_t nbOfInParams,
                            uint8_t *inParams,
                            uint8_t *evntPcktP,
                            uint8_t *outParamsLength,
                            hci_return_command_type *returnCommandType);
static ble_stat_t StartCwTxModulated(uint8_t nbOfInParams,
                                     uint8_t *inParams,
                                     uint8_t *evntPcktP,
                                     uint8_t *outParamsLength,
                                     hci_return_command_type *returnCommandType);
static ble_stat_t StartCwTxModulated(uint8_t nbOfInParams,
                                     uint8_t *inParams,
                                     uint8_t *evntPcktP,
                                     uint8_t *outParamsLength,
                                     hci_return_command_type *returnCommandType);
static ble_stat_t StartCwRx(uint8_t nbOfInParams,
                            uint8_t *inParams,
                            uint8_t *evntPcktP,
                            uint8_t *outParamsLength,
                            hci_return_command_type *returnCommandType);
static ble_stat_t StopCw(uint8_t nbOfInParams,
                         uint8_t *inParams,
                         uint8_t *evntPcktP,
                         uint8_t *outParamsLength,
                         hci_return_command_type *returnCommandType);
static ble_stat_t ReadCwRSSI(uint8_t nbOfInParams,
                             uint8_t *inParams,
                             uint8_t *evntPcktP,
                             uint8_t *outParamsLength,
                             hci_return_command_type *returnCommandType);
static ble_stat_t PrintString(uint8_t nbOfInParams,
                              uint8_t *inParams,
                              uint8_t *evntPcktP,
                              uint8_t *outParamsLength,
                              hci_return_command_type *returnCommandType);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/** Array of pointers to custom commands lists */
static CustomHciCmdListItem_t s_customCmdLists[CUSTOM_CMD_LISTS_MAX];

/** Variable to keep track how many custom command sets are registered */
static int s_customCmdListsCount = 0;

/** Custom commands related to continuous wave */
static const CustomHciCmd s_customHCIOperations_ContinuousWave[] = {
    {.opcode = GET_OP_CODE(HCI_OCF_START_CW_TX, HCI_OGF_EXT), .nbOfInParams = 1, .func = StartCwTx},
    {.opcode = GET_OP_CODE(HCI_OCF_START_CW_RX, HCI_OGF_EXT), .nbOfInParams = 1, .func = StartCwRx},
    {.opcode = GET_OP_CODE(HCI_OCF_STOP_CW, HCI_OGF_EXT), .nbOfInParams = 0, .func = StopCw},
    {.opcode = GET_OP_CODE(HCI_OCF_READ_CW_RSSI, HCI_OGF_EXT), .nbOfInParams = 0, .func = ReadCwRSSI},
    {.opcode          = GET_OP_CODE(HCI_OCF_START_CW_TX_MODULATED, HCI_OGF_EXT),
     .nbOfInParams = 3,
     .func            = StartCwTxModulated},
};

/** Custom commands related to power */
static const CustomHciCmd s_customHCIOperations_Power[] = {
    {.opcode = GET_OP_CODE(HCI_OCF_DCDC_BUCK_CONFIG, HCI_OGF_EXT), .nbOfInParams = 1, .func = ConfigureBuckOutput},
    {.opcode = GET_OP_CODE(HCI_OCF_SET_SUPPLY_MODE, HCI_OGF_EXT), .nbOfInParams = 1, .func = SetSupplyMode},
};

/** Custom commands related to utilities */
static const CustomHciCmd s_customHCIOperations_Utils[] = {{.opcode = GET_OP_CODE(HCI_OCF_PRINT_STRING, HCI_OGF_EXT),
                                                          .nbOfInParams = HCI_BLACKBOX_INIT_STRING_SIZE + 1,
                                                          .func            = PrintString}};

/** Variable to keep track of Continuous Wave state*/
static uint32_t s_cwState = CW_IDLE;

/*******************************************************************************
 * Static functions
 ******************************************************************************/

static const CustomHciCmd *GetCustomOperation(uint16_t opcode)
{
    /* Loop over all registered command sets */
    for (int i = 0; i < s_customCmdListsCount; i++)
    {
        /* Loop over all registered commands within this set */
        for (int j = 0; j < s_customCmdLists[i].cmdCount; j++)
        {
            if (s_customCmdLists[i].cmdList[j].opcode == opcode)
            {
                return &s_customCmdLists[i].cmdList[j];
            }
        }
    }
    return NULL;
}

int HCICUSTOMCMD_Register(const CustomHciCmd *cmdList, int cmdCount)
{
    if (s_customCmdListsCount < CUSTOM_CMD_LISTS_MAX)
    {
        /* Check if there are no duplicate commands*/
        for (int i = 0; i < cmdCount; i++)
        {
            if (GetCustomOperation(cmdList[i].opcode) != NULL)
            {
                return -1;
            }
        }

        /* Register the new command set */
        s_customCmdLists[s_customCmdListsCount].cmdList  = cmdList;
        s_customCmdLists[s_customCmdListsCount].cmdCount = cmdCount;
        s_customCmdListsCount++;
    }
    else
    {
        return -1;
    }
    return 0;
}

ble_stat_t HCICUSTOMCMD_Callback(uint16_t ocf,
                                 uint8_t *pcktP,
                                 uint8_t *evntPcktP,
                                 uint8_t *paramsLength,
                                 hci_return_command_type *returnCommandType)
{
    (void)ocf;

    /* Extract the opcode (OGF + OCF) from 	the received pcktP */
    uint16_t opcode = (((uint16_t)pcktP[0]) | ((uint16_t)pcktP[1]) << 8);

    const CustomHciCmd *op = GetCustomOperation(opcode);
    if (op)
    {
        uint8_t nb_of_params = pcktP[2];
        PRINTF("opcode:%x nbOfInParams:%u nb_of_params:%u\n", opcode, op->nbOfInParams, nb_of_params);
        if (op->nbOfInParams != nb_of_params)
        {
            /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
            /* Wrong length of passed parameters comapred to expected */
            return INVALID_HCI_COMMAND_PARAMETERS;
        }
        return op->func(nb_of_params, (nb_of_params > 0) ? &pcktP[3] : NULL, evntPcktP, paramsLength,
                        returnCommandType);
    }

    return UNKNOWN_HCI_COMMAND;
}

static ble_stat_t SetSupplyMode(uint8_t nbOfInParams,
                                uint8_t *inParams,
                                uint8_t *evntPcktP,
                                uint8_t *outParamsLength,
                                hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;

    /* Extract the supply mode parameter from the received pcktP */
    dcdc_mode_t mode = ((dcdc_mode_t)inParams[0]);
    /* Make sure mode is in the right interval */
    if (POWER_DCDC_SetSupplyMode(mode) == kStatus_Success)
    {
        /* No parameters to return */
        *outParamsLength = 0;
        /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;

        status = SUCCESS;
    }
    else
    {
        /* most probably parameter is out of range */
        status = PARAMETER_OUT_OF_MANDATORY_RANGE;
        /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }
    return status;
}

static ble_stat_t ConfigureBuckOutput(uint8_t nbOfInParams,
                                      uint8_t *inParams,
                                      uint8_t *evntPcktP,
                                      uint8_t *outParamsLength,
                                      hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;

    /* Extract the dcDcBuckOutputLvl parameter  from the received pcktP */
    dcdc_buck_output_level_t buckOutputLevel = ((dcdc_buck_output_level_t)inParams[0]);
    /* Make sure dcDcBuckOutputLvl is in the right interval */
    if ((buckOutputLevel >= kDCDC_BUCK_OUTPUT_1100mv) && (buckOutputLevel <= kDCDC_BUCK_OUTPUT_1800mv))
    {
        POWER_DCDC_ConfigureBuckOutput(buckOutputLevel);

        /* No parameters to return */
        *outParamsLength = 0;
        /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;

        status = SUCCESS;
    }
    else
    {
        /* dcDcBuckOutputLvl is out of range */
        status = PARAMETER_OUT_OF_MANDATORY_RANGE;
        /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }
    return status;
}

static ble_stat_t StartCwTx(uint8_t nbOfInParams,
                            uint8_t *inParams,
                            uint8_t *evntPcktP,
                            uint8_t *outParamsLength,
                            hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;

    if (s_cwState == CW_IDLE)
    {
        /* Extract the channel parameter from the received pcktP */
        channel_selection_cw_t channel = (channel_selection_cw_t)inParams[0];
        if (BLEController_ContinuousWave_StartTx(channel) == kBLEC_Success)
        {
            s_cwState = CW_TX;
            /* No parameters to return */
            *outParamsLength = 0;
            /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
        }
        else
        {
            /* Channel is out of range */
            status = PARAMETER_OUT_OF_MANDATORY_RANGE;
            /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
        }
    }
    else
    {
        /* Not in idle */
        status = COMMAND_DISALLOWED;
        /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }
    return status;
}

static ble_stat_t StartCwTxModulated(uint8_t nbOfInParams,
                                     uint8_t *inParams,
                                     uint8_t *evntPcktP,
                                     uint8_t *outParamsLength,
                                     hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;

    if (s_cwState == CW_IDLE)
    {
        /* Extract the parameters from the received pcktP */
        channel_selection_cw_t channel = (channel_selection_cw_t)inParams[0];
        payload_type_t payloadType    = (payload_type_t)inParams[1];
        tx_rx_phy_t phy                = (tx_rx_phy_t)inParams[2];
        if (BLEController_ContinuousWave_StartTxModulated(channel, payloadType, phy) == kBLEC_Success)
        {
            s_cwState = CW_TX;
            /* No parameters to return */
            *outParamsLength = 0;
            /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
        }
        else
        {
            /* Channel is out of range */
            status = PARAMETER_OUT_OF_MANDATORY_RANGE;
            /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
        }
    }
    else
    {
        /* Not in idle */
        status = COMMAND_DISALLOWED;
        /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }
    return status;
}

static ble_stat_t StartCwRx(uint8_t nbOfInParams,
                            uint8_t *inParams,
                            uint8_t *evntPcktP,
                            uint8_t *outParamsLength,
                            hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;
    if (s_cwState == CW_IDLE)
    {
        /* Extract the channel parameter from the received pcktP */
        channel_selection_cw_t channel = (channel_selection_cw_t)inParams[0];
        if (BLEController_ContinuousWave_StartRx(channel) == kBLEC_Success)
        {
            s_cwState = CW_RX;
            /* No parameters to return */
            *outParamsLength = 0;
            /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
        }
        else
        {
            /* Channel is out of range */
            status = PARAMETER_OUT_OF_MANDATORY_RANGE;
            /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
            *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
        }
    }
    else
    {
        /* Not in idle */
        status = COMMAND_DISALLOWED;
        /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }

    return status;
}

static ble_stat_t StopCw(uint8_t nbOfInParams,
                         uint8_t *inParams,
                         uint8_t *evntPcktP,
                         uint8_t *outParamsLength,
                         hci_return_command_type *returnCommandType)
{
    ble_stat_t status = SUCCESS;
    if (BLEController_ContinuousWave_Stop() == kBLEC_Success)
    {
        /* No parameters to return */
        *outParamsLength = 0;
        /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
        s_cwState             = CW_IDLE;
        status               = SUCCESS;
    }
    else
    {
        /* Should never come here */
        status = UNSPECIFIED_ERROR;
        /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }
    return status;
}

static ble_stat_t ReadCwRSSI(uint8_t nbOfInParams,
                             uint8_t *inParams,
                             uint8_t *evntPcktP,
                             uint8_t *outParamsLength,
                             hci_return_command_type *returnCommandType)
{
#define BLACKBOX_AVERAGING_NR_OF_SAMPLES (8u)
    ble_stat_t status = SUCCESS;
    if (s_cwState == CW_RX)
    {
        /* Read rssi 8 times */
        int16_t rssi = 0;
        for (int i = 0; i < BLACKBOX_AVERAGING_NR_OF_SAMPLES; i++)
        {
            rssi += BLEController_ContinuousWave_ReadRSSI();
        }
        /* Return rssi in dBm and divide and round the sum */
        rssi = (rssi - BLACKBOX_AVERAGING_NR_OF_SAMPLES * 2) / (BLACKBOX_AVERAGING_NR_OF_SAMPLES * 4);

        /* Parameters to return */
        *outParamsLength = 2;
        /* Return HCI_RETURN_COMMAND_TYPE_COMPLETE  */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_COMPLETE;
        evntPcktP[0]       = rssi & 0xff;
        evntPcktP[1]       = (rssi >> 8) & 0xff;
    }
    else
    {
        /* Not in rx */
        status = COMMAND_DISALLOWED;
        /* Return HCI_RETURN_COMMAND_TYPE_STATUS to mention that something went wrong */
        *returnCommandType = HCI_RETURN_COMMAND_TYPE_STATUS;
    }

    return status;
}

static ble_stat_t PrintString(uint8_t nbOfInParams,
                              uint8_t *inParams,
                              uint8_t *evntPcktP,
                              uint8_t *outParamsLength,
                              hci_return_command_type *returnCommandType)
{
    /* If the HCI UART is faster than the printing UART inParams can get
       overwritten by the next HCI packet before they get printed.
       To avoid this, copy the packet in the stack */
    char str[HCI_BLACKBOX_INIT_STRING_SIZE];
    memcpy(str, inParams, HCI_BLACKBOX_INIT_STRING_SIZE);
    str[HCI_BLACKBOX_INIT_STRING_SIZE - 1] = '\0';
    PRINTF("%s\n", (char *)str);
    return SUCCESS;
}
/*******************************************************************************
 * Public functions
 ******************************************************************************/

int HciCustomCmdPreMainInit(void)
{
    int res;
    (void)res;
    res = HCICUSTOMCMD_Register(s_customHCIOperations_ContinuousWave, sizeof(s_customHCIOperations_ContinuousWave) /
                                                                        sizeof(s_customHCIOperations_ContinuousWave[0]));
    assert(res == 0);
    res = HCICUSTOMCMD_Register(s_customHCIOperations_Power,
                                sizeof(s_customHCIOperations_Power) / sizeof(s_customHCIOperations_Power[0]));
    assert(res == 0);
    res = HCICUSTOMCMD_Register(s_customHCIOperations_Utils,
                                sizeof(s_customHCIOperations_Utils) / sizeof(s_customHCIOperations_Utils[0]));
    assert(res == 0);

    return 0;
}
