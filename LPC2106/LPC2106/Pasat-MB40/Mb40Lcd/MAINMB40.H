
#define Size_25C256 0x8000
#define Size_24C256 0x8000
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

#ifndef _MAINMB40_INCLUDED
#define _MAINMB40_INCLUDED

#ifndef MEMORY_ATTRIBUTE
#define MEMORY_ATTRIBUTE
#endif

MEMORY_ATTRIBUTE U8 Check_Input_Source(U8 *FirstChar);

#endif



