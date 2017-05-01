#ifndef COMMON_IOC_H
#define COMMON_IOC_H

#include <common/def.h>

#define IOC_CMD_MAGIC 'N'
#define IOC_CMD _IOWR(IOC_CMD_MAGIC, 0, struct cmd_params *)

#endif
