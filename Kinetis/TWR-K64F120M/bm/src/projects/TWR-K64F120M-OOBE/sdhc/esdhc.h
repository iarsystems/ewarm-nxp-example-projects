#ifndef __ESDHC_H
#define __ESDHC_H

#define DTW_1BIT			0x00
#define DTW_4BIT			0x01
#define DTW_8BIT			0x02

#define BIT_MODE			DTW_4BIT

#define MMC_SET_R0		SDHC_XFERTYP_RSPTYP(0)	/* No Response */
#define MMC_SET_R1		SDHC_XFERTYP_RSPTYP(2)
#define MMC_SET_R1b		SDHC_XFERTYP_RSPTYP(3)	/* normal response command	;Response Length 48, check Busy after response */
#define MMC_SET_R2		SDHC_XFERTYP_RSPTYP(1)	/* CID, CSD register		;Response Length 48 */
#define MMC_SET_R3		SDHC_XFERTYP_RSPTYP(2)	/* OCR register				;Response Length 136 */
#define MMC_SET_R4		SDHC_XFERTYP_RSPTYP(2)	/* OCR register				;Response Length 136 */
#define MMC_SET_R5		SDHC_XFERTYP_RSPTYP(2)	/* normal response command	; Response Length 48 */
#define MMC_SET_R5b		SDHC_XFERTYP_RSPTYP(3)	/* normal response command	;Response Length 48, check Busy after response */
#define MMC_SET_R6		SDHC_XFERTYP_RSPTYP(2)	/* normal response command	; Response Length 48 */

#define MMC_CMD0		SDHC_XFERTYP_CMDINX(0)
#define MMC_CMD1		(SDHC_XFERTYP_CMDINX(1) | MMC_SET_R3)
#define MMC_CMD2		(SDHC_XFERTYP_CMDINX(2) | MMC_SET_R2)
#define MMC_CMD3		(SDHC_XFERTYP_CMDINX(3) | MMC_SET_R1)
#define MMC_CMD4		(SDHC_XFERTYP_CMDINX(4) | MMC_SET_R0)
#define MMC_CMD5		(SDHC_XFERTYP_CMDINX(5) | MMC_SET_R1b)
#define MMC_CMD6		(SDHC_XFERTYP_CMDINX(6) | MMC_SET_R1b)
#define MMC_CMD7		(SDHC_XFERTYP_CMDINX(7) | MMC_SET_R1b)
#define MMC_CMD8		(SDHC_XFERTYP_CMDINX(8) | MMC_SET_R1)
#define MMC_CMD9		(SDHC_XFERTYP_CMDINX(9) | MMC_SET_R2)
#define MMC_CMD10		(SDHC_XFERTYP_CMDINX(10) | MMC_SET_R2)
#define MMC_CMD11		(SDHC_XFERTYP_CMDINX(11) | MMC_SET_R1)
#define MMC_CMD12		(SDHC_XFERTYP_CMDINX(12) | MMC_SET_R1b)
#define MMC_CMD13		(SDHC_XFERTYP_CMDINX(13) | MMC_SET_R1)
#define MMC_CMD14		(SDHC_XFERTYP_CMDINX(14) | MMC_SET_R1)
#define MMC_CMD15		(SDHC_XFERTYP_CMDINX(15) | MMC_SET_R0)
#define MMC_CMD16		(SDHC_XFERTYP_CMDINX(16) | MMC_SET_R1)
#define MMC_CMD17		(SDHC_XFERTYP_CMDINX(17) | MMC_SET_R1)
#define MMC_CMD18		(SDHC_XFERTYP_CMDINX(18) | MMC_SET_R1)
#define MMC_CMD19		(SDHC_XFERTYP_CMDINX(19) | MMC_SET_R1)
#define MMC_CMD20		(SDHC_XFERTYP_CMDINX(20) | MMC_SET_R1)
#define MMC_CMD23		(SDHC_XFERTYP_CMDINX(23) | MMC_SET_R1)
#define MMC_CMD24		(SDHC_XFERTYP_CMDINX(24) | MMC_SET_R1)
#define MMC_CMD25		(SDHC_XFERTYP_CMDINX(25) | MMC_SET_R1)
#define MMC_CMD26		(SDHC_XFERTYP_CMDINX(26) | MMC_SET_R1)
#define MMC_CMD27		(SDHC_XFERTYP_CMDINX(27) | MMC_SET_R1)
#define MMC_CMD28		(SDHC_XFERTYP_CMDINX(28) | MMC_SET_R1b)
#define MMC_CMD29		(SDHC_XFERTYP_CMDINX(29) | MMC_SET_R1b)
#define MMC_CMD30		(SDHC_XFERTYP_CMDINX(30) | MMC_SET_R1)
#define MMC_CMD35		(SDHC_XFERTYP_CMDINX(35) | MMC_SET_R1)
#define MMC_CMD36		(SDHC_XFERTYP_CMDINX(36) | MMC_SET_R1)
#define MMC_CMD38		(SDHC_XFERTYP_CMDINX(38) | MMC_SET_R1)

#define SD_CMD8			(SDHC_XFERTYP_CMDINX(8) | MMC_SET_R1)
#define SD_CMD55		(SDHC_XFERTYP_CMDINX(55) | MMC_SET_R1)
#define SD_ACMD6		(SDHC_XFERTYP_CMDINX(6) | MMC_SET_R1)
#define SD_ACMD13		(SDHC_XFERTYP_CMDINX(13) | MMC_SET_R1)
#define SD_ACMD22		(SDHC_XFERTYP_CMDINX(22) | MMC_SET_R1)
#define SD_ACMD41		(SDHC_XFERTYP_CMDINX(41) | MMC_SET_R3)
#define SD_ACMD51		(SDHC_XFERTYP_CMDINX(51) | MMC_SET_R1)

#define CHECK_PATTEN		0xAA
#define VHS					(1<<8)	/* 2.7-3.6V */

#define CARD_REGISTRY_ON	0x01
#define CARD_REGISTRY_OFF	0x00

#define SD_BUSY				0x80000000

#define IRQ_ERROR_MASK		0xffff0000

#define SDIO_CARD			0x01
#define SD_CARD				0x02
#define MMC_CARD			0x03

#define Idle				0x00
#define Ready				0x01
#define Ident				0x02
#define Stby				0x03
#define Tran				0x04
#define Data				0x05
#define Rcv					0x06
#define Prg					0x07
#define Dis					0x08
#define Btst				0x09
#define Slp					0x0a

#define WML_WML				0x04
#define WML_WML0			0x10
#define WML_WML1			0x20
#define WML_WML2			0x40
#define WML_WML3			0x01
#define WML_WML4			0x80

#define NO_INTERNAL_DMA 	0x0
#define SIMPLE_DMA			0x0
#define ADMA1				0x01
#define ADMA2				0x02

#endif /* __ESDHC_H */
