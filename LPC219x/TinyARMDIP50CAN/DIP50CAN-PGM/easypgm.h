//---------------------------------------------------------------------------
// Filename: Easypgm.H
//---------------------------------------------------------------------------
// Headers for Main routine, initialisation, comms with PC
// V0.1 01-12-2006 Initial release
//---------------------------------------------------------------------------
// Created for TinyARM PHIL40-03
// Tools used: IAR EWARM v4.40A
// Copyright PASAT 2004-2006
//---------------------------------------------------------------------------
// Size of 24C256 and 25C256 is 0x8000 but there is not enough RAM in LPC2103
#define Size_25C256 0x800
#define Size_24C256 0x800
#define SPI25C_READ_CMD  0x03
#define SPI25C_WRITE_CMD 0x02
#define SPI25C_WREN_CMD  0x06
#define SPI25C_WRDI_CMD  0x04
#define SPI25C_RDSR_CMD  0x05
#define KEY_ENTER 0x0d
#define KEY_ESC   0x1B
#define I2C_SLAVEADR_RCV  0xA1
#define I2C_SLAVEADR_SEND 0xA0

#define KEY_RIGHT 0x10
#define KEY_LEFT  0x08
