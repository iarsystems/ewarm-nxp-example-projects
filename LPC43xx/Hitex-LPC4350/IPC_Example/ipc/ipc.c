/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : ipc.c
 *    Description : Inter Process Communication (IPC) Library. Basic
 *                  implementation of NXP's IPC protocol.
 *
 *    History :
 *    1. Date        : December 2011
 *       Author      : Atanas Uzunov
 *       Description : initial revision
 *
 *    $Revision: 28 $
 **************************************************************************/
/** include files **/
#include <stdint.h>
#include "ipc.h"
#include "intrinsics.h"

/** public data **/

/*  Commands Control Block */
volatile ipcCommandControlBlock_t * ipcCCB;
/*  Messages Control Block */
volatile ipcMessageControlBlock_t * ipcMCB;
/** private data **/

/** private functions **/

#if defined (__IPC_MASTER__)

/*************************************************************************
 * Function Name: __ipcGetCommandSize
 * Parameters: type - type of the command
 * Return: uint8_t: command size in words
 *
 * Description: Returns the size of command with the given type
 *
 *************************************************************************/
uint8_t __ipcGetCommandSize(ipcCommands_t type)
{
  uint8_t sz;
  switch(type)
  {
    case CMD_RD_ID:
              sz = 1;
              break;
    case CMD_WR_ID:
              sz = 2;
              break;
    default:
              sz = 0;
              break;
  }
  return sz;
}

/*************************************************************************
 * Function Name: __ipcQueryCommandQueue
 * Parameters: commandSize - size of command
 * Return: ipcQueueStatus_t: Queue status
 *
 * Description: Verifies if command with the given size can be pushed into
 *              the queue.
 *
 *************************************************************************/
ipcQueueStatus_t __ipcQueryCommandQueue(uint8_t commandSize)
{
  ipcCommand_t *sa, *ea, *wp, *rp, *ta;
  sa = ipcCCB->startAddress;
  ea = ipcCCB->endAddress;
  wp = ipcCCB->writePtr;
  rp = ipcCCB->readPtr;

  if (wp == rp) return QS_EMPTY;

  ta = wp + commandSize;

  /* if the target address is beyond the end address, roll back to start address */
  if (ta > ea)
  {
     ta = sa + (ta - ea) - 1;

     if (wp > rp)
     {
       /* target write pointer should not be equal to the read pointer */
       if (ta >= rp) return QS_FULL;
     }
     else return QS_FULL;
  }
  else
  {
     if ((wp < rp) && (ta >= rp)) return QS_FULL;
  }

  return QS_SUCCESS;
}

#elif defined (__IPC_SLAVE__)

/*************************************************************************
 * Function Name: __ipcGetMessageSize
 * Parameters: type - type of the message
 * Return: uint8_t: message size in words
 *
 * Description: Returns the size of message with the given type
 *
 *************************************************************************/
uint8_t __ipcGetMessageSize(ipcMessages_t type)
{
  uint8_t sz;
  switch(type)
  {
    case MSG_SRV_ID:
    case MSG_RD_STS_ID_INV_ARG:
    case MSG_RD_STS_ID_RES1:
    case MSG_RD_STS_ID_RES2:
    case MSG_WR_STS_ID_SUCCESS:
    case MSG_WR_STS_ID_FAIL:
    case MSG_WR_STS_ID_RES1:
              sz = 1;
              break;
    case MSG_RD_ID:
              sz = 2;
              break;
    default:
              sz = 0;
              break;
  }
  return sz;
}

/*************************************************************************
 * Function Name: __ipcQueryMessageQueue
 * Parameters: messageSize - size of message
 * Return: ipcQueueStatus_t: Queue status
 *
 * Description: Verifies if message with the given size can be pushed into
 *              the queue.
 *
 *************************************************************************/
ipcQueueStatus_t __ipcQueryMessageQueue(uint8_t messageSize)
{
  ipcMessage_t *sa, *ea, *wp, *rp, *ta;
  sa = ipcMCB->startAddress;
  ea = ipcMCB->endAddress;
  wp = ipcMCB->writePtr;
  rp = ipcMCB->readPtr;

  if (wp == rp) return QS_EMPTY;

  ta = wp + messageSize;

  /* if the target address is beyond the end address, roll back to start address */
  if (ta > ea)
  {
     ta = sa + (ta - ea) - 1;

     if (wp > rp)
     {
       /* target write pointer should not be equal to the read pointer */
       if (ta >= rp) return QS_FULL;
     }
     else return QS_FULL;
  }
  else
  {
     if ((wp < rp) && (ta >= rp)) return QS_FULL;
  }

  return QS_SUCCESS;
}

#endif

/** public functions **/

#if defined (__IPC_MASTER__)

/*************************************************************************
 * Function Name: ipcMasterPushCommand
 * Parameters: ipcCommand - pointer to command
 * Return: ipcQueueStatus_t: Queue status
 *
 * Description: Pushes one command into the command buffer
 *
 *************************************************************************/
ipcQueueStatus_t ipcMasterPushCommand(ipcValueCommand_t * ipcCommand)
{
  uint8_t commandSize;
  ipcCommand_t *sa, *ea, *wp;
  /* Get the command type and size */
  if (ipcCommand->type >= NUMBER_OF_COMMANDS) return QS_ERROR;
  commandSize = __ipcGetCommandSize(ipcCommand->type);
  sa = ipcCCB->startAddress;
  ea = ipcCCB->endAddress;
  wp = ipcCCB->writePtr;
  if (__ipcQueryCommandQueue(commandSize) == QS_FULL) return QS_FULL;
  /* Write the first word of command */
  *wp = *((ipcCommand_t*)ipcCommand);
  if (++wp == ea) wp = sa;
  /* If the command is 2-word long */
  if (ipcCommand->type == CMD_WR_ID)
  {
    /* Write the second word */
    *((uint32_t*)wp) = ipcCommand->valueWord;
    if (++wp == ea) wp = sa;
  }
  /* Update the write-pointer */
  ipcCCB->writePtr = wp;
  return QS_SUCCESS;
}

/*************************************************************************
 * Function Name: ipcMasterPopMessage
 * Parameters: Message popperd from the buffer
 * Return: Queue status
 *
 * Description: Pop message from message buffer
 *
 *************************************************************************/
ipcQueueStatus_t ipcMasterPopMessage(ipcValueMessage_t *ipcMessage)
{
  ipcMessages_t messageType;
  ipcMessage_t *sa, *ea, *wp, *rp;
  sa = ipcMCB->startAddress;
  ea = ipcMCB->endAddress;
  wp = ipcMCB->writePtr;
  rp = ipcMCB->readPtr;
  if (wp == rp) return QS_EMPTY;
  messageType = rp->type;
  if (messageType >= NUMBER_OF_MESSAGES) return QS_ERROR;
  /* Extract the first word of the message */
  *((ipcMessage_t*)ipcMessage) = *rp;
    if (++rp == ea) rp = sa;
  /* If the message is 2-words long */
  if (messageType == MSG_RD_ID)
  {
    if (rp == wp) return QS_ERROR;
    /* Extract the second word of the message */
    ipcMessage->valueWord = *((uint32_t*)rp);
    if (++rp == ea) rp = sa;
  }
  /* Update the read-pointer */
  ipcMCB->readPtr = rp;
  return QS_SUCCESS;
}

/*************************************************************************
 * Function Name: ipcQueueHasPendingMessages
 * Parameters: none
 * Return: 1 - has pending messages
 *         0 - no pending messages
 *
 * Description: Checks message queue for pending messages
 *
 *************************************************************************/
uint8_t ipcQueueHasPendingMessages()
{
  ipcMessage_t *rp = ipcMCB->readPtr;
  ipcMessage_t *wp = ipcMCB->writePtr;
  if (rp == wp) return 0;
  else return 1;
}

/*************************************************************************
 * Function Name: ipcMasterInitQueue
 * Parameters: cmd_cb - address of command control block,
 *             cmd_buf - address of command buffer,
 *             cmd_size - command buffer length,
 *             msg_cb - address of message control block,
 *             msg_buf - address of message buffer,
 *             msg_size - message buffer length
 *
 * Return: none
 *
 * Description: Master initialization of command and message queues
 *
 *************************************************************************/
void ipcMasterInitQueues(ipcCommandControlBlock_t * cmd_cb, ipcCommand_t * cmd_buf, uint32_t cmd_size, ipcMessageControlBlock_t * msg_cb, ipcMessage_t * msg_buf, uint32_t msg_size)
{
  /* Initialize the Command Control Block */
  ipcCCB = cmd_cb;
  ipcCCB->startAddress = cmd_buf;
  ipcCCB->endAddress = (ipcCommand_t*)((uint32_t)cmd_buf + cmd_size);
  ipcCCB->readPtr = cmd_buf;
  ipcCCB->writePtr = cmd_buf;
  /* Initialize the Message Control Block */
  ipcMCB = msg_cb;
  ipcMCB->startAddress = msg_buf;
  ipcMCB->endAddress = (ipcMessage_t*)((uint32_t)msg_buf + msg_size);
  ipcMCB->readPtr = msg_buf;
  ipcMCB->writePtr = msg_buf;
}

/*************************************************************************
 * Function Name: ipcNotifySlave
 * Parameters: none
 *
 * Return: none
 *
 * Description: Notifies slave that there is pending command on the queue
 *
 *************************************************************************/
void ipcNotifySlave(void)
{
  __DSB();
  __SEV();
}

#elif defined (__IPC_SLAVE__)

/*************************************************************************
 * Function Name: ipcSlavePopCommand
 * Parameters: ipcCommand - pointer for storing the command
 * Return: ipcQueueStatus_t: Queue status
 *
 * Description: Pops one command from the command buffer
 *
 *************************************************************************/
ipcQueueStatus_t ipcSlavePopCommand(ipcValueCommand_t * ipcCommand)
{
  ipcCommands_t commandType;
  ipcCommand_t *sa, *ea, *wp, *rp;
  sa = ipcCCB->startAddress;
  ea = ipcCCB->endAddress;
  wp = ipcCCB->writePtr;
  rp = ipcCCB->readPtr;
  if (wp == rp) return QS_EMPTY;
  commandType = rp->type;
  if (commandType >= NUMBER_OF_COMMANDS) return QS_ERROR;
  /* Get the first word of command */
  *((ipcCommand_t*)ipcCommand) = *rp;
  if (++rp == ea) rp = sa;
  /* If the command is 2-word long */
  if (commandType == CMD_WR_ID)
  {
    if (rp == wp) return QS_ERROR;
    /* get the second word */
    ipcCommand->valueWord = *((uint32_t*)rp);
    if (++rp == ea) rp = sa;
  }
  /* Update the read-pointer */
  ipcCCB->readPtr = rp;
  return QS_SUCCESS;
}

/*************************************************************************
 * Function Name: ipcSlavePushMessage
 * Parameters: ipcMessage - pointer to the message
 * Return: ipcQueueStatus_t: Queue status
 *
 * Description: Push message to the message buffer
 *
 *************************************************************************/
ipcQueueStatus_t ipcSlavePushMessage(ipcValueMessage_t *ipcMessage)
{
  ipcMessages_t messageType;
  uint8_t messageSize;
  ipcMessage_t *sa, *ea, *wp;
  /* Get message type and size*/
  messageType = ipcMessage->type;
  if (messageType >= NUMBER_OF_MESSAGES) return QS_ERROR;
  messageSize = __ipcGetMessageSize(messageType);
  sa = ipcMCB->startAddress;
  ea = ipcMCB->endAddress;
  wp = ipcMCB->writePtr;
  /* Check if there is enough space for the message */
  if (__ipcQueryMessageQueue(messageSize) == QS_FULL) return QS_FULL;
  /* Write the first word of the message */
  *wp = *((ipcMessage_t*)ipcMessage);
  if (++wp == ea) wp = sa;
  /* If the message is 2-words long */
  if (messageType == MSG_RD_ID)
  {
    /* Write the second word of the message */
    *((uint32_t*)wp) = ipcMessage->valueWord;
    if (++wp == ea) wp = sa;
  }
  /* Update the write-pointer */
  ipcMCB->writePtr = wp;
  return QS_SUCCESS;
}

/*************************************************************************
 * Function Name: ipcQueueHasPendingCommands
 * Parameters: none
 * Return: uint8_t: 1 - has pending commands
 *                  0 - no pending commands
 *
 * Description: Checks command queue for pending commands
 *
 *************************************************************************/
uint8_t ipcQueueHasPendingCommands()
{
  ipcCommand_t *rp = ipcCCB->readPtr;
  ipcCommand_t *wp = ipcCCB->writePtr;
  if (rp == wp) return 0;
  else return 1;
}


/*************************************************************************
 * Function Name: ipcSlaveInitQueues
 * Parameters: cmd_cb - address of command control block,
 *             msg_cb - address of message control block
 *
 * Return: none
 *
 * Description: Slave initialization of command amd message queues
 *
 *************************************************************************/
void ipcSlaveInitQueues(ipcCommandControlBlock_t * cmd_cb, ipcMessageControlBlock_t * msg_cb)
{
  ipcCCB = cmd_cb;
  ipcMCB = msg_cb;
}

/*************************************************************************
 * Function Name: ipcNotifyMaster
 * Parameters: none
 *
 * Return: none
 *
 * Description: Notifies master that there is pending message on the queue
 *
 *************************************************************************/
void ipcNotifyMaster(void)
{
  __DSB();
  __SEV();
}

#endif


