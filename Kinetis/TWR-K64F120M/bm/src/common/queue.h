/*
 * All software, source code, included documentation, and any implied know-how
 * are property of Freescale Semiconductor and therefore considered
 * CONFIDENTIAL INFORMATION.
 * This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.
 *
 * All Confidential Information remains the property of Freescale Semiconductor
 * and will not be copied or reproduced without the express written permission
 * of the Discloser, except for copies that are absolutely necessary in order
 * to fulfill the Purpose.
 *
 * Services performed by FREESCALE in this matter are performed AS IS and
 * without any warranty.
 * CUSTOMER retains the final decision relative to the total design and
 * functionality of the end product.
 * FREESCALE neither guarantees nor will be held liable by CUSTOMER for the
 * success of this project.
 *
 * FREESCALE disclaims all warranties, express, implied or statutory
 * including, but not limited to, implied warranty of merchantability or
 * fitness for a particular purpose on any hardware, software ore advise
 * supplied to the project by FREESCALE, and or any product resulting from
 * FREESCALE services.
 * In no event shall FREESCALE be liable for incidental or consequential
 * damages arising out of this agreement. CUSTOMER agrees to hold FREESCALE
 * harmless against any and all claims demands or actions by anyone on account
 * of any damage,or injury, whether commercial, contractual, or tortuous,
 * rising directly or indirectly as a result of the advise or assistance
 * supplied CUSTOMER in connectionwith product, services or goods supplied
 * under this Agreement.
 */

/*
 * File:	queue.h
 * Purpose:	Implement a first in, first out linked list
 *
 * Notes:
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

/* Individual queue node */
typedef struct NODE
{
	struct NODE *next;
} QNODE;

/* Queue Struture - linked list of qentry items */
typedef struct
{
	QNODE *head;
	QNODE *tail;
} QUEUE;

/* Functions provided by queue.c */
void queue_init(QUEUE *);
int queue_isempty(QUEUE *);
void queue_add(QUEUE *, QNODE *);
QNODE* queue_remove(QUEUE *);
QNODE* queue_peek(QUEUE *);
void queue_move(QUEUE *, QUEUE *);

#endif		/* _QUEUE_H_ */
