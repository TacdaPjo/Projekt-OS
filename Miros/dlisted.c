#include "kernel.h"
#include "dlisted.h"

list *createList()
{
    list *myList = (list *)callocSec(1, sizeof(list));
    if(myList == NULL)
    {
        return NULL;
    }

    myList->pHead = (listobj *)callocSec(1,sizeof(listobj));
    if(myList->pHead == NULL)
    {
        
        freeIsOffOn(myList);
        return NULL;
    }
    
    myList->pTail = (listobj *)callocSec(1,sizeof(listobj));
    if(myList->pTail == NULL)
    {
        freeIsOffOn(myList->pHead->pTask);
        freeIsOffOn(myList->pHead);
        freeIsOffOn(myList);
        return NULL;
    }
    myList->pHead->pPrevious = myList->pHead;
    myList->pTail->pPrevious = myList->pHead;
    myList->pHead->pNext = myList->pTail;
    myList->pTail->pNext = myList->pTail;
    return myList;

}

exception remove_list(list *myList)
{
    exception status = OK;

    if (myList->pHead->pNext == myList->pTail)
    {
        freeIsOffOn(myList->pHead);
        freeIsOffOn(myList->pTail);
        freeIsOffOn(myList);
        return status;
    }
    return NOT_EMPTY;
}