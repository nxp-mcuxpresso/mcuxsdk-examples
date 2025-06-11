/*
    * Copyright 2024 NXP
    *
    * SPDX-License-Identifier: BSD-3-Clause
*/

/*
    * FILE NAME: m1_pmsm_appconfig.h
    * DATE: Tue Jan 02 2024, 10:37:11
*/

/*
{
    "parameters": {
        "parametersNmax": 4400,
        "parametersImax": 8,
        "parametersEmax": 15.1,
        "parametersPP": 2,
        "parametersRs": 0.55,
        "parametersLd": 0.000403,
        "parametersLq": 0.000512,
        "parametersKe": 0.0154,
        "parametersJ": 0.0000016,
        "parametersIphNom": 2,
        "parametersUphNom": 15,
        "parametersNnom": 4000,
        "parametersUdcbMax": 36.3,
        "parametersUdcbTrip": 29,
        "parametersUdcbUnder": 16,
        "parametersUdcbOver": 32,
        "parametersNover": 4300,
        "parametersNmin": 400,
        "parametersEblock": 1.4,
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
        "currentLoopF0": 280,
        "currentLoopKsi": 1,
        "currentLoopOutputLimit": 90
    },
    "speedLoop": {
        "speedLoopSampleTime": 0.001,
        "speedLoopF0": 20,
        "speedLoopKsi": 1,
        "speedLoopIncUp": 3000,
        "speedLoopIncDown": 3000,
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
        "sensorlessTrackObsrvF0": 25,
        "sensorlessTrackObsrvKsi": 1,
        "sensorlessTrackObsrvIIRSpeedCutOff": 400,
        "sensorlessStartupRamp": 1000,
        "sensorlessStartupCurrent": 0.6,
        "sensorlessMergingSpeed": 500,
        "sensorlessMergingCoeff": 150
    }
}
*/

/*
{
    "motorName": "linix",
    "motorDescription": "Configuration for LINIX motor."
}
*/

#ifndef __M1_PMSM_APPCONFIG_H
#define __M1_PMSM_APPCONFIG_H

/* PARAMETERS*/
#define M1_N_MAX (4400.0F)
#define M1_FREQ_MAX (146.667F)
#define M1_I_MAX (8.0F)
#define M1_U_MAX (20.9578F)
#define M1_E_MAX (15.1F)
#define M1_MOTOR_PP (2)
#define M1_I_PH_NOM (0.25)
#define M1_N_NOM FRAC16(0.909091)
#define M1_U_DCB_MAX (36.3F)
#define M1_U_DCB_TRIP FRAC16(0.798898)
#define M1_U_DCB_UNDERVOLTAGE FRAC16(0.440771)
#define M1_U_DCB_OVERVOLTAGE FRAC16(0.881543)
#define M1_N_OVERSPEED FRAC16(0.977273)
#define M1_N_MIN FRAC16(0.0909091)
#define M1_E_BLOCK_TRH FRAC16(0.0927152)
#define M1_E_BLOCK_PER (2000)
#define M1_CALIB_DURATION (200)
#define M1_FAULT_DURATION (6000)
#define M1_FREEWHEEL_DURATION (1500)
#define M1_SCALAR_UQ_MIN FRAC16(0.0275482)
#define M1_ALIGN_VOLTAGE FRAC16(0.0477149)
#define M1_ALIGN_DURATION (4000)
#define M1_UDCB_IIR_B0 FRAC32(0.0152295)
#define M1_UDCB_IIR_B1 FRAC32(0.0152295)
#define M1_UDCB_IIR_A1 FRAC32(0.469541)
#define M1_SCALAR_VHZ_FACTOR_GAIN FRAC16(0.787295)
#define M1_SCALAR_VHZ_FACTOR_SHIFT (0)
#define M1_SCALAR_INTEG_GAIN ACC32(0.0293333)
#define M1_SCALAR_RAMP_UP FRAC32(0.0000681818)
#define M1_SCALAR_RAMP_DOWN FRAC32(0.0000681818)
/* CURRENTLOOP*/
#define M1_D_KP_GAIN ACC32(0.331328)
#define M1_D_KI_GAIN ACC32(0.0238064)
#define M1_Q_KP_GAIN ACC32(0.477727)
#define M1_Q_KI_GAIN ACC32(0.0302454)
#define M1_CLOOP_LIMIT FRAC16(0.9)
/* SPEEDLOOP*/
#define M1_SPEED_RAMP_UP FRAC32(0.000681818)
#define M1_SPEED_RAMP_DOWN FRAC32(0.000681818)
#define M1_SPEED_LOOP_HIGH_LIMIT FRAC16(0.375)
#define M1_SPEED_LOOP_LOW_LIMIT FRAC16(-0.375)
#define M1_SPEED_PI_PROP_GAIN ACC32(0.345575)
#define M1_SPEED_PI_INTEG_GAIN ACC32(0.00103673)
#define M1_SPEED_IIR_B0 FRAC32(0.119529)
#define M1_SPEED_IIR_B1 FRAC32(0.119529)
#define M1_SPEED_IIR_A1 FRAC32(0.260943)
/* SENSORLESS*/
#define M1_OL_START_RAMP_INC FRAC32(0.0000227273)
#define M1_OL_START_I FRAC16(0.075)
#define M1_MERG_SPEED_TRH FRAC16(0.113636)
#define M1_MERG_COEFF FRAC16(0.0025)
#define M1_I_SCALE ACC32(0.879913)
#define M1_U_SCALE ACC32(0.571993)
#define M1_E_SCALE ACC32(0.412118)
#define M1_WI_SCALE ACC32(0.103019)
#define M1_BEMF_DQ_KP_GAIN ACC32(0.513523)
#define M1_BEMF_DQ_KI_GAIN ACC32(0.0758613)
#define M1_TO_KP_SHIFT (-1)
#define M1_TO_KP_GAIN FRAC16(0.681818)
#define M1_TO_KI_SHIFT (-8)
#define M1_TO_KI_GAIN FRAC16(0.685438)
#define M1_TO_THETA_SHIFT (-5)
#define M1_TO_THETA_GAIN FRAC16(0.938667)
#define M1_TO_SPEED_IIR_B0 FRAC32(0.0558176)
#define M1_TO_SPEED_IIR_B1 FRAC32(0.0558176)
#define M1_TO_SPEED_IIR_A1 FRAC32(0.388365)
/* USER INPUT START */
#define AA 1
#define BB 3
#define CC 4
#define DD 2
/* USER INPUT END */
#endif /* __M1_PMSM_APPCONFIG_H */
