#include "kernel.h"

list *create_list()
{
    list *mylist = (list *)secureCalloc(1, sizeof(list));
    if (mylist == NULL)
    {
        return NULL;
    }

    mylist->pHead = (listobj *)secureCalloc(1, sizeof(listobj));
    if (mylist->pHead == NULL)
    {
        freeWith_isr_off_on(mylist);
        return NULL;
    }

    mylist->pTail = (listobj *)secureCalloc(1, sizeof(listobj));
    if (mylist->pTail == NULL)
    {
        freeWith_isr_off_on(mylist->pHead->pTask);
        freeWith_isr_off_on(mylist->pHead);
        freeWith_isr_off_on(mylist);
        return NULL;
    }

    mylist->pHead->pPrevious = mylist->pHead; // fan va weird
    // mylist->pHead->pNext = mylist->pTail; // head.next pekar på tail box, good shit
    mylist->pTail->pPrevious = mylist->pHead; // tail.prev pekar på head box, good shit
    mylist->pHead->pNext = mylist->pTail;     // head.prev pekar på tail box
    // mylist->pTail->pNext = mylist->pHead; // ptail.next pekar på head box, good shit
    mylist->pTail->pNext = mylist->pTail; // fan va weird
    return mylist;
}

listobj *create_listobj_nTCnt(int num)
{
    listobj *myobj = (listobj *)secureCalloc(1, sizeof(listobj));

    if (myobj == NULL)
    {
        return NULL;
    }

    myobj->pTask = (TCB *)secureCalloc(1, sizeof(TCB));

    if (myobj->pTask == NULL)
    {
        freeWith_isr_off_on(myobj);
        return NULL;
    }

    myobj->nTCnt = num;

    myobj->pTask->DeadLine = num * 100;
    return (myobj);
}

listobj *create_listobj_dl(int num)
{
    listobj *myobj = (listobj *)secureCalloc(1, sizeof(listobj));
    if (myobj == NULL)
    {
        return NULL;
    }
    myobj->pTask = (TCB *)secureCalloc(1, sizeof(TCB));

    myobj->pTask->DeadLine = num;

    return (myobj);
}

/*
Skapar insert, sorteras efter deadline
*/
void insertSortByDeadLine(list *mylist, listobj *object)
{

    if (object != NULL)
    {
        listobj *temp = mylist->pHead;

        while (temp->pNext->pTask->DeadLine < object->pTask->DeadLine)
        {
            if (temp->pNext == mylist->pTail)
            {
                break;
            }
            temp = temp->pNext;
        }

        object->pNext = temp->pNext;
        object->pPrevious = temp;
        temp->pNext->pPrevious = object;
        temp->pNext = object;
    }
}

/*
Skapar insert, sorteras efter nTCnt.
*/
void insertSortBynTCnt(list *mylist, listobj *object)
{

    if (object != NULL)
    {
        listobj *temp = mylist->pHead;

        while (temp->pNext->nTCnt < object->nTCnt && temp->pNext != mylist->pTail)
        {
            temp = temp->pNext;
            /*
      if(temp->pNext == mylist->pTail) {
        break;
      }
      */
        }

        object->pNext = temp->pNext;
        object->pPrevious = temp;
        //temp->pNext = object;
        temp->pNext->pPrevious = object;
        temp->pNext = object;
    }
}

/*
Extraherar en nod om det finns någon mellan head och tail.
*/
listobj *extractionFirst(list *mylist)
{
    listobj *tempo;
    if (mylist->pHead->pNext != mylist->pTail)
    {
        tempo = mylist->pHead->pNext;
        /*
    mylist->pHead->pNext = mylist->pHead->pNext->pNext;
    snyggas till med tempo
    */
        tempo->pPrevious->pNext = tempo->pNext; // head.next = head.next.next
        /*
    mylist->pHead->pNext->pNext->pPrevious = mylist->pHead->pNext->pPrevious;
    snyggas till med tempo
    */
        tempo->pNext->pPrevious = tempo->pPrevious;
        /*
    noden som extraheras refererar till sig självt
    */
        tempo->pNext = tempo;
        tempo->pPrevious = tempo;

        return tempo;
    }
    return NULL;
}

listobj *extractionAnyObj(listobj *object)
{

    if (object->pNext != object->pPrevious && object->pPrevious != object->pNext)
    {

        object->pPrevious->pNext = object->pNext;
        object->pNext->pPrevious = object->pPrevious;

        object->pNext = object;
        object->pPrevious = object;

        return object;
    }
    return NULL;
}

exception remove_list(list *myList)
{
    exception status = OK;
    if (myList->pHead->pNext == myList->pTail)
    {
        freeWith_isr_off_on(myList->pHead);
        freeWith_isr_off_on(myList->pTail);
        freeWith_isr_off_on(myList);
        return status;
    }
    return NOT_EMPTY;
}