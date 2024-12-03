/*
    * Copyright 2024 NXP
    *
    * SPDX-License-Identifier: BSD-3-Clause
*/

/*
    * FILE NAME: ../boards/mc56f81000evk/mc_pmsm/pmsm_enc/m2_pmsm_appconfig.h
    * DATE: Wed Feb 14 2024, 11:43:44
*/

/*
{
    "parameters": {
        "parametersNmax": 4400,
        "parametersImax": 8.25,
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
        "parametersUdcbMax": 60.8,
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
    "sensors": {
        "sensorEncPulseNumber": 1000,
        "sensorEncDir": 1,
        "sensorEncNmin": 0,
        "sensorObsrvParSampleTime": 0.0001,
        "sensorObsrvParF0": 100,
        "sensorObsrvParKsi": 1,
        "positionLoopPLKp": 0.05
    },
    "sensorless": {
        "sensorlessBemfObsrvF0": 300,
        "sensorlessBemfObsrvKsi": 1,
        "sensorlessTrackObsrvF0": 20,
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
    "motorName": "Teknic M-2310P",
    "motorDescription": "Configuration for the Teknic motor (fixed-point implementation)."
}
*/

#ifndef __M1_PMSM_APPCONFIG_H 
#define __M1_PMSM_APPCONFIG_H 

/* PARAMETERS*/
#define M1_N_MAX (4400.0F)
#define M1_FREQ_MAX (293.333F)
#define M1_I_MAX (8.25F)
#define M1_U_MAX (35.1029F)
#define M1_E_MAX (15.1F)
#define M1_MOTOR_PP (4)
#define M1_I_PH_NOM FRAC16(0.242424)
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
#define M1_ALIGN_VOLTAGE FRAC16(0.0284877)
#define M1_ALIGN_DURATION (4000)
#define M1_UDCB_IIR_B0 FRAC32(0.0152295)
#define M1_UDCB_IIR_B1 FRAC32(0.0152295)
#define M1_UDCB_IIR_A1 FRAC32(0.469541)
#define M1_SCALAR_VHZ_FACTOR_GAIN FRAC16(0.470046)
#define M1_SCALAR_VHZ_FACTOR_SHIFT (0)
#define M1_SCALAR_INTEG_GAIN ACC32(0.0586667)
#define M1_SCALAR_RAMP_UP FRAC32(0.000227273)
#define M1_SCALAR_RAMP_DOWN FRAC32(0.000227273)
/* CURRENTLOOP*/
#define M1_D_KP_GAIN ACC32(0.108071)
#define M1_D_KI_GAIN ACC32(0.0125442)
#define M1_Q_KP_GAIN ACC32(0.0808526)
#define M1_Q_KI_GAIN ACC32(0.0113129)
#define M1_CLOOP_LIMIT FRAC16(0.9)
/* SPEEDLOOP*/
#define M1_SPEED_RAMP_UP FRAC32(0.00227273)
#define M1_SPEED_RAMP_DOWN FRAC32(0.00227273)
#define M1_SPEED_LOOP_HIGH_LIMIT FRAC16(0.363636)
#define M1_SPEED_LOOP_LOW_LIMIT FRAC16(-0.363636)
#define M1_SPEED_PI_PROP_GAIN ACC32(0.670206)
#define M1_SPEED_PI_INTEG_GAIN ACC32(0.00201062)
#define M1_SPEED_IIR_B0 FRAC32(0.119529)
#define M1_SPEED_IIR_B1 FRAC32(0.119529)
#define M1_SPEED_IIR_A1 FRAC32(0.260943)
/* SENSORS*/
#define M1_POSPE_ENC_PULSES (1000)
#define M1_POSPE_ENC_DIRECTION (1)
#define M1_POSPE_ENC_N_MIN FRAC32(0.0)
#define M1_POSPE_MECH_POS_GAIN ACC32(16.384)
#define M1_POS_P_PROP_GAIN FRAC16(0.05)
#define M1_POSPE_TO_KP_SHIFT (2)
#define M1_POSPE_TO_KP_GAIN FRAC16(0.681818)
#define M1_POSPE_TO_KI_SHIFT (-3)
#define M1_POSPE_TO_KI_GAIN FRAC16(0.685438)
#define M1_POSPE_TO_THETA_SHIFT (-6)
#define M1_POSPE_TO_THETA_GAIN FRAC16(0.938667)
/* SENSORLESS*/
#define M1_OL_START_RAMP_INC FRAC32(0.0000681818)
#define M1_OL_START_I FRAC16(0.0727273)
#define M1_MERG_SPEED_TRH FRAC16(0.113636)
#define M1_MERG_COEFF FRAC16(0.005)
#define M1_I_SCALE ACC32(0.819095)
#define M1_U_SCALE ACC32(1.06907)
#define M1_E_SCALE ACC32(0.459875)
#define M1_WI_SCALE ACC32(0.136146)
#define M1_BEMF_DQ_KP_GAIN ACC32(0.278091)
#define M1_BEMF_DQ_KI_GAIN ACC32(0.0632844)
#define M1_TO_KP_SHIFT (-2)
#define M1_TO_KP_GAIN FRAC16(0.545455)
#define M1_TO_KI_SHIFT (-10)
#define M1_TO_KI_GAIN FRAC16(0.877361)
#define M1_TO_THETA_SHIFT (-4)
#define M1_TO_THETA_GAIN FRAC16(0.938667)
#define M1_TO_SPEED_IIR_B0 FRAC32(0.0558176)
#define M1_TO_SPEED_IIR_B1 FRAC32(0.0558176)
#define M1_TO_SPEED_IIR_A1 FRAC32(0.388365)
/* USER INPUT START */
/* USER INPUT END */
#endif /* __M1_PMSM_APPCONFIG_H */
