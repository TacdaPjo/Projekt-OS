#ifndef miros_h
#define miros_h
//Thread Control Block (TCB)

typedef struct
{
    void *sp; // stack pointer
    // other attributes associated witha thread
} OSThread;

typedef void (*OSThreadHandler)();

void OSThread_start(OSThread *me, OSThreadHandler threadHandler, void *stkSto, uint32_t stkSize)

#endif // !miros_h