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
 * File:    queue.c
 * Purpose: Implement a first in, first out linked list
 *
 * Notes:
 */

#include "common.h"
#include "queue.h"

/*
 * Initialize the specified queue to an empty state
 *
 * Parameters:
 *  q	Pointer to queue structure
 */
void queue_init(QUEUE * q)
{
	q->head = NULL;
}

/*
 * Check for an empty queue
 *
 * Parameters:
 *  q	Pointer to queue structure
 *
 * Return Value:
 *  1	if Queue is empty
 *  0	otherwise
 */
int queue_isempty(QUEUE * q)
{
	return (q->head == NULL);
}

/*
 * Add an item to the end of the queue
 *
 * Parameters:
 *  q		Pointer to queue structure
 *  node	New node to add to the queue
 */
void queue_add(QUEUE * q, QNODE * node)
{
	if (queue_isempty(q)) {
		q->head = q->tail = node;
	} else {
		q->tail->next = node;
		q->tail = node;
	}

	node->next = NULL;
}

/*
 * Remove and return first (oldest) entry from the specified queue
 *
 * Parameters:
 *  q		Pointer to queue structure
 *
 * Return Value:
 *  Node at head of queue - NULL if queue is empty
 */
QNODE *queue_remove(QUEUE * q)
{
	QNODE *oldest;

	if (queue_isempty(q))
		return NULL;

	oldest = q->head;
	q->head = oldest->next;
	return oldest;
}

/*
 * Peek into the queue and return pointer to first (oldest) entry.
 * The queue is not modified
 *
 * Parameters:
 *  q		Pointer to queue structure
 *
 * Return Value:
 *  Node at head of queue - NULL if queue is empty
 */
QNODE *queue_peek(QUEUE * q)
{
	return q->head;
}

/*
 * Move entire contents of one queue to the other
 *
 * Parameters:
 *  src		Pointer to source queue
 *  dst		Pointer to destination queue
 */
void queue_move(QUEUE * dst, QUEUE * src)
{
	if (queue_isempty(src))
		return;

	if (queue_isempty(dst))
		dst->head = src->head;
	else
		dst->tail->next = src->head;

	dst->tail = src->tail;
	src->head = NULL;
	return;
}
