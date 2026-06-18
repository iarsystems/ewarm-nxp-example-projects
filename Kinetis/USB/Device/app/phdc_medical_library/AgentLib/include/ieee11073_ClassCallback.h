/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2009 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 ******************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
 * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************//*!
 *
 * @file ieee11073_ClassCallback.h
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief 
 *
 *****************************************************************************/
#ifndef _IEEE11073_CLASS_CALLBACK_H
#define _IEEE11073_CLASS_CALLBACK_H
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <til.h>
#include <RealTimerCounter.h>
/*****************************************************************************
 * Global Function Prototypes
 *****************************************************************************/
/* This funtion calls the Timer api to remove the timer from the queue */
extern uint_8 PHD_Remove_Timer(
	uint_8_ptr pindex); 

/* This funtion calls the Timer api to add a timer to the queue */
extern uint_8 PHD_Add_Timer(
	PTIMER_OBJECT pTimerObject);

/* This callback function is called from SHIM */	
extern void* Ieee11073PHDClassCallback(
	TRANSPORTEVENTID eEventId,
	void* pArg);

#endif /* _IEEE11073_CLASS_CALLBACK_H */