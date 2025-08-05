/*
    * Copyright 2025 NXP 
    *
    * SPDX-License-Identifier: BSD-3-Clause 
*/

/*
    * FILE NAME: ../../../examples/_boards/imx943evk/demo_apps/mc_pmsm/pmsm_servo_dual/cm7_core1/m1_pmsm_appconfig.h
    * DATE: Tue Feb 11 2025, 10:30:16
*/

/*
{
    "parameters": {
        "parametersPP": 3,
        "parametersRs": 0.1105,
        "parametersLd": 0.000116467,
        "parametersLq": 0.000134492,
        "parametersKe": 0.058,
        "parametersJ": 0.00005,
        "parametersIphNom": 11.1,
        "parametersUphNom": 48,
        "parametersNnom": 6000,
        "parametersImax": 34.2,
        "parametersUdcbMax": 77.35,
        "parametersUdcbTrip": 28,
        "parametersUdcbUnder": 12,
        "parametersUdcbOver": 34,
        "parametersNover": 5999,
        "parametersNmin": 300,
        "parametersEblock": 0.2,
        "parametersEblockPer": 2000,
        "parametersNmax": 4400,
        "parametersUdcbIIRf0": 100,
        "parametersCalibDuration": 0.2,
        "parametersFaultDuration": 6,
        "parametersFreewheelDuration": 1.5,
        "parametersScalarUqMin": 1,
        "parametersAlignVoltage": 1.2,
        "parametersAlignDuration": 1,
        "parametersScalarVHzRatio": 200
    },
    "currentLoop": {
        "currentLoopSampleTime": 0.00003125,
        "currentLoopF0": 400,
        "currentLoopKsi": 1,
        "currentLoopOutputLimit": 90
    },
    "speedLoop": {
        "speedLoopSampleTime": 0.00025,
        "speedLoopF0": 10,
        "speedLoopKsi": 1,
        "speedLoopIncUp": 4000,
        "speedLoopIncDown": 5000,
        "speedLoopCutOffFreq": 100,
        "speedLoopUpperLimit": 4,
        "speedLoopLowerLimit": -4,
        "speedLoopSLKp": 0.009202212,
        "speedLoopSLKi": 0.00008095,
        "speedLoopManualConstantTunning": false
    },
    "positionLoop": {
        "positionLoopSampleTime": 0.00025,
        "positionLoopF0": 30,
        "positionLoopKsi": 1,
        "servo_positionLoopUpperLimit": 4000,
        "servo_positionLoopLowerLimit": -4000,
        "servo_speedLoopUpperLimit": 8,
        "servo_speedLoopLowerLimit": -8
    },
    "sensorless": {
        "sensorlessBemfObsrvF0": 150,
        "sensorlessBemfObsrvKsi": 1,
        "sensorlessTrackObsrvF0": 35,
        "sensorlessTrackObsrvKsi": 1,
        "sensorlessTrackObsrvIIRSpeedCutOff": 400,
        "sensorlessStartupRamp": 3000,
        "sensorlessStartupCurrent": 0.6,
        "sensorlessMergingSpeed": 500,
        "sensorlessMergingCoeff": 100
    }
}
*/

/*
{
    "motorName": "TGT3-0065-60-48",
    "motorDescription": "Datasheet:\nKe = 3.49V/1000rpm\nKt = 0.058 Nm/A\nR2ph = 0.221Ohm\nL2ph = 0.25 mH"
}
*/

#ifndef __M2_PMSM_APPCONFIG_H 
#define __M2_PMSM_APPCONFIG_H 

/* PARAMETERS*/
#define M2_MOTOR_PP (3)
#define M2_I_PH_NOM (11.1F)
#define M2_N_NOM (1884.96F)
#define M2_I_MAX (34.2F)
#define M2_U_DCB_MAX (77.35F)
#define M2_U_DCB_TRIP (28.0F)
#define M2_U_DCB_UNDERVOLTAGE (12.0F)
#define M2_U_DCB_OVERVOLTAGE (32.0F)
#define M2_N_OVERSPEED (1884.64F)
#define M2_N_MIN (94.2478F)
#define M2_E_BLOCK_TRH (0.2F)
#define M2_E_BLOCK_PER (2000)
#define M2_N_MAX (1382.30F)
#define M2_CALIB_DURATION (800)
#define M2_FAULT_DURATION (24000)
#define M2_FREEWHEEL_DURATION (6000)
#define M2_SCALAR_UQ_MIN (1.0F)
#define M2_ALIGN_VOLTAGE (1.2F)
#define M2_ALIGN_DURATION (32000)
#define M2_U_MAX (44.6581F)
#define M2_FREQ_MAX (220.0F)
#define M2_N_ANGULAR_MAX (3.18310F)
#define M2_UDCB_IIR_B0 (0.00972203F)
#define M2_UDCB_IIR_B1 (0.00972203F)
#define M2_UDCB_IIR_A1 (0.980556F)
#define M2_SCALAR_VHZ_FACTOR_GAIN (0.32F)
#define M2_SCALAR_INTEG_GAIN ACC32(0.0137500)
#define M2_SCALAR_RAMP_UP (0.00625F)
#define M2_SCALAR_RAMP_DOWN (0.00781250F)
/* CURRENTLOOP*/
#define M2_D_KP_GAIN (0.474927F)
#define M2_D_KI_GAIN (0.0114948F)
#define M2_Q_KP_GAIN (0.565531F)
#define M2_Q_KI_GAIN (0.0132738F)
#define M2_Q_IIR_ZC_B0 (0.0229332F)
#define M2_Q_IIR_ZC_B1 (0.0229332F)
#define M2_Q_IIR_ZC_A1 (0.954134F)
#define M2_CLOOP_LIMIT (0.519615F)
/* SPEEDLOOP*/
#define M2_SPEED_RAMP_UP (0.314159F)
#define M2_SPEED_RAMP_DOWN (0.392699F)
#define M2_SPEED_LOOP_HIGH_LIMIT (4.0F)
#define M2_SPEED_LOOP_LOW_LIMIT (-4.0F)
#define M2_SPEED_PI_PROP_GAIN (0.0361103F)
#define M2_SPEED_PI_INTEG_GAIN (0.000141805F)
#define M2_SPEED_IIR_B0 (0.0728205F)
#define M2_SPEED_IIR_B1 (0.0728205F)
#define M2_SPEED_IIR_A1 (0.854359F)
#define M2_SPEED_IIR_ZC_B0 (0.00391163F)
#define M2_SPEED_IIR_ZC_B1 (0.00391163F)
#define M2_SPEED_IIR_ZC_A1 (0.992177F)
/* POSITIONLOOP*/
#define M2_SERVO_POSITION_P_HIGH_LIMIT (1256.64F)
#define M2_SERVO_POSITION_P_LOW_LIMIT (-1256.64F)
#define M2_SERVO_POSITION_P_PROP_GAIN (1184.35F)
#define M2_SERVO_FEED_FRWD_K1 (12.5664F)
#define M2_SERVO_FEED_FRWD_K2 (0.0333333F)
#define M2_SERVO_IIR_ZC_B0 (0.0230196F)
#define M2_SERVO_IIR_ZC_B1 (0.0230196F)
#define M2_SERVO_IIR_ZC_A1 (0.953961F)
#define M2_SERVO_SPEED_PI_PROP_GAIN (0.162496F)
#define M2_SERVO_SPEED_PI_INTEG_GAIN (0.00382873F)
#define M2_SERVO_SPEED_PI_HIGH_LIMIT (8.0F)
#define M2_SERVO_SPEED_PI_LOW_LIMIT (-8.0F)
/* SENSORLESS*/
#define M2_OL_START_RAMP_INC (0.0294524F)
#define M2_OL_START_I (0.6F)
#define M2_MERG_SPEED_TRH (157.080F)
#define M2_MERG_COEFF FRAC16(0.000793457)
#define M2_I_SCALE (0.971205F)
#define M2_U_SCALE (0.260590F)
#define M2_E_SCALE (0.260590F)
#define M2_WI_SCALE (0.0000350473F)
#define M2_BEMF_DQ_KP_GAIN (0.109035F)
#define M2_BEMF_DQ_KI_GAIN (0.00323292F)
#define M2_TO_KP_GAIN (439.823F)
#define M2_TO_KI_GAIN (1.51128F)
#define M2_TO_THETA_GAIN (0.00000994718F)
#define M2_TO_SPEED_IIR_B0 (0.0377861F)
#define M2_TO_SPEED_IIR_B1 (0.0377861F)
#define M2_TO_SPEED_IIR_A1 (0.924428F)
/* USER INPUT START */
/* USER INPUT END */
#endif /* __M2_PMSM_APPCONFIG_H */
