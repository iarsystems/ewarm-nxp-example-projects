/* MAC7100FlashConfiguration */


struct FLash_Configuration_Field
{
  unsigned long long Backdoor_Comparison_Key ;
  unsigned long Program_Flash_Protection_Bytes ;
  unsigned long Program_Flash_SUPV_Access_Bytes ;
  unsigned long Program_Flash_DATA_Access_Bytes ;
  unsigned long Flash_Security_Word ;
  unsigned char Data_Flash_Protection_Byte ;
  unsigned char Data_Flash_SUPV_Access_Byte ;
  unsigned char Data_Flash_DATA_Access_Byte ;
} ;


#pragma section=".flash_cnfg"
#pragma location=".flash_cnfg"
__root const struct FLash_Configuration_Field MyConfig =
{
  0xABABABABABABABABULL,  /* Backdoor_Comparison_Key */
  0x00000000,  /* Program_Flash_Protection_Bytes */
  0x00000000,  /* Program_Flash_SUPV_Access_Bytes */
  0xFFFFFFFF,  /* Program_Flash_DATA_Access_Bytes */
  0x80000002,  /* Flash_Security_Word */
  0x00,  /* Data_Flash_Protection_Byte */
  0xFF,  /* Data_Flash_SUPV_Access_Byte */
  0x00  /* Data_Flash_DATA_Access_Byte */
} ;
