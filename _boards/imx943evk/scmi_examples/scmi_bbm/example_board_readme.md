## Supported Boards

- IMX943-EVK

## BBM sub-tests on this board

The i.MX943 EVK is real silicon with a physical ON/OFF button and an RTC that
runs at real speed, so every group listed in the example readme is valid here,
including the two marked silicon only.

The button and GPR groups are still off by default. The button check only
reports the current state, so it says nothing unless somebody is at the board to
press the button and compare two readings, and no System Manager configuration
grants a GPR to this logical machine yet. Turn either on in the board `app.h`
when doing that specific check by hand.

## Example output on this board

Captured on an i.MX943 EVK with the default switches, running on the M33S
logical machine:

```
SCMI BBM protocol example
Exercises the BBM protocol of the System Manager so that BBNSM
features can be verified without booting Linux.
Enabled tests, with the target each one is valid on:
  protocol queries : yes (any)
  RTC              : yes (any)
  RTC time         : yes (any)
  RTC alarm        : yes (any to program, silicon to fire)
  button           : no (silicon)
  GPR              : no (any)

  protocol version: 0x   10000
  RTC count: 2, GPR count: 8
  implemented messages:
    -16s (0xFFC8A10): yes, attributes 0x       3
    -16s (0xFFC8A18): yes, attributes 0x       4
    -16s (0xFFC8A20): yes, attributes 0x       5
    -16s (0xFFC8A30): yes, attributes 0x       6
    -16s (0xFFC8A40): yes, attributes 0x       7
    -16s (0xFFC8A50): yes, attributes 0x       8
    -16s (0xFFC8A60): yes, attributes 0x       9
    -16s (0xFFC8A6C): yes, attributes 0x       A
    -16s (0xFFC8A78): yes, attributes 0x       B
    -16s (0xFFC8A88): yes, attributes 0x       C
  negotiated protocol version: 0x   10000
  RTC 0 name: bbnsm
  seconds width: 32, tick width: 47, ticks per second: 32768
  RTC time (seconds): 0x       06955D678
  RTC time write path: writing 2 distinct dates
  RTC time set to: 2026- 1- 1  0: 0: 0 UTC
  RTC time read back: 2026- 1- 1  0: 0: 0 UTC
  RTC time write path: PASS for date 1
  RTC time set to: 2030- 6-15 12:34:56 UTC
  RTC time read back: 2030- 6-15 12:34:56 UTC
  RTC time write path: PASS for date 2
  RTC time write path checked; time advance is not verified here
  RTC alarm enabled at: 2030- 6-15 12:35: 6 UTC
  RTC alarm notification enabled
  RTC battery low: 0, RTC was reset: 0

SCMI BBM example result: PASS

SCMI BBM RTC 0 notification: alarm expired (flags=0x00000001)
```

Two things in that log are worth reading carefully.

The per message rows print `-16s` instead of the message name, and the numbers
are padded to a fixed width rather than to the requested digit count, because
the debug console used on this board prints a reduced set of `printf`
conversions: it does not implement the `%-16s` left justified string field or
the `%08X` zero padded hex field, so it falls back to echoing the flag text and
to its own default padding. The name and value the example passed in are still
correct, only their presentation is off. Read the message ID in parentheses to
tell the rows apart; the order matches the table in the example readme.

The alarm notification arrives after the example has already printed its
verdict, because the alarm is programmed ten seconds ahead and the example does
not wait for it. Seeing the `SCMI BBM RTC 0 notification:` line after the result
line is the expected behaviour, and it is the proof that the alarm actually
fired on silicon rather than only being accepted by the System Manager. The line
comes from `SystemPlatformHandler()` in the device support file, not from the
example, so it appears in any application on this core that enables the BBM
notifications.

An image built before the notification print was corrected reports that line as
`flags=0x20F09200`, because it passed the MU status register where the SCMI
event payload belonged. If a log shows a value with bits above the low three
set, it came from such an image: the number is the mailbox doorbell state, not
an RTC reason, and it should not be read as one.
