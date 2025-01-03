/*
 * Copyright 2016 - 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
/*${standard_header_anchor}*/
#include "usb_pd_config.h"
#include "usb_pd.h"
#include "usb_pd_i2c.h"
#include "pd_app.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pd_power_interface.h"
#include "board.h"
#if (defined PD_CONFIG_ENABLE_AUTO_POLICY) && (PD_CONFIG_ENABLE_AUTO_POLICY)
#include "usb_pd_auto_policy.h"
#endif
#include "pd_board_config.h"
#include "usb_pd_alt_mode.h"
#include "usb_pd_alt_mode_dp.h"
#include "pd_crossbar_cbtl.h"
#include "pd_dp_hpd_detect.h"
#include "pd_ptn36502.h"
#include "pd_dp_board_chip.h"
#include "pd_power_nx20p3483.h"
#include "fsl_adapter_timer.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if (PD_DEMO_PORTS_COUNT > PD_CONFIG_MAX_PORT)
#error "please increase the instance count"
#endif

/* The follow MACROs are for passing compliance test, it is not actual product logical */
#define PD_SOURCE_POWER (18) /* W */
#ifndef PD_TIMER_INSTANCE
#define PD_TIMER_INSTANCE (0)
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void PD_BMEnterCritical(uint32_t *sr);
void PD_BMExitCritical(uint32_t sr);
uint32_t PD_DemoSoftTimer_msGet(void);
uint32_t PD_DemoSoftTimer_getInterval(uint32_t startTime);
uint32_t HW_I2CGetFreq(uint8_t i2cInstance);
void HW_I2CReleaseBus(void);
void PD_DemoInit(void);
void PD_DemoTaskFun(void);
void BOARD_InitHardware(void);
uint32_t HW_TimerGetFreq(void);
void HW_TimerCallback(void *callbackParam);
#if defined(PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT) && (PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT)
void PD_PortsGpioExternalPowerDetectIntCallback(void *callbackParam);
#endif
pd_status_t PD_DpmAppCommandCallback(void *callbackParam, uint32_t event, void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static pd_source_pdo_t s_PortSourceCaps[] = {
    {
        /* PDO1: fixed supply: dual-role power; Externally Powered; no USB communication; dual-role data; 5V; 3A */
        .fixedPDO.dualRoleData    = 1,
        .fixedPDO.dualRolePower   = 1,
        .fixedPDO.externalPowered = 1,
        .fixedPDO.fixedSupply     = kPDO_Fixed,
        .fixedPDO.maxCurrent      = (3 * 100),
        .fixedPDO.peakCurrent     = 0,
#if ((defined PD_CONFIG_REVISION) && (PD_CONFIG_REVISION >= PD_SPEC_REVISION_30))
        .fixedPDO.unchunkedSupported = 1,
#endif
        .fixedPDO.usbCommunicationsCapable = 0,
        .fixedPDO.usbSuspendSupported      = 0,
        .fixedPDO.voltage                  = (5 * 1000 / 50),
    },
    {
        /* PDO2: fixed Supply: 9V - 2A */
        .fixedPDO.fixedSupply = kPDO_Fixed,
        .fixedPDO.maxCurrent  = (2 * 100),
        .fixedPDO.voltage     = (9 * 1000 / 50),
    },
};

static pd_sink_pdo_t s_PortSinkcaps[] = {{
                                             /* PDO1: fixed:5.0V, 3A */
                                             .fixedPDO.fixedSupply     = kPDO_Fixed,
                                             .fixedPDO.dualRoleData    = 1,
                                             .fixedPDO.dualRolePower   = 1,
                                             .fixedPDO.externalPowered = 1,
#if ((defined PD_CONFIG_REVISION) && (PD_CONFIG_REVISION >= PD_SPEC_REVISION_30))
#if (defined PD_CONFIG_COMPLIANCE_TEST_ENABLE) && (PD_CONFIG_COMPLIANCE_TEST_ENABLE)
                                             .fixedPDO.frSwapRequiredCurrent = 0,
#else
                                             .fixedPDO.frSwapRequiredCurrent = kFRSwap_CurrentDefaultUSB,
#endif
#endif
                                             .fixedPDO.higherCapability         = 1,
                                             .fixedPDO.usbCommunicationsCapable = 0,
                                             .fixedPDO.voltage                  = (5 * 1000 / 50),
                                             .fixedPDO.operateCurrent           = (3 * 100),
                                         },
                                         {
                                             /* PDO2: fixed: 9V, 2A */
                                             .fixedPDO.fixedSupply    = kPDO_Fixed,
                                             .fixedPDO.voltage        = (9 * 1000 / 50),
                                             .fixedPDO.operateCurrent = (2 * 100),
                                         }};

const static pd_dp_peripheral_interface_t s_boardChipInterface = {PD_DpBoardChipsInit, PD_DpBoardChipsDeinit,
                                                                  PD_DpBoardChipsControl};

#if (defined PD_CONFIG_ENABLE_AUTO_POLICY) && (PD_CONFIG_ENABLE_AUTO_POLICY)
static pd_auto_policy_t s_autoPolicyForAllPorts = {
    0,                          /* autoRequestPRSwapAsSource */
    1,                          /* autoRequestPRSwapAsSink */
    kAutoRequestProcess_Accept, /* autoAcceptPRSwapAsSource */
    kAutoRequestProcess_Accept, /* autoAcceptPRSwapAsSink */
    kPD_DataRoleNone,           /* autoRequestDRSwap */
    kAutoRequestProcess_Reject, /* autoAcceptDRSwapToDFP */
    kAutoRequestProcess_Accept, /* autoAcceptDRSwapToUFP */
    kPD_VconnNone,              /* autoRequestVConnSwap */
    kAutoRequestProcess_Accept, /* autoAcceptVconnSwapToOn */
    kAutoRequestProcess_Accept, /* autoAcceptVconnSwapToOff */
    1,                          /* autoSinkNegotiation */
};
#endif

#if (defined PD_DEMO_PORT1_ENABLE) && (PD_DEMO_PORT1_ENABLE > 0)
/* displayport config */
const static pd_cbtl_crossbar_config_t s_CrossbarConfig1 = {
    PD_PORT1_SHIELD2_ORIENT_PORT,  /* orientControlPort */
    PD_PORT1_SHIELD2_ORIENT_PIN,   /* orientControlPin */
    PD_PORT1_SHIELD2_XSD_PORT,     /* xsdnControlPort */
    PD_PORT1_SHIELD2_XSD_PIN,      /* xsdnControlPin */
    PD_PORT1_SHIELD2_DP4LANE_PORT, /* dp4laneControlPort */
    PD_PORT1_SHIELD2_DP4LANE_PIN,  /* dp4laneControlPin */
};
const static pd_hpd_detect_config_t s_HpdDetectConfig1 = {
    PD_PORT1_SHIELD2_HPD_PORT, /* hpd port */
    PD_PORT1_SHIELD2_HPD_PIN,  /* hpd pin */
};

static pd_dp_redriver_config_t s_Ptn36502Config1 = {
    kInterface_i2c0 + BOARD_PD_I2C_INDEX, /* I2C master */
    PD_PTN36502_SLAVE_ADDRESS,            /* slave address */
    0u,                                   /* Source clock */
    (void *)HW_I2CReleaseBus,             /* I2C release bus */
};

const static pd_dp_peripheral_config_t s_dpBoardConfig1 = {
    (void *)&s_CrossbarConfig1,
    (void *)&s_HpdDetectConfig1,
    (void *)&s_Ptn36502Config1,
};

const static pd_dp_mode_obj_t s_DPCaps[] = {
    {
        .bitFields.portCap              = kDPPortCap_UFPD,
        .bitFields.signalSupport        = kDPSignal_DP,
        .bitFields.receptacleIndication = kReceptacle_TypeCReceptacle,
        .bitFields.usb2SignalNotUsed    = kUSB2_Required,
        .bitFields.DFPDPinSupport       = kPinAssign_DeSelect,
        .bitFields.UFPDPinSupport       = kPinAssign_C | kPinAssign_D | kPinAssign_E,
    },
};

static pd_alt_mode_dp_config_t s_DPConfig1 = {.peripheralConfig    = &s_dpBoardConfig1,
                                              .peripheralInterface = &s_boardChipInterface,
                                              .modesList           = (uint32_t *)&s_DPCaps, /* mode list */
                                              .modesCount          = 1,                     /* mode count */
                                              .dpStatusConfig =
                                                  {
                                                      .bitFields.DFPDUFPDConnected      = kUFP_D_Connected,
                                                      .bitFields.powerLow               = 0,
                                                      .bitFields.enabled                = 1,
                                                      .bitFields.multiFunctionPreferred = 0,
                                                  },
                                              .dpConfigurationsConfig = {
                                                  .configureVal = 0,
                                              }};

const static pd_vdm_identity_data_t s_DPDiscoverIdentityVDO = {
    .idHeaderVDO =
        {
            .bitFields.usbVendorID                     = PD_VENDOR_VID,
            .bitFields.reserved                        = 0,
            .bitFields.connectorType                   = kConnectorType_UsbTypecReceptacle,
            .bitFields.productTypeDFP                  = kProductTypeDfp_Undefined,
            .bitFields.modalOperateSupport             = kModalOperation_Supported,
            .bitFields.productTypeUFPOrCablePlug       = kProductTypeUfp_PdUsbPeripheral,
            .bitFields.usbCommunicationCapableAsDevice = kUsbCommunicationCapableAsDevice_Unsupported,
            .bitFields.usbCommunicationCapableAsHost   = kUsbCommunicationCapableAsHost_Unsupported,
        },
    .certStatVDO = PD_CERT_STAT_VDO,
    .bcdDevice   = PD_CONFIG_BCD_DEVICE,
    .pid         = PD_CONFIG_PID,
    .ufpVDO =
        {
            .bitFields.usbHighestSpeedSupported = kUsbSuperSpeedSiganl_Usb32Gen1,
            .bitFields.alternateModesSupported  = kAlternateMode_ReconfigureTypecExceptForTBT3,
            .bitFields.vbusRequired             = kVbusRequired_Yes,
            .bitFields.vconnRequired            = kVconnRequired_Yes,
            .bitFields.vconnPower               = kVconnPower_1W5,
            .bitFields.connectorType            = kConnectorType_UsbTypecReceptacle,
            .bitFields.deviceCapability         = kDeviceCapability_Usb32,
            .bitFields.vdoVersion               = PD_VDO_VERSION_12,
        },
};

/* alt mode config */
const static pd_alt_mode_module_t s_AltModeModules1[] = {{0xFF01, &s_DPConfig1}};

const static pd_alt_mode_config_t s_AltModeConfig1 = {
    .altModeRole         = kDataConfig_UFP,
    .modules             = &s_AltModeModules1[0],
    .moduleCount         = 1,
    .identityData        = (uint32_t *)&s_DPDiscoverIdentityVDO,
    .identityObjectCount = 4U,
};

const static pd_power_port_config_t s_Port1PowerConfig = {
    (uint32_t *)&s_PortSourceCaps[0],                   /* source caps */
    (uint32_t *)&s_PortSinkcaps[0],                     /* self sink caps */
    sizeof(s_PortSourceCaps) / sizeof(pd_source_pdo_t), /* source cap count */
    sizeof(s_PortSinkcaps) / sizeof(pd_sink_pdo_t),     /* sink cap count */
    kPowerConfig_SinkDefault,                           /* typec role */
    PD_DEMO_TYPEC_CURRENT,                              /* source: Rp current level */
    kTypecTry_None,                                     /* drp try function */
    kDataConfig_DRD,                                    /* data function */
    1,                                                  /* support vconn */
    0,                                                  /* reserved */
    (void *)&s_AltModeConfig1,                          /* alt mode config */
#if (defined PD_CONFIG_ENABLE_AUTO_POLICY) && (PD_CONFIG_ENABLE_AUTO_POLICY)
    (void *)&s_autoPolicyForAllPorts, /* auto policy */
#else
    NULL, /* auto policy */
#endif
    NULL,
};

static pd_phy_config_t s_Port1PhyConfig = {
    .i2cInstance   = kInterface_i2c0 + BOARD_PD_I2C_INDEX,
    .slaveAddress  = 0x52u,
    .i2cSrcClock   = 0u,
    .i2cReleaseBus = HW_I2CReleaseBus,
    .alertPort     = PD_PORT1_PHY_INTERRUPT_PORT,
    .alertPin      = PD_PORT1_PHY_INTERRUPT_PIN,
    .alertPriority = PD_PORT1_PHY_INTERRUPT_PRIORITY,
};

pd_instance_config_t g_Port1PDConfig = {
    kDeviceType_NormalPowerPort, /* normal power port */
    kPD_PhyPTN5110,
    (void *)&s_Port1PhyConfig,
    (void *)&s_Port1PowerConfig,
};
#endif

pd_instance_config_t *g_PortsConfigArray[] = {
#if (defined PD_DEMO_PORT1_ENABLE) && (PD_DEMO_PORT1_ENABLE)
    &g_Port1PDConfig,
#endif
};

#if (defined PD_DEMO_PORT1_ENABLE) && (PD_DEMO_PORT1_ENABLE > 0)
pd_app_t g_PDAppInstancePort1;
#endif
#if (defined PD_DEMO_PORT2_ENABLE) && (PD_DEMO_PORT2_ENABLE > 0)
pd_app_t g_PDAppInstancePort2;
#endif
#if (defined PD_DEMO_PORT3_ENABLE) && (PD_DEMO_PORT3_ENABLE > 0)
pd_app_t g_PDAppInstancePort3;
#endif
#if (defined PD_DEMO_PORT4_ENABLE) && (PD_DEMO_PORT4_ENABLE > 0)
pd_app_t g_PDAppInstancePort4;
#endif

pd_app_t *g_PDAppInstanceArray[] = {
#if (defined PD_DEMO_PORT1_ENABLE) && (PD_DEMO_PORT1_ENABLE > 0)
    &g_PDAppInstancePort1,
#endif
#if (defined PD_DEMO_PORT2_ENABLE) && (PD_DEMO_PORT2_ENABLE > 0)
    &g_PDAppInstancePort2,
#endif
#if (defined PD_DEMO_PORT3_ENABLE) && (PD_DEMO_PORT3_ENABLE > 0)
    &g_PDAppInstancePort3,
#endif
#if (defined PD_DEMO_PORT4_ENABLE) && (PD_DEMO_PORT4_ENABLE > 0)
    &g_PDAppInstancePort4,
#endif
};

pd_power_handle_callback_t callbackFunctions = {
    PD_PowerSrcTurnOnDefaultVbus,  PD_PowerSrcTurnOnRequestVbus,  PD_PowerSrcTurnOffVbus,
    PD_PowerSrcGotoMinReducePower, PD_PowerSnkDrawTypeCVbus,      PD_PowerSnkDrawRequestVbus,
    PD_PowerSnkStopDrawVbus,       PD_PowerSnkGotoMinReducePower, PD_PowerControlVconn,
};

pd_demo_global_t g_DemoGlobal;
volatile uint32_t g_SoftTimerCount;
TIMER_HANDLE_DEFINE(g_PDTimerHandle);
/*******************************************************************************
 * Code
 ******************************************************************************/

static void PD_GpioInit(pd_app_t *pdAppInstance)
{
    pd_demo_io_init_t portsDemoPinConfigArray[] = {
#if (defined PD_DEMO_PORT1_ENABLE) && (PD_DEMO_PORT1_ENABLE)
        {
            .extraEnSrcPort = PD_PORT1_EXTRA_SRC_PORT,
            .extraEnSrcPin  = PD_PORT1_EXTRA_SRC_PIN,

#if defined(PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT) && (PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT)
            .externalPowerDetectPort = PD_PORT1_SHIELD2_EXTERNAL_POWER_DETECT_PORT,
            .externalPowerDetectPin  = PD_PORT1_SHIELD2_EXTERNAL_POWER_DETECT_PIN,
#endif
        },
#endif
    };
    pd_demo_io_init_t *demoGpioPinConfig = &portsDemoPinConfigArray[pdAppInstance->portNumber - 1];
    hal_gpio_pin_config_t config;

    config.direction = kHAL_GpioDirectionOut;
    config.port      = demoGpioPinConfig->extraEnSrcPort;
    config.pin       = demoGpioPinConfig->extraEnSrcPin;
    config.level     = 1;
    HAL_GpioInit((hal_gpio_handle_t *)(&pdAppInstance->gpioExtraSrcHandle[0]), &config);

#if defined(PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT) && (PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT)
    /* the external power */
    config.direction = kHAL_GpioDirectionIn;
    config.port      = demoGpioPinConfig->externalPowerDetectPort;
    config.pin       = demoGpioPinConfig->externalPowerDetectPin;
    config.level     = 0;
    HAL_GpioInit((hal_gpio_handle_t *)(&pdAppInstance->gpioExternalPowerDetectHandle[0]), &config);

    HAL_GpioInstallCallback((hal_gpio_handle_t *)(&pdAppInstance->gpioExternalPowerDetectHandle[0]),
                            PD_PortsGpioExternalPowerDetectIntCallback, (void *)pdAppInstance);
#endif
}

void PD_BMEnterCritical(uint32_t *sr)
{
    *sr = DisableGlobalIRQ();
    __ASM("CPSID i");
}

void PD_BMExitCritical(uint32_t sr)
{
    EnableGlobalIRQ(sr);
}

#if defined(PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT) && (PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT)
void PD_PortsGpioExternalPowerDetectIntCallback(void *callbackParam)
{
    uint8_t externalPowerState;
    pd_app_t *pdAppInstance = (pd_app_t *)callbackParam;

    HAL_GpioGetInput((hal_gpio_handle_t)(&pdAppInstance->gpioExternalPowerDetectHandle[0]), &externalPowerState);
    if (externalPowerState == pdAppInstance->externalPowerState)
    {
        return;
    }
    pdAppInstance->externalPowerState = externalPowerState;

    if (externalPowerState)
    {
        ((pd_sink_fixed_pdo_t *)(((pd_power_port_config_t *)(pdAppInstance->pdConfigParam->portConfig))->sinkCaps))[0]
            .externalPowered = 1;
        ((pd_source_fixed_pdo_t *)(((pd_power_port_config_t *)(pdAppInstance->pdConfigParam->portConfig))
                                       ->sourceCaps))[0]
            .externalPowered = 1;
    }
    else
    {
        ((pd_sink_fixed_pdo_t *)(((pd_power_port_config_t *)(pdAppInstance->pdConfigParam->portConfig))->sinkCaps))[0]
            .externalPowered = 0;
        ((pd_source_fixed_pdo_t *)(((pd_power_port_config_t *)(pdAppInstance->pdConfigParam->portConfig))
                                       ->sourceCaps))[0]
            .externalPowered = 0;
    }

    PD_Control(pdAppInstance->pdHandle, PD_CONTROL_INFORM_EXTERNAL_POWER_STATE, &externalPowerState);
}
#endif

static pd_status_t PD_DpmConnectCallback(void *callbackParam, uint32_t event, void *param)
{
    pd_status_t status      = kStatus_PD_Error;
    pd_app_t *pdAppInstance = (pd_app_t *)callbackParam;

    switch (event)
    {
        case PD_DISCONNECTED:
#if defined(PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT) && (PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT)
            HAL_GpioSetTriggerMode((hal_gpio_handle_t)(&pdAppInstance->gpioExternalPowerDetectHandle[0]),
                                   kHAL_GpioInterruptDisable); /* Disable interrupt */
#endif
            PD_PowerSnkStopDrawVbus(pdAppInstance, kVbusPower_Stable);
            PRINTF("port %d disconnect\r\n", pdAppInstance->portNumber);
            status = kStatus_PD_Success;
            break;

        case PD_CONNECT_ROLE_CHANGE:
        case PD_CONNECTED:
        {
            uint8_t getInfo;

#if defined(PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT) && (PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT)
            HAL_GpioGetInput((hal_gpio_handle_t)(&pdAppInstance->gpioExternalPowerDetectHandle[0]),
                             (uint8_t *)&pdAppInstance->externalPowerState);
            if (pdAppInstance->externalPowerState)
            {
                ((pd_sink_fixed_pdo_t *)(((pd_power_port_config_t *)(pdAppInstance->pdConfigParam->portConfig))
                                             ->sinkCaps))[0]
                    .externalPowered = 1;
                ((pd_source_fixed_pdo_t *)(((pd_power_port_config_t *)(pdAppInstance->pdConfigParam->portConfig))
                                               ->sourceCaps))[0]
                    .externalPowered = 1;
            }
            else
            {
                ((pd_sink_fixed_pdo_t *)(((pd_power_port_config_t *)(pdAppInstance->pdConfigParam->portConfig))
                                             ->sinkCaps))[0]
                    .externalPowered = 0;
                ((pd_source_fixed_pdo_t *)(((pd_power_port_config_t *)(pdAppInstance->pdConfigParam->portConfig))
                                               ->sourceCaps))[0]
                    .externalPowered = 0;
            }

            HAL_GpioSetTriggerMode((hal_gpio_handle_t)(&pdAppInstance->gpioExternalPowerDetectHandle[0]),
                                   kHAL_GpioInterruptEitherEdge); /* Enable interrupt */
#endif
            PD_NX20PExitDeadBatteryMode(pdAppInstance->portNumber);
            pdAppInstance->partnerSourceCapNumber = 0;
            pdAppInstance->partnerSinkCapNumber   = 0;
            pdAppInstance->portShieldVersion      = 0;
            PD_Control(pdAppInstance->pdHandle, PD_CONTROL_GET_POWER_ROLE, &getInfo);

            PRINTF((event == PD_CONNECTED) ? "port %d connected," : "port %d connect change,",
                   pdAppInstance->portNumber);
            PRINTF(" power role:%s,", (getInfo == kPD_PowerRoleSource) ? "Source" : "Sink");
            PD_Control(pdAppInstance->pdHandle, PD_CONTROL_GET_DATA_ROLE, &getInfo);
            PRINTF(" data role:%s,", (getInfo == kPD_DataRoleDFP) ? "DFP" : "UFP");
            PD_Control(pdAppInstance->pdHandle, PD_CONTROL_GET_VCONN_ROLE, &getInfo);
            PRINTF(" vconn source:%s\r\n", (getInfo == kPD_IsVconnSource) ? "yes" : "no");
            PRINTF("Please input '0' to show test menu\r\n");
            status = kStatus_PD_Success;
            break;
        }

        default:
            break;
    }

    return status;
}

pd_status_t PD_StackDemoAppCallback(void *callbackParam, uint32_t event, void *param)
{
    pd_status_t status      = kStatus_PD_Error;
    pd_app_t *pdAppInstance = (pd_app_t *)callbackParam;

    switch (event)
    {
        case PD_FUNCTION_DISABLED:
            /* need hard or software reset */
            status = kStatus_PD_Success;
            break;

        case PD_CONNECTED:
        case PD_CONNECT_ROLE_CHANGE:
        case PD_DISCONNECTED:
            status = PD_DpmConnectCallback(callbackParam, event, param);
            break;

        case PD_DPM_ALTMODE_DP_UFP_MODE_CONFIGURED:
        {
            PRINTF("enter DP and configure done\r\n");
            break;
        }

        case PD_DPM_ALTMODE_DP_DFP_MODE_UNCONFIGURED:
            PRINTF("exit DP\r\n");
            break;

        case PD_DPM_OVP_OCP_FAULT:
            PD_NX20PClearInt(pdAppInstance->portNumber);
            break;

#if defined(PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT) && (PD_CONFIG_EXTERNAL_POWER_DETECTION_SUPPORT)
        case PD_DPM_GET_EXTERNAL_POWER_STATE:
            HAL_GpioGetInput((hal_gpio_handle_t)(&pdAppInstance->gpioExternalPowerDetectHandle[0]), (uint8_t *)param);
            status = kStatus_PD_Success;
            break;
#endif

        case PD_DPM_VBUS_ALARM:
            /* Users need to take care of this. Some corrective actions may be taken, such as disconnect
               or debounce time to turn off the power switch. It is up to the system power management. */
            break;

        default:
            status = PD_DpmAppCommandCallback(callbackParam, event, param);
            break;
    }
    return status;
}

static void PD_AppInit(void)
{
    uint8_t index;
    pd_app_t *pdAppInstance;
    pd_app_t *pdAppInstanceArray[] = {
#if (defined PD_DEMO_PORT1_ENABLE) && (PD_DEMO_PORT1_ENABLE > 0)
        &g_PDAppInstancePort1,
#else
        NULL,
#endif
#if (defined PD_DEMO_PORT2_ENABLE) && (PD_DEMO_PORT2_ENABLE > 0)
        &g_PDAppInstancePort2,
#else
        NULL,
#endif
#if (defined PD_DEMO_PORT3_ENABLE) && (PD_DEMO_PORT3_ENABLE > 0)
        &g_PDAppInstancePort3,
#else
        NULL,
#endif
#if (defined PD_DEMO_PORT4_ENABLE) && (PD_DEMO_PORT4_ENABLE > 0)
        &g_PDAppInstancePort4,
#else
        NULL,
#endif
    };

    g_SoftTimerCount = 0;
    for (index = 0; index < 4; ++index)
    {
        if (pdAppInstanceArray[index] != NULL)
        {
            pdAppInstanceArray[index]->portNumber = (index + 1);
        }
    }

    for (index = 0; index < PD_DEMO_PORTS_COUNT; ++index)
    {
        pdAppInstance                = g_PDAppInstanceArray[index];
        pdAppInstance->pdHandle      = NULL;
        pdAppInstance->pdConfigParam = g_PortsConfigArray[index];
        ((pd_phy_config_t *)pdAppInstance->pdConfigParam->phyConfig)->i2cSrcClock =
            HW_I2CGetFreq(((pd_phy_config_t *)pdAppInstance->pdConfigParam->phyConfig)->i2cInstance);

        if (PD_InstanceInit(&pdAppInstance->pdHandle, PD_StackDemoAppCallback, &callbackFunctions, pdAppInstance,
                            g_PortsConfigArray[index]) != kStatus_PD_Success)
        {
            PRINTF("pd port %d init fail\r\n", pdAppInstance->portNumber);
        }
        else
        {
            PD_GpioInit(pdAppInstance);
            PD_PowerBoardControlInit(pdAppInstance->portNumber, pdAppInstance->pdHandle,
                                     (hal_gpio_handle_t)(&pdAppInstance->gpioExtraSrcHandle[0]));

            pdAppInstance->msgSop                 = kPD_MsgSOP;
            pdAppInstance->partnerSourceCapNumber = 0;
            pdAppInstance->partnerSinkCapNumber   = 0;
            pdAppInstance->reqestResponse         = kCommandResult_Accept;
            /* self ext cap */
            pdAppInstance->selfExtCap.vid       = PD_VENDOR_VID;
            pdAppInstance->selfExtCap.pid       = PD_CONFIG_PID;
            pdAppInstance->selfExtCap.xid       = PD_CONFIG_XID;
            pdAppInstance->selfExtCap.fwVersion = PD_CONFIG_FW_VER;
            pdAppInstance->selfExtCap.hwVersion = PD_CONFIG_HW_VER;
            pdAppInstance->selfExtCap.batteries = 0;
            pdAppInstance->selfExtCap.sourcePDP = PD_SOURCE_POWER;
            /* self battery */
            pdAppInstance->selfBatteryCap.batteryDesignCap         = 10;
            pdAppInstance->selfBatteryCap.batteryLastFullChargeCap = 10;
            pdAppInstance->selfBatteryCap.batteryType              = 1; /* invalid battery reference */
            pdAppInstance->selfBatteryCap.pid                      = PD_CONFIG_PID;
            pdAppInstance->selfBatteryCap.vid                      = PD_VENDOR_VID;
            pdAppInstance->selfBatteryStatus.batterInfo            = 0;
            /* no battery */
            pdAppInstance->selfBatteryStatus.batterInfoBitFields.invalidBatteryRef = 1;
            pdAppInstance->selfBatteryStatus.batteryPC                             = 10;
            /* self status */
            pdAppInstance->selfStatus.eventFlags          = 0;
            pdAppInstance->selfStatus.internalTemp        = 0;
            pdAppInstance->selfStatus.presentBatteryInput = 0;
            pdAppInstance->selfStatus.presentInput        = 0;
            pdAppInstance->selfStatus.temperatureStatus   = 0;
            pdAppInstance->selfStatus.powerStatus         = 0;
            /* evaluate result */
            pdAppInstance->prSwapAccept    = 1;
            pdAppInstance->drSwapAccept    = 1;
            pdAppInstance->vconnSwapAccept = 1;
            PRINTF("pd port %d init success\r\n", pdAppInstance->portNumber);
        }
    }
}

/* ms */
uint32_t PD_DemoSoftTimer_msGet(void)
{
    return g_SoftTimerCount;
}

/* ms */
uint32_t PD_DemoSoftTimer_getInterval(uint32_t startTime)
{
    if (g_SoftTimerCount >= startTime)
    {
        return (g_SoftTimerCount - startTime);
    }
    else
    {
        return (0xFFFFFFFFu - (startTime - g_SoftTimerCount));
    }
}

/* us */
uint32_t PD_DemoGetTickValue(void)
{
    uint32_t timerUs;

    timerUs = HAL_TimerGetCurrentTimerCount((hal_timer_handle_t)&g_PDTimerHandle[0]) % 1000;
    return (timerUs + (g_SoftTimerCount * 1000));
}

/* us */
uint8_t PD_DemoTimrTickIsTimeAfter(uint32_t firstIntervalVal, uint32_t endIntervalVal)
{
    if (firstIntervalVal > endIntervalVal)
    {
        if ((firstIntervalVal - endIntervalVal) < 0x80000000UL)
        {
            return 1;
        }
    }
    else
    {
        if ((endIntervalVal - firstIntervalVal) > 0x80000000UL)
        {
            return 1;
        }
    }
    return 0;
}

static void PD_AppTimerInit(void)
{
    hal_timer_config_t halTimerConfig;
    halTimerConfig.timeout     = 1000;
    halTimerConfig.srcClock_Hz = HW_TimerGetFreq();
    halTimerConfig.instance    = PD_TIMER_INSTANCE;
    HAL_TimerInit((hal_timer_handle_t)&g_PDTimerHandle[0], &halTimerConfig);

    HAL_TimerInstallCallback((hal_timer_handle_t)&g_PDTimerHandle[0], HW_TimerCallback, NULL);
    HAL_TimerEnable((hal_timer_handle_t)&g_PDTimerHandle[0]);
}

void HW_TimerCallback(void *callbackParam)
{
    /* Callback into timer service */
    uint8_t index;
    for (index = 0; index < PD_DEMO_PORTS_COUNT; ++index)
    {
        PD_TimerIsrFunction(g_PDAppInstanceArray[index]->pdHandle);
    }
    PD_DpBoardChip1msIsr();
    g_SoftTimerCount++;
}

int main(void)
{
    BOARD_InitHardware();

    /* @TEST_ANCHOR */
    PD_AppTimerInit();
    PD_AppInit();
    PD_DemoInit();

    while (1)
    {
#if (defined PD_CONFIG_COMMON_TASK) && (PD_CONFIG_COMMON_TASK)
        PD_Task();
#else
        for (index = 0; index < PD_DEMO_PORTS_COUNT; ++index)
        {
            PD_InstanceTask(g_PDAppInstanceArray[index]->pdHandle);
        }
#endif
        PD_AltModeTask();
        PD_DemoTaskFun();
    }
}
