#ifndef __spi_H__
#define __spi_H__

#include "common.h"

void spi_init();
void SpiRegSet(unsigned char reg);
void SpiSendDataWord(unsigned char value);
unsigned short SpiReadDataWord(unsigned char reg);

#endif /* __spi_H__ */
