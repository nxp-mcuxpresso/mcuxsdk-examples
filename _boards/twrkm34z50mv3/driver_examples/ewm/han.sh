#!/bin/bash
#read -p "Please enter the file name with a suffix:" name

sed -i '1,6d' app.h
sed -i '1,6d' hardware_init.c


sed -i '1 i /*\n * Copyright 2021 NXP\n * All rights reserved.\n *\n * SPDX-License-Identifier: BSD-3-Clause\n */\n' app.h
sed -i '1 i /*\n * Copyright 2021 NXP\n * All rights reserved.\n *\n * SPDX-License-Identifier: BSD-3-Clause\n */\n' hardware_init.c 
sed -i '1 i /*\n * Copyright 2021 NXP\n * All rights reserved.\n *\n * SPDX-License-Identifier: BSD-3-Clause\n */\n' pin_mux.h
sed -i '1 i /*\n * Copyright 2021 NXP\n * All rights reserved.\n *\n * SPDX-License-Identifier: BSD-3-Clause\n */\n' pin_mux.c
