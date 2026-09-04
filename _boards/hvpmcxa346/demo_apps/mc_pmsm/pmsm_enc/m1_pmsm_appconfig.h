/*
	* Copyright 2025 NXP
	*
	* SPDX-License-Identifier: BSD-3-Clause
*/

/*
    * FILE NAME: ../../../examples/_boards/hvpmcxa346/demo_apps/mc_pmsm/pmsm_enc/m1_pmsm_appconfig.h
    * DATE: Fri Sep 04 2026, 13:00:46
*/

/*
{
    "mid": {
        "midInParamINom": 2,
        "midInParamNNom": 3000,
        "midPolePairIAPp": 4,
        "midElParamMeasRs": 0,
        "midElParamMeasLd": 0,
        "midElParamMeasLq": 0,
        "midElParamMeasUdt": 0,
        "midMechParamMeasKe": 0,
        "midMechParamMeasKt": 0,
        "midMechParamMeasJ": 0,
        "midMechParamMeasB": 0,
        "midMechParamMeasA": 0
    },
    "parameters": {
        "parametersPp": 4,
        "parametersRs": 2.25,
        "parametersLd": 0.00601,
        "parametersLq": 0.00659,
        "parametersKt": 0.344,
        "parametersJ": 0.0000253,
        "parametersIphNom": 2,
        "parametersUphNom": 200,
        "parametersNnom": 4000,
        "parametersImax": 8,
        "parametersUdcbMax": 433,
        "parametersUdcbTrip": 346.3,
        "parametersUdcbUnder": 173.2,
        "parametersUdcbOver": 346.4,
        "parametersNover": 4180,
        "parametersNmin": 400,
        "parametersEblock": 7,
        "parametersEblockPer": 2000,
        "parametersNmax": 4400,
        "parametersUdcbIIRf0": 100,
        "parametersCalibDuration": 0.2,
        "parametersFaultDuration": 6,
        "parametersFreewheelDuration": 1.5,
        "parametersScalarUqMin": 4,
        "parametersAlignVoltage": 6,
        "parametersAlignDuration": 0.8,
        "parametersScalarVHzRatio": 100
    },
    "currentLoop": {
        "currentLoopSampleTime": 0.0000625,
        "currentLoopF0": 280,
        "currentLoopKsi": 1,
        "currentLoopOutputLimit": 90
    },
    "speedLoop": {
        "speedLoopSampleTime": 0.001,
        "speedLoopF0": 28,
        "speedLoopKsi": 1,
        "speedLoopIncUp": 4000,
        "speedLoopIncDown": 4000,
        "speedLoopCutOffFreq": 100,
        "speedLoopUpperLimit": 2,
        "speedLoopLowerLimit": -2,
        "speedLoopSLKp": 0.00035,
        "speedLoopSLKi": 0.000001,
        "speedLoopManualConstantTunning": false
    },
    "positionLoop": {
        "positionLoopSampleTime": 0.001,
        "positionLoopF0": 5,
        "positionLoopKsi": 1,
        "servo_positionLoopUpperLimit": 2000,
        "servo_positionLoopLowerLimit": -2000,
        "servo_speedLoopUpperLimit": 2,
        "servo_speedLoopLowerLimit": -2
    },
    "sensorless": {
        "sensorlessBemfObsrvF0": 280,
        "sensorlessBemfObsrvKsi": 1,
        "sensorlessTrackObsrvF0": 25,
        "sensorlessTrackObsrvKsi": 1,
        "sensorlessTrackObsrvIIRSpeedCutOff": 400,
        "sensorlessStartupRamp": 1500,
        "sensorlessStartupCurrent": 0.2,
        "sensorlessMergingSpeed": 500,
        "sensorlessMergingCoeff": 100
    }
}
*/

/*
{
    "motorName": "MIGE 60CST-M01330 HVP",
    "motorDescription": "Curent loop sample frequency 16KHz"
}
*/

#ifndef __M1_PMSM_APPCONFIG_H 
#define __M1_PMSM_APPCONFIG_H 

/* MID*/
/* PARAMETERS*/
#define M1_MOTOR_PP (4)
#define M1_I_PH_NOM (2.0F)
#define M1_N_NOM (1675.52F)
#define M1_I_MAX (8.0F)
#define M1_U_DCB_MAX (433.0F)
#define M1_U_DCB_TRIP (346.3F)
#define M1_U_DCB_UNDERVOLTAGE (173.2F)
#define M1_U_DCB_OVERVOLTAGE (346.4F)
#define M1_N_OVERSPEED (1750.91F)
#define M1_N_MIN (167.552F)
#define M1_E_BLOCK_TRH (7.0F)
#define M1_E_BLOCK_PER (2000)
#define M1_N_MAX (1843.07F)
#define M1_CALIB_DURATION (200)
#define M1_FAULT_DURATION (6000)
#define M1_FREEWHEEL_DURATION (1500)
#define M1_SCALAR_UQ_MIN (4.0F)
#define M1_ALIGN_VOLTAGE (6.0F)
#define M1_ALIGN_DURATION (12800)
#define M1_U_MAX (249.993F)
#define M1_FREQ_MAX (293.333F)
#define M1_N_ANGULAR_MAX (2.38732F)
#define M1_UDCB_IIR_B0 (0.0192568F)
#define M1_UDCB_IIR_B1 (0.0192568F)
#define M1_UDCB_IIR_A1 (0.961486F)
#define M1_SCALAR_VHZ_FACTOR_GAIN (0.75F)
#define M1_SCALAR_INTEG_GAIN ACC32(0.0366667)
#define M1_SCALAR_RAMP_UP (0.0166667F)
#define M1_SCALAR_RAMP_DOWN (0.0166667F)
/* CURRENTLOOP*/
#define M1_D_KP_GAIN (18.9259F)
#define M1_D_KI_GAIN (0.582252F)
#define M1_Q_KP_GAIN (20.9307F)
#define M1_Q_KI_GAIN (0.637363F)
#define M1_Q_IIR_ZC_B0 (0.0295512F)
#define M1_Q_IIR_ZC_B1 (0.0295512F)
#define M1_Q_IIR_ZC_A1 (0.940898F)
#define M1_CLOOP_LIMIT (0.519615F)
/* SPEEDLOOP*/
#define M1_SPEED_RAMP_UP (1.67552F)
#define M1_SPEED_RAMP_DOWN (1.67552F)
#define M1_SPEED_LOOP_HIGH_LIMIT (2.0F)
#define M1_SPEED_LOOP_LOW_LIMIT (-2.0F)
#define M1_SPEED_PI_PROP_GAIN (0.00647341F)
#define M1_SPEED_PI_INTEG_GAIN (0.000284716F)
#define M1_SPEED_IIR_B0 (0.239057F)
#define M1_SPEED_IIR_B1 (0.239057F)
#define M1_SPEED_IIR_A1 (0.521886F)
#define M1_SPEED_IIR_ZC_B0 (0.0421294F)
#define M1_SPEED_IIR_ZC_B1 (0.0421294F)
#define M1_SPEED_IIR_ZC_A1 (0.915741F)
/* POSITIONLOOP*/
#define M1_SERVO_POSITION_P_HIGH_LIMIT (837.758F)
#define M1_SERVO_POSITION_P_LOW_LIMIT (-837.758F)
#define M1_SERVO_POSITION_P_PROP_GAIN (263.189F)
#define M1_SERVO_FEED_FRWD_K1 (16.7552F)
#define M1_SERVO_FEED_FRWD_K2 (0.266667F)
#define M1_SERVO_IIR_ZC_B0 (0.0154650F)
#define M1_SERVO_IIR_ZC_B1 (0.0154650F)
#define M1_SERVO_IIR_ZC_A1 (0.969070F)
#define M1_SERVO_SPEED_PI_PROP_GAIN (0.00173395F)
#define M1_SERVO_SPEED_PI_INTEG_GAIN (0.0000272368F)
#define M1_SERVO_SPEED_PI_HIGH_LIMIT (2.0F)
#define M1_SERVO_SPEED_PI_LOW_LIMIT (-2.0F)
/* SENSORLESS*/
#define M1_OL_START_RAMP_INC (0.0392699F)
#define M1_OL_START_I (0.2F)
#define M1_MERG_SPEED_TRH (209.440F)
#define M1_MERG_COEFF FRAC16(0.00207520)
#define M1_I_SCALE (0.977119F)
#define M1_U_SCALE (0.0101448F)
#define M1_E_SCALE (0.0101448F)
#define M1_WI_SCALE (0.0000668503F)
#define M1_BEMF_DQ_KP_GAIN (18.9259F)
#define M1_BEMF_DQ_KI_GAIN (1.16450F)
#define M1_TO_KP_GAIN (314.159F)
#define M1_TO_KI_GAIN (1.54213F)
#define M1_TO_THETA_GAIN (0.0000198944F)
#define M1_TO_SPEED_IIR_B0 (0.0728205F)
#define M1_TO_SPEED_IIR_B1 (0.0728205F)
#define M1_TO_SPEED_IIR_A1 (0.854359F)
/* USER INPUT START */
/* USER INPUT END */
#endif /* __M1_PMSM_APPCONFIG_H */
