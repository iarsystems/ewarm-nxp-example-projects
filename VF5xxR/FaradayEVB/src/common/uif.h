// --------------------------------------------------------------------------
// COPYRIGHT (c) Freescale 2009
// Freescale Confidential Proprietary
//  All Rights Reserved
// --------------------------------------------------------------------------
//! \file    uif.h
//! \brief   This file declares the variables and functions used for ThirdEye.
// TYPE: C Header File
// DEPARTMENT: MSG-IDC
// --------------------------------------------------------------------------
// PURPOSE:  This file declares the variables and functions used for ThirdEye.
// --------------------------------------------------------------------------
// $Log: uif.h.rca $
// 
// --------------------------------------------------------------------------

#ifndef UIF_H_
#define UIF_H_

#include "uif_cmd.h"
//#include "thirdeye.h"
#include "typedefs_UINT8.h"
#include "printf.h"

void thirdeye_main(const char *npi_name);
void send_test(void);
void wait_for_command(void);
int get_run_command(void);
void get_msg();
void int_to_msg(int i);
UINT32 msg_to_int(char *msg);
void start_running_tests();

extern const  unsigned char start_char;
extern const  unsigned char end_char;
extern int UIF_TEST_SIZE;

#endif
