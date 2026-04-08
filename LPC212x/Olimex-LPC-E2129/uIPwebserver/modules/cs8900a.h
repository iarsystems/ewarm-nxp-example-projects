/***************************************************************************
 **
 **    This file defines the CS8900A specific definitions
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2005
 **
 **    $Revision: 28 $
 **
 ***************************************************************************/
#ifndef __CS8900A_H__
#define __CS8900A_H__

#include "uip_arch.h"

#define RED_LED                 0x0080
#define GREEN_LED               0x0200
#define YELLOW_LED              0x1000

#define UIP_ETHADDR0            0x00        /* 1st byte of the MAC address */
#define UIP_ETHADDR1            0xFF        /* 2nd byte of the MAC address */
#define UIP_ETHADDR2            0xFF        /* 3rd byte of the MAC address */
#define UIP_ETHADDR3            0xFF        /* 4th byte of the MAC address */
#define UIP_ETHADDR4            0xFF        /* 5th byte of the MAC address */
#define UIP_ETHADDR5            0xFF        /* 6th byte of the MAC addres */

#define IOR_PIN                 (1<<12)
#define IOW_PIN                 (1<<13)

#define REG_ISAIOB              0x0020          /* IO base address */
#define REG_CS8900_ISAINT       0x0022          /* ISA interrupt select */
#define REG_CS8900_ISADMA       0x0024          /* ISA Rec DMA channel */
#define REG_ISASOF              0x0026          /* ISA DMA offset */
#define REG_DmaFrameCnt         0x0028          /* ISA DMA Frame count */
#define REG_DmaByteCnt          0x002A          /* ISA DMA Byte count */
#define REG_CS8900_ISAMemB      0x002C          /* Memory base */
#define REG_ISABootBase         0x0030          /* Boot Prom base */
#define REG_ISABootMask         0x0034          /* Boot Prom Mask */

/* EEPROM data and command registers */
#define REG_EECMD               0x0040          /* NVR Interface Command register */
#define REG_EEData              0x0042          /* NVR Interface Data Register */

/* Configuration and control registers */
#define REG_RxCFG               0x0102          /* Rx Bus config */
#define REG_RxCTL               0x0104          /* Receive Control Register */
#define REG_TxCFG               0x0106          /* Transmit Config Register */
#define REG_TxCMD               0x0108          /* Transmit Command Register */
#define REG_BufCFG              0x010A          /* Bus configuration Register */
#define REG_LineCTL             0x0112          /* Line Config Register */
#define REG_SelfCTL             0x0114          /* Self Command Register */
#define REG_BusCTL              0x0116          /* ISA bus control Register */
#define REG_TestCTL             0x0118          /* Test Register */

/* Status and Event Registers */
#define REG_ISQ                 0x0120          /* Interrupt Status */
#define REG_RxEvent             0x0124          /* Rx Event Register */
#define REG_TxEvent             0x0128          /* Tx Event Register */
#define REG_BufEvent            0x012C          /* Bus Event Register */
#define REG_RxMiss              0x0130          /* Receive Miss Count */
#define REG_TxCol               0x0132          /* Transmit Collision Count */
#define REG_LineST              0x0134          /* Line State Register */
#define REG_SelfST              0x0136          /* Self State register */
#define REG_BusST               0x0138          /* Bus Status */
#define REG_TDR                 0x013C          /* Time Domain Reflectometry */

/* Initiate Transmit Registers */
#define REG_TxCommand           0x0144          /* Tx Command */
#define REG_TxLength            0x0146          /* Tx Length */

/* Adress Filter Registers */
#define REG_LAF                 0x0150          /* Hash Table */
#define REG_IA                  0x0158          /* Physical Address Register */

/* Frame Location */
#define REG_RxStatus            0x0400          /* Receive start of frame */
#define REG_RxLength            0x0402          /* Receive Length of frame */
#define REG_RxFrame             0x0404          /* Receive frame pointer */
#define REG_TxFrame             0x0A00          /* Transmit frame pointer */

/* Primary I/O Base Address. If no I/O base is supplied by the user, then this
   can be used as the default I/O base to access the PacketPage Area. */
#define DEFAULTIOBASE           0x0300

/* RxCFG (Register 3)- Receive  Configuration and Interrupt Mask bit definition */
#define SKIP_1                  0x0040
#define RX_STREAM_ENBL          0x0080
#define RX_OK_ENBL              0x0100
#define RX_DMA_ONLY             0x0200
#define AUTO_RX_DMA             0x0400
#define BUFFER_CRC              0x0800
#define RX_CRC_ERROR_ENBL       0x1000
#define RX_RUNT_ENBL            0x2000
#define RX_EXTRA_DATA_ENBL      0x4000

/* RxCTL - Receive Control bit definition */
#define RX_IA_HASH_ACCEPT       0x0040
#define RX_PROM_ACCEPT          0x0080
#define RX_OK_ACCEPT            0x0100
#define RX_MULTCAST_ACCEPT      0x0200
#define RX_IA_ACCEPT            0x0400
#define RX_BROADCAST_ACCEPT     0x0800
#define RX_BAD_CRC_ACCEPT       0x1000
#define RX_RUNT_ACCEPT          0x2000
#define RX_EXTRA_DATA_ACCEPT    0x4000

/* TxCFG (Register 7)- Transmit Configuration Interrupt Mask bit definition */
#define TX_LOST_CRS_ENBL        0x0040
#define TX_SQE_ERROR_ENBL       0x0080
#define TX_OK_ENBL              0x0100
#define TX_LATE_COL_ENBL        0x0200
#define TX_JBR_ENBL             0x0400
#define TX_ANY_COL_ENBL         0x0800
#define TX_16_COL_ENBL          0x8000

/* TxCMD - Transmit Command bit definition */
/* TxCommand - Write-only */
#define TX_START_5_BYTES        0x0000
#define TX_START_381_BYTES      0x0040
#define TX_START_1021_BYTES     0x0080
#define TX_START_ALL_BYTES      0x00C0
#define TX_FORCE                0x0100
#define TX_ONE_COL              0x0200
#define TX_NO_CRC               0x1000
#define TX_RUNT                 0x2000

/* BufCFG (Register B) - Buffer Configuration Interrupt Mask bit definition */
#define GENERATE_SW_INTERRUPT       0x0040
#define RX_DMA_ENBL                 0x0080
#define READY_FOR_TX_ENBL           0x0100
#define TX_UNDERRUN_ENBL            0x0200
#define RX_MISS_ENBL                0x0400
#define RX_128_BYTE_ENBL            0x0800
#define TX_COL_COUNT_OVERFLOW_ENBL  0x1000
#define RX_MISS_COUNT_OVERFLOW_ENBL 0x2000
#define RX_DEST_MATCH_ENBL          0x8000

/* LineCTL - Line Control bit definition */
#define SERIAL_RX_ON            0x0040
#define SERIAL_TX_ON            0x0080
#define AUI_ONLY                0x0100
#define AUTO_AUI_10BASET        0x0200
#define MODIFIED_BACKOFF        0x0800
#define NO_AUTO_POLARITY        0x1000
#define TWO_PART_DEFDIS         0x2000
#define LOW_RX_SQUELCH          0x4000

/* SelfCTL - Software Self Control bit definition */
#define POWER_ON_RESET          0x0040
#define SW_STOP                 0x0100
#define SLEEP_ON                0x0200
#define AUTO_WAKEUP             0x0400
#define HCB0_ENBL               0x1000
#define HCB1_ENBL               0x2000
#define HCB0                    0x4000
#define HCB1                    0x8000

/* BusCTL - ISA Bus Control bit definition */
#define RESET_RX_DMA            0x0040
#define MEMORY_ON               0x0400
#define DMA_BURST_MODE          0x0800
#define IO_CHANNEL_READY_ON     0x1000
#define RX_DMA_SIZE_64K         0x2000
#define ENABLE_IRQ              0x8000

/* TestCTL - Test Control bit definition */
#define LINK_OFF                0x0080
#define ENDEC_LOOPBACK          0x0200
#define AUI_LOOPBACK            0x0400
#define BACKOFF_OFF             0x0800
#define FDX_8900                0x4000

/* RxEvent - Receive Event Bit definition */
#define RX_IA_HASHED            0x0040
#define RX_DRIBBLE              0x0080
#define RX_OK                   0x0100
#define RX_HASHED               0x0200
#define RX_IA                   0x0400
#define RX_BROADCAST            0x0800
#define RX_CRC_ERROR            0x1000
#define RX_RUNT                 0x2000
#define RX_EXTRA_DATA           0x4000
#define HASH_INDEX_MASK         0xFC00

/* TxEvent - Transmit Event Bit definition */
#define TX_LOST_CRS             0x0040
#define TX_SQE_ERROR            0x0080
#define TX_OK                   0x0100
#define TX_LATE_COL             0x0200
#define TX_JBR                  0x0400
#define TX_16_COL               0x8000
#define TX_COL_COUNT_MASK       0x7800

/* BufEvent - Buffer Event Bit definition */
#define SW_INTERRUPT            0x0040
#define RX_DMA                  0x0080
#define READY_FOR_TX            0x0100
#define TX_UNDERRUN             0x0200
#define RX_MISS                 0x0400
#define RX_128_BYTE             0x0800
#define TX_COL_OVRFLW           0x1000
#define RX_MISS_OVRFLW          0x2000
#define RX_DEST_MATCH           0x8000

/* LineST - Ethernet Line Status bit definition */
#define LINK_OK                 0x0080
#define AUI_ON                  0x0100
#define TENBASET_ON             0x0200
#define POLARITY_OK             0x1000
#define CRS_OK                  0x4000

/* SelfST - Chip Software Status bit definition */
#define ACTIVE_33V              0x0040
#define INIT_DONE               0x0080
#define SIBUSY                  0x0100
#define EEPROM_PRESENT          0x0200
#define EEPROMOK                0x0400
#define EL_PRESENT              0x0800
#define EE_SIZE                 0x1000

/* The following block defines the ISQ event types */
#define ISQ_RX_EVENT            0x0004
#define ISQ_TX_EVENT            0x0008
#define ISQ_BUF_EVENT           0x000C
#define ISQ_RX_MISS_EVENT       0x0010
#define ISQ_TX_COL_EVENT        0x0012

#define ISQ_EVENT_MASK          0x003F

/* Ports for I/O-Mode */
#define RX_FRAME_PORT           0x0000
#define TX_FRAME_PORT           0x0000
#define TX_CMD_PORT             0x0004
#define TX_LEN_PORT             0x0006
#define ISQ_PORT                0x0008
#define ADD_PORT                0x000A
#define DATA_PORT               0x000C

/* BusST - ISA Bus Status bit definition */
#define TX_BID_ERROR            0x0080
#define READY_FOR_TX_NOW        0x0100

void cs8900a_init(void);
void cs8900a_send(void);
u16_t cs8900a_poll(void);

#endif /* __CS8900A_H__ */
