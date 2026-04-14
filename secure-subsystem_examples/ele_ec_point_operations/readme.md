# ele_ec_point_operations

## Overview
The ELE EC Point Operations application provides an example how to use SSSAPI to
apply ADD, SUB, and MULT operations to elliptic curve points.
The MULT operation may also be used to validate if a  given point is on a
given curve. The example demonstrates how to perform this check.

This example, and the functionality it demonstrates, depends on the latest
available firmware for the S200 subsystem.

## Running the demo
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ELE EC Point Operations SSSAPI Example

**** Elliptic Curve operation MULT ****
Init kObjIn1...OK
Init kObjIn2...OK
Init kObjOut...OK
Allocate kObjIn1 key object handle...OK
Allocate kObjIn2 key object handle...OK
Allocate kObjOut key object handle...OK
Set scalar in kObjIn1...OK
Set point in kObjIn2...OK
MULT operation...OK
Get the result...OK
MULT result matches expected value...OK
Cleanup...OK

**** Elliptic Curve operation ADD ****
Init kObjIn1...OK
Init kObjIn2...OK
Init kObjOut...OK
Allocate kObjIn1 key object handle...OK
Allocate kObjIn2 key object handle...OK
Allocate kObjOut key object handle...OK
Set point in kObjIn1...OK
Set point in kObjIn2...OK
ADD operation...OK
Get result...OK
ADD result matched expected value...OK
Cleanup...OK

**** Elliptic Curve operation SUB ****
Init kObjIn1...OK
Init kObjIn2...OK
Init kObjOut...OK
Allocate kObjIn1 key object handle...OK
Allocate kObjIn2 key object handle...OK
Allocate kObjOut key object handle...OK
Set point in kObjIn1...OK
Set point in kObjIn2...OK
SUB operation...OK
Get result...OK
SUB result matches expected value...OK
Cleanup...OK

**** Elliptic Curve operation point on curve test ****
Init kObjIn1...OK
Init kObjIn2...OK
Init kObjOut...OK
Allocate kObjIn1 key object handle...OK
Allocate kObjIn2 key object handle...OK
Allocate kObjOut key object handle...OK
Set scalar in kObjIn1...OK
Set valid curve point in kObjIn2...OK
MULT operation on a valid point on the curve...OK
Set invalid curve point in kObjIn2...OK
MULT operation on an invalid point outside the curve...OK
Cleanup...OK

End of Example with SUCCESS!!

Example end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [KW47-EVK](../../_boards/kw47evk/secure-subsystem_examples/ele_ec_point_operations/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/secure-subsystem_examples/ele_ec_point_operations/example_board_readme.md)
- [KW47-LOC](../../_boards/kw47loc/secure-subsystem_examples/ele_ec_point_operations/example_board_readme.md)
- [MCXW72-LOC](../../_boards/mcxw72loc/secure-subsystem_examples/ele_ec_point_operations/example_board_readme.md)
- [FRDM-MCXW72](../../_boards/frdmmcxw72/secure-subsystem_examples/ele_ec_point_operations/example_board_readme.md)
