#include "mallocstats.h"

struct mallinfo meminfo;
void update_meminfo()
{
    meminfo = __iar_dlmallinfo();
}