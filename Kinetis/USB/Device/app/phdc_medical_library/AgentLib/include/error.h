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
 * @file error.h
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file Defines Error Codes returned from Ieee11073 Agent Library
 *
 *****************************************************************************/
#ifndef _ERROR_H
#define _ERROR_H
/******************************************************************************
 * Types
 *****************************************************************************/
/* Error Codes */
typedef enum _ERR_CODE
{
	ERROR_SUCCESS,
	ERROR_UNSOLICITED = 0x80,
	ERROR_INVALID_PARAM,
	ERROR_INVALID_REQUEST,
	ERROR_UNSUPPORTED,
	ERROR_INSUFFICIENT_MEMORY,
	ERROR_ALREADYREGISTERED,
	ERROR_NOTSUPPORTED,
	ERROR_UNINITIALIZED_SHIM,
	ERROR_SHIM_INIT_FAILED,
	ERROR_SEND_FAILED,
	ERROR_RECV_FAILED,
	ERROR_GENERAL,
    ERROR_NO_MORE_CONFIGURATIONS,
    ERROR_OBJECT_BUSY,
    ERROR_INVALID_DATA,
    ERROR_DIM_INIT_FAILED
}ERR_CODE;
#endif /* _ERROR_H */