# tfm_cfpa

## Overview
The TFM CFPA application provides update of CFPA table.
The tfm_cfpa application will download current CFPA table from FFR and it will update the new version number according to current version number.
NOTE: When tfm_cfpa application is loaded to the device and PRINCE is active, the application is executed only ones.
This behaviour is caused by updating of CFPA table in FFR and after reset, the PRINCE input vector is changed in CFPA table and application is invalid.