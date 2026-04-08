#ifndef ARM7TDMIS_TYPES_H_
#define ARM7TDMIS_TYPES_H_

/***********************************************************************
 * Global typedefs
 **********************************************************************/

/* SMA type for character type */
typedef char CHAR;

/* Unsigned char */
typedef unsigned char U8;

/* Signed char */
typedef signed char S8;

/* Unsigned short */
typedef	unsigned short U16;

/* Signed short */
typedef	signed short S16;

/* Unsigned integer */
typedef	unsigned int U32;

/* Signed integer */
typedef	signed int S32;

/* Signed long */
typedef long long S64;

/* Unsigned long */
typedef unsigned long long U64;

/* 32 bit boolean */
typedef S32 BOOL32;

/* 16 bit boolean */
typedef S16 BOOL16;

/* 8 bit boolean type */
typedef S8 BOOL8;

/* defines to define access restrictions to peripheral registers */
#define     __I     volatile const            /* read only     */
#define     __O     volatile                  /* write only    */
#define     __IO    volatile                  /* read / write  */


/***********************************************************************
 * Global Macros
 **********************************************************************/

/* _BIT(n) sets the bit at position "n"
 * _BIT(n) is intended to be used in "OR" and "AND" expressions:
 * e.g., "(_BIT(3) | _BIT(7))".
 */
#undef _BIT
/* Set bit macro */
#define _BIT(n)	(((U32)(1)) << (n))

/* _SBF(f,v) sets the bit field starting at position "f" to value "v".
 * _SBF(f,v) is intended to be used in "OR" and "AND" expressions:
 * e.g., "((_SBF(5,7) | _SBF(12,0xF)) & 0xFFFF)"
 */ 
#undef _SBF
/* Set bit field macro */
#define _SBF(f,v) (((U32)(v)) << (f))

/* _BITMASK constructs a symbol with 'field_width' least significant
 * bits set.
 * e.g., _BITMASK(5) constructs '0x1F', _BITMASK(16) == 0xFFFF
 * The symbol is intended to be used to limit the bit field width
 * thusly:
 * <a_register> = (any_expression) & _BITMASK(x), where 0 < x <= 32.
 * If "any_expression" results in a value that is larger than can be
 * contained in 'x' bits, the bits above 'x - 1' are masked off.  When
 * used with the _SBF example above, the example would be written:
 * a_reg = ((_SBF(5,7) | _SBF(12,0xF)) & _BITMASK(16))
 * This ensures that the value written to a_reg is no wider than 
 * 16 bits, and makes the code easier to read and understand.
 */ 
#undef _BITMASK
/* Bitmask creation macro */
#define _BITMASK(field_width) ( _BIT(field_width) - 1)

/* SUCCESS macro */
#define SUCCESS     0

#ifndef FALSE
/* FALSE macro */
#define FALSE (0==1)
#endif
#ifndef TRUE
/* TRUE macro */
#define TRUE (!(FALSE))
#endif

/* NULL pointer */
#ifndef NULL
#define NULL ((void*) 0)
#endif 

/* Number of elements in an array */
#define NELEMENTS(array)  (sizeof (array) / sizeof (array[0]))

/* Static data/function define */
#define STATIC static
/* External data/function define */
#define EXTERN extern
/* Status type */
typedef S32 STATUS;

/* NO_ERROR macro */
#define _NO_ERROR           (S32)(0)
/* ERROR macro */
#define _ERROR              (S32)(-1)
/* Device unknown macro */
#define LPC_DEV_UNKNOWN     (S32)(-2)
/* Device not supported macro */
#define LPC_NOT_SUPPORTED   (S32)(-3)
/* Device not open macro */
#define LPC_NOT_OPEN        (S32)(-4)
/* Device in use macro */
#define LPC_IN_USE          (S32)(-5)
/* Device oin conflict macro */
#define LPC_PIN_CONFLICT    (S32)(-6)
/* Device bad paramaters macro */
#define LPC_BAD_PARAMS      (S32)(-7)
/* Bad device handle macro */
#define LPC_BAD_HANDLE      (S32)(-8)
/* Bad device clock macro */
#define LPC_BAD_CLK         (S32)(-9)
/* Device can't start macro */
#define LPC_CANT_START      (S32)(-10)
/* Device can't stop macro */
#define LPC_CANT_STOP       (S32)(-11)

#endif /*ARM7TDMIS_TYPES_H_*/
