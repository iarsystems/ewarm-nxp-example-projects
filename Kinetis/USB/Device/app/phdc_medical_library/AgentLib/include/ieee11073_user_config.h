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
 * @file ieee11073_user_config.h
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
#ifndef _IEEE11073_USER_CONFIG_H 
#define _IEEE11073_USER_CONFIG_H 
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <mds_config.h>

/******************************************************************************
 * Macros
 *****************************************************************************/
/* User Defined MACRO to set number of Timer Objects */
#define MAX_TIMER_OBJECTS		    10

#if MAX_TIMER_OBJECTS
	/* When Enabled Timer Callback is invoked with an argument */
	#define TIMER_CALLBACK_ARG 
#endif

#endif /* _IEEE11073_USER_CONFIG_H */