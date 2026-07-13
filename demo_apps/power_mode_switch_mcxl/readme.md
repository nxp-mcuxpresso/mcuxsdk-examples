# power_mode_switch_mcxl

## Overview
This example demonstrate features of low power modes. The demo prints the power mode menu
through the debug console, where the user can set the MCU to a specific power mode. The user can also set the wakeup
source by following the debug console prompts. The purpose of this demo is to show how to switch between different power
 modes, and how to configure a wakeup source and wakeup the MCU from low power modes.

Tips:
Once wakeup from DPD2, the interrupt which cause wakeup should always be handled in CM0Plus project, otherwise the system may not work as expected. For example, if LPTMR is used as wakeup source, the interrupt handler should be implemented in CM0Plus project, and the handler should clear the interrupt flag and stop the timer. If EXT_INT is used as wakeup source, the interrupt handler should also be implemented in CM0Plus project, and the handler should clear the external interrupt flag.

## Sequence Diagrams (ADVC Enabled + Context Saving Enabled)

The five diagrams below cover Sequences E-I with `APP_ENABLE_ADVC=1` and
`APP_ENABLE_CONTEXT_SAVING=1`.

`[ADVC]` marks every step guarded by `ADVC_PreVoltageChangeRequest()` /
`ADVC_PostVoltageChangeRequest()`.

Context save: CM33 -> BCKP2; CM0+ -> MSB_BCKP1.
Wakeup: `Reset_Handler` -> `Power_LowPowerBoot()` detects non-zero register,
restores context, BX lr -> resumes right after `Power_PushContext()`.

---

### Sequence E -- Active -> DPD1 -> Active

CM33 enters DPD1 (powered off, main RAM retained). CM0+ stays active,
services the LPTMR wakeup interrupt, then writes the MSB_BCKP1 sync
token to unblock CM33.

```c
/*
 *  CM33                                    |  CM0+
 *  --------------------------------------- | -------------------------------
 *  APP_EnableWakeupSource():               |
 *    dpd1Config.mainWakeupSrc = LPTMR      |
 *    nextTrans = kPower_Dpd1ToActive       |
 *  [ADVC] Pre(aonFreq)                     |
 *  [ADVC]   CLOCK_AttachClk(FRO16K ->      |
 *  [ADVC]              AON_LPTMR)          |
 *  [ADVC] Post()                           |
 *  LPTMR_Init() + Start()  -- 10 s        |
 *  Power_EnterDeepPowerDown1():            |
 *    [ADVC] PrepareVddCoreAndFro10M():     |
 *    [ADVC]   Pre(aonFreq)                 |
 *    [ADVC]   CGU: disable FRO10M          |
 *    [ADVC]   Post()                       |
 *    SMM: mainRamRetain=All, bandgap off   |
 *    CMC: DeepPowerDown, prevMode=DPD1     |
 *    PushContext() -> BCKP2                |
 *    __WFI() =========================>   | (CM33 powered off, DPD1)
 *  - - - - - - - - - - - - - - - - - - - | APP_DPD1ToActive():
 *                                          |   EnableIRQ(LPTMR_AON_IRQn)
 *                                          |   __WFI()
 *                                          |      << LPTMR fires (10 s) >>
 *                                          | LPTMR_AON_IRQHandler():
 *                                          |   ClearLpPowerSettings()
 *                                          |   stop + deinit LPTMR
 *                                          | [ADVC] AON retained -- no re-init
 *  .- MSB_BCKP1 = 0x5A5A <-------------- -| Power_NotifyCM33ToRun()
 *  | Reset_Handler -> Power_LowPowerBoot():
 *  |   BCKP2 != 0 -> restore SP+regs -> BX lr
 *  |   (resumes in Power_EnterDPD1(), PushContext returns 1)
 *  |   while (MSB_BCKP1 != 0x5A5A)  -- spin
 *  |   clear BCKP2 + MSB_BCKP1
 *  |   return kStatus_Power_WakeupFromDPD1
 *  | BOARD_InitHardware(), ClearLpPowerSettings()
 *  | [ADVC] ADVC_IsEnabled() -> true  (AON retained, no re-init)
 *  '-> Resume main() -- Active
 */
```

---

### Sequence F -- Active -> DPD1 -> DPD2 -> DPD1 -> Active

CM33 enters DPD1. CM0+ independently enters DPD2 from DPD1 (no MU
handshake -- CM33 is already off). LPTMR wakes CM0+ back to DPD1;
EXT_INT then triggers the final DPD1 -> Active transition.

```c
/*
 *  CM33                                    |  CM0+
 *  --------------------------------------- | -------------------------------
 *  (DPD1 entry same as Seq E,             |
 *   nextTrans = Dpd1ToDpd2WakeToDpd1)     |
 *  PushContext() -> BCKP2                  |
 *  __WFI() =========================>     | (CM33 powered off, DPD1)
 *  - - - - - - - - - - - - - - - - - - - | APP_DPD1ToDPD2BackToDPD1():
 *                                          |   GETCHAR (user key, AON LPUART)
 *                                          |   SMM_ClearAllLowPowerSequence()
 *                                          | [ADVC] Pre(3 MHz)
 *                                          | [ADVC]   FRO16K -> AON_LPTMR
 *                                          | [ADVC] Post()
 *                                          | Power_EnterDeepPowerDown2()
 *                                          |   -- DPD1->DPD2 path (no MU):
 *                                          | [ADVC] SetDpd2AdvcWorkaround():
 *                                          | [ADVC]   CLOCK_DisableADVCControl()
 *                                          | [ADVC]   Pre(targetFreq)
 *                                          | [ADVC]   FROdiv4 -> AON_CPU
 *                                          | [ADVC]   disable FRO10M/3M, auto-cal
 *                                          | [ADVC]   Post()
 *                                          |   SMM/PMU config, prevMode=DPD2
 *                                          |   PushContext() -> MSB_BCKP1
 *                                          |   SMM_StartAonDPD2Sequence()
 *                                          |   __WFI() == both cores off (DPD2)
 *                                          |      << LPTMR fires, wakeToDpd1 >>
 *                                          | Reset_Handler -> LowPowerBoot():
 *                                          |   MSB_BCKP1 != 0 -> restore -> BX lr
 *                                          |   (resumes in EnterDPD2(), DPD1-path)
 *                                          |   ClearLpPowerSettings()
 *                                          |   return WakeupFromDPD2
 *                                          | [ADVC] APP_DPD2ClockRecovery():
 *                                          | [ADVC]   Pre(3 MHz), DisableADVCCtrl
 *                                          | [ADVC]   SetupFROAon(3 MHz)
 *                                          | [ADVC]   FROdiv1 -> AON_CPU, Post()
 *                                          | [ADVC]   poll ADVC_OPTIMAL_DONE_MASK
 *                                          | [ADVC]   Pre(3 MHz), disableAUX, Post
 *                                          |   EnableIRQ(LPTMR_AON_IRQn)
 *                                          |   LPTMR_AON_IRQHandler: stop LPTMR
 *                                          |   EnableIRQ(SMM_EXT_IRQn)
 *                                          | APP_DeepPowerDown1Ops():
 *                                          |   prev==DPD2 -> __WFI()
 *                                          |      << EXT_INT button press >>
 *                                          | SMM_EXT_IRQHandler(): ClearFlags()
 *  .- MSB_BCKP1 = 0x5A5A <-------------- -| Power_NotifyCM33ToRun()
 *  | Reset_Handler -> Power_LowPowerBoot():
 *  |   BCKP2 != 0 -> restore SP+regs -> BX lr
 *  |   (resumes in Power_EnterDPD1(), PushContext returns 1)
 *  |   while (MSB_BCKP1 != 0x5A5A)  -- spin
 *  |   clear BCKP2 + MSB_BCKP1
 *  |   return kStatus_Power_WakeupFromDPD1
 *  | BOARD_InitHardware(), ClearLpPowerSettings()
 *  '-> Resume main() -- Active
 */
```

---

### Sequence G -- Active -> DPD1 -> DPD2 -> Active

Same DPD1/DPD2 entry as Seq F, but `wakeToDpd1=false` so LPTMR wakes
CM0+ directly to Active. CM0+ performs ADVC clock recovery, then signals
CM33 to resume from DPD1.

```c
/*
 *  CM33                                    |  CM0+
 *  --------------------------------------- | -------------------------------
 *  (DPD1 entry same as Seq E,             |
 *   nextTrans = Dpd1ToDpd2WakeToActive)    |
 *  PushContext() -> BCKP2                  |
 *  __WFI() =========================>     | (CM33 powered off, DPD1)
 *  - - - - - - - - - - - - - - - - - - - | APP_DPD1ToDPD2BackToActive():
 *                                          |   GETCHAR, ClearAllLowPowerSeq()
 *                                          | [ADVC] Pre(3 MHz)
 *                                          | [ADVC]   FRO16K -> AON_LPTMR
 *                                          | [ADVC] Post()
 *                                          | Power_EnterDeepPowerDown2()
 *                                          |   -- DPD1->DPD2 path (no MU):
 *                                          | [ADVC] SetDpd2AdvcWorkaround()
 *                                          |   SMM/PMU config, prevMode=DPD2
 *                                          |   wakeToDpd1=false, wakeupSrc=LPTMR
 *                                          |   PushContext() -> MSB_BCKP1
 *                                          |   SMM_StartAonDPD2Sequence()
 *                                          |   __WFI() == both cores off (DPD2)
 *                                          |      << LPTMR fires, wakeToActive >>
 *                                          | Reset_Handler -> LowPowerBoot():
 *                                          |   MSB_BCKP1 != 0 -> restore -> BX lr
 *                                          |   ClearLpPowerSettings()
 *                                          |   return WakeupFromDPD2
 *                                          | [ADVC] APP_DPD2ClockRecovery():
 *                                          | [ADVC]   Pre -> FROdiv1 (3 MHz)
 *                                          | [ADVC]   poll ADVC_OPTIMAL_DONE_MASK
 *                                          | [ADVC]   disable AUX CLK -> Post
 *                                          |   EnableIRQ(LPTMR_AON_IRQn)
 *                                          |   LPTMR_AON_IRQHandler: stop LPTMR
 *  .- MSB_BCKP1 = 0x5A5A <-------------- -| Power_NotifyCM33ToRun()
 *  | Reset_Handler -> Power_LowPowerBoot():
 *  |   BCKP2 != 0 -> restore SP+regs -> BX lr
 *  |   (resumes in Power_EnterDPD1(), PushContext returns 1)
 *  |   while (MSB_BCKP1 != 0x5A5A)  -- spin
 *  |   clear BCKP2 + MSB_BCKP1
 *  |   return kStatus_Power_WakeupFromDPD1
 *  | BOARD_InitHardware(), ClearLpPowerSettings()
 *  '-> Resume main() -- Active
 */
```

---

### Sequence H -- Active -> DPD2 -> Active  (both cores, MU handshake)

Both cores enter DPD2 together. CM33 initiates via MU request; CM0+
performs the mandatory ADVC DPD2 workaround before ACKing, then both
save context and execute WFI. On wakeup CM0+ recovers the ADVC clock
tree and signals CM33 via MSB_BCKP1.

```c
/*
 *  CM33                                    |  CM0+
 *  --------------------------------------- | -------------------------------
 *  dpd2Config: wakeToDpd1=false            |
 *    aonWakeupSrc=LPTMR                    |
 *    mainWakeupSrc=LPTMR                   |
 *  [ADVC] Pre(aonFreq)                     |
 *  [ADVC]   CLOCK_AttachClk(FRO16K ->      |
 *  [ADVC]              AON_LPTMR)          |
 *  [ADVC] Post()                           |
 *  Power_EnterDeepPowerDown2():            |
 *    Power_CM33RequestLowPowerMode(DPD2)   |
 *    -- MU request ----------------------->| MU_B_RX_IRQHandler fires
 *                                          | Power_InterpretRequest():
 *                                          |   APP_SecondaryCoreCallback(DPD2):
 *                                          |     EnableIRQ(LPTMR_AON_IRQn)
 *                                          |     MU_DisableInterrupts(MU_RX)
 *                                          | [ADVC] SetDpd2AdvcWorkaround():
 *                                          | [ADVC]   CLOCK_DisableADVCControl()
 *                                          | [ADVC]   Pre(targetFreq)
 *                                          | [ADVC]   FROdiv4 -> AON_CPU
 *                                          | [ADVC]   disable FRO10M, auto-cal
 *                                          | [ADVC]   Post()
 *    MU_A_RX_IRQHandler: ACK received      |
 *    <------------ MU ACK ---------------- | MU_SendMsg() ACK
 *    while (cm0pWFI == false)  -- wait     | previousPowerMode = DPD2
 *                                          | PushContext() -> MSB_BCKP1
 *                                          | cm0pWFI = true, SLEEPDEEP
 *                                          | __WFI()  <- CM0+ ready/sleeping
 *    Power_EnableDualDomainWakeupSources() |
 *    Power_ConfigSleepModeManager()        |
 *    CMC: DeepPowerDown, prevMode=DPD2     |
 *    PushContext() -> BCKP2                |
 *    __WFI() == CM33 + CM0+ both in DPD2 ===========================>
 *                         << LPTMR fires, both cores wake to Active >>
 *
 *  CM0+ wakeup (Reset_Handler):
 *    LowPowerBoot(): MSB_BCKP1 != 0 -> restore -> BX lr
 *    (resumes in Power_InterpretRequest(), PushContext returns 1)
 *    ClearLpPowerSettings(), MSB_BCKP1 = 0
 *    prev==DPD2, Power_GetCurrentPowerMode() -> Active
 *    [ADVC] APP_DPD2ClockRecovery():
 *    [ADVC]   Pre(3 MHz), CLOCK_DisableADVCControl()
 *    [ADVC]   SetupFROAon(3 MHz), FROdiv1 -> AON_CPU, Post()
 *    [ADVC]   poll ADVC_OPTIMAL_DONE_MASK
 *    [ADVC]   Pre(3 MHz), AON_CGU: disable AUX CLK, Post()
 *    assert(curMode == kPower_Active)
 *  .- MSB_BCKP1 = 0x5A5A   (Power_NotifyCM33ToRun)
 *  |
 *  | CM33 wakeup (Reset_Handler):
 *  |   LowPowerBoot(): BCKP2 != 0 -> restore -> BX lr
 *  |   (resumes in Power_EnterDPD2(), PushContext returns 1)
 *  |   while (MSB_BCKP1 != 0x5A5A)  -- spin
 *  |   MSB_BCKP1 = 0, clear BCKP2
 *  |   return kStatus_Power_WakeupFromDPD2
 *  | BOARD_InitHardware(), ClearLpPowerSettings()
 *  | EnableIRQ(MU_A_RX_IRQn)
 *  | [ADVC] ADVC_IsEnabled() -> true  (AON retained, no re-init)
 *  '-> Resume main() -- Active
 */
```

---

### Sequence I -- Active -> DPD2 -> DPD1 -> Active  (both cores, MU handshake)

Same DPD2 entry as Seq H (MU handshake, both cores). `wakeToDpd1=true`
so LPTMR wakes CM0+ to DPD1 only (CM33 stays off). CM0+ recovers ADVC,
waits for EXT_INT to trigger the final DPD1 -> Active transition, then
signals CM33 via MSB_BCKP1.

```c
/*
 *  CM33                                    |  CM0+
 *  --------------------------------------- | -------------------------------
 *  dpd2Config: wakeToDpd1=true             |
 *    aonWakeupSrc=LPTMR                    |
 *    mainWakeupSrc=EXT_INT                 |
 *  Power_CM33RequestLowPowerMode(DPD2)     |
 *  -- MU request ------------------------->| MU_B_RX_IRQHandler fires
 *                                          | Power_InterpretRequest():
 *                                          |   APP_SecondaryCoreCallback(DPD2):
 *                                          |     PRINTF: press EXT_INT to wake
 *                                          |     MU_DisableInterrupts(MU_RX)
 *                                          | [ADVC] SetDpd2AdvcWorkaround()
 *  <------------ MU ACK ----------------  | MU_SendMsg() ACK
 *  cm0pWFI=true -> proceed                 | PushContext() -> MSB_BCKP1
 *                                          | cm0pWFI = true -> __WFI()
 *  Power_ConfigSleepModeManager(), CMC cfg |
 *  previousPowerMode = DPD2                |
 *  PushContext() -> BCKP2                  |
 *  __WFI() == CM33 + CM0+ both in DPD2 ===========================>
 *             << LPTMR fires, wakeToDpd1=true, CM33 stays off >>
 *
 *  CM0+ wakeup to DPD1 (Reset_Handler):
 *    LowPowerBoot(): MSB_BCKP1 != 0 -> restore -> BX lr
 *    (resumes in Power_InterpretRequest(), PushContext returns 1)
 *    ClearLpPowerSettings(), MSB_BCKP1 = 0
 *    prev==DPD2, Power_GetCurrentPowerMode() -> DPD1
 *    [ADVC] APP_DPD2ClockRecovery():
 *    [ADVC]   Pre -> FROdiv1 (3 MHz) -> poll ADVC_OPTIMAL_DONE_MASK
 *    [ADVC]   disable AUX CLK -> Post
 *    APP_ClearPendingIRQs()
 *    LPTMR_AON_IRQHandler: ClearLpPowerSettings(), stop LPTMR
 *    EnableIRQ(SMM_EXT_IRQn), PWDN_CONFIG &= ~Q_TMT_EN
 *    APP_DeepPowerDown1Ops(): prev==DPD2 -> __WFI()
 *                         << EXT_INT button press >>
 *    SMM_EXT_IRQHandler(): ClearExternalIntFlag(), ClearLpPowerSettings()
 *  .- MSB_BCKP1 = 0x5A5A   (Power_NotifyCM33ToRun)
 *  |
 *  | CM33 wakeup (Reset_Handler):
 *  |   LowPowerBoot(): BCKP2 != 0 -> restore -> BX lr
 *  |   (BCKP2 still valid -- CM33 was off the entire DPD2 + DPD1 period)
 *  |   (resumes in Power_EnterDPD2(), PushContext returns 1)
 *  |   while (MSB_BCKP1 != 0x5A5A)  -- spin
 *  |   MSB_BCKP1 = 0, clear BCKP2
 *  |   return kStatus_Power_WakeupFromDPD2
 *  | BOARD_InitHardware(), ClearLpPowerSettings()
 *  | EnableIRQ(MU_A_RX_IRQn)
 *  | [ADVC] ADVC_IsEnabled() -> true  (AON retained, still active)
 *  '-> Resume main() -- Active
 */
```

## Supported Boards
- FRDM-MCXL255
