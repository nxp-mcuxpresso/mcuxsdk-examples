# isi_ci_pi_yuv422

## Overview
This example shows how to use ISI to receive camera frame through CI_PI.

In this example, the camera input frame is YUV422 format and ISI output frame
is RGB656 format. The output frame is shown in the screen.

NOTE:
This example must run continously to get the right result, breaking during debug
will results to display tearing. Because the ISI driver maintains IP state through
ISR, if the core is halted, then driver state does not match the IP state. When
exiting debug, the core might be halted for a short while, so the tearing may
happen when exiting debug.

If the camera and LCD panel resolutions are not the same, then only part of the
camera frame will be shown.
