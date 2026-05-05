/*
    * Copyright 2025 NXP 
    *
    * SPDX-License-Identifier: BSD-3-Clause 
*/

/*
    * FILE NAME: ../../../boards/twrmc56f8200/demo_apps/mc_pmsm/pmsm_snsless/m1_pmsm_appconfig.h
    * DATE: Wed Jun 11 2025, 10:43:07
*/

/*
{
    "parameters": {
        "parametersNmax": 4400,
        "parametersImax": 8,
        "parametersEmax": 15.1,
        "parametersPp": 4,
        "parametersRs": 0.54,
        "parametersLd": 0.0003356,
        "parametersLq": 0.000218,
        "parametersKe": 0.05477461,
        "parametersJ": 0.00001,
        "parametersIphNom": 8,
        "parametersUphNom": 15,
        "parametersNnom": 4000,
        "parametersUdcbMax": 36.3,
        "parametersUdcbTrip": 29,
        "parametersUdcbUnder": 16,
        "parametersUdcbOver": 32,
        "parametersNover": 4399,
        "parametersNmin": 300,
        "parametersEblock": 0.7,
        "parametersEblockPer": 2000,
        "parametersKt": 0.01217,
        "parametersUdcbIIRf0": 100,
        "parametersCalibDuration": 0.2,
        "parametersFaultDuration": 6,
        "parametersFreewheelDuration": 1.5,
        "parametersScalarUqMin": 1,
        "parametersAlignVoltage": 1,
        "parametersAlignDuration": 0.4,
        "parametersScalarVHzRatio": 100
    },
    "currentLoop": {
        "currentLoopSampleTime": 0.0001,
        "currentLoopF0": 288,
        "currentLoopKsi": 1,
        "currentLoopOutputLimit": 90
    },
    "speedLoop": {
        "speedLoopSampleTime": 0.001,
        "speedLoopF0": 28,
        "speedLoopKsi": 1,
        "speedLoopIncUp": 10000,
        "speedLoopIncDown": 10000,
        "speedLoopCutOffFreq": 100,
        "speedLoopUpperLimit": 3,
        "speedLoopLowerLimit": -3,
        "speedLoopSLKp": 0.003,
        "speedLoopSLKi": 0.009,
        "speedLoopManualConstantTunning": true
    },
    "sensorless": {
        "sensorlessBemfObsrvF0": 300,
        "sensorlessBemfObsrvKsi": 1,
        "sensorlessTrackObsrvF0": 70,
        "sensorlessTrackObsrvKsi": 1,
        "sensorlessTrackObsrvIIRSpeedCutOff": 400,
        "sensorlessStartupRamp": 3000,
        "sensorlessStartupCurrent": 0.6,
        "sensorlessMergingSpeed": 500,
        "sensorlessMergingCoeff": 150
    }
}
*/

/*
{
    "motorName": "Teknic M-2311P-LN-08D",
    "motorDescription": "Current loop sample frequency 10KHz."
}
*/

#ifndef __M1_PMSM_APPCONFIG_H 
#define __M1_PMSM_APPCONFIG_H 

/* PARAMETERS*/
#define M1_N_MAX (4400.0F)
#define M1_FREQ_MAX (293.333F)
#define M1_I_MAX (8.0F)
#define M1_U_MAX (20.9578F)
#define M1_E_MAX (15.1F)
#define M1_MOTOR_PP (4)
#define M1_I_PH_NOM FRAC16(1.0)
#define M1_N_NOM FRAC16(0.909091)
#define M1_U_DCB_MAX (36.3F)
#define M1_U_DCB_TRIP FRAC16(0.798898)
#define M1_U_DCB_UNDERVOLTAGE FRAC16(0.440771)
#define M1_U_DCB_OVERVOLTAGE FRAC16(0.881543)
#define M1_N_OVERSPEED FRAC16(0.999773)
#define M1_N_MIN FRAC16(0.0681818)
#define M1_E_BLOCK_TRH FRAC16(0.0463576)
#define M1_E_BLOCK_PER (2000)
#define M1_CALIB_DURATION (200)
#define M1_FAULT_DURATION (6000)
#define M1_FREEWHEEL_DURATION (1500)
#define M1_SCALAR_UQ_MIN FRAC16(0.0477149)
#define M1_ALIGN_VOLTAGE FRAC16(0.0477149)
#define M1_ALIGN_DURATION (4000)
#define M1_UDCB_IIR_B0 FRAC32(0.0152295)
#define M1_UDCB_IIR_B1 FRAC32(0.0152295)
#define M1_UDCB_IIR_A1 FRAC32(0.469541)
#define M1_SCALAR_VHZ_FACTOR_GAIN FRAC16(0.787295)
#define M1_SCALAR_VHZ_FACTOR_SHIFT (0)
#define M1_SCALAR_INTEG_GAIN ACC32(0.0586667)
#define M1_SCALAR_RAMP_UP FRAC32(0.000227273)
#define M1_SCALAR_RAMP_DOWN FRAC32(0.000227273)
/* CURRENTLOOP*/
#define M1_D_KP_GAIN ACC32(0.257498)
#define M1_D_KI_GAIN ACC32(0.0209740)
#define M1_Q_KP_GAIN ACC32(0.0950354)
#define M1_Q_KI_GAIN ACC32(0.0136243)
#define M1_CLOOP_LIMIT FRAC16(0.9)
/* SPEEDLOOP*/
#define M1_SPEED_RAMP_UP FRAC32(0.00227273)
#define M1_SPEED_RAMP_DOWN FRAC32(0.00227273)
#define M1_SPEED_LOOP_HIGH_LIMIT FRAC16(0.375)
#define M1_SPEED_LOOP_LOW_LIMIT FRAC16(-0.375)
#define M1_SPEED_PI_PROP_GAIN ACC32(0.691150)
#define M1_SPEED_PI_INTEG_GAIN ACC32(0.00207345)
#define M1_SPEED_IIR_B0 FRAC32(0.119529)
#define M1_SPEED_IIR_B1 FRAC32(0.119529)
#define M1_SPEED_IIR_A1 FRAC32(0.260943)
/* SENSORLESS*/
#define M1_OL_START_RAMP_INC FRAC32(0.0000681818)
#define M1_OL_START_I FRAC16(0.075)
#define M1_MERG_SPEED_TRH FRAC16(0.113636)
#define M1_MERG_COEFF FRAC16(0.005)
#define M1_I_SCALE ACC32(0.861396)
#define M1_U_SCALE ACC32(0.672415)
#define M1_E_SCALE ACC32(0.484471)
#define M1_WI_SCALE ACC32(0.103129)
#define M1_BEMF_DQ_KP_GAIN ACC32(0.384202)
#define M1_BEMF_DQ_KI_GAIN ACC32(0.0631738)
#define M1_TO_KP_SHIFT (-1)
#define M1_TO_KP_GAIN FRAC16(0.954545)
#define M1_TO_KI_SHIFT (-6)
#define M1_TO_KI_GAIN FRAC16(0.671730)
#define M1_TO_THETA_SHIFT (-4)
#define M1_TO_THETA_GAIN FRAC16(0.938667)
#define M1_TO_SPEED_IIR_B0 FRAC32(0.0558176)
#define M1_TO_SPEED_IIR_B1 FRAC32(0.0558176)
#define M1_TO_SPEED_IIR_A1 FRAC32(0.388365)
/* USER INPUT START */
/* USER INPUT END */
#endif /* __M1_PMSM_APPCONFIG_H */
