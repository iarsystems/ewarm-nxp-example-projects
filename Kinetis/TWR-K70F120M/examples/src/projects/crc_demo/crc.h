#ifndef __CRC_H
#define __CRC_H

// Error code definition
#define CRC_ERR_SUCCESS       0  
#define CRC_ERR_CODE_BASE    (0x2000)
#define CRC_ERR_TOT_VAL      (CRC_ERR_CODE_BASE+1)
#define CRC_ERR_TOTR_VAL     (CRC_ERR_CODE_BASE+2)
#define CRC_ERR_FXOR_VAL     (CRC_ERR_CODE_BASE+3)
#define CRC_ERR_TCRC_VAL     (CRC_ERR_CODE_BASE+4)

// Prototypes
int CRC_Config(uint32_t poly,uint32_t tot,uint32_t totr,uint32_t fxor,uint32_t tcrc);
uint32_t CRC_Cal_16(uint32_t seed,uint8_t *msg, uint32_t sizeBytes);
uint32_t CRC_Cal_32(uint32_t seed,uint8_t *msg, uint32_t sizeBytes);
#endif
