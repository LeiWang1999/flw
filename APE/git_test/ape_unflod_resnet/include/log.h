#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>
#include "ape_dev_if.h"

#if JPEG
#define LOG_DEBUG(x, y...)  ({printf("Func:\t%-32s LINE:%-5d\t",__func__,__LINE__);printf(x,##y); })
#endif

#if RM_JPEG
#define LOG_DEBUG(x, y...)   printf(x,##y)
#endif
#endif