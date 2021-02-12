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
    msg *m = (msg *)allocSafe(sizeof m);
    if (m == NULL)
    {
        return NULL;
    }
    m->pData = data;
    return m;
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

msg *msg_enqueue(mailbox *mBox)
{
}

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
        msg *temp = mBox->pHead;
        if(memcpy(temp->pData, pData, temp->pBlock->pMessage)
    }
    else
    {
    }
    isr_on();
    return OK;
}

exception receive_wait(mailbox *mBox, void *Data)
{
}

exception send_no_wait(mailbox *mBox, void *Data)
{
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
    return OK;
}
