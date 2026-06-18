#include <stdint.h>

#include "ipc_mbx.h"

/* place in a specific named section, for the linker scatter file */
#pragma section = "Master_mbxTable"
Mbx Master_mbxTable[NUM_MASTER_MBX] @ "Master_mbxTable";

/* place in a specific named section, for the linker scatter file */ 
#pragma section = "Slave_mbxTable"
Mbx Slave_mbxTable[NUM_SLAVE_MBX] @ "Slave_mbxTable";

	






