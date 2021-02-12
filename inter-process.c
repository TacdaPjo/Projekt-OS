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
#pragma endregion reg

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

exception mailbox_enqueue(mailbox *mBox, msg *msg)
{


}

exception mailbox_dequeue(mailbox *mBox, void *pData, msg *msg)
{
    if (msg->Status == RECEIVER)
    {
        if (memcpy(msg->pData, pData, mBox->nDataSize) == FAIL)
        {
            return FAIL;
        }
        if (findTask(waitList, msg->pBlock->pTask))
        {
            addTaskToList(readyList, msg->pBlock->pTask);
            removeMsg(waitList, msg->pBlock->pTask);
            
            return OK;
        }
        return OK;
    }
}

exception send_wait(mailbox *mBox, void *pData)
{
    isr_off();

    uint32_t msgstatus = 0;

    msg *temp = mBox->pHead;

    while (temp->pData != NULL)
    {
        if (temp->Status == RECEIVER)
        {
            msgstatus = 1;
            break;
        }
        temp = temp->pNext;

        if (msgstatus == 1)
        {
            if (memcpy(temp->pData, pData, mBox->nDataSize) == FAIL)
            {
                return FAIL;
            }
            temp->pBlock->pMessage = NULL;
        }
    }
    isr_on();
}

exception receive_wait(mailbox *mBox, void *Data)
{
}

exception send_no_wait(mailbox *mBox, void *Data)
{
}

exception receive_no_wait(mailbox *mBox, void *Data)
{
}