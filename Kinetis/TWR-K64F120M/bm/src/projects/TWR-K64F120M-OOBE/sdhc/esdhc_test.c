#include "common.h"

#include "esdhc.h"

#define ESDHC_BASE_CLK		120000000	/* 120Mhz */
#define ESDHC_RDWR_ADDRESS	SDRAM_ADDRESS
#define Store_Card_Reg		0x80001000

#define GPIO_PIN_FUN4		(4 << 8)

uint32_t base_rca = 0, SD_OCR = 0;
unsigned long SD_rca;

volatile int card_registry;
volatile int HCS;

unsigned long ResponseNormal;
unsigned long Response128HH;
unsigned long Response128HL;
unsigned long Response128LH;
unsigned long Response128LL;

unsigned char Current_State;
unsigned char Ready_For_Data;

void esdhc_portcfg(void)
{
	PORTE_PCR3 = 3 | GPIO_PIN_FUN4 | (1 << 6);	/* CMD */
	PORTE_PCR2 = 3 | GPIO_PIN_FUN4 | (1 << 6);	/* DCLK */
	PORTE_PCR1 = 3 | GPIO_PIN_FUN4 | (1 << 6);	/* DAT0 */
	PORTE_PCR0 = 3 | GPIO_PIN_FUN4 | (1 << 6);	/* DAT1 */
	PORTE_PCR5 = 3 | GPIO_PIN_FUN4 | (1 << 6);	/* DAT2 */
	PORTE_PCR4 = 3 | GPIO_PIN_FUN4 | (1 << 6);	/* DAT3 */
}

void Soft_Reset(void)
{
	SDHC_SYSCTL |= SDHC_SYSCTL_RSTA_MASK;

	/* wait sdhc to clear this bit automatically.. */
	while ((SDHC_SYSCTL & SDHC_SYSCTL_RSTA_MASK) != 0);
}

int esdhc_get_sdstb(void)
{
	if ((SDHC_PRSSTAT & SDHC_PRSSTAT_SDSTB_MASK) != 0)
		return (1);
	else
		return (0);
}

int esdhc_get_cardbusy(void)
{
	/* Check DLSL = 0x1 or 0x0 (busy)... */
	if ((SDHC_PRSSTAT & SDHC_PRSSTAT_DLSL(1)) == 0)
		return (1);	/* Card is Busy */
	else
		return (0);	/* Card is Free */
}

int esdhc_get_cdihb(void)
{
	if ((SDHC_PRSSTAT & SDHC_PRSSTAT_CDIHB_MASK) != 0)
		return (1);
	else
		return (0);
}

int esdhc_get_cihb(void)
{
	if ((SDHC_PRSSTAT & SDHC_PRSSTAT_CIHB_MASK) != 0)
		return (1);
	else
		return (0);
}

void esdhc_set_wml(uint16_t wrd_wml)
{
	SDHC_WML = (SDHC_WML_WRWML(wrd_wml)) | (SDHC_WML_RDWML(wrd_wml));
}

void esdhc_set_dtw(uint8_t dtw)
{
	SDHC_PROCTL |= SDHC_PROCTL_DTW(dtw);
}

void esdhc_set_blkattr(uint16_t blk_cnt, uint16_t blk_len)
{
	SDHC_BLKATTR = SDHC_BLKATTR_BLKCNT(blk_cnt) |
	    SDHC_BLKATTR_BLKSIZE(blk_len);
}

void esdhc_set_dma_addr(uint32_t dma_addr)
{
	/* Waiting for DLA was cleared by sdhc */
	while ((SDHC_PRSSTAT & SDHC_PRSSTAT_DLA_MASK) != 0);
	if ((SDHC_IRQSTAT & SDHC_IRQSTAT_TC_MASK) != 0)
		SDHC_IRQSTAT = SDHC_IRQSTAT_TC_MASK;	/* Clear TC before setting DMA Address */

	SDHC_DSADDR = dma_addr;
}

void esdhc_set_dmas(uint8_t dma_sel)
{
	SDHC_PROCTL |= SDHC_PROCTL_DMAS(dma_sel);
}

void esdhc_set_adms_addr(uint32_t adma_addr)
{
	/* Waiting for DLA was cleared by sdhc */
	while ((SDHC_PRSSTAT & SDHC_PRSSTAT_DLA_MASK) != 0);
	SDHC_ADSADDR = adma_addr & 0xfffffffc;
}

void set_init_clk(uint32_t clock)
{
	unsigned long sdrefclk, bestmatch = -1, temp, diff;
	int dvs, sdclkfs;
	int best_dvs, best_sdclkfs;
	volatile int tmp;
	unsigned short clk;

	/*
	 * First set the outdiv3 to min 1, then walk through all to
	 * get closer value with SDCLKDIV and DIV combination
	 */

	sdrefclk = ESDHC_BASE_CLK;

	for (sdclkfs = 2; sdclkfs < 257; sdclkfs <<= 1) {
		for (dvs = 1; dvs < 17; dvs++) {
			temp = sdrefclk / (sdclkfs * dvs);

			if (temp > clock)
				diff = temp - clock;
			else
				diff = clock - temp;

			if (diff <= bestmatch) {
				bestmatch = diff;
				best_sdclkfs = sdclkfs;
				best_dvs = dvs;

				if (bestmatch == 0)
					goto end;
			}
		}
	}

end:
	clk = ((best_sdclkfs >> 1) << 8) | ((best_dvs - 1) << 4);
	SDHC_SYSCTL = clk;
	SDHC_SYSCTL |= SDHC_SYSCTL_DTOCV(0xe);
	SDHC_SYSCTL |= SDHC_SYSCTL_SDCLKEN_MASK
	    | SDHC_SYSCTL_PEREN_MASK
	    | SDHC_SYSCTL_HCKEN_MASK | SDHC_SYSCTL_IPGEN_MASK;

	while (esdhc_get_sdstb() == 0);	/* waiting sd clock stable */
}

uint32_t Rd_Response48(void)
{
	/*
	 * bit 47:47 - 0,Start
	 * bit 46:46 - 0,Tx bit
	 * bit 45:40 - Command index
	 * bit 39:08 - Card Status / OCR / [RCA,Card Status]
	 * bit 07:01 - CRC7
	 * bit 00:00 - 1,Stop
	 *
	 * 47.........32 31..24 23..21  20..17       16 15.14   13      12     11   10.8 7....0
	 * <....> <....> <....> <.....  ........     .> <...     .       .      .   ...> <....>
	 * < CMD> <   Error   > <Erase> <State> <Ready> <Rsv> <AppCMD> <Rsv> <Err> <Rsv> <CRC7>
	 */

	ResponseNormal = SDHC_CMDRSP0;

#if (defined(DEBUG_INFO))
	printf("-- Response48 = %x \n", ResponseNormal);
#endif

	return ResponseNormal;
}

void Rd_Response128(void)
{
	/*
	 * bit 135:135 - 0,Start
	 * bit 134:134 - 0,Tx bit
	 * bit 133:128 - 111111,Rsv
	 * bit 127:001 - CID/CSD incl. CRC7
	 * bit 000:000 - 1,Stop
	 */

	Response128HH = SDHC_CMDRSP3;
	Response128HL = SDHC_CMDRSP2;
	Response128LH = SDHC_CMDRSP1;
	Response128LL = SDHC_CMDRSP0;

#if (defined(DEBUG_INFO))
	printf("-- Response128HH = %x\n ", Response128HH);
	printf("-- Response128HL = %x\n ", Response128HL);
	printf("-- Response128LH = %x\n ", Response128LH);
	printf("-- Response128LL = %x\n ", Response128LL);
#endif

	//printf("-- Response128: %x %x %x %x \n",
	//       Response128HH,Response128HL,Response128LH,Response128LL);
}

void CMD0_GO_IDLE_STATE(void)
{
#if (defined(DEBUG_INFO))
	printf("- Go_idle_cmd Start(0)\n");
#endif

	while (esdhc_get_cardbusy());	/* Wait Card free */
	while (esdhc_get_cihb() != 0);	/* waiting command line is available */

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	/* sending Go_idle_cmd */
	SDHC_CMDARG = 0x0;
	SDHC_XFERTYP = MMC_CMD0;
	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))
	       && (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

#if (defined(DEBUG_INFO))
	if (SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
		printf("- Go_idle_cmd done (0) \n");
	else
		printf("- Go_idle_cmd timeout \n");
#endif

	if ((SDHC_IRQSTAT & IRQ_ERROR_MASK) != 0x0)
		printf("- Error: CMD0_GO_IDLE_STATE SDHC_IRQSTAT  =%x \n",
		       SDHC_IRQSTAT);
}

void CMD8_SEND_EXTCSD(uint8_t* ext_csd_mem)
{
	printf("- Card Select cmd (8)\n");

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	/* sending card select */
	SDHC_CMDARG =  0x00;
	SDHC_XFERTYP =  MMC_CMD8
					|SDHC_XFERTYP_DPSEL_MASK
					|SDHC_XFERTYP_DTDSEL_MASK
					|SDHC_XFERTYP_AC12EN_MASK
					|SDHC_XFERTYP_BCEN_MASK
					|SDHC_XFERTYP_DMAEN_MASK;

	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK))&&(!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));
	//CMD12_STOP_TRANS();

	if(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK) {
		printf("- Card Select cmd (8) done\n");
	} else {
		printf("- Card Select cmd (8) timeout\n");
	}

	if((SDHC_IRQSTAT&IRQ_ERROR_MASK)!= 0x0)
		printf("- SDHC_IRQSTAT = %x\n",SDHC_IRQSTAT);

	Rd_Response48();

	printf("\t- mmcSCMD_SET             = %x\n", ext_csd_mem[504]);
	printf("\t- mmcBOOT_INFO            = %x\n", ext_csd_mem[228]);
	printf("\t- mmcBOOT_SIZE_MULTI      = %x\n", ext_csd_mem[226]);
	printf("\t- mmcACC_SIZE             = %x\n", ext_csd_mem[225]);
	printf("\t- mmcHC_ERASE_GRP_SIZE    = %x\n", ext_csd_mem[224]);
	printf("\t- mmcERASE_TIMEOUT_MULT   = %x\n", ext_csd_mem[223]);
	printf("\t- mmcREL_WR_SEC_C         = %x\n", ext_csd_mem[222]);
	printf("\t- mmcHC_WP_GRP_SIZE       = %x\n", ext_csd_mem[221]);
	printf("\t- mmcS_C_VCC              = %x\n", ext_csd_mem[220]);
	printf("\t- mmcS_C_VCCQ             = %x\n", ext_csd_mem[219]);
	printf("\t- mmcS_A_TIMEOUT          = %x\n", ext_csd_mem[217]);
	printf("\t- mmcSEC_COUNT            = %x\n",
		   ((ext_csd_mem[215] << 24) + (ext_csd_mem[214] << 16) + (ext_csd_mem[213] << 8) + ext_csd_mem[212]));
	printf("\t- mmcMIN_PREF_W_8_52      = %x\n", ext_csd_mem[210]);
	printf("\t- mmcMIN_PREF_R_8_52      = %x\n", ext_csd_mem[209]);
	printf("\t- mmcMIN_PREF_W_8_26_4_52 = %x\n", ext_csd_mem[208]);
	printf("\t- mmcMIN_PREF_R_8_26_4_52 = %x\n", ext_csd_mem[207]);
	printf("\t- mmcMIN_PREF_W_4_26      = %x\n", ext_csd_mem[206]);
	printf("\t- mmcMIN_PREF_R_4_26      = %x\n", ext_csd_mem[205]);
	printf("\t- mmcPWR_CL_26_360        = %x\n", ext_csd_mem[203]);
	printf("\t- mmcPWR_CL_52_360        = %x\n", ext_csd_mem[202]);
	printf("\t- mmcPWR_CL_26_195        = %x\n", ext_csd_mem[201]);
	printf("\t- mmcPWR_CL_52_195        = %x\n", ext_csd_mem[200]);
	printf("\t- mmcCARD_TYPE            = %x\n", ext_csd_mem[196]);
	printf("\t- mmcCSD_STRUCTURE        = %x\n", ext_csd_mem[194]);
	printf("\t- mmcEXT_CSD_REV          = %x\n", ext_csd_mem[192]);
	printf("\t- mmcCMD_SET              = %x\n", ext_csd_mem[191]);
	printf("\t- mmcCMD_SET_REV          = %x\n", ext_csd_mem[189]);
	printf("\t- mmcPOWER_CLASS          = %x\n", ext_csd_mem[187]);
	printf("\t- mmcHS_TIMING            = %x\n", ext_csd_mem[185]);
	printf("\t- mmcBUS_WIDTH            = %x\n", ext_csd_mem[183]);
	printf("\t- mmcERASED_MEM_COUNT     = %x\n", ext_csd_mem[181]);
	printf("\t- mmcBOOT_CONFIG          = %x\n", ext_csd_mem[179]);
	printf("\t- mmcBOOT_BUS_WIDTH       = %x\n", ext_csd_mem[177]);
	printf("\t- mmcERASE_GROUP_DEF      = %x\n", ext_csd_mem[175]);
}

uint32_t CMD8_SEND_IFCOND0(void)
{
#if (defined(DEBUG_INFO))
	printf("- SEND_IF_COND (8)");
#endif

	while (esdhc_get_cardbusy());	/* Wait Card free */
	while (esdhc_get_cihb() != 0);	/* waiting command line is available */

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	/* sending send_rca */
	SDHC_CMDARG = CHECK_PATTEN | VHS;
	SDHC_XFERTYP = SD_CMD8;
	while (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK));

#if (defined(DEBUG_INFO))
	if (SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
		printf("-- SEND_IF_COND (8)\n");
	else
		printf("-- SEND_IF_COND (8) timeout \n");
#endif

	if ((SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK) == SDHC_IRQSTAT_CTOE_MASK)
		printf("-- Ver1.X SD Memory Card or not SD Card\n");
	else
		printf("-- Ver2.00 or later SD Memory Card\n");

	return Rd_Response48();
}

void CMD9_SEND_CSD(uint32_t card_rca)
{
#if 0
	unsigned char CSD_Structure;
	unsigned char SpecVer;
	unsigned char TAAC;
	unsigned char NSAC;
	unsigned char TranSpeed;
	unsigned short CCC;
	unsigned char ReadBlLen;
	unsigned char ReadBlPartial;
	unsigned char WriteBlockMisalign;
	unsigned char ReadBlockMisalign;
	unsigned char DsrImp;
	unsigned short CSize;
	unsigned char VddRdCurrMin;
	unsigned char VddRdCurrMax;
	unsigned char VddWrCurrMin;
	unsigned char VddWrCurrMax;
	unsigned char CSizeMult;
	unsigned char EraseGrpSize;
	unsigned char EraseGrpMult;
	unsigned char WpGrpSize;
	unsigned char WpGrpEnable;
	unsigned char DefaultECC;
	unsigned char R2WFactor;
	unsigned char WrBlLen;
	unsigned char WriteBlPartial;
	unsigned char ContentProtApp;
	unsigned char FileFormatGrp;
	unsigned char Copy;
	unsigned char PermWriteProtect;
	unsigned char TempWriteProtect;
	unsigned char FileFormat;
	unsigned char ECC;
	unsigned char CRC;
#endif

#if (defined(DEBUG_INFO))
	printf("- send CSD cmd start (9)\n");
#endif

	while (esdhc_get_cardbusy());	/* Wait Card free */
	while (esdhc_get_cihb() != 0);	/* waiting command line is available */

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	SDHC_CMDARG = card_rca;
	SDHC_XFERTYP = MMC_CMD9;
	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)) &&
	       (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

#if (defined(DEBUG_INFO))
	if (SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
		printf("- send CSD cmd done (9)\n");
	else
		printf("- send CSD cmd timeout \n");
#endif

	if ((SDHC_IRQSTAT & IRQ_ERROR_MASK) != 0x0)
		printf("- Error: CMD9_SEND_CSD SDHC_IRQSTAT  =%x \n",
		       SDHC_IRQSTAT);

	Rd_Response128();

#if 0
	printf("CSD_Structure %x\n", ((Response128HH & 0xC0000000) >> 30));
	printf("SpecVer %x\n", ((Response128HH & 0x3C000000) >> 26));
	printf("TAAC %x\n", ((Response128HH & 0x00FF0000) >> 16));
	printf("NSAC %x\n", ((Response128HH & 0x0000FF00) >> 8));
	printf("TranSpeed %x\n", ((Response128HH & 0x000000FF) >> 0));
	printf("CCC %x\n", ((Response128HL & 0xFFF00000) >> 20));
	printf("ReadBlLen %x\n", ((Response128HL & 0x000F0000) >> 16));
	printf("ReadBlPartial %x\n", ((Response128HL & 0x00008000) >> 15));
	printf("WriteBlockMisalign %x\n", ((Response128HL & 0x00004000) >> 14));
	printf("ReadBlockMisalign %x\n", ((Response128HL & 0x00002000) >> 13));
	printf("DsrImp %x\n", ((Response128HH & 0x00001000) >> 12));
	printf("CSize %x\n",
	       (((Response128LH & 0xC0000000) >> 30) |
		((Response128HL & 0x000003FF) << 2)));
	printf("VddRdCurrMin %x\n", ((Response128LH & 0x38000000) >> 27));
	printf("VddRdCurrMax %x\n", ((Response128LH & 0x07000000) >> 24));
	printf("VddWrCurrMin %x\n", ((Response128LH & 0x00E00000) >> 21));
	printf("VddWrCurrMax %x\n", ((Response128LH & 0x001C0000) >> 18));
	printf("CSizeMult %x\n", ((Response128LH & 0x00038000) >> 15));
	printf("EraseGrpSize %x\n", ((Response128LH & 0x00007C00) >> 10));
	printf("EraseGrpMult %x\n", ((Response128LH & 0x000003E0) >> 5));
	printf("WpGrpSize %x\n", ((Response128LL & 0x80000000) >> 31));
	printf("WpGrpEnable %x\n", ((Response128LL & 0x80000000) >> 31));
	printf("DefaultECC %x\n", ((Response128LL & 0x60000000) >> 29));
	printf("R2WFactor %x\n", ((Response128LL & 0x1C000000) >> 26));
	printf("WrBlLen %x\n", ((Response128LL & 0x03C00000) >> 22));
	printf("WriteBlPartial %x\n", ((Response128LL & 0x00200000) >> 21));
	printf("ContentProtApp %x\n", ((Response128LL & 0x00010000) >> 16));
	printf("FileFormatGrp %x\n", ((Response128LL & 0x00008000) >> 15));
	printf("Copy %x\n", ((Response128LL & 0x00004000) >> 14));
	printf("PermWriteProtect %x\n", ((Response128LL & 0x00002000) >> 13));
	printf("TempWriteProtect %x\n", ((Response128LL & 0x00001000) >> 12));
	printf("FileFormat %x\n", ((Response128LL & 0x00000C00) >> 10));
	printf("ECC %x\n", ((Response128LL & 0x00000300) >> 8));
	printf("CRC %x\n", ((Response128LL & 0x000000FE) >> 1));
	//#else
	CSD_Structure = (uint8_t) ((Response128HH & 0xC0000000) >> 30);
	SpecVer = (uint8_t) ((Response128HH & 0x3C000000) >> 26);
	TAAC = (uint8_t) ((Response128HH & 0x00FF0000) >> 16);
	NSAC = (uint8_t) ((Response128HH & 0x0000FF00) >> 8);
	TranSpeed = (uint8_t) ((Response128HH & 0x000000FF) >> 0);
	CCC = (uint16_t) ((Response128HL & 0xFFF00000) >> 20);
	ReadBlLen = (uint8_t) ((Response128HL & 0x000F0000) >> 16);
	ReadBlPartial = (uint8_t) ((Response128HL & 0x00008000) >> 15);
	WriteBlockMisalign = (uint8_t) ((Response128HL & 0x00004000) >> 14);
	ReadBlockMisalign = (uint8_t) ((Response128HL & 0x00002000) >> 13);
	DsrImp = (uint8_t) ((Response128HH & 0x00001000) >> 12);
	CSize =
	    (uint16_t) (((Response128LH & 0xC0000000) >> 30) |
			((Response128HL & 0x000003FF) << 2));
	VddRdCurrMin = (uint8_t) ((Response128LH & 0x38000000) >> 27);
	VddRdCurrMax = (uint8_t) ((Response128LH & 0x07000000) >> 24);
	VddWrCurrMin = (uint8_t) ((Response128LH & 0x00E00000) >> 21);
	VddWrCurrMax = (uint8_t) ((Response128LH & 0x001C0000) >> 18);
	CSizeMult = (uint8_t) ((Response128LH & 0x00038000) >> 15);
	EraseGrpSize = (uint8_t) ((Response128LH & 0x00007C00) >> 10);
	EraseGrpMult = (uint8_t) ((Response128LH & 0x000003E0) >> 5);
	WpGrpSize = (uint8_t) ((Response128LH & 0x0000001F) >> 0);
	WpGrpEnable = (uint8_t) ((Response128LL & 0x80000000) >> 31);
	DefaultECC = (uint8_t) ((Response128LL & 0x60000000) >> 29);
	R2WFactor = (uint8_t) ((Response128LL & 0x1C000000) >> 26);
	WrBlLen = (uint8_t) ((Response128LL & 0x03C00000) >> 22);
	WriteBlPartial = (uint8_t) ((Response128LL & 0x00200000) >> 21);
	ContentProtApp = (uint8_t) ((Response128LL & 0x00010000) >> 16);
	FileFormatGrp = (uint8_t) ((Response128LL & 0x00008000) >> 15);
	Copy = (uint8_t) ((Response128LL & 0x00004000) >> 14);
	PermWriteProtect = (uint8_t) ((Response128LL & 0x00002000) >> 13);
	TempWriteProtect = (uint8_t) ((Response128LL & 0x00001000) >> 12);
	FileFormat = (uint8_t) ((Response128LL & 0x00000C00) >> 10);
	ECC = (uint8_t) ((Response128LL & 0x00000300) >> 8);
	CRC = (uint8_t) ((Response128LL & 0x000000FE) >> 1);
#endif
}

uint32_t CMD3_SD_GET_RCA()
{
#if (defined(DEBUG_INFO))
	printf("- send_rca cmd start (3)");
#endif

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	/* sending send_rca */
	SDHC_CMDARG = 0x0;
	SDHC_XFERTYP = MMC_CMD3;
	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)) &&
	       (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

#if (defined(DEBUG_INFO))
	if (SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
		printf("\tsend_rca cmd done (3) \n");
	else
		printf("\tsend_rca cmd timeout \n");
#endif

	if ((SDHC_IRQSTAT & IRQ_ERROR_MASK) != 0x0)
		printf("CMD3_SD_GET_RCA SDHC_IRQSTAT =%x \n", SDHC_IRQSTAT);

	return Rd_Response48();
}

void CMD55_APP_CMD(uint32_t card_rca)
{
#if (defined(DEBUG_INFO))
	printf("- APP CMD (55)");
#endif

	while (esdhc_get_cardbusy());	/* Wait Card free */
	while (esdhc_get_cihb() != 0);	/* waiting command line is available */

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	/* sending send_rca */
	SDHC_CMDARG = card_rca;
	SDHC_XFERTYP = SD_CMD55;
	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)) &&
	       (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

#if (defined(DEBUG_INFO))
	if (SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
		printf("\tAPP CMD (55) \n");
	else
		printf("\tAPP CMD (55) timeout \n");
#endif

	if ((SDHC_IRQSTAT & IRQ_ERROR_MASK) != 0x0)
		printf("Error: CMD55_APP_CMD SDHC_IRQSTAT  =%x \n",
		       SDHC_IRQSTAT);

	Rd_Response48();
}

void CMD7_CARD_SELECT(uint32_t card_rca)
{
#if (defined(DEBUG_INFO))
	printf("- Card Select cmd (7)!\n");
#endif

	while (esdhc_get_cardbusy());	/* Wait Card free */
	while (esdhc_get_cihb() != 0);	/* waiting command line is available */

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	/* sending card select */
	SDHC_CMDARG = card_rca;
	SDHC_XFERTYP = MMC_CMD7;
	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)) &&
	       (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

#if (defined(DEBUG_INFO))
	if (SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
		printf("\tCard Select cmd (7) done \n");
	else
		printf("\tCard Select cmd (7) timeout \n");
#endif

	if ((SDHC_IRQSTAT & IRQ_ERROR_MASK) != 0x0)
		printf("Error: CMD7_CARD_SELECT SDHC_IRQSTAT  =%x \n",
		       SDHC_IRQSTAT);

	Rd_Response48();
}

void CMD2_ALL_SEND_CID(void)
{
#if (defined(DEBUG_INFO))
	printf("- all_send_CID cmd start (2)\n");
#endif

	while (esdhc_get_cardbusy());	/* Wait Card free */
	while (esdhc_get_cihb() != 0);	/* waiting command line is available */

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	/* sending all_send_CID */
	SDHC_CMDARG = 0x0;
	SDHC_XFERTYP = MMC_CMD2;
	while (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK));

#if (defined(DEBUG_INFO))
	if (SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
		printf("\tall_send_CID cmd done (2) \n");
	else
		printf("\tall_send_CID cmd timeout \n");
#endif

	if (SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)
		printf("-- All send CID command Done!\n");

	Rd_Response128();
}

uint32_t CMD13_SEND_STATUS(uint32_t card_rca)
{
	uint32_t status_reg;

#if (defined(DEBUG_INFO))
	printf("- card status cmd start (13) \n");
#endif

	while (esdhc_get_cardbusy());	/* Wait Card free */
	while (esdhc_get_cihb() != 0);	/* waiting command line is available */

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	SDHC_CMDARG = card_rca;
	SDHC_XFERTYP = MMC_CMD13 |
	    SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK;

	while ((SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK) != SDHC_IRQSTAT_CC_MASK);

	while ((SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK) ==
	       SDHC_IRQSTAT_CTOE_MASK) {
		SDHC_IRQSTAT = 0xFFFFFFFF;

		SDHC_CMDARG = card_rca;
		SDHC_XFERTYP = MMC_CMD13 |
		    SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK;
	}

#if (defined(DEBUG_INFO))
	if (SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
		printf("\tcard status cmd done (13) \n");
	else
		printf("\tcard status cmd timeout \n");
#endif

	if ((SDHC_IRQSTAT & IRQ_ERROR_MASK) != 0x0)
		printf("Error: CMD13_SEND_STATUS SDHC_IRQSTAT  =%x \n",
		       SDHC_IRQSTAT);

	status_reg = Rd_Response48();
	Current_State = (uint8_t) (((status_reg) >> 9) & 0x0f);
	Ready_For_Data = (uint8_t) (((status_reg) >> 8) & 0x01);
#if 0
	unsigned char Add_OutOfRange;
	unsigned char Add_Misalign;
	unsigned char Blk_Len_Err;
	unsigned char Erase_Seq_Err;
	unsigned char Erase_Param;
	unsigned char Wp_Violation;
	unsigned char Card_Is_Locked;
	unsigned char Lock_Unlock_Failed;
	unsigned char Com_CRC_Err;
	unsigned char Illegal_Cmd;
	unsigned char Card_Ecc_Fail;
	unsigned char CC_Err;
	unsigned char Err;
	unsigned char UndErrun;
	unsigned char OverRun;
	unsigned char CidCsd_Overwrite;
	unsigned char WP_Erase_Skip;
	unsigned char Erase_Reset;
	unsigned char Current_State;
	unsigned char Ready_For_Data;
	unsigned char Switch_Err;
	unsigned char App_Cmd;

	Add_OutOfRange = (uint8_t) (((status_reg) >> 31) & 0x01);
	Add_Misalign = (uint8_t) (((status_reg) >> 30) & 0x01);
	Blk_Len_Err = (uint8_t) (((status_reg) >> 29) & 0x01);
	Erase_Seq_Err = (uint8_t) (((status_reg) >> 28) & 0x01);
	Erase_Param = (uint8_t) (((status_reg) >> 27) & 0x01);
	Wp_Violation = (uint8_t) (((status_reg) >> 26) & 0x01);
	Card_Is_Locked = (uint8_t) (((status_reg) >> 25) & 0x01);
	Lock_Unlock_Failed = (uint8_t) (((status_reg) >> 24) & 0x01);
	Com_CRC_Err = (uint8_t) (((status_reg) >> 23) & 0x01);
	Illegal_Cmd = (uint8_t) (((status_reg) >> 22) & 0x01);
	Card_Ecc_Fail = (uint8_t) (((status_reg) >> 21) & 0x01);
	CC_Err = (uint8_t) (((status_reg) >> 20) & 0x01);
	Err = (uint8_t) (((status_reg) >> 19) & 0x01);
	UndErrun = (uint8_t) (((status_reg) >> 18) & 0x01);
	OverRun = (uint8_t) (((status_reg) >> 17) & 0x01);
	CidCsd_Overwrite = (uint8_t) (((status_reg) >> 16) & 0x01);
	WP_Erase_Skip = (uint8_t) (((status_reg) >> 15) & 0x01);
	Erase_Reset = (uint8_t) (((status_reg) >> 13) & 0x01);
	Current_State = (uint8_t) (((status_reg) >> 9) & 0x0f);
	Ready_For_Data = (uint8_t) (((status_reg) >> 8) & 0x01);
	Switch_Err = (uint8_t) (((status_reg) >> 7) & 0x01);
	App_Cmd = (uint8_t) (((status_reg) >> 5) & 0x01);
#endif
	return status_reg;
}

void wait_card_inTran(uint32_t card_addr)
{
	CMD13_SEND_STATUS(card_addr);
	while (Current_State != Tran) {
		CMD13_SEND_STATUS(card_addr);
	}
}

void wait_wdata_tc()
{
	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)) &&
	       (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));
	while ((SDHC_PRSSTAT & SDHC_PRSSTAT_WTA_MASK));	/* Write transfer active */
	while (!(SDHC_IRQSTAT & SDHC_IRQSTAT_TC_MASK));
}

void CMD25_MultiWrite_Block2(uint32_t card_addr)
{
#if (defined(DEBUG_INFO))
	printf("\tmultiple write block cmd (25)with DMA \n");
#endif

	//wait_card_inTran(card_addr); /* wait card in trans state! */
	//wait_card_inTran(card_addr); /* wait card in trans state! */
	while (esdhc_get_cihb() != 0);	/* waiting command line is available */
	while (esdhc_get_cdihb() != 0);	/* waiting data line is available, it doesn't work for waitting card out of busy. */

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	SDHC_CMDARG = card_addr;

	SDHC_XFERTYP = MMC_CMD25
	    | SDHC_XFERTYP_DPSEL_MASK
	    | SDHC_XFERTYP_CICEN_MASK
	    | SDHC_XFERTYP_CCCEN_MASK
	    | SDHC_XFERTYP_MSBSEL_MASK
	    | SDHC_XFERTYP_AC12EN_MASK
	    | SDHC_XFERTYP_BCEN_MASK | SDHC_XFERTYP_DMAEN_MASK;

	wait_wdata_tc();	/* write transfer active */

	if ((SDHC_IRQSTAT & IRQ_ERROR_MASK) != 0x0)
		printf("Error: CMD25_MultiWrite_Block2 SDHC_IRQSTAT  =%x \n",
		       SDHC_IRQSTAT);

	Rd_Response48();
}

uint32_t ACMD41_SEND_OPCON(uint32_t args)
{
	CMD55_APP_CMD(0x0);

#if (defined(DEBUG_INFO))
	printf("- SD_SEND_OPCON(A41)");
#endif

	while (esdhc_get_cardbusy());	/* Wait Card free */
	while (esdhc_get_cihb() != 0);	/* waiting command line is available */

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	/* sending send_rca */
	SDHC_CMDARG = args;
	SDHC_XFERTYP = SD_ACMD41;
	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)) &&
	       (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

#if (defined(DEBUG_INFO))
	if (SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
		printf("\tSD_SEND_OPCON(A41) Done \n");
	else
		printf("\tSD_SEND_OPCON(A41) Timeout \n");
#endif

	if ((SDHC_IRQSTAT & IRQ_ERROR_MASK) != 0x0)
		printf("Error: ACMD41_SEND_OPCON SDHC_IRQSTAT  =%x \n",
		       SDHC_IRQSTAT);

	return Rd_Response48();
}

void ACMD13_SD_STATUS(uint32_t card_rca, uint8_t bus_width)
{
	uint16_t blk_len = 0;
	uint16_t blk_cnt = 0;
	uint32_t blk_size = 0;
	uint8_t *sd_status;

	sd_status = ((uint8_t *) Store_Card_Reg);

	blk_cnt = 1;
	blk_len = 0x40;		/* 512 bit SD_Status data */
	blk_size = blk_cnt * blk_len;	/* byte base */

#if (defined(DEBUG_INFO))
	printf("- ACMD13 SD status (13)");
#endif

	esdhc_set_wml(WML_WML0);
	esdhc_set_dtw(bus_width);
	esdhc_set_blkattr(1, blk_len);
	esdhc_set_dma_addr(Store_Card_Reg);

	//Clear_Memory((uint32_t*)Store_Card_Reg, blk_size);
	//memset((uint32_t*)Store_Card_Reg, 0, blk_size);

	CMD55_APP_CMD(card_rca);

	while (esdhc_get_cardbusy());	/* Wait Card free */
	while (esdhc_get_cihb());		/* Wait CMD line free */

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	/* switch setting bus width */
	SDHC_CMDARG = 0x00;	/* stuff bits */
	SDHC_XFERTYP = SD_ACMD13
	    | SDHC_XFERTYP_DPSEL_MASK
	    | SDHC_XFERTYP_DTDSEL_MASK
	    | SDHC_XFERTYP_AC12EN_MASK
	    | SDHC_XFERTYP_BCEN_MASK | SDHC_XFERTYP_DMAEN_MASK;
	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)) &&
	       (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

#if (defined(DEBUG_INFO))
	if (SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
		printf("\tAcmd done (13)\n");
	else
		printf("\tAcmd13 timeout\n");
#endif

	if ((SDHC_IRQSTAT & IRQ_ERROR_MASK) != 0x0)
		printf("Error: ACMD13_SD_STATUS SDHC_IRQSTAT  =%x \n",
		       SDHC_IRQSTAT);

	Rd_Response48();

#if 0
	unsigned char sdDat_Bus_Width;
	unsigned char sdSecured_Mode;
	unsigned char sdSd_Card_Type;
	unsigned char sdSize_Pro_EdArea;
	unsigned char sdSpeed_Class;
	unsigned char sdPreFormance_Move;
	unsigned char sdAu_Size;
	unsigned char sdErase_Size;
	unsigned char sdErase_Timeout;
	unsigned char sdErase_Offset;

	sdDat_Bus_Width = ((sd_status[0]) >> 6) & 0x3;
	sdSecured_Mode = ((sd_status[1]) >> 5) & 0x1;
	sdSd_Card_Type = sd_status[60] & 0xf;
	sdSize_Pro_EdArea = sd_status[56] & 0x7;
	sdSpeed_Class = sd_status[55] & 0x7;
	sdPreFormance_Move = sd_status[54] & 0x7;
	sdAu_Size = ((sd_status[53]) >> 2) & 0x3;
	sdErase_Size = sd_status[51] & 0xf;
	sdErase_Timeout = ((sd_status[50]) >> 2) & 0x3;
	sdErase_Offset = sd_status[50] & 0x3;
#endif
}

void ACMD6_SWITCH(uint32_t card_rca, uint8_t bus_width)
{
#if (defined(DEBUG_INFO))
	printf("- Switch setting command (6)\n");
#endif

	CMD55_APP_CMD(card_rca);

	while (esdhc_get_cardbusy());	/* Wait Card free */
	while (esdhc_get_cihb());		/* Wait CMD line free */

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	/* switch setting bus width */
	if (bus_width == DTW_4BIT)
		SDHC_CMDARG = 0x2;	/* bus_width; */
	else
		SDHC_CMDARG = 0x0;	/* bus_width; */

	SDHC_XFERTYP = SD_ACMD6;
	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)) &&
	       (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

#if (defined(DEBUG_INFO))
	if (SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)
		printf("\tAcmd done (6)\n");
	else
		printf("\tAcmd timeout\n");
#endif

	if ((SDHC_IRQSTAT & IRQ_ERROR_MASK) != 0x0)
		printf("Error: ACMD6_SWITCH  SDHC_IRQSTAT  =%x \n",
		       SDHC_IRQSTAT);

	Rd_Response48();
}

int label_card_type(void)
{
	int HCS = 0;

	/* sending Go_idle_cmd */
	CMD0_GO_IDLE_STATE();

	/* Label SD Card */
#if (defined(DEBUG_INFO))
	printf("\tSD send SEND_IF_COND (8)");
#endif

	if (CMD8_SEND_IFCOND0() == 0)
		HCS = 0;
	else
		HCS = 1;

	CMD55_APP_CMD(0x0);

#if (defined(DEBUG_INFO))
	printf("- SD_SEND_OPCON(A41)");
#endif

	/* Clear Command Complete bit */
	SDHC_IRQSTAT = 0xFFFFFFFF;

	/* sending send_rca */
	SDHC_CMDARG = (HCS << 30);
	SDHC_XFERTYP = SD_ACMD41;
	while ((!(SDHC_IRQSTAT & SDHC_IRQSTAT_CC_MASK)) &&
	       (!(SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)));

	if ((SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK) == 0x0) {
		printf("-- SD Card is accepted\n");
		return (SD_CARD);
	}
	return -1;
}

void Init_SD_Card(uint8_t dat_width)
{
	int card_num = 0;
	card_registry = CARD_REGISTRY_ON;

	printf("- Start to initialize SD Card.... \n");

	/* sending Go_idle_cmd */
	//CMD0_GO_IDLE_STATE();

	SD_OCR = ACMD41_SEND_OPCON((base_rca | HCS << 30));

	while (1) {
		if ((ACMD41_SEND_OPCON((SD_OCR | HCS << 30)) & SD_BUSY) != 0x0)	/* SD stay at busy state when =0x0 */
			break;
	}

	/* Card Registry (Don't support on Palladium MMC model) */
	while (card_registry == CARD_REGISTRY_ON) {
		/* all send CID */
		CMD2_ALL_SEND_CID();

		if ((SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK) ==
		    SDHC_IRQSTAT_CTOE_MASK) {
			card_registry = CARD_REGISTRY_OFF;

			printf("- There are %1d Cards on the SD bus!\n",
			       card_num + 1);
			printf("- Card Registry is Done!\n");
		} else {
			SD_rca = (CMD3_SD_GET_RCA() & 0xFFFF0000);
			printf("-- Card RCA =%x \n", SD_rca);

			/* Check CSD & CID */
			CMD9_SEND_CSD(SD_rca);

			/* card_num ++; */
		}
	}

	base_rca = SD_rca;

	/* send card status register */
	CMD13_SEND_STATUS(base_rca);
	if (Current_State == Stby || Ready_For_Data == Ready) {
		printf("- SD card is in standby state now!\n");
	}
	/* card select */
	CMD7_CARD_SELECT(base_rca);

	/* Set SD Clock */
	if (dat_width != DTW_1BIT) {
		set_init_clk(50000000);	/* 50MHz */
	} else
		set_init_clk(25000000);	/* 25MHz */

	/* Set bit mode */
	if (dat_width == DTW_4BIT) {
		/* Set 4bit Mode */
		ACMD6_SWITCH(base_rca, DTW_4BIT);
	} else if (dat_width == DTW_1BIT) {
		/* Set 1bit Mode */
		ACMD6_SWITCH(base_rca, DTW_1BIT);
		//printf("Default 1 bit bus width!!\n\n");
		//ACMD13_SD_STATUS(base_rca, DTW_1BIT);
	} else
		printf("- Error: Please set the data bus width!!\n");

	ACMD13_SD_STATUS(base_rca, dat_width);

	printf("- SD card initialization done!\n\n");
}

int Init_card(uint32_t base_rca, uint8_t dat_width)
{
	/* Config P2 SDHC related Pad */
	Soft_Reset();		/* Reset SDHC */

	set_init_clk(25000000);

	printf("- Initiate 25MHz communication\n");
	label_card_type();

	Init_SD_Card(dat_width);
	return 1;
}

int Init_card1(uint32_t base_rca, uint8_t dat_width)
{
	/* Config P2 SDHC related Pad */
	Soft_Reset();		/* Reset SDHC */

	set_init_clk(100000);

	printf("- Initiate 100KHz communication\n");

	return label_card_type();
}

void esdhc_adma2_rw(uint8_t bit_mode)
{
	uint16_t blk_len = 0;
	uint16_t blk_cnt = 0;
	uint32_t blk_size = 0;
	uint32_t card_addr = 0x0;
	uint32_t count = 0;
	uint32_t loop_cnt = 20;
	uint32_t loop_div = 256;
	int i, j;
	volatile unsigned long *descidx, *desc = (unsigned long *)0x20002000;
	volatile unsigned long *dramidx, *dram = (unsigned long *)0x20004000;

	blk_cnt = 8;
	blk_len = 0x200;				/* 512 Byte */
	blk_size = blk_cnt * blk_len;	/* Byte base */

	printf("- Initialize ADMA2 TCD\n");

	esdhc_set_wml(WML_WML4);
	esdhc_set_dtw(bit_mode);	/* 8bit mode */
	esdhc_set_dmas(ADMA2);		/* select ADMA1 mode */
	esdhc_set_blkattr(blk_cnt, blk_len);

	descidx = desc;
	dramidx = dram;
	/* 8 descriptors */
#if 0
	*(desc + 0) = (blk_size << 15) | 0x25;
	*(desc + 1) = (unsigned long)dram;
	dram += (blk_size >> 2);
	*(desc + 2) = (blk_size << 15) | 0x25;
	*(desc + 3) = (unsigned long)dram;
	dram += (blk_size >> 2);
	*(desc + 4) = (blk_size << 15) | 0x25;
	*(desc + 5) = (unsigned long)dram;
	dram += (blk_size >> 2);
	*(desc + 6) = (blk_size << 15) | 0x35;
	*(desc + 7) = (unsigned long)(desc + 8);
	*(desc + 8) = (blk_size << 15) | 0x27;
	*(desc + 9) = (unsigned long)dram;
#else

#if 1
	i = 0, j = 0;;
	*(desc + i++) = 0x08000025;
	*(desc + i++) = (0x80020000 + j);
	j += 0x1000;
/*
	*(desc + i++) = 0x08000025;
	*(desc + i++) = (0x80020000 + j);
	j += 0x1000;

	*(desc + i++) = 0x08000025;
	*(desc + i++) = (0x80020000 + j);
	j += 0x1000;

	*(desc + i++) = 0x08000025;
	*(desc + i++) = (0x80020000 + j);
	j += 0x1000;

	*(desc + i++) = 0x08000025;
	*(desc + i++) = (0x80020000 + j);
	j += 0x1000;

	*(desc + i++) = 0x08000025;
	*(desc + i++) = (0x80020000 + j);
	j += 0x1000;

	*(desc + i++) = 0x08000025;
	*(desc + i++) = (0x80020000 + j);
	j += 0x1000;
*/
	*(desc + i++) = 0x00000031;
	*(desc + i) = (unsigned long)(desc + i);
  ++i;

	*(desc + i++) = 0x08000027;
	*(desc + i++) = (0x80020000 + j);
#endif

#if 0
	for (i = 0; i < 7; i++) {
		*(desc + i * 4 + 0) = (blk_size << 15) | 0x25;
		*(desc + i * 4 + 1) = (unsigned long)dram;

		*(desc + i * 4 + 2) = 0x31;
		*(desc + i * 4 + 3) = (unsigned long)(desc + i * 4 + 4);
		dram += (blk_size >> 2);
	}
	*(desc + i * 4 + 0) = (blk_size << 15) | 0x27;
	*(desc + i * 4 + 1) = (unsigned long)dram;
#endif

#endif

	printf("- Start esdhc_adma2_rw_mmc test now! \n"
		   "- Will check %4d bytes data.... \n",
		   loop_cnt);

	if ((count % loop_div) == 0) {
		printf("- checking %4d to %4d bytes data...\n",
			   (count/loop_div)*loop_div*blk_size,
			   (count/loop_div +1)*loop_div*blk_size);
	}

	for (count = 0; count < loop_cnt; count++) {
		/* Show checking progress on terminal */

		card_addr = count * blk_size;

		/* set adma system addr */
		esdhc_set_adms_addr((unsigned long)descidx);
#if 1
		/* Write data to MMC Card ..... */
		CMD25_MultiWrite_Block2(card_addr);
#else
		while (esdhc_get_cihb() != 0);		/* waiting command line is available */
		while (esdhc_get_cdihb() != 0);	/* waiting data line is available, it doesn't work for waitting card out of busy. */

		/* Clear Command Complete bit */
		MCF_ESDHC_ISR = 0xFFFFFFFF;

		MCF_ESDHC_CAR = card_addr;

		MCF_ESDHC_TTR = MMC_CMD25
		    | MCF_ESDHC_TTR_DPSEL
		    | MCF_ESDHC_TTR_CICEN
		    | MCF_ESDHC_TTR_CCCEN
		    | MCF_ESDHC_TTR_MSBSEL
		    | MCF_ESDHC_TTR_AC12EN
		    | MCF_ESDHC_TTR_BCEN | MCF_ESDHC_TTR_DMAEN;

		while ((!(MCF_ESDHC_ISR & MCF_ESDHC_ISR_CC)) &&
		       (!(MCF_ESDHC_ISR & MCF_ESDHC_ISR_CTOE)));
		while ((MCF_ESDHC_PSR & MCF_ESDHC_PSR_WTA));	/* Write transfer active */
		while (!(MCF_ESDHC_ISR & MCF_ESDHC_ISR_TC));

		if ((MCF_ESDHC_ISR & IRQ_ERROR_MASK) != 0x0)
			printf
			    ("- Error: CMD25_MultiWrite_Block2 SDHC_IRQSTAT  =%x \n",
			     MCF_ESDHC_ISR);

		Rd_Response48();
#endif
	}

	printf("Finished esdhc_adma2_rw_mmc test !\n");
}

void esdhc_send_extcsd(uint8_t bit_mode)
{
	uint16_t		blk_len	=0;
	uint32_t		card_addr = 0x0;

	blk_len = 0x200;	/* 512 Byte */

	printf("- esdhc_send_extcsd test!\n");

	/* Initial ESDHC */
	esdhc_set_wml(WML_WML0);
	esdhc_set_dtw(bit_mode);

	esdhc_set_blkattr(1, blk_len);

	esdhc_set_dma_addr(0x20000000);

	/* Write data to MMC Card ..... */
	CMD8_SEND_EXTCSD((uint8_t*)0x20000000);

	printf("- MMC block access finished!\n");
}

void esdhc_test(void)
{
	SIM_SCGC3 |= SIM_SCGC3_SDHC_MASK;

	printf("SDHC Demo\n");

	esdhc_portcfg();

	if (Init_card1(0x0, 1) == -1) {
		printf("- Error: SD card not present\n");
		return;
	}

	if (Init_card(0x0, 4) == 0) {
		printf("- Error: Initialize Card operation Failed\n");
		return;
	}

	esdhc_send_extcsd(DTW_4BIT);
/*
	esdhc_single_rw(BIT_MODE);
	esdhc_multiple_rw(BIT_MODE);
	esdhc_adma1_rw(BIT_MODE);
	esdhc_adma2_rw(BIT_MODE);
*/
	printf("Exit SDHC\n\n");
}
