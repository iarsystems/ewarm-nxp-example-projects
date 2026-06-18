#ifndef _FLEXSPI_BOOT_H_
#define _FLEXSPI_BOOT_H_

#include <stddef.h>
#include <stdint.h>

#define FLEXSPI_LUT_INST(code,pads,opr)       ((uint16_t)((code<<10) | (pads<<8) | (opr)))

#define LUT_CODE_STOP         0x00
#define LUT_CODE_CMD_SDR      0x01
#define LUT_CODE_CMD_DDR      0x21
#define LUT_CODE_RADDR_SDR    0x02
#define LUT_CODE_RADDR_DDR    0x22
#define LUT_CODE_CADDR_SDR    0x03
#define LUT_CODE_CADDR_DDR    0x23
#define LUT_CODE_MODE1_SDR    0x04
#define LUT_CODE_MODE1_DDR    0x24
#define LUT_CODE_MODE2_SDR    0x05
#define LUT_CODE_MODE2_DDR    0x25
#define LUT_CODE_MODE4_SDR    0x06
#define LUT_CODE_MODE4_DDR    0x26
#define LUT_CODE_MODE8_SDR    0x07
#define LUT_CODE_MODE8_DDR    0x27
#define LUT_CODE_WRITE_SDR    0x08
#define LUT_CODE_WRITE_DDR    0x28
#define LUT_CODE_READ_SDR     0x09
#define LUT_CODE_READ_DDR     0x29
#define LUT_CODE_LEARN_SDR    0x0A
#define LUT_CODE_LEARN_DDR    0x2A
#define LUT_CODE_DATSZ_SDR    0x0B
#define LUT_CODE_DATSZ_DDR    0x2B
#define LUT_CODE_DUMMY_SDR    0x0C
#define LUT_CODE_DUMMY_DDR    0x2C
#define LUT_CODE_DUMMY_RWDS_SDR  0x0D
#define LUT_CODE_DUMMY_RWDS_DDR  0x2D
#define LUT_CODE_JMP_ON_CS    0x1F

#define LUT_PADS_ONE         0
#define LUT_PADS_TWO         1
#define LUT_PADS_FOUR        2
#define LUT_PADS_EIGHT       3

/*************************************
 *  FlexSPI Configuration Information
 *************************************/
#pragma pack(1)
/*Flex SPI configuration structures */
typedef struct __flexspi_nor_config_param
{
  char      tag[4];                 //ascii:”FCFB”
  uint32_t  version;                //[07:00] bugfix = 0
                                    //[15:08] minor = 0 / 1
                                    //[23:16] major = 1
                                    //[31:24] ascii ‘V’
  uint8_t   rsvd0[4];               //Reserved
  uint8_t   readSampleClkSrc;       //0 – internal loopback
                                    //1 – loopback from DQS pad
                                    //3 – Flash provided DQS
  uint8_t   dataHoldTime;           //Serial Flash CS Hold Time
  uint8_t   dataSetupTime;          //Serial Flash CS setup time
  uint8_t   columnAdressWidth;      //3 – For HyperFlash
                                    //12/13 – For Serial NAND
                                    //0 – Other devices
  uint8_t   deviceModeCfgEnable;    //Device Mode Configuration
  uint8_t   rsvd1;                  //Reserved
  uint16_t  waitTimeCfgCommands;    //Wait time for all configuration
                                    //commands, unit 100us.
  uint32_t  deviceModeSeq;          //Sequence parameter for
                                    //device mode configuration
  uint32_t  deviceModeArg;          //Device Mode argument
                                    //effective only when deviceModeCfgEnable = 1
  uint8_t   configCmdEnable;        //Config Command Enable feature
  uint8_t   rsvd2[3];               //Reserved
  uint32_t  configCmdSeqs[4];       //Sequences for Config Command
  uint32_t  cfgCmdArgs[4];          //Arguments for each separate
                                    //configuration command sequence.
  uint32_t  controllerMiscOption;   //
  uint8_t   deviceType;             //1 – Serial NOR
                                    //2 – Serial NAND
  uint8_t   sflashPadType;          //1 – Single pad
                                    //2 – Dual pads
                                    //4 – Quad pads
                                    //8 – Octal pads
  uint8_t   serialClkFreq;          //Chip specific value, for this
                                    //silicon
                                    //1 – 30 MHz
                                    //2 – 50 MHz
                                    //3 – 60 MHz
                                    //4 – 75 MHz
                                    //5 – 80 MHz
                                    //6 – 100 MHz
                                    //7 – 133 MHz
                                    //8 – 166 MHz
                                    //Other value: 30 MHz
  uint8_t   lutCustomSeqEnable;     //0 – Use pre-defined LUT sequence index and number
                                    //1 - Use LUT sequence parameters provided in this block
  uint8_t   rsvd3[8];               //Reserved
  uint32_t  sflashA1Size;           //
  uint32_t  sflashA2Size;           //
  uint32_t  sflashB1Size;           //
  uint32_t  sflashB2Size;           //
  uint32_t  csPadSettingOverride;   //
  uint32_t  sclkPadSettingOverride; // 
  uint32_t  dataPadSettingOverride; // 
  uint32_t  dqsPadSettingOverride;  //  
  uint32_t  timeoutInMs;            //Maximum wait time during read busy status
  uint32_t  commandInterval;        //
  uint32_t  dataValidTime;          //Time from clock edge to data
                                    //[31:16] data valid time for DLLB in terms of 0.1 ns
                                    //[15:0] data valid time for DLLA in terms of 0.1 ns
  uint16_t  busyOffset;             //busy bit offset, valid range : 0-31
  uint16_t  busyBitPolarity;        //0 – busy bit is 1 if device is busy           
                                    //1 – busy bit is 0 if device is busy
  uint16_t  lookupTable[128];       //
  uint8_t   lutCustomSeq[48];       //  
  uint8_t   rsvd4[16];              //Reserved  
  uint32_t  pageSize;               //Page size in terms of bytes,not used by ROM
  uint32_t  sectorSize;             //Sector size in terms of bytes,not used by ROM
  uint32_t  ipCmdSerialClkFreq;     //Chip specific value, not used by ROM
  uint8_t   rsvd5[52];              //Reserved
} flexspi_nor_config_param_t;
#pragma pack()

#define FLEXSPI_CFG_VERSION 0x56010100

/*************************************
 *  IVT Data
 *************************************/
typedef struct __ivt {
    /** @ref hdr with tag #HAB_TAG_IVT, length and HAB version fields
     *  (see @ref data)
     */
    uint32_t hdr;
    /** Absolute address of the first instruction to execute from the
     *  image
     */
    uint32_t entry;
    /** Reserved in this version of HAB: should be NULL. */
    uint32_t reserved1;
    /** Absolute address of the image DCD: may be NULL. */
    uint32_t dcd;
    /** Absolute address of the Boot Data: may be NULL, but not interpreted
     *  any further by HAB
     */
    uint32_t boot_data;
    /** Absolute address of the IVT.*/
    uint32_t self;
    /** Absolute address of the image CSF.*/
    uint32_t csf;
    /** Reserved in this version of HAB: should be zero. */
    uint32_t reserved2;
} ivt;

#define IVT_MAJOR_VERSION           0x4
#define IVT_MAJOR_VERSION_SHIFT     0x4
#define IVT_MAJOR_VERSION_MASK      0xF
#define IVT_MINOR_VERSION           0x1
#define IVT_MINOR_VERSION_SHIFT     0x0
#define IVT_MINOR_VERSION_MASK      0xF

#define IVT_VERSION(major, minor)   \
  ((((major) & IVT_MAJOR_VERSION_MASK) << IVT_MAJOR_VERSION_SHIFT) |  \
  (((minor) & IVT_MINOR_VERSION_MASK) << IVT_MINOR_VERSION_SHIFT))


#define IVT_TAG_HEADER        (0xD1)       /**< Image Vector Table */
#define IVT_SIZE              (uint16_t)(((sizeof(ivt)&0x00FF) << 8) |			\
                                        ((sizeof(ivt)&0xFF00) >> 8) )
#define IVT_PAR               IVT_VERSION(IVT_MAJOR_VERSION, IVT_MINOR_VERSION)

#define IVT_HEADER          (IVT_TAG_HEADER | (IVT_SIZE << 8) | (IVT_PAR << 24))
#define IVT_RSVD            (uint32_t)(0x00000000)


/*************************************
 *  Boot Data
 *************************************/
typedef struct __boot_data{
  void * start;           /* boot start location */
  int size;               /* size */
  uint32_t plugin;        /* plugin flag - 1 if downloaded application is plugin */
  uint32_t placeholder;   /* placehoder to make even 0x10 size */
} boot_data_t;

/* External Variables */
extern const uint32_t device_config_data[];
extern const boot_data_t boot_data;
extern void __iar_program_start(void);

#endif //_FLEXSPI_BOOT_H_
