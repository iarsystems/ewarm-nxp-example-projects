/*
 * Copyright (C) 2010, Freescale Semiconductor, Inc. All Rights Reserved
 * THIS SOURCE CODE IS CONFIDENTIAL AND PROPRIETARY AND MAY NOT
 * BE USED OR DISTRIBUTED WITHOUT THE WRITTEN PERMISSION OF
 * Freescale Semiconductor, Inc.
*/

#include "macros.h"

/**************************************************************************
 *  Write With Mask (write only certain bits)
 **************************************************************************/
void reg8_write_mask(uint32_t addr, uint8_t wdata, uint8_t mask)
{
    uint8_t masked_wdata, rdata;

    rdata = reg8_read(addr);
    masked_wdata = (((~mask) & rdata) | (mask & wdata));
    reg8_write(addr, masked_wdata);
}

void reg16_write_mask(uint32_t addr, uint16_t wdata, uint16_t mask)
{
    uint16_t masked_wdata, rdata;

    rdata = reg16_read(addr);
    masked_wdata = (((~mask) & rdata) | (mask & wdata));
    reg16_write(addr, masked_wdata);
}

void reg32_write_mask(uint32_t addr, uint32_t wdata, uint32_t mask)
{
    uint32_t masked_wdata, rdata;

    rdata = reg32_read(addr);
    masked_wdata = (((~mask) & rdata) | (mask & wdata));
    reg32_write(addr, masked_wdata);
}
