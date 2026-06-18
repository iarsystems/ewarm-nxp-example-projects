#ifndef _PLLTURBO_INCLUDED
#define _PLLTURBO_INCLUDED

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

//Function prototypes
MEMORY_ATTRIBUTE void Exec_Speed_Test(void);
MEMORY_ATTRIBUTE void ConnectPLL(void);
MEMORY_ATTRIBUTE void DisconnectPLL(void);
MEMORY_ATTRIBUTE void EnableMAM(void);
MEMORY_ATTRIBUTE void DisableMAM(void);

#endif



