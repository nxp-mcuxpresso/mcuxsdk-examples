/*
    * Copyright 2024 NXP
    *
    * SPDX-License-Identifier: BSD-3-Clause
*/

/*
    * FILE NAME: ../../../boards/twrmc56f8200/demo_apps/mc_pmsm/pmsm_snsless/m2_pmsm_appconfig.h
    * DATE: Fri May 10 2024, 12:00:57
*/

/*
{
    "parameters": {
        "parametersNmax": 4400,
        "parametersImax": 8,
        "parametersEmax": 15.1,
        "parametersPP": 4,
        "parametersRs": 0.72,
        "parametersLd": 0.000326,
        "parametersLq": 0.000294,
        "parametersKe": 0.0393,
        "parametersJ": 0.000017,
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
        "speedLoopUpperLimit": 2,
        "speedLoopLowerLimit": -2,
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
    "motorName": "Teknic M-2310P",
    "motorDescription": "Configuration for the Teknic motor."
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
#define M1_I_PH_NOM FRAC16(0.25)
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
#define M1_SCALAR_UQ_MIN FRAC16(0.0477149)
#define M1_ALIGN_VOLTAGE FRAC16(0.0477149)
#define M1_ALIGN_DURATION (4000)
#define M1_UDCB_IIR_B0 FRAC32(0.0152295)
#define M1_UDCB_IIR_B1 FRAC32(0.0152295)
#define M1_UDCB_IIR_A1 FRAC32(0.469541)
#define M1_SCALAR_VHZ_FACTOR_GAIN FRAC16(0.787295)
#define M1_SCALAR_VHZ_FACTOR_SHIFT (0)
#define M1_SCALAR_INTEG_GAIN ACC32(0.0586667)
#define M1_SCALAR_RAMP_UP FRAC32(0.0000681818)
#define M1_SCALAR_RAMP_DOWN FRAC32(0.0000681818)
/* CURRENTLOOP*/
#define M1_D_KP_GAIN ACC32(0.163016)
#define M1_D_KI_GAIN ACC32(0.0192578)
#define M1_Q_KP_GAIN ACC32(0.120037)
#define M1_Q_KI_GAIN ACC32(0.0173675)
#define M1_CLOOP_LIMIT FRAC16(0.9)
/* SPEEDLOOP*/
#define M1_SPEED_RAMP_UP FRAC32(0.000681818)
#define M1_SPEED_RAMP_DOWN FRAC32(0.000681818)
#define M1_SPEED_LOOP_HIGH_LIMIT FRAC16(0.25)
#define M1_SPEED_LOOP_LOW_LIMIT FRAC16(-0.25)
#define M1_SPEED_PI_PROP_GAIN ACC32(0.691150)
#define M1_SPEED_PI_INTEG_GAIN ACC32(0.00207345)
#define M1_SPEED_IIR_B0 FRAC32(0.119529)
#define M1_SPEED_IIR_B1 FRAC32(0.119529)
#define M1_SPEED_IIR_A1 FRAC32(0.260943)
/* SENSORLESS*/
#define M1_OL_START_RAMP_INC FRAC32(0.0000227273)
#define M1_OL_START_I FRAC16(0.075)
#define M1_MERG_SPEED_TRH FRAC16(0.113636)
#define M1_MERG_COEFF FRAC16(0.005)
#define M1_I_SCALE ACC32(0.819095)
#define M1_U_SCALE ACC32(0.658223)
#define M1_E_SCALE ACC32(0.474246)
#define M1_WI_SCALE ACC32(0.136146)
#define M1_BEMF_DQ_KP_GAIN ACC32(0.269664)
#define M1_BEMF_DQ_KI_GAIN ACC32(0.0613667)
#define M1_TO_KP_SHIFT (-2)
#define M1_TO_KP_GAIN FRAC16(0.681818)
#define M1_TO_KI_SHIFT (-9)
#define M1_TO_KI_GAIN FRAC16(0.685438)
#define M1_TO_THETA_SHIFT (-4)
#define M1_TO_THETA_GAIN FRAC16(0.938667)
#define M1_TO_SPEED_IIR_B0 FRAC32(0.0558176)
#define M1_TO_SPEED_IIR_B1 FRAC32(0.0558176)
#define M1_TO_SPEED_IIR_A1 FRAC32(0.388365)
/* USER INPUT START */
/* USER INPUT END */
#endif /* __M1_PMSM_APPCONFIG_H */
