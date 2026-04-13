# dpd1_aon_monitor_mcxl

## Overview
This example demonstrates the ROM CGU-clobber workaround for DPD1 mode on MCXL25x
dual-core devices.

When CM33 enters DPD1 with context saving enabled, CM0+ remains active in the AON
domain and prints periodic heartbeat messages via AON UART.  Sending any character
on the AON UART triggers CM0+ to wake the Main domain while protecting its own clock
tree from ROM's System Init, which unconditionally modifies AON CGU registers.

The workaround demonstrated by CM0+:
1. Clear `WKUP_MAIN` so no hardware source automatically powers the Main domain.
2. Save AON CGU registers (`CLK_CONFIG`, `CLOCK_DIV`, `PER_CLK_CONFIG`, `PER_CLK_EN`).
3. Call `SMM_WakeupMainDomain()` to power on the Main domain (CM33 ROM runs here and
   clobbers AON CGU).
4. Call `Power_NotifyCM33ToRun()` -- waits for the `fsl_power.c` BCKP1 sync signal
   (CM33 context restore complete, ROM System Init finished), then writes
   `dualCoreSynced` to unblock CM33.
5. Restore AON CGU registers -- UART and other AON peripherals resume normally.

### Interrupt Architecture
- **CM33** enables the AON LPUART RX interrupt at peripheral level
  (`LPUART_EnableInterrupts`) before entering DPD1.
- **CM0+** enables the corresponding NVIC IRQ (`EnableIRQ(LPUART0_AON_IRQn)`) in its
  `APP_DeepPowerDown1Ops`.  This demonstrates the dual-core interrupt pattern where
  CM33 configures the peripheral and CM0+ intercepts the interrupt.

## Sequence Diagram -- Active -> DPD1 -> Active (UART RX wakeup)

```
  CM33                                    |  CM0+
  --------------------------------------- | -------------------------------
  BOARD_InitHardware(), sync with CM0+    |  BOARD_InitHardware(), sync
                                          |
  LPUART_EnableInterrupts(AON_LPUART, RX) |
  dpd1Config: saveContext=true            |
    mainWakeupSource = NONE               |
    nextTrans = Dpd1ToActive              |
  Power_EnterDeepPowerDown1():            |
    BCKP1 = RomWakeupVector               |
    BCKP2 = Handle_Offset                 |  APP_ActiveOps():
    SMM config, CMC: DeepPowerDown        |    Power_GetCurrentPowerMode()
    PushContext()                          |    polling -> detects DPD1
    __WFI() =========================>   |
  - - - - - - - - - - - - - - - - - - - |
                                          | APP_DeepPowerDown1Ops():
                                          |   EnableIRQ(LPUART0_AON_IRQn)
                                          |   PRINTF("[DPD1] Heartbeat #N ...")
                                          |   SDK_DelayAtLeastUs(10 ms steps)
                                          |   ...heartbeat loop repeats...
                                          |
                                          |      << user sends char on UART >>
                                          | LPUART0_AON_IRQHandler():
                                          |   ReadByte(), DisableIRQ, flag=true
                                          |
                                          | APP_WakeupMainDomainSafely():
                                          |   APP_SaveCguState(&snapshot)
                                          |   SMM_WakeupMainDomain()
                                          |     (ROM runs, clobbers AON CGU)
                                          |   Power_NotifyCM33ToRun():
                                          |     wait BCKP1 sync + set dualCoreSynced
  .- dualCoreSynced = Synced <---------- |
  | Reset_Handler -> Power_LowPowerBoot():
  |   BCKP2 != 0 -> restore -> BX lr
  |   (resumes after PushContext, ret=1)
  |   BCKP1 = 0x5A5A/0xA55A (sync signal)
  |   while (dualCoreSynced != Synced) -- spin
  |   clear BCKP + return WakeupFromDPD1
  | BOARD_InitHardware()              |   APP_RestoreCguState(&snapshot)
  '-> Resume main() -- Active         |     (UART resumes correctly)
```

## Supported Boards
