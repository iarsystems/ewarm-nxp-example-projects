/*-----------------------------------------------------------------------*/
/* MMCv3/SDv1/SDv2 (in SPI mode) control module  (C)ChaN, 2007           */
/*-----------------------------------------------------------------------*/
/* Only rcvr_spi(), xmit_spi(), disk_timerproc() and some macros         */
/* are platform dependent.                                               */
/*-----------------------------------------------------------------------*/


#include <string.h>
#include "ea3250_board.h"
#include "diskio.h"
#include "lpc32xx_sdcard_driver.h"
#include "lpc_sdmmc.h"


/* ----- MMC/SDC command ----- */                                                   
#define CMD0  (0)        /* GO_IDLE_STATE */
#define CMD1  (1)        /* SEND_OP_COND (MMC) */
#define CMD2  (2)        /* ALL_SEND_CID */
#define CMD3  (3)        /* SEND_RELATIVE_ADDR */
#define ACMD6  (6|0x80)    /* SET_BUS_WIDTH (SDC) */
#define CMD7  (7)        /* SELECT_CARD */
#define CMD8  (8)        /* SEND_IF_COND */
#define CMD9  (9)        /* SEND_CSD */
#define CMD10  (10)      /* SEND_CID */
#define CMD12  (12)      /* STOP_TRANSMISSION */
#define CMD13  (13)      /* SEND_STATUS */
#define ACMD13  (13|0x80)    /* SD_STATUS (SDC) */
#define CMD16  (16)      /* SET_BLOCKLEN */
#define CMD17  (17)      /* READ_SINGLE_BLOCK */
#define CMD18  (18)      /* READ_MULTIPLE_BLOCK */
#define  CMD23  (23)      /* SET_BLK_COUNT (MMC) */
#define  ACMD23  (23|0x80)    /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24  (24)      /* WRITE_BLOCK */
#define CMD25  (25)      /* WRITE_MULTIPLE_BLOCK */
#define  ACMD41  (41|0x80)    /* SEND_OP_COND (SDC) */
#define CMD55  (55)      /* APP_CMD */

/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

static volatile
DSTATUS Stat = STA_NOINIT;	/* Disk status */

static volatile
UNS_32 Timer1, Timer2;	/* 100Hz decrement timer */

static
BYTE CardType,      /* Card type flag */
   CardInfo[16+16+4];  /* CSD(16), CID(16), OCR(4) */

static
WORD CardRCA;      /* Assigned RCA */


DWORD get_fattime (void)
{
    return 0;
}

/* ####
 * Porting START
 * #### */


/* Operating condition register value */
#define OCRVAL OCR_VOLTAGE_RANGE_MSK /* around 3.1v */

/* SDMMC Block (sector) size in bytes */
#define SDMMC_BLK_SIZE    512

/* SDMMC OCR sequence clock speed - also the default clock speed of
   the bus whenever a new card is detected and configured */
#define SDMMC_OCR_CLOCK   390000

/* Normal clock speeds - the FIFOs may not be able to be maintained
   without using DMA at these clock speeds. */
#define SDMMC_NORM_CLOCK  800000 //5000000

#define INIT_OP_RETRIES   10  /* initial OP_COND retries */
#define SET_OP_RETRIES    200 /* set OP_COND retries */



static INT_32 sddev = 0;
static volatile INT_32 cmdresp, datadone;

/***********************************************************************
 *
 * Function: wait4cmddone
 *
 * Purpose: Sets command completion flag (callback)
 *
 * Processing:
 *     Sets the cmdresp flag to 1.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
 static void wait4cmddone(void)
 {
 	cmdresp = 1;
 }

/***********************************************************************
 *
 * Function: wait4datadone
 *
 * Purpose: Sets data completion flag (callback)
 *
 * Processing:
 *     Sets the datadone flag to 1.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
 static void wait4datadone(void)
 {
 	datadone = 1;
 }


static BOOL sdcard_init(SDC_PRMS_T* params)
{
  SDC_XFER_SETUP_T dataset;

	sddev = sdcard_open(SDCARD, 0);
	if (sddev == 0)
	{
		return FALSE;
	}

	/* Setup controller parameters */
	params->opendrain = TRUE;
	params->powermode = SD_POWER_ON_MODE;
	params->pullup0 = 1;
	params->pullup1 = 1;
	params->pullup23 = 1;
	params->pwrsave = FALSE;
	params->sdclk_rate = SDMMC_OCR_CLOCK;
	params->use_wide = FALSE;
	if (sdcard_ioctl(sddev, SD_SETUP_PARAMS,
		(INT_32) params) == _ERROR)
	{
//		blkdev_deinit();
		return FALSE;
	}

	/* Setup data transfer paramaters */
	dataset.data_callback = (PFV) wait4datadone;
	dataset.cmd_callback = (PFV) wait4cmddone;
	dataset.blocksize = SDMMC_BLK_SIZE;
	dataset.data_to = 0x001FFFFF; /* Long timeout for slow MMC cards */
	dataset.use_dma = FALSE;
	if (sdcard_ioctl(sddev, SD_SETUP_DATA_XFER, (INT_32) &dataset) != _NO_ERROR) {
    return FALSE;
  }
  
  return TRUE;  
}

static void sdmmc_cmd_setup(SD_CMD_T *pcmdsetup,
                            UNS_32 cmd,
                            UNS_32 arg,
                            UINT rt)
{
	INT_32 tmp = 0;

	/* Determine response size */
	switch (rt)
	{
		case 0:
			break;

		case 2:
			tmp = 136;
			break;

		default:
			tmp = 48;
			break;
	}

	/* Setup SD command structure */
	pcmdsetup->cmd = cmd;
	pcmdsetup->arg = arg;
	pcmdsetup->cmd_resp_size = tmp;
}

/* ####
 * Porting END
 * #### */

static
void bswap_cp (BYTE *dst, const DWORD *src)
{
  DWORD d;

  d = *src;
  *dst++ = (BYTE)(d >> 24);
  *dst++ = (BYTE)(d >> 16);
  *dst++ = (BYTE)(d >> 8);
  *dst++ = (BYTE)(d >> 0);
}


/*-----------------------------------------------------------------------*/
/* Power Control  (Platform dependent)                                   */
/*-----------------------------------------------------------------------*/
/* When the target system does not support socket power control, there   */
/* is nothing to do in these functions and chk_power always returns 1.   */

static
void power_on (void)
{
  ea3250_sdpower_enable(TRUE);
}

static
void power_off (void)
{
  ea3250_sdpower_enable(FALSE);
}

/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

static
int send_cmd (    /* Returns 1 when function succeeded otherwise returns 0 */
  UINT idx,    /* Command index (bit[5..0]), ACMD flag (bit7) */
  DWORD arg,    /* Command argument */
  UINT rt,    /* Expected response type. None(0), Short(1) or Long(2) */
  DWORD *buff    /* Response return buffer */
)
{
  SD_CMDDATA_T sdcmd;
  SD_CMDRESP_T resp;


  if (idx & 0x80) {        /* Send a CMD55 prior to the specified command if it is ACMD class */
    if (!send_cmd(CMD55, (DWORD)CardRCA << 16, 1, buff)  /* When CMD55 is faild, */
      || !(buff[0] & 0x00000020)) return 0;    /* exit with error */
  }
  idx &= 0x3F;          /* Mask out ACMD flag */
  
  sdmmc_cmd_setup(&sdcmd.cmd, idx, arg, rt);
  sdcmd.data.dataop = SD_DATAOP_NONE;

	cmdresp = 0;
	sdcard_ioctl(sddev, SD_ISSUE_CMD, (INT_32) &sdcmd);
	
  // TODO handle timeout...
  while (cmdresp == 0);

  sdcard_ioctl(sddev, SD_GET_CMD_RESP, (INT_32) &resp);

  if (idx == 41) {
    idx = idx;  
  }

  buff[0] = resp.cmd_resp[/*0*/1];      /* Read the response words */
  if (rt == 2) {
    buff[1] = resp.cmd_resp[/*1*/2];
    buff[2] = resp.cmd_resp[/*2*/3];
    buff[3] = resp.cmd_resp[/*3*/4];
  }

  return 1;    /* Return with success */
}


/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static
BYTE wait_ready (void)
{
  DWORD rc;

  Timer1 = 50;
  while (Timer1) {
    if (send_cmd(CMD13, (DWORD)CardRCA << 16, 1, &rc)
      && ((rc & 0x01E00) == 0x00800)) break;
    /* This loop will take a time. Insert rot_rdq() here for multitask envilonment. */
  }

  return Timer1 ? 1 : 0;
}

/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv		/* Physical drive nmuber (0) */
)
{
//	BYTE n, cmd, ty, ocr[4];
  UINT cmd, n;
  DWORD resp[4];
  BYTE ty;
  SDC_PRMS_T params;

  if (!sdcard_init(&params)) {
    return STA_NOINIT;
  }


	if (drv) return STA_NOINIT;			/* Supports only single drive */
	if (Stat & STA_NODISK) return Stat;	/* No card in the socket */

	power_on();							/* Force socket power on */
	for (Timer1 = 2; Timer1; );

  send_cmd(CMD0, 0, 0, NULL);       /* Enter idle state */
  CardRCA = 0;

  Timer1 = 10;
  while(Timer1);

  /*---- Card is 'idle' state ----*/

  Timer1 = 100;            /* Initialization timeout of 1000 msec */
  if (send_cmd(CMD8, 0x1AA, 1, resp)    /* SDC Ver2 */
    && (resp[0] & 0xFFF) == 0x1AA) {    /* The card can work at vdd range of 2.7-3.6V */
    do {                  /* Wait while card is busy state (use ACMD41 with HCS bit) */
      /* This loop will take a time. Insert wai_tsk(1) here for multitask envilonment. */
      if (!Timer1) goto di_fail;
    } while (!send_cmd(ACMD41, 0x40FF8000, 1, resp) || !(resp[0] & 0x80000000));
    ty = (resp[0] & 0x40000000) ? CT_SD2|CT_BLOCK : CT_SD2;  /* Check CCS bit in the OCR */
  }
  else {                  /* SDC Ver1 or MMC */
    if (send_cmd(ACMD41, /*0x00FF8000*/0, 1, resp)) {
      ty = CT_SD1; cmd = ACMD41;      /* ACMD41 is accepted -> SDC Ver1 */
    } else {
      ty = CT_MMC; cmd = CMD1;      /* ACMD41 is rejected -> MMC */
    }

    do {                  /* Wait while card is busy state (use ACMD41 or CMD1) */
      /* This loop will take a time. Insert wai_tsk(1) here for multitask envilonment. */
      if (!Timer1) {
        Timer1 = Timer1;
        goto di_fail;
      }
    } while (!send_cmd(cmd, 0x00FF8000, 1, resp) || !(resp[0] & 0x80000000));
    
  }

  CardType = ty;              /* Save card type */
  bswap_cp(&CardInfo[32], resp);      /* Save OCR */

  /*---- Card is 'ready' state ----*/

  if (!send_cmd(CMD2, 0, 2, resp)) goto di_fail;  /* Enter ident state */
  for (n = 0; n < 4; n++) bswap_cp(&CardInfo[n * 4 + 16], &resp[n]);  /* Save CID */

  /*---- Card is 'ident' state ----*/

  if (ty & CT_SDC) {            /* SDC: Get generated RCA and save it */
    if (!send_cmd(CMD3, 0, 1, resp)) goto di_fail;
    CardRCA = (WORD)(resp[0] >> 16);
  } else {                /* MMC: Assign RCA to the card */
    if (!send_cmd(CMD3, 1 << 16, 1, resp)) goto di_fail;
    CardRCA = 1;
  }

  /*---- Card is 'stby' state ----*/

  if (!send_cmd(CMD9, (DWORD)CardRCA << 16, 2, resp))    /* Get CSD and save it */
    goto di_fail;
  for (n = 0; n < 4; n++) bswap_cp(&CardInfo[n * 4], &resp[n]);

  if (!send_cmd(CMD7, (DWORD)CardRCA << 16, 1, resp))    /* Select card */
    goto di_fail;

  /*---- Card is 'tran' state ----*/

  if (!(ty & CT_BLOCK)) {    /* Set data block length to 512 (for byte addressing cards) */
    if (!send_cmd(CMD16, 512, 1, resp) || (resp[0] & 0xFDF90000))
      goto di_fail;
  }

  if (ty & CT_SDC) {
    // doesn't work...
	  //params.use_wide = TRUE;

	  params.opendrain = FALSE;
	  params.sdclk_rate = SDMMC_NORM_CLOCK;
	  sdcard_ioctl(sddev, SD_SETUP_PARAMS, (INT_32) &params);
  }

  Stat &= ~STA_NOINIT;  /* Clear STA_NOINIT */
  return Stat;

di_fail:
  power_off();
  Stat |= STA_NOINIT;    /* Set STA_NOINIT */
  return Stat;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0) */
)
{
	if (drv) return STA_NOINIT;		/* Supports only single drive */
	return Stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
  BYTE drv,      /* Physical drive nmuber (0) */
  BYTE *buff,      /* Pointer to the data buffer to store read data */
  DWORD sector,    /* Start sector number (LBA) */
  BYTE count      /* Sector count (1..127) */
)
{
  UINT cmd;
  SD_CMDDATA_T sdcmd;
  SD_CMDRESP_T resp;

  if (drv != 0 || count < 1 || count > 127) return RES_PARERR;  /* Check parameter */
  if (Stat & STA_NOINIT) return RES_NOTRDY;  /* Check drive status */

  if (!(CardType & CT_BLOCK)) sector *= 512; /* Convert LBA to byte address if needed */
  if (!wait_ready()) return RES_ERROR;       /* Make sure that card is tran state */

  cmd = (count > 1) ? CMD18 : CMD17;         /* Transfer type: Single block or Multiple block */

	sdcmd.data.dataop     = SD_DATAOP_READ;
	sdcmd.data.blocks     = count;
	sdcmd.data.buff       = (UNS_32*)buff;
	sdcmd.data.usependcmd = FALSE;
	sdcmd.data.stream     = FALSE; 
  
  sdmmc_cmd_setup(&sdcmd.cmd, cmd, sector, 1);

	datadone = 0;
	sdcard_ioctl(sddev, SD_ISSUE_CMD, (INT_32) &sdcmd);	
  // TODO handle timeout...
  while (datadone == 0);

  sdcard_ioctl(sddev, SD_GET_CMD_RESP, (INT_32) &resp); 
  
	if ((resp.data_status & SD_DATABLK_END) == 0)
	{
		return RES_ERROR;
	}

  return RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0

DRESULT disk_write (
  BYTE drv,      /* Physical drive nmuber (0) */
  const BYTE *buff,  /* Pointer to the data to be written */
  DWORD sector,    /* Start sector number (LBA) */
  BYTE count      /* Sector count (1..127) */
)
{
  DWORD rc;
  UINT cmd;
  SD_CMDDATA_T sdcmd;
  SD_CMDRESP_T resp;

  if (drv != 0 || count < 1 || count > 127) return RES_PARERR;  /* Check parameter */
  if (Stat & STA_NOINIT) return RES_NOTRDY;  /* Check drive status */
  if (Stat & STA_PROTECT) return RES_WRPRT;  /* Check write protection */

  if (!(CardType & CT_BLOCK)) sector *= 512;  /* Convert LBA to byte address if needed */
  if (!wait_ready()) return RES_ERROR;    /* Make sure that card is tran state */


  if (count == 1) {  /* Single block write */
    cmd = CMD24;
  } else {      /* Multiple block write */
    cmd = (CardType & CT_SDC) ? ACMD23 : CMD23;
    if (!send_cmd(cmd, count, 1, &rc)    /* Preset number of blocks to write */
      || (rc & 0xC0580000)) {
      return RES_ERROR;
    }
    cmd = CMD25;
  }

	sdcmd.data.dataop     = SD_DATAOP_WRITE;
	sdcmd.data.blocks     = count;
	sdcmd.data.buff       = (UNS_32*)buff;
	sdcmd.data.usependcmd = FALSE;
	sdcmd.data.stream     = FALSE; 
  
  sdmmc_cmd_setup(&sdcmd.cmd, cmd, sector, 1);

	datadone = 0;
	sdcard_ioctl(sddev, SD_ISSUE_CMD, (INT_32) &sdcmd);	
  // TODO handle timeout...
  while (datadone == 0);

  sdcard_ioctl(sddev, SD_GET_CMD_RESP, (INT_32) &resp); 
  
	if ((resp.data_status & SD_DATABLK_END) == 0)
	{
		return RES_ERROR;
	}

  if (cmd == CMD25 && (CardType & CT_SDC))  /* Terminate to write (SDC w/MB) */
    send_cmd(CMD12, 0, 1, &rc);

  return RES_OK;
}
#endif /* _READONLY == 0 */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL != 0
DRESULT disk_ioctl (
  BYTE drv,    /* Physical drive nmuber (0) */
  BYTE ctrl,    /* Control code */
  void *buff    /* Buffer to send/receive data block */
)
{
  DRESULT res;
  BYTE n, *p, *ptr = buff;
  DWORD csize, resp[4];


  if (drv) return RES_PARERR;
  if (Stat & STA_NOINIT) return RES_NOTRDY;

  res = RES_ERROR;

  switch (ctrl) {
    case CTRL_SYNC :  /* Make sure that all data has been written on the media */
      if (wait_ready())   /* Wait for card enters tarn state */
        res = RES_OK;
      break;

    case GET_SECTOR_COUNT :  /* Get number of sectors on the disk (DWORD) */
      p = &CardInfo[0];
      if ((p[0] >> 6) == 1) {  /* SDC ver 2.00 */
        csize = p[9] + ((WORD)p[8] << 8) + 1;
        *(DWORD*)buff = (DWORD)csize << 10;
      } else {          /* MMC or SDC ver 1.XX */
        n = (p[5] & 15) + ((p[10] & 128) >> 7) + ((p[9] & 3) << 1) + 2;
        csize = (p[8] >> 6) + ((WORD)p[7] << 2) + ((WORD)(p[6] & 3) << 10) + 1;
        *(DWORD*)buff = (DWORD)csize << (n - 9);
      }
      res = RES_OK;
      break;

    case GET_SECTOR_SIZE :  /* Get sectors on the disk (WORD) */
      *(WORD*)buff = 512;
      res = RES_OK;
      break;

    case GET_BLOCK_SIZE :  /* Get erase block size in unit of sectors (DWORD) */
      if (CardType & CT_SD2) {  /* SDC ver 2.00 */

/* TODO
        p = (BYTE*)DmaBuff[1];
        if (disk_ioctl(drv, MMC_GET_SDSTAT, p) == RES_OK) {
          *(DWORD*)buff = 16UL << (p[10] >> 4);
          res = RES_OK;
        }
*/
        res = RES_ERROR;
      } else {          /* SDC ver 1.XX or MMC */
        p = &CardInfo[0];
        if (CardType & CT_SD1)  /* SDC v1 */
          *(DWORD*)buff = (((p[10] & 63) << 1) + ((WORD)(p[11] & 128) >> 7) + 1) << ((p[13] >> 6) - 1);
        else          /* MMC */
          *(DWORD*)buff = ((WORD)((p[10] & 124) >> 2) + 1) * (((p[11] & 3) << 3) + ((p[11] & 224) >> 5) + 1);
        res = RES_OK;
      }
      break;

    case MMC_GET_TYPE :    /* Get card type flags (1 byte) */
      *ptr = CardType;
      res = RES_OK;
      break;

    case MMC_GET_CSD :    /* Get CSD as a data block (16 bytes) */
      memcpy(buff, &CardInfo[0], 16);
      res = RES_OK;
      break;

    case MMC_GET_CID :    /* Get CID as a data block (16 bytes) */
      memcpy(buff, &CardInfo[16], 16);
      res = RES_OK;
      break;

    case MMC_GET_OCR :    /* Get OCR (4 bytes) */
      memcpy(buff, &CardInfo[32], 4);
      res = RES_OK;
      break;

    case MMC_GET_SDSTAT :  /* Receive SD status as a data block (64 bytes) */
#if 0
      if (CardType & CT_SDC) {  /* SDC */
        if (wait_ready()) {
          ready_reception(1, 64);        /* Ready to receive data blocks */
          if (send_cmd(ACMD13, 0, 1, resp)  /* Start to read */
              && !(resp[0] & 0xC0580000)) {
            while ((XferWp == 0) && !(XferStat & 0xC));
            if (!(XferStat & 0xC)) {
              Store_Block(buff, DmaBuff[0], 64);
              res = RES_OK;
            }
          }
        }
        stop_transfer();          /* Close data path */
      }
#endif
      res = RES_ERROR;
      break;

    default:
      res = RES_PARERR;
  }

  return res;
}

#endif /* _USE_IOCTL != 0 */


/*-----------------------------------------------------------------------*/
/* Device Timer Interrupt Procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This function must be called in period of 10ms                        */

void disk_timerproc (void)
{
	static BYTE pv;
	BYTE s;
  UNS_32 n;


	n = Timer1;						/* 100Hz decrement timer */
	if (n) Timer1 = --n;
	n = Timer2;
	if (n) Timer2 = --n;

	n = pv;
  // TODO check if card is inserted
	//pv = SOCKPORT & (SOCKWP | SOCKINS);	/* Sample socket switch */
  //pv = GPIOGetValue( PORT2, 10);

	if (n == pv) {					/* Have contacts stabled? */
		s = Stat;

		/* write protect NOT supported */

		/* check card detect */
		if (pv)			       /* (Socket empty) */
			s |= (STA_NODISK | STA_NOINIT);
		else				       /* (Card inserted) */
			s &= ~STA_NODISK;

		Stat = s;
	}
}

