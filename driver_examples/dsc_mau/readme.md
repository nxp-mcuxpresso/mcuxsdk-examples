# dsc_mau

## Overview
This example shows how to use the MAU (Math Accelerator Unit) on DSC.
The MAU calculated results are compared with the software calculated
results to verify the MAU function.

## Running the demo
The expected output is shown below.
```
MAU Driver Example
--- SinFlt32 ---
  sin(0)               MAU=0  ref=0  err=0  [PASS]
  sin(PI/2)            MAU=1.000000  ref=1.000000  err=0.000000  [PASS]
  sin(PI/4)            MAU=0.707107  ref=0.707107  err=0  [PASS]
  sin(-PI/2)           MAU=-1.000000  ref=-1.000000  err=0.000000  [PASS]

--- CosFlt32 ---
  cos(0)               MAU=1.000000  ref=1.000000  err=0  [PASS]
  cos(PI/2)            MAU=0  ref=-0.000000  err=0.000000  [PASS]
  cos(PI/4)            MAU=0.707107  ref=0.707107  err=0  [PASS]
  cos(PI)              MAU=-1.000000  ref=-1.000000  err=0.000000  [PASS]

--- AtanFlt32 ---
  atan(0.0)/PI         MAU=0  ref=0  err=0  [PASS]
  atan(1.0)/PI         MAU=0.250000  ref=0.250000  err=0.000000  [PASS]
  atan(-1.0)/PI        MAU=-0.250000  ref=-0.250000  err=0.000000  [PASS]
  atan(0.5)/PI         MAU=0.147584  ref=0.147584  err=0.000000  [PASS]

--- SqrtFlt32 ---
  sqrt(1.0)            MAU=1.000000  ref=1.000000  err=0  [PASS]
  sqrt(4.0)            MAU=2.000000  ref=2.000000  err=0  [PASS]
  sqrt(9.0)            MAU=3.000000  ref=3.000000  err=0  [PASS]
  sqrt(0.25)           MAU=0.500000  ref=0.500000  err=0  [PASS]

--- InvSqrtFlt32 ---
  1/sqrt(1.0)          MAU=1.000000  ref=1.000000  err=0  [PASS]
  1/sqrt(4.0)          MAU=0.500000  ref=0.500000  err=0  [PASS]
  1/sqrt(0.25)         MAU=2.000000  ref=2.000000  err=0  [PASS]
  1/sqrt(9.0)          MAU=0.333333  ref=0.333333  err=0.000000  [PASS]

--- RecipFlt32 ---
  1/1.0                MAU=1.000000  ref=1.000000  err=0  [PASS]
  1/2.0                MAU=0.500000  ref=0.500000  err=0  [PASS]
  1/4.0                MAU=0.250000  ref=0.250000  err=0  [PASS]
  1/0.5                MAU=2.000000  ref=2.000000  err=0  [PASS]

--- SinFrac32 (Q1.31) ---
  sin(0)               MAU=0x00000000(0)  ref=0x00000000(0)  [PASS]
  sin(PI/2)            MAU=0x7FFFFFFF(1.000000)  ref=0x7FFFFF80(1.000000)  [PASS]
  sin(PI/4)            MAU=0x5A8279A0(0.707107)  ref=0x5A827980(0.707107)  [PASS]
  sin(3PI/4)           MAU=0x5A8279A0(0.707107)  ref=0x5A827980(0.707107)  [PASS]

--- CosFrac32 (Q1.31) ---
  cos(0)               MAU=0x7FFFFFFF(1.000000)  ref=0x7FFFFFFF(1.000000)  [PASS]
  cos(PI/2)            MAU=0x00000000(0)  ref=0xFFFFFFA3(-0.000000)  [PASS]
  cos(PI/4)            MAU=0x5A8279A0(0.707107)  ref=0x5A827980(0.707107)  [PASS]
  cos(3PI/4)           MAU=0xA57D8660(-0.707107)  ref=0xA57D8680(-0.707107)  [PASS]

--- AtanFrac32 (Q1.31) ---
  atan(0.0)/PI         MAU=0x00000000(0)  ref=0x00000000(0)  [PASS]
  atan(1.0)/PI         MAU=0x1FFFFFF0(0.250000)  ref=0x1FFFFFC0(0.250000)  [PASS]
  atan(0.0)/PI         MAU=0x00000000(0)  ref=0x00000000(0)  [PASS]
  atan(1.0)/PI         MAU=0x1FFFFFF0(0.250000)  ref=0x1FFFFFC0(0.250000)  [PASS]

--- SqrtFrac32 (Q1.31) ---
  sqrt(0.0)            MAU=0x00000000(0)  ref=0x00000000(0)  [PASS]
  sqrt(0.25)           MAU=0x40000000(0.500000)  ref=0x40000000(0.500000)  [PASS]
  sqrt(0.5)            MAU=0x5A8279A0(0.707107)  ref=0x5A827980(0.707107)  [PASS]
  sqrt(1.0)            MAU=0x7FFFFFE0(1.000000)  ref=0x7FFFFFFF(1.000000)  [PASS]

--- SinFrac16 (Q1.15) ---
  sin(0)               MAU=0x0000(0)  ref=0x0000(0)  [PASS]
  sin(PI/2)            MAU=0x7FFF(0.999969)  ref=0x7FFF(0.999969)  [PASS]
  sin(PI/4)            MAU=0x5A83(0.707123)  ref=0x5A82(0.707092)  [PASS]
  sin(3PI/4)           MAU=0x5A83(0.707123)  ref=0x5A82(0.707092)  [PASS]

--- CosFrac16 (Q1.15) ---
  cos(0)               MAU=0x7FFF(0.999969)  ref=0x7FFF(0.999969)  [PASS]
  cos(PI/2)            MAU=0x0000(0)  ref=0x0000(0)  [PASS]
  cos(PI/4)            MAU=0x5A83(0.707123)  ref=0x5A82(0.707092)  [PASS]
  cos(3PI/4)           MAU=0xA57D(-0.707123)  ref=0xA57E(-0.707092)  [PASS]

--- AtanFrac16 (Q1.15) ---
  atan(0.0)/PI         MAU=0x0000(0)  ref=0x0000(0)  [PASS]
  atan(1.0)/PI         MAU=0x2000(0.250000)  ref=0x1FFF(0.249969)  [PASS]
  atan(0.0)/PI         MAU=0x0000(0)  ref=0x0000(0)  [PASS]
  atan(1.0)/PI         MAU=0x2000(0.250000)  ref=0x1FFF(0.249969)  [PASS]

--- SqrtFrac16 (Q1.15) ---
  sqrt(0.0)            MAU=0x0000(0)  ref=0x0000(0)  [PASS]
  sqrt(0.25)           MAU=0x4000(0.500000)  ref=0x4000(0.500000)  [PASS]
  sqrt(0.5)            MAU=0x5A82(0.707092)  ref=0x5A82(0.707092)  [PASS]
  sqrt(1.0)            MAU=0x7FFF(0.999969)  ref=0x7FFF(0.999969)  [PASS]

--- SqrtInt32 ---
  sqrt(0)              MAU=0  ref=0  [PASS]
  sqrt(4)              MAU=2  ref=2  [PASS]
  sqrt(9)              MAU=3  ref=3  [PASS]
  sqrt(100)            MAU=10  ref=10  [PASS]


====================
Results: 60 / 60 passed
MAU Driver Example Completed Successfully.
```

## Supported Boards
