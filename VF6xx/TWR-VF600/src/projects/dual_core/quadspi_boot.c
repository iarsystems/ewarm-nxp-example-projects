#include <quadspi_boot.h>

#pragma location=".conf"
const SFLASH_CONFIGURATION_PARAM quadspi_conf = {
  0,                      /* DQS LoopBack */
  0,                      /* Reserved 1*/
  0,                      /* Reserved 2*/
  0,                      /* Reserved 3*/
  0,                      /* Reserved 4*/
  0,                      /* cs_hold_time */
  0,                      /* cs_setup_time */
  0x1000000,              /* A1 flash size */
  0,                      /* A2 flash size */
  0x1000000,              /* B1 flash size */
  0,                      /* B2 flash size */
  1,                      /* SCLK Freq - 60Mhz*/
  0,                      /* Reserved 5*/
  4,                      /* Quad Mode Flash */
  0,                      /* Port - Only A1 */
  0,                      /* DDR Mode Disable */
  0,                      /* DQS Disable */
  0,                      /* Parallel Mode Disable */
  0,                      /* Port A CS1 */
  0,                      /* Port B CS1 */
  0,                      /* FS Phase */
  0,                      /* FS Delay */
  0,                      /* DDR Sampling */
  /* LUT Programming */
  /* Quad read*/
  0x046B,
  0x0818,
  0x0C08,
  0x1E80,
  0x2400,
  0x0000
};

#pragma location=".ivt"
const ivt image_vector_table = {
  IVT_HEADER,                         /* IVT Header */
  (uint32_t)IMAGE_ENTRY_FUNCTION,     /* Image  Entry Function */
  IVT_RSVD,                           /* Reserved = 0 */
  (uint32_t)device_config_data,       /* Address where DCD information is stored */
  (uint32_t)&boot_data,        				/* Address where BOOT Data Structure is stored */
  (uint32_t)&image_vector_table,      /* Pointer to IVT Self (absolute address */
  (uint32_t)CSF_ADDRESS,              /* Address where CSF file is stored */
  IVT_RSVD                            /* Reserved = 0 */
};

const BOOT_DATA_T boot_data = {
  FLASH_BASE,                 /* boot start location */
  (FLASH_END-FLASH_BASE),     /* size */
  PLUGIN_FLAG,                /* Plugin flag*/
  0xFFFFFFFF  				  			/* empty - extra data word */
};


const uint32_t device_config_data[DCD_ARRAY_SIZE] = {
  /* DCD Header */
  (uint32_t)(DCD_TAG_HEADER |                       \
    (byte_swap16(sizeof(device_config_data)) << 8) | \
    (DCD_VERSION << 24)),
  /* DCD Commands */
  /* Add additional DCD Commands here */
};
