# scmi_bbm

## Overview

The SCMI BBM example exercises the BBM (Battery Backed Module) protocol of the
System Manager over the SCMI agent-to-platform channel. It lets the BBNSM
features be verified from an MCUXpresso SDK application, without depending on
Linux.

The example performs the following steps:

1. Read the BBM protocol version.
2. Read the protocol attributes and report the number of RTCs and GPRs.
3. Query the attributes of every BBM message and report which ones the
   platform implements.
4. Negotiate the protocol version that was just read.
3. Read the attributes of RTC 0 (name, seconds width, tick width, ticks).
4. Read the current RTC time in seconds.
5. Read the RTC state (battery low, RTC reset).
6. Write a pattern to GPR 0 and read it back to confirm the round trip.
7. Read the ON/OFF button state.

Each request is checked, and a request that does not succeed is reported
with the name of the SCMI status code as well as its numeric value.
The example ends with a single PASS or FAILED result line.

## Selecting the sub-tests

Not every BBM feature can be exercised on every target, so each functional
group has its own compile-time switch. An emulation platform such as Zebu has
no physical ON/OFF button, for example, so the button test has to be turned
off there.

| Switch                    | Default | Valid on          | Covers                                               |
|---------------------------|---------|-------------------|------------------------------------------------------|
| always on                 | -       | emulation and silicon | Protocol version, protocol attributes, per message attributes, version negotiation |
| `DEMO_BBM_TEST_RTC`       | 1       | emulation and silicon | RTC attributes, first time read, RTC state       |
| `DEMO_BBM_TEST_RTC_TIME`  | 1       | emulation and silicon | RTC time write path, two dates written and read back |
| `DEMO_BBM_TEST_RTC_ALARM` | 1       | silicon           | RTC alarm program plus alarm notification            |
| `DEMO_BBM_TEST_BUTTON`    | 0       | silicon           | ON/OFF button state                                  |
| `DEMO_BBM_TEST_GPR`       | 0       | emulation and silicon | GPR write and read-back round trip               |

"Valid on" says where the check produces a meaningful result, not where it
compiles or runs without crashing.

A check is valid on emulation and silicon alike when it only sends a request
and inspects the answer, because that path does not depend on how fast the
target runs or on anything a person has to do. That covers the protocol
queries, the RTC attributes and state, the RTC time write path and the GPR
round trip.

A check is silicon only when it needs the target to reach a point in time or
needs a physical part. The alarm has to wait for the comparison to fire, which
an emulation platform cannot reach in reasonable time, and the button needs a
person to press it. The alarm group is split in the log for that reason:
programming the comparison value is reported on any target, while whether it
fires is left to silicon.

For an emulation platform, keep the groups marked "emulation and silicon" and
turn the other two off:

```c
#define DEMO_BBM_TEST_RTC        1
#define DEMO_BBM_TEST_RTC_TIME   1
#define DEMO_BBM_TEST_RTC_ALARM  0
#define DEMO_BBM_TEST_BUTTON     0
#define DEMO_BBM_TEST_GPR        0
```

The default is not the same question as where a check is valid. Two groups are
valid on silicon yet still default to off, because neither decides anything on
its own in a run nobody is watching:

- The button check reads the current state. With nobody pressing the button it
  reports whatever state the button happens to be in, which says nothing about
  whether the reporting works. Turn it on when somebody is at the board to press
  it and compare the two readings.
- The GPR round trip needs a System Manager configuration that grants a GPR to
  this logical machine. None of the configurations shipped today does, so with
  the group on the log only fills with skipped requests. Turn it on after
  granting one.

What is left on by default is the set of checks that reach a verdict without
help: the protocol queries, the RTC queries, the RTC time write path and, on
silicon, the alarm programming.

The RTC group is split because the checks prove different things.

The RTC attributes and RTC state queries only read static information, so they
work anywhere the RTC is granted.

The time check writes two clearly different dates and reads each one back. What
it proves is the write path: this agent's request reached the System Manager,
passed its permission check, and landed in the right RTC register. Two dates
are used rather than one so a read-back cannot pass by returning a constant or
a value left over from an earlier run. What it does not prove is that the
counter advances at the right rate, because the read-back is compared against
the value written rather than against wall-clock time. That makes the check
valid on an emulation platform whose counter barely moves, while leaving the
rate itself to be verified on a target running at real speed.

The alarm check only programs the comparison value and enables the
notification. Whether the comparison actually fires is not observed, because
the alarm is a few seconds in the future and an emulation platform cannot reach
that point in reasonable time. Turn `DEMO_BBM_TEST_RTC_ALARM` off there.

`DEMO_BBM_TEST_GPR` is off by default because the System Manager
configurations shipped today do not grant a GPR to the logical machine this
example runs on. Turn it on after granting one.

Turning `DEMO_BBM_TEST_RTC` off also turns the time and alarm groups off,
because they use the same RTC instance. Turning `DEMO_BBM_TEST_RTC_TIME` off
also turns the alarm group off, because the alarm is programmed relative to the
time this example writes.

The protocol version, the protocol attributes, the per message attributes and
the version negotiation are always exercised, because they depend only on the
channel and not on any BBM hardware feature.

The per message query walks every message the BBM protocol defines and reports
whether the platform implements it. That makes an unimplemented message visible
up front instead of only when a later request that needs it fails. The version
negotiation asks for the version the platform just reported, which must
succeed and therefore confirms both sides agree on the version in use.

Override a switch in the board `app.h` of the example, for example to add the
button check to a session where somebody is at the board:

```c
#define DEMO_BBM_TEST_RTC        1
#define DEMO_BBM_TEST_RTC_TIME   1
#define DEMO_BBM_TEST_RTC_ALARM  1
#define DEMO_BBM_TEST_BUTTON     1
#define DEMO_BBM_TEST_GPR        0
```

If even the RTC time write path has to go, for example while the RTC itself is
not yet brought up, keep only the static RTC queries:

```c
#define DEMO_BBM_TEST_RTC        1
#define DEMO_BBM_TEST_RTC_TIME   0
#define DEMO_BBM_TEST_RTC_ALARM  0
#define DEMO_BBM_TEST_BUTTON     0
#define DEMO_BBM_TEST_GPR        0
```

A switch can also be set on the compiler command line, for example
`-DDEMO_BBM_TEST_BUTTON=0`. The example prints the active configuration at
startup, together with the target each group is valid on, so the log states
both what was run and whether the result means anything on the target it came
from:

```
Enabled tests, with the target each one is valid on:
  protocol queries : yes (any)
  RTC              : yes (any)
  RTC time         : yes (any)
  RTC alarm        : no (any to program, silicon to fire)
  button           : no (silicon)
  GPR              : no (any)
```

## Resources the System Manager does not grant

Each BBM resource is assigned to a logical machine by the System Manager
configuration. A request for a resource this logical machine does not own is
answered with `SCMI_ERR_DENIED` (-3), and a feature the platform does not
implement is answered with `SCMI_ERR_NOT_SUPPORTED` (-1). The example reports
both as SKIPPED and keeps the overall result at PASS, because neither is a
software or hardware defect.

On the i.MX943 EVK, for example, `mx94evk.cfg` assigns the BBM GPRs to the SM
and the AP logical machines, so an example running on the M33S logical machine
owns no GPR and the GPR requests are reported as skipped:

```
  GPR set: SKIPPED, not granted to this agent (SCMI_ERR_DENIED, -3)
```

To exercise the GPRs from this logical machine instead, move one of the GPR
resources to it in the System Manager configuration, or turn the GPR group off
with `DEMO_BBM_TEST_GPR` so the example does not issue the requests at all.

## Prerequisites

The System Manager must be running and must grant this logical machine access
to the BBM protocol; otherwise requests return SCMI_ERR_DENIED.

## Running the demo

The log below shows the output of the example in the terminal window.

```
SCMI BBM protocol example
...
SCMI BBM example result: PASS
```
- Configure the RTC time and read it back.
- Program and enable an RTC alarm, and enable RTC alarm notifications.
