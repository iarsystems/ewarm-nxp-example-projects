
#ifndef OTP_H_
#define OTP_H_

typedef struct {          				/*!< (@ 0x40045000) OTP Structure        */
	__IO uint32_t OTP0_0;           	/*!< (@ 0x40045000) OTP content */
	__IO uint32_t OTP0_1;           	/*!< (@ 0x40045004) OTP content */
	__IO uint32_t OTP0_2;           	/*!< (@ 0x40045008) OTP content */
	__IO uint32_t OTP0_3;           	/*!< (@ 0x4004500C) OTP content */
	__IO uint32_t OTP1_0;               /*!< (@ 0x40045010) OTP content */
	__IO uint32_t OTP1_1;               /*!< (@ 0x40045014) OTP content */
	__IO uint32_t OTP1_2;               /*!< (@ 0x40045018) OTP content */
	__IO uint32_t OTP1_3;               /*!< (@ 0x4004501C) OTP content */
	__IO uint32_t OTP2_0;               /*!< (@ 0x40045020) OTP content */
	__IO uint32_t OTP2_1;               /*!< (@ 0x40045024) OTP content */
	__IO uint32_t OTP2_2;               /*!< (@ 0x40045028) OTP content */
	__IO uint32_t OTP2_3;               /*!< (@ 0x4004502C) OTP content */
	     uint32_t RESERVED0[4];		                      
	__IO uint32_t UNIQUE_KEY0;          /*!< (@ 0x40045040) Unique Key bit 31..0. R/W locked at default */
	__IO uint32_t UNIQUE_KEY1;          /*!< (@ 0x40045044) Unique Key bit 63..32. R/W locked at default */
	__IO uint32_t UNIQUE_KEY2;          /*!< (@ 0x40045048) Unique Key bit 95..64. R/W locked at default */
	__IO uint32_t UNIQUE_KEY3;          /*!< (@ 0x4004504C) Unique Key bit 127..96. R/W locked at default */
	__IO uint32_t RANDOM_NUM0;          /*!< (@ 0x40045050) Random number bit 31..0. R/W locked at default */
	__IO uint32_t RANDOM_NUM1;          /*!< (@ 0x40045054) Random number bit 63..32. R/W locked at default */
	__IO uint32_t RANDOM_NUM2;          /*!< (@ 0x40045058) Random number bit 95..64. R/W locked at default */
	__IO uint32_t RANDOM_NUM3;          /*!< (@ 0x4004505C) Random number bit 127..96. R/W locked at default */
	__IO uint32_t USER_KEY0;            /*!< (@ 0x40045060) User Key bit 31..0. R locked at default */
	__IO uint32_t USER_KEY1;            /*!< (@ 0x40045064) User Key bit 63..32. R locked at default */
	__IO uint32_t USER_KEY2;            /*!< (@ 0x40045068) User Key bit 95..64. R locked at default */
	__IO uint32_t USER_KEY3;            /*!< (@ 0x4004506C) User Key bit 127..96. R locked at default */
	     uint32_t RESERVED1[4];		                      
	__IO uint32_t WRTMASK;              /*!< (@ 0x40045080) Masks APB write to fuses */
	__O  uint32_t STATUS;               /*!< (@ 0x40045084) Indicates write status of fuses */
	     uint32_t RESERVED2[2];		                        
	__IO uint32_t PROGRAM;              /*!< (@ 0x40045090) Enables write of Shuffled AES value to OTP1c */
	__IO uint32_t AES_SSEL;             /*!< (@ 0x40045094) Controls selecting source for SKey output to AES Engine */
	__IO uint32_t SHUFFLE_CTRL;         /*!< (@ 0x40045098) Shuffle block control */
		 uint32_t RESERVED3;
	__IO uint32_t OTP0_WR_LOCK;         /*!< (@ 0x400450A0) Locks write access to itself and OTP0 write access */
	__IO uint32_t OTP0_RD_LOCK;         /*!< (@ 0x400450A4) Locks write access to itself and OTP0 read access */
	__IO uint32_t OTP1_WR_LOCK;         /*!< (@ 0x400450A8) Locks write access to itself and OTP1 write access */    
	__IO uint32_t OTP1_RD_LOCK;         /*!< (@ 0x400450AC) Locks write access to itself and OTP1 read access */
	__IO uint32_t OTP2_WR_LOCK;         /*!< (@ 0x400450B0) Locks write access to itself and OTP2 write access */
	__IO uint32_t OTP2_RD_LOCK;         /*!< (@ 0x400450B4) Locks write access to itself and OTP2 read access */
	__IO uint32_t RESERVED4[2];          
	__IO uint32_t UNIQUE_KEY_WR_LOCK;   /*!< (@ 0x400450C0) Locks write access to itself and Unique Key write access */
	__IO uint32_t UNIQUE_KEY_RD_LOCK;   /*!< (@ 0x400450C4)  */
	__IO uint32_t RANDOM_NUM_WR_LOCK;   /*!< (@ 0x400450C8)  */
	__IO uint32_t RANDOM_NUM_RD_LOCK;   /*!< (@ 0x400450CC)  */
	__IO uint32_t RESERVED5;          	/*!< (@ 0x400450D0)  */
	__IO uint32_t USER_KEY_RD_LOCK;     /*!< (@ 0x400450D4)  */
} LPC_OTP_Type;
#define LPC_OTP_BASE 0x40045000
#define LPC_OTP ((LPC_OTP_Type*) LPC_OTP_BASE)

#endif /*OTP_H_*/
