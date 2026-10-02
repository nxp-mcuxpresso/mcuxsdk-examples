/*
	* Copyright 2024 NXP
	*
	* SPDX-License-Identifier: BSD-3-Clause
*/

/*
    * FILE NAME: ../../../examples/_boards/frdmmcxc162/demo_apps/mc_pmsm/pmsm_snsless/m1_pmsm_appconfig.h
    * DATE: Fri Oct 02 2026, 09:25:02
*/

/*
{
    "mid": {
        "midInParamINom": 4,
        "midInParamNNom": 3000,
        "midPolePairIAPp": 4,
        "midElParamMeasRs": 0,
        "midElParamMeasLd": 0,
        "midElParamMeasLq": 0,
        "midElParamMeasUdt": 0
    },
    "parameters": {
        "parametersNmax": 4400,
        "parametersImax": 8.25,
        "parametersEmax": 15.1,
        "parametersPp": 4,
        "parametersRs": 0.54,
        "parametersLd": 0.0003356,
        "parametersLq": 0.00021799999999999999,
        "parametersKt": 0.01217,
        "parametersJ": 0.0000016000000000000001,
        "parametersIphNom": 8,
        "parametersUphNom": 15,
        "parametersNnom": 4000,
        "parametersUdcbMax": 60.8,
        "parametersUdcbTrip": 29,
        "parametersUdcbUnder": 16,
        "parametersUdcbOver": 32,
        "parametersNover": 4399,
        "parametersNmin": 300,
        "parametersEblock": 0.7,
        "parametersEblockPer": 2000,
        "parametersUdcbIIRf0": 100,
        "parametersCalibDuration": 0.2,
        "parametersFaultDuration": 6,
        "parametersFreewheelDuration": 1.5,
        "parametersScalarUqMin": 1,
        "parametersAlignVoltage": 1.5,
        "parametersAlignDuration": 1.2,
        "parametersScalarVHzRatio": 100
    },
    "currentLoop": {
        "currentLoopSampleTime": 0.0001,
        "currentLoopF0": 600,
        "currentLoopKsi": 1,
        "currentLoopOutputLimit": 90
    },
    "speedLoop": {
        "speedLoopSampleTime": 0.001,
        "speedLoopF0": 10,
        "speedLoopKsi": 1,
        "speedLoopIncUp": 5000,
        "speedLoopIncDown": 5000,
        "speedLoopCutOffFreq": 400,
        "speedLoopUpperLimit": 4,
        "speedLoopLowerLimit": -4,
        "speedLoopSLKp": 0.0005,
        "speedLoopSLKi": 0.008,
        "speedLoopManualConstantTunning": false
    },
    "positionLoop": {
        "positionLoopSampleTime": 0.001,
        "positionLoopF0": 5,
        "positionLoopKsi": 1,
        "servo_positionLoopUpperLimit": 3000,
        "servo_positionLoopLowerLimit": -3000,
        "servo_speedLoopUpperLimit": 4,
        "servo_speedLoopLowerLimit": -4
    },
    "sensorless": {
        "sensorlessBemfObsrvF0": 300,
        "sensorlessBemfObsrvKsi": 1,
        "sensorlessTrackObsrvF0": 70,
        "sensorlessTrackObsrvKsi": 1,
        "sensorlessTrackObsrvIIRSpeedCutOff": 400,
        "sensorlessStartupRamp": 3000,
        "sensorlessStartupCurrent": 0.65,
        "sensorlessMergingSpeed": 300,
        "sensorlessMergingCoeff": 100
    }
}
*/

/*
{
    "motorName": "Teknic M-2311P-LN-08D",
    "motorDescription": "Configuration for the Teknic motor (fixed-point implementation)."
}
*/

#ifndef __M1_PMSM_APPCONFIG_H 
#define __M1_PMSM_APPCONFIG_H 

/* MID*/
/* PARAMETERS*/
#define M1_N_MAX (1843.07F)
#define M1_FREQ_MAX (293.333F)
#define M1_N_ANGULAR_MAX (4400.0F)
#define M1_I_MAX (8.25F)
#define M1_U_MAX (35.1029F)
#define M1_E_MAX (15.1F)
#define M1_MOTOR_PP (4)
#define M1_I_PH_NOM FRAC16(0.969697)
#define M1_N_NOM FRAC16(0.909091)
#define M1_U_DCB_MAX (60.8F)
#define M1_U_DCB_TRIP FRAC16(0.476974)
#define M1_U_DCB_UNDERVOLTAGE FRAC16(0.263158)
#define M1_U_DCB_OVERVOLTAGE FRAC16(0.526316)
#define M1_N_OVERSPEED FRAC16(0.999773)
#define M1_N_MIN FRAC16(0.0681818)
#define M1_E_BLOCK_TRH FRAC16(0.0463576)
#define M1_E_BLOCK_PER (2000)
#define M1_CALIB_DURATION (200)
#define M1_FAULT_DURATION (6000)
#define M1_FREEWHEEL_DURATION (1500)
#define M1_SCALAR_UQ_MIN FRAC16(0.0284877)
#define M1_ALIGN_VOLTAGE FRAC16(0.0427315)
#define M1_ALIGN_DURATION (12000)
#define M1_UDCB_IIR_B0 FRAC32(0.0152295)
#define M1_UDCB_IIR_B1 FRAC32(0.0152295)
#define M1_UDCB_IIR_A1 FRAC32(0.469541)
#define M1_SCALAR_VHZ_FACTOR_GAIN FRAC16(0.470046)
#define M1_SCALAR_VHZ_FACTOR_SHIFT (0)
#define M1_SCALAR_INTEG_GAIN ACC32(0.0586667)
#define M1_SCALAR_RAMP_UP FRAC32(0.000113636)
#define M1_SCALAR_RAMP_DOWN FRAC32(0.000113636)
/* CURRENTLOOP*/
#define M1_D_KP_GAIN ACC32(0.467782)
#define M1_D_KI_GAIN ACC32(0.112097)
#define M1_Q_KP_GAIN ACC32(0.259391)
#define M1_Q_KI_GAIN ACC32(0.0728165)
#define M1_Q_IIR_ZC_B0 FRAC32(0.109595)
#define M1_Q_IIR_ZC_B1 FRAC32(0.109595)
#define M1_Q_IIR_ZC_A1 FRAC32(0.280810)
#define M1_CLOOP_LIMIT FRAC16(0.9)
/* SPEEDLOOP*/
#define M1_SPEED_RAMP_UP FRAC32(0.00113636)
#define M1_SPEED_RAMP_DOWN FRAC32(0.00113636)
#define M1_SPEED_LOOP_HIGH_LIMIT FRAC16(0.484848)
#define M1_SPEED_LOOP_LOW_LIMIT FRAC16(-0.484848)
#define M1_SPEED_PI_PROP_GAIN ACC32(0.922713)
#define M1_SPEED_PI_INTEG_GAIN ACC32(0.0289879)
#define M1_SPEED_IIR_B0 FRAC32(0.278431)
#define M1_SPEED_IIR_B1 FRAC32(0.278431)
#define M1_SPEED_IIR_A1 FRAC32(-0.0568627)
#define M1_SPEED_IIR_ZC_B0 FRAC32(0.0152295)
#define M1_SPEED_IIR_ZC_B1 FRAC32(0.0152295)
#define M1_SPEED_IIR_ZC_A1 FRAC32(0.469541)
/* POSITIONLOOP*/
#define M1_SERVO_POSITION_P_HIGH_LIMIT FRAC32(0.681818)
#define M1_SERVO_POSITION_P_LOW_LIMIT FRAC32(-0.681818)
#define M1_SERVO_SPEED_PI_HIGH_LIMIT FRAC16(0.484848)
#define M1_SERVO_SPEED_PI_LOW_LIMIT FRAC16(-0.484848)
#define M1_SERVO_POSITION_P_PROP_GAIN ACC32(9358.52)
#define M1_SERVO_FEED_FRWD_K1 ACC32(595.782)
#define M1_SERVO_FEED_FRWD_K2 ACC32(9.48216)
#define M1_SERVO_IIR_ZC_B0 FRAC32(0.00773252)
#define M1_SERVO_IIR_ZC_B1 FRAC32(0.00773252)
#define M1_SERVO_IIR_ZC_A1 FRAC32(0.484535)
#define M1_SERVO_SPEED_PI_PROP_GAIN ACC32(0.692035)
#define M1_SERVO_SPEED_PI_INTEG_GAIN ACC32(0.0217409)
/* SENSORLESS*/
#define M1_OL_START_RAMP_INC FRAC32(0.0000681818)
#define M1_OL_START_I FRAC16(0.0787879)
#define M1_MERG_SPEED_TRH FRAC16(0.0681818)
#define M1_MERG_COEFF FRAC16(0.002)
#define M1_I_SCALE ACC32(0.861396)
#define M1_U_SCALE ACC32(1.09212)
#define M1_E_SCALE ACC32(0.469790)
#define M1_WI_SCALE ACC32(0.103129)
#define M1_BEMF_DQ_KP_GAIN ACC32(0.396209)
#define M1_BEMF_DQ_KI_GAIN ACC32(0.0651480)
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
#define AA 1
#define BB 3
#define CC 4
#define DD 2
/* USER INPUT END */
#endif /* __M1_PMSM_APPCONFIG_H */
