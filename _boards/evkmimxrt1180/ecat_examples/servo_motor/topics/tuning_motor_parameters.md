# Tuning motor parameters

Tuning motor parameters, constants and controller parameters can be necessary to achieve the required behavior and responses of a servo control application. There can be another motor type and different conditions than described in this document, so tuning parameters can be important. For the tuning motor parameters, perform the following steps:

1.  Download the latest SDK package with the motor control software for permanent magnet synchronous motor \(PMSM\) from [https://mcuxpresso.nxp.com/en/builder?hw=MIMXRT1180-EVK&components=nxp\_motor\_control](https://mcuxpresso.nxp.com/en/builder?hw=MIMXRT1180-EVK&components=nxp_motor_control).
2.  Use the motor control example \(*mc\_pmsm*\) for tuning motor parameters, constants, and so on. For more information about the application tuning, refer to the motor control application tuning \(MCAT\) training [https://www.nxp.com/design/training/motor-control-application-tuning-mcat-tool-pmsm-servo-tuning:TIP-MCAT-PMSM](https://www.nxp.com/design/training/motor-control-application-tuning-mcat-tool-pmsm-servo-tuning:TIP-MCAT-PMSM).
3.  When the parameters are tuned, use the updated m1\_pmsm\_appconfig.h header file with tuned parameters in the servo motor example.

    **Note:** The application timings \(slow state machine, fast state machine\) must be the same for the motor control and servo motor examples.


