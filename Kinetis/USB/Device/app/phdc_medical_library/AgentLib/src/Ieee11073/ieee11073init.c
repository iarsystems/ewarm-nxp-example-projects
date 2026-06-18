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
 * @file Ieee11073Init.c
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains IEEE11073 Agent Library Initialization
 *
 *****************************************************************************/
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <type.h>
#include <error.h>
#include <stack.h>
#include <til.h>
#include <ieee11073.h>
#include <ieee11073_dimfuncproto.h>
#include <ieee11073_ClassCallback.h>
/*****************************************************************************
 * Macro's
 *****************************************************************************/
#define SEND_QOS	0x8
#define RECV_QOS	0x8
/*****************************************************************************
 * Local Variables - None
 *****************************************************************************/

/*****************************************************************************
 * Local Functions Prototypes - None
 *****************************************************************************/

 /*****************************************************************************
 * Local Functions - None
 *****************************************************************************/

/*****************************************************************************
 * Global Variables 
 *****************************************************************************/
/* Stores Association Layer Initiailization Structure */
volatile ASSOC_STRUCT g_AssocStruct;

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  Ieee11073Initialize
 *
 * @brief This funtion initializes IEEE11073 Agent Library
 *
 * @param pTil				:	Pointer to TIL
 * @param pShim				:	Pointer to SHIM
 * @param mempooldesc		:	Memory Pool Descriptor
 * @param SendQos			:	Send QOS
 * @param RecvQos			:	Receive QOS
 * @param pfnAppCallback	:	Application Callback Pointer
 *
 * @return ERR_CODE	: ERROR_SUCCESS if Successful
 ******************************************************************************
 * This funtion initializes IEEE11073 Agent Library
 *****************************************************************************/
ERR_CODE Ieee11073Initialize(
	PTIL pTil,						/* [IN] Pointer to TIL */
	eShimID ShimID,					/* [IN] Pointer to SHIM */
	MED_APP_CALLBACK pfnAppCallback	/* [IN] Application Callback Pointer */
)
{
	g_AssocStruct.pTil = pTil;
	g_AssocStruct.ShimId = ShimID;
	g_AssocStruct.AssocState = DISCONNECTED;
	g_AssocStruct.SendQos = SEND_QOS;
	g_AssocStruct.RecvQos = RECV_QOS;
	g_AssocStruct.pfnAppCallback = pfnAppCallback;
	g_AssocStruct.TimerIndex = INVALID_TIMER_INDEX;
	/* Initialize and Start Transport */
	if(TIL_StartTransport(pTil, ShimID, &Ieee11073PHDClassCallback) == NULL)
		return ERROR_GENERAL;
	return ERROR_SUCCESS;
}

/**************************************************************************//*!
 *
 * @name  Ieee11073GetSendQos
 *
 * @brief This funtion returns Send Data QOS
 *
 * @return Send Data QOS value
 ******************************************************************************
 * This funtion returns Send Data QOS
 *****************************************************************************/
intu8 Ieee11073GetSendQos(void)
{
	return g_AssocStruct.SendQos;
}

/**************************************************************************//*!
 *
 * @name  Ieee11073GetRecvQos
 *
 * @brief This funtion returns Receive Data QOS
 *
 * @return Receive Data QOS value
 ******************************************************************************
 * This funtion returns Receive Data QOS
 *****************************************************************************/
intu8 Ieee11073GetRecvQos(void)
{
	return g_AssocStruct.RecvQos;
}

/**************************************************************************//*!
 *
 * @name  Ieee11073SetSendQos
 *
 * @brief This funtion sets Send Data QOS
 *
 * @param SendQos		:	Send QOS
 *
 * @return None
 ******************************************************************************
 * This funtion sets Send Data QOS
 *****************************************************************************/
void Ieee11073SetSendQos(
	intu8 SendQos		/* [IN] Send QOS */
)
{
	g_AssocStruct.SendQos = SendQos;
}

/**************************************************************************//*!
 *
 * @name  Ieee11073SetRecvQos
 *
 * @brief This funtion sets Receive Data QOS
 *
 * @param RecvQos		:	Receive QOS
 *
 * @return None
 ******************************************************************************
 * This funtion sets Receive Data QOS
 *****************************************************************************/
void Ieee11073SetRecvQos(
	intu8 RecvQos		/* [IN] Receive QOS */
)
{
	g_AssocStruct.RecvQos = RecvQos;
}

/**************************************************************************//*!
 *
 * @name  Ieee11073SetTimerIndex
 *
 * @brief This funtion sets Timer Index
 *
 * @param TimerIndex		:	Timer Index
 *
 * @return None
 ******************************************************************************
 * This funtion sets Timer Index
 *****************************************************************************/
void Ieee11073SetTimerIndex(
	intu8 TimerIndex	/* [IN] Timer Index */
)
{
    g_AssocStruct.TimerIndex = TimerIndex;
}

/**************************************************************************//*!
 *
 * @name  Ieee11073GetTimerIndex
 *
 * @brief This funtion returns Timer Index
 *
 * @return Timer Index
 ******************************************************************************
 * This funtion returns Timer Index
 *****************************************************************************/
intu8 Ieee11073GetTimerIndex(void)
{
    return g_AssocStruct.TimerIndex;
}

/**************************************************************************//*!
 *
 * @name  Ieee11073GetState
 *
 * @brief This funtion returns IEEE11073 Library State
 *
 * @return Timer Index
 ******************************************************************************
 * This funtion returns IEEE11073 Library State
 *****************************************************************************/
PHDCOMSTATE Ieee11073GetState(void)
{
	return g_AssocStruct.AssocState;
}

/**************************************************************************//*!
 *
 * @name  Ieee11073SetState
 *
 * @brief This funtion returns IEEE11073 Library State
 *
 * @param State		:	IEEE11073 Agent State
 *
 * @return Timer Index
 ******************************************************************************
 * This funtion returns IEEE11073 Library State
 *****************************************************************************/
void Ieee11073SetState(
	PHDCOMSTATE State	/* [IN] IEEE11073 Agent State */
)
{
	g_AssocStruct.AssocState = State;
}

/**************************************************************************//*!
 *
 * @name  Ieee11073GetTil
 *
 * @brief This funtion returns Pointer to TIL
 *
 * @return Pointer to TIL
 ******************************************************************************
 * This funtion returns Pointer to TIL
 *****************************************************************************/
PTIL Ieee11073GetTil(void)
{
	return g_AssocStruct.pTil;
}

/**************************************************************************//*!
 *
 * @name  Ieee11073GetShim
 *
 * @brief This funtion returns Pointer to Shim
 *
 * @return Shim ID
 ******************************************************************************
 * This funtion returns Pointer to Shim
 *****************************************************************************/
eShimID Ieee11073GetShim(void)
{
	return g_AssocStruct.ShimId;
}

/**************************************************************************//*!
 *
 * @name  Ieee11073GetAppCallback
 *
 * @brief This funtion returns Pointer to Application Callback Function
 *
 * @return Pointer to Application Callback Function
 ******************************************************************************
 * This funtion returns Pointer to Application Callback Function
 *****************************************************************************/
MED_APP_CALLBACK Ieee11073GetAppCallback(void)
{
    return g_AssocStruct.pfnAppCallback;
}

/**************************************************************************//*!
 *
 * @name  Ieee11073GetBuffStack
 *
 * @brief This funtion returns Pointer to Buffer Stack
 *
 * @return Pointer to Pointer to Buffer Stack
 ******************************************************************************
 * This funtion returns Pointer to Buffer Stack
 *****************************************************************************/
PTR_BUFFSTACK Ieee11073GetBuffStack(void)
{
    return g_AssocStruct.pBuffStack;
}

/**************************************************************************//*!
 *
 * @name  Ieee11073SetBuffStack
 *
 * @brief This funtion saves Pointer to Buffer Stack in Ieee11073 Structure
 *
 * @param pBuffStack	:	Pointer to Buffer Stack
 *
 * @return None
 ******************************************************************************
 * This funtion saves Pointer to Buffer Stack in Ieee11073 Structure
 *****************************************************************************/
void Ieee11073SetBuffStack(
	PTR_BUFFSTACK pBuffStack	/* [IN] Pointer to Buffer Stack */
)
{
    g_AssocStruct.pBuffStack = pBuffStack;
}
