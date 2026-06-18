/*
 * File:		rnga.h
 * Purpose:     Hardware random number generator driver
 *
 * Notes:
 */

#ifndef __RNGA_H__
#define __RNGA_H__

/********************************************************************/

void  rnga_init (unsigned int);
unsigned int rnga_getnumber (void);
void rnga_stop (void);

/********************************************************************/

#endif /* __RNGA_H__ */
