#include "data.h"

#pragma region reg
mailbox *create_mailbox(uint maxMsg, uint dataSize);
exception remove_mailbox(mailbox *mBox);
exception send_wait(mailbox *mBox, void *Data);
exception receive_wait(mailbox *mBox, void *Data);
exception send_no_wait(mailbox *mBox, void *Data);
exception receive_no_wait(mailbox *mBox, void *Data);
msg *createMsg(void *data, i32 size);
exception removes_whole_mailbox(mailbox *mBox);
exception mailbox_dequeue(mailbox *mBox, void *pData, msg *msg);
exception mailbox_enqueue(mailbox *mBox, msg *msg);
#pragma endregion reg

exception isEmpty(mailbox *mBox)
{
    return mBox->pHead->pNext == mBox->pHead;
}

msg *createMsg(void *data, i32 size)
{
    msg *msg = allocSafe(sizeof msg);
    if (msg == NULL)
    {
        return NULL;
    }
    msg->pData = data;
    return msg;
}

// nof_msg: Maximum number of Messages the Mailbox can hold.
// Size_of msg: The size of one Message in the Mailbox
mailbox *create_mailbox(uint nMessages, uint dataSize)
{
    mailbox *mBox;
    mBox = (mailbox *)allocSafe(sizeof(mailbox));
    if (mBox == NULL)
        return NULL;
    //mBox->pHead = allocSafe(sizeof *mBox->pHead);

    // mBox->pHead->pNext = mBox->pHead;
    // mBox->pHead->pPrevious = mBox->pHead;

    mBox->nDataSize = dataSize;
    mBox->nMessages = nMessages;

    return mBox;
}

exception remove_mailbox(mailbox *mBox)
{
    if (mBox->nMessages == 0)
        memoryFree(mBox);
    return OK;

    return NOT_EMPTY;
}

exception removes_whole_mailbox(mailbox *mBox)
{
    msg *temp = mBox->pHead;

    if (mBox->nMessages == 0)
        memoryFree(mBox);
    return OK;

    while (temp != NULL)
    {
        if (temp->pNext == NULL)
        {
            memoryFree(mBox);
            return OK;
        }

        temp = temp->pNext;
        memoryFree(temp->pPrevious);
        memoryFree(mBox);
        return OK;
    }
}

//Enqueue from the tail
exception msg_enqueue(mailbox *mBox, msg *msg)
{

    if (msg == NULL || mBox == NULL)
        return NULL;

    if (mBox->pTail == NULL)
    {
        mBox->pHead = mBox->pTail = msg;
    }

    mBox->pTail->pNext = msg;
    msg->pPrevious = mBox->pHead;
    mBox->pTail = msg;

    return OK;
}

//dequeue from the head
msg *msg_dequeue(mailbox *mBox)
{
    if (mBox == NULL || mBox->pHead == NULL)
        return NULL;
    msg *msg = mBox->pHead;
    if (mBox->pHead == mBox->pTail)
    {
        mBox->pHead == NULL;
        mBox->pTail == NULL;
        return msg;
    }
    mBox->pHead = mBox->pHead->pNext;
    mBox->pHead->pPrevious = NULL;

    return msg;
}

exception send_wait(mailbox *mBox, void *pData)
{
    isr_off();
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == RECEIVER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }

    else
    {
        msg *msg = allocSafe(sizeof msg);
        msg->pData = pData;
        msg->pBlock = ReadyList->pHead;
        msg->Status = SENDER;

        msg_enqueue(mBox, msg);

        PreviousTask = ReadyList->pHead->pTask;
        moveListObj(ReadyList, WaitingList, msg->pBlock);
        NextTask = ReadyList->pHead->pTask;
    }

    isr_on();
    SwitchContext();

    if (deadlineReached(NextTask))
    {
        isr_off();

        msg_dequeue(mBox);

        isr_on();

        return DEADLINE_REACHED;
    }
    return OK;
}

exception receive_wait(mailbox *mBox, void *Data)
{
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
    isr_off();
    if (mBox == NULL)
        return NULL;
    uint32_t msgstatus = 0;

    if (mBox->pHead == NULL && mBox->pHead->Status == SENDER)
        return msgstatus = FAIL;
    else
        return msgstatus = OK;
    if (msgstatus)
    {
        if (memcpy(mBox->pHead->pData, pData, mBox->nDataSize))
            isr_on();

        msg *msg = msg_dequeue(mBox);

        PreviousTask = NextTask;

        moveListObj(WaitingList, ReadyList, msg->pBlock);

        NextTask = ReadyList->pHead->pTask;
    }
}

exception send_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    exception status = OK;
    return status;
}
