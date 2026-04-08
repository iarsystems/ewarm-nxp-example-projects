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
 * File:        alloc.c
 * Purpose:     generic malloc() and free() engine
 *
 * Notes:       99% of this code stolen/borrowed from the K&R C
 *              examples.
 *
 */

#include "common.h"
#include "stdlib.h"

#pragma section = "HEAP"

/*
 * This struct forms the minimum block size which is allocated, and
 * also forms the linked list for the memory space used with alloc()
 * and free().  It is padded so that on a 32-bit machine, all malloc'ed
 * pointers are 16-byte aligned.
 */
typedef struct ALLOC_HDR {
	struct {
		struct ALLOC_HDR *ptr;
		unsigned int size;
	} s;
	unsigned int align;
	unsigned int pad;
} ALLOC_HDR;

static ALLOC_HDR base;
static ALLOC_HDR *freep = NULL;

void free(void *ap)
{
	ALLOC_HDR *bp, *p;

	bp = (ALLOC_HDR *) ap - 1;	/* point to block header */
	for (p = freep; !((bp > p) && (bp < p->s.ptr)); p = p->s.ptr) {
		if ((p >= p->s.ptr) && ((bp > p) || (bp < p->s.ptr))) {
			break;				/* freed block at start or end of arena */
		}
	}

	if ((bp + bp->s.size) == p->s.ptr) {
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else {
		bp->s.ptr = p->s.ptr;
	}

	if ((p + p->s.size) == bp) {
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	} else {
		p->s.ptr = bp;
	}

	freep = p;
}

void *malloc(unsigned nbytes)
{
	/* Get addresses for the HEAP start and end */
#if defined(CW)
	extern char __HEAP_START[];
	extern char __HEAP_END[];
#elif defined(IAR)
	char *__HEAP_START = __section_begin("HEAP");
	char *__HEAP_END = __section_end("HEAP");
#endif

	ALLOC_HDR *p, *prevp;
	unsigned nunits;

	nunits = ((nbytes + sizeof(ALLOC_HDR) - 1) / sizeof(ALLOC_HDR)) + 1;

	if ((prevp = freep) == NULL) {
		p = (ALLOC_HDR *) __HEAP_START;
		p->s.size = (((uint32) __HEAP_END - (uint32) __HEAP_START)
					 / sizeof(ALLOC_HDR));
		p->s.ptr = &base;
		base.s.ptr = p;
		base.s.size = 0;
		prevp = freep = &base;
	}

	for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr) {
		if (p->s.size >= nunits) {
			if (p->s.size == nunits) {
				prevp->s.ptr = p->s.ptr;
			} else {
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			}
			freep = prevp;
			return (void *)(p + 1);
		}

		if (p == freep)
			return NULL;
	}
}
