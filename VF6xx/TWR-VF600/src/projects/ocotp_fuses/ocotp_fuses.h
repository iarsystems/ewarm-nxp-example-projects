///////////////////////////////////////////////////////////////////////////////
//
// Filename: program.c
//
// Description: This is the C code for the OCOTP hwvfuse Programming.
//
////////////////////////////////////////////////////////////////////////////////

#include "common.h"

void wait_for_fuse(void);
void program_fuse_word(int address,int data_temp);
void read_fuse_word(int address);

#define OCOTP_WR_UNLOCK_KEY_VALUE 		0x3e770000



typedef struct fuse_struct {
	char bankno;
	char word;
	char bytepos;
	char value;
} fusedata_t, *pfuse_t;
