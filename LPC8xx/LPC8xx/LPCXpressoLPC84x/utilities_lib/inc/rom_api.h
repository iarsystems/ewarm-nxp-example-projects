/*
 * rom_api_.h
 *
 *  Created on: 
 *   By Arthur:
 */

#ifndef __ROM_API_H_
#define __ROM_API_H_



// Power APIs 
//typedef struct _PWRD {
//  void (*set_pll)(unsigned int cmd[], unsigned int resp[]);
//} PWRD_API_T;



// Integer divide API routines
typedef struct {
  int quot;      // Quotient
  int rem;       // Remainder
} IDIV_RETURN_T;

typedef struct {
  unsigned quot; // Quotient
  unsigned rem;  // Reminder
} UIDIV_RETURN_T;

typedef struct {
  int (*sidiv)(int numerator, int denominator);                         // Signed integer division 
  unsigned (*uidiv)(unsigned numerator, unsigned denominator);          // Unsigned integer division 
  IDIV_RETURN_T (*sidivmod)(int numerator, int denominator);            // Signed integer division with remainder 
  UIDIV_RETURN_T (*uidivmod)(unsigned numerator, unsigned denominator); // Unsigned integer division with remainder 
} ROM_DIV_API_T;





// The master structure that defines the table of all ROM APIs on the device (a.k.a ROM Driver table)
typedef struct _ROM_API {
  const unsigned int reserved4[4];   // Offsets 0, 4, 8, 0xC
  const ROM_DIV_API_T *divApiBase;   // Offset 0x10. Integer division routines function table base address
  const unsigned int reserved7[7];   // Offsets 0x14 - 0x2C
} LPC_ROM_API_T;





#define ROM_DRIVER_BASE (0x0F001FF8UL)

// Define a pointer to the master table
#define LPC_ROM_API     (*(LPC_ROM_API_T * *)ROM_DRIVER_BASE)

// Use like this:
// ROM_DIV_API_T const *pROMDiv = LPC_ROM_API->divApiBase; // Create and initialize a pointer to the DIVIDE functions table
// int32_t result;                                         // Declare an int variable
// result = pROMDiv->sidiv(-99, 6);                        // result now contains -99/6 = -16



// Alternate form
#define LPC_DIVD_API    ((ROM_DIV_API_T *) ((*(LPC_ROM_API_T * *) (ROM_DRIVER_BASE))->divApiBase))



#endif // rom_api.h
