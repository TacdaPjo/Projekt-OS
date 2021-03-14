#include "data.h"

#pragma region reg
mailbox *create_mailbox(uint maxMsg, uint dataSize);
exception remove_mailbox(mailbox *mBox);
exception send_wait(mailbox *mBox, void *Data);
exception receive_wait(mailbox *mBox, void *Data);
exception send_no_wait(mailbox *mBox, void *Data);
exception receive_no_wait(mailbox *mBox, void *Data);
exception removes_whole_mailbox(mailbox *mBox);
exception wait(uint nTicks);

#pragma endregion reg

// nof_msg: Maximum number of Messages the Mailbox can hold.
// Size_of msg: The size of one Message in the Mailbox
mailbox *create_mailbox(uint nMessages, uint dataSize)
{
    mailbox *mBox;
    mBox = (mailbox *)allocSafe(sizeof(mailbox));
    if (mBox == NULL)
        return NULL;

    mBox->nDataSize = dataSize;
    mBox->nMaxMessages = nMessages;
    msg *head;
    if (head == NULL)
        return FAIL;

    mBox->pHead = head;
    mBox->pTail = head;
    mBox->pTail->pPrevious = mBox->pHead;
    mBox->pHead->pNext = mBox->pTail;
    return mBox;
}

exception remove_mailbox(mailbox *mBox)
{

    if (mBox->nMessages == 0)
    {
        memoryFree(mBox);
        return OK;
    }

    return NOT_EMPTY;
}

/* exception removes_whole_mailbox(mailbox *mBox)
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
} */

exception send_wait(mailbox *mBox, void *pData)
{
    isr_off(); //Disable interrupt
    uint32_t msgstatus = 0;
    msg *tempMsg;

    if (mBox->pHead != NULL && mBox->pHead->Status == RECEIVER) //Receving task is waiting
        msgstatus = OK;
    else
        msgstatus = FAIL;

    if (msgstatus)
    {

        memcpy(mBox->pHead->pData, pData, mBox->nDataSize); //Copy sender’s data to the data area of the receivers Message

        tempMsg = msg_dequeue(mBox); // Remove receiving task’s Message struct from the mailbox

        PreviousTask = NextTask; //Update PreviousTask

        push(ReadyList, moveFromList(WaitingList, tempMsg->pBlock)); //Insert receiving task in ReadyList

        NextTask = ReadyList->pHead->pTask; //Update NextTask
    }

    else
    {
        tempMsg = allocSafe(sizeof(msg)); //Allocate a Message structure
        if (tempMsg == NULL)
        {
            return FAIL;
        }

        tempMsg->pData = allocSafe(mBox->nDataSize); //Set data Pointer

        memcpy(tempMsg->pData, pData, mBox->nDataSize);

        tempMsg->Status = SENDER;

        tempMsg->pBlock = ReadyList->pHead;

        msg_enqueue(mBox, tempMsg); // Add Message to the Mailbox

        PreviousTask = NextTask; //Update PreviousTask

        push(WaitingList, pop(ReadyList)); //Move sending task from ReadyList to WaitingList

        NextTask = ReadyList->pHead->pTask; //Update NextTask
    }

    SwitchContext();

    if (tickCounter >= ReadyList->pHead->pTask->Deadline) //deadline is reached
    {
        isr_off(); //Disable interrupt

        memoryFree(tempMsg);

        msg_dequeue(mBox); //Remove send Message

        isr_on(); //Enable interrupt

        return DEADLINE_REACHED;
    }
    else
        return OK;
}

exception receive_wait(mailbox *mBox, void *pData)
{
    if (mBox == NULL || pData == NULL)
        return NULL;

    isr_off(); // Disable interrupt
    uint32_t msgstatus = 0;
    msg *tempMsg;

    if (mBox->pHead != NULL && mBox->pHead->Status == SENDER) //send Message is waiting
        msgstatus = OK;
    else
        msgstatus = FAIL;

    if (msgstatus)
    {

        tempMsg = allocSafe(sizeof(msg)); //Allocate a Message structure
        if (tempMsg == NULL)
        {
            return FAIL;
        }

        memcpy(pData, mBox->pHead->pData, mBox->nDataSize); //Copy sender’s data to receiving task’s data area

        tempMsg = msg_dequeue(mBox); //Remove sending task’s Message struct from the Mailbox

        if (tempMsg->pBlock != NULL)
        {

            PreviousTask = NextTask; //Update PreviousTask

            push(ReadyList, moveFromList(WaitingList, tempMsg->pBlock)); //Move sending task to ReadyList

            NextTask = ReadyList->pHead->pTask; //Update NextTask
        }
        else
        {
            memoryFree(tempMsg);
        }
    }

    else
    {
        tempMsg = allocSafe(sizeof(msg)); //Allocate a Message structure
        if (tempMsg == NULL)
        {
            return FAIL;
        }

        tempMsg->pData = allocSafe(mBox->nDataSize); //Set data Pointer

        tempMsg->Status = RECEIVER;

        tempMsg->pBlock = ReadyList->pHead;

        msg_enqueue(mBox, tempMsg); // Add Message to the Mailbox

        PreviousTask = NextTask; //Update PreviousTask

        push(WaitingList, moveFromList(ReadyList, mBox->pHead->pBlock)); //Move sending task from ReadyList to WaitingList

        NextTask = ReadyList->pHead->pTask; //Update NextTask

        //memoryFree(tempMsg);
    }

    SwitchContext();
    if(NextTask->Deadline < tickCounter)
    //if (ReadyList->pHead->pTask->Deadline <= tickCounter)
    {
        isr_off();         //Disable interrupt
        msg_dequeue(mBox); //Remove receive Message
        //memoryFree(mBox->pTail);

        msg *delety = mBox->pTail;
        mBox->pTail = mBox->pTail->pPrevious;
        mBox->pTail->pNext = NULL;
        memoryFree(delety);

        isr_on(); //Enable interrupt
        return DEADLINE_REACHED;
    }

    else
        return OK;
}

exception send_no_wait(mailbox *mBox, void *pData)
{
    exception status = OK;

    if (mBox == NULL || pData == NULL)
        return NULL;

    isr_off(); // Disable interrupt
    uint32_t msgstatus = 0;
    msg *tempMsg2;

    if (mBox->pHead != NULL && mBox->pHead->Status == RECEIVER) // receiving task is waiting
        msgstatus = OK;
    else
        msgstatus = FAIL;

    if (msgstatus)
    {
        memcpy(mBox->pHead->pData, pData, mBox->nDataSize); // Copy data to receiving tasks’ data area.

        msg *tempMsg = msg_dequeue(mBox); // Remove receiving task’s Message struct from the Mailbox

        PreviousTask = NextTask; //Update PreviousTask

        push(ReadyList, moveFromList(WaitingList, tempMsg->pBlock));

        NextTask = ReadyList->pHead->pTask; // Update NextTask

        SwitchContext();
    }

    else
    {
        tempMsg2 = allocSafe(sizeof(msg)); //Allocate a Message structure

        if (tempMsg2 == NULL)
        {
            return FAIL;
        }

        tempMsg2->pData = allocSafe(mBox->nDataSize);

        if (tempMsg2->pData == NULL)
            return NULL;

        memcpy(tempMsg2->pData, pData, mBox->nDataSize);

        tempMsg2->Status = SENDER;

        //tempMsg2->pBlock = ReadyList->pHead;

        if (mBox->nMessages >= mBox->nMaxMessages) //mailbox is full
        {
            msg_dequeue(mBox); //Remove the oldest Message
        }

        msg_enqueue(mBox, tempMsg2); //Add Message to the Mailbox

        return status;
    }
}

exception receive_no_wait(mailbox *mBox, void *pData)
{
    exception status = OK;
    msg *tempMsg;

    if (mBox == NULL || pData == NULL)
        return NULL;

    isr_off(); //Disable interrupt
    uint32_t msgstatus = 0;

    if (mBox->pHead != NULL && mBox->pHead->Status == SENDER) //send Message is waiting
        msgstatus = OK;
    else
        msgstatus = FAIL;

    if (msgstatus)
    {
        memcpy(pData, mBox->pHead->pData, mBox->nDataSize); //Copy sender’s data to receiving task’s data area

        tempMsg = msg_dequeue(mBox); //Remove sending task’s Message struct from the Mailbox

        if (tempMsg->pBlock != NULL)
        {
            PreviousTask = NextTask; //Update PreviousTask

            push(ReadyList, moveFromList(WaitingList, mBox->pHead->pBlock)); //Move sending task to ReadyList

            NextTask = ReadyList->pHead->pTask; //Update NextTask

            SwitchContext(); //SwitchContext
        }
        else
        {

            memoryFree(tempMsg); //FUNKAR INTE????
            //FREE SENDERS's DATA AREA
        }
    }

    else
        return FAIL;

    return status;
}

exception wait(uint nTicks)
{

    isr_off(); //Disable interrupt

    PreviousTask = NextTask; //Update PreviousTask

    ReadyList->pHead->nTCnt = nTicks + ticks();

    timeListPush(pop(ReadyList)); //Place running task in the TimerList

    NextTask = ReadyList->pHead->pTask; //Update NextTask

    SwitchContext();

    if (deadline() <= tickCounter)
    {
        return DEADLINE_REACHED;
    }
    else
    {
        return OK;
    }
}