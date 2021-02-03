#include "data.h"

#pragma region four
mailbox *create_mailbox(uint nMessages, uint nDataSize);
int no_messages(mailbox *mBox);

exception send_wait(mailbox *mBox, void *pData);
exception receive_wait(mailbox *mBox, void *pData);

exception send_no_wait(mailbox *mBox, void *pData);
int receive_no_wait(mailbox *mBox, void *pData);
#pragma endregion four

mailbox *create_mailbox(uint nMessages, uint nDataSize)
{
}

int no_messages(mailbox *mBox)
{
}

exception send_wait(mailbox *mBox, void *pData)
{
}

exception receive_wait(mailbox *mBox, void *pData)
{
}

exception send_no_wait(mailbox *mBox, void *pData)
{
}

int receive_no_wait(mailbox *mBox, void *pData)
{
}