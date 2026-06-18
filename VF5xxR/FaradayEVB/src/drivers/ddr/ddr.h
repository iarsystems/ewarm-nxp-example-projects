/***************************************************************** 
 * FILE        : dramc.h
 * 
 * DESCRIPTION : Header file for Vybrid DDR      
 * 
 * COPYRIGHT   :(c) 2011, Freescale Semiconductor
 * 
 * 
 *
 ******************************************************************/
 
 /* ================================================================================
   ==============================        dramc         ==============================  
   ================================================================================ */
#ifndef dramc_H
#define dramc_H
#include "typedefs.h"

/* DRAMC - Peripheral register structure */
typedef struct DRAMC_MemMap {
  vuint32_t CCGR0;          /* offset: 0x00000040*/
} volatile *Dramc_MemMapPtr;

/*** define base address ***/
#define DDR_CR_BASE		0x400ae000

#define DDR_PHY_BASE            0x400ae400

/* DRAMC Register per module */
#define DDR_CR000               (DDR_CR_BASE + 0x000)
#define DDR_CR001               (DDR_CR_BASE + 0x004)
#define DDR_CR002               (DDR_CR_BASE + 0x008)
#define DDR_CR003               (DDR_CR_BASE + 0x00C)
#define DDR_CR004               (DDR_CR_BASE + 0x010)
#define DDR_CR005               (DDR_CR_BASE + 0x014)
#define DDR_CR006               (DDR_CR_BASE + 0x018)
#define DDR_CR007               (DDR_CR_BASE + 0x01C)
#define DDR_CR008               (DDR_CR_BASE + 0x020)
#define DDR_CR009               (DDR_CR_BASE + 0x024)

#define DDR_CR010               (DDR_CR_BASE + 0x028)
#define DDR_CR011               (DDR_CR_BASE + 0x02C)
#define DDR_CR012               (DDR_CR_BASE + 0x030)
#define DDR_CR013               (DDR_CR_BASE + 0x034)
#define DDR_CR014               (DDR_CR_BASE + 0x038)
#define DDR_CR015               (DDR_CR_BASE + 0x03C)
#define DDR_CR016               (DDR_CR_BASE + 0x040)
#define DDR_CR017               (DDR_CR_BASE + 0x044)
#define DDR_CR018               (DDR_CR_BASE + 0x048)
#define DDR_CR019               (DDR_CR_BASE + 0x04C)

#define DDR_CR020               (DDR_CR_BASE + 0x050)
#define DDR_CR021               (DDR_CR_BASE + 0x054)
#define DDR_CR022               (DDR_CR_BASE + 0x058)
#define DDR_CR023               (DDR_CR_BASE + 0x05C)
#define DDR_CR024               (DDR_CR_BASE + 0x060)
#define DDR_CR025               (DDR_CR_BASE + 0x064)
#define DDR_CR026               (DDR_CR_BASE + 0x068)
#define DDR_CR027               (DDR_CR_BASE + 0x06C)
#define DDR_CR028               (DDR_CR_BASE + 0x070)
#define DDR_CR029               (DDR_CR_BASE + 0x074)

#define DDR_CR030               (DDR_CR_BASE + 0x078)
#define DDR_CR031               (DDR_CR_BASE + 0x07C)
#define DDR_CR032               (DDR_CR_BASE + 0x080)
#define DDR_CR033               (DDR_CR_BASE + 0x084)
#define DDR_CR034               (DDR_CR_BASE + 0x088)
#define DDR_CR035               (DDR_CR_BASE + 0x08C)
#define DDR_CR036               (DDR_CR_BASE + 0x090)
#define DDR_CR037               (DDR_CR_BASE + 0x094)
#define DDR_CR038               (DDR_CR_BASE + 0x098)
#define DDR_CR039               (DDR_CR_BASE + 0x09C)

#define DDR_CR040               (DDR_CR_BASE + 0x0A0)
#define DDR_CR041               (DDR_CR_BASE + 0x0A4)
#define DDR_CR042               (DDR_CR_BASE + 0x0A8)
#define DDR_CR043               (DDR_CR_BASE + 0x0AC)
#define DDR_CR044               (DDR_CR_BASE + 0x0B0)
#define DDR_CR045               (DDR_CR_BASE + 0x0B4)
#define DDR_CR046               (DDR_CR_BASE + 0x0B8)
#define DDR_CR047               (DDR_CR_BASE + 0x0BC)
#define DDR_CR048               (DDR_CR_BASE + 0x0C0)
#define DDR_CR049               (DDR_CR_BASE + 0x0C4)

#define DDR_CR050               (DDR_CR_BASE + 0x0C8)
#define DDR_CR051               (DDR_CR_BASE + 0x0CC)
#define DDR_CR052               (DDR_CR_BASE + 0x0D0)
#define DDR_CR053               (DDR_CR_BASE + 0x0D4)
#define DDR_CR054               (DDR_CR_BASE + 0x0D8)
#define DDR_CR055               (DDR_CR_BASE + 0x0DC)
#define DDR_CR056               (DDR_CR_BASE + 0x0E0)
#define DDR_CR057               (DDR_CR_BASE + 0x0E4)
#define DDR_CR058               (DDR_CR_BASE + 0x0E8)
#define DDR_CR059               (DDR_CR_BASE + 0x0EC)

#define DDR_CR060               (DDR_CR_BASE + 0x0F0)
#define DDR_CR061               (DDR_CR_BASE + 0x0F4)
#define DDR_CR062               (DDR_CR_BASE + 0x0F8)
#define DDR_CR063               (DDR_CR_BASE + 0x0FC)
#define DDR_CR064               (DDR_CR_BASE + 0x100)
#define DDR_CR065               (DDR_CR_BASE + 0x104)
#define DDR_CR066               (DDR_CR_BASE + 0x108)
#define DDR_CR067               (DDR_CR_BASE + 0x10C)
#define DDR_CR068               (DDR_CR_BASE + 0x110)
#define DDR_CR069               (DDR_CR_BASE + 0x114)

#define DDR_CR070               (DDR_CR_BASE + 0x118)
#define DDR_CR071               (DDR_CR_BASE + 0x11C)
#define DDR_CR072               (DDR_CR_BASE + 0x120)
#define DDR_CR073               (DDR_CR_BASE + 0x124)
#define DDR_CR074               (DDR_CR_BASE + 0x128)
#define DDR_CR075               (DDR_CR_BASE + 0x12C)
#define DDR_CR076               (DDR_CR_BASE + 0x130)
#define DDR_CR077               (DDR_CR_BASE + 0x134)
#define DDR_CR078               (DDR_CR_BASE + 0x138)
#define DDR_CR079               (DDR_CR_BASE + 0x13C)

#define DDR_CR080               (DDR_CR_BASE + 0x140)
#define DDR_CR081               (DDR_CR_BASE + 0x144)
#define DDR_CR082               (DDR_CR_BASE + 0x148)
#define DDR_CR083               (DDR_CR_BASE + 0x14C)
#define DDR_CR084               (DDR_CR_BASE + 0x150)
#define DDR_CR085               (DDR_CR_BASE + 0x154)
#define DDR_CR086               (DDR_CR_BASE + 0x158)
#define DDR_CR087               (DDR_CR_BASE + 0x15C)
#define DDR_CR088               (DDR_CR_BASE + 0x160)
#define DDR_CR089               (DDR_CR_BASE + 0x164)

#define DDR_CR090               (DDR_CR_BASE + 0x168)
#define DDR_CR091               (DDR_CR_BASE + 0x16C)
#define DDR_CR092               (DDR_CR_BASE + 0x170)
#define DDR_CR093               (DDR_CR_BASE + 0x174)
#define DDR_CR094               (DDR_CR_BASE + 0x178)
#define DDR_CR095               (DDR_CR_BASE + 0x17C)
#define DDR_CR096               (DDR_CR_BASE + 0x180)
#define DDR_CR097               (DDR_CR_BASE + 0x184)
#define DDR_CR098               (DDR_CR_BASE + 0x188)
#define DDR_CR099               (DDR_CR_BASE + 0x18C)

#define DDR_CR100               (DDR_CR_BASE + 0x190)
#define DDR_CR101               (DDR_CR_BASE + 0x194)
#define DDR_CR102               (DDR_CR_BASE + 0x198)
#define DDR_CR103               (DDR_CR_BASE + 0x19C)
#define DDR_CR104               (DDR_CR_BASE + 0x1A0)
#define DDR_CR105               (DDR_CR_BASE + 0x1A4)
#define DDR_CR106               (DDR_CR_BASE + 0x1A8)
#define DDR_CR107               (DDR_CR_BASE + 0x1AC)
#define DDR_CR108               (DDR_CR_BASE + 0x1B0)
#define DDR_CR109               (DDR_CR_BASE + 0x1B4)

#define DDR_CR110               (DDR_CR_BASE + 0x1B8)
#define DDR_CR111               (DDR_CR_BASE + 0x1BC)
#define DDR_CR112               (DDR_CR_BASE + 0x1C0)
#define DDR_CR113               (DDR_CR_BASE + 0x1C4)
#define DDR_CR114               (DDR_CR_BASE + 0x1C8)
#define DDR_CR115               (DDR_CR_BASE + 0x1CC)
#define DDR_CR116               (DDR_CR_BASE + 0x1D0)
#define DDR_CR117               (DDR_CR_BASE + 0x1D4)
#define DDR_CR118               (DDR_CR_BASE + 0x1D8)
#define DDR_CR119               (DDR_CR_BASE + 0x1DC)

#define DDR_CR120               (DDR_CR_BASE + 0x1E0)
#define DDR_CR121               (DDR_CR_BASE + 0x1E4)
#define DDR_CR122               (DDR_CR_BASE + 0x1E8)
#define DDR_CR123               (DDR_CR_BASE + 0x1EC)
#define DDR_CR124               (DDR_CR_BASE + 0x1F0)
#define DDR_CR125               (DDR_CR_BASE + 0x1F4)
#define DDR_CR126               (DDR_CR_BASE + 0x1F8)
#define DDR_CR127               (DDR_CR_BASE + 0x1FC)
#define DDR_CR128               (DDR_CR_BASE + 0x200)
#define DDR_CR129               (DDR_CR_BASE + 0x204)

#define DDR_CR130               (DDR_CR_BASE + 0x208)
#define DDR_CR131               (DDR_CR_BASE + 0x20C)
#define DDR_CR132               (DDR_CR_BASE + 0x210)
#define DDR_CR133               (DDR_CR_BASE + 0x214)
#define DDR_CR134               (DDR_CR_BASE + 0x218)
#define DDR_CR135               (DDR_CR_BASE + 0x21C)
#define DDR_CR136               (DDR_CR_BASE + 0x220)
#define DDR_CR137               (DDR_CR_BASE + 0x224)
#define DDR_CR138               (DDR_CR_BASE + 0x228)
#define DDR_CR139               (DDR_CR_BASE + 0x22C)

#define DDR_CR140               (DDR_CR_BASE + 0x230)
#define DDR_CR141               (DDR_CR_BASE + 0x234)
#define DDR_CR142               (DDR_CR_BASE + 0x238)
#define DDR_CR143               (DDR_CR_BASE + 0x23C)
#define DDR_CR144               (DDR_CR_BASE + 0x240)
#define DDR_CR145               (DDR_CR_BASE + 0x244)
#define DDR_CR146               (DDR_CR_BASE + 0x248)
#define DDR_CR147               (DDR_CR_BASE + 0x24C)
#define DDR_CR148               (DDR_CR_BASE + 0x250)
#define DDR_CR149               (DDR_CR_BASE + 0x254)

#define DDR_CR150               (DDR_CR_BASE + 0x258)
#define DDR_CR151               (DDR_CR_BASE + 0x25C)
#define DDR_CR152               (DDR_CR_BASE + 0x260)
#define DDR_CR153               (DDR_CR_BASE + 0x264)
#define DDR_CR154               (DDR_CR_BASE + 0x268)
#define DDR_CR155               (DDR_CR_BASE + 0x26C)
#define DDR_CR156               (DDR_CR_BASE + 0x270)
#define DDR_CR157               (DDR_CR_BASE + 0x274)
#define DDR_CR158               (DDR_CR_BASE + 0x278)
#define DDR_CR159               (DDR_CR_BASE + 0x27C)

#define DDR_CR160               (DDR_CR_BASE + 0x280)
#define DDR_CR161               (DDR_CR_BASE + 0x284)
#define DDR_CR162               (DDR_CR_BASE + 0x288)
#define DDR_CR163               (DDR_CR_BASE + 0x28C)
#define DDR_CR164               (DDR_CR_BASE + 0x290)
#define DDR_CR165               (DDR_CR_BASE + 0x294)
#define DDR_CR166               (DDR_CR_BASE + 0x298)
#define DDR_CR167               (DDR_CR_BASE + 0x29C)
#define DDR_CR168               (DDR_CR_BASE + 0x2A0)
#define DDR_CR169               (DDR_CR_BASE + 0x2A4)

#define DDR_CR170               (DDR_CR_BASE + 0x2A8)
#define DDR_CR171               (DDR_CR_BASE + 0x2AC)
#define DDR_CR172               (DDR_CR_BASE + 0x2B0)
#define DDR_CR173               (DDR_CR_BASE + 0x2B4)
#define DDR_CR174               (DDR_CR_BASE + 0x2B8)
#define DDR_CR175               (DDR_CR_BASE + 0x2BC)
#define DDR_CR176               (DDR_CR_BASE + 0x2C0)
#define DDR_CR177               (DDR_CR_BASE + 0x2C4)
#define DDR_CR178               (DDR_CR_BASE + 0x2C8)
#define DDR_CR179               (DDR_CR_BASE + 0x2CC)

//
// PHY
//
#define DDR_PHY000              (DDR_PHY_BASE + 0x000)
#define DDR_PHY001              (DDR_PHY_BASE + 0x004)
#define DDR_PHY002              (DDR_PHY_BASE + 0x008)
#define DDR_PHY003              (DDR_PHY_BASE + 0x00C)
#define DDR_PHY004              (DDR_PHY_BASE + 0x010)
#define DDR_PHY005              (DDR_PHY_BASE + 0x014)
#define DDR_PHY006              (DDR_PHY_BASE + 0x018)
#define DDR_PHY007              (DDR_PHY_BASE + 0x01C)
#define DDR_PHY008              (DDR_PHY_BASE + 0x020)
#define DDR_PHY009              (DDR_PHY_BASE + 0x024)

#define DDR_PHY010              (DDR_PHY_BASE + 0x028)
#define DDR_PHY011              (DDR_PHY_BASE + 0x02C)
#define DDR_PHY012              (DDR_PHY_BASE + 0x030)
#define DDR_PHY013              (DDR_PHY_BASE + 0x034)
#define DDR_PHY014              (DDR_PHY_BASE + 0x038)
#define DDR_PHY015              (DDR_PHY_BASE + 0x03C)
#define DDR_PHY016              (DDR_PHY_BASE + 0x040)
#define DDR_PHY017              (DDR_PHY_BASE + 0x044)
#define DDR_PHY018              (DDR_PHY_BASE + 0x048)
#define DDR_PHY019              (DDR_PHY_BASE + 0x04C)

#define DDR_PHY020              (DDR_PHY_BASE + 0x050)
#define DDR_PHY021              (DDR_PHY_BASE + 0x054)
#define DDR_PHY022              (DDR_PHY_BASE + 0x058)
#define DDR_PHY023              (DDR_PHY_BASE + 0x05C)
#define DDR_PHY024              (DDR_PHY_BASE + 0x060)
#define DDR_PHY025              (DDR_PHY_BASE + 0x064)
#define DDR_PHY026              (DDR_PHY_BASE + 0x068)
#define DDR_PHY027              (DDR_PHY_BASE + 0x06C)
#define DDR_PHY028              (DDR_PHY_BASE + 0x070)
#define DDR_PHY029              (DDR_PHY_BASE + 0x074)

#define DDR_PHY030              (DDR_PHY_BASE + 0x078)
#define DDR_PHY031              (DDR_PHY_BASE + 0x07C)
#define DDR_PHY032              (DDR_PHY_BASE + 0x080)
#define DDR_PHY033              (DDR_PHY_BASE + 0x084)
#define DDR_PHY034              (DDR_PHY_BASE + 0x088)
#define DDR_PHY035              (DDR_PHY_BASE + 0x08C)
#define DDR_PHY036              (DDR_PHY_BASE + 0x090)
#define DDR_PHY037              (DDR_PHY_BASE + 0x094)
#define DDR_PHY038              (DDR_PHY_BASE + 0x098)
#define DDR_PHY039              (DDR_PHY_BASE + 0x09C)

#define DDR_PHY040              (DDR_PHY_BASE + 0x0A0)
#define DDR_PHY041              (DDR_PHY_BASE + 0x0A4)
#define DDR_PHY042              (DDR_PHY_BASE + 0x0A8)
#define DDR_PHY043              (DDR_PHY_BASE + 0x0AC)
#define DDR_PHY044              (DDR_PHY_BASE + 0x0B0)
#define DDR_PHY045              (DDR_PHY_BASE + 0x0B4)
#define DDR_PHY046              (DDR_PHY_BASE + 0x0B8)
#define DDR_PHY047              (DDR_PHY_BASE + 0x0BC)
#define DDR_PHY048              (DDR_PHY_BASE + 0x0C0)
#define DDR_PHY049              (DDR_PHY_BASE + 0x0C4)

#define DDR_PHY050              (DDR_PHY_BASE + 0x0C8)
#define DDR_PHY051              (DDR_PHY_BASE + 0x0CC)
#define DDR_PHY052              (DDR_PHY_BASE + 0x0D0)
#define DDR_PHY053              (DDR_PHY_BASE + 0x0D4)
#define DDR_PHY054              (DDR_PHY_BASE + 0x0D8)
#define DDR_PHY055              (DDR_PHY_BASE + 0x0DC)
#define DDR_PHY056              (DDR_PHY_BASE + 0x0E0)
#define DDR_PHY057              (DDR_PHY_BASE + 0x0E4)
#define DDR_PHY058              (DDR_PHY_BASE + 0x0E8)
#define DDR_PHY059              (DDR_PHY_BASE + 0x0EC)

#define DDR_PHY060              (DDR_PHY_BASE + 0x0F0)
#define DDR_PHY061              (DDR_PHY_BASE + 0x0F4)
#define DDR_PHY062              (DDR_PHY_BASE + 0x0F8)
#define DDR_PHY063              (DDR_PHY_BASE + 0x0FC)
#define DDR_PHY064              (DDR_PHY_BASE + 0x100)
#define DDR_PHY065              (DDR_PHY_BASE + 0x104)
#define DDR_PHY066              (DDR_PHY_BASE + 0x108)
#define DDR_PHY067              (DDR_PHY_BASE + 0x10C)
#define DDR_PHY068              (DDR_PHY_BASE + 0x110)

void ddr_iomux(void);
void ddr_init(void);
void ddr_phy_init(void);
void ddr_init1(void);
void ddr_phy_init1(void);
void ddr_init2(void);
void ddr_phy_init2(void);
void ddr_init3(void);
void ddr_phy_init3(void);

#endif
